#include "Entity.hpp"
#include <algorithm>
using namespace std;
using namespace DirectX;


namespace Objects 
{
    Entity::Entity() : position_(0.0f, 0.0f, 0.0f), rotation_(0.0f, 0.0f, 0.0f), scale_(1.0f, 1.0f, 1.0f)
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
        updateWorldMatrix(); 
    }

    void Entity::setRotation(const Math::Vec3& rot) 
    { 
        rotation_ = rot; 
        updateWorldMatrix(); 
    }

    void Entity::setScale(const Math::Vec3& scale) 
    { 
        scale_ = scale; 
        updateWorldMatrix(); 
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

        child->updateTransform();
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
            (*it)->updateTransform();
            children_.erase(it);
        }
    }

    void Entity::updateTransform()
    {
        updateWorldMatrix();
        for (auto& child : children_)
        {
            child->updateTransform();
        }
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
            XMMATRIX parentMat = XMLoadFloat4x4(&parent_->worldMatrix_);
            local *= parentMat;
        }
        
        XMStoreFloat4x4(&worldMatrix_, local);
    }
}
