#include "SceneSerializer.hpp"
#include <fstream>
#include <filesystem>
#include <cassert>
#include <Windows.h>
#include "command/CommandStack.hpp"
#include "core/coordinator/scene/PassiveObjectCoordinator.hpp"
#include "core/manager/resources/MaterialManager.hpp"
#include "core/manager/scene/SceneObjectManager.hpp"
#include "core/PathConfig.hpp"
#include "object/SceneObject.hpp"
#include "render/primitives/PrimitivesType.hpp"
#include "ui/window/environmentConfig/EnvironmentConfig.hpp"
#include "utils/FontUtils.hpp"
#include "../lib/nlohmann-json/json.hpp"

#include "event/appEvent/AppEventPublisher.hpp"
#include "event/appEvent/ui/EnvironmentDataRequestedEvent.hpp"

#include "common/DebugLog.hpp"
using namespace std;
using json = nlohmann::json;


namespace Scene 
{
    bool SceneSerializer::initialize(SceneContext context)
    {
        assert(context.envConfig && "초기화 실패");
        assert(context.sceneObjManager && "초기화 실패");
        assert(context.matManager && "초기화 실패");
        
        envConfig_ = context.envConfig;
        sceneObjectManager_ = context.sceneObjManager;
        matManager_ = context.matManager;
        passiveObjCoord_ = sceneObjectManager_->getPassiveCoordinator();

        if (!envConfig_ || !sceneObjectManager_ || !matManager_ || !passiveObjCoord_) { return false; }

        return true;
    }

    bool SceneSerializer::serialize(const std::string& filepath) 
    {
        json root;

        root["FileType"] = "Scene";

        json environment;
        environment["envConfig"]["fileName"] = envConfig_->getCurrentEnvConfigFileName();
        filesystem::path abs = envConfig_->getCurrentEnvConfigFilePath();
        filesystem::path rel = filesystem::relative(abs, Path::assetPath());
        environment["envConfig"]["filePath"] = rel.string();

        root["environment"] = environment;


        json objectList = json::array();
        const auto& sceneObjects = sceneObjectManager_->getAllSceneObjects();

        for (const auto& o : sceneObjects)
        {
            json object;
            object["id"] = std::to_string(o->getID());
            object["name"] = o->getName();

            object["objectType"] = static_cast<int>(o->getObjectType());
            object["primitiveType"] = static_cast<int>(o->getPrimitiveType());
            object["colliderType"] = static_cast<int>(o->getColliderType());

            const auto& pos = o->getPosition();
            const auto& rot = o->getRotation();
            const auto& scale = o->getScale();
            object["position"] = { Math::roundFloat(pos.x), Math::roundFloat(pos.y), Math::roundFloat(pos.z) };
            object["rotation"] = { Math::roundFloat(rot.x), Math::roundFloat(rot.y), Math::roundFloat(rot.z) };
            object["scale"] = { Math::roundFloat(scale.x), Math::roundFloat(scale.y), Math::roundFloat(scale.z) };

            object["isVisible"] = o->isVisible();
            object["castShadow"] = o->isCastShadow();
            object["receiveShadow"] = o->isReceiveShadow();
            object["isCullable"] = o->isCullable();
            object["layer"] = o->getLayer();
            object["isDynamic"] = o->isDynamic();

            json materialList = json::array();
            for (size_t i = 0; i < o->getMaterialCount(); ++i)
            {
                auto m = o->getMaterial(i);
                if (!m) { continue; }

                json material;
                material["presetName"] = convertWstrToStr(m->getPreset());

                json property;
                for (const auto& [name, pData] : m->getUIPropertyMap())
                {
                    switch (pData.data)
                    {
                    case UI::WidgetData::Float:
                    {
                        property[name] = Math::roundFloat(m->getProperty<float>(name));
                        break;
                    }
                    case UI::WidgetData::Float2:
                    {
                        auto v = m->getProperty<Math::Vec2>(name);
                        property[name] = { Math::roundFloat(v.x), Math::roundFloat(v.y) };
                        break;
                    }
                    case UI::WidgetData::Float3:
                    {
                        auto v = m->getProperty<Math::Vec3>(name);
                        property[name] = { Math::roundFloat(v.x), Math::roundFloat(v.y), Math::roundFloat(v.z) };
                        break;
                    }
                    case UI::WidgetData::Float4:
                    {
                        auto v = m->getProperty<Math::Vec4>(name);
                        property[name] = { Math::roundFloat(v.x), Math::roundFloat(v.y), Math::roundFloat(v.z), Math::roundFloat(v.w) };
                        break;
                    }
                    case UI::WidgetData::Int:
                    {
                        property[name] = m->getProperty<int>(name);
                        break;
                    }
                    case UI::WidgetData::Int2:
                    {
                        auto v = m->getProperty<Math::Int2>(name);
                        property[name] = { v.x, v.y };
                        break;
                    }
                    default:
                        break;
                    }
                }

                material["properties"] = property;
                materialList.push_back(material);
            }

            object["materials"] = materialList;
            objectList.push_back(object);
        }
        
        root["objectList"] = objectList;

        ofstream file(filepath);
        if (!file.is_open()) { return false; }

        file << root.dump(4);
        return true;
    }

    bool SceneSerializer::deserialize(const string& filepath, string& errorMsg) 
    {
        ifstream file(filepath);
        if (!file.is_open()) { return false; }

        json root;
        try { root = json::parse(file); }
        catch (...) { return false; }

        if (!root.contains("FileType") || root["FileType"] != "Scene")
        {
            return false;
        }

        sceneObjectManager_->removeAllObjects();
        CommandStack::get().clear();

        if (root.contains("environment")) { loadEnvConfig(root["environment"], errorMsg); }
        if (root.contains("objectList"))
        {
            for (const auto& objJson : root["objectList"])
            {
                auto o = loadObject(objJson, errorMsg);
                if (!o) { continue; }

                if (objJson.contains("materials"))
                {
                    const auto& mats = objJson["materials"];
                    for (size_t i = 0; i < mats.size(); ++i)
                    {
                        loadMaterial(mats[i], o, i, errorMsg);
                    }
                }
            }
        }
        return true;
    }

    void SceneSerializer::loadEnvConfig(const json& envJson, string& errorMsg)
    {
        if (!envJson.contains("envConfig")) { return; }
        const auto& env = envJson["envConfig"];

        string filePath = Path::assetPath() + env.value("filePath", "");
        string fileName = env.value("fileName", "");
        if (filePath.empty() && fileName.empty())
        {
            AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::New, "" });
            return;
        }

        if (!filePath.empty() && filesystem::exists(filePath))
        {
            AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::Load, filePath });
        }
        else
        {
            AppEventPublisher::get().publish(EnvironmentDataRequestedEvent{ EnvActionType::New, ""});
            errorMsg += "EnvironmentConfig 로드 실패\n";
        }
    }

    shared_ptr<SceneObject> SceneSerializer::loadObject(const json& objJson, string& errorMsg)
    {
        if (!objJson.contains("primitiveType")) { return nullptr; }

        Render::Primitives::PrimitiveData data;
        data = Render::Primitives::setDefaultData(static_cast<Render::Primitives::PrimitiveType>(objJson["primitiveType"].get<int>()));

        auto o = passiveObjCoord_->createPrimitive(data);
        string name = objJson.value("name", "Unknown");
        if (!o)
        {
            errorMsg += name + " 오브젝트 로드 실패\n";
            return nullptr;
        }

        o->setName(name);
        if (objJson.contains("id"))
        {
            o->setID(stoull(objJson["id"].get<string>()));
        }

        if (objJson.contains("position"))
        {
            const auto& p = objJson["position"];
            o->setPosition({ p[0], p[1], p[2] });
        }

        if (objJson.contains("rotation"))
        {
            const auto& r = objJson["rotation"];
            o->setRotation({ r[0], r[1], r[2] });
        }

        if (objJson.contains("scale"))
        {
            const auto& s = objJson["scale"];
            o->setScale({ s[0], s[1], s[2] });
        }

        o->setVisible(objJson.value("isVisible", true));
        o->setCastShadow(objJson.value("castShadow", true));
        o->setReceiveShadow(objJson.value("receiveShadow", true));
        o->setCullable(objJson.value("isCullable", true));
        o->setLayer(objJson.value("layer", 1u));
        o->setDynamic(objJson.value("isDynamic", false));

        return o;
    }

    void SceneSerializer::loadMaterial(const json& matJson, shared_ptr<SceneObject>obj, size_t index, string& errorMsg)
    {
        if (!matJson.contains("presetName")) { return; }

        wstring presetName = convertStrToWstr(matJson["presetName"].get<string>());

        auto m = matManager_->createMaterial(presetName);
        if (!m)
        {
            errorMsg += obj->getName() + "매티리얼 로드 실패\n";
            return;
        }

        if (matJson.contains("properties"))
        {
            const auto& props = matJson["properties"];
            for (const auto& [name, propData] : m->getUIPropertyMap())
            {
                if (!props.contains(name)) { continue; }

                switch (propData.data)
                {
                case UI::WidgetData::Float:
                {
                    m->setProperty<float>(name, props[name].get<float>());
                    break;
                }
                case UI::WidgetData::Float2:
                {
                    const auto& v = props[name];
                    m->setProperty<Math::Vec2>(name, { v[0], v[1] });
                    break;
                }
                case UI::WidgetData::Float3:
                {
                    const auto& v = props[name];
                    m->setProperty<Math::Vec3>(name, { v[0], v[1], v[2] });
                    break;
                }
                case UI::WidgetData::Float4:
                {
                    const auto& v = props[name];
                    m->setProperty<Math::Vec4>(name, { v[0], v[1], v[2], v[3] });
                    break;
                }
                case UI::WidgetData::Int:
                {
                    m->setProperty<int>(name, props[name].get<int>());
                    break;
                }
                case UI::WidgetData::Int2:
                {
                    const auto& v = props[name];
                    m->setProperty<Math::Int2>(name, { v[0], v[1] });
                    break;
                }
                default:
                    break;
                }
            }
        }

        obj->setMaterial(index, m);
    }
}
