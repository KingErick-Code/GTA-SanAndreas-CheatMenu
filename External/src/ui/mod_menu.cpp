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


	ImVec2 windowSize = ImGui::GetIO().DisplaySize;
	float sidebarWidth = 150.0f;
	float sidebarHeight = windowSize.y - 45.0f;

	ImVec2 buttonSize(sidebarWidth - 20, 40);
	sideBar(ImVec2(sidebarWidth, sidebarHeight), buttonSize);
	ImGui::SameLine();
	mainPanel();
	ImGui::End();
}

void Menu::sideBar(ImVec2 sideBarSize, ImVec2 buttonSize)
{
	ImGui::BeginChild("SideBar", sideBarSize, true);

	centerText("CheatMenu (v1.0.0)");
	ImGui::Separator();
	ImGui::Spacing();

	for (int i = 0; i < OPTION_COUNT; ++i) {
		// Highlight the currently selected option
		if (i == currentOption) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 1.0f, 1.0f)); // Blueish
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.7f, 1.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.9f, 1.0f));
		}

		if (ImGui::Button(OPTION_NAMES[i], buttonSize)) {
			currentOption = static_cast<OPTION>(i);
		}

		if (i == currentOption) {
			ImGui::PopStyleColor(3);
		}
	}

	ImGui::EndChild();
}

void Menu::mainPanel()
{
	ImVec2 windowSize = ImGui::GetIO().DisplaySize;
	ImGui::BeginChild("MainPanel", ImVec2(0.0f, windowSize.y - 45), true);
	centerText("MOD OPTIONS");
	ImGui::Separator();
	ImGui::Spacing();

	switch (currentOption)
	{
	case Player:
		playerMods();
	default:
		break;
	}

	ImGui::EndChild();
}

void Menu::centerText(const char* text) {
	float windowWidth = ImGui::GetWindowSize().x;
	float textWidth = ImGui::CalcTextSize(text).x;
	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text("%s", text);
}

void Menu::playerMods()
{
}
