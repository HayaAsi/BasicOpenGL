#include "Parser.h"

ObjectType GetObjectType(char objectTypeChar)
{
    switch (objectTypeChar)
    {
    case 'r':
        return Reflective;
    case 't':
        return Transparent;
    case 'o':
        return Object;
    default:
        return Nothing;
    }
}

Sphere* CreateSphere(char typeChar, double posX, double posY, double posZ, double radius)
{
    ObjectType objectType = GetObjectType(typeChar);
    Sphere* sphere = new Sphere(posX, posY, posZ, radius, objectType);
    sphere->setRadius(radius);
    return sphere;
}

Plane* CreatePlane(char typeChar, double normalX, double normalY, double normalZ, double distance)
{
    ObjectType objectType = GetObjectType(typeChar);
    Plane* plane = new Plane(normalX, normalY, normalZ, distance, objectType);
    return plane;
}

Parser::Parser() {
    this->eye = new Eye(0, 0, 0);
    this->ambientLight = new glm::vec4(0.0f);
    this->lights = new std::vector<Light*>();
    this->spotlights = new std::vector<SpotLight*>();
    this->spheres = new std::vector<Sphere*>();
    this->planes = new std::vector<Plane*>();
};

void Parser::parse(const std::string& fileName)
{
    int colorIndex = 0, positionIndex = 0, intensityIndex = 0;

    char objectTypeChar = 'a';
    double paramX1 = 1, paramX2 = 1, paramX3 = 1, paramX4 = 1;

    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error opening file " << fileName << ": " << std::strerror(errno) << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        char lineTypeChar = 'a';
        std::vector<double> parameters;

        std::istringstream lineStream(line);
        lineStream >> lineTypeChar;

        double number;

        while (lineStream >> number) {
            parameters.push_back(number);
        }

        if (parameters.size() >= 4) {
            paramX1 = parameters[0];
            paramX2 = parameters[1];
            paramX3 = parameters[2];
            paramX4 = parameters[3];
        }

        objectTypeChar = lineTypeChar;

        switch (objectTypeChar)
        {
        case 'e':
            this->eye = new Eye(paramX1, paramX2, paramX3);
            break;
        case 'a':
            this->ambientLight = new vec4(paramX1, paramX2, paramX3, paramX4);
            break;
        case 'd':
            ParseLightDirection(paramX1, paramX2, paramX3, paramX4);
            break;
        case 'p':
            this->spotlights->at(positionIndex)->setPosition(paramX1, paramX2, paramX3);
            this->spotlights->at(positionIndex)->setAngle(paramX4);
            positionIndex++;
            break;
        case 'i':
            this->lights->at(intensityIndex)->setIntensity(vec4(paramX1, paramX2, paramX3, paramX4));
            intensityIndex++;
            break;
        case 'c':
            this->objects->at(colorIndex)->setColor(vec4(paramX1, paramX2, paramX3, paramX4));
            this->objects->at(colorIndex)->setShininess(paramX4);
            colorIndex++;
            break;
        default:
            if (paramX4 > 0) {
                Sphere* sphere = CreateSphere(objectTypeChar, paramX1, paramX2, paramX3, paramX4);
                this->spheres->push_back(sphere);
                this->objects->push_back(sphere);
            } else {
                Plane* plane = CreatePlane(objectTypeChar, paramX1, paramX2, paramX3, paramX4);
                this->planes->push_back(plane);
                this->objects->push_back(plane);
            }
        }
    }
}

Light* Parser::ParseLightDirection(double dirX, double dirY, double dirZ, double lightType)
{
    Light* light = nullptr;
    if (lightType == 1) {
        light = new SpotLight(vec3(dirX, dirY, dirZ));
        this->spotlights->push_back((SpotLight*)light);
    } else {
        light = new DirectionalLight(vec3(dirX, dirY, dirZ));
    }
    light->setDirection(dirX, dirY, dirZ);
    this->lights->push_back(light);
    return light;
}