#pragma once
#include <string>
#include "common/Math.hpp"
#include "SelectableType.hpp"


namespace Selection 
{
    class ISelectable
    {
    public:
        virtual ~ISelectable() = default;

        virtual std::string getName() const = 0;
        virtual SelectableType getSelectableType() const = 0;
        virtual bool isSelected() const = 0;


        //선택 상태가 변할 때 시각적 처리 등
        virtual void onSelected() {}
        virtual void onDeselected() {}
        virtual bool intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) = 0;

    };
}
