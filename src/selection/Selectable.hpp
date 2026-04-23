#pragma once
#include "ISelectable.hpp"
#include "common/Math.hpp"
#include "DirectXMath.h"


namespace Selection 
{
    class Selectable : public ISelectable 
    {
    public:
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
        virtual bool intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) = 0;


    protected:
        virtual void onSelectImpl() {}
        virtual void onDeselectImpl() {}


    private:
        bool isSelected_ = false;
    };
}
