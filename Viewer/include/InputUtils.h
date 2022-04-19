#pragma once
#include <fstream>
#include <filesystem>
#include "MeshModel.h"

class InputUtils
{
public:
	static const float TRANSLATION_STEP;
	static const float SCALING_STEP;
	static const float ROTATION_STEP;

	static std::string QuickAccess(std::string name);
	static void TranslateMesh(MeshModel& mesh, COORDINATES coordinates, AXIS axis, DIRECTION dir);
	static void ScaleMesh(MeshModel& mesh, COORDINATES coordinates, DIRECTION dir);
	static void RotateMesh(MeshModel& mesh, COORDINATES coordinates, AXIS axis, DIRECTION dir);
};