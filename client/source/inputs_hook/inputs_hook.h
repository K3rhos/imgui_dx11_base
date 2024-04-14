#pragma once



extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND _window, UINT _message, WPARAM _word_param, LPARAM _long_param);



namespace client
{
	class inputs_hook : public singleton<inputs_hook>
	{
		public:
			inputs_hook();
			~inputs_hook();

			void init(HWND _window);
			void shutdown();

		private:
			typedef BOOL(WINAPI* def_set_cursor_pos)(int _x, int _y);

			HWND m_window;

			static inline WNDPROC m_original_window_procedure = nullptr;
			static LRESULT WINAPI new_window_procedure(HWND _window, UINT _message, WPARAM _word_param, LPARAM _long_param);

			static inline def_set_cursor_pos m_original_set_cursor_pos = nullptr;
			static BOOL WINAPI new_set_cursor_pos(int _x, int _y);
	};
}