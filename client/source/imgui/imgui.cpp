#include <pch.h>



namespace client
{
	imgui::imgui()
	{

	}



	imgui::~imgui()
	{

	}



	void imgui::pre_init()
	{
		m_context = ImGui::CreateContext();

		m_io = &ImGui::GetIO();
		m_io->IniFilename = NULL;
		m_io->ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		// TODO: Add a custom font (instead of default one).
		m_io->Fonts->AddFontDefault();

		m_style = &ImGui::GetStyle();

		// TODO: Manage imgui default style here...

		static window win;
		win.init();

		add_window(&win);

		// TODO: Load config for first time...

		log::get()->print(log_success, "Pre-initialized ImGui");
	}



	void imgui::post_init(directx_hook* _dx)
	{
		ImGui_ImplWin32_Init(_dx->get_window());
		ImGui_ImplDX11_Init(_dx->get_device(), _dx->get_device_context());

		log::get()->print(log_success, "Post-initialized ImGui");
	}



	void imgui::render()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		for (const auto& window : m_windows)
		{
			window->render();
		}

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}



	void imgui::shutdown(bool _destroy_context)
	{
		log::get()->print(log_success, "Shutting down ImGui...");

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		if (_destroy_context)
			ImGui::DestroyContext();
	}



	bool imgui::is_any_window_open()
	{
		for (const auto& window : m_windows)
		{
			if (window->is_open())
			{
				return true;
			}
		}

		return false;
	}
}