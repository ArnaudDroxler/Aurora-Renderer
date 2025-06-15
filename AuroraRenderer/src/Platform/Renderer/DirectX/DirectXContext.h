#pragma once

#include "aurorapch.h"
#include "Platform/Renderer/GraphicContext.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

namespace Aurora 
{
	class DirectXContext : public GraphicContext
	{
	public:

		DirectXContext();
		~DirectXContext();

		void Init() override;

		bool InitDirectX(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
		void Shutdown();

		void SetVSync(bool enabled);

		void OnResize(unsigned int width, unsigned int height);

		void ClearColor(float red, float green, float blue, float alpha);
		void SwapBuffer();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void GetProjectionMatrix(XMMATRIX&);
		void GetWorldMatrix(XMMATRIX&);
		void GetOrthoMatrix(XMMATRIX&);

		void GetVideoCardInfo(char*, int&);

		void SetBackBufferRenderTarget();
		void ResetViewport();

	private:

		bool vsyncEnabled;

		int videoCardMemory;
		char videoCardDescription[128];

		IDXGISwapChain* swapChain;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		ID3D11RenderTargetView* renderTargetView;
		ID3D11Texture2D* depthStencilBuffer;
		ID3D11DepthStencilState* depthStencilState;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11RasterizerState* rasterState;

		XMMATRIX projectionMatrix;
		XMMATRIX worldMatrix;
		XMMATRIX orthoMatrix;

		D3D11_VIEWPORT viewport;



	};


}
