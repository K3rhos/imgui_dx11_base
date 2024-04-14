#include <pch.h>



namespace client
{
	directx_hook::directx_hook()
	{
		m_window = nullptr;

		m_swap_chain = nullptr;
		m_device = nullptr;
		m_device_context = nullptr;
	}



	directx_hook::~directx_hook()
	{
		m_window = nullptr;

		utils::safe_release(&m_swap_chain);
		utils::safe_release(&m_device);
		utils::safe_release(&m_device_context);
	}



	void directx_hook::init()
	{
		WNDCLASSEX window_class{};
		window_class.cbSize = sizeof(WNDCLASSEX);
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = DefWindowProc;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = GetModuleHandle(NULL);
		window_class.hIcon = NULL;
		window_class.hCursor = NULL;
		window_class.hbrBackground = NULL;
		window_class.lpszMenuName = NULL;
		window_class.lpszClassName = L"dx11_hook";
		window_class.hIconSm = NULL;
		
		RegisterClassEx(&window_class);

		HWND window = CreateWindow(window_class.lpszClassName, L"dx11_hook", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL);

		HMODULE d3d11_module;
		if ((d3d11_module = GetModuleHandle(L"d3d11.dll")) == NULL)
		{
			DestroyWindow(window);
			UnregisterClass(window_class.lpszClassName, window_class.hInstance);

			return;
		}

		void* D3D11CreateDeviceAndSwapChain;
		if ((D3D11CreateDeviceAndSwapChain = GetProcAddress(d3d11_module, "D3D11CreateDeviceAndSwapChain")) == NULL)
		{
			DestroyWindow(window);
			UnregisterClass(window_class.lpszClassName, window_class.hInstance);

			return;
		}

		D3D_FEATURE_LEVEL feature_level;
		const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

		DXGI_RATIONAL refresh_rate;
		refresh_rate.Numerator = 60;
		refresh_rate.Denominator = 1;

		DXGI_MODE_DESC buffer_desc;
		buffer_desc.Width = 100;
		buffer_desc.Height = 100;
		buffer_desc.RefreshRate = refresh_rate;
		buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sample_desc;
		sample_desc.Count = 1;
		sample_desc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		swap_chain_desc.BufferDesc = buffer_desc;
		swap_chain_desc.SampleDesc = sample_desc;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.BufferCount = 1;
		swap_chain_desc.OutputWindow = window;
		swap_chain_desc.Windowed = 1;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* swap_chain;
		ID3D11Device* device;
		ID3D11DeviceContext* context;

		if (((long(__stdcall*)(
			IDXGIAdapter*,
			D3D_DRIVER_TYPE,
			HMODULE,
			UINT,
			const D3D_FEATURE_LEVEL*,
			UINT,
			UINT,
			const DXGI_SWAP_CHAIN_DESC*,
			IDXGISwapChain**,
			ID3D11Device**,
			D3D_FEATURE_LEVEL*,
			ID3D11DeviceContext**))(D3D11CreateDeviceAndSwapChain))(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, feature_levels, 1, D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device, &feature_level, &context) < 0)
		{
			DestroyWindow(window);
			UnregisterClass(window_class.lpszClassName, window_class.hInstance);

			return;
		}

		void** table = *(void***)swap_chain;

		minhook::add(table[8], &new_present, &original_present);

		utils::safe_release(&swap_chain);
		utils::safe_release(&device);
		utils::safe_release(&context);

		DestroyWindow(window);
		UnregisterClass(window_class.lpszClassName, window_class.hInstance);
	}



	void directx_hook::do_run(IDXGISwapChain* _swap_chain)
	{
		if (m_swap_chain == nullptr)
		{
			create_devices(_swap_chain);
		}
		else
		{
			if (m_swap_chain == _swap_chain)
			{
				call_event("on_render");
			}
			else
			{
				remove_devices();
			}
		}
	}



	void directx_hook::shutdown()
	{
		// remove_devices();

		log::get()->print(log_success, "Unitialized directx hook");
	}



	HRESULT STDMETHODCALLTYPE directx_hook::new_present(IDXGISwapChain* _swap_chain, UINT _sync_interval, UINT _flags)
	{
		directx_hook::get()->do_run(_swap_chain);

		return original_present(_swap_chain, _sync_interval, _flags);
	}



	void directx_hook::create_devices(IDXGISwapChain* _swap_chain)
	{
		DXGI_SWAP_CHAIN_DESC desc;
		_swap_chain->GetDesc(&desc);

		m_window = desc.OutputWindow;

		HRESULT hr;

		hr = _swap_chain->GetDevice(IID_PPV_ARGS(&m_device));

		if (FAILED(hr))
		{
			log::get()->print(log_error, "Failed to retrieve D3D11 device !");
			return;
		}

		m_device->GetImmediateContext(&m_device_context);
		
		call_event("on_create_devices", this);

		m_swap_chain = _swap_chain;
	}



	void directx_hook::remove_devices()
	{
		m_swap_chain = nullptr;

		call_event("on_remove_devices");
	}
}