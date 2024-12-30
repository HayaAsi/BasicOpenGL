#pragma once
#include <fstream>
#include "Light.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

class Parser {
public:
    Eye* eye;
    vec4* ambientLight;
    vector<Light*>* lights;
    vector<SpotLight*>* spotlights;
    vector<Sphere*>* spheres;
    vector<Plane*>* planes;
    vector<Surface*>* objects = new vector<Surface*>();

    Parser();
 
    void parse(const std::string& fileName);
    Light* ParseLightDirection(double dirX, double dirY, double dirZ, double lightType);
};
