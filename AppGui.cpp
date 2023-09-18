#include "AppGui.h"

namespace imgui = ImGui;

void AppGui::DisplayMenu()
{

	
	if (ImGui::Button("Previous"))
	{
		if (currentPath.has_parent_path())
		{
			std::cout << "Relative Path: " << currentPath.relative_path();
			std::cout << "Parent Path: " << currentPath.parent_path();

			currentPath = currentPath.parent_path();
		}

	} 
	ImGui::SameLine();
	ImGui::Text("You are here: %s", currentPath.string().c_str());


}

void AppGui::DisplayContent()
{
	for (const auto& item : std::filesystem::directory_iterator(currentPath))
	{
		const auto is_selected = item.path() == selectedItem;
		const auto is_dir = item.is_directory();
		const auto is_file = item.is_regular_file();
		std::string name = item.path().filename().string();
		
		

		if (is_dir)
		{
			name = "Dir: " + name;
		}
		else if (is_file) 
		{
			name = "File: " + name;
		}
		
		//ImGui::Text("%s", name.c_str());
		//ImGuiSelectable allows to select an item. returs true when selected.
		if (ImGui::Selectable(name.c_str(), is_selected))
		{
			if (is_dir)
			{
				currentPath /= item.path().filename();
			}
			selectedItem = item.path();
		}
	}

}

void AppGui::DisplayActions()
{
	if (std::filesystem::is_directory(selectedItem))
	{
		ImGui::Text("selected directory: %s", selectedItem.string().c_str());
	}else if (std::filesystem::is_regular_file(selectedItem))
	{
		ImGui::Text("selected file: %s", selectedItem.string().c_str());

	}
	else
	{
		ImGui::Text("Nothing selected");
	}

	if (imgui::Button("Open") && std::filesystem::is_regular_file(selectedItem))
	{
		openFile();
	}

	imgui::SameLine();
	if (imgui::Button("Rename"))
	{
		should_renameModal_open = true;
		ImGui::OpenPopup("Rename File");
		
	}

	imgui::SameLine();
	if (imgui::Button("Delete"))
	{
		should_deleteModal_open = true;
		imgui::OpenPopup("Delete File");
	}

	renameFilePopup();
	deleteFilePopup();

	

}

void AppGui::openFile()
{
	#ifdef _WIN32
		const auto cmd = "start \"\" \"" + selectedItem.string() + "\"";
	#elif __APPLE__
		const auto cmd = "open \"\" \"" + selectedItem.string() + "\"";
	#else
		const auto cmd = "xdg-open \"\" \"" + selectedItem.string() + "\"";
	#endif
		std::system(cmd.c_str());
}

void AppGui::renameFilePopup()
{
	

	if (imgui::BeginPopupModal("Rename File", &should_renameModal_open))
	{
		static char name_[250] = { '\0' };
		imgui::Text("New file name: ");
		imgui::InputText("###", name_, sizeof(name_));

		if (ImGui::Button("Rename"))
		{
			auto new_filename_path = selectedItem.parent_path() / name_;
			if(renameFile(selectedItem, new_filename_path))
			{
				should_renameModal_open = false;
				selectedItem = new_filename_path;
				std::memset(name_, 0, sizeof(name_));
				
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			should_renameModal_open = false;
		}

		ImGui::EndPopup();
	}
}



void AppGui::deleteFilePopup()
{
	if (imgui::BeginPopupModal("Delete File", &should_deleteModal_open))
	{
		ImGui::Text("Are you sure you want to delete %s ?", selectedItem.filename().string().c_str());

		if (ImGui::Button("Yes"))
		{
			if (deleteFile(selectedItem))
			{
				selectedItem.clear();
				should_deleteModal_open = false;
			}
		}
		ImGui::SameLine();

		if (ImGui::Button("No"))
		{
			should_deleteModal_open = false;
		}
		ImGui::EndPopup();

	}
}

bool AppGui::deleteFile(const std::filesystem::path& path)
{
	try
	{
		std::filesystem::remove(path);
		return true;
	}
	catch (const std::exception& e)
	{
		bool open_errorPopup = true;
		imgui::OpenPopup("Error Popup");
		imgui::BeginPopupModal("Error Popup",&open_errorPopup);
		imgui::Text("Error occured: %s", e.what());
		if (imgui::Button("close"))
		{
			open_errorPopup = false;
		}
		imgui::EndPopup();

		return false;
	}
}

bool AppGui::renameFile(const std::filesystem::path& oldpath, const std::filesystem::path& newpath)
{
	try
	{
		std::filesystem::rename(oldpath, newpath);
		return true;
	}
	catch (const std::exception& e)
	{
		imgui::OpenPopup("Error Popup");
		imgui::BeginPopupModal("Error Popup");
		imgui::Text("Error occured: %s", e.what());
		imgui::EndPopup();

		return false;
	}


}



void AppGui::DisplayFilter()
{
	static char filter[10] = { "\0" };
	ImGui::Text("Filter by file type");
	ImGui::SameLine();
	ImGui::InputText("###extension", filter,sizeof(filter));

	if (std::strlen(filter)==0)
	{
		return;
	}

	auto counter = std::size_t{ 0 };
	for (const auto& item : std::filesystem::directory_iterator(currentPath))
	{
		//iterate over each file to see the ones matching the filter extension
		//discard directory
		if (!std::filesystem::is_regular_file(item))
		{
			continue;
		}

		if (item.path().extension().string() == filter)
		{
			++counter;
		}

		
	}

	ImGui::Text("Number of %s files: %d", filter, counter);
}
