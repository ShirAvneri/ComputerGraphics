#include "TransformableObject.h"

TransformableObject::TransformableObject() : model_scale_all(true), world_scale_all(true)
{
	model_matrices = {
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>()
	};

	world_matrices = {
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>(),
		glm::identity<glm::mat4>()
	};

	model_vectors = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
	world_vectors = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
}

TransformationVectors& TransformableObject::GetVectors(COORDINATES coordinates)
{
	switch (coordinates)
	{
	case COORDINATES::MODEL:
		return model_vectors;
	case COORDINATES::WORLD:
		return world_vectors;
	default:
		throw("Unknown coordinates");
	}
}

TransformationMatrices& TransformableObject::GetTransformations(COORDINATES coordinates)
{
	switch (coordinates)
	{
	case COORDINATES::MODEL:
		return model_matrices;
	case COORDINATES::WORLD:
		return world_matrices;
	default:
		throw("Unknown coordinates");
	}
}

void TransformableObject::Validate(COORDINATES coordinates)
{
	TransformationVectors& input = GetVectors(coordinates);
	input.scale.x = input.scale.x < 0 ? 0 : input.scale.x;
	input.scale.y = input.scale.y < 0 ? 0 : input.scale.y;
	input.scale.z = input.scale.z < 0 ? 0 : input.scale.z;
}

void TransformableObject::Refresh(COORDINATES coordinates)
{
	Validate(coordinates);

	TransformationVectors& input = GetVectors(coordinates);
	TransformationMatrices& transformations = GetTransformations(coordinates);

	transformations.translation = glm::translate(input.translation);
	transformations.scale = glm::scale(input.scale);
	transformations.rotation_x = MathUtils::GetRotationTransformation(AXIS::X, input.rotation.x);
	transformations.rotation_y = MathUtils::GetRotationTransformation(AXIS::Y, input.rotation.y);
	transformations.rotation_z = MathUtils::GetRotationTransformation(AXIS::Z, input.rotation.z);
	transformations.rotation = transformations.rotation_z * transformations.rotation_y * transformations.rotation_x;
	transformations.composition = transformations.translation * transformations.scale * transformations.rotation;
}

void TransformableObject::Refresh()
{
	Refresh(COORDINATES::MODEL);
	Refresh(COORDINATES::WORLD);
}

void TransformableObject::Reset(COORDINATES coordinates)
{

	switch (coordinates)
	{
	case COORDINATES::MODEL:
		model_matrices = {
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>()
		};
		model_vectors = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
		break;
	case COORDINATES::WORLD:
		world_matrices = {
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>(),
			glm::identity<glm::mat4>()
		};
		world_vectors = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
		break;
	default:
		throw("Unknown coordinates");
	}
}

glm::mat4 TransformableObject::GetTranslation(COORDINATES coordinates)
{
	switch (coordinates)
	{
	case COORDINATES::MODEL:
		return model_matrices.translation;
	case COORDINATES::WORLD:
		return world_matrices.translation;
	default:
		throw("Unknown coordinates");
	}
}

glm::mat4 TransformableObject::GetScale(COORDINATES coordinates)
{
	switch (coordinates)
	{
	case COORDINATES::MODEL:
		return model_matrices.scale;
	case COORDINATES::WORLD:
		return world_matrices.scale;
	default:
		throw("Unknown coordinates");
	}
}

glm::mat4 TransformableObject::GetRotation(COORDINATES coordinates, AXIS axis)
{
	TransformationMatrices& transformations = GetTransformations(coordinates);
	switch (axis)
	{
	case AXIS::X:
		return transformations.rotation_x;
	case AXIS::Y:
		return transformations.rotation_y;
	case AXIS::Z:
		return transformations.rotation_z;
	default:
		throw("Unknown axis");
	}
}

glm::mat4 TransformableObject::GetRotation(COORDINATES coordinates)
{
	switch (coordinates)
	{
	case COORDINATES::MODEL:
		return world_matrices.rotation;
	case COORDINATES::WORLD:
		return model_matrices.rotation;
	default:
		throw("Unknown coordinates");
	}
}

glm::mat4& TransformableObject::GetTransformation(COORDINATES coordinates)
{
	switch (coordinates)
	{
	case COORDINATES::MODEL:
		return world_matrices.composition;
	case COORDINATES::WORLD:
		return model_matrices.composition;
	default:
		throw("Unknown coordinates");
	}
}

glm::mat4 TransformableObject::GetTransformation()
{
	return world_matrices.composition * model_matrices.composition;
}

glm::mat4 TransformableObject::GetComposition(COORDINATES coordinates, bool translation, bool scale, bool x_rotation, bool y_rotation, bool z_rotation)
{
	auto composition = glm::identity<glm::mat4>();
	TransformationMatrices& transformations = GetTransformations(coordinates);

	if (translation && scale && x_rotation && y_rotation && z_rotation)
		return transformations.composition;

	if (x_rotation && y_rotation && z_rotation)
		composition = transformations.rotation;
	else {
		if (x_rotation)
			composition = transformations.rotation_x;
		if (y_rotation)
			composition = transformations.rotation_y * composition;
		if (z_rotation)
			composition = transformations.rotation_z * composition;
	}

	if (scale)
		composition = transformations.scale * composition;

	if (translation)
		composition = transformations.translation * composition;

	return composition;
}

glm::mat4 TransformableObject::GetComposition(bool translation, bool scale, bool x_rotation, bool y_rotation, bool z_rotation)
{
	auto model_composition = GetComposition(COORDINATES::MODEL, translation, scale, x_rotation, y_rotation, z_rotation);
	auto world_composition = GetComposition(COORDINATES::WORLD, translation, scale, x_rotation, y_rotation, z_rotation);
	return world_composition * model_composition;
}

