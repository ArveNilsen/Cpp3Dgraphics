#include "Application.hpp"
#include "WindowConfig.hpp"
#include "Window.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

int main() {
    Application app;

    WindowConfig wc;
    Window window(wc);
    Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
    Renderer renderer(window, camera);
    renderer.init();

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!window.shouldClose())
    {
        float currentFrame = static_cast<float>(window.getTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle inputs
        const InputState inputs = window.snapshotInput(); 
        if (inputs.closeRequested())
        {
            window.requestClose();
        }

        if (inputs.keyDown(InputState::KEY::W))
            camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (inputs.keyDown(InputState::KEY::S))
            camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (inputs.keyDown(InputState::KEY::A))
            camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        if (inputs.keyDown(InputState::KEY::D))
            camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

        camera.ProcessMouseMovement(inputs.mouseDeltaX, inputs.mouseDeltaY);

        // Render frame
        renderer.renderFrame();
        
        // Poll events
        window.pollEvents();

        // Swap buffers
        window.swapBuffers();
    }

    return 0;
}
