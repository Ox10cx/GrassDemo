#ifndef OBJECT_H
#define OBJECT_H

#include "core.h"
#include <vector>

enum class ObjectType {
    Object,
    Mesh,
    Scene,
    InstancedMesh,
};

class Object
{
public:
    Object();
    ~Object();

    void setPosition(glm::vec3 pos);

    // 增量旋转
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    void setAngleX(float angle);
    void setAngleY(float angle);
    void setAngleZ(float angle);
    

    void setScale(glm::vec3 scale);

    glm::mat4 getModelMatrix() const;

    glm::vec3 getPosition() const { return mPosition;}

    void addChild(Object* object);

    std::vector<Object*> getChilderen();

    Object* getParent();

    ObjectType getType() const {return mType;}



protected:
   glm::vec3 mPosition{0.0f};

   // unity旋转标准 pitch yaw roll
   float mAngleX {0.0f};
   float mAngleY {0.0f};
   float mAngleZ {0.0f};

   glm::vec3 mScale {1.0f};

   // 父子关系
   std::vector<Object*> mChildren{};
   Object* mParent{nullptr};
   
   // 类型
   ObjectType mType;

};

#endif