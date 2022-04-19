#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include "AppWindows.h"
#include "InputUtils.h"
#include "Materials.h"

// Fields
bool show_controller_window = true;
bool show_data_window = false;
bool show_shortcuts_window = false;
glm::vec4 clear_color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
static int viewport_width = 1280;
static int viewport_height = 720;
std::shared_ptr<Scene> scene;
GLFWwindow* window;

// Function declarations
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io);

// Function implementation 
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int frameBufferWidth, frameBufferHeight;
	GLFWwindow* window = SetupGlfwWindow(viewport_width, viewport_height, "Mesh Viewer");
	if (!window)
	{
		return 1;
	}
	ImGuiIO& io = SetupDearImgui(window);

	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);

	Renderer renderer = Renderer(viewport_width, viewport_height);
	scene = std::make_shared<Scene>();
	scene->AddLight(glm::vec3(640.0f, 360.0f, 1000.0f));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		DrawImguiMenus(io);
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.Render(scene);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	Cleanup(window);
	return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	window = glfwCreateWindow(w, h, window_name, NULL, NULL);

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Renderer& renderer, ImGuiIO& io)
{
	static COORDINATES coordinates = COORDINATES::MODEL;
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight()) {
		renderer.SetViewportSize(frameBufferWidth, frameBufferHeight);
		viewport_width = frameBufferWidth;
		viewport_height = frameBufferHeight;
	}

	if (!io.WantCaptureKeyboard)
	{
		if (io.KeysDown[(short)SHORTCUTS::MODEL_UPPER] ||
			io.KeysDown[(short)SHORTCUTS::MODEL_LOWER])
			coordinates = COORDINATES::MODEL;

		if (io.KeysDown[(short)SHORTCUTS::WORLD_UPPER] ||
			io.KeysDown[(short)SHORTCUTS::WORLD_LOWER])
			coordinates = COORDINATES::WORLD;

		if (scene->HasModels()) {
			MeshModel& active_model = scene->GetActiveModel();

			if (io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]])
				InputUtils::TranslateMesh(active_model, coordinates, AXIS::X, DIRECTION::POS);
			if (io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]])
				InputUtils::TranslateMesh(active_model, coordinates, AXIS::X, DIRECTION::NEG);

			if (io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]])
				InputUtils::TranslateMesh(active_model, coordinates, AXIS::Y, DIRECTION::POS);
			if (io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]])
				InputUtils::TranslateMesh(active_model, coordinates, AXIS::Y, DIRECTION::NEG);

			if (io.KeysDown[(short)SHORTCUTS::TRANSLATION_Z_INC])
				InputUtils::TranslateMesh(active_model, coordinates, AXIS::Z, DIRECTION::POS);
			if (io.KeysDown[(short)SHORTCUTS::TRANSLATION_Z_DEC])
				InputUtils::TranslateMesh(active_model, coordinates, AXIS::Z, DIRECTION::NEG);

			if (io.KeysDown[(short)SHORTCUTS::ROTATION_X_INC])
				InputUtils::RotateMesh(active_model, coordinates, AXIS::X, DIRECTION::POS);
			if (io.KeysDown[(short)SHORTCUTS::ROTATION_X_DEC])
				InputUtils::RotateMesh(active_model, coordinates, AXIS::X, DIRECTION::NEG);

			if (io.KeysDown[(short)SHORTCUTS::ROTATION_Y_INC])
				InputUtils::RotateMesh(active_model, coordinates, AXIS::Y, DIRECTION::POS);
			if (io.KeysDown[(short)SHORTCUTS::ROTATION_Y_DEC])
				InputUtils::RotateMesh(active_model, coordinates, AXIS::Y, DIRECTION::NEG);

			if (io.KeysDown[(short)SHORTCUTS::ROTATION_Z_INC])
				InputUtils::RotateMesh(active_model, coordinates, AXIS::Z, DIRECTION::POS);
			if (io.KeysDown[(short)SHORTCUTS::ROTATION_Z_DEC])
				InputUtils::RotateMesh(active_model, coordinates, AXIS::Z, DIRECTION::NEG);

			if (io.KeysDown[(short)SHORTCUTS::SCALING_INC])
				InputUtils::ScaleMesh(active_model, coordinates, DIRECTION::POS);
			if (io.KeysDown[(short)SHORTCUTS::SCALING_DEC])
				InputUtils::ScaleMesh(active_model, coordinates, DIRECTION::NEG);
		}
	}

	renderer.Render(scene);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io)
{
	static float current_viewport_width = 1280.0f, current_viewport_height = 720.0f;
	static float data_width = 350.0f, data_height = 350.0f;
	static float data_current_width = 350.0f, data_current_height = 350.0f;
	static float controller_width = 350.0f, controller_height = 350.0f;
	static float controller_current_width = 350.0f, controller_current_height = 350.0f;
	static bool set_controller_size = false, set_data_size = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene->AddModel(Utils::LoadMeshModel(outPath));
					scene->SetActiveModelIndex(scene->GetModelCount() - 1);
					free(outPath);
				}
				else if (result == NFD_CANCEL) {}
				else {}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Quick Access"))
		{
			if (ImGui::MenuItem("banana"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("banana")));
			if (ImGui::MenuItem("beethoven"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("beethoven")));
			if (ImGui::MenuItem("bishop"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("bishop")));
			if (ImGui::MenuItem("blob"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("blob")));
			if (ImGui::MenuItem("bunny"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("bunny")));
			if (ImGui::MenuItem("camera"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("camera")));
			if (ImGui::MenuItem("chain"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("chain")));
			if (ImGui::MenuItem("cow"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("cow")));
			if (ImGui::MenuItem("demo"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("demo")));
			if (ImGui::MenuItem("dolphin"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("dolphin")));
			if (ImGui::MenuItem("feline"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("feline")));
			if (ImGui::MenuItem("pawn"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("pawn")));
			if (ImGui::MenuItem("sphere"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("sphere")));
			if (ImGui::MenuItem("teapot"))
				scene->AddModel(Utils::LoadMeshModel(InputUtils::QuickAccess("teapot")));

			scene->SetActiveModelIndex(scene->GetModelCount() - 1);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Materials"))
		{
			auto materials_count = Materials::list().size();
			for (const std::string& m : Materials::list())
			{
				if (ImGui::MenuItem(m.c_str()))
				{
					if (scene->HasModels())
					{
						scene->GetActiveModel().SetMaterial(m, Materials::map()[m]);
					}
				}
			}
			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("Textures"))
		{
			std::string path = "../Data/Textures/";
			static std::list<std::string> list = { "Bricks", "Fire", "Grass", "Leather", "Sea", "Stone" };

			for (const std::string& t : list)
			{
				if (ImGui::MenuItem(t.c_str()))
				{
					scene->features_input.texture_path = path + t + ".jpg";
				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Scene Controller", "CTRL+M"))
				show_controller_window = !show_controller_window;
			if (ImGui::MenuItem("Active Model Data", "CTRL+D"))
				show_data_window = !show_data_window;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Shortcuts", "CTRL+S"))
				show_shortcuts_window = !show_shortcuts_window;


			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (viewport_width != current_viewport_width || viewport_height != current_viewport_height) {
		float width = controller_current_width + (viewport_width - current_viewport_width) / 4.0f;
		controller_current_width = width < 200 ? 200 : width;
		controller_current_height = (viewport_height - 20) / 2.0f;
		width = data_current_width + (viewport_width - current_viewport_width) / 4.0f;
		data_current_width = width < 200 ? 200 : width;
		data_current_height = (viewport_height - 20) / 2.0f;
		current_viewport_width = viewport_width;
		current_viewport_height = viewport_height;
		set_controller_size = set_data_size = true;
	}

	if (show_controller_window && controller_current_width < viewport_width && controller_current_height < viewport_height) {
		if (set_controller_size) {
			ImGui::SetNextWindowPos(ImVec2(viewport_width - controller_current_width, 20));
			ImGui::SetNextWindowSize(ImVec2(controller_current_width, controller_current_height));
			set_controller_size = false;
		}
		else {
			ImGui::SetNextWindowPos(ImVec2(viewport_width - controller_current_width, 20), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(controller_current_width, controller_current_height), ImGuiCond_Once);
		}
		ImGui::Begin("Scene Controllers", &show_controller_window, ImGuiWindowFlags_MenuBar);
		AppWindows::DrawControllersWindow(io, scene);
		ImGui::End();
	}

	if (show_data_window && data_current_width < viewport_width && data_current_height < viewport_height) {
		if (set_data_size) {
			ImGui::SetNextWindowPos(ImVec2(viewport_width - data_current_width, data_current_height + 20));
			ImGui::SetNextWindowSize(ImVec2(data_current_width, data_current_height));
			set_data_size = false;
		}
		else {
			ImGui::SetNextWindowPos(ImVec2(viewport_width - data_current_width, data_current_height + 20), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(data_current_width, data_current_height), ImGuiCond_Once);
		}
		ImGui::Begin("Active Model Transformations", &show_data_window);
		if (scene->HasModels())
			AppWindows::DrawModelTransformations(scene->GetActiveModel());
		else
			AppWindows::DrawModelTransformations();
		ImGui::End();
	}

	if (show_shortcuts_window) {
		ImGui::Begin("Shortcuts", &show_shortcuts_window);
		AppWindows::DrawShortcuts();
		ImGui::End();
	}
}
/*
	static bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
*/
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{
	viewport_width = width;
	viewport_height = height;
	glViewport(0, 0, width, height);
}
