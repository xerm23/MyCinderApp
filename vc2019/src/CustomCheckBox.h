
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui/imgui.h"
#ifndef IMGUI_DISABLE

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui/imgui_internal.h"

// System includes
#include <ctype.h>      // toupper
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

namespace ImGui {
	IMGUI_API bool          MyCheckbox(const char* label, bool* v);
}

bool ImGui::MyCheckbox(const char* label, bool* v) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = ImGuiStyle::ImGuiStyle();
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImVec2 pading = ImVec2(2, 2);
	const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.x * 6, label_size.y + style.FramePadding.y / 2));
	ItemSize(check_bb, style.FramePadding.y);
	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
	if (label_size.x > 0)
	{
		ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
		total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
	}
	if (!ItemAdd(total_bb, id))
		return false;
	bool hovered, held;
	bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
		*v = !(*v);
	const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
	const float check_sz2 = check_sz / 2;
	const float pad = ImMax(1.0f, (float)(int)(check_sz / 4.f));

	//draw red background if not pressed
	for (int i = 6; i >= -6; i--) window->DrawList->AddCircleFilled(ImVec2(check_bb.Min.x + (check_bb.Max.x - check_bb.Min.x) / 2 + i, check_bb.Min.y + 9), 7, GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)), 12);

	//draw circle
	window->DrawList->AddCircleFilled(ImVec2(check_bb.Min.x + (check_bb.Max.x - check_bb.Min.x) / 2 - 6, check_bb.Min.y + 9), 7, GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 12);

	if (*v) {
		//draw green background if pressed
		for (int i = 6; i >= -6; i--) window->DrawList->AddCircleFilled(ImVec2(check_bb.Min.x + (check_bb.Max.x - check_bb.Min.x) / 2 + i, check_bb.Min.y + 9), 7, GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f)), 12);
		//draw circle
		window->DrawList->AddCircleFilled(ImVec2(check_bb.Min.x + (check_bb.Max.x - check_bb.Min.x) / 2 + 6, check_bb.Min.y + 9), 7, GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 12);
	}
	if (label_size.x > 0.0f)
		RenderText(text_bb.GetTL(), label);
	return pressed;
}
#endif // #ifndef IMGUI_DISABLE
