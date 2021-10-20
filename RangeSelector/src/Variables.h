#pragma once
#include "imgui.h"
#include "implot.h"

// General Variables & Constants
const int DOUBLE_PRECISION = 9;
bool isPlotLine = true;
bool saveFile = false;
bool errorWrongSelectedRange = false;

// Main window Variables & Constants
// 1. Variables
int windowWidth;
int windowHeight;



// ImGui Window Variables & Constants
// 1. ImGui Window Variables
ImVec4 backgroundColor;
int imguiWindowWidth;
int imguiWindowHeight;
ImVec2 imguiWindowSize;

// 2. ImGui Window Constants
const ImVec2 IMGUI_WINDOW_POS = ImVec2(0.0f, 0.0f);
const ImGuiWindowFlags IMGUI_WINDOW_FLAGS = (ImGuiWindowFlags_NoTitleBar
	| ImGuiWindowFlags_NoResize
	| ImGuiWindowFlags_NoMove
	| ImGuiWindowFlags_NoCollapse
	| ImGuiWindowFlags_MenuBar);



// The main window is divide into two main sections
// Section1: The main plot
// 1. Section1 Variables
ImVec2 section1Size;
ImPlotLimits queriedLimit;

// 2. Section1 Constants
const ImPlotFlags SECTION1_FLAGS = (ImPlotFlags_Query | ImPlotFlags_NoTitle);


// Section2: A subplot of two windows, the small zoom window and The selected ranges table
// 1. Section2 Variables
ImVec2 section2Size;
bool isPlotQueried;
ImVec2 tableSize;

// 2. Section2 Constants
ImPlotSubplotFlags SECTION2_SUBPLOT_FLAGS = ( ImPlotSubplotFlags_NoTitle);
const int SECTION2_ROWS = 1;
const int SECTION2_COLS = 2;
static float SECTION2_ROWS_RATIOS[] = { 1.0 };
static float SECTION2_COLS_RATIOS[] = { 2.0, 2.0 };
const int TABLE_COLS = 4;
const ImGuiTableFlags TABLE_FLAGS = (ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg);
double TABLE_MARGINS = 20.0;