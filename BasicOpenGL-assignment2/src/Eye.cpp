#include "Eye.h" 

Eye::Eye(double x, double y, double z) {
    position = vec3(x, y, z);
}

vec3 Eye::getPosition() {
    return position;
}