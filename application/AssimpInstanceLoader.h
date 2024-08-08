#ifndef ASSIMP_INSTANCE_LOADER_H
#define ASSIMP_INSTANCE_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../glframework/core.h"
#include "../glframework/Object.h"
#include "../glframework/mesh/InstancedMesh.h"
#include "../glframework/Texture.h"

class AssimpInstanceLoader
{
public:
    static Object *load(const std::string &path, int instanceCount);

private:
    static void process(const aiScene *scene, aiNode *aiNode, Object *parent, const std::string &prefix, int instanceCount);
    static InstancedMesh *processMesh(const aiScene *scene, aiMesh *aiMesh, const std::string &prefix, int instanceCount);
    static Texture *processTexture(const aiScene *scene, const aiMaterial *aiMat, const aiTextureType &type, const std::string &prefix);
    static glm::mat4 getMat4f(aiMatrix4x4 value);
};

#endif