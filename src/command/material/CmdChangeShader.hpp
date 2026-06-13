#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "common/Math.hpp"
#include "core/manager/resources/MaterialManager.hpp"
#include "command/ICommand.hpp"
#include "render/Material.hpp"
#include "ui/WidgetData.hpp"
#include "object/SceneObject.hpp"


class CmdChangeShader : public ICommand
{
public:
    CmdChangeShader(MaterialManager* matManager, std::shared_ptr<SceneObject> targetObj, size_t matIndex,
        std::wstring beforePreset, std::unordered_map<std::string, Render::MaterialPropData> beforePropMap,
        std::unordered_map<std::string, Render::UIPropData> beforeUIPropMap, std::vector<uint8_t> beforeBufferData, std::wstring afterPreset) 
        : matManager_(matManager), targetObj_(targetObj), matIndex_(matIndex), beforePreset_(std::move(beforePreset)), beforePropMap_(std::move(beforePropMap)), 
          beforeUIPropMap_(std::move(beforeUIPropMap)), beforeBufferData_(std::move(beforeBufferData)), afterPreset_(std::move(afterPreset)) {}
    ~CmdChangeShader() override = default;

    void execute() override
    {
        auto newMat = matManager_->createMaterial(afterPreset_);
        copyProperties(newMat, beforePropMap_, beforeBufferData_, beforeUIPropMap_);
        targetObj_->setMaterial(matIndex_, newMat); //TO_DO: 멀티 매티리얼시 대응 필요
    }

    void undo() override
    {
        auto restoreMat = matManager_->createMaterial(beforePreset_);
        copyProperties(restoreMat, beforePropMap_, beforeBufferData_, beforeUIPropMap_);
        targetObj_->setMaterial(matIndex_, restoreMat);
    }

    bool emitsDataChanged() const override { return true; }


private:
    std::shared_ptr<SceneObject> targetObj_;
    size_t matIndex_ = 0;
    std::wstring beforePreset_;
    std::unordered_map<std::string, Render::MaterialPropData> beforePropMap_;
    std::unordered_map<std::string, Render::UIPropData> beforeUIPropMap_;
    std::vector<uint8_t> beforeBufferData_;
    std::wstring afterPreset_;
    MaterialManager* matManager_ = nullptr;

    void copyProperties(std::shared_ptr<Render::Material> mat, const std::unordered_map<std::string, Render::MaterialPropData>& propMap, 
        const std::vector<uint8_t>& bufferData, const std::unordered_map<std::string, Render::UIPropData>& uiPropMap)
    {
        for (const auto& [name, prop] : propMap)
        {
            const uint8_t* src = bufferData.data() + prop.offset;

            auto uiP = uiPropMap.find(name);
            if (uiP == uiPropMap.end()) { continue; }

            switch (uiP->second.data)
            {
            case UI::WidgetData::Float:
            {
                float v;
                memcpy(&v, src, sizeof(v));
                mat->setProperty(name, v);
                break;
            }
            case UI::WidgetData::Float3:
            {
                Math::Vec3 v;
                memcpy(&v, src, sizeof(v));
                mat->setProperty(name, v);
                break;
            }
            case UI::WidgetData::Float4:
            {
                Math::Vec4 v;
                memcpy(&v, src, sizeof(v));
                mat->setProperty(name, v);
                break;
            }
            default:
                break;
            }
        }
    }
};
