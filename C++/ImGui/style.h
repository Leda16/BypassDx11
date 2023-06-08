#pragma once

#include "imgui/imgui.h"

namespace MyStyles
{
    inline void ButtonStyle()
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); 
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); 

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f); 
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    }

    inline void InputTextStyle()
    {
        ImVec4 bgColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImVec4 borderColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 textActiveColor = ImVec4(2.95f, 0.95f, 0.95f, 0.2f);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, borderColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, borderColor);
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, textActiveColor);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
    }



    inline void PopStyleVars(int count)
    {

        for (int i = 0; i < count; i++) {
            ImGui::PopStyleVar();
        }
    }

    inline void PopStyleColor(int count)
    {
        for (int i = 0; i < count; i++) {
            ImGui::PopStyleColor();
        }
    }
}