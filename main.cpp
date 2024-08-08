#include "glframework/core.h"
#include "GLHead.h"
#include <iostream>
#include <vector>
#include "application/Application.h"
#include "application/camera/trackballControl.h"
#include "application/camera/gameCameraControl.h"
#include "application/camera/persepectiveCamera.h"
#include "application/camera/orthographicCamera.h"
#include "application/AssimpLoader.h"
#include "application/AssimpInstanceLoader.h"

#include "glframework/Shader.h"
#include "application/stb_image.h"
#include "glframework/Texture.h"
#include "glframework/Geometry.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/material/DepthMaterial.h"
#include "glframework/material/OpacityMaterial.h"
#include "glframework/material/ScreenMaterial.h"
#include "glframework/material/CubeMaterial.h"
#include "glframework/material/CubeEnvMaterial.h"
#include "glframework/material/PhongInstanceMaterial.h"
#include "glframework/material/GrassInstanceMaterial.h"
#include "glframework/mesh/Mesh.h"
#include "glframework/mesh/InstancedMesh.h"
#include "glframework/Scene.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/framebuffer/FrameBuffer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


Camera *camera = nullptr;
CameraControl *cameraControl = nullptr;
void OnResize(int width, int height)
{
    GLCall(glViewport(0, 0, width, height));
    std::cout << "OnResize :" << width << "  :" << height << std::endl;
}

void OnKeyBorad(int key, int action, int mods)
{
    std::cout << "key: " << key << " action: " << action << " mods: " << mods << std::endl;
    cameraControl->onKey(key, action, mods);
}

void OnMouseCallback(int button, int action, int mods)
{
    double x, y;
    Application *app = Application::getInstance();
    app->getCursorPosition(&x, &y);
    std::cout << "鼠标点击 " << "按键是 " << button << " 松开是否按下" << action << " " << mods << " x: " << x << " y: " << y << std::endl;
    cameraControl->onMouse(button, action, x, y);
}

void onCursorCallback(double xpos, double ypos)
{
    std::cout << "鼠标移动 " << xpos << " " << ypos << std::endl;
    cameraControl->onCursor(xpos, ypos);
}

void onScorllCallback(double xoffset, double yoffset)
{
    std::cout << "鼠标缩放 " << xoffset << " " << yoffset << std::endl;
    cameraControl->onScorll(yoffset);
}



glm::mat4 viewMatrix(1.0f);
void prepareCamera()
{
    float aspect = (float)Application::getInstance()->getWidth() / (float)Application::getInstance()->getHeight();
    // camera = new PersepectiveCamera(60.0f, aspect, 0.1f, 1000.0f);
    camera = new PersepectiveCamera(60.0f, aspect, 0.1f, 50.0f);

    // float size =3.0f;
    // camera = new OrthoGraphicCamera(-size, size, -size, size, size, -size);


    //cameraControl = new TrackBallCameraControl();
    cameraControl = new GameCameraControl();
    cameraControl->setCamera(camera); 
    cameraControl->setSensitivity(0.4f);
	cameraControl->setScaleSpeed(0.1f);

    viewMatrix = glm::lookAt(glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}


Renderer* mRenderer = nullptr;
std::vector<Mesh*> meshes{};
DirectionalLight* mDirectionalLight = nullptr;
AmbientLight* mAmbientLight = nullptr;
PointLight* mPointLight = nullptr;
SpotLight* mSpotLight = nullptr;
std::vector<PointLight*> mPointLights{};
glm::vec3 mClearColor{0.0f};
Scene* mInScreenScene = nullptr;
Scene* mOffScreenScene = nullptr;
unsigned int mFbo = 0;
Texture* mColorAttachment = nullptr;
FrameBuffer* frameBuffer = nullptr;
GrassInstanceMaterial* grassMaterial = nullptr;


void prepare() {
    mRenderer = new Renderer();

    // // 创建一个Geometry
    // auto geometry = Geometry::createSphere(0.5f);

    // // 创建一个Material
    // auto material01 = new PhongMaterial();
    // material01->mShiness = 16.0f;
    // material01->mDiffuse = new Texture("assets/textures/goku.jpg", 0);

    // auto material02 = new PhongMaterial();
    // material02->mShiness = 16.0f;
    // material02->mDiffuse = new Texture("assets/textures/wall.jpg", 0);

    // // 生成Mesh
    // auto mesh01 = new Mesh(geometry, material01);
    // auto mesh02 = new Mesh(geometry, material02);
    // mesh02->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));

    // meshes.push_back(mesh01);
    // meshes.push_back(mesh02);

    // mDirectionalLight = new DirectionalLight();
    // mAmbientLight = new AmbientLight();
    // mAmbientLight->mColor = glm::vec3(0.1f);

    // // ---------------高光模版Demo--------------
    // // 创建一个Geometry
    // auto geometry = Geometry::createBox(0.5f);

    // // 创建一个Material
    // auto material = new PhongMaterial();
    // material->mShiness = 4.0f;
    // material->mDiffuse = new Texture("assets/textures/box.png", 0);
    // material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);

    // // 生成Mesh
    // auto mesh = new Mesh(geometry, material);
    // meshes.push_back(mesh);

    // mDirectionalLight = new DirectionalLight();
    // mDirectionalLight->mDirection = glm::vec3(-1.0, 0.0, -1.0);
    // mAmbientLight = new AmbientLight();
    // mAmbientLight->mColor = glm::vec3(0.1f);

    // // ---------------高光模版Demo--------------

    // // ---------------点光源Demo--------------

    // auto geometry = Geometry::createBox(0.5f);

    // // 创建一个Material
    // auto material = new PhongMaterial();
    // material->mShiness = 4.0f;
    // material->mDiffuse = new Texture("assets/textures/box.png", 0);
    // material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);

    // // 生成Mesh
    // auto mesh = new Mesh(geometry, material);
    // meshes.push_back(mesh);
    
    // auto whiteGeometry = Geometry::createSphere(0.1f);
    // auto whiteMaterial = new WhiteMaterial();
 
    // // 生成Mesh
    // auto meshWhite = new Mesh(whiteGeometry, whiteMaterial);
    // meshWhite->setPosition(glm::vec3(0.5f, 0.0f, 0.0f));
    // meshes.push_back(meshWhite);

    // mPointLight = new PointLight();
    // mPointLight->setPosition(meshWhite->getPosition());
    // mPointLight->mK2 = 0.017;
    // mPointLight->mK1 = 0.07;
    // mPointLight->mKc = 1.0;

    // mAmbientLight = new AmbientLight();
    // mAmbientLight->mColor = glm::vec3(0.4f);
    // // ---------------点光源Demo--------------


    // // ---------------探照灯光源Demo--------------

    // auto geometry = Geometry::createBox(1.0f);

    // // 创建一个Material
    // auto material = new PhongMaterial();
    // material->mShiness = 16.0f;
    // material->mDiffuse = new Texture("assets/textures/box.png", 0);
    // material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);

    // // 生成Mesh
    // auto mesh = new Mesh(geometry, material);
    // meshes.push_back(mesh);
    
    // auto whiteGeometry = Geometry::createSphere(0.1f);
    // auto whiteMaterial = new WhiteMaterial();
 
    // // 生成Mesh
    // auto meshWhite = new Mesh(whiteGeometry, whiteMaterial);
    // meshWhite->setPosition(glm::vec3(0.0f, 0.0f, 0.8f));
    // meshes.push_back(meshWhite);

    // mSpotLight = new SpotLight();
    // mSpotLight->mTargetDirection = glm::vec3(0.0f,0.0f,-1.0f);
    // mSpotLight->mInnerAngle = 30.0f;
    // mSpotLight->mOuterAngle = 45.0f;
    // mSpotLight->setPosition(meshWhite->getPosition());
 

    // mAmbientLight = new AmbientLight();
    // mAmbientLight->mColor = glm::vec3(0.15f);
    // // ---------------探照灯Demo--------------
    

    // // ---------------多光源混合Demo--------------

    // auto geometry = Geometry::createBox(1.0f);

    // // 创建一个Material
    // auto material = new PhongMaterial();
    // material->mShiness = 16.0f;
    // material->mDiffuse = new Texture("assets/textures/box.png", 0);
    // material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);

    // // 生成Mesh
    // auto mesh = new Mesh(geometry, material);
    // meshes.push_back(mesh);
    
    // auto whiteGeometry = Geometry::createSphere(0.1f);
    // auto whiteMaterial = new WhiteMaterial();
 
    // // 生成Mesh
    // auto meshWhite = new Mesh(whiteGeometry, whiteMaterial);
    // meshWhite->setPosition(glm::vec3(0.8f, 0.0f, 0.0f));
    // meshes.push_back(meshWhite);

    // mSpotLight = new SpotLight();
    // mSpotLight->mTargetDirection = glm::vec3(-1.0f,0.0f,0.0f);
    // mSpotLight->mInnerAngle = 30.0f;
    // mSpotLight->mOuterAngle = 45.0f;
    // mSpotLight->setPosition(meshWhite->getPosition());
 
    // mDirectionalLight = new DirectionalLight();
    // mDirectionalLight->mDirection = glm::vec3(1.0f);

    // mPointLight = new PointLight();
    // mPointLight->setPosition(glm::vec3(0.0f, 0.0f, 1.5f));
    // mPointLight->mSpecularIntensity = 0.5f;
    // mPointLight->mK2 = 0.017;
    // mPointLight->mK1 = 0.07;
    // mPointLight->mKc = 1.0;

    // mAmbientLight = new AmbientLight();
    // mAmbientLight->mColor = glm::vec3(0.15f);
    // // ---------------多光源混合Demo--------------


    // ---------------多光源混合Demo2--------------

    auto geometry = Geometry::createBox(1.0f);

    // 创建一个Material
    auto material = new PhongMaterial();
    material->mShiness = 16.0f;
    material->mDiffuse = new Texture("assets/textures/box.png", 0);
    material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);

    // 生成Mesh
    auto mesh = new Mesh(geometry, material);
    meshes.push_back(mesh);

    auto spGeometry = Geometry::createSphere(1.0f);
    auto spMesh = new Mesh(spGeometry, material);
    spMesh->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    meshes.push_back(spMesh);

    // 构建父子关系
    mesh->addChild(spMesh);
    
    auto whiteGeometry = Geometry::createSphere(0.1f);
    auto whiteMaterial = new WhiteMaterial();
 
    // 生成Mesh
    auto meshWhite = new Mesh(whiteGeometry, whiteMaterial);
    meshWhite->setPosition(glm::vec3(0.8f, 0.0f, 0.0f));
    meshes.push_back(meshWhite);

    mSpotLight = new SpotLight();
    mSpotLight->mTargetDirection = glm::vec3(-1.0f,0.0f,0.0f);
    mSpotLight->mInnerAngle = 30.0f;
    mSpotLight->mOuterAngle = 45.0f;
    mSpotLight->setPosition(meshWhite->getPosition());
 
    mDirectionalLight = new DirectionalLight();
    mDirectionalLight->mDirection = glm::vec3(-1.0f);

    auto pointLight1 = new PointLight();
    pointLight1->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    pointLight1->mColor = glm::vec3(1.0f, 0.0f, 0.0f);
    pointLight1->mK2 = 0.0;
    pointLight1->mK1 = 0.0;
    pointLight1->mKc = 1.0;
    mPointLights.push_back(pointLight1);

    auto pointLight2 = new PointLight();
    pointLight2->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    pointLight2->mColor = glm::vec3(0.0f, 1.0f, 0.0f);
    pointLight2->mK2 = 0.0;
    pointLight2->mK1 = 0.0;
    pointLight2->mKc = 1.0;
    mPointLights.push_back(pointLight2);

    auto pointLight3 = new PointLight();
    pointLight3->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    pointLight3->mColor = glm::vec3(0.0f, 0.0f, 1.0f);
    pointLight3->mK2 = 0.0;
    pointLight3->mK1 = 0.0;
    pointLight3->mKc = 1.0;
    mPointLights.push_back(pointLight3);

    auto pointLight4 = new PointLight();
    pointLight4->setPosition(glm::vec3(1.0f, 0.0f, -1.0f));
    pointLight4->mColor = glm::vec3(0.0f, 1.0f, 1.0f);
    pointLight4->mK2 = 0.0;
    pointLight4->mK1 = 0.0;
    pointLight4->mKc = 1.0;
    mPointLights.push_back(pointLight4);


    
    mAmbientLight = new AmbientLight();
    mAmbientLight->mColor = glm::vec3(0.15f);
    // ---------------多光源混合Demo2--------------
    
}

/**
 * @brief 封装后的场景绘制
 * 
 */
void prepareModel(){
    mRenderer = new Renderer();
    mInScreenScene = new Scene();

    // auto model = AssimpLoader::load("assets/fbx/test/test.fbx");
    // auto model = AssimpLoader::load("assets/fbx/Fist Fight B.fbx");
    auto model = AssimpLoader::load("assets/fbx/bag/backpack.obj");
    // auto model = AssimpLoader::load("assets/fbx/car/sportsCar.obj");
    model->setScale(glm::vec3(0.5));
    mInScreenScene->addChild(model);

    // auto geometry = Geometry::createBox(1.0f);
    // // 创建一个Material
    // auto material = new PhongMaterial();
    // material->mShiness = 16.0f;
    // material->mDiffuse = new Texture("assets/textures/box.png", 0);
    // material->mSpecularMask = new Texture("assets/textures/sp_mask.png", 1);
    // auto mesh = new Mesh(geometry, material);
     
    // auto spGeometry = Geometry::createSphere(1.0f);
    // auto spMesh = new Mesh(spGeometry, material);
    // spMesh->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    
    // //白色圆球
    // auto whiteGeometry = Geometry::createSphere(0.1f);
    // auto whiteMaterial = new WhiteMaterial();
    // auto meshWhite = new Mesh(whiteGeometry, whiteMaterial);
    // meshWhite->setPosition(glm::vec3(0.8f, 0.0f, 0.0f));

    // // 构建父子关系
    // mesh->addChild(spMesh);
    // mScene->addChild(mesh);
    // mScene->addChild(meshWhite);

 
    mDirectionalLight = new DirectionalLight();
    mDirectionalLight->mDirection = glm::vec3(-1.0f);
    mDirectionalLight->mSpecularIntensity = 1.0f;

    mAmbientLight = new AmbientLight();
    mAmbientLight->mColor = glm::vec3(0.1f);
}


/**
 * @brief Set the Model Blend object
 *  OIT算法 排序无关的透明度算法
 *  Depth Peeling 
 * @param obj 
 * @param blend 
 * @param opacity 
 */
void setModelBlend(Object* obj, bool blend, float opacity) {
    if (obj->getType() == ObjectType::Mesh) {
        Mesh* mesh = (Mesh*)obj;
        Material* mat = mesh->mMaterial;
        mat->mBlend = blend;
        mat->mOpacity = opacity;

        // 出现透明效果， 关闭深度写入
        mat->mDepthWrite = false;
    }
    
    auto children = obj->getChilderen();
    for (int i = 0; i < children.size(); i++) {
        setModelBlend(children[i], blend, opacity);
    }
}
// 检测
void prepareTest() {
    mRenderer = new Renderer();
    mInScreenScene = new Scene();
   
    // -------------- 深度写入Demo--------------
    // auto geometry = Geometry::createPlane(5.0f, 5.0f);
    // auto material = new PhongMaterial();
    // material->mDiffuse = new Texture("assets/textures/goku.jpg", 0);
    // Mesh* meshA = new Mesh(geometry, material);

    // auto materiaB = new PhongMaterial();
    // materiaB->mDiffuse = new Texture("assets/textures/box.png", 0);
    // // 深度写入关闭
    // materiaB->mDepthWrite = false;
    // Mesh* meshB = new Mesh(geometry, materiaB);
    // meshB->setPosition(glm::vec3(-1.0f));

    // auto materiaC = new PhongMaterial();
    // materiaC->mDiffuse = new Texture("assets/textures/wall.jpg", 0);
    // Mesh* meshC = new Mesh(geometry, materiaC);
    // meshC->setPosition(glm::vec3(-2.0f));

    // mScene->addChild(meshA);
    // mScene->addChild(meshB);
    // mScene->addChild(meshC);
    // -------------- 深度写入Demo--------------

    // // -------------- 深度值理论封装Demo--------------
    // auto geometry = Geometry::createPlane(5.0f, 5.0f);
    // auto material = new DepthMaterial();
    // Mesh* meshA = new Mesh(geometry, material);

    // Mesh* meshB = new Mesh(geometry, material);
    // meshB->setPosition(glm::vec3(-1.0f));

    // Mesh* meshC = new Mesh(geometry, material);
    // meshC->setPosition(glm::vec3(-2.0f));

    // mScene->addChild(meshA);
    // mScene->addChild(meshB);
    // mScene->addChild(meshC);
    // // -------------- 深度值理论封装Demo--------------

    // // -------------- ZFighting Demo--------------
    // auto geometry = Geometry::createPlane(5.0f, 5.0f);
    // auto material = new PhongMaterial();
    // material->mDiffuse = new Texture("assets/textures/goku.jpg", 0);
    // Mesh* meshA = new Mesh(geometry, material);

    // auto materiaC = new PhongMaterial();
    // materiaC->mDiffuse = new Texture("assets/textures/wall.jpg", 0);
    // materiaC->mPolygonOffset = true;
    // materiaC->mFactor = 1.0f;
    // materiaC->mUnit = 1.0f;
    // Mesh* meshC = new Mesh(geometry, materiaC);
    // meshC->setPosition(glm::vec3(2.0f, 0.5f, 0.0000001f));

    // mScene->addChild(meshA);
    // mScene->addChild(meshC);
    // // -------------- ZFighting--------------

    // // -------------- StencilTest 模版测试Demo--------------
    // auto geometryA = Geometry::createBox(1.0f);
    // auto materialA = new PhongMaterial();
    // materialA->mDiffuse = new Texture("assets/textures/box.png", 0);
    // materialA->mStencilTest = true;
    // // 控制模版测试后的结果写入
    // materialA->mSFail = GL_KEEP;
    // materialA->mZFail = GL_KEEP;
    // materialA->mSZPass = GL_REPLACE;
    // // 控制模版写入
    // materialA->mStencilMask = 0xFF;
    // //  控制模版测试条件
    // materialA->mStencilFunc = GL_ALWAYS;
    // materialA->mStencilRef = 1;
    // materialA->mStencilMask = 0xFF;


    // Mesh* meshA = new Mesh(geometryA, materialA);

    // auto materalABound = new WhiteMaterial();
    // // 关闭 深度测试，如果需要共享边界，边界盒子的深度检测要关闭
    // materalABound->mDepthTest = false;
    // materalABound->mStencilTest = true;
    // // 控制模版测试后的结果写入
    // materalABound->mSFail = GL_KEEP;
    // materalABound->mZFail = GL_KEEP;
    // materalABound->mSZPass = GL_KEEP;
    // // 控制模版写入
    // materalABound->mStencilMask = 0x00;
    // // 控制模版测试条件
    // materalABound->mStencilFunc = GL_NOTEQUAL;
    // materalABound->mStencilRef = 1;
    // materalABound->mStencilMask = 0xFF;

    // Mesh* meshABound = new Mesh(geometryA, materalABound);
    // meshABound->setPosition(meshA->getPosition());
    // meshABound->setScale(glm::vec3(1.2f));

    // mScene->addChild(meshA);
    // mScene->addChild(meshABound);

    // auto geometryB = Geometry::createBox(1.0f);
    // auto materialB = new PhongMaterial();
    // materialB->mStencilTest = true;
    // // 控制模版测试后的结果写入
    // materialB->mSFail = GL_KEEP;
    // materialB->mZFail = GL_KEEP;
    // materialB->mSZPass = GL_REPLACE;
    // // 控制模版写入
    // materialB->mStencilMask = 0xFF;
    // //  控制模版测试条件
    // materialB->mStencilFunc = GL_ALWAYS;
    // materialB->mStencilRef = 1;
    // materialB->mStencilMask = 0xFF;


    // materialB->mDiffuse = new Texture("assets/textures/wall.jpg", 0);
    // Mesh* meshB = new Mesh(geometryB, materialB);
    // meshB->setPosition(glm::vec3(0.5f, 0.5f, 0.5f));

    // auto materialBBound = new WhiteMaterial();
    // // 关闭 深度测试，如果需要共享边界，边界盒子的深度检测要关闭
    // materialBBound->mDepthTest = false;
    // materialBBound->mStencilTest = true;
    // // 控制模版测试后的结果写入
    // materialBBound->mSFail = GL_KEEP;
    // materialBBound->mZFail = GL_KEEP;
    // materialBBound->mSZPass = GL_KEEP;
    // // 控制模版写入
    // materialBBound->mStencilMask = 0x00;
    // // 控制模版测试条件
    // materialBBound->mStencilFunc = GL_NOTEQUAL;
    // materialBBound->mStencilRef = 1;
    // materialBBound->mStencilMask = 0xFF;

    // Mesh* meshBBound = new Mesh(geometryB, materialBBound);
    // meshBBound->setPosition(meshB->getPosition());
    // meshBBound->setScale(glm::vec3(1.2f));

    // mScene->addChild(meshB);
    // mScene->addChild(meshBBound);

    // // -------------- StencilTest 模版测试Demo--------------


    // -------------- 颜色混合 Demo--------------
    // auto materiaB = new PhongMaterial();
    // auto geometryB = Geometry::createBox(4.0f);
    // materiaB->mDiffuse = new Texture("assets/textures/box.png", 0);
    // Mesh* meshB = new Mesh(geometryB, materiaB);

    // auto geometry = Geometry::createPlane(6.0f, 6.0f);
    // auto material = new PhongMaterial();
    // material->mDiffuse = new Texture("assets/textures/window.png", 0);
    // Mesh* meshA = new Mesh(geometry, material);
    // meshA->setPosition(glm::vec3(0.0, 0.0, 4.0));

    // mScene->addChild(meshB);
    // mScene->addChild(meshA);



    // auto materiaB = new PhongMaterial();
    // auto geometryB = Geometry::createBox(4.0f);
    // materiaB->mDiffuse = new Texture("assets/textures/window.png", 0);
    // materiaB->mBlend = true;  
    // // 半透明物体时，需要把深度写入关闭
    // materiaB->mDepthWrite = false;
    // materiaB->mOpacity = 0.3;

    // Mesh* meshB = new Mesh(geometryB, materiaB);
    // mScene->addChild(meshB);

    // // 半透明物体时，需要把深度写入关闭
    // auto model = AssimpLoader::load("assets/fbx/bag/backpack.obj");
    // // auto model = AssimpLoader::load("assets/fbx/car/sportsCar.obj");
    // setModelBlend(model, true, 0.2);
    // model->setScale(glm::vec3(0.5));
    // mScene->addChild(model);


    // // 不透明物体先绘制，透明物体按摄像机距离远近，从后往前绘制
    // auto planeGeo = Geometry::createPlane(5.0f, 5.0f);
    // auto planeMat = new PhongMaterial();
    // planeMat->mDiffuse = new Texture("assets/textures/box.png", 0);
    // Mesh* pMesh = new Mesh(planeGeo, planeMat);
    // pMesh->setPosition(glm::vec3(0.0f, 0.0f, 6.0f));
    // mScene->addChild(pMesh);

    // auto planeGeometry = Geometry::createPlane(10.0f, 10.0f);
    // auto planeMaterial = new PhongMaterial();
    // planeMaterial->mDiffuse = new Texture("assets/textures/wall.jpg", 0);
    // planeMaterial->mBlend = true;
    // planeMaterial->mOpacity = 0.4;

    // Mesh* planeMesh = new Mesh(planeGeometry, planeMaterial);
    // planeMesh->setPosition(glm::vec3(0.0f, 0.0f, -6.0f));
    // mScene->addChild(planeMesh);
    

    // -------------- 颜色混合 Demo--------------

    // // -------------- 透明度蒙版 Demo--------------

    // auto planeGeo = Geometry::createPlane(5.0f, 5.0f);
    // auto planeMat = new OpacityMaterial();
    // planeMat->mDiffuse = new Texture("assets/textures/grass.jpg", 0);
    // planeMat->mOpacityMask = new Texture("assets/textures/grassMask.png", 1);
    // planeMat->mBlend = true;
    // Mesh* pMesh = new Mesh(planeGeo, planeMat);
    // mScene->addChild(pMesh);

    // // -------------- 透明度蒙版 Demo--------------


    // -------------- 面剔除 Demo--------------

    auto planeGeo = Geometry::createPlane(5.0f, 5.0f);
    auto planeMat = new PhongMaterial();
    planeMat->mDiffuse = new Texture("assets/textures/grass.jpg", 0);

    planeMat->mFaceCulling = true;
    planeMat->mFrontFace = GL_CCW;
    planeMat->mCullFace = GL_BACK;

    Mesh* pMesh = new Mesh(planeGeo, planeMat);
    mInScreenScene->addChild(pMesh);

    // -------------- 面剔除 Demo--------------
    
    mDirectionalLight = new DirectionalLight();
    mDirectionalLight->mDirection = glm::vec3(-1.0f);
    mDirectionalLight->mSpecularIntensity = 0.3f;

    mAmbientLight = new AmbientLight();
    mAmbientLight->mColor = glm::vec3(0.1f);
}


void prepareFBO() {
    // 1 生成fbo 对象并绑定
    GLCall(glGenFramebuffers(1, &mFbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFbo));

    //2 生成颜色附件 加入到VBO中
    Application *app = Application::getInstance();
    mColorAttachment = new Texture(app->getWidth(), app->getHeight(), 0);
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->getTexture(), 0));

    //3 生成depth Stencil附件加入到VBO
    unsigned int depthStencil;
    GLCall(glGenTextures(1, &depthStencil));
    GLCall(glBindTexture(GL_TEXTURE_2D, depthStencil));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, app->getWidth(), app->getHeight(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));

    // 设置纹理采样方式
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencil, 0));

    //  检查当前构建的FBO是否完整
    GLCall(unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "error frameBuffer is not completed" << std::endl;
    }

    // 解绑为0号单元
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void prepareFrameBuffer() {
    mRenderer = new Renderer();
    mInScreenScene = new Scene();
    mOffScreenScene = new Scene();
    Application *app = Application::getInstance();

    // -------------- 帧缓冲 Demo--------------
    frameBuffer = new FrameBuffer(app->getWidth(), app->getHeight());
    
    // 离屏渲染的box
    auto boxGeo = Geometry::createBox(1.0f);
    auto boxMat = new PhongMaterial();
    boxMat->mDiffuse = new Texture("assets/textures/wall.jpg", 0);
    Mesh* boxMesh = new Mesh(boxGeo, boxMat);
    mOffScreenScene->addChild(boxMesh);

    // 贴到屏幕上的矩形
    auto planeGeo = Geometry::createScreenPlane();
    auto planeMat = new ScreenMaterial();
    // 离屏的pass 贴到 屏幕上pass
    // planeMat->mScreenTexture = new Texture("assets/textures/goku.jpg", 0);
    planeMat->mScreenTexture = frameBuffer->mColorAttachment;
    Mesh* pMesh = new Mesh(planeGeo, planeMat);
    mInScreenScene->addChild(pMesh);
    // -------------- 帧缓冲 Demo--------------
    
    mDirectionalLight = new DirectionalLight();
    mDirectionalLight->mDirection = glm::vec3(-1.0f);
    mDirectionalLight->mSpecularIntensity = 0.3f;

    mAmbientLight = new AmbientLight();
    mAmbientLight->mColor = glm::vec3(0.2f);

}

void prepareCubeBox() {
    mRenderer = new Renderer();
    mInScreenScene = new Scene();

    // // -------------- 天空盒 Demo--------------
    // std::vector<std::string> paths = {
    //     "assets/textures/skybox/right.jpg",
    //     "assets/textures/skybox/left.jpg",
    //     "assets/textures/skybox/top.jpg",
    //     "assets/textures/skybox/bottom.jpg",
    //     "assets/textures/skybox/back.jpg",
    //     "assets/textures/skybox/front.jpg"
    // };

    // Texture* envBoxTex = new Texture(paths, 0);

    // // 可以让天空盒的深度永远为1，让它永远深度值为1
    // // 将剪裁空间左边系的zc恒等于wc,输出zndc恒为1
    // auto boxGeo = Geometry::createBox(1.0f);
    // auto boxMat = new CubeMaterial();
    // boxMat->mDiffuseTexture = envBoxTex;
    // // boxMat->mDepthWrite = false;
    // Mesh* boxMesh = new Mesh(boxGeo, boxMat);
    // mInScreenScene->addChild(boxMesh);

    // // cubemap 环境贴图
    // Texture* envSphereTex = new Texture(paths, 1);
    // auto sphereGeo = Geometry::createSphere(4.0f);
    // auto sphereMat = new CubeEnvMaterial();
    // sphereMat->mDiffuse = new Texture("assets/textures/earth.jpg", 0);
    // sphereMat->mEnv = envSphereTex;
    // Mesh* pMesh = new Mesh(sphereGeo, sphereMat);
    // mInScreenScene->addChild(pMesh);


    // // -------------- 天空盒 Demo--------------


     // -------------- 天空盒 球形贴图Demo--------------
    // 可以让天空盒的深度永远为1，让它永远深度值为1
    // 将剪裁空间左边系的zc恒等于wc,输出zndc恒为1
    auto boxGeo = Geometry::createBox(1.0f);
    auto boxMat = new CubeMaterial();
    // 设置纹理过滤方式
    boxMat->mDiffuseTexture = new Texture("assets/textures/bk.jpg", 0);;
    // boxMat->mDepthWrite = false;
    Mesh* boxMesh = new Mesh(boxGeo, boxMat);
    mInScreenScene->addChild(boxMesh);

    // cubemap 环境贴图
    auto sphereGeo = Geometry::createSphere(4.0f);
    auto sphereMat = new PhongMaterial();
    sphereMat->mDiffuse = new Texture("assets/textures/earth.jpg", 0);
    Mesh* pMesh = new Mesh(sphereGeo, sphereMat);
    mInScreenScene->addChild(pMesh);


    // -------------- 天空盒 Demo--------------
    
    mDirectionalLight = new DirectionalLight();
    mDirectionalLight->mDirection = glm::vec3(-1.0f);
    mDirectionalLight->mSpecularIntensity = 0.3f;

    mAmbientLight = new AmbientLight();
    mAmbientLight->mColor = glm::vec3(0.2f);
}

void prepareInstanceGeo() {
    mRenderer = new Renderer();
    mInScreenScene = new Scene();
    
    // -------------- 实例绘制 Demo--------------
    // 可以让天空盒的深度永远为1，让它永远深度值为1
    // 将剪裁空间左边系的zc恒等于wc,输出zndc恒为1
    auto boxGeo = Geometry::createBox(1.0f);
    auto boxMat = new CubeMaterial();
    // 设置纹理过滤方式
    boxMat->mDiffuseTexture = new Texture("assets/textures/bk.jpg", 0);;
    Mesh* boxMesh = new Mesh(boxGeo, boxMat);
    mInScreenScene->addChild(boxMesh);

    // cubemap 环境贴图
    auto sphereGeo = Geometry::createSphere(4.0f);
    auto sphereMat = new PhongInstanceMaterial();
    sphereMat->mDiffuse = new Texture("assets/textures/earth.jpg", 0);
    InstancedMesh* instanceMesh = new InstancedMesh(sphereGeo, sphereMat, 2);

    glm::mat4 transformA = glm::mat4(1.0f);
    glm::mat4 transformB = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f));
    instanceMesh->mInstanceMatrices.push_back(transformA);
    instanceMesh->mInstanceMatrices.push_back(transformB);
    instanceMesh->updateMatrices();

    mInScreenScene->addChild(instanceMesh);


    // -------------- 实例绘制 Demo--------------
    
    mDirectionalLight = new DirectionalLight();
    mDirectionalLight->mDirection = glm::vec3(-1.0f);
    mDirectionalLight->mSpecularIntensity = 0.3f;

    mAmbientLight = new AmbientLight();
    mAmbientLight->mColor = glm::vec3(0.2f);
}

void setInstanceMatrix(Object* obj, int index, glm::mat4 matrix) {
    if (obj->getType() == ObjectType::InstancedMesh) {
        InstancedMesh* im = (InstancedMesh*)obj;
        im->mInstanceMatrices[index] = matrix;
    }
    auto children = obj->getChilderen();
    for (int i = 0; i < children.size(); i++) {
        setInstanceMatrix(children[i], index, matrix);
    }
}


void updateInstanceMatrix(Object* obj) {
    if (obj->getType() == ObjectType::InstancedMesh) {
        InstancedMesh* im = (InstancedMesh*)obj;
        im->updateMatrices();
    }
    auto children = obj->getChilderen();
    for (int i = 0; i < children.size(); i++) {
        updateInstanceMatrix(children[i]);
    }
}

void setInstanceMaterial(Object* obj, Material* material) {
    if (obj->getType() == ObjectType::InstancedMesh) {
        InstancedMesh* im = (InstancedMesh*)obj;
        im->mMaterial = material;
    }
    auto children = obj->getChilderen();
    for (int i = 0; i < children.size(); i++) {
        setInstanceMaterial(children[i], material);
    }
}

void prepareGrassDemo() {
    mRenderer = new Renderer();
    mInScreenScene = new Scene();
    
    // -------------- Grass Demo--------------
    // 可以让天空盒的深度永远为1，让它永远深度值为1
    // 将剪裁空间左边系的zc恒等于wc,输出zndc恒为1
    auto boxGeo = Geometry::createBox(1.0f);
    auto boxMat = new CubeMaterial();
    // 设置纹理过滤方式
    boxMat->mDiffuseTexture = new Texture("assets/textures/bk.jpg", 0);
    Mesh* boxMesh = new Mesh(boxGeo, boxMat);
    mInScreenScene->addChild(boxMesh);


    // auto transformA = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // auto transformB = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // setInstanceMatrix(grassModel, 0, transformA);
    // setInstanceMatrix(grassModel, 1, transformB);
    // updateInstanceMatrix(grassModel);
    
    int rNum = 30;
    int cNum = 30;
    auto grassModel = AssimpInstanceLoader::load("assets/fbx/grassNew.obj", rNum * cNum);

    glm::mat4 transform;
    glm::mat4 translate;
    glm::mat4 rotate;
    srand(glfwGetTime());
    for (int r = 0; r < rNum; r++) {
		for (int c = 0; c < cNum; c++) {
			translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.2 * r, 0.0f,0.2 * c));
			rotate = glm::rotate(glm::mat4(1.0f), glm::radians((float)(rand() % 90)), glm::vec3(0.0, 1.0, 0.0));
			transform = translate  * rotate;
			setInstanceMatrix(grassModel, r * cNum + c, transform);
		}
	}
    updateInstanceMatrix(grassModel);

    grassMaterial = new GrassInstanceMaterial();
    grassMaterial->mDiffuse = new Texture("assets/textures/GRASS.PNG", 0); 
    grassMaterial->mOpacityMask = new Texture("assets/textures/grassMask.png", 1); 
    grassMaterial->mCloudMask = new Texture("assets/textures/CLOUD.PNG", 2); 
    grassMaterial->mBlend = true;
    grassMaterial->mDepthWrite = false;
    setInstanceMaterial(grassModel, grassMaterial);

    mInScreenScene->addChild(grassModel);


     auto model = AssimpLoader::load("assets/fbx/house.fbx");
    // auto model = AssimpLoader::load("assets/fbx/bag/backpack.obj");
    // // auto model = AssimpLoader::load("assets/fbx/car/sportsCar.obj");
    model->setScale(glm::vec3(0.5));
    model->setPosition(glm::vec3(rNum * 0.2f / 2.0f, 0.4, cNum * 0.2f / 2.0f));
    mInScreenScene->addChild(model);


    // -------------- Grass Demo--------------
    
    mDirectionalLight = new DirectionalLight();
    mDirectionalLight->mDirection = glm::vec3(-1.0f);
    mDirectionalLight->mSpecularIntensity = 0.3f;

    mAmbientLight = new AmbientLight();
    mAmbientLight->mColor = glm::vec3(0.2f);
}

void lightTransform()
{
    float xPos = glm::sin(glfwGetTime()) + 1.5f;
    meshes[1]->setPosition(glm::vec3(xPos, 0.0f, 0.0f));
    mPointLight->setPosition(glm::vec3(xPos, 0.0f, 0.0f));
}

void initImGui()
{
    Application *app = Application::getInstance();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(app->getWindow(), true);
    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

}

void renderImGui()
{
    Application *app = Application::getInstance();

    // 开启GUI渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 决定当前GUI有哪些控件
    ImGui::Begin("GrassMaterialEditor");
    ImGui::Text("GrassColor");
    ImGui::SliderFloat("UVScale", &grassMaterial->mUvScale, 0.0, 100.0);
    ImGui::InputFloat("Brightness", &grassMaterial->mBrightness);
    ImGui::Text("Wind");
    ImGui::InputFloat("WindScale", &grassMaterial->mWindScale);
    ImGui::InputFloat("PhaseScale", &grassMaterial->mPhaseScale);
    ImGui::ColorEdit3("WindDirection", (float*)&grassMaterial->mWindDirection);
    ImGui::Text("Cloud");
    ImGui::ColorEdit3("CloudWhiteColor", (float*)&grassMaterial->mCloudWhiteColor);
    ImGui::ColorEdit3("CloudBlackColor", (float*)&grassMaterial->mCloudBlackColor);
    ImGui::SliderFloat("CloudUvScale", &grassMaterial->mCloudUvScale, 0.0, 100.0);
    ImGui::InputFloat("CloudSpeed", &grassMaterial->mCloudSpeed);
    ImGui::SliderFloat("CloudLerp", &grassMaterial->mCloudLerp, 0.0, 1.0);
    ImGui::End();

    // 执行UI渲染
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(app->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void destroyImGui() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void renderFBO() {
    cameraControl->update();
    mRenderer->setClearColor(mClearColor);

    //pass01 (离屏渲染) 将 pass01 渲染到colorAttachment上，新的fbo上
     mRenderer->render(mOffScreenScene, camera, mDirectionalLight, mAmbientLight, frameBuffer->mFbo);

    //pass02 （屏幕渲染）将colorAttachment作为纹理，绘制到整个屏幕上
     mRenderer->render(mInScreenScene, camera, mDirectionalLight, mAmbientLight);
}

void renderCommon() {
    cameraControl->update();

    // meshes[0]->rotateZ(2.0f);
    // meshes[1]->rotateY(4.0f);

    mRenderer->setClearColor(mClearColor);

    // lightTransform();
    // mRenderer->render(meshes, camera, mPointLight, mAmbientLight);

    // mRenderer->render(meshes, camera, mDirectionalLight, mAmbientLight);

    // mRenderer->render(meshes, camera, mSpotLight, mAmbientLight);
    // mRenderer->render(meshes, camera, mDirectionalLight, mSpotLight, mPointLight, mAmbientLight);

    // mRenderer->render(meshes, camera, mDirectionalLight, mSpotLight, mPointLights, mAmbientLight);

    mRenderer->render(mInScreenScene, camera, mDirectionalLight, mAmbientLight);
}

int main(void)
{
    Application *app = Application::getInstance();
    if (!app->init(800, 600))
    {
        return -1;
    }

    app->setResizeCallBack(OnResize);
    app->setKeyBoardCallback(OnKeyBorad);
    app->setMouseCallback(OnMouseCallback);
    app->setCursorCallback(onCursorCallback);
    app->setScorllCallback(onScorllCallback);

    GLCall(glViewport(0, 0, app->getWidth(), app->getHeight()));

    initImGui();

    // prepare()
    // prepareModel();
    // prepareTest();
    // prepareFBO();
    // prepareFrameBuffer();
    // prepareCubeBox();
    //  prepareInstanceGeo();
    prepareGrassDemo();
    prepareCamera();

    while (app->update())
    {
        // renderFBO();
        renderCommon();
        renderImGui();
    }
    app->destroy();
    destroyImGui();

    return 0;
}