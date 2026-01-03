#pragma once
#include "../Mesh.hpp"

namespace Render 
{
    namespace Primitives 
    {
        class Cube : public Mesh 
        {
        public:
            Cube(float size = 1.0f);
            virtual ~Cube();

        private:
            void build(float size);
        };
    }
}
