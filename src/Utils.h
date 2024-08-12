#pragma once

#include <GLFW/glfw3.h>

#include <chrono>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Config.h"
#include "Structs.h"

static Vertex* CreateQuad(Vertex* target, float x, float y, float radius, float selected) {
	target->x = x - radius;
	target->y = y - radius;
	target->u = -1.0f;
	target->v = -1.0f;
	target->s = selected;
	target++;

	target->x = x + radius;
	target->y = y - radius;
	target->u = 1.0f;
	target->v = -1.0f;
	target->s = selected;
	target++;

	target->x = x + radius;
	target->y = y + radius;
	target->u = 1.0f;
	target->v = 1.0f;
	target->s = selected;
	target++;

	target->x = x - radius;
	target->y = y + radius;
	target->u = -1.0f;
	target->v = 1.0f;
	target->s = selected;
	target++;

	return target;
}

static Edge* CreateEdge(Edge* target, float x1, float y1, float x2, float y2, float half_width) {
	float theta = atan2(y2 - y1, x2 - x1);
	float cos_theta = cos(theta);
	float sin_theta = sin(theta);

	target->x = x1 + half_width * sin_theta;
	target->y = y1 - half_width * cos_theta;
	target->u = -1.0f;
	target->v = -1.0f;
	target++;

	target->x = x2 + half_width * sin_theta;
	target->y = y2 - half_width * cos_theta;
	target->u = 1.0f;
	target->v = -1.0f;
	target++;

	target->x = x2 - half_width * sin_theta;
	target->y = y2 + half_width * cos_theta;
	target->u = 1.0f;
	target->v = 1.0f;
	target++;

	target->x = x1 - half_width * sin_theta;
	target->y = y1 + half_width * cos_theta;
	target->u = -1.0f;
	target->v = 1.0f;
	target++;

	return target;
}

static void InitQuadIndeces(unsigned int *indices, unsigned int length) {
	unsigned int offset = 0;
	for (size_t i = 0; i < length; i += 6) {
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}
}

static void InitEdgeIndeces(unsigned int vertex_indices[MAX_EDGE_INDEX_COUNT]) {
	for (size_t i = 0; i < MAX_EDGE_INDEX_COUNT; i++) {
		vertex_indices[i] = i;
	}
}

static void EnableBlend() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static void InitModelViewProj(glm::mat4& model, glm::mat4& view, glm::mat4& proj, int width, int height) {
	proj = glm::ortho(0.0f, float(width), 0.0f, float(height), -1.0f, 1.0f);
	view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
}

static glm::vec4 NDCToCoord(float x, float y, glm::mat4& mvp) {
	glm::vec4 coord = glm::inverse(mvp) * glm::vec4(x, y, 0.0f, 1.0f);
	return coord;
}



class Timer {
private:
	std::chrono::time_point<std::chrono::system_clock> m_Start;
	std::chrono::time_point<std::chrono::system_clock> m_End;
	std::chrono::duration<float> m_Elapsed_Seconds;

public:
	Timer():
		m_Start(std::chrono::system_clock::now()), m_End(std::chrono::system_clock::now()), m_Elapsed_Seconds(std::chrono::duration<float>(0.0f)){

	}
	~Timer() {

	};

	float GetSeconds() {
		m_End = std::chrono::system_clock::now();
		m_Elapsed_Seconds = m_End - m_Start;
		return m_Elapsed_Seconds.count();
	}
};