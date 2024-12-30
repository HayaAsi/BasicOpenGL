#pragma once
#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "Parser.h"

struct RayTrace {
public:
    unsigned char* GenerateImage(const char* scene);
private:
    Ray UpdateRay(int j, int i, Surface* ob, bool update, Ray reflectedRay, Parser* scene);
    vec3 CalculateNormal(const vec3& hitPoint, Surface* object);
    float CalculateDiffuseLighting(const vec3& normal, const Ray& ray, Light* light);
    float CalculateSpecularLighting(const vec3& viewDirection, const Ray& ray, Light* light);
    float CalculateShadowIntensity(const Ray& ray, Light* light, Parser* scene);
    Ray ApplySnellLaw(const Ray& ray, const vec3& normal, const vec3& rayDirection, float refractiveIndex);
    float calcNLi(vec3 N, Ray ray, Light* light);
    float calcVRi(vec3 V, Ray ray, Light* light);
    float calcSiIi(Ray ray, Light* light, Parser* scene);
    vec4 GetPixelColor(int j, int i, Ray ray, int count, Parser* scene);
    unsigned char* rendering(Parser* scene);
};

#endif