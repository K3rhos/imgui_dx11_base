#pragma once



struct minhook
{
	static void init()
	{
		MH_Initialize();
	}

	template<typename T>
	static bool add(void* _target, T _new_function)
	{
		MH_STATUS status = MH_CreateHook(_target, _new_function, NULL);

		if (status != MH_OK)
			return false;

		status = MH_EnableHook(_target);

		if (status != MH_OK)
			return false;

		return true;
	}

	template<typename T1, typename T2>
	static bool add(void* _target, T1 _new_function, T2** _original_function)
	{
		MH_STATUS status = MH_CreateHook(_target, _new_function, (void**)_original_function);

		if (status != MH_OK)
			return false;

		status = MH_EnableHook(_target);

		if (status != MH_OK)
			return false;

		return true;
	}

	static bool remove(void* _target)
	{
		MH_STATUS status = MH_DisableHook(_target);

		if (status != MH_OK)
			return false;

		status = MH_RemoveHook(_target);

		if (status != MH_OK)
			return false;

		return true;
	}

	static void shutdown()
	{
		MH_DisableHook(MH_ALL_HOOKS);
		MH_RemoveHook(MH_ALL_HOOKS);
		MH_Uninitialize();

		client::log::get()->print(client::log_success, "Unitialized minhook");
	}
};