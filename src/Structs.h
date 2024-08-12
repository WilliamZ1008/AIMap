#pragma once

#include <string>

struct Vertex {
	float x, y, u, v, s;
};

struct Element {
	float x, y;
	unsigned int t;
};

struct ElementInfo {
	std::string en_name;
	std::string cn_name;
	std::string abbreviation;
	std::string en_induction;
	std::string cn_induction;
};

struct Edge {
	float x, y, u, v;
};

enum struct Type {
	Vertex,
	Edge,
	Background
};