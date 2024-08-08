#include "AssimpInstanceLoader.h"
#include <iostream>
#include "../glframework/tools/tools.h"
#include "../glframework/material/PhongMaterial.h"
#include "../glframework/material/WhiteMaterial.h"
#include "../glframework/material/ColorMaterial.h"
#include "../glframework/material/ColorWithTexureMaterial.h"

Object *AssimpInstanceLoader::load(const std::string &path, int instanceCount)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        // 读取失败
        std::cerr << "Error:Model Loading Failed !" << std::endl;
        return nullptr;
    }

    Object *rootNode = new Object();

    // 获取模型的路径的前缀名
    std::size_t lastIndex = path.find_last_of("//");
    auto rootPath = path.substr(0, lastIndex + 1);

    process(scene, scene->mRootNode, rootNode, rootPath, instanceCount);
    return rootNode;
}

void AssimpInstanceLoader::process(const aiScene *scene, aiNode *aiNode, Object *parent, const std::string &prefix, int instanceCount)
{
    Object *node = new Object();
    parent->addChild(node);

    // 拿到本地矩阵相关信息
    glm::mat4 localMatrix = getMat4f(aiNode->mTransformation);
    glm::vec3 positon, eulerAngle, scale;
    Tools::decompose(localMatrix, positon, eulerAngle, scale);
    node->setPosition(positon);
    node->setAngleX(eulerAngle.x);
    node->setAngleX(eulerAngle.y);
    node->setAngleX(eulerAngle.z);
    node->setScale(scale);

    // 检查有没有Mesh，并且解析
    for (int i = 0; i < aiNode->mNumMeshes; i++)
    {
        int meshID = aiNode->mMeshes[i];
        // 从aiScene 的 Mesh 数组中找到mesh
        aiMesh *aiMesh = scene->mMeshes[meshID];
        // 解析得到Mesh
        auto mesh = processMesh(scene, aiMesh, prefix, instanceCount);
        // 将Mesh 添加到Node子节点上
        node->addChild(mesh);
    }

    // 遍历寻找处理子节点
    for (int i = 0; i < aiNode->mNumChildren; i++)
    {
        process(scene, aiNode->mChildren[i], node, prefix, instanceCount);
    }
}

InstancedMesh *AssimpInstanceLoader::processMesh(const aiScene *scene, aiMesh *aiMesh, const std::string &prefix, int instanceCount)
{
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<float> colors;
    std::vector<unsigned int> indices;
    

    for (int i = 0; i < aiMesh->mNumVertices; i++)
    {
        // 第i个顶点的位置
        positions.push_back(aiMesh->mVertices[i].x);
        positions.push_back(aiMesh->mVertices[i].y);
        positions.push_back(aiMesh->mVertices[i].z);

        // 第i个顶点的法线
        normals.push_back(aiMesh->mNormals[i].x);
        normals.push_back(aiMesh->mNormals[i].y);
        normals.push_back(aiMesh->mNormals[i].z);

        // 第i个顶点的颜色  obj文件不包含面的颜色定义信息，可以引用材质库 fbx有颜色信息
        if (aiMesh->HasVertexColors(0)) {
            colors.push_back(aiMesh->mColors[0][i].r);
            colors.push_back(aiMesh->mColors[0][i].g);
            colors.push_back(aiMesh->mColors[0][i].b);
        }

        // 第i个顶点的uv
        //  关注其第0套uv，一般情况下是贴图uv
        if (aiMesh->mTextureCoords[0] != nullptr)
        {
            uvs.push_back(aiMesh->mTextureCoords[0][i].x);
            uvs.push_back(aiMesh->mTextureCoords[0][i].y);
        }
        else
        {
            // todo 这里可做优化，没有uv数据怎么处理
            uvs.push_back(0.0f);
            uvs.push_back(0.0f);
        }
    }

    // 解析mesh中的索引值
    for (int i = 0; i < aiMesh->mNumFaces; i++)
    {
        aiFace face = aiMesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    auto geometry = new Geometry(positions, normals, uvs, colors, indices);
    Material *material = nullptr;

    // 判断是否有材质
    if (aiMesh->mMaterialIndex >= 0)
    {
        // ColorWithTexureMaterial *colorWithTexureMaterial = new ColorWithTexureMaterial();

        // // 有材质
        // aiMaterial *aiMat = scene->mMaterials[aiMesh->mMaterialIndex];

        // // 读取diffuse贴图
        // auto diffuseTexture = processTexture(scene, aiMat, aiTextureType_DIFFUSE, prefix);
        // // 读取Specular贴图
        // auto specularMaskTexture = processTexture(scene, aiMat, aiTextureType_SPECULAR, prefix);

        // if (diffuseTexture != nullptr) {
        //     diffuseTexture->setUnit(0);
        // } 

        // if (specularMaskTexture != nullptr) {
        //     specularMaskTexture->setUnit(1);
        // } 

        // colorWithTexureMaterial->mDiffuse = diffuseTexture;
        // colorWithTexureMaterial->mSpecularMask = specularMaskTexture;

        // // if (diffuseTexture != nullptr)
        // // {
        // //     diffuseTexture->setUnit(0);
        // //     colorWithTexureMaterial->mDiffuse = diffuseTexture;
        // // }

        // // if (specularMaskTexture != nullptr)
        // // {
        // //     specularMaskTexture->setUnit(1);
        // //     colorWithTexureMaterial->mSpecularMask = specularMaskTexture;
        // // }

        // // 如果没有纹理贴图，就读取颜色材质
        // // 读取材质的颜色

        // aiColor3D color;
        // aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
        // if (AI_SUCCESS != aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color))
        // {
        //     colorWithTexureMaterial->setKa(glm::vec3(1.0f));
        // }
        // else
        // {
        //     colorWithTexureMaterial->setKa(glm::vec3(color.r, color.g, color.b));
        //     // std::cout << " AMBIENT ka r: " << color.r << " g: " << color.g << " b:" << color.b << std::endl;
        // }
        // if (AI_SUCCESS != aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
        // {
        //     colorWithTexureMaterial->setKd(glm::vec3(1.0f));
        // }
        // else
        // {
        //     colorWithTexureMaterial->setKd(glm::vec3(color.r, color.g, color.b));
        //     // std::cout << " DIFFUSE kd r: " << color.r << " g: " << color.g << " b:" << color.b << std::endl;
        // }

        // if (AI_SUCCESS != aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color))
        // {
        //     colorWithTexureMaterial->setKs(glm::vec3(1.0f));
        // }
        // else
        // {
        //     colorWithTexureMaterial->setKs(glm::vec3(color.r, color.g, color.b));
        //     // std::cout << " SPECULAR ka r: " << color.r << " g: " << color.g << " b:" << color.b << std::endl;
        // }


        // float shiness;
        // if (AI_SUCCESS != aiMat->Get(AI_MATKEY_SHININESS, shiness))
        // {
        //     colorWithTexureMaterial->setShiness(1.0f);
        // }
        // else
        // {
        //     colorWithTexureMaterial->setShiness(shiness);
        //     std::cout << " AI_MATKEY_SHININESS : " << shiness << std::endl;
        // }
        // material = colorWithTexureMaterial;
        WhiteMaterial *whiteMaterial = new WhiteMaterial();
        material = whiteMaterial;
    }
    else
    {
        // 无材质 用默认材质
        WhiteMaterial *whiteMaterial = new WhiteMaterial();
        material = whiteMaterial;
        // material->mDiffuse = new Texture("assets/textures/box.png", 0);
        // material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);
    }

    return new InstancedMesh(geometry, material, instanceCount);
}

Texture *AssimpInstanceLoader::processTexture(const aiScene *scene, const aiMaterial *aiMat,
                                      const aiTextureType &type, const std::string &prefix)
{
    Texture *texture = nullptr;

    // 获取图片的读取路径(相对模型的相对路径)
    aiString aipath;
    aiMat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);
    if (aipath.length == 0)
    {
        return nullptr;
    } 

    // 判断是否是嵌入内存中的图片
    const aiTexture *aitexture = scene->GetEmbeddedTexture(aipath.C_Str());

    if (aitexture != nullptr)
    {
        // 不为空，说明纹理是内嵌的
        unsigned char *data = reinterpret_cast<unsigned char *>(aitexture->pcData);
        uint32_t width = aitexture->mWidth;
        uint32_t height = aitexture->mHeight;
        texture = Texture::createTexture(aipath.C_Str(), data, width, height, 0);
    }
    else
    {
        // 说明纹理是存在磁盘路径上的
        std::string path = prefix + aipath.C_Str();
        std::replace(path.begin(), path.end(), '\\', '/');
        texture = Texture::createTexture(path, 0);
    }
    return texture;
}

glm::mat4 AssimpInstanceLoader::getMat4f(aiMatrix4x4 value)
{
    glm::mat4 to(
        value.a1, value.a2, value.a3, value.a4,
        value.b1, value.b2, value.b3, value.b4,
        value.c1, value.c2, value.c3, value.c4,
        value.d1, value.d2, value.d3, value.d4);

    return to;
}

// Texture *AssimpInstanceLoader::processTexture(const aiScene *scene, const aiMaterial *aiMat,
//                                       const aiTextureType &type, const std::string &prefix)
// {
//     Texture *texture = nullptr;

//     // 获取图片的读取路径(相对模型的相对路径)
//     aiString aipath;
//     aiMat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);
//     if (aipath.length == 0) {
//         return nullptr;
//     }

//     // 判断是否是嵌入内存中的图片
//     const aiTexture *aitexture = scene->GetEmbeddedTexture(aipath.C_Str());

//     if (aitexture != nullptr)
//     {
//         // 不为空，说明纹理是内嵌的
//         unsigned char *data = reinterpret_cast<unsigned char *>(aitexture->pcData);
//         uint32_t width = aitexture->mWidth;
//         uint32_t height = aitexture->mHeight;
//         texture = Texture::createTexture(aipath.C_Str(), data, width, height, 0);
//     }
//     else
//     {
//         // 说明纹理是存在磁盘路径上的
//         std::string path = prefix + aipath.C_Str();
//         std::replace(path.begin(), path.end(), '\\', '/');
//     }
//     return texture;
// }

// Mesh *AssimpLoader::processMesh(const aiScene *scene, aiMesh *aiMesh, const std::string &prefix)
// {
//     std::vector<float> positions;
//     std::vector<float> normals;
//     std::vector<float> uvs;
//     std::vector<unsigned int> indices;

//     for (int i = 0; i < aiMesh->mNumVertices; i++)
//     {
//         // 第i个顶点的位置
//         positions.push_back(aiMesh->mVertices[i].x);
//         positions.push_back(aiMesh->mVertices[i].y);
//         positions.push_back(aiMesh->mVertices[i].z);

//         // 第i个顶点的法线
//         normals.push_back(aiMesh->mNormals[i].x);
//         normals.push_back(aiMesh->mNormals[i].y);
//         normals.push_back(aiMesh->mNormals[i].z);

//         // 第i个顶点的uv
//         //  关注其第0套uv，一般情况下是贴图uv
//         if (aiMesh->mTextureCoords[0] != nullptr)
//         {
//             uvs.push_back(aiMesh->mTextureCoords[0][i].x);
//             uvs.push_back(aiMesh->mTextureCoords[0][i].y);
//         }
//         else
//         {
//             // todo 这里可做优化，没有uv数据怎么处理
//             uvs.push_back(0.0f);
//             uvs.push_back(0.0f);
//         }
//     }

//     // 解析mesh中的索引值
//     for (int i = 0; i < aiMesh->mNumFaces; i++)
//     {
//         aiFace face = aiMesh->mFaces[i];
//         for (int j = 0; j < face.mNumIndices; j++)
//         {
//             indices.push_back(face.mIndices[j]);
//         }
//     }

//     auto geometry = new Geometry(positions, normals, uvs, indices);
//     Material *material = nullptr;

//     // 判断是否有材质
//     if (aiMesh->mMaterialIndex >= 0)
//     {
//         PhongMaterial *phoneMaterial = new PhongMaterial();

//         // 有材质
//         aiMaterial *aiMat = scene->mMaterials[aiMesh->mMaterialIndex];

//         // 读取diffuse贴图
//         auto diffuseTexture = processTexture(scene, aiMat, aiTextureType_DIFFUSE, prefix);
//         // 读取Specular贴图
//         auto specularMaskTexture = processTexture(scene, aiMat, aiTextureType_SPECULAR, prefix);

//         if (diffuseTexture == nullptr) {
//             // todo 这里可以做判断 临时生成
//             diffuseTexture = new Texture("assets/textures/box.png", 0);
//         }
//         diffuseTexture->setUnit(0);

//         if (specularMaskTexture == nullptr) {
//             // todo 这里可以做判断 临时生成
//             specularMaskTexture = new Texture("assets/textures/sp_mask.png", 0);
//         }
//         specularMaskTexture->setUnit(1);

//         phoneMaterial->mDiffuse = diffuseTexture;
//         phoneMaterial->mSpecularMask = specularMaskTexture;
//         material = phoneMaterial;

//         // // 读取diffuse贴图
//         // auto *diffuseTexture = processTexture(scene, aiMat, aiTextureType_DIFFUSE, prefix);

//         // // 读取Specular贴图
//         // auto specularMaskTexture = processTexture(scene, aiMat, aiTextureType_SPECULAR, prefix);

//         // // 这里写死了，判断是否有纹理贴图可以灵活判断
//         // if (diffuseTexture != nullptr && specularMaskTexture != nullptr)
//         // {
//         //     PhongMaterial *phoneMaterial = new PhongMaterial();

//         //     diffuseTexture->setUnit(0);
//         //     specularMaskTexture->setUnit(1);

//         //     phoneMaterial->mDiffuse = diffuseTexture;
//         //     phoneMaterial->mSpecularMask = specularMaskTexture;
//         //     material = phoneMaterial;
//         // }
//         // else
//         // {
//         //     // 如果没有纹理贴图，就读取颜色材质
//         //     // 读取材质的颜色
//         //     ColorMaterial *colorMaterial = new ColorMaterial();

//         //     aiColor3D color;
//         //     aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color);
//         //     if (AI_SUCCESS != aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
//         //         colorMaterial->setKa(glm::vec3(1.0f));
//         //     } else {
//         //         colorMaterial->setKa(glm::vec3(color.r, color.g, color.b));
//         //         //std::cout << " AMBIENT ka r: " << color.r << " g: " << color.g << " b:" << color.b << std::endl;
//         //     }
//         //     if (AI_SUCCESS != aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
//         //         colorMaterial->setKd(glm::vec3(1.0f));
//         //     } else {
//         //         colorMaterial->setKd(glm::vec3(color.r, color.g, color.b));
//         //         // std::cout << " DIFFUSE kd r: " << color.r << " g: " << color.g << " b:" << color.b << std::endl;
//         //     }

//         //     if (AI_SUCCESS != aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
//         //         colorMaterial->setKs(glm::vec3(1.0f));
//         //     } else {
//         //         colorMaterial->setKs(glm::vec3(color.r, color.g, color.b));
//         //         //std::cout << " SPECULAR ka r: " << color.r << " g: " << color.g << " b:" << color.b << std::endl;
//         //     }

//         //     float shiness;
//         //     if (AI_SUCCESS != aiMat->Get(AI_MATKEY_SHININESS, shiness)) {
//         //         colorMaterial->setShiness(1.0f);
//         //     } else {
//         //         colorMaterial->setShiness(shiness);
//         //         std::cout << " AI_MATKEY_SHININESS : " << shiness << std::endl;
//         //     }
//         //     material = colorMaterial;
//         // }
//     }
//     else
//     {
//         // 无材质 用默认材质
//         ColorMaterial *whiteMaterial = new ColorMaterial();
//         material = whiteMaterial;
//         // material->mDiffuse = new Texture("assets/textures/box.png", 0);
//         // material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);
//     }

//     return new Mesh(geometry, material);
// }