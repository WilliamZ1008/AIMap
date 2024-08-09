#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <array>
#include <chrono>
#include <Windows.h>


#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "QuadShader.h"
#include "ElementCoordinator.h"
#include "Structs.h"
#include "Config.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

GLFWwindow* window;
int width, height;
double cursorX, cursorY, scale;
bool isLeftButtonPressed, isRightButtonPressed;
float u_Color[] = { 0.1f, 0.8f, 1.0f };
glm::mat4 model, view, proj, mvp;
ElementCoordinator element_coordinator;

glm::vec4 NDCToCoord(float x, float y) {
    glm::vec4 coord = glm::inverse(mvp) * glm::vec4(x, y, 0.0f, 1.0f);
    return coord;
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isLeftButtonPressed) {
        if (element_coordinator.ElementSelected()) {
            element_coordinator.MoveSelectedElement(NDCToCoord(2 * xpos / width - 1, 2 * (height - ypos) / height - 1));
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
            element_coordinator.SelectElementByCoord(NDCToCoord(2 * cursorX / width - 1, 2 * (height - cursorY) / height - 1));

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

int main(void)
{


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


    EnableBlend();

    // vertex
    QuadShader vertex_shader("res/shaders/chunk.glsl", Type::Vertex);

    vertex_shader.PushLayout<float>(2);
    vertex_shader.PushLayout<float>(2);
    vertex_shader.PushLayout<float>(1);
    vertex_shader.AddBuffer();

    // edge
    QuadShader edge_shader("res/shaders/edge.glsl", Type::Edge);

    edge_shader.PushLayout<float>(2);
    edge_shader.PushLayout<float>(2);
    edge_shader.AddBuffer();


    proj = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    std::cout << glGetString(GL_VERSION) << std::endl;

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> end;
    std::chrono::duration<float> elapsed_seconds;
    float seconds;


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {   

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        mvp = proj * view * model;
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        seconds = elapsed_seconds.count();

        // edge
        edge_shader.Bind();
        edge_shader.SetUniformMat4f("u_MVP", mvp);
        edge_shader.SetUniform1f("iTime", seconds);
        edge_shader.Draw(element_coordinator.getGLEdges().data(), element_coordinator.getGLEdgeNumber() * sizeof(Edge), element_coordinator.getGLEdgeIndexNumber());

        // vertex
        vertex_shader.Bind();
        vertex_shader.SetUniform3f("u_Color", u_Color[0], u_Color[1], u_Color[2]);
        vertex_shader.SetUniformMat4f("u_MVP", mvp);
        vertex_shader.SetUniform1f("iTime", seconds);
        vertex_shader.Draw(element_coordinator.getGLVertices().data(), element_coordinator.getGLVertexNumber() * sizeof(Vertex), element_coordinator.getGLVertexIndexNumber());

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

