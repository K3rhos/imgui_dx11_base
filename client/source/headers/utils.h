#pragma once



namespace client
{
	struct utils
	{
		static tm get_system_time()
		{
			tm time_struct;
			time_t current_time = time(NULL);

			localtime_s(&time_struct, &current_time);

			return time_struct;
		}

		static const std::string relative_path_from_module(HMODULE _module, const std::string& _path)
		{
			static char buffer[MAX_PATH];

			GetModuleFileNameA(_module, buffer, MAX_PATH);

			std::string module_path(buffer);

			size_t last_index = module_path.find_last_of("/\\");

			return module_path.substr(0, last_index) + "\\" + _path;
		}

		template<class I>
		static void safe_release(I** _interface)
		{
			if (*_interface == nullptr)
				return;

			(*_interface)->Release();
			(*_interface) = nullptr;
		}
	};
}