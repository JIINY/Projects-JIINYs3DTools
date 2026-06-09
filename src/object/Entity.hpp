#pragma once
#include <cstdint>
#include <DirectXMath.h>
#include <memory>
#include <vector>
#include "../common/Math.hpp"


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

        void setID(uint64_t id) { id_ = id; };
        const uint64_t& getID() const { return id_; }

        const Math::Mat4& getWorldMatForShader() const { return worldMatForShader_; }
        const Math::Mat4& getWorldMatForCollider() const { return worldMatForCollider_; }

        void addChild(std::shared_ptr<Entity> child);
        void removeChild(Entity* child);
        Entity* getParent() const { return parent_; }
        const std::vector<std::shared_ptr<Entity>>& getChildren() const { return children_; }

        void updateTransform();


    protected:
        uint64_t id_ = 0;
        Math::Vec3 position_ = {};
        Math::Vec3 rotation_ = {};
        Math::Vec3 scale_ = {};
        Math::Mat4 worldMatForShader_ = {};
        Math::Mat4 worldMatForCollider_ = {};

        Entity* parent_ = nullptr;
        std::vector<std::shared_ptr<Entity>> children_;
        bool isDirty_ = true;

        //TO_DO: 부모-자식 실제 도입시, 리스트에 등록할 단위(루트만 vs 부모/자식 등록+정렬) 결정 필요
        //TO_DO: 부모-자식 도입시 저장/로드에도 반영
        void markDirty();
        void updateWorldMatrix();
    };
}
