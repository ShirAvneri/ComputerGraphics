#include "MathUtils.h"

glm::mat4 MathUtils::GetRotationTransformation(const AXIS type, float degree)
{
	double cos_degree = cos(degree * 3.14 / 180);
	double sin_degree = sin(degree * 3.14 / 180);
	switch (type)
	{
	case AXIS::X:
		return glm::mat4(
			1, 0, 0, 0,
			0, cos_degree, sin_degree, 0,
			0, -sin_degree, cos_degree, 0,
			0, 0, 0, 1);
	case AXIS::Y:
		return glm::mat4(
			cos_degree, 0, -sin_degree, 0,
			0, 1, 0, 0,
			sin_degree, 0, cos_degree, 0,
			0, 0, 0, 1);
	case AXIS::Z:
		return glm::mat4(
			cos_degree, -sin_degree, 0, 0,
			sin_degree, cos_degree, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	default:
		return glm::mat4(1.0);
	}
}

void MathUtils::ApplyTransformation(glm::vec3& vertex, glm::mat4 transformation)
{
	glm::vec4 transformed = transformation * glm::vec4(vertex, 1.0f);
	vertex = transformed / transformed.w;
}

void MathUtils::ApplyTransformation(std::vector<glm::vec3>& vertices, glm::mat4 transformation)
{
	size_t vertices_count = vertices.size();
	for (size_t i = 0; i < vertices_count; i++)
	{
		auto& current = vertices[i];
		glm::vec4 transformed = transformation * glm::vec4(current, 1.0f);
		current = transformed / transformed.w;
	}
}

glm::ivec2 MathUtils::TriangleMin(const Triangle& triangle)
{
	return glm::ivec2(glm::min(glm::floor(triangle.v3), glm::min(glm::floor(triangle.v2), glm::floor(triangle.v1))));
}

glm::ivec2 MathUtils::TriangleMax(const Triangle& triangle)
{
	return glm::ivec2(glm::max(glm::ceil(triangle.v3), glm::max(glm::ceil(triangle.v2), glm::ceil(triangle.v1))));
}

float MathUtils::GetRandom()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

bool MathUtils::IsPointInTriangle(glm::vec2 point, const Triangle& triangle)
{
	float x = point.x - triangle.v1.x, y = point.y - triangle.v1.y;
	bool temp = y * (triangle.v2.x - triangle.v1.x) - x * (triangle.v2.y - triangle.v1.y) > 0;

	if (((triangle.v3.x - triangle.v2.x) * (point.y - triangle.v2.y) - (triangle.v3.y - triangle.v2.y) * (point.x - triangle.v2.x) > 0) ^ temp ||
		!((y * (triangle.v3.x - triangle.v1.x) - x * (triangle.v3.y - triangle.v1.y) > 0) ^ temp))
		return false;

	return true;
}

float MathUtils::TriangleArea(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
{
	float area = p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y)+ p3.x * (p1.y - p2.y);
	return area < 0 ? -0.5 * area : 0.5 * area;
}

float MathUtils::LinearInterpolation(const Triangle& triangle, const glm::vec2& point)
{
	float A1 = MathUtils::TriangleArea(triangle.v2, triangle.v3, point);
	float A2 = MathUtils::TriangleArea(triangle.v1, triangle.v3, point);
	float A3 = MathUtils::TriangleArea(triangle.v2, triangle.v1, point);
	float A = A1 + A2 + A3;

	return triangle.v1.z * (A1 / A) + triangle.v2.z * (A2 / A) + triangle.v3.z * (A3 / A);
}

glm::vec3 MathUtils::LinearInterpolation(const Triangle& triangle, const Triangle& normals, const glm::vec2& point)
{
	float A1 = MathUtils::TriangleArea(triangle.v2, triangle.v3, point);
	float A2 = MathUtils::TriangleArea(triangle.v1, triangle.v3, point);
	float A3 = MathUtils::TriangleArea(triangle.v2, triangle.v1, point);
	float A = A1 + A2 + A3;

	return glm::vec3(normals.v1 * (A1 / A) + normals.v2 * (A2 / A) + normals.v3 * (A3 / A));
}