#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>
#include <Windows.h>

#include "QuadRenderer.h"
#include "ElementCoordinator.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


GLFWwindow* window;
int width, height;
double cursorX, cursorY, scale;
bool isLeftButtonPressed, isRightButtonPressed;
float u_Color[] = { 0.1f, 0.8f, 1.0f };
glm::mat4 model, view, proj, mvp;
ElementCoordinator element_coordinator;

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isLeftButtonPressed) {
        if (element_coordinator.ElementSelected()) {
            element_coordinator.MoveSelectedElement(NDCToCoord(2 * xpos / width - 1, 2 * (height - ypos) / height - 1, mvp));
        }
    }

    if (isRightButtonPressed) {
        view = glm::translate(view, glm::vec3((xpos - cursorX) / scale, (cursorY - ypos) / scale, 0.0f));
        cursorX = xpos;
        cursorY = ypos;
    }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        switch (action) {
        case GLFW_PRESS:
            isLeftButtonPressed = true;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            element_coordinator.SelectElementByCoord(NDCToCoord(2 * cursorX / width - 1, 2 * (height - cursorY) / height - 1, mvp));

            break;
        case GLFW_RELEASE:
            isLeftButtonPressed = false;
            break;
        default:
            break;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        switch (action) {
        case GLFW_PRESS:
            u_Color[0] = 0.9f;
            u_Color[1] = 1.0f;
            u_Color[2] = 0.2f;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            isRightButtonPressed = true;
            break;
        case GLFW_RELEASE:
            u_Color[0] = 0.1f;
            u_Color[1] = 0.8f;
            u_Color[2] = 1.0f;
            isRightButtonPressed = false;
            break;
        default:
            break;
        }
    
    }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0) {
        glfwGetCursorPos(window, &cursorX, &cursorY);
        view = glm::translate(view, glm::vec3((cursorX), (height - cursorY), 0.0f));
        view = glm::scale(view, glm::vec3(1.1f, 1.1f, 1.0f));
        view = glm::translate(view, glm::vec3(-cursorX, cursorY - height, 0.0f));
        scale *= 1.1f;
    }
    else if (yoffset < 0) {
        glfwGetCursorPos(window, &cursorX, &cursorY);
        view = glm::translate(view, glm::vec3((cursorX), (height - cursorY), 0.0f));
        view = glm::scale(view, glm::vec3(0.9f, 0.9f, 1.0f));
        view = glm::translate(view, glm::vec3(-(cursorX), cursorY - height, 0.0f));
        scale *= 0.9f;
    }
}

int main(void){
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    width = 960;
    height = 540;
    scale = 1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    window = glfwCreateWindow(width, height, "AIMap Early Access", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    int max_width = GetSystemMetrics(SM_CXSCREEN);
    int max_hieght = GetSystemMetrics(SM_CYSCREEN);
    glfwSetWindowMonitor(window, NULL, (max_width / 2) - (width / 2), (max_hieght / 2) - (height / 2), width, height, GLFW_DONT_CARE);

    isLeftButtonPressed = false;
    isRightButtonPressed = false;

    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    EnableBlend();

    // Vertex
    QuadRenderer vertex_renderer("res/shaders/chunk.glsl", Type::Vertex);

    // Edge
    QuadRenderer edge_renderer("res/shaders/edge.glsl", Type::Edge);

    // Init model, view, proj
    InitModelViewProj(model, view, proj, width, height);

    // Timer
    Timer timer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {   

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // MVP
        //mvp = proj * view * model;
        mvp = proj * view;
 
        // Edge
        edge_renderer.Bind();
        edge_renderer.SetUniformMat4f("u_MVP", mvp);
        edge_renderer.SetUniform1f("iTime", timer.GetSeconds());
        edge_renderer.Draw(element_coordinator.GetGLEdges().data(), element_coordinator.GetGLEdgeSize(), element_coordinator.GetGLEdgeIndexNumber());

        // Vertex
        vertex_renderer.Bind();
        vertex_renderer.SetUniform3f("u_Color", u_Color[0], u_Color[1], u_Color[2]);
        vertex_renderer.SetUniformMat4f("u_MVP", mvp);
        vertex_renderer.SetUniform1f("iTime", timer.GetSeconds());
        vertex_renderer.Draw(element_coordinator.GetGLVertices().data(), element_coordinator.GetGLVertexSize(), element_coordinator.GetGLVertexIndexNumber());

        // Update
        element_coordinator.OnUpdate();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

