#include <pch.h>



namespace client
{
	app::app()
	{
		m_module = nullptr;
		m_is_running = true;
	}



	app::~app()
	{

	}



	void app::mainthread(app* _self)
	{
		log::get()->init();

		minhook::init();

		imgui::get()->pre_init();

		directx_hook::get()->register_event("on_create_devices", [](directx_hook* _self)
		{
			imgui::get()->post_init(_self);

			inputs_hook::get()->init(_self->get_window());
		});

		directx_hook::get()->register_event("on_render", [](LPVOID _unused)
		{
			imgui::get()->render();
		});

		directx_hook::get()->register_event("on_remove_devices", [](LPVOID _unused)
		{
			imgui::get()->shutdown();

			inputs_hook::get()->shutdown();
		});

		directx_hook::get()->init();

		log::get()->print(log_success, "Successfully loaded 'imgui_dx11_base'...");

		while (_self->m_is_running)
		{
			if (GetAsyncKeyState(VK_INSERT))
			{
				break;
			}

			Sleep(0);
		}

		log::get()->print(log_info, "Unloading 'imgui_dx11_base'...");

		FreeLibraryAndExitThread(_self->m_module, EXIT_SUCCESS);
	}



	void app::initialize(HMODULE _module)
	{
		m_module = _module;

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mainthread, this, NULL, NULL);
	}



	void app::shutdown()
	{
		m_is_running = false;

		minhook::shutdown();

		directx_hook::get()->shutdown();

		inputs_hook::get()->shutdown();

		imgui::get()->shutdown(true);
	}
}