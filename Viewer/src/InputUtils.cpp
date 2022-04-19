#include "InputUtils.h"

const float InputUtils::TRANSLATION_STEP = 3.0f;
const float InputUtils::SCALING_STEP = 0.01f;
const float InputUtils::ROTATION_STEP = 10.0f;

std::string InputUtils::QuickAccess(std::string name)
{
	try
	{
		namespace fs = std::filesystem;
		fs::path rel_path = "../Data/" + name + ".obj";
		fs::path abs_path = fs::absolute(rel_path);
		if (fs::exists(abs_path))
			return abs_path.u8string();
		else
			throw;
	}
	catch (const std::exception&)
	{
		throw "File could not be loaded";
	}
}

void InputUtils::TranslateMesh(MeshModel& mesh, COORDINATES coordinates, AXIS axis, DIRECTION dir)
{
	short direction = (short)dir;
	auto& translation = mesh.GetVectors(coordinates).translation;

	switch (axis) {
	case AXIS::X:
		translation.x += direction * InputUtils::TRANSLATION_STEP;
		break;
	case AXIS::Y:
		translation.y += direction * InputUtils::TRANSLATION_STEP;
		break;
	case AXIS::Z:
		translation.z += direction * InputUtils::TRANSLATION_STEP;
		break;
	default:
		throw("Unknown axis");
	}
}

void InputUtils::ScaleMesh(MeshModel& mesh, COORDINATES coordinates, DIRECTION dir)
{
	auto& scale = mesh.GetVectors(coordinates).scale;
	auto val = (short)dir * SCALING_STEP;

	scale.x += val;
	scale.y += val;
	scale.z += val;
}

void InputUtils::RotateMesh(MeshModel& mesh, COORDINATES coordinates, AXIS axis, DIRECTION dir)
{
	short direction = (short)dir;
	auto& rotation = mesh.GetVectors(coordinates).rotation;

	switch (axis) {
	case AXIS::X:
		rotation.x += direction * ROTATION_STEP;
		if (rotation.x < 0.0)
			rotation.x += 360.0;
		if (rotation.x > 360.0)
			rotation.x -= 360.0;

		break;
	case AXIS::Y:
		rotation.y += direction * ROTATION_STEP;
		if (rotation.y < 0.0)
			rotation.y += 360.0;
		if (rotation.y > 360.0)
			rotation.y -= 360.0;
		break;
	case AXIS::Z:
		rotation.z += direction * ROTATION_STEP;
		if (rotation.z < 0.0)
			rotation.z += 360.0;
		if (rotation.z > 360.0)
			rotation.z -= 360.0;
		break;
	default:
		throw("Unknown axis");
	}
}