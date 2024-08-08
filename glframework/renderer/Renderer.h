#ifndef RENDERER_H
#define RENDERER_H

#include "../core.h"
#include <vector>
#include "../mesh/Mesh.h"
#include "../Scene.h"
#include "../../application/camera/camera.h"
#include "../light/DirectionalLight.h"
#include "../light/PointLight.h"
#include "../light/AmbientLight.h"
#include "../light/SpotLight.h"
#include "../Shader.h"
#include "../material/Material.h"


class Renderer
{
public:
    Renderer();
    ~Renderer();

    void setClearColor(const glm::vec3& color);

    void render(
        const std::vector<Mesh *> &meshes,
        Camera *camera,
        DirectionalLight *directionLight,
        AmbientLight *ambientLight);

    void render(const std::vector<Mesh *> &meshes,
        Camera *camera,
        PointLight* pointLight,
        AmbientLight *ambientLight);

    void render(const std::vector<Mesh *> &meshes,
        Camera *camera,
        SpotLight* spotLight,
        AmbientLight *ambientLight);

    void render(
        const std::vector<Mesh *> &meshes,
        Camera *camera,
        DirectionalLight *directionLight,
        SpotLight* spotLight,
        PointLight* pointLight,
        AmbientLight *ambientLight);

     void render(
        const std::vector<Mesh *> &meshes,
        Camera *camera,
        DirectionalLight *directionLight,
        SpotLight* spotLight,
        const std::vector<PointLight*>& pointLights,
        AmbientLight *ambientLight);  

    void render(
        Scene* scene,
        Camera *camera,
        DirectionalLight *directionLight,
        AmbientLight *ambientLight,
        unsigned int fbo = 0);

private:
   Shader* pickShader(MaterialType type);

   void renderObject(Object* object,
        Camera *camera,
        DirectionalLight *directionLight,
        AmbientLight *ambientLight);

    void setDepthState(Material* material);
    void setPolygonOffSetState(Material* material);
    void setStencilState(Material* material);
    void setBlendState(Material* material);
    void setFaceCullingState(Material* material);

    void projectObject(Object* obj);


private:
    Shader* mPhongShader{nullptr};
    Shader* mWhiteShader{nullptr};
    Shader* mColorShader{nullptr};
    Shader* mColorWithTextureShader{nullptr};
    Shader* mDepthShader{nullptr};
    Shader* mOpacityMaskShader{nullptr};
    Shader* mScreenShader{nullptr};
    Shader* mCubeShader{nullptr};
    Shader* mCubeEnvShader{nullptr};
    Shader* mPhongInstanceShader{nullptr};
    Shader* mGrassInstanceShader{nullptr};

    // 不透明的物体 与 透明物体的队列
    // 注意，每一帧都要清空队列
    std::vector<Mesh*> mOpacityObjects{};
    std::vector<Mesh*> mTransParentObjects{};
};

#endif