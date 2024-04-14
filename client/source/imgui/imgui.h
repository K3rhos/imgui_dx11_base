#pragma once



namespace client
{
	class windows
	{
		public:
			void add_window(window* _window)
			{
				m_windows.emplace_back(_window);
			}

		protected:
			std::vector<window*> m_windows;
	};



	class imgui : public singleton<imgui>, public windows
	{
		public:
			imgui();
			~imgui();

			void pre_init();
			void post_init(directx_hook* _dx);

			void render();
			void shutdown(bool _destroy_context = false);

			bool is_any_window_open();

		private:
			ImGuiIO* m_io;
			ImGuiStyle* m_style;
			ImGuiContext* m_context;
	};
}