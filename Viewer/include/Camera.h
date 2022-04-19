#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "TransformableObject.h"

class Camera : public TransformableObject
{
public:
	Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 up);

	void Refresh(int viewport_width, int viewport_height);

	PROJECTION GetProjectionType() const { return projection_type; }
	void SetProjectionType(const PROJECTION projection_type) { this->projection_type = projection_type; }
	glm::mat4 GetZoom();

	glm::mat4 GetViewTransformation() { return inv_view_transformation; }
	glm::mat4 GetProjectionTransformation() { return projection_transformation; }
	glm::mat4 GetMVPTransformation();
	glm::vec3 GetPosition() { return position; }

	ViewingVectors viewing;
	ViewVolume view_volume;
	Perspective perspective;

private:
	glm::mat4 view_transformation;
	glm::mat4 inv_view_transformation;
	PROJECTION projection_type;
	glm::mat4 projection_transformation;
	glm::vec3 position;
	int viewport_width, viewport_height;

	void RefreshViewVolume();
	void RefreshLookAt();
	void RefreshProjection();
};
