#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Application *Application::mInstance = nullptr;

Application *Application::getInstance()
{
    if (mInstance == nullptr)
    {
        mInstance = new Application();
    }
    return mInstance;
}

Application::Application()
{
}

bool Application::init(const int &width, const int &height)
{
    mWidth = width;
    mHeight = height;

    if (!glfwInit())
        return false;

    // 设置OpenGL核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // 设置OpenGL主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */

    mWindow = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!mWindow)
    {
        glfwTerminate();
        return false;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(mWindow);
    // 窗口大小变化事件
    glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);
    // 键盘响应事件
    glfwSetKeyCallback(mWindow, keyBoardCallback);
    // 鼠标按下
    glfwSetMouseButtonCallback(mWindow, mouseCallback);

    glfwSetCursorPosCallback(mWindow, cursorCallback);

    glfwSetScrollCallback(mWindow, scorllCallback);

    // this是当前全局唯一的Application对象
    glfwSetWindowUserPointer(mWindow, this);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewinit error" << std::endl;
        return false;
    }

    return true;
}

bool Application::update()
{
    if (glfwWindowShouldClose(mWindow))
    {
        return false;
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(mWindow);

    /* Poll for and process events */
    glfwPollEvents();

    return true;
}

void Application::destroy()
{
    glfwTerminate();
}

Application::~Application()
{
}

void Application::frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    Application *self = (Application *)glfwGetWindowUserPointer(window);
    if (self->mResizeCallback != nullptr)
    {
        self->mResizeCallback(width, height);
    }
    // if (Application::getInstance()->mResizeCallback != nullptr) {
    //     Application::getInstance()->mResizeCallback(width, height);
    // }
}

void Application::keyBoardCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Application *self = (Application *)glfwGetWindowUserPointer(window);
    if (self->mKeyBoardCallback != nullptr)
    {
        self->mKeyBoardCallback(key, action, mods);
    }
}

void Application::mouseCallback(GLFWwindow *window, int button, int action, int mods)
{
    // std::cout << "鼠标点击 " << "按键是 " << button << " 松开是否按下" << action << " " << mods << std::endl;
    Application *self = (Application *)glfwGetWindowUserPointer(window);
    if (self->mMouseCallback != nullptr)
    {
        self->mMouseCallback(button, action, mods);
    }
}
void Application::cursorCallback(GLFWwindow *window, double xpos, double ypos)
{
    // std::cout << "鼠标移动 " << xpos << " " << ypos << std::endl;
    Application *self = (Application *)glfwGetWindowUserPointer(window);
    if (self->mCursorCallback != nullptr)
    {
        self->mCursorCallback(xpos, ypos);
    }
}

void Application::scorllCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    Application *self = (Application *)glfwGetWindowUserPointer(window);
    if (self->mScorllCallback != nullptr)
    {
        self->mScorllCallback(xOffset, yOffset);
    }
}

void Application::getCursorPosition(double *posX, double *posY)
{
    glfwGetCursorPos(mWindow, posX, posY);
}
