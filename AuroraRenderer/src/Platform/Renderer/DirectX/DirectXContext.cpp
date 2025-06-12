#include "aurorapch.h"
#include "DirectXContext.h"


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

	bool DirectXContext::InitDirectX(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
	{
		HRESULT result;
		IDXGIFactory* factory;
		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		unsigned int numModes, i, numerator, denominator = 0;
		unsigned long long stringLength;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		int error;

		D3D_FEATURE_LEVEL featureLevel;
		ID3D11Texture2D* backBufferPtr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_RASTERIZER_DESC rasterDesc;
		float fieldOfView, screenAspect;

		vsyncEnabled = vsync;

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result))
		{
			return false;
		}

		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result))
		{
			return false;
		}

		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result))
		{
			return false;
		}

		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
		if (FAILED(result))
		{
			return false;
		}

		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList)
		{
			return false;
		}

		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result))
		{
			return false;
		}

		numerator = 0;
		denominator = 1;
		for (i = 0; i < numModes; i++)
		{
			if (displayModeList[i].Width == (unsigned int)screenWidth)
			{
				if (displayModeList[i].Height == (unsigned int)screenHeight)
				{

					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))
		{
			return false;
		}

		videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		error = wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128);
		if (error != 0)
		{
			return false;
		}

		delete[] displayModeList;
		displayModeList = 0;

		adapterOutput->Release();
		adapterOutput = nullptr;

		adapter->Release();
		adapter = nullptr;

		factory->Release();
		factory = nullptr;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.BufferCount = 1;

		swapChainDesc.BufferDesc.Width = screenWidth;
		swapChainDesc.BufferDesc.Height = screenHeight;

		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (vsyncEnabled)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
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

		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDesc.Flags = 0;

		featureLevel = D3D_FEATURE_LEVEL_11_1;

		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &deviceContext);
		if (FAILED(result))
		{
			return false;
		}

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

		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

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

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

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

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
		if (FAILED(result))
		{
			return false;
		}

		deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

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

	void DirectXContext::OnResize(unsigned int width, unsigned int height)
	{
		if (deviceContext && swapChain && renderTargetView)
		{
			// Libérez les ressources existantes
			deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
			renderTargetView->Release();
			renderTargetView = nullptr;

			// Redimensionnez le swap chain
			HRESULT hr = swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
			if (FAILED(hr))
			{
				Debug::CoreError("Error resizing swap chain");
				return;
			}

			// Recréez le render target view
			ID3D11Texture2D* backBuffer = nullptr;
			hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
			if (SUCCEEDED(hr))
			{
				hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
				backBuffer->Release();
			}

			// Mettez à jour le viewport
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

		return;
	}

	void DirectXContext::SwapBuffer()
	{
		if (vsyncEnabled)
		{
			swapChain->Present(1, 0);
		}
		else
		{
			swapChain->Present(0, 0);
		}

		return;
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

	void DirectXContext::GetVideoCardInfo(char* cardName, int& memory)
	{
		strcpy_s(cardName, 128, videoCardDescription);
		memory = videoCardMemory;
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