#include "ModMenu.h"
#include "myfonts.h"

Active ModOptions;
Memory memory;
bool messageShown = false;

void CenterText(const char* text) {
	ImVec2 textSize = ImGui::CalcTextSize(text);
	float windowWidth = ImGui::GetWindowSize().x;
	float textPosX = (windowWidth - textSize.x) * 0.5f;

	ImGui::SetCursorPosX(textPosX);
	ImGui::Text("%s", text);

}

void LoadCustomFont(ImGuiIO& io) {
	ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;

	io.Fonts->AddFontFromMemoryTTF(RobotoMedium, RobotoMediumSize, 18.5f, &font_cfg);
}

void SetCustomTheme() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Background and Window
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);  // Darker background
	colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.95f);  // Slight transparency for popups

	// Headers
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.40f, 0.75f, 1.0f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.50f, 0.85f, 1.0f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.30f, 0.65f, 1.0f);

	// Borders
	colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 0.6f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

	// Buttons
	colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.47f, 0.85f, 1.0f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.57f, 0.95f, 1.0f);

	// Frames
	colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.0f);

	// Sliders
	colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.57f, 0.95f, 1.0f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.67f, 1.0f, 1.0f);

	// Scrollbar
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);

	// Checkboxes, Text Fields, etc.
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.67f, 1.0f, 1.0f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.47f, 0.85f, 0.5f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.40f, 0.75f, 1.0f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.50f, 0.85f, 1.0f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.60f, 0.95f, 1.0f);

	// Styling Tweaks
	style.WindowRounding = 10.0f;
	style.ChildRounding = 8.0f;
	style.FrameRounding = 6.0f;
	style.GrabRounding = 5.0f;
	style.ScrollbarRounding = 7.0f;
	style.PopupRounding = 8.0f;
	style.FrameBorderSize = 1.5f;
	style.WindowBorderSize = 1.2f;
	style.PopupBorderSize = 1.2f;
	style.GrabMinSize = 10.0f;

}

void SideBar(const std::vector<const char*>& labels, const ImVec2& sideBarSize, ImVec2 buttonSize) {
	ImGui::BeginChild("Sidebar", sideBarSize, true);
	CenterText("CheatMenu (v1.0.0)");
	ImGui::Separator();
	ImGui::Spacing();
	for (size_t i = 0; i < labels.size(); i++) {
		if (labels[i]) {
			if (ImGui::Button(labels[i], buttonSize)) {
				switch (i)
				{
				case 0:
					ModOptions = Active::Player;
					break;
				case 1:
					ModOptions = Active::Vehicle;
					break;
				default:
					break;
				}
			}
		}
	}
	ImGui::EndChild();
}

void PlayerGodMode(float& maxHealth) {
	memory.Freeze(memory.Health->pointsAddress, maxHealth);
	memory.Freeze(memory.Armor->pointsAddress, 255.0f);
}

void RenderModMenu() {

	static float health, maxHealth = 0.0f;
	static float armor = 0.0f;
	static CVector playerPosition(0.0f, 0.0f, 0.0f);

	static bool playerGodMode = false;

	static bool initialized = false;
	static byte menuState = false;

	if (!initialized) {
		memory.ReadMemory(memory.Health->pointsAddress, health);
		memory.ReadMemory(memory.Armor->pointsAddress, armor);
		initialized = true;
	}
	else {
		memory.ReadMemory(memory.MenuState->pointsAddress, menuState);
		memory.ReadMemory(memory.MaxHealth->pointsAddress, maxHealth);
		memory.ReadMemory(memory.PlayerPosX->pointsAddress, playerPosition.x);
		memory.ReadMemory(memory.PlayerPosY->pointsAddress, playerPosition.y);
		memory.ReadMemory(memory.PlayerPosZ->pointsAddress, playerPosition.z);
	}

	// Display Message Once
	if (!menuState && !messageShown) {
		memory.WriteMemory(memory.TextBox->pointsAddress, "Cheat Menu connected V1.0.0");
		messageShown = true;
	}

	// ImGui UI
	ImVec2 windowSize = ImGui::GetIO().DisplaySize;
	ImGui::SetNextWindowSize(windowSize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	if (ImGui::Begin("San Andreas Mod Menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {

		// Sidebar Navigation
		float sidebarWidth = 150.0f;
		float sidebarHeight = windowSize.y - 20;
		ImVec2 buttonSize(sidebarWidth - 20, 40);
		SideBar({ "Player", "Vehicle" }, ImVec2(sidebarWidth, sidebarHeight), buttonSize);
		ImGui::SameLine();

		// Mainpanel
		ImGui::BeginChild("MainPanel", ImVec2(0.0f, sidebarHeight), true);
		CenterText("MOD OPTIONS");
		ImGui::Separator();
		ImGui::Spacing();

		switch (ModOptions) {
		case Player:
			ImGui::Checkbox("GodMode", &playerGodMode);
			break;
		case Vehicle:
			break;
		default:
			break;
		}

		if (playerGodMode && !menuState) PlayerGodMode(maxHealth);
		ImGui::EndChild();
	}
	ImGui::End();

}
