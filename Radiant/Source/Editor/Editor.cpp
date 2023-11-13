#include "pch.h"
#include "Editor.h"
#include "Gui/GuiManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Texture/TextureManager.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Model.h"
#include "Physics/Collider.h"
#include "ECS/ECS.h"
#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "Utils/Input.h"
#include "Polygon/Polygon.h"
#include "Utils/MathTypes.h"
#include "Utils/Utils.h"
#include "IconsForkAwesome.h"


namespace fs = std::filesystem;

// =====================================================================================
enum rdt::core::EditorTheme {
	Theme_Codz,
	Nightingale,
	ET_NAT, // Not a theme
};

enum rdt::core::EditorFont {
	NunitoSans = 1,
	NunitoSans_Bold,
	ForkAwesome,
};
// =====================================================================================
namespace rdt::core {
	constexpr float GameWindowPanelGuiWidth = 1115;
	constexpr float GameWindowPanelGuiHeight = 653.0f;
	constexpr float PanelMargin = 7.5f;

	GameRenderWindow::GameRenderWindow()
	{
		m_gui_width = GameWindowPanelGuiWidth;
		m_gui_height = GameWindowPanelGuiHeight;
		update_pos = true;
	}

	GameRenderWindow::~GameRenderWindow()
	{
	}

	void GameRenderWindow::OnBegin()
	{
		ImGui::PushFont(GuiManager::GetFont(NunitoSans, 18));

		// First render config
		//ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
		
		/*if (update_pos) {
			ImGui::SetNextWindowPos(ImVec2((float)m_gui_pos.x, (float)m_gui_pos.y), ImGuiCond_Always);
			update_pos = false;
		}*/
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);

		ImGuiWindowFlags flags = 0;

		ImGui::Begin("Game Window", (bool*)0, flags);
		if (ImGui::GetMouseCursor() == ImGuiMouseCursor_ResizeNWSE) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}
	}

	void GameRenderWindow::OnEnd()
	{
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::PopFont();
	}

	void GameRenderWindow::TriggerUpdatePos()
	{
		update_pos = true;
	}

	// ==============================================================================
	/*
		PopUpWIndow Implementation
	*/

	namespace PanelDefaults {
		constexpr float DiagnosticGuiWidth = 300.0f;
		constexpr float DiagnosticGuiHeight = 105.0f;

		constexpr float ScenePanelGuiWidth = 300.0f;
		constexpr float ScenePanelGuiHeight = 400.0f;

		constexpr float EntityHierarchyPanelGuiWidth = 300.0f;
		constexpr float EntityHierarchyPanelGuiHeight = 400.0f;

		constexpr float ConsolePanelWidth = GameWindowPanelGuiWidth;
		constexpr float ConsolePanelHeight = 300.0f;

		constexpr float TemplateWizardGuiWidth = 500.0f;
		constexpr float TemplateWizardGuiHeight = 575.0f;

		constexpr float GameWindowSettingsPanelWidth = 250.0f;
	}

	// =========================================================
	using PanelID = int;
	struct Panel::Impl {

		PanelID mID = 0;
		bool isOpen = false;
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

		std::string GetWindowLabel(const std::string& title) const { return title + "###" + typeid(*this).name(); }
		bool Panel_ImGuiBegin(const std::string& title) { return ImGui::Begin(GetWindowLabel(title).c_str(), &isOpen, windowFlags); }

		/*
			Virtual function to render ImGui Window
		*/
		virtual void OnRender() {}

		/*
			Virtual function to get data from popup window.
		*/
		virtual void* OnGetData() { return nullptr; }

		/*
			Virtual function to set the config from the panel wrapper.
		*/
		virtual void ApplyConfigPreferences(PanelConfig& config) {}

		void OpenWindow() { isOpen = true; }
		void CloseWindow() { isOpen = false; }
	};
	// =========================================================
	struct OpenPanelRequestData
	{
		PanelType panelToOpen;

		OpenPanelRequestData(PanelType panel)
			: panelToOpen(panel) {}
	};
	struct ChangeThemeRequestData
	{
		EditorTheme theme;

		ChangeThemeRequestData(EditorTheme nTheme)
			: theme(nTheme) {}
	};
	struct MenuBarImpl : public Panel::Impl {

		void CallOpenPanel(PanelType panel) {
			/*MessageBus::SendDirectMessage(FROM_ANONYMOUS, MessageBus::GetMessageID("Editor"), CM_OpenPanelRequest,
				new OpenPanelRequestData(panel));*/
		}

		void CallChangeTheme(EditorTheme nTheme) {
			/*MessageBus::SendDirectMessage(FROM_ANONYMOUS, MessageBus::GetMessageID("Editor"), CM_ChangeSceneRequest,
				new ChangeThemeRequestData(nTheme));*/
		}

		void OnRender() override final {

			if (ImGui::BeginMainMenuBar()) {

				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Create")) {
					}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {
					}
					if (ImGui::MenuItem("Save", "Ctrl+S")) {
					}
					if (ImGui::MenuItem("Save as..")) {
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Tools")) {
					if (ImGui::MenuItem("Template Wizard")) {
						CallOpenPanel(TemplateWizard);
					}
					if (ImGui::MenuItem("Add Entity Definition")) {
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View")) {

					if (ImGui::MenuItem("Scene Hierarchy Panel")) {
						CallOpenPanel(ScenePanel);
					}
					if (ImGui::MenuItem("Entity Hierarchy Panel")) {
						CallOpenPanel(EntityHierarchyPanel);
					}
					if (ImGui::MenuItem("Diagnostics Panel")) {
						CallOpenPanel(DiagnosticsPanel);
					}
					if (ImGui::MenuItem("Console Window")) {
						CallOpenPanel(ConsolePanel);
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Themes")) {

					if (ImGui::MenuItem("Codz")) {
						CallChangeTheme(Theme_Codz);
					}
					if (ImGui::MenuItem("Nightingale")) {
						CallChangeTheme(Nightingale);
					}

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}
	};
	// =========================================================
	struct DiagnosticsPanelImpl : public Panel::Impl {

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::DiagnosticGuiWidth;
			config.height = PanelDefaults::DiagnosticGuiHeight;

			config.xPos = 100;
			config.yPos = 100;
		}
		
		void OnRender() override final {
			
			if (Panel_ImGuiBegin("Diagnostic Tools")) {
				// Show framerate
				Vec2d screenCoords = Input::GetMouseCoords(MouseCond::SCREEN_COORDS);
				Vec2d worldCoords = Input::GetMouseCoords(MouseCond::WORLD_COORDS);
				ImGui::Text("Mouse screen-coordinates: (%.2f %2.f)", screenCoords.x, screenCoords.y);
				ImGui::Text("Mouse world-coordinates: (%.2f %2.f)", worldCoords.x, worldCoords.y);
				ImGui::Text("Performance: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			ImGui::End();
		}
	};
	// =========================================================
	struct GameWindowPanelImpl : public Panel::Impl {

		GameWindowPanelImpl()
		{
			Editor::m_gameWindow->SetShouldShow(isOpen);

			Editor::m_gameWindow->SetGuiPositionY(88);
			Editor::m_gameWindow->SetGuiPositionX((Renderer::GetWindowWidth() / 2)
				- (Editor::m_gameWindow->GetGuiDimensions().x / 2));
		}

		void OnRender() override final {
			if (!Editor::m_gameWindow->ShouldShow()) {
				Editor::m_gameWindow->SetShouldShow(true);
			}
		}
	};
	// =========================================================
	struct GameWindowSettingsPanelImpl : public Panel::Impl {
		void OnRender() override final {
			ImGuiWindowFlags windowConfig = 0;
			windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

			ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.7f, 0.7f, 0.7f, 0.9f });
			ImGui::Begin("##GameWindowSettingsPanel", (bool*)0, windowConfig);

			ImGui::PushFont(Editor::m_fonts[ForkAwesome][18]);
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f, 0.8f, 0.8f, 1.0f });

			/* Play Button */
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 0.8f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.9f, 0.2f, 0.2f });
			ImGui::SetCursorPos({ 10, 10 });
			if (ImGui::Button(ICON_FK_PLAY)) {

			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();

			/* Pause Button */
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.9f, 0.2f, 0.2f, 0.2f });
			if (ImGui::Button(ICON_FK_PAUSE)) {

			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();

			/* Restart Button */
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f, 0.1f, 0.1f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.7f, 0.7f, 0.7f, 0.2f });
			if (ImGui::Button(ICON_FK_REFRESH)) {

			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();

			/* Expand Window Button*/
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f, 0.1f, 0.1f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.7f, 0.7f, 0.7f, 0.2f });
			if (Renderer::UsingDefaultViewport()) {
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - Editor::GetButtonWidth(ICON_FK_PAUSE) - 10);
				if (ImGui::Button(ICON_FK_COMPRESS)) {

					if (Renderer::IsFullscreen()) {
						Renderer::DisableFullscreen();
					}

					Renderer::SetDefaultViewport(false);
					Input::SetTargetRenderWindow(Editor::m_gameWindowId);
					
					/*m_diagnostics_panel.yPos = GetDockPosY(DockTop, m_diagnostics_panel.height, PanelMargin) + m_menu_bar_height;
					m_game_window_settings_panel.xPos = GetDockPosX(DockRight, m_game_window_settings_panel.width + m_diagnostics_panel.width + PanelMargin, PanelMargin);
					m_game_window_settings_panel.yPos = GetDockPosY(DockTop, m_game_window_settings_panel.height, PanelMargin) + m_menu_bar_height;
					m_diagnostics_panel.update = true;
					m_game_window_settings_panel.update = true;*/
				}
			}
			else {
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (2 * Editor::GetButtonWidth(ICON_FK_PAUSE)) - 18);

				if (ImGui::Button(ICON_FK_EXPAND)) {
					Renderer::SetDefaultViewport(true);
					Input::SetTargetRenderWindow(-1);
					
					/*m_showTools = false;
					m_game_window_settings_panel.yPos = PanelMargin;
					m_game_window_settings_panel.xPos = GetDockPosX(DockRight, m_game_window_settings_panel.width, PanelMargin);
					m_diagnostics_panel.yPos = m_game_window_settings_panel.yPos + m_game_window_settings_panel.height + PanelMargin;*/
				}

				ImGui::SameLine();
				if (ImGui::Button(ICON_FK_ARROWS_ALT)) {
					Renderer::SetDefaultViewport(true);
					Renderer::EnableFullscreen();
					Input::SetTargetRenderWindow(-1);

					/*m_showTools = false;
					m_game_window_settings_panel.yPos = PanelMargin;
					m_game_window_settings_panel.xPos = GetDockPosX(DockRight, m_game_window_settings_panel.width, PanelMargin);
					m_diagnostics_panel.yPos = m_game_window_settings_panel.yPos + m_game_window_settings_panel.height + PanelMargin;*/
				}
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();


			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::PopFont();

			ImGui::End();
			ImGui::PopStyleColor();
		}
	};
	// =========================================================
	struct LayerPanelImpl : public Panel::Impl {
		std::string panel_header = "Layer: ";
		float lastGameObjectPanelHeight = 0.0f;
		std::shared_ptr<Layer>layer = nullptr;
		float nodeListMaxY = 500.0f;

		enum NodeType {
			LPNT_Entity,
		};

		struct Node {
			float lastSize = 0.0f;
			bool nodeOpen = false;
			size_t itemCount = 0;
			NodeType type;
		};

		// Index 0: Entity Node
		// Index 1: Game Object Node
		Node nodes[1];

		LayerPanelImpl(std::shared_ptr<Layer> ref)
			: layer(ref) 
		{
			if (ref == nullptr) {
				RDT_CORE_ERROR("LayerPanelImpl - Tried to use nullptr layer ref");
				panel_header += "ERROR";
				return;
			} 
			panel_header += typeid(*layer).name();

			nodes[0].type = LPNT_Entity;
		}

		void AddEntityItem(Entity entity) {
			std::string panel_header = "Entity: ";
			const char* alias = EntityManager::GetEntityAlias(entity);
			if (alias == nullptr) {
				alias = "ERROR";
			}
			panel_header += alias;
			
			if (ImGui::CollapsingHeader(panel_header.c_str())) {

				Signature signature = EntityManager::GetSignature(entity);
				
				if (ImGui::Button("Open in Entity Viewer")) {

				}

				ImGui::Indent(10);
				ImGui::Text("Components (%d):", signature.count());


				ImGui::Indent(10);
				for (int index = 0; index < signature.size(); index++) {
					if (signature[index]) {
						std::string componentName = ComponentManager::GetComponenentName(index);
						ImGui::Text("%s", componentName.c_str());
					}
				}
				ImGui::Unindent(10);

				std::vector<std::string> registeredSystems;
				for (auto& [systemName, systemPtr] : SystemManager::GetSystemMap()) {
					if (systemPtr->HasEntity(entity)) {
						registeredSystems.push_back(systemName);
					}
				}
				ImGui::Text("Systems (%d):", registeredSystems.size());
				ImGui::Indent(10);
				for (auto& name : registeredSystems) {
					ImGui::Text("%s", name.c_str());
				}
				ImGui::Unindent(20);
			}
		}

		void AddEntityPanel() {

			nodes[0].itemCount = layer->GetEntities().size();
			RenderNode(nodes[0]);
		}

		void RenderNode(Node& node) {

			ImVec2 nodeChildWindowSize = ImVec2(ImGui::GetWindowSize().x * 0.90f, node.lastSize);

			if (!node.nodeOpen) {
				nodeChildWindowSize.y = 25;
			}
			else if (node.lastSize == 0.0f) {
				nodeChildWindowSize.y = Utils::Min(nodeListMaxY, (node.itemCount * 34.0f) + 25);
			}

			float indent = (ImGui::GetWindowSize().x - nodeChildWindowSize.x) / 2;
			ImGui::SetCursorPosX(indent);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 10);

			std::string child1 = (panel_header + "- " + (node.type == LPNT_Entity ? "Entity" : "GameObject") + "Node");
			if (ImGui::BeginChild(child1.c_str(), nodeChildWindowSize, false, ImGuiWindowFlags_NoScrollbar)) {

				if (node.nodeOpen = ImGui::CollapsingHeader(((node.type == LPNT_Entity ? "Entities: " : "GameObjects: ") + std::to_string(node.itemCount)).c_str())) {
					ImGui::Text(("Browse " + (std::string)(node.type == LPNT_Entity ? "Entities: " : "GameObjects: ")).c_str());
					if (node.itemCount > 0) {

						nodeChildWindowSize.x *= 0.95f;
						nodeChildWindowSize.y -= 75.0f;
						indent = (ImGui::GetWindowSize().x - nodeChildWindowSize.x) / 2;
						ImGui::SetCursorPosX(indent);
						std::string child2 = (panel_header + "- " + (node.type == LPNT_Entity ? "Entity" : "GameObject") + "List");
						if (ImGui::BeginChild(child2.c_str(), nodeChildWindowSize)) {

							switch (node.type) {
							case LPNT_Entity:
								for (auto entity : layer->GetEntities()) {
									AddEntityItem(entity);
								}
								break;
							}
						}
						node.lastSize = Utils::Min(nodeListMaxY, ImGui::GetCursorPosY() + 4 + 75);
						ImGui::EndChild();
					}
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar(1);
		}

		void OnRender() override final {
			if (ImGui::CollapsingHeader(panel_header.c_str())) {
				AddEntityPanel();
			}
		}
	};
	// =========================================================
	struct ScenePanelImpl : public Panel::Impl {

		std::unordered_map<UniqueID, LayerPanelImpl*> m_layers;

		~ScenePanelImpl()
		{
			for (auto& [layerID, layerPanel] : m_layers) {
				delete layerPanel;
			}
		}

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::ScenePanelGuiWidth;
			config.height = PanelDefaults::ScenePanelGuiHeight;

			config.xPos = 500;
			config.yPos = 100;
		}

		void AddLayerPanel(std::shared_ptr<Layer> layer) {

			if (m_layers.find(layer->GetID()) == m_layers.end()) {
				m_layers[layer->GetID()] = new LayerPanelImpl(layer);
			}
		}

		void RenderLayerPanel(std::shared_ptr<Layer> layer) {
			m_layers.at(layer->GetID())->OnRender();
		}

		void OnRender() override final {

			if (Panel_ImGuiBegin("Scene Hierarchy")) {
				if (Editor::m_scene == nullptr) {
					ImGui::Text("No Scene Selected!");
				}
				else {
					ImGui::Text("Scene: %s", Editor::m_scene->GetName());
					for (auto& layer : Editor::m_scene->GetLayers()) {
						AddLayerPanel(layer);
						RenderLayerPanel(layer);
					}
				}
			}

			ImGui::End();
		}
	};
	// =========================================================
	struct EntityHierarchyPanelImpl : public Panel::Impl {

		Entity selectedEntity = NO_ENTITY_ID;

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::EntityHierarchyPanelGuiWidth;
			config.height = PanelDefaults::EntityHierarchyPanelGuiHeight;

			config.xPos = 200;
			config.yPos = 900;
		}
		
		void RenderEntityTable() {
			ImGuiTableFlags flags = ImGuiTableFlags_None;
			flags |= ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY;

			if (ImGui::BeginTable("EntityTable", 3, flags, ImVec2(0, 352))) {
				ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 30);
				ImGui::TableSetupColumn("Alias");
				ImGui::TableSetupColumn("# of Components");
				ImGui::TableHeadersRow();

				int current_row = 0;

				for (auto& [entity, entitySignature] : EntityManager::GetEntityMap()) {

					ImGui::TableNextRow();
					if (entity == selectedEntity) {
						ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(0.8f, 0.1f, 0.2f, 1.0f));
					}
					else {
						if (current_row % 2 == 0) {
							ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(0.1f, 0.1f, 0.1f, 1.0f));
						}
						else {
							ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(0.2f, 0.2f, 0.2f, 1.0f));
						}
					}

					ImGui::TableNextColumn();

					char buffer[25];
					bool hovered = false;
					sprintf_s(buffer, 25, "##EntityTableRow%d", current_row);

					if (entity != selectedEntity) {
						ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
						ImGui::Selectable(buffer, &hovered, ImGuiSelectableFlags_SpanAllColumns);
						ImGui::PopStyleColor(2);
						if (ImGui::IsItemClicked()) {
							selectedEntity = entity;
						}

						ImGui::SameLine();
					}

					ImGui::Text("%d", entity);


					ImGui::TableNextColumn();
					ImGui::Text("%s", EntityManager::GetEntityAlias(entity));

					ImGui::TableNextColumn();
					ImGui::Text("%d", entitySignature.count() - RDT_NUM_HIDDEN_COMPONENTS);

					current_row++;
				}

				for (; current_row < 15; current_row++) {
					ImGui::TableNextRow();
					ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImColor(0.1f, 0.1f, 0.1f, 1.0f));
					
					ImGui::TableNextColumn();
					ImGui::Text("");
				}
				ImGui::EndTable();
			}
		}
		
		void ShowSetTexturePanel(bool* isOpen) {
			char texSelectionLabel[60];
			sprintf_s(texSelectionLabel, 60, "Set Texture for Entity [id:%d] ###SetTexturePanel", selectedEntity);

			ImGui::SetNextWindowSize(ImVec2(300, 350), ImGuiCond_Appearing);

			ImGuiWindowFlags setTexturePanelFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar;
			if (ImGui::Begin(texSelectionLabel, isOpen, setTexturePanelFlags)) {
				if (ImGui::BeginMenuBar()) {
					if (ImGui::BeginMenu("File")) {
						if (ImGui::MenuItem("Load Texture (.png)")) {
						}
						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}
			}
			ImGui::End();
		}

		void RenderEditTool(const char* name, void* data, const core::TraceData& info) {
			
			auto next_sub_row = []() {
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::TableNextColumn();
				};
			auto title = [](const char* type, const char* name) {
				ImGui::PushFont(Editor::m_fonts[NunitoSans_Bold][18]);
				ImGui::Text(type);
				ImGui::PopFont();
				ImGui::SameLine();
				ImGui::Text(name);
				ImGui::SameLine();
				ImGui::InvisibleButton("##rowheightadjustment", ImVec2(1, ImGui::GetFontSize() + (ImGui::GetStyle().FramePadding.y * 2)));
				ImGui::TableNextColumn();
				};

			if (data == nullptr) {
				ImGui::Text("ERROR");
				return;
			}
			data = (unsigned char*)data + info.offset;

			switch (info.type) {
			case SupportedTraceType_textureID:
			{
				title("TextureID", name);

				
				static bool showTextureSelection = false;

				TextureID texture = *((TextureID*)data);
				const char* texture_name = TextureManager::GetTextureAlias(texture);
				ImGui::Text("Using:");
				ImGui::SameLine();
				ImGui::PushFont(Editor::m_fonts[NunitoSans_Bold][18]);
				ImGui::Text(texture_name);
				ImGui::PopFont();
				
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - Editor::GetButtonWidth("Browse        "));
				if (ImGui::Button("Browse        ")) {
					showTextureSelection = true;
				}

				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::CalcTextSize("C").x - ImGui::GetStyle().FramePadding.x - ImGui::GetStyle().ItemSpacing.x * 2);
				Editor::AddIcon(ICON_FK_FILE_IMAGE_O);

				next_sub_row();

				// Show what texture looks like
				Vec2i img_size = Vec2i::Zero();
				glTextureID glTexID = Editor::GetImGuiTextureData(texture, img_size.x, img_size.y);
				img_size.y = ((ImGui::GetContentRegionAvail().x - 2) * img_size.y) / img_size.x;
				img_size.x = ImGui::GetContentRegionAvail().x - 2;

				ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
				ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
				ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);

				ImGui::Image((void*)(intptr_t)glTexID, ImVec2(img_size.x, img_size.y), uv_min, uv_max, tint_col, border_col);
				
				if (showTextureSelection) {
					ShowSetTexturePanel(&showTextureSelection);
				}
			}
			break;
			case SupportedTraceType_bool:
			{
				title("Bool", name);
				bool val = *((bool*)data);

				ImGui::Text("Enable:");
				ImGui::SameLine();
				
				char checkbox_label[40];
				sprintf_s(checkbox_label, 40, "##checkboxLabel_%s", name);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				if (ImGui::Checkbox(checkbox_label, &val)) {
					*((bool*)data) = val;
				}
				ImGui::PopStyleVar();
				
			}
				break;
			case SupportedTraceType_modelID:
			{
				title("ModelID", name);

				ModelID model = *((ModelID*)data);
				const char* model_name = ModelManager::GetModelAlias(model);
				ImGui::Text("Name:");
				ImGui::SameLine();
				ImGui::PushFont(Editor::m_fonts[NunitoSans_Bold][18]);
				ImGui::Text(model_name);
				ImGui::PopFont();
			}
				break;
			case SupportedTraceType_colliderID:
			{
				title("ColliderID", name);

				ColliderID colliderID = *((ColliderID*)data);
				
				const char* collider_name = ColliderManager::GetColliderAlias(colliderID);
				ImGui::Text("Name:");
				ImGui::SameLine();
				ImGui::PushFont(Editor::m_fonts[NunitoSans_Bold][18]);
				ImGui::Text(collider_name);
				ImGui::PopFont();

				next_sub_row();
				ImGui::Text("Show Hitbox:");
				ImGui::SameLine();

				DebugComponent* dc = Editor::GetDebugComponent(selectedEntity);
				
				char checkbox_label[40];
				sprintf_s(checkbox_label, 40, "##checkboxLabel_%s", name);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
				if(ImGui::Checkbox(checkbox_label, &dc->show_collider_hitbox)) {

				}
				ImGui::PopStyleVar();

			}
				break;
			case SupportedTraceType_angle:
			{
				title("Angle", name);

				char combo_label[40];
				sprintf_s(combo_label, 40, "##angleUnitOption_%s", name);
				int selection = Editor::GetComboxBoxResult(combo_label);

				float val = ((Angle*)data)->radians;
				char label[30];
				sprintf_s(label, 30, "##editAngle_%s", name);

				if (selection <= 0) {
					if (ImGui::SliderFloat(label, &val, 0.0f, 2 * (float)M_PI)) {
						((Angle*)data)->radians = val;
					}
				}
				else {
					val = Utils::RadiansToDegrees(val);
					if (ImGui::SliderFloat(label, &val, 0.0f, 360)) {
						((Angle*)data)->radians = Utils::DegreesToRadians(val);
					}
				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 5);

				const char* options[] = { "  radians", "  degrees" };
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				Editor::AddComboBox(combo_label, options, 2, "  radians");
				ImGui::PopItemWidth();
			}
			break;
			case SupportedTraceType_float:
			{
				title("float", name);

				float val = *((float*)data);
				char label[30];
				sprintf_s(label, 30, "##editfloat_%s", name);
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::DragFloat(label, &val, 2)) {
					*((float*)data) = val;
				}
				ImGui::PopItemWidth();

			}
			break;
			case SupportedTraceType_vec2d:
			{
				title("Vec2d", name);

				float valX = (float)((Vec2d*)data)->x;
				float valY = (float)((Vec2d*)data)->y;

				// X label
				float xStart = ImGui::GetCursorPosX();
				ImGui::Text("x:");
				ImGui::SameLine();
				float itemWidth = (ImGui::GetContentRegionAvail().x - (ImGui::GetCursorPosX() - xStart)) / 2;

				// Edit X
				char label[30];
				sprintf_s(label, 30, "##editX_%s", name);
				ImGui::PushItemWidth(itemWidth);
				if (ImGui::DragFloat(label, &valX, 2)) {
					((Vec2d*)data)->x = (double)valX;
				}
				ImGui::PopItemWidth();

				// Y label
				ImGui::SameLine();
				ImGui::Text("y:");

				// Edit Y
				ImGui::SameLine();
				sprintf_s(label, 30, "##editY_%s", name);
				ImGui::PushItemWidth(itemWidth);
				if (ImGui::DragFloat(label, &valY, 2)) {
					((Vec2d*)data)->y = (double)valY;
				}
				ImGui::PopItemWidth();
			}
				break;
			case SupportedTraceType_color:
			{
				title("Color", name);

				auto& color = ((Color*)data)->GetColor();
				ImVec4 colVals = { color.x1, color.x2, color.x3, color.x4 };

				ImGuiColorEditFlags flags = 0;
				//flags |= ImGuiColorEditFlags_DisplayRGB;
				static bool openColorPicker = false;
				
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::ColorEdit4("##editColor4", &colVals.x)) {
					*((Color*)data) = Color(colVals.x, colVals.y, colVals.z, colVals.w);
				}
				ImGui::PopItemWidth();

				if (openColorPicker) {
					ImGui::SetNextWindowSize(ImVec2(270, 280), ImGuiCond_Appearing);
					ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize;
					if (ImGui::Begin("Edit Color ###ColorPickerWindow", &openColorPicker, flags)) {
						if (ImGui::ColorPicker4("##picker", &colVals.x, 0)) {
							*((Color*)data) = Color(colVals.x, colVals.y, colVals.z, colVals.w);
						}
					}
					ImGui::End();
				}
			}
				break;
			case SupportedTraceType_polygon:
			{	
				title("Polygon", name);
				
				Vec2d coords = (*((std::shared_ptr<Polygon>*)data))->GetOrigin();
				float vals[2] = { (float)coords.x, (float)coords.y };


				ImGui::Text("Position:");
				ImGui::SameLine();
				float xAlign = ImGui::GetCursorPosX();
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::DragFloat2("##updateOrigin", vals, 2)) {
					(*((std::shared_ptr<Polygon>*)data))->SetPosition({ vals[0], vals[1] });
				}
				ImGui::PopItemWidth();		
				next_sub_row();

				vals[0] = (float)(*((std::shared_ptr<Polygon>*)data))->GetWidth();
				vals[1] = (float)(*((std::shared_ptr<Polygon>*)data))->GetHeight();
				ImGui::Text("Size:");
				ImGui::SameLine();
				ImGui::SetCursorPosX(xAlign);
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::DragFloat2("##updateSize", vals)) {
					(*((std::shared_ptr<Polygon>*)data))->SetSize({ vals[0], vals[1] });
				}
				ImGui::PopItemWidth();
			}
			break;
			case SupportedTraceType_uint:
			{
				title("unsigned int", name);

				unsigned int val = *((unsigned int*)data);

				int input = val;

				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				char label[60];
				sprintf_s(label, 60, "##updateuint_%s", name);
				if (ImGui::InputInt(label, &input)) {
					if (input >= 0) {
						*((unsigned int*)data) = input;
					}
				}
				ImGui::PopItemWidth();
			}
				break;
			case SupportedTraceType_double:
			{
				title("double", name);

				float val = (float)(*((double*)data));

				char label[60];
				sprintf_s(label, 60, "##updatedouble_%s", name);
				ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::DragFloat(label, &val, 2)) {
					*((double*)data) = (double)val;
				}
				ImGui::PopItemWidth();
			}
			break;
			default:
				title("(?)", name);
				ImGui::Text("No introspection support!");
				break;
			}
		}

		void RenderEntityViewer()
		{
			std::string headerLabel = "Entity Viewer: ";
			if (selectedEntity == NO_ENTITY_ID) {
				headerLabel += "No Entity Selected";
			}
			else {
				headerLabel += " [ID: " + std::to_string(selectedEntity) + "] " + EntityManager::GetEntityAlias(selectedEntity);
			}

			if (ImGui::TreeNodeEx((const void*)this, ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_DefaultOpen, headerLabel.c_str())) {

				if (selectedEntity == NO_ENTITY_ID) {
					ImGui::Text("Select an entity from the Entity List to view it here.");
					return;
				}

				Layer* owner = EntityManager::GetEntityOwner(selectedEntity);
				ImGui::PushFont(Editor::m_fonts[NunitoSans_Bold][18]);
				ImGui::Text("Owned by Layer:");
				ImGui::PopFont();
				ImGui::SameLine();
				ImGui::Text("%s", owner == nullptr ? "None" : typeid(*owner).name());

				Signature signature = EntityManager::GetSignature(selectedEntity);

				for (int index = 0; index < signature.size(); index++) {
					if (signature[index]) {
						std::string componentName = ComponentManager::GetComponenentName(index);
						
						// Don't show hidden component
						if (ComponentManager::IsHiddenComponent(componentName)) {
							continue;
						}

						// Component Icon
						Editor::AddIcon(ICON_FK_CUBES);
						ImGui::SameLine();
						float headerStartX = ImGui::GetCursorPosX();

						// Component Collapsing Header
						char checkboxLabel[60];
						sprintf_s(checkboxLabel, 60, "##%s_enable", componentName.c_str());
						ImGui::PushFont(Editor::m_fonts[NunitoSans_Bold][18]);
						bool componentMenuOpen = ImGui::TreeNodeEx(componentName.c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_AllowItemOverlap, componentName.c_str());
						ImGui::PopFont();
						
						// Set component enable button position
						ImGui::SameLine();
						ImGui::SetCursorPosX(headerStartX + ImGui::GetItemRectSize().x - (ImGui::CalcTextSize("C").x + 13.5f + ImGui::GetStyle().ItemInnerSpacing.x));
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.5);

						// Component enable button
						bool isChecked = EntityManager::IsComponentEnabled(selectedEntity, index);
						ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
						ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.1f, 0.1f, 0.1f, 1.0f));
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						if (ImGui::Checkbox(checkboxLabel, &isChecked)) {
							if (isChecked) {
								EntityManager::EnableComponent(selectedEntity, index);
							}
							else {
								EntityManager::DisableComponent(selectedEntity, index);
							}
						}
						ImGui::PopStyleVar();
						ImGui::PopStyleColor(3);

						// If component collapsing header is open
						if (componentMenuOpen) {
							ImGui::Indent(10);

							if (!ComponentTraceTracker::GetTraceData(componentName.c_str()).size()) {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
								ImGui::Text("Could not find any tracable data.");
								ImGui::Text("(Did you remember to call TRACE_COMPONENT_DATA() on its members?");
								ImGui::PopStyleColor();
							}
							else {
								char tableLabel[60];
								sprintf_s(tableLabel, 60, "##%s_datatable", componentName.c_str());
								if (ImGui::BeginTable(tableLabel, 2)) {
									ImGui::TableSetupColumn("memberName", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("###################").x);
									ImGui::TableSetupColumn("dataIntrospection");
									
									void* entity_data = ComponentManager::GetData(index, selectedEntity);
									for (auto& type_definition : ComponentTraceTracker::GetTraceData(componentName.c_str())) {
										ImGui::TableNextRow();
										ImGui::TableNextColumn();
										RenderEditTool(type_definition.name, entity_data, type_definition);
									}
									ImGui::EndTable();
								}
							}
						
							ImGui::Unindent(10);

							//ImGui::Separator();
						}
					}
				}
			}
		}
		void OnRender() override final {
			if (Panel_ImGuiBegin("Entity Component System")) {

				RenderEntityTable();
				RenderEntityViewer();
			}
			ImGui::End();
		}
	};
	// =========================================================
	struct ConsolePanelImpl : public Panel::Impl {

		std::string m_last_log = "";

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::ConsolePanelWidth;
			config.height = PanelDefaults::ConsolePanelHeight;

			config.xPos = 200;
			config.yPos = 900;
		}

		void OnRender() override final {

			ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.1f, 0.1f, 0.1f, 1.0f });

			if (Panel_ImGuiBegin("Console Window")) {

				int index = (int)Log::GetLogCount() - 1;
				std::string log;
				Color logColor;
				while (index >= 0) {
					Log::GetLog(index--, log, logColor);

					ImGui::PushStyleColor(ImGuiCol_Text, logColor.GetImGuiColor());
					ImGui::Text("%s", log.c_str());
					ImGui::PopStyleColor();
				}

				if (m_last_log != log) {
					ImGui::SetScrollHereY(0.999f);
					m_last_log = log;
				}
			}
			ImGui::End();
			ImGui::PopStyleColor();
		}
	};
	// =========================================================
	struct TemplateWizardImpl : public Panel::Impl {

		int m_template_selection_index = -1;
		char m_template_name[60];
		bool m_template_name_edited = false;

		TemplateWizardImpl ()
		{
			windowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar 
				| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;

			strcpy_s(m_template_name, 60, "");
		}

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::TemplateWizardGuiWidth;
			config.height = PanelDefaults::TemplateWizardGuiHeight;
			config.xPos = (Renderer::GetWindowWidth() / 2) - (config.width / 2);
			config.yPos = (Renderer::GetWindowHeight() / 2) - (config.height / 2);
			config.update = true;
		}

		enum TemplateType {
			T_GameObject,
			T_Layer,
			T_Scene,
		};

		void CreateFileFromTemplate(TemplateType type, const std::string& name)
		{

			fs::path filepath = fs::path(Editor::GetProjectSourcePath());
			fs::path template_h = fs::path(Editor::GetRadiantTemplatePath());
			fs::path template_cpp = fs::path(Editor::GetRadiantTemplatePath());


			switch (type) {
			case T_GameObject:
				filepath = filepath / fs::path("GameObjects");
				template_h = template_h / fs::path("gameObject.h.txt");
				template_cpp = template_cpp / fs::path("gameObject.cpp.txt");
				break;
			case T_Layer:
				filepath = filepath / fs::path("Layers");
				template_h = template_h / fs::path("layer.h.txt");
				template_cpp = template_cpp / fs::path("layer.cpp.txt");
				break;
			case T_Scene:
				filepath = filepath / fs::path("Scenes");
				template_h = template_h / fs::path("scene.h.txt");
				template_cpp = template_cpp / fs::path("scene.cpp.txt");
				break;
			default:
				return;
			}

			if (!Utils::PathExists(template_h.generic_string())) {
				RDT_CORE_WARN("Could not locate template header at '{}'", template_h.generic_string());
				return;
			}

			if (!Utils::PathExists(template_cpp.generic_string())) {
				RDT_CORE_WARN("Could not locate template source at '{}'", template_cpp.generic_string());
				return;
			}

			if (!fs::is_directory(filepath) || !fs::exists(filepath)) {
				fs::create_directory(filepath);
			}
			std::string headerFile = (filepath / fs::path(name + ".h")).generic_string();
			std::string srcFile = (filepath / fs::path(name + ".cpp")).generic_string();

			if (Utils::PathExists(headerFile) || Utils::PathExists(srcFile)) {
				RDT_CORE_WARN("Could not create from template, duplicate '{}' found!", name);
				return;
			}

			std::string contents;

			// Write header file;
			Utils::ReadTextFile(template_h.generic_string(), contents);
			Utils::ReplaceAll(contents, "TEMPLATENAME", name);
			Utils::WriteFile(headerFile, contents);
			contents.clear();

			// Write src file
			Utils::ReadTextFile(template_cpp.generic_string(), contents);
			Utils::ReplaceAll(contents, "TEMPLATENAME", name);
			Utils::WriteFile(srcFile, contents);

		}

		bool ValidTemplateName(const std::string& name, std::string& errorMsg) {
			if (name.length() == 0) {
				errorMsg = "Template name cannot be empty!";
				return false;
			}

			for (int i = 0; i < name.length(); i++) {
				if (name[i] >= 'A' && name[i] <= 'Z') {
					continue;
				}
				if (name[i] >= 'a' && name[i] <= 'z') {
					continue;
				}
				if (name[i] >= '0' && name[i] <= '9') {

					if (i == 0) {
						errorMsg = "Template cannot start with a number!";
						return false;
					}
					continue;
				}
				if (name[i] == '_') {
					continue;
				}

				errorMsg = "Invalid character '";
				errorMsg += name[i];
				errorMsg += "'!";
				return false;
			}
			return true;
		}

		void OnRender() override final {

			if (Panel_ImGuiBegin("")) {
				bool createRequested = false;

				ImGui::PushFont(Editor::m_fonts[NunitoSans][36]);
				Editor::AddCenteredText("Template Wizard");
				ImGui::PopFont();
				ImGui::PushFont(Editor::m_fonts[NunitoSans][24]);
				Editor::AddCenteredText("-- Create New File -- ");
				ImGui::NewLine();

				ImGui::Indent(80);

				const char* items[] = { "Game Object", "Layer", "Scene" };
				const char* preview = m_template_selection_index < 0 ? "Template Options" : items[m_template_selection_index];

				if (ImGui::BeginCombo("##TemplateOptions", preview))
				{

					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (m_template_selection_index == n);
						if (ImGui::Selectable(items[n], is_selected)) {
							m_template_selection_index = n;
							strcpy_s(m_template_name, 60, (std::string("Name of ") + items[n]).c_str());
							m_template_name_edited = false;
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::NewLine();
				ImGuiInputTextFlags textFlags;
				textFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackEdit;

				if (m_template_selection_index < 0) {
					textFlags |= ImGuiInputTextFlags_ReadOnly;
					Editor::InactiveTextBoxBegin();
				}

				if (ImGui::InputText("##Filename", m_template_name, 60, textFlags, Editor::MyTextCallback, (void*)&m_template_name_edited)) {
					createRequested = true;
				}

				if (m_template_selection_index < 0) {
					Editor::InactiveTextBoxEnd();
				}

				bool validName = false;
				if (m_template_name_edited) {
					std::string errorMsg;
					validName = ValidTemplateName(m_template_name, errorMsg);

					if (!validName) {
						ImGui::NewLine();
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.2f, 0.2f, 1.0f });
						ImGui::Text("* %s", errorMsg.c_str());
						ImGui::PopStyleColor();
					}
				}

				ImGui::Unindent(80);
				ImGui::Indent(195);

				ImGui::SetCursorPosY(500);

				if (!validName) { Editor::InactiveButtonBegin(); }
				if (ImGui::Button("Create File") && validName) {
					createRequested = true;
				}
				if (!validName) { Editor::InactiveButtonEnd(); }

				ImGui::Unindent(160);
				ImGui::PopFont();

				if (validName && createRequested) {
					CreateFileFromTemplate((TemplateType)m_template_selection_index, m_template_name);
				}
			}

			ImGui::End();

			if (!isOpen) {
				// Reset form on close
				m_template_selection_index = -1;
				strcpy_s(m_template_name, 60, "");
				m_template_name_edited = false;
			}
		}
	};
	// =========================================================
	struct EntityWizardImpl : public Panel::Impl {

		bool m_entityWizardLaunched = false;
		bool m_template_name_edited = false;
		char m_entity_name[60];

		void OnRender() override final {
			if (!m_entityWizardLaunched) {
				return;
			}

			ImGuiWindowFlags windowConfig = 0;
			windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

			if (ImGui::Begin("##Entity Wizard", &m_entityWizardLaunched, windowConfig)) {
				ImGui::PushFont(Editor::m_fonts[NunitoSans][36]);
				Editor::AddCenteredText("Entity Wizard");
				ImGui::PopFont();
				ImGui::PushFont(Editor::m_fonts[NunitoSans][24]);
				Editor::AddCenteredText("-- Create Entity Definition -- ");
				ImGui::PopFont();

				ImGui::NewLine();
				ImGuiInputTextFlags textFlags;
				textFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackEdit;

				if (ImGui::InputText("##Filename", m_entity_name, 60, textFlags, Editor::MyTextCallback, (void*)&m_template_name_edited)) {
				}
			}

			ImGui::End();
		}
	};
	// =========================================================

	Panel::Panel(PanelType type)
	{
		m_impl = nullptr;
		SetPanelType(type);
	}

	Panel::~Panel()
	{
		if (m_impl != nullptr) {
			delete m_impl;
		}
	}

	void Panel::SetPanelType(PanelType type)
	{
		if (m_impl != nullptr) {
			delete m_impl;
		}

		m_type = type;

		switch (type)
		{
		case PanelType::PT_NAP:
			m_impl = nullptr;
			break;
		case PanelType::MenuBar:
			m_impl = new MenuBarImpl;
			break;
		case PanelType::DiagnosticsPanel:
			m_impl = new DiagnosticsPanelImpl;
			break;
		case PanelType::ScenePanel:
			m_impl = new ScenePanelImpl;
			break;
		case PanelType::EntityHierarchyPanel:
			m_impl = new EntityHierarchyPanelImpl;
			break;
		case PanelType::ConsolePanel:
			m_impl = new ConsolePanelImpl;
			break;
		case PanelType::GameWindowSettingsPanel:
			m_impl = new GameWindowSettingsPanelImpl;
			break;
		case PanelType::GameWindowPanel:
			m_impl = new GameWindowPanelImpl;
			break;
		case PanelType::TemplateWizard:
			m_impl = new TemplateWizardImpl;
			break;
		case PanelType::EntityWizard:
			m_impl = new EntityWizardImpl;
			break;
		default:
			RDT_CORE_ERROR("Unknown popup window type!");
			m_impl = new Panel::Impl;
			break;
		}

		if (m_impl == nullptr) {
			return;
		}

		m_impl->mID = type;
		m_impl->ApplyConfigPreferences(m_config);
	}

	void Panel::Render()
	{
		if (!m_impl->isOpen) {
			return;
		}

		Editor::ApplyGuiConfig(m_config);
		m_impl->OnRender();
	}

	void* Panel::GetData()
	{
		return m_impl->OnGetData();
	}

	PanelConfig& Panel::GetConfig()
	{
		return m_config;
	}

	PanelType Panel::GetType()
	{
		return m_type;
	}
	
	void Panel::SetShow(bool shouldShow)
	{
		m_impl->isOpen = shouldShow;
	}
	// ==============================================================================

	/*
		Docking Layout Defualts
	*/

	namespace LayoutDefaults {
		constexpr float BorderTopHeight = 15.0f;
	}

	PanelManager::PanelManager()
	{
	}

	PanelManager::~PanelManager()
	{
		for (auto& [type, panel] : m_panels) {
			delete panel;
		}
	}

	void PanelManager::RegisterPanel(PanelType type)
	{
		m_panels[type] = new Panel(type);
	}

	void PanelManager::OpenPanel(PanelType type)
	{
		if (m_panels.find(type) == m_panels.end()) {
			RDT_CORE_ERROR("Could not open panel of type: {}", type);
			return;
		}

		m_panels.at(type)->SetShow(true);
	}

	const std::unordered_map<PanelType, void*>& PanelManager::GetMessages()
	{
		for (auto& [type, panel] : m_panels) {
			m_messages[type] = panel->GetData();
		}

		return m_messages;
	}

	void PanelManager::RenderMDI()
	{
		for (auto& [type, panel] : m_panels) {
			panel->Render();
		}
	}
}
// =====================================================================================
// Static Variables
std::shared_ptr<rdt::Scene> rdt::core::Editor::m_scene = nullptr;
std::unordered_map<rdt::core::EditorFont, std::unordered_map<unsigned int, ImFont*>> rdt::core::Editor::m_fonts;
rdt::core::GameRenderWindow* rdt::core::Editor::m_gameWindow = nullptr;
int rdt::core::Editor::m_gameWindowId = -1;
std::unordered_map<std::string, int> rdt::core::Editor::m_combo_selections = std::unordered_map<std::string, int>();
std::unordered_map<std::string, bool> rdt::core::Editor::m_checkbox_selections = std::unordered_map<std::string, bool>();

rdt::core::Editor* rdt::core::Editor::m_instance = nullptr;

rdt::core::Editor::Editor()
	: m_showEditor(true)
{
	RDT_CORE_TRACE("Launching editor...");
	Renderer::AttachGui(this);

	// Find base directory for child application
	m_project_base_directory = Utils::GetCWD();
	RDT_CORE_INFO("Base directory defined: {}", m_project_base_directory);

	// Get child application's Radiant profile
	fs::path configFile = fs::path(m_project_base_directory) / fs::path("radiant.ini");
	m_config.SetTargetFile(configFile.generic_string());
	m_config.Read();

	// Get the source directory for the child application
	fs::path sourceFile = fs::path(m_project_base_directory) / fs::path("Source");
	SetSourcePath(sourceFile.generic_string());

	// Get attributes from Radiant profile
	if (m_config.GetAttribute("Core", "ProjectName", m_projectName)) {
		RDT_CORE_TRACE("DevTools enabled for '{}'", m_projectName);
	}
	else {
		RDT_CORE_WARN("No project found!");
	}

	if (m_config.GetAttribute("Core", "Resources", m_radiant_resources_filepath)) {
		RDT_CORE_TRACE("Resources found at '{}'", m_radiant_resources_filepath);
		InitResources();
	}
	else {
		RDT_CORE_WARN("No resource filepath found!");
	}

	// Apply saved configurations from the Radiant profile
	ApplyConfig();

	Renderer::SetBackgroundColor({ 0.2f, 0.2f, 0.2f, 1.0f });

	m_panel_manager.RegisterPanel(MenuBar);
	m_panel_manager.RegisterPanel(DiagnosticsPanel);
	m_panel_manager.RegisterPanel(ScenePanel);
	m_panel_manager.RegisterPanel(GameWindowPanel);
	m_panel_manager.RegisterPanel(ConsolePanel);
	m_panel_manager.RegisterPanel(EntityHierarchyPanel);
	m_panel_manager.RegisterPanel(TemplateWizard);

	m_panel_manager.OpenPanel(MenuBar);
	m_panel_manager.OpenPanel(DiagnosticsPanel);
	m_panel_manager.OpenPanel(ScenePanel);
	m_panel_manager.OpenPanel(GameWindowPanel);
	m_panel_manager.OpenPanel(ConsolePanel);
	m_panel_manager.OpenPanel(EntityHierarchyPanel);
}

rdt::core::Editor::~Editor()
{
	Renderer::DetachGui(this);
}

void rdt::core::Editor::Initialize()
{
	Destroy();
	m_instance = new Editor;
}

void rdt::core::Editor::Destroy()
{
	if (m_instance != nullptr) {
		delete m_instance;
		m_instance = nullptr;
	}
}

void rdt::core::Editor::OnRender()
{
	if (!m_showEditor) {
		return;
	}

	m_panel_manager.RenderMDI();

	bool isFullscreen = Renderer::UsingDefaultViewport();
}

void rdt::core::Editor::SetSourcePath(const std::string& path)
{
	if (Utils::PathExists(path)) {
		m_project_source_directory = path;
	}
	else {
		RDT_CORE_WARN("EditorLayout - Source path does not exist: {}", path);
	}
}

void rdt::core::Editor::InitResources()
{
	fs::path fontFolder = fs::path(m_radiant_resources_filepath) / fs::path("fonts");
	std::string ttfFile;

	// Set the default font
	ttfFile = (fontFolder / fs::path("NunitoSans_7pt_Condensed-Medium.ttf")).generic_string();
	GuiManager::LoadFont(NunitoSans, ttfFile);
	GuiManager::SetDefaultFont(NunitoSans, 18);
	AddFont(NunitoSans, ttfFile, std::vector<unsigned int>{18, 24, 36});

	// Set the default font
	ttfFile = (fontFolder / fs::path("NunitoSans_7pt_Condensed-Bold.ttf")).generic_string();
	GuiManager::LoadFont(NunitoSans_Bold, ttfFile);
	AddFont(NunitoSans_Bold, ttfFile, std::vector<unsigned int>{18, 24, 36});

	// Load Icons from ForkAwesome
	ttfFile = (fontFolder / fs::path(FONT_ICON_FILE_NAME_FK)).generic_string();
	static const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_16_FK, 0 };

	GuiManager::LoadIcons(ForkAwesome, ttfFile, icons_ranges);
	m_fonts[ForkAwesome][36] = GuiManager::GetFont(ForkAwesome, 36);
	m_fonts[ForkAwesome][13] = GuiManager::GetFont(ForkAwesome, 13);
	m_fonts[ForkAwesome][18] = GuiManager::GetFont(ForkAwesome, 18);

	fs::path templateFolder = fs::path(m_radiant_resources_filepath) / fs::path("templates");
	if (!fs::is_directory(templateFolder) || !fs::exists(templateFolder)) {
		RDT_CORE_WARN("Could not find templates at '{}'", templateFolder.generic_string());
		return;
	}
	m_radiant_template_filepath = templateFolder.generic_string();
}

void rdt::core::Editor::ApplyConfig()
{
	std::string config_theme;
	if (m_config.GetAttribute("DevTools", "Theme", config_theme)) {
		int themeVal = std::stoi(config_theme);

		if (themeVal >= EditorTheme::Theme_Codz && themeVal <= EditorTheme::Nightingale) {
			SetTheme((EditorTheme)themeVal);
		}
		else {
			RDT_CORE_WARN("Could not find theme associated with id: {}", themeVal);
		}
	}
}

void rdt::core::Editor::AddFont(EditorFont name, std::string& ttfFile, const std::vector<unsigned int>& sizes)
{
	if (Utils::PathExists(ttfFile)) {

		if (!GuiManager::FontExists(name)) {
			GuiManager::LoadFont(name, ttfFile);
		}

		for (auto size : sizes) {
			m_fonts[name][size] = GuiManager::GetFont(name, size);
		}
	}
	else {
		RDT_CORE_WARN("Could not find file '{}'", ttfFile);
		return;
	}
}

void rdt::core::Editor::SetTheme(EditorTheme nTheme)
{
	ImGuiStyle& style = ImGui::GetStyle();
	switch (nTheme) {
	case Theme_Codz:
		// From: https://github.com/ocornut/imgui/issues/707
		style.WindowRounding = 5.3f;
		style.FrameRounding = 2.3f;
		style.ScrollbarRounding = 0;

		style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 0.95f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 1.0f, 1.0f, 0.65f);

		Log::SetLogColor(LogLevel::L_TRACE, WHITE);

		break;

	case Nightingale:
		style.WindowRounding = 5.3f;
		style.FrameRounding = 2.3f;
		style.ScrollbarRounding = 0;

		ImVec4 black = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
		ImVec4 white = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
		ImVec4 header(0.153f, 0.157f, 0.161f, 1.0f);
		ImVec4 base(0.38f, 0.404f, 0.478f, 0.95f);
		ImVec4 border(0.847f, 0.851f, 0.855f, 1.0f);

		style.Colors[ImGuiCol_Text] = white;
		style.Colors[ImGuiCol_TextDisabled] = white;
		style.Colors[ImGuiCol_WindowBg] = base;
		style.Colors[ImGuiCol_PopupBg] = base;
		style.Colors[ImGuiCol_Border] = border;
		style.Colors[ImGuiCol_BorderShadow] = border;
		style.Colors[ImGuiCol_FrameBg] = header;
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = header;
		style.Colors[ImGuiCol_TitleBgCollapsed] = header;
		style.Colors[ImGuiCol_TitleBgActive] = header;
		style.Colors[ImGuiCol_MenuBarBg] = header;
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Header] = header;
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 1.0f, 1.0f, 0.65f);

		Log::SetLogColor(LogLevel::L_TRACE, WHITE);
		break;
	}

	m_config.SetAttribute("DevTools", "Theme", nTheme);
}

void rdt::core::Editor::AddCenteredText(const std::string& text)
{
	ImVec2 size = ImGui::CalcTextSize(text.c_str());

	ImVec2 pos = { (ImGui::GetWindowSize().x / 2) - (size.x / 2), ImGui::GetCursorPosY() };
	ImGui::SetCursorPos(pos);
	ImGui::Text(text.c_str());
}

void rdt::core::Editor::InactiveButtonBegin()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 color;

	color = style.Colors[ImGuiCol_Button];
	color.w = 0.1f;
	ImGui::PushStyleColor(ImGuiCol_Button, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

	color = style.Colors[ImGuiCol_Text];
	color.w = 0.1f;
	ImGui::PushStyleColor(ImGuiCol_Text, color);
}

void rdt::core::Editor::InactiveButtonEnd()
{
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void rdt::core::Editor::InactiveTextBoxBegin()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 color;

	color = style.Colors[ImGuiCol_FrameBg];
	color.w = 0.3f;
	ImGui::PushStyleColor(ImGuiCol_FrameBg, color);
	ImGui::BeginDisabled();
}

void rdt::core::Editor::InactiveTextBoxEnd()
{
	ImGui::PopStyleColor();
	ImGui::EndDisabled();
}

int rdt::core::Editor::AddComboBox(const char* combo_label, const char** options, unsigned int optionCount, const char* preview)
{
	if (m_combo_selections.find(combo_label) == m_combo_selections.end()) {
		m_combo_selections[combo_label] = -1;
	}
	auto& selection_index = m_combo_selections.at(combo_label);

	const char* preview_label;
	if (preview == nullptr) {
		preview_label = options[0];
	}
	else {
		preview_label = selection_index < 0 ? preview : options[selection_index];
	}

	if (ImGui::BeginCombo(combo_label, preview_label))
	{
		for (unsigned int n = 0; n < optionCount; n++)
		{
			const bool is_selected = (selection_index == n);
			if (ImGui::Selectable(options[n], is_selected)) {
				selection_index = n;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	return selection_index;
}

int rdt::core::Editor::GetComboxBoxResult(const char* label)
{
	if (m_combo_selections.find(label) == m_combo_selections.end()) {
		m_combo_selections[label] = -1;
	}

	return m_combo_selections.at(label);
}

void rdt::core::Editor::AddIcon(const char* unicode, size_t size)
{
	ImGui::PushFont(m_fonts[ForkAwesome][size]);
	ImGui::Text(unicode);
	ImGui::PopFont();
}

const std::string& rdt::core::Editor::GetProjectSourcePath()
{
	return m_instance->m_project_source_directory;
}

const std::string& rdt::core::Editor::GetRadiantTemplatePath()
{
	return m_instance->m_radiant_template_filepath;
}

float rdt::core::Editor::GetButtonWidth(const char* label)
{
	ImGuiStyle& style = ImGui::GetStyle();
	return ImGui::CalcTextSize(label).x + (style.FramePadding.x * 2);
}

float rdt::core::Editor::GetButtonHeight(const char* label)
{
	ImGuiStyle& style = ImGui::GetStyle();
	return ImGui::CalcTextSize(label).y + (style.FramePadding.y * 2);
}

int rdt::core::Editor::MyTextCallback(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
	{
		bool* p_bool = (bool*)data->UserData;
		*p_bool = true;
	}

	return 0;
}

void rdt::core::Editor::ApplyGuiConfig(PanelConfig& config)
{
	if (config.update) {
		ImGui::SetNextWindowSize(ImVec2(config.width, config.height), ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImVec2(config.xPos, config.yPos), ImGuiCond_Always);
		config.update = false;
	}
	else {
		ImGui::SetNextWindowSize(ImVec2(config.width, config.height), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(config.xPos, config.yPos), ImGuiCond_FirstUseEver);
	}
}
bool* rdt::core::Editor::GetCheckboxSelection(const std::string& checkbox_label)
{
	if (m_checkbox_selections.find(checkbox_label) == m_checkbox_selections.end()) {
		m_checkbox_selections[checkbox_label] = false;
	}

	return &m_checkbox_selections.at(checkbox_label);
}

rdt::DebugComponent* rdt::core::Editor::GetDebugComponent(Entity entity)
{
	return EntityManager::GetComponent<DebugComponent>(entity);
}

rdt::glTextureID rdt::core::Editor::GetImGuiTextureData(rdt::TextureID tID, int& imageWidth, int& imageHeight)
{
	if (!TextureManager::TextureExists(tID)) {
		return 0;
	}

	auto& tex = TextureManager::GetTexture(tID);
	imageWidth = tex.GetImageWidth();
	imageHeight = tex.GetImageHeight();
	return tex.GetID();
}