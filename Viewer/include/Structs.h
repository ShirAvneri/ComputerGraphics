#pragma once
#include "Enums.h"

typedef struct TransformationMatrices
{
	glm::mat4 translation;
	glm::mat4 scale;
	glm::mat4 rotation_x;
	glm::mat4 rotation_y;
	glm::mat4 rotation_z;
	glm::mat4 rotation;
	glm::mat4 composition;
} TransformationMatrices;

typedef struct TransformationVectors
{
	glm::vec3 translation;
	glm::vec3 scale;
	glm::vec3 rotation;
} TransformationVectors;

typedef struct ViewingVectors
{
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
} ViewingVectors;

typedef struct ViewVolume
{
	float left;
	float right;
	float bottom;
	float top;
} ViewVolume;

typedef struct Perspective
{
	float fovy;
	float aspect;
	float z_near;
	float z_far;
	float zoom;
} Perspective;

typedef struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
} Material;

typedef struct Features 
{
	bool display_model_axes;
	bool display_world_axes;
	bool display_bounding_box;
	bool display_face_normals;
	bool display_vertex_normals;
	bool draw_bounding_rect;
	FILL triangles_fill;
	SHADING shading_type;
	bool gaussian_blur;
	int kernel_width;
	int kernel_height;
	float kernel_sigma;
	std::string texture_path;
	CANONIAL_PROJECTION projection_type;
	bool apply_toon_shading;
	float toon_shading_level;
} Features;

typedef struct Triangle 
{
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
} Triangle;

typedef struct Vertex
{
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec2 texture_coords;
} Vertex;

typedef struct CanonialProjection
{
	CANONIAL_PROJECTION type;
	float scale;
	float offset;
} CanonialProjection;