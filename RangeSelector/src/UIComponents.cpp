#include "imgui.h"
#include "implot.h"
#include "Variables.h"

void MenuBar(bool* saveFile, bool* isPlotLine) {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("New File")) {
				//TODO: add file browser to select the new file to read from
			}
			if (ImGui::MenuItem("Save File")) {
				//TODO: add file browser to select where to save the file
				*saveFile = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Preferences")) {
			ImGui::MenuItem("Plot Type", NULL, false, false);
			if (ImGui::MenuItem("Line Plot")) {
				*isPlotLine = true;
			}
			if (ImGui::MenuItem("Scatter Plot")) {
				*isPlotLine = false;
			}
			ImGui::MenuItem("Style", NULL, false, false);
			ImGui::ShowStyleSelector("");
			ImGui::MenuItem("Time", NULL, false, false);
			ImGui::Checkbox("Use ISO 8601", &ImPlot::GetStyle().UseISO8601);
			ImGui::Checkbox("Use 24 Hour Clock", &ImPlot::GetStyle().Use24HourClock);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}