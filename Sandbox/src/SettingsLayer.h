#pragma once

#include "AuroraRenderer.h"
#include "Aurora/Core/Layer.h"
#include "Platform/Windows/WindowsWindow.h"

#include "imgui.h"

namespace Aurora
{
    class SettingsLayer : public Layer
    {
    public:

        SettingsLayer() : Layer("Settings")
        {
            Application& app = Application::Get();

            window = app.GetWindowPtr();

            WindowsWindow* windowsWindow = dynamic_cast<WindowsWindow*>(app.GetWindowPtr());
            windowsWindow->GetResolutions();

            auto displayMode = window->GetCurrentDisplayMode();

            resolutions = window->GetResolutions();

            resolutionDisplay = new char* [resolutions.size()];

            for (int i = 0; i < resolutions.size(); ++i)
            {

                auto resolution = resolutions[i];
                std::string str = std::to_string(resolution.first) + " x " + std::to_string(resolution.second);

                resolutionDisplay[i] = new char[str.size() + 1];
                std::copy(str.begin(), str.end(), resolutionDisplay[i]);
                resolutionDisplay[i][str.size()] = '\0';

                if (resolution.first == displayMode.width && resolution.second == displayMode.height) {
                    currentResolutionIndex = i;
                }

            }
            
            fullscreen = window->GetFullScreen();
			vSync = window->IsVSync();
        }

        void OnEvent(Event& event)
        {

        }

        void OnImGUIRender()
        {

            ImGui::ShowMetricsWindow();

            if (!open) {
                return;
            }

            ImGuiViewport* viewport = ImGui::GetMainViewport();

            float panel_width = 400.0f;

            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(ImVec2(panel_width, viewport->Size.y));

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

            if (!ImGui::Begin("Settings", &open, window_flags))
            {
                ImGui::End();
				return;
            }

            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Menu"))
                {
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }


            if (ImGui::CollapsingHeader("Help"))
            {
                ImGui::SeparatorText("About Aurora Render:");
                ImGui::BulletText("F1 : Show/Hide Settings window");
                ImGui::BulletText("F2 : Show/Hide Overlay window");

                //ImGui::SeparatorText("USER GUIDE:");
                //ImGui::ShowUserGuide();
            }


            if (ImGui::CollapsingHeader("Window"))
            {

                auto displayMode = window->GetCurrentDisplayMode();

                refreshRates = window->GetRefreshRatesForResolution(displayMode.width, displayMode.height);

                refreshRatesDisplay = new char* [refreshRates.size()];

                for (int i = 0; i < refreshRates.size(); ++i)
                {

                    auto refreshRate = refreshRates[i];
                    std::string str = std::to_string((float)refreshRate.first / refreshRate.second) + " Hz";

                    refreshRatesDisplay[i] = new char[str.size() + 1];
                    std::copy(str.begin(), str.end(), refreshRatesDisplay[i]);
                    refreshRatesDisplay[i][str.size()] = '\0';


                    if (refreshRate.first == displayMode.refreshRateNumerator && refreshRate.second == displayMode.refreshRateDenominator) {
                        currentRefreshRateIndex = i;
                    }

                }


				int previousResolutionIndex = currentResolutionIndex;

                ImGui::Text("Resolution :");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);

                ImGui::Combo("##Resolution", &currentResolutionIndex, resolutionDisplay, resolutions.size());
                if (currentResolutionIndex != previousResolutionIndex)
                {
					window->SetResolutionAndRefreshRate({ resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second, refreshRates[currentRefreshRateIndex].first, refreshRates[currentRefreshRateIndex].second});
                }


                ImGui::Spacing();

                if (ImGui::Checkbox("Vsync", &vSync)) {
                    window->SetVSync(vSync);
                }

                if (vSync)
                {
                    int previousRefreshRateIndex = currentRefreshRateIndex;
                    ImGui::Text("Refresh rate :");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(200);
                    ImGui::Combo("##Refreshrate", &currentRefreshRateIndex, refreshRatesDisplay, refreshRates.size());
                    if (currentRefreshRateIndex != previousRefreshRateIndex)
                    {
                        window->SetResolutionAndRefreshRate({ resolutions[currentResolutionIndex].first, resolutions[currentResolutionIndex].second, refreshRates[currentRefreshRateIndex].first, refreshRates[currentRefreshRateIndex].second });
                    }

                }
              


                ImGui::Text("Display Mode:");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);
                if (ImGui::Combo("##WindowMode", &currentModeIndex, modeLabels, IM_ARRAYSIZE(modeLabels))) {
                    WindowMode newMode = static_cast<WindowMode>(currentModeIndex);
                    dynamic_cast<WindowsWindow*>(window)->SetWindowMode(newMode);
                }

                ImGui::Spacing();

                if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
                    window->SetFullScreen(fullscreen);
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
            }
            ImGui::End();

        }

        void OnUpdate()
        {
            if (Input::GetKeyDown(KeyCode::F1))
            {
                open = !open;
            }           
        }

    private:
        bool open = true;
		bool vSync = true;

        bool fullscreen = false;

		Window* window;

        int currentModeIndex = 0;
        int currentResolutionIndex = 0;
        int currentRefreshRateIndex = 0;

        std::vector<std::pair<unsigned int, unsigned int>> resolutions;
        std::vector<std::pair<unsigned int, unsigned int>> refreshRates;

		char** resolutionDisplay = nullptr;
		char** refreshRatesDisplay = nullptr;

        const char* modeLabels[3] = { "Windowed", "Borderless", "Exclusive" };

        bool settingsChanged = false;

    };

}