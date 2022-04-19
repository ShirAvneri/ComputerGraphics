#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Structs.h"
#include "Enums.h"

class MathUtils
{
public:
	// Transformations required helper functions
	static glm::mat4 GetRotationTransformation(const AXIS type, float degree);
	static void ApplyTransformation(glm::vec3& vertex, glm::mat4 transformation);
	static void ApplyTransformation(std::vector<glm::vec3>& vertices, glm::mat4 transformation);
	static glm::ivec2 TriangleMin(const Triangle& triangle);
	static glm::ivec2 TriangleMax(const Triangle& triangle);

	// Coloring required helper functions
	static float GetRandom();
	static bool IsPointInTriangle(glm::vec2 point, const Triangle& triangle);
	static float LinearInterpolation(const Triangle& triangle, const glm::vec2& point);
	static glm::vec3 LinearInterpolation(const Triangle& triangle, const Triangle& normals, const glm::vec2& point);


private:
	static float TriangleArea(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
};