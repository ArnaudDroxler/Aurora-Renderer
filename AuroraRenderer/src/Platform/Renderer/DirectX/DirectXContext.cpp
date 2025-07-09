#include "aurorapch.h"
#include "DirectXContext.h"
#include <AuroraRenderer.h>
#include <dwmapi.h>


namespace Aurora
{

	DirectXContext::DirectXContext() : GraphicContext()
	{
		swapChain = nullptr;
		device = nullptr;
		deviceContext = nullptr;
		renderTargetView = nullptr;
		depthStencilBuffer = nullptr;
		depthStencilState = nullptr;
		depthStencilView = nullptr;
		rasterState = nullptr;
	}

	DirectXContext::~DirectXContext()
	{

	}

	void DirectXContext::Init()
	{

	}

	bool DirectXContext::InitDirectX(int screenWidth, int screenHeight, bool vSync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
	{

		HRESULT result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		result = adapter->EnumOutputs(0, &output);
		if (FAILED(result))
		{
			return false;
		}


		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.BufferCount = 1;

		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;

		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (vSync)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapChainDesc.OutputWindow = hwnd;

		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		if (fullscreen)
		{
			swapChainDesc.Windowed = false;
		}
		else
		{
			swapChainDesc.Windowed = true;
		}

		swapChainDesc.Windowed = true;

		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		swapChainDesc.Flags = 0;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &deviceContext);
		if (FAILED(result))
		{
			return false;
		}

		ID3D11Texture2D* backBufferPtr;

		result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result))
		{
			return false;
		}

		result = device->CreateRenderTargetView(backBufferPtr, nullptr, &renderTargetView);
		if (FAILED(result))
		{
			return false;
		}

		backBufferPtr->Release();
		backBufferPtr = nullptr;

		D3D11_TEXTURE2D_DESC depthBufferDesc = {};

		depthBufferDesc.Width = screenWidth;
		depthBufferDesc.Height = screenHeight;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		result = device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
		if (FAILED(result))
		{
			return false;
		}


		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		result = device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		if (FAILED(result))
		{
			return false;
		}

		deviceContext->OMSetDepthStencilState(depthStencilState, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);


		D3D11_RASTERIZER_DESC rasterDesc = {};

		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		result = device->CreateRasterizerState(&rasterDesc, &rasterState);
		if (FAILED(result))
		{
			return false;
		}

		deviceContext->RSSetState(rasterState);

		viewport.Width = (float)screenWidth;
		viewport.Height = (float)screenHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		deviceContext->RSSetViewports(1, &viewport);

		SetFullscreen(fullscreen);
		SetVSync(vSync);

		float fieldOfView, screenAspect;

		fieldOfView = 3.141592654f / 4.0f;
		screenAspect = (float)screenWidth / (float)screenHeight;

		projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

		worldMatrix = XMMatrixIdentity();

		orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

		return true;
	}

	void DirectXContext::Shutdown()
	{
		if (swapChain)
		{
			swapChain->SetFullscreenState(false, nullptr);
		}

		if (output)
		{
			output->Release();
			output = nullptr;
		}

		if (adapter)
		{
			adapter->Release();
			adapter = nullptr;
		}

		if (factory)
		{
			factory->Release();
			factory = nullptr;
		}

		if (rasterState)
		{
			rasterState->Release();
			rasterState = nullptr;
		}

		if (depthStencilView)
		{
			depthStencilView->Release();
			depthStencilView = nullptr;
		}

		if (depthStencilState)
		{
			depthStencilState->Release();
			depthStencilState = nullptr;
		}

		if (depthStencilBuffer)
		{
			depthStencilBuffer->Release();
			depthStencilBuffer = nullptr;
		}

		if (renderTargetView)
		{
			renderTargetView->Release();
			renderTargetView = nullptr;
		}

		if (deviceContext)
		{
			deviceContext->Release();
			deviceContext = nullptr;
		}

		if (device)
		{
			device->Release();
			device = nullptr;
		}

		if (swapChain)
		{
			swapChain->Release();
			swapChain = nullptr;
		}

	}

	void DirectXContext::SetVSync(bool enabled)
	{
		vSync = enabled;

		/*auto displayMode = GetCurrentDisplayMode();

		DXGI_MODE_DESC modeDesc = {};
		modeDesc.Width = displayMode.width;
		modeDesc.Height = displayMode.height;
		modeDesc.RefreshRate.Numerator = 60;
		modeDesc.RefreshRate.Denominator = 1;
		modeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

		HRESULT result = swapChain->ResizeTarget(&modeDesc);*/
	}

	void DirectXContext::OnResize(unsigned int width, unsigned int height)
	{
		Debug::CoreLog("DirectXContext::OnResize {0} {1}", width , height);

		if (deviceContext && swapChain && renderTargetView)
		{

			deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
			renderTargetView->Release();
			renderTargetView = nullptr;

			HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
			if (FAILED(hr))
			{
				Debug::CoreError("Error resizing swap chain");
				return;
			}

			ID3D11Texture2D* backBuffer = nullptr;
			hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
			if (SUCCEEDED(hr))
			{
				hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
				backBuffer->Release();
			}

			D3D11_VIEWPORT viewport = {};
			viewport.Width = (float)width;
			viewport.Height = (float)height;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;

			deviceContext->RSSetViewports(1, &viewport);
			deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
		}
	}

	void DirectXContext::ClearColor(float red, float green, float blue, float alpha)
	{
		float color[4];

		color[0] = red;
		color[1] = green;
		color[2] = blue;
		color[3] = alpha;

		deviceContext->ClearRenderTargetView(renderTargetView, color);

		deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void DirectXContext::SwapBuffer()
	{
		if (vSync)
		{
			swapChain->Present(1, 0);
		}
		else
		{
			swapChain->Present(0, 0);
		}
	}

	std::vector<DisplayMode> DirectXContext::GetDisplayModes()
	{
		std::vector<DisplayMode> displayModes = {};

		unsigned int numModes = 0;
		unsigned long long stringLength;

		int error;

		HRESULT result;
		result = output->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &numModes, nullptr);

		if (SUCCEEDED(result) && numModes > 0)
		{

			std::vector<DXGI_MODE_DESC> modes(numModes);

			result = output->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, 0, &numModes, modes.data());
			if (SUCCEEDED(result))
			{

				for (const auto& mode : modes)
				{
					DisplayMode displayMode;
					displayMode.width = mode.Width;
					displayMode.height = mode.Height;
					displayMode.refreshRateNumerator = mode.RefreshRate.Numerator;
					displayMode.refreshRateDenominator = mode.RefreshRate.Denominator;

					bool exists = false;
					for (const auto& existing : displayModes)
					{
						if (existing.width == displayMode.width &&
							existing.height == displayMode.height &&
							existing.refreshRateNumerator == displayMode.refreshRateNumerator &&
							existing.refreshRateDenominator == displayMode.refreshRateDenominator)
						{
							exists = true;
							break;
						}
					}

					if (!exists)
					{
						displayModes.push_back(displayMode);
					}
				}
			}
		}



		return displayModes;
	}

	bool DirectXContext::SetFullscreen(bool fullscreen)
	{
		this->fullscreen = fullscreen;

		HRESULT result = swapChain->SetFullscreenState(this->fullscreen, nullptr);

		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void DirectXContext::SetResolutionAndRefreshRate(DisplayMode displayMode)
	{
		Debug::CoreLog("DirectXContext::SetResolutionAndRefreshRate");

		swapChain->SetFullscreenState(false, nullptr);

		HRESULT result = swapChain->ResizeBuffers(
			1, 
			displayMode.width,
			displayMode.height,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0 
		);

		DXGI_MODE_DESC modeDesc = {};
		modeDesc.Width = displayMode.width;
		modeDesc.Height = displayMode.height;
		modeDesc.RefreshRate.Numerator = displayMode.refreshRateNumerator;
		modeDesc.RefreshRate.Denominator = displayMode.refreshRateDenominator;
		modeDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

		result = swapChain->ResizeTarget(&modeDesc);


		swapChain->SetFullscreenState(fullscreen, nullptr);

	}

	DisplayMode DirectXContext::GetCurrentDisplayMode()
	{
		DXGI_SWAP_CHAIN_DESC desc = {};
		DisplayMode current = {};

		auto result = swapChain->GetDesc(&desc);

		if (SUCCEEDED(result))
		{
			current.width = desc.BufferDesc.Width;
			current.height = desc.BufferDesc.Height;
			current.refreshRateNumerator = desc.BufferDesc.RefreshRate.Numerator;
			current.refreshRateDenominator = desc.BufferDesc.RefreshRate.Denominator;
		}

		//Debug::CoreCritical("DirectXContext::GetCurrentDisplayMode - Current Display Mode: {0}x{1} @ {2}Hz", current.width, current.height, current.refreshRateNumerator / (float)current.refreshRateDenominator);
		return current;
	}

	ID3D11Device* DirectXContext::GetDevice()
	{
		return device;
	}

	ID3D11DeviceContext* DirectXContext::GetDeviceContext()
	{
		return deviceContext;
	}

	void DirectXContext::GetProjectionMatrix(XMMATRIX& projectionMatrix)
	{
		projectionMatrix = this->projectionMatrix;
		return;
	}

	void DirectXContext::GetWorldMatrix(XMMATRIX& worldMatrix)
	{
		worldMatrix = this->worldMatrix;
		return;
	}

	void DirectXContext::GetOrthoMatrix(XMMATRIX& orthoMatrix)
	{
		orthoMatrix = this->orthoMatrix;
		return;
	}

	void DirectXContext::SetBackBufferRenderTarget()
	{
		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

		return;
	}

	void DirectXContext::ResetViewport()
	{
		deviceContext->RSSetViewports(1, &viewport);

		return;
	}
}