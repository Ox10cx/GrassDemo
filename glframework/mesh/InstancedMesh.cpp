#include "InstancedMesh.h"
#include <algorithm>
#include "../GLHead.h"

InstancedMesh::InstancedMesh(Geometry *geometry,
                             Material *material, 
                             unsigned int instanceCount)
                             : Mesh(geometry, material)
{
    mType = ObjectType::InstancedMesh;
    mInstanceCount = instanceCount;
    mInstanceMatrices.resize(mInstanceCount);

    GLCall(glGenBuffers(1, &mMatrixVbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices.data(), GL_DYNAMIC_DRAW));

    GLCall(glBindVertexArray(geometry->getVao()));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo));

    for (int i = 0; i < 4; i++) {
        GLCall(glEnableVertexAttribArray(4 + i));
        GLCall(glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4 * i)));
        GLCall(glVertexAttribDivisor(4 + i, 1)); // 逐个实例渲染
    }

    GLCall(glBindVertexArray(0));
}


void InstancedMesh::updateMatrices()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo));
    // 如果使用glBufferData进行数据更新，会导致重新分配显存空间
    // GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices.data(), GL_DYNAMIC_DRAW));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * mInstanceCount, mInstanceMatrices.data()));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void InstancedMesh::sortMatrices(const glm::mat4& viewMatrix)
{
    // 对透明物体进行排序
    std::sort(
        mInstanceMatrices.begin(),
        mInstanceMatrices.end(),
        [viewMatrix](const glm::mat4& a, const glm::mat4& b) {
            // 计算a的相机坐标系z
            auto modelMatrixA = a;
            //  modelMatrix 是基于原点
            auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
            auto cameraPostionA = viewMatrix * worldPositionA;

            // 计算b的相机坐标系z
            auto modelMatrixB = b;
            //  modelMatrix 是基于原点
            auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
            auto cameraPostionB = viewMatrix * worldPositionB;

            return cameraPostionA.z < cameraPostionB.z;
        });

    
}

InstancedMesh::~InstancedMesh()
{
    
}