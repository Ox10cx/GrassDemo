#include "FrameBuffer.h"
#include<iostream>
#include "../GLHead.h"


FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
    mWidth = width;
	mHeight = height;

	//1 生成fbo 对象并绑定
	GLCall(glGenFramebuffers(1, &mFbo));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mFbo));


	//2 生成颜色附件 加入到VBO中
	mColorAttachment = Texture::createColorAttachment(mWidth, mHeight, 0);
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment->getTexture(), 0));

	//3 生成depth Stencil附件加入到VBO
	mDepthStencilAttachment = Texture::createDepthStencilAttachment(mWidth, mHeight, 0);
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthStencilAttachment->getTexture(), 0));

	 //  检查当前构建的FBO是否完整
    GLCall(unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "error frameBuffer is not completed" << std::endl;
    }

	// 解绑FrameBuffer
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
FrameBuffer::~FrameBuffer()
{
    if (mFbo) {
        GLCall(glDeleteFramebuffers(1, &mFbo));
    }
    if (mColorAttachment) {
        delete mColorAttachment;
    }
    if (mDepthStencilAttachment) {
        delete mDepthStencilAttachment;
    }
}