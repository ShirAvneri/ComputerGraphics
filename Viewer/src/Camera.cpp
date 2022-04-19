#include "Camera.h"
#include <MathUtils.h>

Camera::Camera(glm::vec3 eye, glm::vec3 at, glm::vec3 up) :
	viewport_width(0),
	viewport_height(0),
	projection_type(PROJECTION::PERSPECTIVE),
	view_transformation(glm::identity<glm::mat4>()),
	inv_view_transformation(glm::identity<glm::mat4>()),
	projection_transformation(glm::identity<glm::mat4>()),
	position(glm::vec3(0.0f, 0.0f, 0.0f))
{
	viewing = { glm::vec3(eye), glm::vec3(at), glm::vec3(up) };
	view_volume = { -640.0f, 640.0f, -360.0f, 360.0f };
	perspective = { 45.0f, 1280.0f / 720.0f, 1.0f, 1000.0f, 1.0f };
}

void Camera::RefreshLookAt()
{
	inv_view_transformation = glm::lookAt(viewing.eye, viewing.at, viewing.up);
	view_transformation = glm::inverse(inv_view_transformation);
}

void Camera::Refresh(int viewport_width, int viewport_height) 
{
	if (this->viewport_width != viewport_width || this->viewport_height != viewport_height) 
	{
		this->viewport_width = viewport_width;
		this->viewport_height = viewport_height;
		RefreshViewVolume();
	}

	RefreshLookAt();
	RefreshProjection();
	TransformableObject::Refresh();
}

glm::mat4 Camera::GetMVPTransformation()
{
	auto camera_translation = GetTranslation(COORDINATES::MODEL);
	auto camera_rotation = GetRotation(COORDINATES::MODEL);
	auto world_translation = GetTranslation(COORDINATES::WORLD);
	auto world_rotation = GetRotation(COORDINATES::WORLD);
	auto transformation = glm::inverse(camera_rotation) * camera_translation * view_transformation * glm::inverse(world_rotation) * world_translation;
	position = transformation * glm::vec4(1.0f);

	return projection_transformation * glm::inverse(transformation);
}

glm::mat4 Camera::GetZoom()
{
	perspective.zoom = perspective.zoom < 0.0f ? 0.0f : perspective.zoom;
	if (projection_type == PROJECTION::PERSPECTIVE)
		return glm::mat4(glm::scale(glm::vec3(perspective.zoom, perspective.zoom, perspective.zoom)));
	else
		return glm::identity<glm::mat4>();
}

void Camera::RefreshProjection()
{
	switch (projection_type)
	{
	case PROJECTION::ORTHO:
		projection_transformation = glm::ortho(view_volume.left, view_volume.right, view_volume.bottom, view_volume.top);
		break;
	case PROJECTION::PERSPECTIVE:
		projection_transformation = glm::perspective(glm::radians(perspective.fovy), perspective.aspect, perspective.z_near, perspective.z_far);
		break;
	default:
		break;
	}
}

void Camera::RefreshViewVolume()
{
	float half_width = viewport_width / 2.0f;
	float half_height = viewport_height / 2.0f;

	view_volume.left = -half_width;
	view_volume.right = half_width;
	view_volume.bottom = -half_height;
	view_volume.top = half_height;

	perspective.aspect = (float)viewport_width / viewport_height;
}