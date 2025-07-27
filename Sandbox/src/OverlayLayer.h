#pragma once

#include "Aurora/Core/Layer.h"
#include "Aurora/Core/Time.h"

#include "imgui.h"

namespace Aurora
{

    class OverlayLayer : public Layer
    {
    public:
        OverlayLayer() : Layer("Overlay")
        {

        }

        void OnEvent(Event& event)
        {

        }

        void OnImGUIRender()
        {

            if (!open) {
                return;
            }

            static int location = 1;
            ImGuiIO& io = ImGui::GetIO();
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            if (location >= 0)
            {
                const float PAD = 10.0f;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImVec2 work_pos = viewport->WorkPos; 
                ImVec2 work_size = viewport->WorkSize;
                ImVec2 window_pos, window_pos_pivot;
                window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
                window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
                window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
                window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                window_flags |= ImGuiWindowFlags_NoMove;
            }
            else if (location == -2)
            {
                ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                window_flags |= ImGuiWindowFlags_NoMove;
            }
            ImGui::SetNextWindowBgAlpha(0.35f); 

            if (!ImGui::Begin("Overlay", &open, window_flags))
            {
                ImGui::End();
                return;
            }

            ImGui::Text("Framerate : %.1f FPS", Time::FPS());

            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("Mouse Position: <invalid>");


            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
                if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
                if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
                if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
                if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
                if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                if (open && ImGui::MenuItem("Close")) open = false;
                ImGui::EndPopup();
            }
         
            ImGui::End();

        }

        void OnUpdate()
        {
            if (Input::GetKeyDown(KeyCode::F2))
            {
				open = !open;
            }
        }

    private:
        bool open;
    };

}