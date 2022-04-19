#include "AppWindows.h"

void AppWindows::DrawControllersWindow(ImGuiIO& io, std::shared_ptr<Scene> scene)
{
	static vector<std::pair<string, function<void(std::shared_ptr<Scene>)>>> tabs =
	{
		{ "Model", DrawModelContent },
		{ "Camera", DrawCameraContent },
		{ "Light", DrawLightContent },
		{ "Renderer", DrawRendererContent }
	};
	static function<void(std::shared_ptr<Scene>)> content = tabs[0].second;

	ImGui::BeginMenuBar();
	for (const auto& tab : tabs)
	{
		if (ImGui::MenuItem(tab.first.c_str()))
			content = tab.second;
	}
	ImGui::EndMenuBar();
	content(scene);
}

void AppWindows::DrawTransformationsWidgets(TransformationVectors& input, bool draw_scale, bool& scale_all)
{
	if (ImGui::TreeNode("Translation"))
	{
		ImGui::InputFloat("x", (float*)&input.translation.x, 1.0f, 1.0f, "%.2f");
		ImGui::InputFloat("y", (float*)&input.translation.y, 1.0f, 1.0f, "%.2f");
		ImGui::InputFloat("z", (float*)&input.translation.z, 1.0f, 1.0f, "%.2f");

		ImGui::TreePop();
		ImGui::Separator();
	}
	if (draw_scale) {
		if (ImGui::TreeNode("Scale"))
		{
			ImGui::Checkbox("Scale All", &scale_all);
			if (scale_all)
			{
				float& scale_x = input.scale.x;
				float& scale_y = input.scale.y;
				float& scale_z = input.scale.z;

				float scale_x_prev = scale_x;
				float scale_y_prev = scale_y;
				float scale_z_prev = scale_z;

				if (ImGui::InputFloat("x", &scale_x, 0.1f, 0.1f, "%.2f")) {
					auto diff = scale_x - scale_x_prev;
					scale_y += diff;
					scale_z += diff;
				}
				if (ImGui::InputFloat("y", &scale_y, 0.1f, 0.1f, "%.2f")) {
					auto diff = scale_y - scale_y_prev;
					scale_x += diff;
					scale_z += diff;
				}
				if (ImGui::InputFloat("z##3", &scale_z, 0.1f, 0.1f, "%.2f")) {
					auto diff = scale_z - scale_z_prev;
					scale_x += diff;
					scale_y += diff;
				}
			}
			else {
				float& scale_x = input.scale.x;
				float& scale_y = input.scale.y;
				float& scale_z = input.scale.z;

				ImGui::InputFloat("x", &scale_x, 0.1f, 0.1f, "%.2f");
				ImGui::InputFloat("y", &scale_y, 0.1f, 0.1f, "%.2f");
				ImGui::InputFloat("z", &scale_z, 0.1f, 0.1f, "%.2f");
			}

			ImGui::TreePop();
			ImGui::Separator();
		}
	}
	if (ImGui::TreeNode("Rotation"))
	{
		ImGui::SliderFloat("x", (float*)&input.rotation.x, 0, 360);
		ImGui::SliderFloat("y", (float*)&input.rotation.y, 0, 360);
		ImGui::SliderFloat("z", (float*)&input.rotation.z, 0, 360);

		ImGui::TreePop();
		ImGui::Separator();
	}
}

void AppWindows::DrawTransformationsWidgets(TransformableObject& object, bool draw_scale, bool is_camera)
{
	static int transformation = (int)COORDINATES::MODEL;
	ImGui::RadioButton(!is_camera ? "Model Frame" : "Camera Frame", &transformation, (int)COORDINATES::MODEL);
	ImGui::SameLine();
	ImGui::RadioButton("World Frame", &transformation, (int)COORDINATES::WORLD);
	ImGui::SameLine();

	auto selected = static_cast<COORDINATES>(transformation);
	switch (selected)
	{
	case COORDINATES::MODEL:
		if (ImGui::Button("Reset##1"))
			object.Reset(selected);
		if (ImGui::TreeNode(!is_camera ? "Model Transformations##1" : "Camera Transformations##1"))
		{
			DrawTransformationsWidgets(object.GetVectors(selected), draw_scale, object.model_scale_all);
			ImGui::TreePop();
		}
		break;
	case COORDINATES::WORLD:
		if (ImGui::Button("Reset##2"))
			object.Reset(selected);
		if (ImGui::TreeNode("World Transformations##2"))
		{
			DrawTransformationsWidgets(object.GetVectors(selected), draw_scale, object.world_scale_all);
			ImGui::TreePop();
		}
		break;
	default:
		break;
	}
}

void AppWindows::DrawModelContent(std::shared_ptr<Scene> scene)
{
	if (scene->HasModels()) {
		ImGui::Text("Currently loaded mesh models.");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
		if (ImGui::Button("Remove All"))
			scene->ClearAllModels();
		ImGui::PopStyleColor(3);
	}
	else
		ImGui::Text("More options are available when loading \nmesh models.");

	size_t models_count = scene->GetModelCount();
	for (int i = 0; i < models_count; i++)
	{
		auto& active_model = scene->GetActiveModel();
		ImGui::PushID(i);
		if (ImGui::CollapsingHeader(scene->GetModel(i).GetModelName().c_str()))
		{
			if (scene->GetActiveModelIndex() != i) {
				ImGui::Text("Set mesh model as active model in order for it to \nshow on your display.");
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.8f, 0.8f));
				if (ImGui::Button("Active"))
					scene->SetActiveModelIndex(i);
				ImGui::PopStyleColor(3);

			}
			else {
				ImGui::Text("Mesh model is active and showing on your display.");
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
				if (ImGui::Button("Remove")) {
					scene->RemoveModel(i);
					ImGui::PopStyleColor(3);
					ImGui::PopID();
					break;
				}
				ImGui::PopStyleColor(3);

				if (ImGui::TreeNode("Material"))
				{
					auto material_name = active_model.GetMaterialName();
					if (!material_name.empty())
					{
						std::string message = "Displayed Material: " + material_name;
						ImGui::Text(message.c_str());
					}

					if (ImGui::ColorEdit3("Ambient", (float*)&active_model.material.ambient))
						active_model.ClearMaterialName();
					if (ImGui::ColorEdit3("Diffuse", (float*)&active_model.material.diffuse))
						active_model.ClearMaterialName();
					if (ImGui::ColorEdit3("Specular", (float*)&active_model.material.specular))
						active_model.ClearMaterialName();
					if (ImGui::SliderFloat("Shininess", (float*)&active_model.material.shininess, 0, 1))
						active_model.ClearMaterialName();
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Transformations"))
				{
					DrawTransformationsWidgets(active_model, true, false);
					ImGui::TreePop();
				}
			}
		}
		ImGui::PopID();
	}
}

void AppWindows::DrawCameraContent(std::shared_ptr<Scene> scene)
{
	static int projection = 0;
	auto& active_camera = scene->GetActiveCamera();

	if (ImGui::CollapsingHeader("Transformations"))
	{
		DrawTransformationsWidgets(active_camera, false, true);
	}
	if (ImGui::CollapsingHeader("View"))
	{
		if (ImGui::TreeNode("Eye##1"))
		{
			ImGui::InputFloat("x", (float*)&active_camera.viewing.eye.x, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("y", (float*)&active_camera.viewing.eye.y, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("z", (float*)&active_camera.viewing.eye.z, 1.0f, 1.0f, "%.2f");
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("At##2"))
		{
			ImGui::InputFloat("x", (float*)&active_camera.viewing.at.x, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("y", (float*)&active_camera.viewing.at.y, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("z", (float*)&active_camera.viewing.at.z, 1.0f, 1.0f, "%.2f");
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode("Up##2"))
		{
			ImGui::InputFloat("x", (float*)&active_camera.viewing.up.x, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("y", (float*)&active_camera.viewing.up.y, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("z", (float*)&active_camera.viewing.up.z, 1.0f, 1.0f, "%.2f");
			ImGui::TreePop();
		}
	}

	if (ImGui::CollapsingHeader("Projections"))
	{
		projection = active_camera.GetProjectionType() == PROJECTION::PERSPECTIVE ? 0 : 1;
		if (ImGui::RadioButton("Perspective", &projection, 0)) {
			active_camera.SetProjectionType(PROJECTION::PERSPECTIVE);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Orthographic", &projection, 1)) {
			active_camera.SetProjectionType(PROJECTION::ORTHO);
		}

		if (!projection)
		{
			ImGui::PushID(0);
			ImGui::InputFloat("fovy", &active_camera.perspective.fovy, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("aspect", &active_camera.perspective.aspect, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("near", &active_camera.perspective.z_near, 5.0f, 5.0f, "%.2f");
			ImGui::InputFloat("far", &active_camera.perspective.z_far, 5.0f, 5.0f, "%.2f");
			ImGui::InputFloat("zoom", &active_camera.perspective.zoom, 0.1f, 0.1f, "%.2f");
			ImGui::PopID();
		}
		if (projection)
		{
			ImGui::PushID(1);
			ImGui::InputFloat("left", &active_camera.view_volume.left, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("right", &active_camera.view_volume.right, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("bottom", &active_camera.view_volume.bottom, 1.0f, 1.0f, "%.2f");
			ImGui::InputFloat("top", &active_camera.view_volume.top, 1.0f, 1.0f, "%.2f");
			ImGui::PopID();
		}
	}
}

void AppWindows::DrawLightContent(std::shared_ptr<Scene> scene)
{
	if (scene->HasLights())
	{
		ImGui::Text("Current scene's lighting.");
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
		if (ImGui::Button("Remove All")) {
			scene->ClearAllLights();
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
	}
	else {
		ImGui::Text("More options are available when adding lights.");
	}

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.0f / 7.0f, 0.8f, 0.8f));
	if (ImGui::Button("Add Light")) {
		scene->AddLight();
	}
	ImGui::PopStyleColor(3);

	size_t lights_count = scene->GetLightCount();
	for (int i = 0; i < lights_count; i++)
	{
		auto& current_light = scene->GetLight(i);
		ImGui::PushID(i);
		if (ImGui::CollapsingHeader(scene->GetLight(i).GetName().c_str()))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
			if (ImGui::Button("Remove")) {
				scene->RemoveLight(i);
				ImGui::PopStyleColor(3);
				ImGui::PopID();
				break;
			}
			ImGui::PopStyleColor(3);
			if (ImGui::TreeNode("Reflection"))
			{
				ImGui::ColorEdit3("Ambient", (float*)&current_light.ambient);
				ImGui::ColorEdit3("Diffuse", (float*)&current_light.diffuse);
				ImGui::ColorEdit3("Specular", (float*)&current_light.specular);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Position"))
			{
				auto& pos = current_light.GetPosition();
				ImGui::InputFloat("x", (float*)&pos.x, 1.0f, 1.0f, "%.2f");
				ImGui::InputFloat("y", (float*)&pos.y, 1.0f, 1.0f, "%.2f");
				ImGui::InputFloat("z", (float*)&pos.z, 1.0f, 1.0f, "%.2f");
				ImGui::TreePop();
			}
		}

		ImGui::PopID();
	}
}

void AppWindows::DrawRendererContent(std::shared_ptr<Scene> scene)
{
	static int triangles_fill = 2;
	static int shading = 1;

	if (scene->HasModels()) 
	{
		if (ImGui::CollapsingHeader("Toon Shading"))
		{
			ImGui::Text("Apply toon shading to the rendered scene.");
			ImGui::Checkbox("Apply", &scene->features_input.apply_toon_shading);
		}
		if (ImGui::CollapsingHeader("Gaussian Blur"))
		{
			ImGui::Text("Apply Gaussian Blur to the rendered scene.");
			ImGui::Checkbox("Apply", &scene->features_input.gaussian_blur);
			if (ImGui::TreeNode("Kernel"))
			{
				ImGui::InputInt("width", &scene->features_input.kernel_width, 1.0f, 1.0f);
				ImGui::InputInt("height", &scene->features_input.kernel_height, 1.0f, 1.0f);
				ImGui::InputFloat("sigma", &scene->features_input.kernel_sigma, 1.0f, 1.0f, "%.2f");
				ImGui::TreePop();
			}
		}
		if (ImGui::CollapsingHeader("Model"))
		{
			ImGui::Text("Mesh Color");
			ImGui::ColorEdit3("color", (float*)&scene->model_color);

			ImGui::Text("Triangle Bounding Rectangles");
			ImGui::Checkbox("Display", &scene->features_input.draw_bounding_rect);;

			ImGui::Text("Fill");
			ImGui::RadioButton("Empty", &triangles_fill, (int)FILL::EMPTY);
			ImGui::SameLine();
			ImGui::RadioButton("Grey Scale", &triangles_fill, (int)FILL::GREY);
			ImGui::SameLine();
			ImGui::RadioButton("Material", &triangles_fill, (int)FILL::COLOR);

			scene->features_input.triangles_fill = static_cast<FILL>(triangles_fill);

			ImGui::Text("Shading");
			ImGui::RadioButton("Flat", &shading, (int)SHADING::FLAT);
			ImGui::SameLine();
			ImGui::RadioButton("Phong", &shading, (int)SHADING::PHONG);

			scene->features_input.shading_type = static_cast<SHADING>(shading);
		}

		if (ImGui::CollapsingHeader("Bounding Box"))
		{
			ImGui::Checkbox("Display", &scene->features_input.display_bounding_box);

			ImGui::Text("Bounding Box Color");
			ImGui::ColorEdit3("color", (float*)&scene->bounding_box_color);
		}

		if (ImGui::CollapsingHeader("Normals"))
		{
			if (ImGui::TreeNode("Face Normals"))
			{
				ImGui::Checkbox("Display", &scene->features_input.display_face_normals);

				ImGui::Text("Face Normals Color");
				ImGui::ColorEdit3("color", (float*)&scene->normals_colors[0]);

				ImGui::Separator();
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Vertex Normals"))
			{
				ImGui::Checkbox("Display", &scene->features_input.display_vertex_normals);

				ImGui::Text("Vertex Normals Color");
				ImGui::ColorEdit3("color", (float*)&scene->normals_colors[1]);

				ImGui::TreePop();
			}
		}

		if (ImGui::CollapsingHeader("Axes"))
		{
			if (ImGui::TreeNode("Model Axes"))
			{
				ImGui::Checkbox("Display", &scene->features_input.display_model_axes);

				ImGui::Text("X Axis Color");
				ImGui::ColorEdit3("x", (float*)&scene->model_axes_colors[0]);

				ImGui::Text("Y Axis Color");
				ImGui::ColorEdit3("y", (float*)&scene->model_axes_colors[1]);

				ImGui::Text("Z Axis Color");
				ImGui::ColorEdit3("z", (float*)&scene->model_axes_colors[2]);

				ImGui::TreePop();
				ImGui::Separator();
			}

			if (ImGui::TreeNode("World Axes"))
			{
				ImGui::Checkbox("Display", &scene->features_input.display_world_axes);

				ImGui::Text("X Axis Color");
				ImGui::ColorEdit3("x", (float*)&scene->world_axes_colors[0]);

				ImGui::Text("Y Axis Color");
				ImGui::ColorEdit3("y", (float*)&scene->world_axes_colors[1]);

				ImGui::Text("Z Axis Color");
				ImGui::ColorEdit3("z", (float*)&scene->world_axes_colors[2]);

				ImGui::TreePop();
			}
		}
	}
	else
	{
		ImGui::Text("More options are available when loading \nmesh models.");
		if (ImGui::CollapsingHeader("Axes"))
		{
			if (ImGui::TreeNode("World Axes"))
			{
				ImGui::Checkbox("Display", &scene->features_input.display_world_axes);

				ImGui::Text("X Axis Color");
				ImGui::ColorEdit3("x", (float*)&scene->world_axes_colors[0]);

				ImGui::Text("Y Axis Color");
				ImGui::ColorEdit3("y", (float*)&scene->world_axes_colors[1]);

				ImGui::Text("Z Axis Color");
				ImGui::ColorEdit3("z", (float*)&scene->world_axes_colors[2]);

				ImGui::TreePop();
			}
		}
	}
}

void AppWindows::DrawModelTransformations()
{
	ImGui::Text("No mesh loaded at this time.");
}

void AppWindows::DrawModelTransformations(MeshModel& active_mesh)
{
	auto& model_tranformation = active_mesh.GetTransformation(COORDINATES::MODEL);
	auto& world_tranformation = active_mesh.GetTransformation(COORDINATES::WORLD);
	ImGui::Text("Applied transformations to the active model\n");
	ImGui::BulletText("Model Transformations Matrix");
	ImGui::Indent();
	ImGui::Columns(4, "model", false);
	ImGui::Text("%f", model_tranformation[0][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[0][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[0][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[0][3]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[1][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[1][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[1][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[1][3]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[2][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[2][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[2][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[2][3]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[3][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[3][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[3][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", model_tranformation[3][3]);
	ImGui::Columns(1);
	ImGui::Unindent();
	ImGui::BulletText("World Transformations Matrix");
	ImGui::Indent();
	ImGui::Columns(4, "world", false);
	ImGui::Text("%f", world_tranformation[0][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[0][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[0][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[0][3]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[1][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[1][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[1][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[1][3]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[2][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[2][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[2][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[2][3]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[3][0]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[3][1]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[3][2]);
	ImGui::NextColumn();
	ImGui::Text("%f", world_tranformation[3][3]);
	ImGui::Columns(1);
	ImGui::Unindent();
}

void AppWindows::DrawShortcuts()
{
	ImGui::SetWindowSize(ImVec2(350, 350), ImGuiCond_Once);
	ImGui::Text("SHORTCUTS GUIDE");

	if (ImGui::CollapsingHeader("APPLY TO"))
	{
		ImGui::Columns(2);
		ImGui::Text("To");
		ImGui::NextColumn();
		ImGui::Text("Shortcut");
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("Model");
		ImGui::NextColumn();
		ImGui::Text("[m] or [M]");
		ImGui::NextColumn();
		ImGui::Text("World");
		ImGui::NextColumn();
		ImGui::Text("[w] or [W]");
		ImGui::Columns(1);
	}

	if (ImGui::CollapsingHeader("TRANSLATIONS"))
	{
		ImGui::Columns(3);
		ImGui::Text("Axis");
		ImGui::NextColumn();
		ImGui::Text("[+] Shortcut");
		ImGui::NextColumn();
		ImGui::Text("[-] Shortcut");
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("X");
		ImGui::NextColumn();
		ImGui::Text("Right Arrow");
		ImGui::NextColumn();
		ImGui::Text("Left Arrow");
		ImGui::NextColumn();
		ImGui::Text("Y");
		ImGui::NextColumn();
		ImGui::Text("Up Arrow");
		ImGui::NextColumn();
		ImGui::Text("Down Arrow");
		ImGui::NextColumn();
		ImGui::Text("Z");
		ImGui::NextColumn();
		ImGui::Text("Keypad [.]");
		ImGui::NextColumn();
		ImGui::Text("Keypad [0]");
		ImGui::Columns(1);
	}

	if (ImGui::CollapsingHeader("SCALING"))
	{
		ImGui::Columns(2);
		ImGui::Text("[+] Shortcut");
		ImGui::NextColumn();
		ImGui::Text("[-] Shortcut");
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("Keypad [*]");
		ImGui::NextColumn();
		ImGui::Text("Keypad [/]");
		ImGui::Columns(1);
	}

	if (ImGui::CollapsingHeader("ROTATIONS"))
	{
		ImGui::Columns(3);
		ImGui::Text("Axis");
		ImGui::NextColumn();
		ImGui::Text("[+] Shortcut");
		ImGui::NextColumn();
		ImGui::Text("[-] Shortcut");
		ImGui::NextColumn();
		ImGui::Separator();
		ImGui::Text("X");
		ImGui::NextColumn();
		ImGui::Text("Keypad [3]");
		ImGui::NextColumn();
		ImGui::Text("Keypad [1]");
		ImGui::NextColumn();
		ImGui::Text("Y");
		ImGui::NextColumn();
		ImGui::Text("Keypad [6]");
		ImGui::NextColumn();
		ImGui::Text("Keypad [4]");
		ImGui::NextColumn();
		ImGui::Text("Z");
		ImGui::NextColumn();
		ImGui::Text("Keypad [9]");
		ImGui::NextColumn();
		ImGui::Text("Keypad [7]");
		ImGui::Columns(1);
		ImGui::Separator();
	}
}
