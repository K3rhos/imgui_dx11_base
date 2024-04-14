#pragma once



template<typename T>
class singleton
{
	public:
		singleton() = default;

		singleton(singleton& _other) = delete;
		void operator=(const singleton&) = delete;
		void* operator new(size_t _size) = delete;
		void operator delete(void* _ptr) = delete;

		static T* get()
		{
			static T singleton;

			return &singleton;
		}
};