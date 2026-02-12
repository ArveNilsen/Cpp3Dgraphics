#pragma once

class CameraController
{
    void updateCamera(const InputState inputState)
    {
        if (inputState.keyDown(InputState::KEY::W))
            camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (inputState.keyDown(InputState::KEY::S))
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (inputState.keyDown(InputState::KEY::A))
            camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        if (inputState.keyDown(InputState::KEY::D))
            camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

        camera.ProcessMouseMovement(inputState.mouseDeltaX, inputState.mouseDeltaY);
    }

    Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
};
