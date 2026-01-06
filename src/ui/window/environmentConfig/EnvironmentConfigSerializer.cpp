#include "EnvironmentConfigSerializer.hpp"
#include <fstream>
#include <cassert>
#include "core/manager/scene/LightManager.hpp"
using namespace std;
using json = nlohmann::json;


namespace EnvConfig 
{
    void EnvironmentConfigSerializer::initialize(EnvConfigContext context)
    {
        lightManager_ = context.lightManager;
    }

    void EnvironmentConfigSerializer::serialize(const std::string& filepath)
    {
        if (!lightManager_)
        {
            assert(0 && "[에러] 저장: lightManager_가 비었습니다.");
            return;
        }

        json savefile;

        shared_ptr<Render::DirectionLight> sun = lightManager_->getDirectionalLight();
        Math::Vec3 sunDir = sun->getDirection();
        Math::Vec3 sunColor = sun->getColor();

        savefile["SunLight"] =
        {
            { "SunDirection", {sunDir.x, sunDir.y, sunDir.z}},
            { "SunColor", {sunColor.x, sunColor.y, sunColor.z}},
            { "SunIntensity", sun->getIntensity()}
        };

        auto [top, mid, bot] = lightManager_->getAmbientColors();
        savefile["AmbientLight"] =
        {
            {"Sky", {top.x, top.y, top.z}},
            {"Horizon", {mid.x, mid.y, mid.z}},
            {"Ground", {bot.x, bot.y, bot.z}}
        };


        //파일 저장
        ofstream fout(filepath);
        if (fout.is_open())
        {
            fout << savefile.dump(4); //들여쓰기 4칸 지정
            fout.close();
        }
        else
        {
            assert(0 && "[에러] Scene파일 저장 실패\n");
        }
    }

    bool EnvironmentConfigSerializer::deserialize(const std::string& filepath)
    {
        if (!lightManager_)
        {
            assert(0 && "[에러] 로드: lightManager_가 비었습니다.\n");
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
            assert(0 && "[에러] 로드: JSON 파싱\n");
            return false;
        }
        fin.close();

        if (loadfile.contains("SunLight"))
        {
            auto& sun = loadfile["SunLight"];

            if (sun.contains("Direction"))
            {
                auto& dir = sun["Direction"];
                lightManager_->setDirectionalLightDir({ dir[0], dir[1], dir[2] });
            }
            if (sun.contains("Color"))
            {
                auto& color = sun["Color"];
                lightManager_->setDirectionalLightColor({ color[0], color[1], color[2] });
            }
            if (sun.contains("Intensity"))
            {
                lightManager_->setDirectionalLightIntensity(sun["Intensity"]);
            }
        }

        if (loadfile.contains("AmbientLight"))
        {
            auto& amb = loadfile["AmbientLight"];

            if (amb.contains("Sky"))
            {
                auto& c = amb["Sky"];
                lightManager_->setAmbientTop({ c[0], c[1], c[2] });
            }
            if (amb.contains("Horizon"))
            {
                auto& c = amb["Horizon"];
                lightManager_->setAmbientMid({ c[0], c[1], c[2] });
            }
            if (amb.contains("Ground"))
            {
                auto& c = amb["Ground"];
                lightManager_->setAmbientBot({ c[0], c[1], c[2] });
            }
        }

        return true;
    }
}
