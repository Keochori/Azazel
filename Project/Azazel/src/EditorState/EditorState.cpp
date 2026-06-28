#include "pch.h"
#include "EditorState.h"
#include "json/nlohmann/json.hpp"
#include <fstream>

using json = nlohmann::json;

EditorState& EditorState::GetInstance()
{
	static EditorState ourInstance;
	return ourInstance;
}

void EditorState::LoadState()
{
	if (!std::filesystem::exists(myJsonPath))
	{
		json j;
		// Expanded Folders
		j["expandedFolders"] = json::array();
		// Window Maximized
		j["windowMaximized"] = false;
		// Window Size
		j["windowSize"]["width"] = 0;
		j["windowSize"]["height"] = 0;

		std::ofstream output(myJsonPath);
		output << j.dump(4);
	}
	else
	{
		std::ifstream input(myJsonPath);

		json j;
		input >> j;

		// Expanded Folders
		myExpandedFolders = j["expandedFolders"].get<std::unordered_set<AssetGUID>>();
		// Window Maximized
		myWindowMaximized = j["windowMaximized"].get<bool>();
		// Window Size
		myWindowWidth = j["windowSize"]["width"].get<UINT>();
		myWindowHeight = j["windowSize"]["height"].get<UINT>();
	}
}

void EditorState::SaveState()
{
	json j;

	std::ifstream input(myJsonPath);
	input >> j;

	// Expanded Folders
	if (!myExpandedFolders.empty())
		j["expandedFolders"] = myExpandedFolders;
	// Window Maximized
	j["windowMaximized"] = myWindowMaximized;
	// Non Maximized Window Size
	if (myWindowWidth != 0 && myWindowHeight != 0)
	{
		j["windowSize"]["width"] = myWindowWidth;
		j["windowSize"]["height"] = myWindowHeight;
	}

	std::ofstream output(myJsonPath);
	output << j.dump(4);
}

void EditorState::SetExpandedFolders(const std::unordered_set<AssetGUID>& aExpandedFolders)
{
	myExpandedFolders = aExpandedFolders;
}

void EditorState::SetWindowMaximized(bool aMaximized)
{
	myWindowMaximized = aMaximized;
}

void EditorState::SetWindowSize(UINT aWindowWidth, UINT aWindowHeight)
{
	myWindowWidth = aWindowWidth;
	myWindowHeight = aWindowHeight;
}

bool EditorState::GetWindowMaximized()
{
	return myWindowMaximized;
}

UINT EditorState::GetWindowWidth()
{
	return myWindowWidth;
}

UINT EditorState::GetWindowHeight()
{
	return myWindowHeight;
}

std::unordered_set<AssetGUID> EditorState::GetExpandedFolders()
{
	return myExpandedFolders;
}
