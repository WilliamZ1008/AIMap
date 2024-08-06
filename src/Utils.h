#pragma once

#include <GLFW/glfw3.h>

#include <math.h>

#include "Config.h"
#include "Structs.h"

static Vertex* CreateQuad(Vertex* target, float x, float y, float radius) {
	target->x = x - radius;
	target->y = y - radius;
	target->u = -1.0f;
	target->v = -1.0f;
	target++;

	target->x = x + radius;
	target->y = y - radius;
	target->u = 1.0f;
	target->v = -1.0f;
	target++;

	target->x = x + radius;
	target->y = y + radius;
	target->u = 1.0f;
	target->v = 1.0f;
	target++;

	target->x = x - radius;
	target->y = y + radius;
	target->u = -1.0f;
	target->v = 1.0f;
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

static void InitQuadIndeces(unsigned int vertex_indices[MAX_VERTEX_INDEX_COUNT]) {
	unsigned int offset = 0;
	for (size_t i = 0; i < MAX_VERTEX_INDEX_COUNT; i += 6) {
		vertex_indices[i + 0] = offset + 0;
		vertex_indices[i + 1] = offset + 1;
		vertex_indices[i + 2] = offset + 2;

		vertex_indices[i + 3] = offset + 2;
		vertex_indices[i + 4] = offset + 3;
		vertex_indices[i + 5] = offset + 0;

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