#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "implot.h"

#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include "Data.cpp"
#include "Range.cpp"

#include "Helpers.h"
#include "Variables.h"

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {
	// Set the reading and writing precision to 
	std::cout.precision(DOUBLE_PRECISION);

	// Reading the data as Data object
	std::string fileName = "ExampleSet.csv";
	Data data = parseCSVFile(fileName);
	Data normalData = data;
	std::vector<Range> ranges;

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window;
	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[0]);

	// Variables to make the window resize dynamically
	windowWidth = videoMode->width * (2.0 / 3.0);
	windowHeight = videoMode->height * (2.0 / 3.0);

	// Create window with graphics context
	window = glfwCreateWindow(windowWidth, windowHeight, "Range Selector", NULL, NULL);
	if (window == NULL)
		return 1;

	// Reset the window hints to default
	glfwDefaultWindowHints();

	// Set the window position 
	glfwSetWindowPos(window, (videoMode->width - windowWidth) / 2, (videoMode->height - windowHeight) / 2);

	// Show the window
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context and ImPlot context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImPlot::GetStyle().UseLocalTime;

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	// ImGui window values
	backgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
	imguiWindowWidth = windowWidth;
	imguiWindowHeight = windowHeight;
	imguiWindowSize = ImVec2(imguiWindowWidth, imguiWindowHeight);

	// The main window is divide into two main sections
	// Section1: The main plot
	section1Size = ImVec2(-1, imguiWindowWidth / 2);
	// Section2: A subplot of two windows, the small zoom window and The selected ranges table
	section2Size = ImVec2(-1, imguiWindowWidth / 3);
	isRangeSelected = false;
	
	while (!glfwWindowShouldClose(window)) {
		
			glfwPollEvents();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("MAIN_WINDOW", NULL, IMGUI_WINDOW_FLAGS);
			//ImPlot::ShowDemoWindow();
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			// Set the window position to (0, 0) top left corner
			ImGui::SetWindowPos(IMGUI_WINDOW_POS, 0);

			// Get the new window size incase of user modifications and update the imgui size
			glfwGetWindowSize(window, &imguiWindowWidth, &imguiWindowHeight);
			imguiWindowSize.x = imguiWindowWidth;
			imguiWindowSize.y = imguiWindowHeight;
			ImGui::SetWindowSize(imguiWindowSize, 0);

			// Section1: The main plot
			if (ImPlot::BeginPlot("SECTION1", NULL, NULL, section1Size, SECTION1_FLAGS, ImPlotAxisFlags_Time)) {
				for (int i = 0; i < normalData.numberOfColumns - 1; i++) {
					// TODO: Remove the hardcoding of the timestamp location when using file browser to read the csv file.
					ImPlot::PlotLine(normalData.columns.at(i).name.data(), &normalData.columns.at(6).values[0], &normalData.columns.at(i).values[0], normalData.numberOfRows);
				}

				// Check if the plot is queried or not
				if (ImPlot::IsPlotQueried()) 
					isRangeSelected = true;
				else 
					isRangeSelected = false;
				

				// Update Section1 hegith
				section1Size.y = imguiWindowHeight / 2;

				ImPlot::EndPlot();
			}
			
			// Section2: The small zoom window and The selected ranges table
			if (ImPlot::BeginSubplots("SECTION2", SECTION2_ROWS, SECTION2_COLS, section2Size, ImPlotSubplotFlags_NoTitle, SECTION2_ROWS_RATIOS, SECTION2_COLS_RATIOS)) {
				if (ImPlot::BeginPlot("", NULL, NULL, ImVec2(), ImPlotFlags_NoLegend, ImPlotAxisFlags_Time)) {
					if (isRangeSelected) {
						for (int i = 0; i < normalData.numberOfColumns - 1; i++) {
							// TODO: Remove the hardcoding of the timestamp location when using file browser to read the csv file.
							ImPlot::PlotLine(normalData.columns.at(i).name.data(), &normalData.columns.at(6).values[0], &normalData.columns.at(i).values[0], normalData.numberOfRows);
						}
					}
					ImPlot::EndPlot();
				}

				// Update Section2 hegith
				section2Size.y = imguiWindowHeight / 3;

				ImPlot::EndSubplots();
			}

			ImGui::End();

			// Rendering
			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(backgroundColor.x * backgroundColor.w, backgroundColor.y * backgroundColor.w, backgroundColor.z * backgroundColor.w, backgroundColor.w);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
			glfwWaitEvents();
	}
}