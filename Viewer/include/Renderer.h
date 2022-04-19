#pragma once
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include "Scene.h"
#include "Light.h"
#include "Texture2D.h"
#include "InitShader.h"
#include "ShaderProgram.h"

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();

	void LoadShaders();
	void LoadTexture();
	void LoadNormalMap(std::string& path);

	void Render(std::shared_ptr<Scene> scene);

	int GetViewportWidth() const { return viewport_width; }
	int GetViewportHeight() const { return viewport_height; }
	void SetViewportSize(int viewport_width, int viewport_height);

private:
	int viewport_width, viewport_height;
	glm::mat4 viewport_transformation;

	ShaderProgram light_shader;
	ShaderProgram color_shader;

	std::string texture_path;
	Texture2D texture;
	Texture2D normal_map;
};