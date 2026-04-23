#include "SceneObject.hpp"
#include <string>
#include <tuple>
#include <cmath>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "common/Math.hpp"
#include "SceneObjectType.hpp"
#include "ColliderMesh.hpp"
#include "render/RenderObject.hpp"
using namespace std;
using namespace Math;
using namespace DirectX;


SceneObject::SceneObject(const string& name, ColliderType colType, SceneObjectType objType) 
    : Render::RenderObject(), name_(name), colliderType_(colType), objType_(objType)
{
    boundBox_.Center = Vec3(0.0f, 0.0f, 0.0f);
    boundBox_.Extents = Vec3(1.0f, 1.0f, 1.0f);
}

void SceneObject::onSelectImpl() 
{
    //선택됐을 때 시각적 처리
}

void SceneObject::onDeselectImpl() 
{
    //해제됐을 때 시각적 처리
}

bool SceneObject::intersects(const Math::Vec3& rayOrigin, const Math::Vec3& rayDir, float& outDist)
{
    if (colliderType_ == ColliderType::None || colliderType_ == ColliderType::Count) { return false; }

    Math::Mat4 worldMat = this->getWorldMatForCollider();
    XMMATRIX worldMatXM = XMLoadFloat4x4(&worldMat);

    //역행렬 계산
    XMVECTOR determinant;
    XMMATRIX invWorldMat = XMMatrixInverse(&determinant, worldMatXM);

    //Ray변환(world -> local변환)
    XMVECTOR vWorldRayOrigin = XMLoadFloat3(&rayOrigin);
    XMVECTOR vWorldRayDir = XMLoadFloat3(&rayDir);

    //시작점 이동
    XMVECTOR vLocalRayOrigin = XMVector3TransformCoord(vWorldRayOrigin, invWorldMat);

    //방향은 회전/크기만 적용되고 위치는 무시되야하기 때문에 normalize
    XMVECTOR vLocalRayDir = XMVector3TransformNormal(vWorldRayDir, invWorldMat);
    vLocalRayDir = XMVector3Normalize(vLocalRayDir);

    //1단계 Box로 검사
    float boxDist = 0.0f;
    if (!boundBox_.Intersects(vLocalRayOrigin, vLocalRayDir, boxDist)) 
    {
        return false;
    }

    //2단계 상세 검사: Box
    if (colliderType_ == ColliderType::BoundingBox) 
    {
        outDist = boxDist;
        return true;
    }
    if (colliderType_ == ColliderType::Primitive) 
    {
        if (primitive_ == Render::Primitives::PrimitiveType::Cube) 
        {
            outDist = boxDist;
            return true;
        }

        Math::Vec3 localOrigin, localDir;
        XMStoreFloat3(&localOrigin, vLocalRayOrigin);
        XMStoreFloat3(&localDir, vLocalRayDir);

        return intersectPrimitive(localOrigin, localDir, outDist);
    }

    //상세: 정밀
    if (colliderType_ == ColliderType::Mesh || colliderType_ == ColliderType::CustomCollider) 
    {
        Math::Vec3 localOrigin, localDir;
        XMStoreFloat3(&localOrigin, vLocalRayOrigin);
        XMStoreFloat3(&localDir, vLocalRayDir);

        return intersectMesh(localOrigin, localDir, outDist);
    }
    return false;
}

bool SceneObject::intersectMesh(const Math::Vec3& localOrigin, const Math::Vec3& localDir, float& outDist) 
{
    XMVECTOR vRayOrigin = XMLoadFloat3(&localOrigin);
    XMVECTOR vRayDir = XMLoadFloat3(&localDir);
    float minDist = FLT_MAX;

    bool isHit = false;
    if (colliderType_ == ColliderType::CustomCollider)
    {
        if (!colliderMesh_) { return false; }

        const auto& vertices = colliderMesh_->getVertices();
        const auto& indices = colliderMesh_->getIndices();

        for (size_t i = 0; i < indices.size(); i += 3) 
        {
            const auto [i0, i1, i2] = make_tuple(indices[i], indices[i + 1], indices[i + 2]);
            XMVECTOR v0 = XMLoadFloat3(&vertices[i0]);
            XMVECTOR v1 = XMLoadFloat3(&vertices[i1]);
            XMVECTOR v2 = XMLoadFloat3(&vertices[i2]);

            float dist = 0.0f;
            if (TriangleTests::Intersects(vRayOrigin, vRayDir, v0, v1, v2, dist))
            {
                if (dist < minDist)
                {
                    minDist = dist;
                    isHit = true;
                }
            }
        }
    }
    else //ColliderType::Mesh
    {
        auto mesh = this->getMesh();
        if (!mesh) { return false; }

        const auto& vertices = mesh->getVertices();
        const auto& indices = mesh->getIndices();

        for (size_t i = 0; i < indices.size(); i += 3) 
        {
            const auto [i0, i1, i2] = make_tuple(indices[i], indices[i + 1], indices[i + 2]);
            XMVECTOR v0 = XMLoadFloat3(&vertices[i0].position);
            XMVECTOR v1 = XMLoadFloat3(&vertices[i1].position);
            XMVECTOR v2 = XMLoadFloat3(&vertices[i2].position);

            float dist = 0.0f;
            if (TriangleTests::Intersects(vRayOrigin, vRayDir, v0, v1, v2, dist))
            {
                if (dist < minDist)
                {
                    minDist = dist;
                    isHit = true;
                }
            }
        }
    }

    if (isHit) { outDist = minDist; }
    return isHit;
}

bool SceneObject::intersectPrimitive(const Math::Vec3& localOrigin, const Math::Vec3& localDir, float& outDist) 
{
    XMVECTOR vOrigin = XMLoadFloat3(&localOrigin);
    XMVECTOR vDir = XMLoadFloat3(&localDir);
    
    XMFLOAT3 origin, dir;
    XMStoreFloat3(&origin, vOrigin);
    XMStoreFloat3(&dir, vDir);

    float minDist = FLT_MAX;
    bool isHit = false;

    float extentX = boundBox_.Extents.x; //Radius
    float extentY = boundBox_.Extents.y; //Height Half
    float extentZ = boundBox_.Extents.z; //Depth Half

    switch (primitive_) 
    {
    case Render::Primitives::PrimitiveType::Sphere: 
    {
        BoundingSphere sphere;
        sphere.Center = XMFLOAT3(0.0f, 0.0f, 0.0f);
        sphere.Radius = extentX;

        if (sphere.Intersects(vOrigin, vDir, minDist)) 
        {
            isHit = true;
        }
        break;
    }
    case Render::Primitives::PrimitiveType::Plane:
    {
        if (abs(dir.y) > 1e-6f)
        {
            float t = -origin.y / dir.y;
            if (t > 0.0f)
            {
                float hitX = origin.x + t * dir.x;
                float hitZ = origin.z + t * dir.z;

                if (abs(hitX) <= extentX && abs(hitZ) <= extentZ) 
                {
                    minDist = t;
                    isHit = true;
                }
            }
        }
        break;
    }
    case Render::Primitives::PrimitiveType::Cylinder:
    {
        float r = extentX;
        float A = dir.x * dir.x + dir.z * dir.z;
        float B = 2.0f * (origin.x * dir.x + origin.z * dir.z);
        float C = (origin.x * origin.x + origin.z * origin.z) - (r * r);

        float det = B * B - 4.0f * A * C;

        if (det >= 0.0f && A > 1e-6f) 
        {
            float sqrtDet = sqrtf(det);
            float t1 = (-B - sqrtDet) / (2.0 * A);

            if (t1 > 0.0f) 
            {
                float hitY = origin.y + t1 * dir.y;
                if (abs(hitY) <= extentY) 
                {
                    minDist = t1;
                    isHit = true;
                }
            }
        }

        //뚜껑
        if (abs(dir.y) > 1e-6f) 
        {
            float top = (extentY - origin.y) / dir.y;
            float bot = (-extentY - origin.y) / dir.y;

            auto checkCap = [&](float t)
                {
                    if (t > 0.0f && t < minDist)
                    {
                        float hx = origin.x + t * dir.x;
                        float hz = origin.z + t * dir.z;
                        if (hx * hx + hz * hz <= r * r)
                        {
                            minDist = t;
                            isHit = true;
                        }
                    }
                };

            checkCap(top);
            checkCap(bot);
        }
        break;
    }
    case Render::Primitives::PrimitiveType::Capsule:
    {
        float r = extentX;
        float H = extentY;
        float h = max(0.0f, H - r);

        //원기둥
        if (h > 0.0f) 
        {
            float A = dir.x * dir.x + dir.z * dir.z;
            float B = 2.0f * (origin.x * dir.x + origin.z * dir.z);
            float C = (origin.x * origin.x + origin.z * origin.z) - (r * r);
            float det = B * B - 4.0f * A * C;

            if (det >= 0.0f && A > 1e-6f) 
            {
                float t = (-B - sqrtf(det)) / (2.0f * A);
                if (t > 0.0f) 
                {
                    float hitY = origin.y + t * dir.y;
                    if (abs(hitY) <= h) 
                    {
                        minDist = t;
                        isHit = true;
                    }
                }
            }
        }

        //윗 반구
        BoundingSphere topHemi;
        topHemi.Center = XMFLOAT3(0.0f, h, 0.0f);
        topHemi.Radius = r;
        float top = FLT_MAX;
        if (topHemi.Intersects(vOrigin, vDir, top)) 
        {
            if (top < minDist) 
            { 
                minDist = top;
                isHit = true;
            }
        }

        //아랫 반구
        BoundingSphere botHemi;
        botHemi.Center = XMFLOAT3(0.0f, -h, 0.0f);
        botHemi.Radius = r;
        float bot = FLT_MAX;
        if (botHemi.Intersects(vOrigin, vDir, bot)) 
        {
            if (bot < minDist) 
            {
                minDist = bot;
                isHit = true;
            }
        }
        break;
    }
    default:
        outDist = 0.0f; return true;
    }

    if (isHit)
    {
        outDist = minDist;
        return true;
    }
    return false;
}
