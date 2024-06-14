#include <pch.h>



namespace client
{
	log::log()
	{
		m_types =
		{
			{ log_info, "info" },
			{ log_success, "success" },
			{ log_warning, "warning" },
			{ log_error, "error" }
		};
	}



	log::~log()
	{
		m_types.clear();
	}



	void log::init()
	{
		HMODULE module = app::get()->get_module();

		m_module_path = utils::relative_path_from_module(module, __SOLUTION_NAME__ ".log");

		FILE* file;
		if (fopen_s(&file, m_module_path.c_str(), "w") == 0)
		{
			tm time = utils::get_system_time();

			fprintf_s(file, "[%02d:%02d:%02d] [init] Initializing '%s'...\n", time.tm_hour, time.tm_min, time.tm_sec, __SOLUTION_NAME__);

			fclose(file);
		}
	}



	void log::print(log_type _type, const char* _fmt, ...)
	{
		FILE* file;
		if (fopen_s(&file, m_module_path.c_str(), "a") == 0)
		{
			tm time = utils::get_system_time();

			static char buffer[UCHAR_MAX];

			va_list va_buffer;

			va_start(va_buffer, _fmt);
			_vsnprintf_s(buffer, sizeof(buffer), _fmt, va_buffer);
			va_end(va_buffer);

			fprintf_s(file, "[%02d:%02d:%02d] [%s] %s\n", time.tm_hour, time.tm_min, time.tm_sec, m_types.at(_type), buffer);

			fclose(file);
		}
	}


	void test()
	{}
}