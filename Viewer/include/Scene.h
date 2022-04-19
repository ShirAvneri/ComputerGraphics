#pragma once
#include <vector>
#include <memory>
#include <string>
#include <imgui/imgui.h>
#include "MeshModel.h"
#include "Camera.h"
#include "Light.h"

using namespace std;

class Scene {
public:
	Scene();

	void AddModel(const shared_ptr<MeshModel>& mesh_model);
	size_t GetModelCount() const;
	MeshModel& GetModel(int index) const;
	MeshModel& GetActiveModel() const;
	void SetActiveModelIndex(size_t index);
	size_t GetActiveModelIndex() const;
	bool HasModels() const { return !mesh_models.empty(); }
	void RemoveModel(size_t index);

	void AddCamera(const shared_ptr<Camera>& camera);
	size_t GetCameraCount() const;
	Camera& GetCamera(size_t index) const;
	Camera& GetActiveCamera() const;
	void SetActiveCameraIndex(size_t index);
	size_t GetActiveCameraIndex() const;

	void AddLight();
	void AddLight(glm::vec3 position);
	size_t GetLightCount() const;
	Light& GetLight(size_t index) const;
	Light& GetActiveLight() const;
	void SetActiveLightIndex(size_t index);
	size_t GetActiveLightIndex() const;
	bool HasLights() const { return !lights.empty(); }
	void RemoveLight(size_t index);

	glm::vec3 GetModelColor() const;
	glm::vec3 GetAxisColor(AXIS axis, bool is_model) const;
	glm::vec3 GetBoundingBoxColor() const;
	glm::vec3 GetNormalsColor(bool is_face) const;

	void ClearAll();
	void ClearAllModels();
	void ClearAllLights();

	Features features_input;
	glm::vec3 model_color;
	glm::vec3 model_axes_colors[3];
	glm::vec3 world_axes_colors[3];
	glm::vec3 bounding_box_color;
	glm::vec3 normals_colors[2]; // [0]: face, [1]: vertex
private:	
	vector<shared_ptr<MeshModel>> mesh_models;
	vector<shared_ptr<Camera>> cameras;
	vector<shared_ptr<Light>> lights;
	
	size_t active_model_index;
	size_t active_camera_index;
	size_t active_light_index;
	int next_light_id;
};