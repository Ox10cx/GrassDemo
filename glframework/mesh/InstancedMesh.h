#ifndef INSTANCED_MESH_H
#define INSTANCED_MESH_H

#include "Mesh.h"
#include <vector>

class InstancedMesh : public Mesh
{
public:
    InstancedMesh(Geometry* geometry, Material* material, unsigned int instanceCount);
    ~InstancedMesh();

    void updateMatrices();
    void sortMatrices(const glm::mat4& viewMatrix);

public:
   unsigned int mInstanceCount {0};
   std::vector<glm::mat4> mInstanceMatrices{};
   unsigned int mMatrixVbo {0};
};

#endif