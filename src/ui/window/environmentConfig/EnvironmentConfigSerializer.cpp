#include "EnvironmentConfigSerializer.hpp"
#include <fstream>
#include <cassert>
#include "common/Math.hpp"
#include "core/manager/scene/LightManager.hpp"
#include "render/lights/DirectionalLight.hpp"
#include "../lib/nlohmann-json/json.hpp"
using namespace std;
using json = nlohmann::json;


namespace EnvConfig 
{
    void EnvironmentConfigSerializer::initialize(EnvConfigContext context)
    {
        lightManager_ = context.lightManager;
    }

    bool EnvironmentConfigSerializer::serialize(const std::string& filepath)
    {
        if (!lightManager_)
        {
            assert(0 && "저장: lightManager_가 비었습니다.");
            return false;
        }

        json savefile;

        Render::DirectionalLight* sun = lightManager_->getSun();
        Math::Vec3 sunDir = sun->getDirection();
        Math::Vec3 sunColor = sun->getColor();

        savefile["FileType"] = "EnvironmentConfig";

        savefile["SunLight"] =
        {
            { "SunDirection", {
                Math::roundFloat(sunDir.x, 3),
                Math::roundFloat(sunDir.y, 3),
                Math::roundFloat(sunDir.z, 3)}},
            { "SunColor", {
                Math::floatToColor255(sunColor.x), 
                Math::floatToColor255(sunColor.y),
                Math::floatToColor255(sunColor.z)}},
            { "SunIntensity", Math::roundFloat(sun->getIntensity(), 3)}
        };

        auto [top, mid, bot] = lightManager_->getAmbientColors();
        savefile["AmbientLight"] =
        {
            {"Sky", {
                Math::floatToColor255(top.x),
                Math::floatToColor255(top.y),
                Math::floatToColor255(top.z)}},
            {"Horizon", {
                Math::floatToColor255(mid.x),
                Math::floatToColor255(mid.y),
                Math::floatToColor255(mid.z)}},
            {"Ground", {
                Math::floatToColor255(bot.x),
                Math::floatToColor255(bot.y),
                Math::floatToColor255(bot.z)}}
        };


        //파일 저장
        ofstream fout(filepath);
        if (!fout.is_open())
        {
            assert(0 && "EnvironmentConfig파일 열기 실패\n");
            return false;
        }

        fout << savefile.dump(4); //들여쓰기 4칸 지정

        if (fout.fail()) 
        {
            assert(0 && "EnvironmentConfig파일 쓰기 실패\n");
            return false;
        }

        fout.close();
        return true;
    }

    bool EnvironmentConfigSerializer::deserialize(const std::string& filepath)
    {
        if (!lightManager_)
        {
            assert(0 && "로드: lightManager_가 비었습니다.\n");
            return false;
        }

        ifstream fin(filepath);
        if (!fin.is_open())
        {
            //새 시작
            return false;
        }

        json loadfile;
        try
        {
            fin >> loadfile;
        }
        catch (json::parse_error& e)
        {
            assert(0 && "로드: JSON 파싱\n");
            return false;
        }
        fin.close();

        if (!loadfile.contains("FileType") || loadfile["FileType"] != "EnvironmentConfig")
        {
            return false;
        }

        if (loadfile.contains("SunLight"))
        {
            auto& sun = loadfile["SunLight"];

            if (sun.contains("SunDirection"))
            {
                auto& dir = sun["SunDirection"];
                lightManager_->setSunDir({ dir[0], dir[1], dir[2] });
            }
            if (sun.contains("SunColor"))
            {
                auto& color = sun["SunColor"];
                float r = Math::color255ToFloat(color[0]);
                float g = Math::color255ToFloat(color[1]);
                float b = Math::color255ToFloat(color[2]);
                lightManager_->setSunColor({ r, g, b });
            }
            if (sun.contains("SunIntensity"))
            {
                lightManager_->setSunIntensity(sun["SunIntensity"]);
            }
        }

        if (loadfile.contains("AmbientLight"))
        {
            auto& amb = loadfile["AmbientLight"];

            if (amb.contains("Sky"))
            {
                auto& color = amb["Sky"];
                float r = Math::color255ToFloat(color[0]);
                float g = Math::color255ToFloat(color[1]);
                float b = Math::color255ToFloat(color[2]);
                lightManager_->setAmbientTop({ r, g, b });
            }
            if (amb.contains("Horizon"))
            {
                auto& color = amb["Horizon"];
                float r = Math::color255ToFloat(color[0]);
                float g = Math::color255ToFloat(color[1]);
                float b = Math::color255ToFloat(color[2]);
                lightManager_->setAmbientMid({ r, g, b });
            }
            if (amb.contains("Ground"))
            {
                auto& color = amb["Ground"];
                float r = Math::color255ToFloat(color[0]);
                float g = Math::color255ToFloat(color[1]);
                float b = Math::color255ToFloat(color[2]);
                lightManager_->setAmbientBot({ r, g, b });
            }
        }

        return true;
    }
}
