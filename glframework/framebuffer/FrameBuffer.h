#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "../core.h"
#include "../Texture.h"

class FrameBuffer
{
public:
    FrameBuffer(unsigned int width, unsigned int height);
    ~FrameBuffer();

public:
   unsigned int mWidth {0};
   unsigned int mHeight {0};

   unsigned int mFbo {0};
   Texture* mColorAttachment{nullptr};
   Texture* mDepthStencilAttachment{nullptr};
   
};

#endif