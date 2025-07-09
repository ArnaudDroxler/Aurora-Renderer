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

            windowMode = window->GetWindowMode();
			vSync = window->GetVSync();

            resolutions = window->GetResolutions();

            resolutionDisplay = new char* [resolutions.size()];

            for (int i = 0; i < resolutions.size(); ++i)
            {

                auto resolution = resolutions[i];
                std::string str = std::to_string(resolution.first) + " x " + std::to_string(resolution.second);

                resolutionDisplay[i] = new char[str.size() + 1];
                std::copy(str.begin(), str.end(), resolutionDisplay[i]);
                resolutionDisplay[i][str.size()] = '\0';

            }

            currentResolutionIndex = -1;
        }

        void OnEvent(Event& event)
        {

        }

        void OnImGUIRender()
        {

            ImGui::ShowMetricsWindow();
            ImGui::ShowDemoWindow();

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

                auto currentDisplayMode = window->GetCurrentDisplayMode();
             
                if (currentResolutionIndex == -1 || displayMode.width != currentDisplayMode.width || displayMode.height != currentDisplayMode.height)
                {
                    displayMode = currentDisplayMode;

					Debug::CoreCritical("SettingsLayer::OnImGUIRender - Updating resolutions and refresh rates");
					Debug::CoreCritical("SettingsLayer::OnImGUIRender - Current Display Mode: {0}", displayMode.ToString());
                    
					Debug::CoreCritical("Resolution size : {0}", resolutions.size());

					currentResolutionIndex = -1;
                    for (int i = 0; i < resolutions.size(); ++i)
                    {
                        auto resolution = resolutions[i];
                        if (resolution.first == displayMode.width && resolution.second == displayMode.height) {
                            currentResolutionIndex = i;
                        }
                    }

					Debug::CoreCritical("Current Resolution Index: {0}", currentResolutionIndex);

                    if (currentResolutionIndex == -1)
                    {
                        for (int i = 0; i < resolutions.size(); ++i)
                        {
                            auto resolution = resolutions[i];
                            if (resolution.first == displayMode.width) {
                                currentResolutionIndex = i;
                            }
                        }
                    }
                    
                    Debug::CoreCritical("Current Resolution Index: {0}", currentResolutionIndex);

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

					
                }

                ImGui::Text("Display Mode:");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);
                if (ImGui::Combo("##WindowMode", &currentModeIndex, modeLabels, IM_ARRAYSIZE(modeLabels))) {
                    WindowMode newMode = static_cast<WindowMode>(currentModeIndex);
                    window->SetWindowMode(newMode);
                    windowMode = newMode;
                }

                ImGui::Spacing();

                ImGui::Text("Resolution :");
                ImGui::SameLine();
                ImGui::SetNextItemWidth(200);

                int previousResolutionIndex = currentResolutionIndex;
                ImGui::Combo("##Resolution", &currentResolutionIndex, resolutionDisplay, resolutions.size());
                if (currentResolutionIndex != previousResolutionIndex)
                {
                    DisplayMode displayMode = {};
                    displayMode.width = resolutions[currentResolutionIndex].first;
                    displayMode.height = resolutions[currentResolutionIndex].second;
                    displayMode.refreshRateNumerator = this->displayMode.refreshRateNumerator;
                    displayMode.refreshRateDenominator = this->displayMode.refreshRateDenominator;

					Debug::CoreCritical("SettingsLayer::OnImGUIRender - Setting resolution: {0}", displayMode.ToString());
				    window->SetResolutionAndRefreshRate(displayMode);
				    
                }
                ImGui::Spacing();

                if (windowMode == WindowMode::BorderlessFullscreen /* || windowMode == WindowMode::ExclusiveFullscreen*/)
                {
                    if (ImGui::Checkbox("Vsync", &vSync)) {
                        window->SetVSync(vSync);
                    }

                    /*if (vSync && windowMode == WindowMode::ExclusiveFullscreen)
                    {
          
                        ImGui::Text("Refresh rate:");
                        ImGui::SameLine();
                        ImGui::SetNextItemWidth(200);

                        int previousRefreshRateIndex = currentRefreshRateIndex;
                        ImGui::Combo("##Refreshrate", &currentRefreshRateIndex, refreshRatesDisplay, refreshRates.size());
                        if (currentRefreshRateIndex != previousRefreshRateIndex)
                        {
                            DisplayMode displayMode = {};
                            displayMode.width = this->displayMode.width;
                            displayMode.height = this->displayMode.height;
                            displayMode.refreshRateNumerator = refreshRates[currentRefreshRateIndex].first;
                            displayMode.refreshRateDenominator = refreshRates[currentRefreshRateIndex].second;

                            Debug::CoreCritical("SettingsLayer::OnImGUIRender - Setting Refreshrate: {0}", displayMode.ToString());
                            window->SetResolutionAndRefreshRate(displayMode);
                        }

                    }*/
                    ImGui::Spacing();
                }


                ImGui::Spacing();

                Application& app = Application::Get();

                int framerate = app.GetTargetFrameRate();
                bool userFramerateLimiter = app.GetUseTargetFrameRate();

                if (ImGui::Checkbox("Use framerate limiter", &userFramerateLimiter)) {
                    app.SetUseTargetFrameRate(userFramerateLimiter);

                }

                if (userFramerateLimiter)
                {
                    ImGui::Text("Target framerate :");
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(200);

                    ImGui::InputInt(" ", &framerate);
                    app.SetTargetFrameRate(framerate);
                }
                
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
		bool vSync;

		DisplayMode displayMode;

        WindowMode windowMode;

		Window* window;

        int currentModeIndex = 0;
        int currentResolutionIndex = 0;
        int currentRefreshRateIndex = 0;

        std::vector<std::pair<unsigned int, unsigned int>> resolutions;
        std::vector<std::pair<unsigned int, unsigned int>> refreshRates;

		char** resolutionDisplay = nullptr;
		char** refreshRatesDisplay = nullptr;

        const char* modeLabels[3] = { "Windowed", "Borderless" };

        bool settingsChanged = false;

    };

}