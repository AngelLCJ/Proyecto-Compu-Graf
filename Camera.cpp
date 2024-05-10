#include "Camera.h"

bool aerialCamera = false;
glm::vec3 lastCameraPosition;

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
    GLfloat velocity = moveSpeed * deltaTime;

    if (!aerialCamera) {
        if (keys[GLFW_KEY_W])
        {
            position += front * velocity;
        }

        if (keys[GLFW_KEY_S])
        {
            position -= front * velocity;
        }

        if (keys[GLFW_KEY_A])
        {
            position -= right * velocity;
        }

        if (keys[GLFW_KEY_D])
        {
            position += right * velocity;
        }
    }
    else {
        // Movimiento en el plano XZ
        if (keys[GLFW_KEY_A])
        {
            position -= glm::normalize(glm::cross(front, worldUp)) * velocity; // Mover hacia la izquierda en el plano XZ
        }

        if (keys[GLFW_KEY_D])
        {
            position += glm::normalize(glm::cross(front, worldUp)) * velocity; // Mover hacia la derecha en el plano XZ
        }
    }

    // Toggle aerial camera with 'p' key
    if (keys[GLFW_KEY_P] && !aerialCamera) {
        aerialCamera = true;
        lastCameraPosition = position;
        // Ajustar la posici�n de la c�mara a�rea m�s arriba en el eje Y
        position.y += 10.0f; // Ajusta seg�n tu necesidad
        // Ajustar la orientaci�n de la c�mara para mirar hacia abajo
        pitch = -89.0f;
        // Ajustar la direcci�n frontal de la c�mara
        front = glm::vec3(0.0f, -1.0f, 0.0f);
    }
    else if (keys[GLFW_KEY_P] && aerialCamera) {
        aerialCamera = false;
        position = lastCameraPosition;
        // Restaurar la orientaci�n y la direcci�n frontal de la c�mara
        pitch = 0.0f;
        front = glm::vec3(0.0f, 0.0f, -1.0f);
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
    return position;
}

glm::vec3 Camera::getCameraDirection()
{
    return glm::normalize(front);
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {}