
#include "RayTrace.h"

Ray RayTrace::UpdateRay(int j, int i, Surface* ob, bool update, Ray reflectedRay, parser* scene) {

    float width = 2.0f / 800.0f;
    float height = 2.0f / 800.0f;

    if (!update) {
        vec3 pixelCenter(-1 + width / 2, 1 - height / 2, 0);
        vec3 exactPixel = pixelCenter + vec3(j * width, -1 * (i * height), 0);
        vec3 eyeVec = scene->eye->getPosition();
        vec3 rayDirection = normalize(exactPixel - eyeVec);

        reflectedRay.setRayDirection(rayDirection);
        reflectedRay.setRayOrigin(eyeVec);

    }

    // update the ray
    float minT = INFINITY;
    Surface* closestObject = new Plane(0.0, 0.0, 0.0, 0.0, Nothing);
    reflectedRay.setHitPoint(reflectedRay.getRayOrigin() + reflectedRay.getRayDirection());
    reflectedRay.setSceneObject(closestObject);

    for (unsigned int i = 0; i < scene->objects->size(); i++) {
        float t = 0.0;
        Surface* currentObject = scene->objects->at(i);
        if (currentObject != ob) {

            if (currentObject->getClassOfObject() == PlaneObject) {
                float denominator = glm::dot(reflectedRay.getRayDirection(), currentObject->getPosition());

                if (abs(denominator) < 0.0001f) {
                    t = -1.0f; // No intersection

                }
                // intersection equation
                t = -(glm::dot(reflectedRay.getRayOrigin(), currentObject->getPosition()) + ((Plane*)currentObject)->getD()) / denominator;

                if (t < 0.0f) {
                    t = -1.0f; // No intersection

                }

            }

            else {
                vec3 oc = reflectedRay.getRayOrigin() - currentObject->getPosition();
                float a = dot(reflectedRay.getRayDirection(), reflectedRay.getRayDirection());
                float b = 2.0f * dot(oc, reflectedRay.getRayDirection());
                float c = dot(oc, oc) - ((Sphere*)currentObject)->getRadius() * ((Sphere*)currentObject)->getRadius();

                float delta = b * b - 4 * a * c; // Discriminant of the quadratic equation

                if (delta < 0) {
                    t = -1.0f;

                }
                else {
                    float t1 = (-b - sqrt(delta)) / (2.0f * a);
                    float t2 = (-b + sqrt(delta)) / (2.0f * a);

                    if (t1 < 0 && t2 < 0) {
                        t = -1.0f; // no intersection

                    }
                    float first = (t1 >= 0) ? t1 : t2;

                    if (first <= 0.0001f) {
                        float second = (t1 >= 0 && t2 >= 0) ? glm::max(t1, t2) : -1.0f;
                        t = second;

                    }
                    else {
                        t = first;

                    }
                }
            }
            if ((t >= 0) && t < minT) {
                closestObject = currentObject;
                minT = t;
                reflectedRay.setSceneObject(closestObject);
                reflectedRay.setHitPoint(reflectedRay.getRayOrigin() + reflectedRay.getRayDirection() * minT);
            }
        }
    }

    return reflectedRay;
}


vec3 RayTrace::CalculateNormal(const vec3& hitPoint, Surface* object) {
    if (object->getClassOfObject() == PlaneObject) {
        return normalize(vec3(object->getCoordinates()));
    }
    return normalize(hitPoint - ((Sphere*)object)->getPosition());
}

float RayTrace::CalculateDiffuseLighting(const vec3& normal, const Ray& ray, Light* light) {
    vec3 lightDirection;

    if (light->type == Directional) {
        lightDirection = -normalize(light->direction);
        float cosTheta = dot(normal, -lightDirection);
        return glm::max(cosTheta, 0.0f);
    } else { // Spotlight
        vec3 spotDirection = normalize(ray.getHitPoint() - ((SpotLight*)light)->getPosition());
        float cosTheta = dot(spotDirection, -lightDirection);

        if (cosTheta < ((SpotLight*)light)->getAngle()) {
            return 0.0f;
        } else {
            lightDirection = -spotDirection;
            cosTheta = dot(normal, -lightDirection);
            return glm::max(cosTheta, 0.0f);
        }
    }
}

float RayTrace::CalculateSpecularLighting(const vec3& viewDirection, const Ray& ray, Light* light) {
    vec3 lightDirection = normalize(light->direction);
    vec3 normal = CalculateNormal(ray.getHitPoint(), ray.getSceneObject());

    if (light->type == Directional) {
        vec3 reflectedRay = lightDirection - 2.0f * normal * dot(lightDirection, normal);
        float cosTheta = dot(viewDirection, reflectedRay);
        return pow(glm::max(0.0f, cosTheta), ray.getSceneObject()->getShininess());
    } else { // Spotlight
        vec3 spotDirection = normalize(ray.getHitPoint() - ((SpotLight*)light)->getPosition());
        float cosTheta = dot(spotDirection, lightDirection);

        if (cosTheta < ((SpotLight*)light)->getAngle()) {
            return 0.0f;
        } else {
            lightDirection = spotDirection;
            vec3 reflectedRay = lightDirection - 2.0f * normal * dot(lightDirection, normal);
            cosTheta = dot(viewDirection, reflectedRay);
            return pow(glm::max(0.0f, cosTheta), ray.getSceneObject()->getShininess());
        }
    }
}

float RayTrace::CalculateShadowIntensity(const Ray& ray, Light* light, parser* scene) {
    vec3 lightDirection = normalize(light->direction);
    float minIntersection = INFINITY;

    if (light->type == Spotlight) {
        vec3 spotDirection = normalize(ray.getHitPoint() - ((SpotLight*)light)->getPosition());
        float cosTheta = dot(spotDirection, lightDirection);


        if (cosTheta < ((SpotLight*)light)->getAngle()) {
            return 0.0f;
        } else {
            lightDirection = spotDirection;
            minIntersection = glm::length(((SpotLight*)light)->getPosition() - ray.getHitPoint());
        }
    }

    for (unsigned int objectIndex = 0; objectIndex < scene->objects->size(); ++objectIndex) {
        Surface* currentObject = scene->objects->at(objectIndex);

        if (currentObject != ray.getSceneObject()) {
            Ray shadowRay(-lightDirection, ray.getHitPoint());
            float intersectionDistance = 0.0f;

            if (currentObject->getClassOfObject() == PlaneObject) {
                float denominator = glm::dot(shadowRay.getRayDirection(), currentObject->getPosition());
                if (abs(denominator) > 0.0001f) {
                    intersectionDistance = -(glm::dot(shadowRay.getRayOrigin(), currentObject->getPosition()) + ((Plane*)currentObject)->getD()) / denominator;
                }
            } else if (currentObject->getClassOfObject() == SphereObject) {
                vec3 originToCenter = shadowRay.getRayOrigin() - currentObject->getPosition();
                float a = dot(shadowRay.getRayDirection(), shadowRay.getRayDirection());
                float b = 2.0f * dot(originToCenter, shadowRay.getRayDirection());
                float c = dot(originToCenter, originToCenter) - pow(((Sphere*)currentObject)->getRadius(), 2);
                float discriminant = b * b - 4 * a * c;

                if (discriminant >= 0) {
                    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
                    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
                    intersectionDistance = (t1 > 0) ? t1 : t2;
                }
            }

            if ((intersectionDistance > 0) && (intersectionDistance < minIntersection)) {
                return 0.0f;
            }
        }
    }
    return 1.0f;
}


Ray RayTrace::ApplySnellLaw(const Ray& ray, const vec3& normal, const vec3& rayDirection, float refractiveIndex) {
    const float PI = 3.14159265f;

    float cosTheta1 = dot(normal, -ray.getRayDirection());
    float sinTheta1 = sqrt(1.0f - cosTheta1 * cosTheta1);
    float sinTheta2 = refractiveIndex * sinTheta1;
    float cosTheta2 = sqrt(1.0f - sinTheta2 * sinTheta2);

    vec3 refractedDirection = refractiveIndex * ray.getRayDirection() + (refractiveIndex * cosTheta1 - cosTheta2) * normal;
    return Ray(refractedDirection, ray.getHitPoint());
}

vec4 RayTrace::GetPixelColor(int pixelX, int pixelY, Ray ray, int recursionDepth, parser* scene) {
    vec3 finalColor(0.0f);

    if (ray.getSceneObject()->getType() == Object) {
        vec3 ambientColor = ray.getSceneObject()->getColor(ray.getHitPoint()) * vec3(scene->ambientLight->r, scene->ambientLight->g, scene->ambientLight->b);
        vec3 lightingContributions(0.0f);


        for (unsigned int lightIndex = 0; lightIndex < scene->lights->size(); ++lightIndex) {
            vec3 normal = CalculateNormal(ray.getHitPoint(), ray.getSceneObject());
            vec3 viewDirection = normalize(ray.getRayOrigin() - ray.getHitPoint());

            float diffuse = CalculateDiffuseLighting(normal, ray, scene->lights->at(lightIndex));
            float specular = CalculateSpecularLighting(viewDirection, ray, scene->lights->at(lightIndex));
            float shadow = CalculateShadowIntensity(ray, scene->lights->at(lightIndex), scene);

            lightingContributions += (diffuse + specular) * shadow;
        }

        finalColor = ambientColor + lightingContributions;
    }

    finalColor = glm::clamp(finalColor, vec3(0.0f), vec3(1.0f));
    return vec4(finalColor, 1.0f);
}

unsigned char* RayTrace::GenerateImage(parser* scene) {
    auto* imageData = new unsigned char[800 * 800 * 4];

    for (int row = 0; row < 800; ++row) {
        for (int column = 0; column < 800; ++column) {
            Ray initialRay(vec3(0, 0, 0), vec3(0, 0, 0));
            Plane* defaultPlane = new Plane(0.0f, 0.0f, 0.0f, 0.0f, Nothing);
            Ray ray = UpdateRay(column, row, defaultPlane, false, initialRay, scene);
            vec4 pixelColor = GetPixelColor(column, row, ray, 0, scene);

            int pixelIndex = (column + 800 * row) * 4;
            imageData[pixelIndex] = static_cast<unsigned char>(pixelColor.r * 255);
            imageData[pixelIndex + 1] = static_cast<unsigned char>(pixelColor.g * 255);
            imageData[pixelIndex + 2] = static_cast<unsigned char>(pixelColor.b * 255);
            imageData[pixelIndex + 3] = static_cast<unsigned char>(pixelColor.a * 255);
        }
    }

    return imageData;
}