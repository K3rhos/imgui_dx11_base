#pragma once



namespace client
{
	enum log_type
	{
		log_info,
		log_success,
		log_warning,
		log_error
	};



	class log : public singleton<log>
	{
		public:
			log();
			~log();

			void init();
			void print(log_type _type, const char* _fmt, ...);

		private:
			std::string m_module_path;

			std::map<log_type, const char*> m_types;
	};
}