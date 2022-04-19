#include "Scene.h"

Scene::Scene() : active_model_index(0), active_camera_index(0), active_light_index(0), next_light_id(1)
{
	model_color = glm::vec3(0.0f, 0.0f, 0.0f);
	model_axes_colors[0] = glm::vec3(1.0f, 0.0f, 0.0f); model_axes_colors[1] = glm::vec3(0.2f, 0.8f, 0.2f); model_axes_colors[2] = glm::vec3(0.0f, 0.0f, 1.0f);
	world_axes_colors[0] = glm::vec3(1.0f, 0.0f, 0.0f); world_axes_colors[1] = glm::vec3(0.2f, 0.8f, 0.2f); world_axes_colors[2] = glm::vec3(0.0f, 0.0f, 1.0f);
	bounding_box_color = glm::vec3(1.0f, 0.0f, 0.0f);
	normals_colors[0] = glm::vec3(0.2f, 0.8f, 0.5f); normals_colors[1] = glm::vec3(0.2f, 0.8f, 0.5f);
	features_input = { false, false, false, false, false, false, FILL::COLOR, SHADING::PHONG, false, 15, 15, 10.0f, "../Data/Textures/Crate.jpg", CANONIAL_PROJECTION::SPHERE, true, 1.0f };
	cameras.emplace_back(std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 500.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models.push_back(mesh_model);
}

size_t Scene::GetModelCount() const
{
	return mesh_models.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models[active_model_index];
}

size_t Scene::GetActiveModelIndex() const
{
	return active_model_index;
}

void Scene::SetActiveModelIndex(size_t index)
{
	active_model_index = index;
}

void Scene::RemoveModel(size_t index)
{
	if (index < 0 || index > mesh_models.size())
		return;

	mesh_models.erase(mesh_models.begin() + index);
	active_model_index = 0;
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras.push_back(camera);
}

size_t Scene::GetCameraCount() const
{
	return cameras.size();
}

Camera& Scene::GetCamera(size_t index) const
{
	return *cameras[index];
}

Camera& Scene::GetActiveCamera() const
{
	return *cameras[active_camera_index];
}

void Scene::SetActiveCameraIndex(size_t index)
{
	active_camera_index = index;
}

size_t Scene::GetActiveCameraIndex() const
{
	return active_camera_index;
}

void Scene::AddLight()
{
	lights.emplace_back(std::make_shared<Light>(next_light_id++, glm::vec3(0.0f, 0.0f, 0.0f)));
}

void Scene::AddLight(glm::vec3 position)
{
	lights.emplace_back(std::make_shared<Light>(next_light_id++, position));
}

size_t Scene::GetLightCount() const
{
	return lights.size();
}

Light& Scene::GetLight(size_t index) const
{
	return *lights[index];
}

Light& Scene::GetActiveLight() const
{
	return *lights[active_light_index];
}

void Scene::SetActiveLightIndex(size_t index)
{
	active_light_index = index;
}

size_t Scene::GetActiveLightIndex() const
{
	return active_light_index;
}

void Scene::RemoveLight(size_t index)
{
	if (index < 0 || index > lights.size())
		return;

	lights.erase(lights.begin() + index);
	active_light_index = 0;
}

glm::vec3 Scene::GetModelColor() const
{
	return model_color;
}

glm::vec3 Scene::GetAxisColor(AXIS axis, bool is_model) const
{
	switch (axis)
	{
	case AXIS::X:
		if (is_model)
			return model_axes_colors[0];
		else
			return world_axes_colors[0];
	case AXIS::Y:
		if (is_model)
			return model_axes_colors[1];
		else
			return world_axes_colors[1];
	case AXIS::Z:
		if (is_model)
			return model_axes_colors[2];
		else
			return world_axes_colors[2];
	}
}

glm::vec3 Scene::GetBoundingBoxColor() const
{
	return glm::vec3(bounding_box_color.x, bounding_box_color.y, bounding_box_color.z);
}

glm::vec3 Scene::GetNormalsColor(bool is_face) const
{
	if (is_face)
		return normals_colors[0];
	else
		return normals_colors[1];
}

void Scene::ClearAll()
{
	mesh_models.clear();
	active_model_index = 0;

	cameras.clear();
	active_camera_index = 0;
	cameras.emplace_back(std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 1000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	lights.clear();
	active_light_index = 0;

	model_color = glm::vec3(0.0f, 0.0f, 0.0f);
	model_axes_colors[0] = glm::vec3(1.0f, 0.0f, 0.0f); model_axes_colors[1] = glm::vec3(0.2f, 0.8f, 0.2f); model_axes_colors[2] = glm::vec3(0.0f, 0.0f, 1.0f);
	world_axes_colors[0] = glm::vec3(1.0f, 0.0f, 0.0f); world_axes_colors[1] = glm::vec3(0.2f, 0.8f, 0.2f); world_axes_colors[2] = glm::vec3(0.0f, 0.0f, 1.0f);
	bounding_box_color = glm::vec3(1.0f, 0.0f, 0.0f);
	normals_colors[0] = glm::vec3(0.2f, 0.8f, 0.5f); normals_colors[1] = glm::vec3(0.2f, 0.8f, 0.5f);
	features_input = { false, false, false, false, false, false, FILL::COLOR };
}

void Scene::ClearAllModels() {
	mesh_models.clear();
	active_model_index = 0;
}

void Scene::ClearAllLights()
{
	lights.clear();
	active_light_index = 0;
	next_light_id = 1;
}
