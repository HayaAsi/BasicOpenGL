#pragma once
#ifndef SURFACE_H
#define SURFACE_H

#include <vector>
#include <glm/glm.hpp>
#include <string>

using namespace std;
using namespace glm;

enum ObjectType { Object, Reflective, Transparent , Nothing};
enum ObjectClass { PlaneObject, SphereObject };
enum LightType { Directional, Spotlight };

struct Surface
{
protected:
    ObjectType type;
    ObjectClass objectClass = PlaneObject;
    vec4 coordinates = vec4(0, 0, 0, 0);
    vec3 color = vec3(0, 0, 0);
    vec3 position = vec3(0, 0, 0);
    float shininess = 0;

public:
    
    void setColor(vec4 color) {
        this->color = vec3(color.r, color.g, color.b);
        this->shininess = color.w;
    }
    void setShininess(float shininess) {
        this->shininess = shininess;
    }
    float getShininess() {
        return this->shininess;
    }
    vec4 getCoordinates() {
        return this->coordinates;
    }
    ObjectType getType() {
        return this->type;
    }
    ObjectClass getClassOfObject() {
        return this->objectClass;
    }
    vec3 getPosition() {
        return position;
    }
    virtual vec3 getColor(vec3 hit_point) = 0;
};


struct Sphere : Surface
{
private:
    double radius;

public:
    Sphere(double x, double y, double z, double r, ObjectType type) {
        this->coordinates = vec4(x, y, z, r);
        this->position = glm::vec3(x, y, z);
        this->radius = r;
        this->type = type;
        this->objectClass = SphereObject;
    };

    void setRadius(double r){ 
        this->radius = r;
    }

    vec3 getPosition() {
        return this->position;
    }

    float getRadius() {
        return this->radius;
    }

    vec3 getColor(vec3 hitPoint) {
        return this->color;
    }

};

struct Plane : Surface
{

    Plane(double a, double b, double c, double d, ObjectType type) {
        this->coordinates = vec4(a, b, c, d);
        this->position = vec3(a, b, c);
        this->type = type;
        this->objectClass = PlaneObject;
    };

    vec3 getPosition() {
        return this->position;
    }
    float getD() {
        return this->coordinates.w;
    }
    ////////////from the tergol 5 /////////////////
    vec3 getColor(vec3 hitPoint) {
        // Checkerboard pattern
        float scale_parameter = 0.5f;
        float chessboard = 0;

        if (hitPoint.x < 0) {
            chessboard += floor((0.5 - hitPoint.x) / scale_parameter);
        }
        else {
            chessboard += floor(hitPoint.x / scale_parameter);
        }

        if (hitPoint.y < 0) {
            chessboard += floor((0.5 - hitPoint.y) / scale_parameter);
        }
        else {
            chessboard += floor(hitPoint.y / scale_parameter);
        }

        chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
        chessboard *= 2;
        if (chessboard > 0.5) {
            return 0.5f * this->color;
        }
        return this->color;
    }
  
};

#endif