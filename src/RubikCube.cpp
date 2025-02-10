#include "RubikCube.h"
#include <ctime>
#include <fstream>
#include <cstdlib>

Cube::Cube(const glm::vec3& pos, int idx) : position(pos), index(idx), rotationMatrix(glm::mat4(1.0f)) {}
Cube::Cube() : index(0), position(glm::vec3(0.0f)), rotationMatrix(glm::mat4(1.0f)) {}

glm::vec3 Cube::getPosition() const { return position; }
glm::mat4 Cube::getModelMatrix() const { return modelMatrix; }
glm::mat4 Cube::getRotationMatrix() const { return rotationMatrix; }
void Cube::setPosition(const glm::vec3& pos) { position = pos; }
void Cube::setModelMatrix(const glm::mat4& matrix) { modelMatrix = matrix; position = glm::vec3(round(matrix[3][0]), round(matrix[3][1]), round(matrix[3][2])); }
void Cube::setRotationMatrix(const glm::mat4& matrix) { rotationMatrix = matrix; }
int Cube::getIndex(){ return index; }
RubikCube::RubikCube() {
    cubePositions.clear();
    int cubeSize = 3;
    cubePositions.resize(cubeSize, std::vector<std::vector<Cube*>>(cubeSize, std::vector<Cube*>(cubeSize)));
    cubes.clear();
    int index = 0;
    const int offset = 1;
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                Cube* cube = new Cube(glm::vec3(x, y, z), index);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
                cube->setModelMatrix(model);
                cubePositions[x + offset][y + offset][z + offset] = cube;
                cubes.push_back(cube);
                if (index == 13) centerCube = cube;
                index++;
            }
        }
    }
}

RubikCube::~RubikCube() { cubes.clear(); }

glm::vec3 RubikCube::getPosition() const { return centerCube ? centerCube->getPosition() : glm::vec3(0.0f); }

void RubikCube::rotateLeftWall() {
    if(tilted[2] || tilted[3] || tilted[4] || tilted[5]){
        std::cout << "Wall is currently locked!" << std::endl;
        return;
    }
    
    for (int i = 0; i < rotationAngle; ++i) {
        for (auto& cube : cubes) {
            if (glm::abs(cube->getPosition().x + 1.0f) < 0.01f) {
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-static_cast<float>(rotationDirection)), glm::vec3(1.0f, 0.0f, 0.0f));
                cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());
            }
        }
        render(window);
    }

    if (rotationAngle == 45) {
        tilted[0] = !tilted[0];
    }
}

void RubikCube::rotateRightWall() {
    if(tilted[2] || tilted[3] || tilted[4] || tilted[5]){
        std::cout << "Wall is currently locked!" << std::endl;
        return;
    }
    
    for (int i = 0; i < rotationAngle; ++i) {
        for (auto& cube : cubes) {
            if (glm::abs(cube->getPosition().x - 1.0f) < 0.01f) {
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(rotationDirection)), glm::vec3(1.0f, 0.0f, 0.0f));
                cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());
            }
        }
        render(window);
    }

    if (rotationAngle == 45) {
        tilted[1] = !tilted[1];
    }
}

void RubikCube::rotateFrontWall() {
    if(tilted[0] || tilted[1] || tilted[4] || tilted[5]){
        std::cout << "Wall is currently locked!" << std::endl;
        return;
    }
    
    for (int i = 0; i < rotationAngle; ++i) {
        for (auto& cube : cubes) {
            if (glm::abs(cube->getPosition().z - 1.0f) < 0.01f) {
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-static_cast<float>(rotationDirection)), glm::vec3(0.0f, 0.0f, 1.0f));
                cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());
            }
        }
        render(window);
    }

    if (rotationAngle == 45) {
        tilted[2] = !tilted[2];
    }
}

void RubikCube::rotateBackWall() {
    if(tilted[0] || tilted[1] || tilted[4] || tilted[5]){
        std::cout << "Wall is currently locked!" << std::endl;
        return;
    }
    
    for (int i = 0; i < rotationAngle; ++i) {
        for (auto& cube : cubes) {
            if (glm::abs(cube->getPosition().z + 1.0f) < 0.01f) {
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(rotationDirection)), glm::vec3(0.0f, 0.0f, 1.0f));
                cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());
            }
        }
        render(window);
    }

    if (rotationAngle == 45) {
        tilted[3] = !tilted[3];
    }
}

void RubikCube::rotateDownWall() {
    if(tilted[0] || tilted[1] || tilted[2] || tilted[3]){
        std::cout << "Wall is currently locked!" << std::endl;
        return;
    }
    
    for (int i = 1; i <= rotationAngle; i++) {
        for (auto& cube : cubes) {
            if (glm::abs(cube->getPosition().y + 1.0f) < 0.01f) {
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-static_cast<float>(rotationDirection)), glm::vec3(0.0f, 1.0f, 0.0f));
                cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());
            }
        }
        render(window);
    }
    
    if (rotationAngle == 45) {
        tilted[4] = !tilted[4];
    }
}

void RubikCube::rotateUpWall() {
    if(tilted[0] || tilted[1] || tilted[2] || tilted[3]){
        std::cout << "Wall is currently locked!" << std::endl;
        return;
    }
    
    for (int i = 1; i <= rotationAngle; i++) {
        for (auto& cube : cubes) {
            if (glm::abs(cube->getPosition().y - 1.0f) < 0.01f) {
                glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(rotationDirection)), glm::vec3(0.0f, 1.0f, 0.0f));
                cube->setModelMatrix(rotationMatrix * cube->getModelMatrix());
            }
        }
        render(window);
    }
    
    if (rotationAngle == 45) {
        tilted[5] = !tilted[5];
    }
}


void RubikCube::update(glm::mat4 proj, glm::mat4 view) {
    for (auto& cube : cubes) {
        glm::mat4 model = cube->getRotationMatrix() * cube->getModelMatrix();
        glm::mat4 mvp = proj * view * model;
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        // Pass the cube index to the shader to determine highlight
        shader->Bind();
        int isPicked = (pickedCube && pickedCube->getIndex() == cube->getIndex()) ? 1 : 0;
        shader->SetUniform1i("u_IsPicked", isPicked);
        shader->SetUniform1i("u_PickingMode", 0);
        shader->SetUniform4f("u_Color", color);
        shader->SetUniformMat4f("u_MVP", mvp);

        vertexArray->Bind();
        indexBuffer->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));

        shader->Unbind();
    }
    glfwSwapBuffers(window);

    if (pickMode) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto& cube : cubes) {
            glm::mat4 model = cube->getRotationMatrix() * cube->getModelMatrix();
            glm::mat4 mvp = proj * view * model;
            shader->Bind();
            glm::vec3 uniqueColor = glm::vec3(cube->getIndex(), cube->getIndex(), cube->getIndex());
            glm::vec4 pickingColor = glm::vec4(uniqueColor / 255.0f, 1.0f);
            shader->SetUniform1i("u_PickingMode", 1);
            shader->SetUniform4f("u_Color", pickingColor);
            shader->SetUniformMat4f("u_MVP", mvp);
            vertexArray->Bind();
            indexBuffer->Bind();
            GLCall(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
            shader->Unbind();
        }
        glFlush();
        glFinish();
    }
}

void RubikCube::render(GLFWwindow* win) {
    GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f)); // Set background to white
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update(proj, view);
}

void RubikCube::setProj(glm::mat4 proj){
    this->proj = proj;
}
void RubikCube::setView(glm::mat4 view){
    this->view = view;
}

void RubikCube::changeRotationDirection(){
	rotationDirection = -rotationDirection;
}

void RubikCube::divideAngle(){
	if(rotationAngle > 45){
        rotationAngle = rotationAngle/2;
        std::cout << "new rotation angle " << rotationAngle << std::endl;
    } else {
        std::cout << "minimum angle reached " << rotationAngle << std::endl;
    }
}

void RubikCube::multiplyAngle(){
	if(rotationAngle < 180){
        rotationAngle = rotationAngle*2;
        std::cout << "new rotation angle " << rotationAngle << std::endl;
    } else {
        std::cout << "maximum angle reached " << rotationAngle << std::endl;
    }
}
void RubikCube::togglePickMode(){
    pickMode = !pickMode;
    std::cout << "picking is " << pickMode << rotationAngle << std::endl;
}

bool RubikCube::isPickMode(){
    return pickMode;
}

void RubikCube::setPickedCube(Cube* cube){
    this->pickedCube = cube;
}

std::vector<Cube*> RubikCube::getCubes() const{
    return cubes;
}

Cube* RubikCube::getPickedCube(){
    return pickedCube;
}

void RubikCube::mixer(){
    // Seed random number generator with current time
    std::srand(std::time(0));

    // List of available functions
    std::vector<std::pair<void(RubikCube::*)(), std::string>> functions = {
        {&RubikCube::rotateLeftWall, "L"},
        {&RubikCube::rotateRightWall, "R"},
        {&RubikCube::rotateFrontWall, "F"},
        {&RubikCube::rotateBackWall, "B"},
        {&RubikCube::rotateDownWall, "D"},
        {&RubikCube::rotateUpWall, "U"},
        {&RubikCube::changeRotationDirection, "SPACE"},
        {&RubikCube::divideAngle, "Z"},
        {&RubikCube::multiplyAngle, "A;"}
    };

    // Open file for writing
    std::ofstream outFile("../mixer.txt");
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing!" << std::endl;
        return;
    }

    // Generate 100 random function calls
    for (int i = 0; i < 100; i++) {
        int randomIndex = std::rand() % functions.size();  // Pick a random function

        // Write function call to file
        outFile << functions[randomIndex].second << std::endl;

        // Call the function
        (this->*functions[randomIndex].first)();

    }

    // Close the file
    outFile.close();

    std::cout << "Generated 100 random function calls in mixer.txt" << std::endl;
    
}