#pragma once
#include <vector>
#include "common/Math.hpp"


class ColliderMesh 
{
public:
    void setVertices(const std::vector<Math::Vec3>& vertices) { vertices_ = vertices; }
    void setIndices(const std::vector<unsigned int>& indices) { indices_ = indices; }

    const std::vector<Math::Vec3>& getVertices() const { return vertices_; }
    const std::vector<unsigned int>& getIndices() const { return indices_; }

    unsigned int getVertexCount() const { return static_cast<unsigned int>(vertices_.size()); }
    unsigned int getIndexCount() const { return static_cast<unsigned int>(indices_.size()); }


private:
    std::vector<Math::Vec3> vertices_;
    std::vector<unsigned int> indices_;
};
