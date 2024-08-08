#include "Texture.h"
#include "../application/stb_image.h"
#include "../GLHead.h"
#include <iostream>

std::map<std::string, Texture*> Texture::mTextureCache{};

Texture::Texture()
{

}
Texture::Texture(const std::string &path, unsigned int unit)
{
    // 1 stbImage 读取图片
    int channels;

    // 反转y轴
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);

    // 2 生成纹理并且激活单元绑定
    GLCall(glGenTextures(1, &mTexture));
    GLCall(glActiveTexture(GL_TEXTURE0 + unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, mTexture));

    // 3 传输纹理数据, 开辟显存
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

    // // 自动生成mipmap
    // GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    // 手动生成mipmap
    // // 遍历每个mipmap的层级，为每个级别的mipmap填充数据
    // int width = mWidth, height = mHeight;
    // for (int level = 0; true; level++)
    // {
    //     // 传输当前对应级别的mipmap数据传输给GPU端
    //     GLCall(glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

    //     // 退出循环
    //     if (width == 1 && height == 1)
    //     {
    //         break;
    //     }

    //     // 计算下一次宽度和高度
    //     width = width > 1 ? width / 2 : 1;
    //     height = height > 1 ? height / 2 : 1;
    // }

    // 释放数据
    stbi_image_free(data);

    // 4 设置纹理过滤方式
    //  需要像素 > 图片像素
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // // // 需要像素 < 图片像素 //
    // // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    // //  MIPMAP_LINEAR 在两层mipmap中采用线性插值
    // //  GL_NEAREST 在单个mipmap上最临进采样
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    // 5 设置纹理包裹方式
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    mUnit = unit;
}

Texture::Texture(const std::vector<std::string>& paths, unsigned int unit)
{
    mUnit = unit;
    mTextureTarget = GL_TEXTURE_CUBE_MAP;

    // 不需要反转y轴
    stbi_set_flip_vertically_on_load(false);

    //1 创建CubeMap对象
    GLCall(glGenTextures(1, &mTexture));
    GLCall(glActiveTexture(GL_TEXTURE0 + unit));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture));

    //2 循环读取六张贴图，并且放置到CubeMap六个GPU空间
    int channels;
    int width, height;
    unsigned char *data = nullptr;

    // path顺序 右 左 上 下 后 前 （+X -X +Y -Y +Z -Z)
    for (int i = 0; i < paths.size(); i++) {
        data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (data != nullptr) {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
            stbi_image_free(data);
        } else {
            std::cout << "Error:CubeMap Texture failed to loaded at path" << paths[i] << std::endl;
            stbi_image_free(data);
        }
    }

    //3 设置纹理参数
    //  设置纹理过滤方式
    //  需要像素 > 图片像素
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // // 需要像素 < 图片像素 //
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    //  MIPMAP_LINEAR 在两层mipmap中采用线性插值
    //  GL_NEAREST 在单个mipmap上最临进采样
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    // 5 设置纹理包裹方式
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT));


}

Texture::Texture(unsigned char* picData, uint32_t width, uint32_t height, unsigned int unit)
{
    // 1 stbImage 读取图片
    int channels;

    // 反转y轴
    stbi_set_flip_vertically_on_load(true);

    // 计算整张图片大小
    // assimp 规定 内嵌纹理是png/JPG压缩格式的话， height = 0, width 代表整张图片的大小
    uint32_t dataSize = 0;
    if (height == 0)
    {
        dataSize = width;
    }
    else
    {
        // todo 默认数据格式是rgba, 也有可能不是, 这里要判断图片格式
        dataSize = width * height * 4;
    }

    // 1 从内存中加载纹理数据
    unsigned char* data = stbi_load_from_memory(picData, dataSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);

    // 2 生成纹理并且激活单元绑定
    GLCall(glGenTextures(1, &mTexture));
    GLCall(glActiveTexture(GL_TEXTURE0 + unit));
    GLCall(glBindTexture(GL_TEXTURE_2D, mTexture));

    // 3 传输纹理数据, 开辟显存
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

    // 自动生成mipmap
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    // 手动生成mipmap
    // // 遍历每个mipmap的层级，为每个级别的mipmap填充数据
    // int width = mWidth, height = mHeight;
    // for (int level = 0; true; level++)
    // {
    //     // 传输当前对应级别的mipmap数据传输给GPU端
    //     GLCall(glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

    //     // 退出循环
    //     if (width == 1 && height == 1)
    //     {
    //         break;
    //     }

    //     // 计算下一次宽度和高度
    //     width = width > 1 ? width / 2 : 1;
    //     height = height > 1 ? height / 2 : 1;
    // }

    // 释放数据
    stbi_image_free(data);

    // 4 设置纹理过滤方式
    //  需要像素 > 图片像素
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // // 需要像素 < 图片像素 //
    // GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    //  MIPMAP_LINEAR 在两层mipmap中采用线性插值
    //  GL_NEAREST 在单个mipmap上最临进采样
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));

    // 5 设置纹理包裹方式
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

    mUnit = unit;
}

Texture::Texture(unsigned int width, unsigned int height, unsigned int unit)
{
    mWidth = width;
	mHeight = height;
	mUnit = unit;

	GLCall(glGenTextures(1, &mTexture));
	GLCall(glActiveTexture(GL_TEXTURE0 + mUnit));
	GLCall(glBindTexture(GL_TEXTURE_2D, mTexture));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));


}

Texture::~Texture()
{
    if (mTexture != 0)
    {
        glDeleteTextures(1, &mTexture);
    }
}

Texture* Texture::createColorAttachment(unsigned int width, unsigned int height, unsigned int unit)
{
    return new Texture(width, height, unit);
}

Texture* Texture::createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit)
{
    Texture* dsTex = new Texture();

	unsigned int depthStencil;
	GLCall(glGenTextures(1, &depthStencil));
	GLCall(glBindTexture(GL_TEXTURE_2D, depthStencil));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	dsTex->mTexture = depthStencil;
	dsTex->mWidth = width;
	dsTex->mHeight = height;
	dsTex->mUnit = unit;

	return dsTex;
}


Texture *Texture::createTexture(const std::string &path, unsigned int unit)
{
    // 检查是否缓存过本路径对应的纹理
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end())
    {
        // 找到了
        return iter->second;
    }

    // 没有则生成
    auto texture = new Texture(path, unit);
    mTextureCache[path] = texture;
    return texture;
}

Texture *Texture::createTexture(const std::string &path, unsigned char *data,
                                uint32_t width, uint32_t height, unsigned int unit)
{
        // 检查是否缓存过本路径对应的纹理
    auto iter = mTextureCache.find(path);
    if (iter != mTextureCache.end())
    {
        // 找到了
        return iter->second;
    }

    // 没有则生成
    auto texture = new Texture(data, width, height, unit);
    mTextureCache[path] = texture;
    return texture;

}

void Texture::bind()
{
    // 先切换纹理单元，在切换纹理对象
    GLCall(glActiveTexture(GL_TEXTURE0 + mUnit));
    GLCall(glBindTexture(mTextureTarget, mTexture));
}