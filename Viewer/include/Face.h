#pragma once
#include "vector"
#include <string>
#include <glm/glm.hpp>

class Face
{
public:
	Face(std::istream& issLine);
	size_t GetVertexIndex(size_t index) const;
	size_t GetNormalIndex(size_t index) const;
	size_t GetTextureIndex(size_t index) const;

private:
	std::vector<int> vertex_indices;
	std::vector<int> normal_indices;
	std::vector<int> texture_indices;
};