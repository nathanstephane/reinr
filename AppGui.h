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

	bool should_renameModal_open = false;
	bool should_deleteModal_open = false;
	
public:
	void DisplayMenu();
	void DisplayContent();
	void DisplayActions();
	void DisplayFilter();

	void openFile();
	void renameFilePopup();
	void deleteFilePopup();
	bool deleteFile(const std::filesystem::path& path);
	bool renameFile(const std::filesystem::path& oldpath, const std::filesystem::path& newpath);
};

