#ifndef TEXTURE_H
#define TEXTURE_H
#include "core.h"
#include <string>
#include <map>
#include <vector>


class Texture
{
public:
    Texture();
    Texture(const std::string& path, unsigned int unit);
    Texture(const std::vector<std::string>& paths, unsigned int unit);
    // 从内存中加载数据
    Texture(unsigned char* picData, uint32_t width, uint32_t height, unsigned int unit);
    Texture(unsigned int width, unsigned int height, unsigned int unit);
    ~Texture();

    static Texture* createTexture(const std::string& path, unsigned int unit);
    static Texture* createTexture(const std::string& path, unsigned char* data, uint32_t width, uint32_t height, unsigned int unit);
    static Texture* createColorAttachment(unsigned int width, unsigned int height, unsigned int unit);
    static Texture* createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit);
    void bind();
    void setUnit(unsigned int unit) {mUnit = unit;}

    int getWidth() const {return mWidth;}
    int getHeight() const {return mHeight;}
    GLuint getTexture() const {return mTexture;}


private:
    GLuint mTexture{0};
    int mWidth{0};
    int mHeight{0};
    unsigned int mUnit{0};
    unsigned int mTextureTarget {GL_TEXTURE_2D};

    //注意： 静态||属于类的，不属于某个对象
    static std::map<std::string, Texture*> mTextureCache;
};

#endif