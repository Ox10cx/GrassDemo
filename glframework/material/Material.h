#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core.h"

enum class MaterialType
{
    PhongMaterial,
    WhiteMaterial,
    ColorMaterial,
    ColorWithTextureMaterial,
    DepthMaterial,
    OpacityMaterial,
    ScreenMaterial,
    CubeMaterial,
    CubeEnvMaterial,
    PhongInstanceMaterial,
    GrassInstanceMaterial,
};

class Material
{
public:
    Material();
    ~Material();

public:
    MaterialType mType;

    // 深度检测相关
    bool mDepthTest {true};
    GLenum mDepthFunc {GL_LEQUAL};
    // 是否要深度写入
    bool mDepthWrite {true};

    // PolyGon_OFFSET 相关
    bool mPolygonOffset {false};
    unsigned int mPolygonOffsetType {GL_POLYGON_OFFSET_FILL};
    float mFactor  {0.0f};
    float mUnit    {0.0f};

    // stencil 模版测试相关
    bool mStencilTest {false};
    unsigned int mSFail {GL_KEEP};  // 模版测试失败怎么办
    unsigned int mZFail {GL_KEEP};  // 模版测试失败，深度检测通过
    unsigned int mSZPass {GL_KEEP}; // 模版 + 深度测试通过
    unsigned int mStencilMask {0xFF}; // 用于控制模版写入

    unsigned int mStencilFunc {GL_ALWAYS};
    unsigned int mStencilRef {0};
    unsigned int mStencilFuncMask {0xFF};

    // 颜色混合相关
    bool  mBlend {false};
    unsigned int mSFactor {GL_SRC_ALPHA};
    unsigned int mDFactor {GL_ONE_MINUS_SRC_ALPHA};
    // 整体透明度 越大越不透明
    float mOpacity {1.0f};

    // 面剔除相关
    bool mFaceCulling {false};
    unsigned int mFrontFace{GL_CCW};
    unsigned int mCullFace{GL_BACK};


};

#endif