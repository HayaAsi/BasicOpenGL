#include "Light.h"

void Light::setDirection(float x, float y, float z) {
    this->direction = glm::vec3(x, y, z);
}

void Light::setIntensity(vec4 intensity) {
    this->intensity = vec3(intensity.r, intensity.g, intensity.b);
    this->shininess = intensity.w;
}

vec3 Light::getIntensity() {
    return this->intensity;
}

DirectionalLight::DirectionalLight(vec3 direction) {
    this->type = Directional;
    this->direction = direction;
}

SpotLight::SpotLight(vec3 direction) {
    this->type = Spotlight;
    this->direction = direction;
    w = 0;
    position = vec3(0, 0, 0);
}


void SpotLight::setPosition(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}
void SpotLight::setAngle(float w) {
    this->w = w;
}
float SpotLight::getAngle() {
    return w;
}
vec3 SpotLight::getPosition() {
    return this->position;
}
