#ifndef APPLICAITON_H
#define APPLICAITON_H
#include <cstdint>

class GLFWwindow;

using ResizeCallback = void (*)(int width, int height);
using KeyBoardCallback = void (*)(int key, int action, int mods);
using MouseCallback = void (*)(int button, int action, int mods);
using CursorCallback = void (*)(double xpos, double ypos);
using ScorllCallback = void (*)(double xoffset, double yoffset);

class Application
{
public:
    ~Application();
    static Application *getInstance();


    bool init(const int &width = 800, const int &height = 600);

    bool update();

    void destroy();

    uint32_t getWidth() const { return mWidth * 2; }
    uint32_t getHeight() const { return mHeight * 2; }

    void setResizeCallBack(ResizeCallback callback) { mResizeCallback = callback; }

    void setKeyBoardCallback(KeyBoardCallback callback) { mKeyBoardCallback = callback; }

    void setMouseCallback(MouseCallback callback) {mMouseCallback  = callback;}

    void setCursorCallback(CursorCallback callback) {mCursorCallback = callback;}

    void setScorllCallback(ScorllCallback callback) {mScorllCallback = callback;}

    void getCursorPosition(double* posX, double* posY);

    GLFWwindow* getWindow() {return mWindow;}

private:
    // C++ 类内函数指针
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    static void keyBoardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorCallback(GLFWwindow *window, double xpos, double ypos);
    static void scorllCallback(GLFWwindow *window, double xOffset, double yOffset);


private:
    static Application *mInstance;
    Application();

    uint32_t mWidth{0};
    uint32_t mHeight{0};

    GLFWwindow *mWindow{nullptr};

    ResizeCallback mResizeCallback{nullptr};
    KeyBoardCallback mKeyBoardCallback{nullptr};
    MouseCallback mMouseCallback{nullptr};
    CursorCallback mCursorCallback{nullptr};
    ScorllCallback mScorllCallback {nullptr};
};

#endif