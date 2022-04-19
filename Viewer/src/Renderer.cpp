#include "Renderer.h"

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width(viewport_width),
	viewport_height(viewport_height),
	viewport_transformation(glm::mat4(
		viewport_width / 2.0f, 0.0f, 0.0f, 0.0f,
		0.0f, viewport_height / 2.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		viewport_width / 2.0f, viewport_height / 2.0f, -1.0f, 1.0f))
{
	texture_path = "../Data/Textures/Crate.jpg";
	LoadShaders();
	LoadTexture();
}

Renderer::~Renderer() {

}

void Renderer::LoadShaders()
{
	color_shader.loadShaders("vshader.glsl", "fshader.glsl");
}

void Renderer::LoadTexture()
{
	texture.loadTexture(texture_path);
}

void Renderer::LoadNormalMap(std::string& path)
{
	normal_map.loadTexture(path);
}

void Renderer::Render(std::shared_ptr<Scene> scene)
{
	if (scene->features_input.texture_path != texture_path)
	{
		texture_path = scene->features_input.texture_path;
		LoadTexture();
	}

	if (scene->HasModels())
	{
		auto& camera = scene->GetActiveCamera();
		camera.Refresh(viewport_width, viewport_height);

		auto& active_model = scene->GetActiveModel();
		active_model.Refresh();

		color_shader.use();

		color_shader.setUniform("model", active_model.GetTransformation());
		color_shader.setUniform("view", camera.GetViewTransformation());
		color_shader.setUniform("projection", camera.GetProjectionTransformation());

		color_shader.setUniform("camera_pos", glm::vec4(scene->GetActiveCamera().GetPosition(), 1.0f));

		color_shader.setUniform("material.ambient", glm::vec4(active_model.material.ambient, 1.0f));
		color_shader.setUniform("material.diffuse", glm::vec4(active_model.material.diffuse, 1.0f));
		color_shader.setUniform("material.specular", glm::vec4(active_model.material.specular, 1.0f));
		color_shader.setUniform("material.shininess", active_model.material.shininess);

		color_shader.setUniform("projection_type", 0);
		color_shader.setUniform("projection_scale", 1.0f);
		color_shader.setUniform("projection_offset", 0.0f);

		color_shader.setUniform("apply_toon_shading", scene->features_input.apply_toon_shading);

		int light_count = scene->GetLightCount();
		color_shader.setUniform("lights.size", light_count);
		for (int i = 0; i < light_count; i++) 
		{
			auto& light = scene->GetLight(i);

			std::ostringstream light_stream;
			light_stream << "lights.list[" << i << "].";

			std::ostringstream ambient;
			ambient << light_stream.str() << "ambient";

			std::ostringstream diffuse;
			diffuse << light_stream.str() << "diffuse";

			std::ostringstream specular;
			specular << light_stream.str() << "specular";

			std::ostringstream pos;
			pos << light_stream.str() << "pos";

			color_shader.setUniform(ambient.str().data(), glm::vec4(light.ambient, 1.0f));
			color_shader.setUniform(diffuse.str().data(), glm::vec4(light.diffuse, 1.0f));
			color_shader.setUniform(specular.str().data(), glm::vec4(light.specular, 1.0f));
			color_shader.setUniform(pos.str().data(), glm::vec4(light.GetPosition(), 1.0f));
		}

		color_shader.setUniform("material.texture_map", 0);

		//texture.bind(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(active_model.GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, active_model.GetModelVerticesCount());
		glBindVertexArray(0);

		//texture.unbind(0);
	}
}

void Renderer::SetViewportSize(int viewport_width, int viewport_height)
{
	this->viewport_width = viewport_width;
	this->viewport_height = viewport_height;
}