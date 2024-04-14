#pragma once



namespace client
{
	class directx_hook : public singleton<directx_hook>, public events_handler<directx_hook>
	{
		public:
			directx_hook();
			~directx_hook();

			void init();
			void do_run(IDXGISwapChain* _swap_chain);
			void shutdown();

			bool is_running() const
			{
				return m_swap_chain != nullptr;
			}

			HWND get_window() const
			{
				return m_window;
			}

			ID3D11Device* get_device() const
			{
				return m_device;
			}

			ID3D11DeviceContext* get_device_context() const
			{
				return m_device_context;
			}

		private:
			typedef HRESULT(STDMETHODCALLTYPE* def_present)(IDXGISwapChain* _swap_chain, UINT _sync_interval, UINT _flags);

			static inline def_present original_present = nullptr;
			static HRESULT STDMETHODCALLTYPE new_present(IDXGISwapChain* _swap_chain, UINT _sync_interval, UINT _flags);

			void create_devices(IDXGISwapChain* _swap_chain);
			void remove_devices();

			HWND m_window;

			IDXGISwapChain* m_swap_chain;
			ID3D11Device* m_device;
			ID3D11DeviceContext* m_device_context;
	};
}