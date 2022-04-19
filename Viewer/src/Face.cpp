#pragma once
#include <istream>
#include "Face.h"

Face::Face(std::istream& issLine)
{
	vertex_indices  = { 0,0,0 };
	normal_indices  = { 0,0,0 };
	texture_indices = { 0,0,0 };

	char c;
	for (int i = 0; i < 3; i++)
	{
		issLine >> std::ws >> vertex_indices.at(i) >> std::ws;
		if (issLine.peek() != '/')
		{
			continue;
		}
		
		issLine >> c >> std::ws;
		if (issLine.peek() == '/')
		{
			issLine >> c >> std::ws >> normal_indices.at(i);
			continue;
		}

		issLine >> texture_indices.at(i);
		if (issLine.peek() != '/')
		{
			continue;
		}
		
		issLine >> c >> normal_indices.at(i);
	}
}

size_t Face::GetVertexIndex(size_t internal_index) const
{
	return vertex_indices[internal_index];
}

size_t Face::GetNormalIndex(size_t internal_index) const
{
	return normal_indices[internal_index];
}

size_t Face::GetTextureIndex(size_t internal_index) const
{
	return texture_indices[internal_index];
}
