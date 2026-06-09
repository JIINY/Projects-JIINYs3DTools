#pragma once


namespace Render::Primitives
{
    enum class PrimitiveType
    {
        Cube,
        Plane,
        Sphere,
        Cylinder,
        Capsule,
        Torus,
        Count
    };

    struct PrimitiveData 
    {
        PrimitiveType type = PrimitiveType::Cube;

        float width = 1.0f;
        float height = 1.0f;
        float heightHemi = 0.25f;
        float depth = 1.0f;
        float radius = 0.5f;
        float radiusTop = 0.5f;
        float radiusBot = 0.5f;
        float thickness = 0.1f;

        int segmentsX = 16;
        int segmentsY = 1;
        int segmentsZ = 1;
        int segmentsHemi = 3;
    };

    inline PrimitiveData setDefaultData(PrimitiveType type)
    {
        PrimitiveData data = {};
        data.type = type;

        switch (type)
        {
        case PrimitiveType::Cube:
        {
            data.width = 1.0f; data.depth = 1.0f; data.height = 1.0f;
            data.segmentsX = 1; data.segmentsY = 1; data.segmentsZ = 1;
            return data;
        }
        case PrimitiveType::Cylinder:
        {
            data.radiusTop = 0.5f; data.radiusBot = 0.5f; data.height = 2.0f;
            data.segmentsX = 8; data.segmentsY = 1;
            return data;
        }
        case PrimitiveType::Plane:
        {
            data.width = 5.0f; data.depth = 5.0f;
            data.segmentsX = 1; data.segmentsY = 1;
            return data;
        }
        case PrimitiveType::Sphere:
        {
            data.radius = 1.0f; data.segmentsX = 16; data.segmentsY = 8;
            return data;
        }
        case PrimitiveType::Capsule:
        {
            data.radius = 0.5f; data.height = 2.0f; data.heightHemi = 0.5f;
            data.segmentsX = 8; data.segmentsY = 1; data.segmentsHemi = 3;
            return data;
        }
        default:
            return data;
        }
    }
}
