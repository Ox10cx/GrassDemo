#include "Object.h"
#include <iostream>

Object::Object()
{
    mType = ObjectType::Object;
}
Object::~Object()
{
}

void Object::addChild(Object *object)
{
    auto iter = std::find(mChildren.begin(), mChildren.end(), object);
    if (iter != mChildren.end())
    {
        std::cerr << "Duplite Child added" << std::endl;
        return;
    }
    mChildren.push_back(object);
    object->mParent = this;
}

std::vector<Object *> Object::getChilderen()
{
    return mChildren;
}

Object *Object::getParent()
{
    return mParent;
}

void Object::setPosition(glm::vec3 pos)
{
    mPosition = pos;
}
void Object::rotateX(float angle)
{
    mAngleX += angle;
}
void Object::rotateY(float angle)
{
    mAngleY += angle;
}
void Object::rotateZ(float angle)
{
    mAngleZ += angle;
}

void Object::setAngleX(float angle)
{
    mAngleX = angle;
}
void Object::setAngleY(float angle)
{
    mAngleY = angle;
}
void Object::setAngleZ(float angle)
{
    mAngleZ = angle;
}

void Object::setScale(glm::vec3 scale)
{
    mScale = scale;
}

glm::mat4 Object::getModelMatrix() const
{
    // 拿到Parent 的世界变换矩阵
    glm::mat4 parentModelMatrix{1.0f};
    if (mParent != nullptr)
    {
        parentModelMatrix = mParent->getModelMatrix();
    }

    // unity 标准 先缩放 旋转 平移
    glm::mat4 transform{1.0f};
    transform = glm::scale(transform, mScale);

    // unity旋转标准 pitch yaw roll
    transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

    // 平移，这里定义以世界坐标系坐标为准
    transform = parentModelMatrix * glm::translate(glm::mat4(1.0f), mPosition) * transform;

    return transform;
}