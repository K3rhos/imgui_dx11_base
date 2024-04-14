#pragma once



namespace client
{
	class app : public singleton<app>
	{
		public:
			app();
			~app();

			void initialize(HMODULE _module);
			void shutdown();

			HMODULE get_module() const
			{
				return m_module;
			}

		private:
			static void mainthread(app* _self);

			HMODULE m_module;
			bool m_is_running;
	};
}