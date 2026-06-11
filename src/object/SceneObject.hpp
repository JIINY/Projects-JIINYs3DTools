#pragma once
#include <string>
#include <memory>
#include <DirectXCollision.h>
#include "common/Math.hpp"
#include "SceneObjectType.hpp"
#include "ColliderMesh.hpp"
#include "render/RenderObject.hpp"
#include "render/primitives/PrimitivesType.hpp"
#include "selection/Selectable.hpp"

enum class ColliderType
{
    None,
    BoundingBox,
    Mesh,
    CustomCollider,
    Primitive,
    Count
};


class SceneObject : public Render::RenderObject, public Selection::Selectable 
{
public:
    SceneObject(const std::string& name, ColliderType colType, SceneObjectType objType);
    virtual ~SceneObject() = default;

    void setBoundingBox(const DirectX::BoundingBox& box) { boundBox_ = box; }
    const DirectX::BoundingBox& getBoundingBox() const { return boundBox_; }
    const ColliderType& getColliderType() const { return colliderType_; }
    void setColliderMesh(std::shared_ptr<ColliderMesh> mesh) { colliderMesh_ = mesh; }

    void setName(const std::string& name) { name_ = name; }
    virtual std::string getName() const override { return name_; }
    virtual bool intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist) override;

    void setObjectType(SceneObjectType newType) { objType_ = newType; }
    SceneObjectType getObjectType() const { return objType_; }
    void setPrimitiveType(Render::Primitives::PrimitiveType type) { primitive_ = type; }
    const Render::Primitives::PrimitiveType& getPrimitiveType() const { return primitive_; }


private:
    std::string name_ = "";
    Render::Primitives::PrimitiveType primitive_ = Render::Primitives::PrimitiveType::Count;

    SceneObjectType objType_ = SceneObjectType::Count;
    ColliderType colliderType_ = ColliderType::Count;
    DirectX::BoundingBox boundBox_;
    std::shared_ptr<ColliderMesh> colliderMesh_ = nullptr; //TO_DO: CustomCollider도입시 저장에도 추가

    bool intersectMesh(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist);
    bool intersectPrimitive(const Math::Vec3& localOrigin, const Math::Vec3& localDir, float& outDist);
};
