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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int width, height;
double cursorX, cursorY;
bool isLeftButtonPressed;
glm::mat4 view;
float u_Color[] = { 0.1f, 0.8f, 1.0f };

struct Vertex{
	float x, y, u, v;
};

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isLeftButtonPressed) {
        view = glm::translate(view, glm::vec3((xpos - cursorX) * 1.0f, (cursorY - ypos) * 1.0f, 0.0f));
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
	}
	else if (yoffset < 0) {
        glfwGetCursorPos(window, &cursorX, &cursorY);
        view = glm::translate(view, glm::vec3((cursorX), (height - cursorY), 0.0f));
		view = glm::scale(view, glm::vec3(0.9f, 0.9f, 1.0f));
        view = glm::translate(view, glm::vec3(-(cursorX), cursorY - height, 0.0f));
	}
	
}

static Vertex* CreateQuad(Vertex* target, float x, float y, float radius) {
	target->x = x - radius;
	target->y = y - radius;
	target->u = -1.0f;
	target->v = -1.0f;
	target++;

	target->x = x + radius;
	target->y = y - radius;
	target->u =  1.0f;
	target->v = -1.0f;
	target++;

	target->x = x + radius;
	target->y = y + radius;
	target->u =  1.0f;
	target->v =  1.0f;
	target++;

	target->x = x - radius;
	target->y = y + radius;
	target->u = -1.0f;
	target->v =  1.0f;
    target++;

	return target;
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


    const size_t MaxQuadCount = 1000;
    const size_t MaxVertexCount = MaxQuadCount * 4;
    const size_t MaxIndexCount = MaxQuadCount * 6;

    unsigned int indices[MaxIndexCount];
    unsigned int offset = 0;
    for (size_t i = 0; i < MaxIndexCount; i += 6) {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va = VertexArray();

    VertexBuffer vb(MaxVertexCount * sizeof(Vertex));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);
    IndexBuffer ib(indices, MaxIndexCount);

    Shader shader("res/shaders/chunk.glsl");
    shader.Bind();

    glm::mat4 proj = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    glm::mat4 model;
    glm::mat4 mvp;

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
        shader.SetUniform3f("u_Color", u_Color[0], u_Color[1], u_Color[2]);
        shader.SetUniformMat4f("u_MVP", mvp);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end - start;
        seconds = elapsed_seconds.count();
        shader.SetUniform1f("iTime", seconds);
        unsigned int indexCount = 0;
        std::array<Vertex, MaxVertexCount> vertices;
        Vertex* target = vertices.data();

        for (int y = 0; y < 5; ++y) {
            for (int x = 0; x < 8; ++x) {
				target = CreateQuad(target, x * 120.0f, y * 120.0f, 50.0f);
                indexCount += 6;
			}
        }

        vb.DynamicBufferSubData(vertices.data(), vertices.size() * sizeof(Vertex));

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

