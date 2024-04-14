#include <pch.h>



namespace client
{
	inputs_hook::inputs_hook()
	{
		m_window = nullptr;
	}



	inputs_hook::~inputs_hook()
	{
		m_window = nullptr;
	}



	void inputs_hook::init(HWND _window)
	{
		m_window = _window;

		m_original_window_procedure = (WNDPROC)SetWindowLongPtr(m_window, GWLP_WNDPROC, (LONG_PTR)new_window_procedure);

		minhook::add(SetCursorPos, &new_set_cursor_pos, &m_original_set_cursor_pos);
	}



	void inputs_hook::shutdown()
	{
		SetWindowLongPtr(m_window, GWLP_WNDPROC, (LONG_PTR)m_original_window_procedure);

		minhook::remove(SetCursorPos);

		log::get()->print(log_success, "Unitialized inputs hook");
	}



	LRESULT WINAPI inputs_hook::new_window_procedure(HWND _window, UINT _message, WPARAM _word_param, LPARAM _long_param)
	{
		if (directx_hook::get()->is_running())
		{
			ImGui_ImplWin32_WndProcHandler(_window, _message, _word_param, _long_param);

			if (imgui::get()->is_any_window_open())
			{
				switch (_message)
				{
					// Keyboard buttons.
					case WM_SYSKEYDOWN:
					case WM_KEYDOWN:

					// Mouse buttons.
					case WM_LBUTTONDBLCLK:
					case WM_LBUTTONDOWN:
					case WM_MBUTTONDBLCLK:
					case WM_MBUTTONDOWN:
					case WM_RBUTTONDBLCLK:
					case WM_RBUTTONDOWN:
					case WM_MOUSEWHEEL:

					// Mouse movements.
					case WM_MOUSEMOVE:
					case WM_INPUT:
					{
						return false;
					}
					break;
				};
			}
		}

		return CallWindowProc(m_original_window_procedure, _window, _message, _word_param, _long_param);
	}



	BOOL WINAPI inputs_hook::new_set_cursor_pos(int _x, int _y)
	{
		if (directx_hook::get()->is_running() && imgui::get()->is_any_window_open())
		{
			return FALSE;
		}

		return m_original_set_cursor_pos(_x, _y);
	}
}