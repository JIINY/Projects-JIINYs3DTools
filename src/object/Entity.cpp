#include "Entity.hpp"
#include <algorithm>

#include "common/DebugLog.hpp"
using namespace std;
using namespace DirectX;


namespace Objects 
{
    Entity::Entity() : position_(0.0f, 0.0f, 0.0f), rotation_(0.0f, 0.0f, 0.0f), scale_(1.0f, 1.0f, 1.0f), isDirty_(true)
    {
        updateWorldMatrix();
    }
    Entity::~Entity() 
    {
        for (auto& child : children_) 
        {
            if (child) { child->parent_ = nullptr; }
        }
    }

    void Entity::setPosition(const Math::Vec3& pos) 
    { 
        position_ = pos; 
        markDirty();
    }

    void Entity::setRotation(const Math::Vec3& rot) 
    { 
        rotation_ = rot; 
        markDirty();
    }

    void Entity::setScale(const Math::Vec3& scale) 
    { 
        scale_ = scale; 
        markDirty();
    }

    void Entity::markDirty()
    {
        isDirty_ = true;
        for (auto& child : children_)
        {
            child->markDirty();
        }
    }

    void Entity::addChild(shared_ptr<Entity> child)
    {
        if (!child) { return; }
        if (child.get() == this) return;
        if (child->parent_ == this) return;

        //다른 부모에게서 입양
        if (child->parent_) 
        {
            child->parent_->removeChild(child.get());
        }

        child->parent_ = this;
        children_.push_back(child);
        child->markDirty();
    }

    void Entity::removeChild(Entity* child) 
    {
        if (!child) return;

        //자식 명단에서 찾기
        auto it = find_if(children_.begin(), children_.end(),
            [child](const shared_ptr<Entity>& ptr) 
            {
                return ptr.get() == child;
            });

        if (it != children_.end()) 
        {
            (*it)->parent_ = nullptr;
            (*it)->markDirty();
            children_.erase(it);
        }
    }

    void Entity::updateTransform()
    {
        if (!isDirty_) return;

        updateWorldMatrix();
        isDirty_ = false;
    }

    void Entity::updateWorldMatrix()
    {
        XMVECTOR s = XMLoadFloat3(&scale_);
        XMVECTOR r = XMLoadFloat3(&rotation_);
        XMVECTOR t = XMLoadFloat3(&position_);

        XMMATRIX matS = XMMatrixScalingFromVector(s);
        XMMATRIX matR = XMMatrixRotationRollPitchYawFromVector(r);
        XMMATRIX matT = XMMatrixTranslationFromVector(t);

        XMMATRIX local = matS * matR * matT;

        if (parent_ != nullptr)
        {
            XMMATRIX parentMat = XMLoadFloat4x4(&parent_->worldMatForShader_);
            local *= parentMat;
        }

        XMStoreFloat4x4(&worldMatForShader_, local); //셰이더에서 column-major로 전치
        XMStoreFloat4x4(&worldMatForCollider_, local);

        DebugLog("[updateWorldMat] this: %p pos: %.3f, %.3f, %.3f scale: %.3f, %.3f, %.3f", this, position_.x, position_.y, position_.z, scale_.x, scale_.y, scale_.z);
        DebugLog("[updateWorldMat] this: %p mat _11, _12, _13, _14: %.3f, %.3f, %.3f, %.3f", this, worldMatForShader_._11, worldMatForShader_._12, worldMatForShader_._13, worldMatForShader_._14);
        DebugLog("[updateWorldMat] this: %p mat _21, _22, _23, _24: %.3f, %.3f, %.3f, %.3f", this, worldMatForShader_._21, worldMatForShader_._22, worldMatForShader_._23, worldMatForShader_._24);
        DebugLog("[updateWorldMat] this: %p mat _31, _32, _33, _14: %.3f, %.3f, %.3f, %.3f", this, worldMatForShader_._31, worldMatForShader_._32, worldMatForShader_._33, worldMatForShader_._34);
        DebugLog("[updateWorldMat] this: %p mat _41, _42, _43, _44: %.3f, %.3f, %.3f, %.3f", this, worldMatForShader_._41, worldMatForShader_._42, worldMatForShader_._43, worldMatForShader_._44);
    }
}
