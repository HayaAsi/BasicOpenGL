#include "Camera.h"

void Camera::SetPerspective(float FOVdegree, float near, float far)
{
    m_Aspect = (float) m_Width / (float) m_Height;
    m_FOVdegree = FOVdegree;
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::perspective(glm::radians(FOVdegree), m_Aspect, near, far);
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

void Camera::SetOrthographic(float near, float far)
{
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, near, far);
    m_View = lookAt(m_Position, m_Position + m_Orientation, m_Up);
}

void Camera::TranslateProjection(glm::vec3 trans)
{
    m_Projection = glm::translate(m_Projection, trans);
}

void Camera::setPosition(glm::vec3 position)
{
    m_Position = position;
}

void Camera::TranslateView(glm::vec3 trans)
{
    m_View = glm::translate(m_View, trans);
}

void Camera::RotateProjection(float angle, glm::vec3 rot)
{
    m_Projection = glm::rotate(m_Projection, angle, rot);
}

void Camera::RotateView(float angle, glm::vec3 rot)
{
    m_View = glm::rotate(m_View, angle, rot);
}

/////////////////////
// Input Callbacks //
/////////////////////

void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    float sensitivity = camera->m_KeySensitivity;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
                camera->TranslateView(glm::vec3(0.0f, -sensitivity, 0.0f));
                break;
            case GLFW_KEY_DOWN:
                camera->TranslateView(glm::vec3(0.0f, sensitivity, 0.0f));
                break;
            case GLFW_KEY_LEFT:
                camera->TranslateView(glm::vec3(sensitivity, 0.0f, 0.0f));
                break;
            case GLFW_KEY_RIGHT:
                camera->TranslateView(glm::vec3(-sensitivity, 0.0f, 0.0f));
                break;
            case GLFW_KEY_R:
                camera->cube.rotateRightWall();
    			break;
            case GLFW_KEY_L:
                camera->cube.rotateLeftWall();
                break;
            case GLFW_KEY_U:
                camera->cube.rotateUpWall();
                break;
            case GLFW_KEY_D:
                camera->cube.rotateDownWall();
                break;
            case GLFW_KEY_B:
                camera->cube.rotateBackWall();
                break;
            case GLFW_KEY_F:
                camera->cube.rotateFrontWall();
                break;
            case GLFW_KEY_SPACE:
                camera->cube.changeRotationDirection();
                break;
            case GLFW_KEY_Z:
                camera->cube.divideAngle();
                break;
            case GLFW_KEY_A:
                camera->cube.multiplyAngle();
                break;
            case GLFW_KEY_P:
                camera->cube.togglePickMode();
                break;
            case GLFW_KEY_M:
                camera->cube.mixer();
                break;
            default:
                break;
        }
    }
}

void CursorPosCallback(GLFWwindow* window, double currMouseX, double currMouseY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    float sensitivity = camera->m_MouseSensitivity;

    camera->m_NewMouseX = camera->m_OldMouseX - currMouseX;
    camera->m_NewMouseY = camera->m_OldMouseY - currMouseY;
    camera->m_OldMouseX = currMouseX;
    camera->m_OldMouseY = currMouseY;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(camera->cube.isPickMode()){
            Cube* cube = camera->cube.getPickedCube();
            if(!cube) return;
            float rotationAngleY = camera->m_NewMouseX * camera->m_MouseSensitivity;
            float rotationAngleX = camera->m_NewMouseY * camera->m_MouseSensitivity;
            glm::mat4 translationToOrigin = glm::translate(-cube->getPosition());
            glm::mat4 rotationMatrixX = rotate(rotationAngleX, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 rotationMatrixY = rotate(rotationAngleY, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 translateBack = glm::translate(cube->getPosition());
            glm::mat4 finalMatrix = translateBack * rotationMatrixY * rotationMatrixX * translationToOrigin;
            cube->setModelMatrix(finalMatrix * cube->getModelMatrix());

        } else {
            glm::mat3 viewRotateInverse = glm::transpose(glm::mat3(camera->GetViewMatrix()));
            camera->TranslateView(viewRotateInverse * glm::vec3((float) camera->m_NewMouseX * sensitivity, (float) -camera->m_NewMouseY * sensitivity, 0.0f));
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if(camera->cube.isPickMode()){
            Cube* cube = camera->cube.getPickedCube();
            if(!cube) return;
            glm::vec3 translation(-camera->m_NewMouseX * camera->m_KeySensitivity/30,camera->m_NewMouseY * camera->m_KeySensitivity/30,0.0f);
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
            cube->setModelMatrix(translationMatrix * cube->getModelMatrix());
        } else {
            glm::mat3 viewRotateInverse = glm::transpose(glm::mat3(camera->GetViewMatrix()));
            camera->RotateView((float) camera->m_NewMouseX * sensitivity, viewRotateInverse * glm::vec3(0.0f, -1.0f, 0.0f));
            camera->RotateView((float) camera->m_NewMouseY * sensitivity, viewRotateInverse * glm::vec3(-1.0f, 0.0f, 0.0f));
        }
    }
}

void ScrollCallback(GLFWwindow* window, double scrollOffsetX, double scrollOffsetY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! ScrollCallback is skipped" << std::endl;
        return;
    }

    float sensitivity = camera->m_ScrollSensitivity;

    camera->TranslateView(glm::vec3(0.0f, 0.0f, scrollOffsetY * sensitivity));
}

void MouseButtonCallback(GLFWwindow *window, double currMouseX, double currMouseY){
    //error checking
	Camera *camera = (Camera *)glfwGetWindowUserPointer(window);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(camera->cube.isPickMode()){
            double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);
			int flippedY = camera->m_Height - static_cast<int>(mouseY);
			unsigned char color_picked[4] = {0, 0, 0, 0};
			glReadPixels(static_cast<int>(mouseX), flippedY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color_picked);
			int color_id = color_picked[0];
			int index = color_id;
            printf("index is %d\n", index);
			if (index < 0 || index > 26){
				camera->cube.setPickedCube(nullptr);
            } else {
                camera->cube.setPickedCube(camera->cube.getCubes()[index]);
			}
        }
    }
}

void Camera::EnableInputs(GLFWwindow* window)
{
    // Set camera as the user pointer for the window
    glfwSetWindowUserPointer(window, this);

    // Handle key inputs
    glfwSetKeyCallback(window, (void(*)(GLFWwindow *, int, int, int, int)) KeyCallback);

    // Handle cursor postion and inputs
    glfwSetCursorPosCallback(window , (void(*)(GLFWwindow *, double, double)) CursorPosCallback);

    // Handle scroll inputs
    glfwSetScrollCallback(window, (void(*)(GLFWwindow *, double, double)) ScrollCallback);


	// Handle cursor buttons
	glfwSetMouseButtonCallback(window, (void (*)(GLFWwindow *, int, int, int))MouseButtonCallback);
}
