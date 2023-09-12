#pragma once

#include <string_view>
#include <filesystem>
#include "imgui.h"
#include <iostream>


class AppGui
{
public:
	AppGui() :currentPath(std::filesystem::current_path()), selectedItem(std::filesystem::path{}) {}

private:
	//std::string currentPath;
	std::filesystem::path currentPath;
	std::filesystem::path selectedItem;
	
public:
	void DisplayMenu();
	void DisplayContent();
	void DisplayActions();
	void DisplayFilter();
};

