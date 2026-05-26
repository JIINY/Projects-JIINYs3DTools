#include "MaterialContainer.hpp"
#include <string>
#include "common/Fonts.hpp"
#include "utils/FontUtils.hpp"
#include "imgui.h"
using namespace std;


namespace Material
{
    bool MaterialContainer::MaterialContainer::initialize()
    {
        return true;
    }

    void MaterialContainer::MaterialContainer::draw()
    {
        ImGui::Text(utf8("선택한 오브젝트가 없거나, 여러 개입니다.\n현재는 개별 오브젝트일 때만 동작"));
    }
}
