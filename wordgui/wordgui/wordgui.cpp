
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
#define _CRT_SECURE_NO_WARNINGS
#define MAX_BYTES 1024 * 16
#include "imguifilesystem.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "Core.h"
// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

// Include glfw3.h after our OpenGL definitions
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <windows.h>

typedef int(*p_gen_chain_word)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_gen_chain_char)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_build_map)(char* words[], char* text);

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
static bool OpenButtonPressed, SaveButtonPressed;

static char inputText[MAX_BYTES] = "";
static char outputText[MAX_BYTES] = "";
static void ShowMenuFile()
{
	if (ImGui::MenuItem("Open", "Ctrl+O"))
	{
		OpenButtonPressed = true;
	}

	if (ImGui::MenuItem("Save As", "Ctrl+S"))
	{
		SaveButtonPressed = true;
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Quit", "Alt+F4"))
	{
		exit(0);
	}
}

// Demonstrate creating a fullscreen menu bar and populating it.
static void ShowMainMenuBar(Core& core)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowMenuFile();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

static void OpenButtonMonitor(Core& core, char* text)
{
	static ImGuiFs::Dialog openDlg;
	const char* myPath = openDlg.chooseFileDialog(OpenButtonPressed);
	OpenButtonPressed = false;

	if (strlen(openDlg.getChosenPath()) > 0)
	{
		ImGui::Text("Open file: \"%s\"", openDlg.getChosenPath());
	}
	else
	{
		ImGui::Text("Open file: \"%s\"", "");
	}
	if (strlen(myPath) > 0)
	{
		FILE* fp = fopen(myPath, "r");
		int i = 0;
		while ((text[i] = fgetc(fp)) != EOF && i < MAX_BYTES - 1) i++;
		text[i] = '\0';
		fclose(fp);
	}
}

static void SaveButtonMonitor(Core& core, char* text)
{
	static ImGuiFs::Dialog saveDlg;
	const char* myPath = saveDlg.saveFileDialog(SaveButtonPressed); 
	SaveButtonPressed = false;

	if (strlen(saveDlg.getChosenPath()) > 0)
	{
		ImGui::Text("Save file: \"%s\"", saveDlg.getChosenPath());
	}
	else
	{
		ImGui::Text("Save file: \"%s\"", "");
	}
	if (strlen(myPath) > 0)
	{
		FILE* fp = fopen(myPath, "w");
		fprintf(fp, "%s", text);
		fclose(fp);
	}
}

bool w, c, h, t, r;
char *words[10001];

static void FlagCheckBox(Core& core, char* inputText, char* outputText)
{
	HINSTANCE CoreDLL = LoadLibrary("Core.dll");
	if (CoreDLL == NULL)
	{
		std::cout << "File 'Core.dll' not found." << std::endl;
		exit(1);
	}


	p_gen_chain_word gen_chain_word = (p_gen_chain_word)GetProcAddress(CoreDLL, "gen_chain_word");
	p_gen_chain_char gen_chain_char = (p_gen_chain_char)GetProcAddress(CoreDLL, "gen_chain_char");
	p_build_map build_map = (p_build_map)GetProcAddress(CoreDLL, "build_map");
	if (gen_chain_word == NULL || gen_chain_char == NULL || build_map == NULL)
	{
		std::cout << "Invalid file 'Core.dll'." << std::endl;
		exit(1);
	}

	ImGui::Text(" ");
	ImGui::Checkbox("-w", &w);
	ImGui::Text(" ");
	c = c & !w;
	//ImGui::SameLine();
	ImGui::Checkbox("-c", &c);
	ImGui::Text(" ");
	w = w & !c;
	//ImGui::SameLine();
	ImGui::Checkbox("-h", &h);
	ImGui::SameLine(0, 20);
	//ImGui::Text("h:");
	//ImGui::SameLine();
	static char head[16] = "";
	ImGui::PushItemWidth(16);
	ImGui::InputText("##source1", head, 2, ImGuiInputTextFlags_AllowTabInput);
	ImGui::PopItemWidth();
	ImGui::Text(" ");
	//ImGui::SameLine();

	ImGui::Checkbox("-t", &t);
	ImGui::SameLine(0, 20);
	//ImGui::Text("t:");
	//ImGui::SameLine();
	static char tail[16] = "";
	ImGui::PushItemWidth(16);
	ImGui::InputText("##source2", tail, 2, ImGuiInputTextFlags_AllowTabInput);
	ImGui::PopItemWidth();
	ImGui::Text(" ");
	//ImGui::SameLine();

	ImGui::Checkbox("-r", &r);
	ImGui::Text(" ");
	//ImGui::SameLine();

	if (ImGui::Button("submit"))
	{
		if (!h) head[0] = 0;
		if (!t) tail[0] = 0;

		int n = build_map(words, inputText);
		char* result[105];
		int result_len = 0;
		if (!w && !c)
		{
			sprintf(outputText, "Command line parameters do not contain - w or -c");
			return;
		}
		try
		{
			if (w)
				result_len = gen_chain_word(words, n, result, tolower(head[0]), tolower(tail[0]), r);
			else
				result_len = gen_chain_char(words, n, result, tolower(head[0]), tolower(tail[0]), r);
		}
		catch (const char* msg)
		{
			sprintf(outputText, "%s", msg);
			return;
		}

		sprintf(outputText, "%d\n", result_len);
		printf("%s",outputText);
		for (int i = 0; i < result_len; i++)
			printf("%s\n", result[i]);
		int i = strlen(outputText), j = 0, k = 0;
		while (1)
		{
			if (k == result_len)
			{
				outputText[i] = '\0';
				break;
			} 
			else if (result[k][j] == '\0')
			{
				k++;
				j = 0;
				outputText[i++] = '\n';
			}
			else
			{
				outputText[i] = result[k][j];
				i++; j++;
			}
		}

	}
}

int main(int, char**)
{


	

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
#if __APPLE__
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(720, 540, "Longest Word List", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	bool show_window = true;
	ImGuiWindowFlags window_flag = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec2 window_size = ImVec2(720.0f, 540.0f);
	ImVec2 window_pos = ImVec2(0.0f, 0.0f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{

			static Core core;

			ImGui::SetNextWindowSize(window_size);
			ImGui::SetNextWindowPos(window_pos);

			if (!ImGui::Begin("ImGui Demo", &show_window, window_flag))
			{
				ImGui::End();
				break;
			}

			ShowMainMenuBar(core);
			OpenButtonMonitor(core, inputText);
			SaveButtonMonitor(core, outputText);
			
			ImGui::Columns(3, "mixed");
			ImGui::Text("Input");
			ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_AllowTabInput;
			ImGui::InputTextMultiline("##Input", inputText, IM_ARRAYSIZE(inputText), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 32), inputFlags);
			//ImGui::Separator();
			ImGui::NextColumn();
			ImGui::Text("Output");
			ImGuiInputTextFlags outputFlags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly;
			ImGui::InputTextMultiline("##output", outputText, IM_ARRAYSIZE(outputText), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 32), outputFlags);
			
        	ImGui::NextColumn();
			FlagCheckBox(core, inputText, outputText);
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}