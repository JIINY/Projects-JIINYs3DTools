#include "MaterialContainer.hpp"
#include "utils/FontUtils.hpp"
#include "imgui.h"
using namespace std;


namespace MaterialEditor
{
    bool MaterialContainer::initialize()
    {
        return true;
    }

    void MaterialContainer::draw()
    {
        ImGui::Text(utf8("임시) 오브젝트 1개를 선택한 경우만 편집 가능"));
    }
}
