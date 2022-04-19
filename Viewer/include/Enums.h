#pragma once

enum class COORDINATES { MODEL = 0, WORLD = 1 };
enum class AXIS { X, Y, Z };
enum class TRANSFORMATION { ID, TRANSLATION, SCALING, ROTATION, REFLECTION };
enum class PROJECTION { ORTHO, PERSPECTIVE };
enum class FILL { EMPTY = 0, GREY = 1, COLOR = 2 };
enum class SHADING { FLAT = 0, PHONG = 1 };
enum class CANONIAL_PROJECTION { NONE, PLANE = 1, CYLINDER = 2, SPHERE = 3 };
enum class DIRECTION { POS = 1, NEG = -1 };
enum class SHORTCUTS
{ // Use the ASCII table for more key codes (https://www.asciitable.com/)
	MODEL_UPPER = 77, MODEL_LOWER = 109, 
	WORLD_UPPER = 87, WORLD_LOWER = 119,
	TRANSLATION_Z_DEC = 320, TRANSLATION_Z_INC = 330,
	SCALING_DEC = 331, SCALING_INC = 332,
	ROTATION_X_DEC = 321, ROTATION_X_INC = 323,
	ROTATION_Y_DEC = 324, ROTATION_Y_INC = 326,
	ROTATION_Z_DEC = 327, ROTATION_Z_INC = 329
};