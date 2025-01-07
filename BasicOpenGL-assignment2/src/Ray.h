#pragma once
#ifndef RAY_H
#define RAY_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Surface.h"

using namespace std;
using namespace glm;

class Ray {

private:
    vec3 direction;
    vec3 origin;
    vec3 hitPoint;
    Surface* sceneObject;

public:
    Ray(vec3 direction, vec3 origin);

    vec3 getRayDirection() const;
    vec3 getRayOrigin() const;
    vec3 getHitPoint() const;
    Surface* getSceneObject() const;
    void setRayDirection(vec3 vec);

    void setRayOrigin(vec3 vec);
    void setHitPoint(vec3 vec);
    void setSceneObject(Surface* obj);
    
};
#endif