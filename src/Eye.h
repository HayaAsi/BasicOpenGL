#pragma once
#ifndef EYE_H
#define EYE_H

#include <vector>
#include <glm/glm.hpp>
#include <string>

using namespace std;
using namespace glm;
class Eye 
{
private:
    vec3 position;
public:
    Eye(double x, double y, double z);
    vec3 getPosition();
};

#endif