#include "Ray.h"
Ray::Ray(vec3 direction, vec3 origin) {
    this->direction = direction;
    this->origin = origin;
    hitPoint = origin + direction;
    sceneObject = new Plane(0.0, 0.0, 0.0, 0.0, Nothing);;
}

vec3 Ray::getRayDirection() const {
    return direction;
}
vec3 Ray::getRayOrigin() const {
    return origin;
}
vec3 Ray::getHitPoint() const {
    return hitPoint;
}
Surface* Ray::getSceneObject() const {
    return sceneObject;
}
void Ray::setRayDirection(vec3 vec) {
    direction = vec;
}

void Ray::setRayOrigin(vec3 vec) {
    origin = vec;
}
void Ray::setHitPoint(vec3 vec) {
    hitPoint = vec;
}
void Ray::setSceneObject(Surface* obj) {
    sceneObject = obj;
}