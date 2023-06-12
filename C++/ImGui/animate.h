#pragma once

#include "imgui/imgui.h"
#include <math.h>

namespace ButtonAnimations
{
    inline void HoverAnimation(float animSpeed)
    {
        static float t = 0.0f;

        t += ImGui::GetIO().DeltaTime;

        ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 0.1f + (sinf(t * animSpeed) * 0.5f + 0.5f) * 0.9f);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
    }
}