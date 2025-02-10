#ifndef RUBIKCUBE_H
#define RUBIKCUBE_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <VertexArray.h>
#include <IndexBuffer.h>
#include <Camera.h>
#include <iostream>
#include <GLFW/glfw3.h>

class Cube {
private:
    glm::vec3 position;
    glm::mat4 modelMatrix;
    glm::mat4 rotationMatrix;
    int index;

public:
    Cube(const glm::vec3& pos, int idx);
    Cube();

    glm::vec3 getPosition() const;
    glm::mat4 getModelMatrix() const;
    glm::mat4 getRotationMatrix() const;
    void setPosition(const glm::vec3& pos);
    void setModelMatrix(const glm::mat4& matrix);
    void setRotationMatrix(const glm::mat4& matrix);
    int getIndex();
};

class RubikCube {
private:
    std::vector<Cube*> cubes;
    std::vector<std::vector<std::vector<Cube*>>> cubePositions;
    bool tilted[6] = {false};
    Cube* centerCube = nullptr;
    int rotationDirection = 1;
    int rotationAngle = 90;
    float sensitivity = 0.1f;
    bool pickMode = false;
    Cube* pickedCube = nullptr;
    glm::mat4 proj;
    glm::mat4 view;
public:
    RubikCube();
    ~RubikCube();
    void render(GLFWwindow* win);
    void update(glm::mat4 proj, glm::mat4 view);
    void rotateLeftWall();
    void rotateRightWall();
    void rotateFrontWall();
    void rotateBackWall();
    void rotateDownWall();
    void rotateUpWall();
    void changeRotationDirection();
    void divideAngle();
    void multiplyAngle();
    void togglePickMode();
    glm::vec3 getPosition() const;
    std::vector<Cube*> getCubes() const;
    Shader* shader;
    VertexArray* vertexArray;
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    GLFWwindow* window = nullptr;
    void setProj(glm::mat4 proj);
    void setView(glm::mat4 proj);
    bool isPickMode();
    void setPickedCube(Cube* cube);
    Cube* getPickedCube();
    void mixer();
};

#endif // RUBIKCUBE_H