#include "MaterialContainer.hpp"
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
        ImGui::Text(utf8("현재는 낱개 오브젝트를 선택한 경우만 편집 가능"));
    }
}
