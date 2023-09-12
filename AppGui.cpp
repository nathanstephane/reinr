#include "AppGui.h"

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
	ImGui::Text("ActionsDisplay");

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
