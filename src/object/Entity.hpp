#pragma once
#include "../common/Math.hpp"
#include <DirectXMath.h>
#include <memory>
#include <vector>


namespace Objects
{
    class Entity 
    {
    public:
        Entity();
        virtual ~Entity();

        void setPosition(const Math::Vec3& pos);
        void setRotation(const Math::Vec3& rot);
        void setScale(const Math::Vec3& scale);

        const Math::Vec3& getPosition() const { return position_; }
        const Math::Vec3& getRotation() const { return rotation_; }
        const Math::Vec3& getScale() const { return scale_; }

        const Math::Mat4& getWorldMatrix() const { return worldMatrix_; }

        void addChild(std::shared_ptr<Entity> child);
        void removeChild(Entity* child);
        Entity* getParent() const { return parent_; }
        const std::vector<std::shared_ptr<Entity>>& getChildren() const { return children_; }
        void updateTransform();


    protected:
        Math::Vec3 position_;
        Math::Vec3 rotation_;
        Math::Vec3 scale_;
        Math::Mat4 worldMatrix_;

        Entity* parent_ = nullptr;
        std::vector<std::shared_ptr<Entity>> children_;

        void updateWorldMatrix();
    };
}
