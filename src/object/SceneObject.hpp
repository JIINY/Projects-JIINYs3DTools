#pragma once
#include <string>
#include <memory>
#include <DirectXCollision.h>
#include "common/Math.hpp"
#include "SceneObjectType.hpp"
#include "ColliderObject.hpp"
#include "render/RenderObject.hpp"
#include "render/primitives/PrimitivesType.hpp"
#include "selection/Selectable.hpp"

enum class ColliderType
{
    None,
    BoundingBox,
    Mesh,
    Primitive,
    Count
};


class SceneObject : public Render::RenderObject, public Selection::Selectable 
{
public:
    SceneObject(const std::string& name, ColliderType colType, SceneObjectType objType);
    virtual ~SceneObject() = default;

    void setName(const std::string& name) { name_ = name; }
    virtual std::string getName() const override { return name_; }
    void setObjectType(SceneObjectType newType) { objType_ = newType; }
    SceneObjectType getObjectType() const { return objType_; }

    virtual bool intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) override;
    void setBoundingBox(const DirectX::BoundingBox& box) { boundBox_ = box; }
    const DirectX::BoundingBox& getBoundingBox() const { return boundBox_; }
    void setColliderType(const ColliderType& type) { colliderType_ = type; }
    const ColliderType& getColliderType() const { return colliderType_; }
    void setPrimitiveType(Render::Primitives::PrimitiveType type) { primitive_ = type; }
    const Render::Primitives::PrimitiveType& getPrimitiveType() const { return primitive_; }
    void setColliderObject(std::shared_ptr<ColliderObject> obj) { colliderObj_ = obj; }
    std::shared_ptr<ColliderObject> getColliderObject() const { return colliderObj_; }


private:
    std::string name_ = "";
    Render::Primitives::PrimitiveType primitive_ = Render::Primitives::PrimitiveType::Count;

    SceneObjectType objType_ = SceneObjectType::Count;
    ColliderType colliderType_ = ColliderType::Count;
    std::shared_ptr<ColliderObject> colliderObj_ = nullptr;
    DirectX::BoundingBox boundBox_;

    bool intersectMesh(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist);
    bool intersectPrimitive(const Math::Vec3& localOrigin, const Math::Vec3& localDir, float& outDist);
};
