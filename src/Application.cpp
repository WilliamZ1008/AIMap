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
#include "ElementCoordinator.h"
#include "Structs.h"
#include "Config.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int width, height;
double cursorX, cursorY, scale;
bool isLeftButtonPressed;
glm::mat4 view;
float u_Color[] = { 0.1f, 0.8f, 1.0f };

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isLeftButtonPressed) {
        view = glm::translate(view, glm::vec3((xpos - cursorX) / scale, (cursorY - ypos) / scale, 0.0f));
        cursorX = xpos;
        cursorY = ypos;
    }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            u_Color[0] = 0.9f;
            u_Color[1] = 1.0f;
            u_Color[2] = 0.2f;
            glfwGetCursorPos(window, &cursorX, &cursorY);
            isLeftButtonPressed = true;
        }
        else if (action == GLFW_RELEASE) {
            u_Color[0] = 0.1f;
            u_Color[1] = 0.8f;
            u_Color[2] = 1.0f;
            isLeftButtonPressed = false;
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
    GLFWwindow* window;

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
    VertexArray va_vertex = VertexArray();

    VertexBuffer vb_vertex(MAX_VERTEX_COUNT * sizeof(Vertex));
    VertexBufferLayout layout_vertex;
    layout_vertex.Push<float>(2);
    layout_vertex.Push<float>(2);

    unsigned int vertex_indices[MAX_VERTEX_INDEX_COUNT];
    InitQuadIndeces(vertex_indices);
    Shader shader_vertex("res/shaders/chunk.glsl");


    // edge
    VertexArray va_edge = VertexArray();

    VertexBuffer vb_edge(MAX_EDGE_COUNT * sizeof(Edge));
    VertexBufferLayout layout_edge;
    layout_edge.Push<float>(2);
    layout_edge.Push<float>(2);

    unsigned int edge_indices[MAX_EDGE_INDEX_COUNT];
    InitQuadIndeces(edge_indices);

    Shader shader_edge("res/shaders/edge.glsl");





    glm::mat4 proj = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model;
    glm::mat4 mvp;

    std::cout << glGetString(GL_VERSION) << std::endl;

    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::system_clock> end;
    std::chrono::duration<float> elapsed_seconds;
    float seconds;

    ElementCoordinator element_coordinator;

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
        shader_edge.Bind();
        va_edge.AddBuffer(vb_edge, layout_edge);
        IndexBuffer ib_edge(edge_indices, MAX_EDGE_INDEX_COUNT);
        shader_edge.SetUniformMat4f("u_MVP", mvp);
        shader_edge.SetUniform1f("iTime", seconds);
        std::array<Edge, MAX_EDGE_COUNT> edges = element_coordinator.getGLEdges();
        vb_edge.DynamicBufferSubData(edges.data(), element_coordinator.getGLEdgeNumber() * sizeof(Edge));
        glDrawElements(GL_TRIANGLES, element_coordinator.getGLEdgeIndexNumber(), GL_UNSIGNED_INT, nullptr);

        // vertex
        shader_vertex.Bind();
        va_vertex.AddBuffer(vb_vertex, layout_vertex);
        IndexBuffer ib_vertex(vertex_indices, MAX_VERTEX_INDEX_COUNT);
        shader_vertex.SetUniform3f("u_Color", u_Color[0], u_Color[1], u_Color[2]);
        shader_vertex.SetUniformMat4f("u_MVP", mvp);
        shader_vertex.SetUniform1f("iTime", seconds);
        std::array<Vertex, MAX_VERTEX_COUNT> vertices = element_coordinator.getGLVertices();
        vb_vertex.DynamicBufferSubData(vertices.data(), element_coordinator.getGLVertexNumber() * sizeof(Vertex));
        glDrawElements(GL_TRIANGLES, element_coordinator.getGLVertexIndexNumber(), GL_UNSIGNED_INT, nullptr);



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

