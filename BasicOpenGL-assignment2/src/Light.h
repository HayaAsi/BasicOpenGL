#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "Surface.h"
#include "Ray.h"
#include "Eye.h"

using namespace std;
using namespace glm;

class Light 
{
public:
    glm::vec3 direction;
    glm::vec3 intensity;
    float shininess = 0;
    LightType type;

    void setDirection(float x, float y, float z);
    void setIntensity(vec4 intensity);
    vec3 getIntensity();
};

struct DirectionalLight : Light
{
public:
    DirectionalLight(vec3 direction);
};

struct SpotLight : Light
{
private:
    vec3 position;
    float w = 0;

public:
    SpotLight(vec3 direction);
    void setPosition(float x, float y, float z);
    void setAngle(float w);
    float getAngle();
    vec3 getPosition();
};
#endif