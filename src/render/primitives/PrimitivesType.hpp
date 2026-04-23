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
}
