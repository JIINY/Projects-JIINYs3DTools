#pragma once
#include <memory>

namespace Material 
{
    class MaterialContainer;
    class MaterialConfig;
}


namespace Material
{
    class MaterialPanel
    {
    public:
        MaterialPanel();
        ~MaterialPanel();

        bool initialize();
        void draw(bool isVisible);

    private:
        std::unique_ptr<MaterialContainer> matContainer_;
        std::unique_ptr<MaterialConfig> matConfig_;
    };
}
