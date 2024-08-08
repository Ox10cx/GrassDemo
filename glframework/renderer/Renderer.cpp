#include "Renderer.h"

#include "../../GLHead.h"
#include "../material/PhongMaterial.h"
#include "../material/WhiteMaterial.h"
#include "../material/ColorMaterial.h"
#include "../material/ColorWithTexureMaterial.h"
#include "../material/OpacityMaterial.h"
#include "../material/ScreenMaterial.h"
#include "../material/CubeMaterial.h"
#include "../material/CubeEnvMaterial.h"
#include "../material/PhongInstanceMaterial.h"
#include "../material/GrassInstanceMaterial.h"
#include "../mesh/InstancedMesh.h"
#include <string>
#include <algorithm>

Renderer::Renderer()
{
    mPhongShader = new Shader("assets/shaders/phong.vert", "assets/shaders/phong.frag");
    // mPhongShader = new Shader("assets/shaders/pointlight.vert", "assets/shaders/pointlight.frag");
    // mPhongShader = new Shader("assets/shaders/spotlight.vert", "assets/shaders/spotlight.frag");

    //  mPhongShader = new Shader("assets/shaders/spotlightpkg.vert", "assets/shaders/spotlightpkg.frag");
    // mPhongShader = new Shader("assets/shaders/pointlightpkg.vert", "assets/shaders/pointlightpkg.frag");

    // mPhongShader = new Shader("assets/shaders/phongpkg.vert", "assets/shaders/phongpkg.frag");

    // mPhongShader = new Shader("assets/shaders/lightspkg.vert", "assets/shaders/lightspkg.frag");

    mWhiteShader = new Shader("assets/shaders/white.vert", "assets/shaders/white.frag");

    mColorShader = new Shader("assets/shaders/colormat.vert", "assets/shaders/colormat.frag");

    mColorWithTextureShader = new Shader("assets/shaders/colorwithtexture.vert", "assets/shaders/colorwithtexture.frag");

    mDepthShader = new Shader("assets/shaders/depth.vert", "assets/shaders/depth.frag");

    mOpacityMaskShader = new Shader("assets/shaders/opacity.vert", "assets/shaders/opacity.frag");

    mScreenShader = new Shader("assets/shaders/screen.vert", "assets/shaders/screen.frag");

    mCubeShader = new Shader("assets/shaders/cube.vert", "assets/shaders/cube.frag"); 

    mCubeEnvShader = new Shader("assets/shaders/cubeenv.vert", "assets/shaders/cubeenv.frag"); 

    mPhongInstanceShader = new Shader("assets/shaders/phongInstance.vert", "assets/shaders/phongInstance.frag"); 

    mGrassInstanceShader = new Shader("assets/shaders/grassInstance.vert", "assets/shaders/grassInstance.frag"); 
}

Renderer::~Renderer()
{
}

void Renderer::setClearColor(const glm::vec3 &color)
{
    GLCall(glClearColor(color.r, color.g, color.b, 1.0));
}

void Renderer::render(
    const std::vector<Mesh *> &meshes,
    Camera *camera,
    SpotLight *spotLight,
    AmbientLight *ambientLight)
{
    // 1.设置当前帧绘制的时候，opengl的必要状态机参数
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    // 2 清理画布
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 3 遍历Mesh绘制
    for (int i = 0; i < meshes.size(); i++)
    {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 决定使用哪一个Shader
        Shader *shader = pickShader(material->mType);
        if (shader == nullptr)
        {
            return;
        }

        // 更新Shader
        shader->begin();

        switch (material->mType)
        {
        case MaterialType::PhongMaterial:
        {
            PhongMaterial *phongMaterial = (PhongMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            phongMaterial->mDiffuse->bind();

            // 高光蒙版
            shader->setUniformValue("specularMaskSampler", 1);
            phongMaterial->mSpecularMask->bind();

            // shader->setUniformValue("lightPosition", spotLight->getPosition());
            // shader->setUniformValue("lightColor", spotLight->mColor);
            // shader->setUniformValue("targetDirection", spotLight->mTargetDirection);
            // shader->setUniformValue("innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
            // shader->setUniformValue("outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
            // shader->setUniformValue("specularIntensity", spotLight->mSpecularIntensity);

            shader->setUniformValue("spotLight.position", spotLight->getPosition());
            shader->setUniformValue("spotLight.color", spotLight->mColor);
            shader->setUniformValue("spotLight.targetDirection", spotLight->mTargetDirection);
            shader->setUniformValue("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
            shader->setUniformValue("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
            shader->setUniformValue("spotLight.specularIntensity", spotLight->mSpecularIntensity);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", phongMaterial->mShiness);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::WhiteMaterial:
        {
            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        default:
            continue;
        }

        // 绑定 vao
        GLCall(glBindVertexArray(geometry->getVao()));

        // 绘制
        GLCall(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

        // GLCall(glBindVertexArray(0));
        // shader->end();
    }
}

void Renderer::render(
    const std::vector<Mesh *> &meshes,
    Camera *camera,
    PointLight *pointLight,
    AmbientLight *ambientLight)
{
    // 1.设置当前帧绘制的时候，opengl的必要状态机参数
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    // 2 清理画布
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 3 遍历Mesh绘制
    for (int i = 0; i < meshes.size(); i++)
    {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 决定使用哪一个Shader
        Shader *shader = pickShader(material->mType);
        if (shader == nullptr)
        {
            return;
        }

        // 更新Shader
        shader->begin();

        switch (material->mType)
        {
        case MaterialType::PhongMaterial:
        {
            PhongMaterial *phongMaterial = (PhongMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            phongMaterial->mDiffuse->bind();

            // 高光蒙版
            shader->setUniformValue("specularMaskSampler", 1);
            phongMaterial->mSpecularMask->bind();

            // shader->setUniformValue("lightPosition", pointLight->getPosition());
            // shader->setUniformValue("lightColor", pointLight->mColor);
            // shader->setUniformValue("k2", pointLight->mK2);
            // shader->setUniformValue("k1", pointLight->mK1);
            // shader->setUniformValue("kc", pointLight->mKc);
            // shader->setUniformValue("specularIntensity", pointLight->mSpecularIntensity);

            shader->setUniformValue("pointLight.position", pointLight->getPosition());
            shader->setUniformValue("pointLight.color", pointLight->mColor);
            shader->setUniformValue("pointLight.k2", pointLight->mK2);
            shader->setUniformValue("pointLight.k1", pointLight->mK1);
            shader->setUniformValue("pointLight.kc", pointLight->mKc);
            shader->setUniformValue("pointLight.specularIntensity", pointLight->mSpecularIntensity);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", phongMaterial->mShiness);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::WhiteMaterial:
        {
            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        default:
            continue;
        }

        // 绑定 vao
        GLCall(glBindVertexArray(geometry->getVao()));

        // 绘制
        GLCall(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

        // GLCall(glBindVertexArray(0));
        // shader->end();
    }
}

void Renderer::render(
    const std::vector<Mesh *> &meshes,
    Camera *camera,
    DirectionalLight *directionLight,
    AmbientLight *ambientLight)
{
    // 1.设置当前帧绘制的时候，opengl的必要状态机参数
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    // 2 清理画布
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 3 遍历Mesh绘制
    for (int i = 0; i < meshes.size(); i++)
    {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 决定使用哪一个Shader
        Shader *shader = pickShader(material->mType);
        if (shader == nullptr)
        {
            return;
        }

        // 更新Shader
        shader->begin();

        switch (material->mType)
        {
        case MaterialType::PhongMaterial:
        {
            PhongMaterial *phongMaterial = (PhongMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            phongMaterial->mDiffuse->bind();

            // 高光蒙版
            shader->setUniformValue("specularMaskSampler", 1);
            phongMaterial->mSpecularMask->bind();

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", phongMaterial->mShiness);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::WhiteMaterial:
        {
            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        default:
            continue;
        }

        // 绑定 vao
        GLCall(glBindVertexArray(geometry->getVao()));

        // 绘制
        GLCall(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

        // GLCall(glBindVertexArray(0));
        // shader->end();
    }
}

void Renderer::render(
    const std::vector<Mesh *> &meshes,
    Camera *camera,
    DirectionalLight *directionLight,
    SpotLight *spotLight,
    PointLight *pointLight,
    AmbientLight *ambientLight)
{
    // 1.设置当前帧绘制的时候，opengl的必要状态机参数
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    // 2 清理画布
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 3 遍历Mesh绘制
    for (int i = 0; i < meshes.size(); i++)
    {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 决定使用哪一个Shader
        Shader *shader = pickShader(material->mType);
        if (shader == nullptr)
        {
            return;
        }

        // 更新Shader
        shader->begin();

        switch (material->mType)
        {
        case MaterialType::PhongMaterial:
        {
            PhongMaterial *phongMaterial = (PhongMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            phongMaterial->mDiffuse->bind();

            // 高光蒙版
            shader->setUniformValue("specularMaskSampler", 1);
            phongMaterial->mSpecularMask->bind();

            // 平行光
            shader->setUniformValue("directionLight.targetDirection", directionLight->mDirection);
            shader->setUniformValue("directionLight.color", directionLight->mColor);
            shader->setUniformValue("directionLight.specularIntensity", directionLight->mSpecularIntensity);

            // 探照灯
            shader->setUniformValue("spotLight.position", spotLight->getPosition());
            shader->setUniformValue("spotLight.color", spotLight->mColor);
            shader->setUniformValue("spotLight.targetDirection", spotLight->mTargetDirection);
            shader->setUniformValue("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
            shader->setUniformValue("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
            shader->setUniformValue("spotLight.specularIntensity", spotLight->mSpecularIntensity);

            // 点光源
            shader->setUniformValue("pointLight.position", pointLight->getPosition());
            shader->setUniformValue("pointLight.color", pointLight->mColor);
            shader->setUniformValue("pointLight.k2", pointLight->mK2);
            shader->setUniformValue("pointLight.k1", pointLight->mK1);
            shader->setUniformValue("pointLight.kc", pointLight->mKc);
            shader->setUniformValue("pointLight.specularIntensity", pointLight->mSpecularIntensity);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", phongMaterial->mShiness);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::WhiteMaterial:
        {
            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        default:
            continue;
        }

        // 绑定 vao
        GLCall(glBindVertexArray(geometry->getVao()));

        // 绘制
        GLCall(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

        // GLCall(glBindVertexArray(0));
        // shader->end();
    }
}

void Renderer::render(
    const std::vector<Mesh *> &meshes,
    Camera *camera,
    DirectionalLight *directionLight,
    SpotLight *spotLight,
    const std::vector<PointLight *> &pointLights,
    AmbientLight *ambientLight)
{
    // 1.设置当前帧绘制的时候，opengl的必要状态机参数
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    // 2 清理画布
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // 3 遍历Mesh绘制
    for (int i = 0; i < meshes.size(); i++)
    {
        auto mesh = meshes[i];
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 决定使用哪一个Shader
        Shader *shader = pickShader(material->mType);
        if (shader == nullptr)
        {
            return;
        }

        // 更新Shader
        shader->begin();

        switch (material->mType)
        {
        case MaterialType::PhongMaterial:
        {
            PhongMaterial *phongMaterial = (PhongMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            phongMaterial->mDiffuse->bind();

            // 高光蒙版
            shader->setUniformValue("specularMaskSampler", 1);
            phongMaterial->mSpecularMask->bind();

            // 平行光
            shader->setUniformValue("directionLight.targetDirection", directionLight->mDirection);
            shader->setUniformValue("directionLight.color", directionLight->mColor);
            shader->setUniformValue("directionLight.specularIntensity", directionLight->mSpecularIntensity);

            // 探照灯
            shader->setUniformValue("spotLight.position", spotLight->getPosition());
            shader->setUniformValue("spotLight.color", spotLight->mColor);
            shader->setUniformValue("spotLight.targetDirection", spotLight->mTargetDirection);
            shader->setUniformValue("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
            shader->setUniformValue("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));
            shader->setUniformValue("spotLight.specularIntensity", spotLight->mSpecularIntensity);

            for (int i = 0; i < pointLights.size(); i++)
            {
                auto pointLight = pointLights[i];

                std::string baseName = "pointLights[";
                baseName.append(std::to_string(i));
                baseName.append("]");

                // 点光源
                shader->setUniformValue(baseName + ".position", pointLight->getPosition());
                shader->setUniformValue(baseName + ".color", pointLight->mColor);
                shader->setUniformValue(baseName + ".k2", pointLight->mK2);
                shader->setUniformValue(baseName + ".k1", pointLight->mK1);
                shader->setUniformValue(baseName + ".kc", pointLight->mKc);
                shader->setUniformValue(baseName + ".specularIntensity", pointLight->mSpecularIntensity);
            }

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", phongMaterial->mShiness);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::WhiteMaterial:
        {
            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        default:
            continue;
        }

        // 绑定 vao
        GLCall(glBindVertexArray(geometry->getVao()));

        // 绘制
        GLCall(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

        // GLCall(glBindVertexArray(0));
        // shader->end();
    }
}

void Renderer::render(
    Scene *scene,
    Camera *camera,
    DirectionalLight *directionLight,
    AmbientLight *ambientLight,
    unsigned int fbo)
{
    // 绑定fbo
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

    // 重要紧急 一定要打开深度检测相关的内容
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));
    GLCall(glDepthMask(GL_TRUE));

    // PolyGonOffset
    GLCall(glDisable(GL_POLYGON_OFFSET_FILL));
    GLCall(glDisable(GL_POLYGON_OFFSET_LINE));

    // 开启模版测试写入
    GLCall(glEnable(GL_STENCIL_TEST));
    GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
    GLCall(glStencilMask(0xFF)); // 保证了模版缓冲可以被清理

    // 颜色混合
    GLCall(glDisable(GL_BLEND));


    // 2 清理画布
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

    mOpacityObjects.clear();
    mTransParentObjects.clear();
    projectObject(scene);

    // 对透明物体进行排序
    std::sort(
        mTransParentObjects.begin(),
        mTransParentObjects.end(),
        [camera](const Mesh *a, const Mesh *b) {
            // 视图变换矩阵
            auto viewMatrix = camera->getViewMatrix();
            // 计算a的相机坐标系z
            auto modelMatrixA = a->getModelMatrix();
            //  modelMatrix 是基于原点
            auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
            auto cameraPostionA = viewMatrix * worldPositionA;

            // 计算b的相机坐标系z
            auto modelMatrixB = b->getModelMatrix();
            //  modelMatrix 是基于原点
            auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
            auto cameraPostionB = viewMatrix * worldPositionB;

            return cameraPostionA.z < cameraPostionB.z;
        });

    // 渲染两个队列
    for (int i = 0; i < mOpacityObjects.size(); i++) {
        renderObject(mOpacityObjects[i], camera, directionLight, ambientLight);
    }  
    for (int i = 0; i < mTransParentObjects.size(); i++) {
        renderObject(mTransParentObjects[i], camera, directionLight, ambientLight);
    }  

    // // 3 scene跟节点递归绘制渲染
    // renderObject(scene, camera, directionLight, ambientLight);
}

void Renderer::renderObject(Object *object,
                            Camera *camera,
                            DirectionalLight *directionLight,
                            AmbientLight *ambientLight)
{
    // 1  判断是Mesh还是Object, 如果是Mesh 则需要渲染
    if (object->getType() == ObjectType::Mesh || object->getType() == ObjectType::InstancedMesh)
    {
        auto mesh = (Mesh *)object;
        auto geometry = mesh->mGeometry;
        auto material = mesh->mMaterial;

        // 深度检测
        setDepthState(material);
        // 检测PolygonOffset
        setPolygonOffSetState(material);
        // 模版测试
        setStencilState(material);
        // 颜色混合
        setBlendState(material);
        // 面剔除
        setFaceCullingState(material);

        // 决定使用哪一个Shader
        Shader *shader = pickShader(material->mType);
        if (shader == nullptr)
        {
            return;
        }

        // 更新Shader
        shader->begin();

        switch (material->mType)
        {
        case MaterialType::PhongMaterial:
        {
            PhongMaterial *phongMaterial = (PhongMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            phongMaterial->mDiffuse->bind();

            // // 高光蒙版
            // shader->setUniformValue("specularMaskSampler", 1);
            // phongMaterial->mSpecularMask->bind();

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", phongMaterial->mShiness);

            shader->setUniformValue("opacity", phongMaterial->mOpacity);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::WhiteMaterial:
        {
            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::ColorMaterial:
        {
            ColorMaterial *colorMaterial = (ColorMaterial *)material;

            shader->setUniformValue("matColor.aAmbient", colorMaterial->mKa);
            shader->setUniformValue("matColor.aDiffuse", colorMaterial->mKd);
            shader->setUniformValue("matColor.aSpecular", colorMaterial->mKs);

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", colorMaterial->mShiness);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::ColorWithTextureMaterial:
        {
            ColorWithTexureMaterial *colorWithTexureMaterial = (ColorWithTexureMaterial *)material;

            // diffuse 贴图
            if (colorWithTexureMaterial->mDiffuse != nullptr)
            {
                shader->setUniformValue("diffuseSampler", 0);
                colorWithTexureMaterial->mDiffuse->bind();
            }
            shader->setUniformValue("hasDiffuseTexure", colorWithTexureMaterial->mDiffuse != nullptr);

            // specular 贴图
            if (colorWithTexureMaterial->mSpecularMask != nullptr)
            {
                shader->setUniformValue("specularMaskSampler", 1);
                colorWithTexureMaterial->mSpecularMask->bind();
            }
            shader->setUniformValue("hasSpecularMaskTexure", colorWithTexureMaterial->mSpecularMask != nullptr);

            shader->setUniformValue("matColor.aAmbient", colorWithTexureMaterial->mKa);
            shader->setUniformValue("matColor.aDiffuse", colorWithTexureMaterial->mKd);
            shader->setUniformValue("matColor.aSpecular", colorWithTexureMaterial->mKs);
            shader->setUniformValue("shiness", colorWithTexureMaterial->mShiness);

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);

            shader->setUniformValue("opacity", colorWithTexureMaterial->mOpacity);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        case MaterialType::DepthMaterial:
        {
            shader->setUniformValue("near", camera->mNear);
            shader->setUniformValue("far", camera->mFar);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::OpacityMaterial:
        {
            OpacityMaterial *opacityMaterial = (OpacityMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            opacityMaterial->mDiffuse->bind();

            // 透明度蒙版
            shader->setUniformValue("opacityMaskSampler", 1);
            opacityMaterial->mOpacityMask->bind();

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", opacityMaterial->mShiness);

            shader->setUniformValue("opacity", opacityMaterial->mOpacity);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::ScreenMaterial:
        {
            ScreenMaterial *screenMaterial = (ScreenMaterial *)material;
            shader->setUniformValue("screenTexSampler", 0);
            shader->setUniformValue("texWidth", screenMaterial->mScreenTexture->getWidth());
            shader->setUniformValue("texWidth", screenMaterial->mScreenTexture->getHeight());
            screenMaterial->mScreenTexture->bind();
        }
        break;
        case MaterialType::CubeMaterial:
        {
            CubeMaterial *cubeMaterial = (CubeMaterial *)material;
            mesh->setPosition(camera->mPosition);
            // diffuse
            // shader->setUniformValue("cubeSampler", 0);

            shader->setUniformValue("sphereSampler", 0);
            // 将纹理与纹理单元挂钩
            cubeMaterial->mDiffuseTexture->bind();

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::CubeEnvMaterial:
        {
            CubeEnvMaterial *cubeEnvMaterial = (CubeEnvMaterial *)material;

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            cubeEnvMaterial->mDiffuse->bind();

            //  // 高光蒙版
            // shader->setUniformValue("specularMaskSampler", 1);
            // phongMaterial->mSpecularMask->bind();

            shader->setUniformValue("envSampler", 1);
            cubeEnvMaterial->mEnv->bind();

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", cubeEnvMaterial->mShiness);

            shader->setUniformValue("opacity", cubeEnvMaterial->mOpacity);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());
        }
        break;
        case MaterialType::PhongInstanceMaterial:
        {
            PhongInstanceMaterial *phongInstanceMaterial = (PhongInstanceMaterial *)material;
            InstancedMesh* instanceMesh = (InstancedMesh*) mesh;
            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            phongInstanceMaterial->mDiffuse->bind();

            // // 高光蒙版
            // shader->setUniformValue("specularMaskSampler", 1);
            // phongMaterial->mSpecularMask->bind();

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", phongInstanceMaterial->mShiness);

            shader->setUniformValue("opacity", phongInstanceMaterial->mOpacity);

            // 法线矩阵
            glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());

            // 传输矩阵变换数组
            shader->setUnifromValue("matrices", instanceMesh->mInstanceMatrices.data(), instanceMesh->mInstanceCount);
 
        }
        break;
        case MaterialType::GrassInstanceMaterial:
        {
            GrassInstanceMaterial *grassInstanceMaterial = (GrassInstanceMaterial *)material;
            InstancedMesh* instanceMesh = (InstancedMesh*) mesh;
            
            //排序矩阵
            instanceMesh->sortMatrices(camera->getViewMatrix());
            instanceMesh->updateMatrices();

            // diffuse
            shader->setUniformValue("sampler", 0);
            // 将纹理与纹理单元挂钩
            grassInstanceMaterial->mDiffuse->bind();

            // // 高光蒙版
            // shader->setUniformValue("specularMaskSampler", 1);
            // phongMaterial->mSpecularMask->bind();

            shader->setUniformValue("opacityMaskSampler", 1);
            grassInstanceMaterial->mOpacityMask->bind();

            shader->setUniformValue("lightDirection", directionLight->mDirection);
            shader->setUniformValue("lightColor", directionLight->mColor);

            shader->setUniformValue("cameraPosition", camera->mPosition);
            shader->setUniformValue("specularIntensity", directionLight->mSpecularIntensity);
            shader->setUniformValue("ambientLight", ambientLight->mColor);
            shader->setUniformValue("shiness", grassInstanceMaterial->mShiness);

            shader->setUniformValue("opacity", grassInstanceMaterial->mOpacity);

            shader->setUniformValue("time", (float)glfwGetTime());

            // 贴图特性
            shader->setUniformValue("uvScale", grassInstanceMaterial->mUvScale);
            shader->setUniformValue("brightness", grassInstanceMaterial->mBrightness);

            // 风力相关
            shader->setUniformValue("windScale", grassInstanceMaterial->mWindScale);
            shader->setUniformValue("windDirection", grassInstanceMaterial->mWindDirection);
            shader->setUniformValue("phaseScale", grassInstanceMaterial->mPhaseScale);
            

            // 云层更新
            shader->setUniformValue("cloudMaskSampler", 2);
            grassInstanceMaterial->mCloudMask->bind();
            shader->setUniformValue("cloudWhiteColor", grassInstanceMaterial->mCloudWhiteColor);
            shader->setUniformValue("cloudBlackColor", grassInstanceMaterial->mCloudBlackColor);
            shader->setUniformValue("cloudUvScale", grassInstanceMaterial->mCloudUvScale);
            shader->setUniformValue("cloudSpeed", grassInstanceMaterial->mCloudSpeed);
            shader->setUniformValue("cloudLerp", grassInstanceMaterial->mCloudLerp);


            // // 法线矩阵
            // glm::mat4 normalMatrix = glm::transpose(glm::inverse(mesh->getModelMatrix()));
            // shader->setUnifromValue("normalMatrix", glm::mat3(normalMatrix));

            shader->setUnifromValue("modelMatrix", mesh->getModelMatrix());
            shader->setUnifromValue("viewMatrix", camera->getViewMatrix());
            shader->setUnifromValue("projectMatrix", camera->getProjectionMatrix());

            // 传输矩阵变换数组
            shader->setUnifromValue("matrices", instanceMesh->mInstanceMatrices.data(), instanceMesh->mInstanceCount);
 
        }
        break;
        default:
            break;
        }
        // 绑定 vao
        GLCall(glBindVertexArray(geometry->getVao()));
        // 绘制
        if (object->getType() == ObjectType::InstancedMesh) {
            // 如果是实例化绘制
            InstancedMesh* instanceMesh = (InstancedMesh*) mesh;
            GLCall(glDrawElementsInstanced(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0, instanceMesh->mInstanceCount));
        } else {
            GLCall(glDrawElements(GL_TRIANGLES, geometry->getIndicesCount(), GL_UNSIGNED_INT, 0));

        }
        GLCall(glBindVertexArray(0));
        shader->end();
    }

    // // 2 遍历object的子节点，对每个子节点进行渲染
    // auto children = object->getChilderen();
    // for (int i = 0; i < children.size(); i++)
    // {
    //     renderObject(children[i], camera, directionLight, ambientLight);
    // }
}

Shader *Renderer::pickShader(MaterialType type)
{
    Shader *pShader = nullptr;
    switch (type)
    {
    case MaterialType::PhongMaterial:
        pShader = mPhongShader;
        break;
    case MaterialType::WhiteMaterial:
        pShader = mWhiteShader;
        break;
    case MaterialType::ColorMaterial:
        pShader = mColorShader;
        break;
    case MaterialType::ColorWithTextureMaterial:
        pShader = mColorWithTextureShader;
        break;
    case MaterialType::DepthMaterial:
        pShader = mDepthShader;
        break;
    case MaterialType::OpacityMaterial:
        pShader = mOpacityMaskShader;
        break;
    case MaterialType::ScreenMaterial:
        pShader = mScreenShader;
        break;
    case MaterialType::CubeMaterial:
        pShader = mCubeShader;
        break;
    case MaterialType::CubeEnvMaterial:
        pShader = mCubeEnvShader;
        break;
    case MaterialType::PhongInstanceMaterial:
        pShader = mPhongInstanceShader;
        break;
    case MaterialType::GrassInstanceMaterial:
        pShader = mGrassInstanceShader;
        break;
    default:
        break;
    }
    return pShader;
}

void Renderer::setDepthState(Material *material)
{
    if (material->mDepthTest)
    {
        GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glDepthFunc(material->mDepthFunc));
    }
    else
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }
    if (material->mDepthWrite)
    {
        GLCall(glDepthMask(GL_TRUE));
    }
    else
    {
        GLCall(glDepthMask(GL_FALSE));
    }
}

void Renderer::setPolygonOffSetState(Material *material)
{
    if (material->mPolygonOffset)
    {
        GLCall(glEnable(material->mPolygonOffsetType));
        GLCall(glPolygonOffset(material->mFactor, material->mUnit));
    }
    else
    {
        GLCall(glDisable(GL_POLYGON_OFFSET_FILL));
        GLCall(glDisable(GL_POLYGON_OFFSET_LINE));
    }
}

void Renderer::setStencilState(Material *material)
{
    if (material->mStencilTest)
    {
        GLCall(glEnable(GL_STENCIL_TEST));
        GLCall(glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask));
        GLCall(glStencilOp(material->mSFail, material->mZFail, material->mSZPass));
        GLCall(glStencilMask(material->mStencilMask));
    }
    else
    {
        GLCall(glDisable(GL_STENCIL_TEST));
    }
}

void Renderer::setBlendState(Material *material)
{
    if (material->mBlend)
    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }
    else
    {
        GLCall(glDisable(GL_BLEND));
    }
}

void Renderer::setFaceCullingState(Material* material) {
    if (material->mFaceCulling) {
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glFrontFace(material->mFrontFace));
        GLCall(glCullFace(material->mCullFace));
    } else {
        GLCall(glDisable(GL_CULL_FACE));
    }
}

void Renderer::projectObject(Object *obj)
{
    if (obj->getType() == ObjectType::Mesh || obj->getType() == ObjectType::InstancedMesh)
    {
        Mesh *mesh = (Mesh *)obj;
        auto material = mesh->mMaterial;
        if (material->mBlend)
        {
            // 如果是透明
            mTransParentObjects.push_back(mesh);
        }
        else
        {
            mOpacityObjects.push_back(mesh);
        }
    }

    auto children = obj->getChilderen();
    for (int i = 0; i < children.size(); i++)
    {
        projectObject(children[i]);
    }
}