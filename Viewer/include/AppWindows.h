#pragma once
#include <imgui/imgui.h>
#include <functional>
#include "Scene.h"
#include "Renderer.h"

class AppWindows
{
public:
	static void DrawControllersWindow(ImGuiIO& io, std::shared_ptr<Scene> scene);
	static void DrawModelTransformations();
	static void DrawModelTransformations(MeshModel& active_mesh);
	static void DrawShortcuts();

private:
	static void DrawTransformationsWidgets(TransformationVectors& input, bool draw_scale, bool& scale_all);
	static void DrawTransformationsWidgets(TransformableObject& object, bool draw_scale, bool is_camera);
	static void DrawModelContent(std::shared_ptr<Scene> scene);
	static void DrawCameraContent(std::shared_ptr<Scene> scene);
	static void DrawLightContent(std::shared_ptr<Scene> scene);
	static void DrawRendererContent(std::shared_ptr<Scene> scene);
};