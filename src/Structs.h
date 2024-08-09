#pragma once

struct Vertex {
	float x, y, u, v, s;
};

struct Element {
	float x, y;
};

struct Edge {
	float x, y, u, v;
};

enum struct Type {
	Vertex,
	Edge,
	Background
};