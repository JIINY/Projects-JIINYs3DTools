#include "Cube.hpp"
#include <vector>
using namespace std;
using namespace Math;

namespace Render 
{
    namespace Primitives 
    {
        Cube::Cube(float size) { build(size); }
        Cube::~Cube() {}

        void Cube::build(float size) 
        {
            float w = size * 0.5f;
            vertices_ = {
                {Vec3(-w, -w, -w), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(-w, w, -w), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, w, -w), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, -w, -w), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},

                {Vec3(-w, -w, w), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, -w, w), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, w, w), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(-w, w, w), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},

                {Vec3(-w, w, -w), Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(-w, w, w), Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, w, w), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, w, -w), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},

                {Vec3(-w, -w, -w), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, -w, -w), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, -w, w), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(-w, -w, w), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},

                {Vec3(-w, -w, w), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(-w, w, w), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(-w, w, -w), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(-w, -w, -w), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},

                {Vec3(w, -w, -w), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, w, -w), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, w, w), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)},
                {Vec3(w, -w, w), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f)}
            };

            indices_ = {
                0, 1, 2, 0, 2, 3, //Front
                4, 5, 6, 4, 6, 7, //Back
                8, 9, 10, 8, 10, 11, //Top
                12, 13, 14, 12, 14, 15, //Bottom
                16, 17, 18, 16, 18, 19, //Left
                20, 21, 22, 20, 22, 23 //Right
            };
        }
    }
}
