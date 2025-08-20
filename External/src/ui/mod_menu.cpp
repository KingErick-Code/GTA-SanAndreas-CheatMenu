#include "mod_menu.h"

Menu::Menu(HWND window) : window(window)
{

}

void Menu::render()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImFont* font = ImGui::GetFont();
	float targetHeight = 30.0f;
	float framePaddingY = (targetHeight - ImGui::GetFontSize()) / 2.0f;
	style.FramePadding.y = framePaddingY < 0 ? 0 : framePaddingY;

	RECT rect;
	GetClientRect(window, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
	ImGui::Begin("##main", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_AlwaysAutoResize
	);
	
	if (ImGui::BeginMenuBar()) {
		ImGui::Text("GTA: SanAndreas External Cheat Menu");

		static float buttonWidth = 30.0f;
		static float buttonHeight = 30.0f;
		float totalWidth = buttonWidth * 3;

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalWidth);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));

		if (ImGui::Button("_", ImVec2(buttonWidth, buttonHeight)))
			ShowWindow(window, SW_MINIMIZE);
		ImGui::SameLine();
		if (ImGui::Button("[]", ImVec2(buttonWidth, buttonHeight)))
			ShowWindow(window, SW_MAXIMIZE);
		ImGui::SameLine();
		if (ImGui::Button("X", ImVec2(buttonWidth, buttonHeight)))
			PostMessage(window, WM_CLOSE, 0, 0);

		ImGui::PopStyleColor(3);
		ImGui::PopStyleVar(2);
	}
	ImGui::EndMenuBar();
	ImGui::End();
}