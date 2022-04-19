#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Structs.h"
#include "MathUtils.h"

class TransformableObject
{
public:
	TransformableObject();

	TransformationVectors& GetVectors(COORDINATES coordinates);
	void Refresh();
	void Reset(COORDINATES coordinates);

	glm::mat4 GetTranslation(COORDINATES coordinates);
	glm::mat4 GetScale(COORDINATES coordinates);
	glm::mat4 GetRotation(COORDINATES coordinates, AXIS axis);
	glm::mat4 GetRotation(COORDINATES coordinates);
	glm::mat4 GetComposition(COORDINATES coordinates, bool translation, bool scale, bool x_rotation, bool y_rotation, bool z_rotation);
	glm::mat4 GetComposition(bool translation, bool scale, bool x_rotation, bool y_rotation, bool z_rotation);
	glm::mat4 GetTransformation();
	glm::mat4& GetTransformation(COORDINATES coordinates);

	bool model_scale_all;
	bool world_scale_all;
private:
	TransformationVectors model_vectors;
	TransformationVectors world_vectors;
	TransformationMatrices model_matrices;
	TransformationMatrices world_matrices;

	void Validate(COORDINATES coordinates);
	TransformationMatrices& GetTransformations(COORDINATES coordinates);
	void Refresh(COORDINATES coordinates);
};