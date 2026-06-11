#pragma once
#include "ISelectable.hpp"
#include "common/Math.hpp"
#include "DirectXMath.h"

#include "common/DebugLog.hpp"


namespace Selection 
{
    class Selectable : public ISelectable 
    {
    public:
        explicit Selectable(SelectableType type) : selectableType_(type) {}
        ~Selectable() = default;

        virtual bool isSelected() const override { return isSelected_; }

        virtual void onSelected() override 
        {
            if (isSelected_) { return; }
            isSelected_ = true;
            onSelectImpl();
        }
        virtual void onDeselected() override 
        {
            if (!isSelected_) { return; }
            isSelected_ = false;
            onDeselectImpl();
        }
        
        SelectableType getSelectableType() const override { return selectableType_; }
        virtual bool intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) = 0;


    protected:
        virtual void onSelectImpl() {}
        virtual void onDeselectImpl() {}


    private:
        SelectableType selectableType_ = SelectableType::Count;
        bool isSelected_ = false;
    };
}
