#include "pch.h"
#include "ConsoleTab.h"
#include "ImGui/imguiIncludes.h"

void ConsoleTab::Update()
{
	if (myTabOpen)
	{
		ImGui::Begin("Console", &myTabOpen, ImGuiWindowFlags_NoCollapse);

		ImGui::End();
	}
}

void ConsoleTab::OpenTab()
{
	myTabOpen = true;
}