#include <stdio.h>
#include <iostream>
#include <wtypes.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "implot.h";

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    int count;
    int windowWidth, windowHeight;
    int monitorX, monitorY;

    GLFWwindow* window;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    const GLFWvidmode* videoMode = glfwGetVideoMode(monitors[1]);
    windowWidth = videoMode->width / 1.5;
    windowHeight = videoMode->height / 1.5;


    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);
    // Set the visibility window hint to false for subsequent window creation
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Range Selector", NULL, NULL);

    if (window == NULL){
        glfwTerminate();
        std::cout << "ERROR: FAILED TO CREATE MAIN WINDOW." << std::endl;
        return 1;
    }

    // Reset the window hints to default
    glfwDefaultWindowHints();

    glfwSetWindowPos(window, monitorX + (videoMode->width - windowWidth) / 2, monitorY + (videoMode->height - windowHeight) / 2);

    // Show the window
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
    ImVec2 imguiWindowPos = ImVec2(0.0f, 0.0f);
    int imguiWindowWidth = windowWidth;
    int imguiWindowHeight = windowHeight;
    ImVec2 imguiWindowSize = ImVec2(imguiWindowWidth, imguiWindowHeight);

    int rows = 2;
    int cols = 2;
    
    static float x_data[512];
    static float y_data1[512];
    static float y_data2[512];
    static float y_data3[512];
    static float sampling_freq = 44100;
    static float freq = 500;
    for (size_t i = 0; i < 512; ++i) {
        const float t = i / sampling_freq;
        x_data[i] = t;
        const float arg = 2 * 3.14f * freq * t;
        y_data1[i] = sinf(arg);
        y_data2[i] = y_data1[i] * -0.6f + sinf(2 * arg) * 0.4f;
        y_data3[i] = y_data2[i] * -0.6f + sinf(3 * arg) * 0.4f;
    }

    ImPlotLimits range;



    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //ImPlot::ShowDemoWindow();
        ImGui::Begin("Range Selector");

        ImGui::SetWindowPos(imguiWindowPos, 0);
        glfwGetWindowSize(window, &imguiWindowWidth, &imguiWindowHeight);
        imguiWindowSize.x = imguiWindowWidth;
        imguiWindowSize.y = imguiWindowHeight;
        ImGui::SetWindowSize(imguiWindowSize, 0);

        ImGui::BulletText("Query the first plot to render a subview in the second plot (see above for controls).");
        ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
        bool is_viewed = false;
        ImPlot::SetNextPlotLimits(0, 0.01, -1, 1);
        if (ImPlot::BeginPlot("##View1", NULL, NULL, ImVec2(-1, imguiWindowHeight/3), ImPlotFlags_Query, flags, flags)) {
            ImPlot::PlotLine("Signal 1", x_data, y_data1, 512);
            ImPlot::PlotLine("Signal 2", x_data, y_data2, 512);
            ImPlot::PlotLine("Signal 3", x_data, y_data3, 512);

            range = ImPlot::GetPlotQuery();
            is_viewed = ImPlot::IsPlotQueried();

            ImPlot::EndPlot();
        }
        
        if (is_viewed) {
            ImPlot::SetNextPlotLimits(range.X.Min, range.X.Max, range.Y.Min, range.Y.Max, ImGuiCond_Always);
            if (ImPlot::BeginPlot("##View2", NULL, NULL, ImVec2(-1, imguiWindowHeight/3), ImPlotFlags_CanvasOnly, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations)) {
                ImPlot::PlotLine("Signal 1", x_data, y_data1, 512);
                ImPlot::PlotLine("Signal 2", x_data, y_data2, 512);
                ImPlot::PlotLine("Signal 3", x_data, y_data3, 512);

                ImPlot::EndPlot();
            }
        }
        ImGui::End();
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
