#include "DX11Context.h"
#include "CompiledModel.h"
#include "Model.h"
#include <stdexcept>

#define THROW_IF_FAILED(x,m) if((x)!=S_OK) throw std::runtime_error(m)

DX11Context::DX11Context(HWND hWnd) : 
hWnd_(hWnd)
{

}

void DX11Context::Init()
{
	RECT clientRect;
	GetClientRect(hWnd_, &clientRect);

	UINT bbWidth  = clientRect.right  - clientRect.left;
	UINT bbHeight = clientRect.bottom - clientRect.top;

	// Fill out swap chain desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Double buffering
	swapChainDesc.BufferCount = 1;

	// Backbuffer attributes
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width = bbWidth;
	swapChainDesc.BufferDesc.Height = bbHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	// Buffer usage
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd_;

	// No multisampling
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = TRUE;

	// Required feature levels
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_11_0};
	UINT numLevels = sizeof(featureLevels)/sizeof(D3D_FEATURE_LEVEL);

	// Attempt to create device and swap chain
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, featureLevels, numLevels, D3D11_SDK_VERSION,
		&swapChainDesc, &swapChain_, &device_, &featureLevel_, &immediateContext_ )))
	{
		throw std::runtime_error("Couldn't create device and swap chain");

	}

	// Create default render target and depth stencil
	ResizeBuffer(core::ui_size(bbWidth, bbHeight));
	
	// Set viewport to cover the whole backbuffer
	SetViewport(core::ui_rect(0, 0, bbWidth, bbHeight));
}

void DX11Context::ResizeBuffer(core::ui_size const& size)
{
	// Release old stuff if any
	defaultRenderTarget_.Release();
	defaultDepthBuffer_.Release();

	// Resize swapchain accordingly
	THROW_IF_FAILED(swapChain_->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0), "Couldn't resize swap chain");

	// Retrieve default render target
	CComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	THROW_IF_FAILED(swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer), "Couldn't retrieve back buffer");

	// Create default render target view
	THROW_IF_FAILED(device_->CreateRenderTargetView(pBackBuffer, nullptr, &defaultRenderTarget_), "Couldn't create default render target view");

	CComPtr<ID3D11Texture2D> depthTexture;

	// Create depth-stencil texture
	D3D11_TEXTURE2D_DESC depthTextureDesc;
	ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
	depthTextureDesc.ArraySize = 1;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTextureDesc.Height = size.w;
	depthTextureDesc.Width = size.h;
	depthTextureDesc.MipLevels = 1;
	depthTextureDesc.SampleDesc.Count = 1;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;

	THROW_IF_FAILED(device_->CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture.p), "Couldn't create default depth-stencil");

	// Create depth-stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	ZeroMemory(&depthViewDesc, sizeof(depthViewDesc));
	depthViewDesc.Format = depthTextureDesc.Format;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;

	THROW_IF_FAILED(device_->CreateDepthStencilView(depthTexture, &depthViewDesc, &defaultDepthBuffer_),"Couldn't create default depth-stencil view");

	// Set default render targte and depth stencil
	immediateContext_->OMSetRenderTargets(1, &defaultRenderTarget_.p, defaultDepthBuffer_);
}

/// Set viewport within  [0,0,bbheight, bbwidth] range
void DX11Context::SetViewport(core::ui_rect const& vp)
{
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = static_cast<FLOAT>(vp.x);
	viewport.TopLeftY = static_cast<FLOAT>(vp.y);
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	viewport.Width = static_cast<FLOAT>(vp.w);
	viewport.Height = static_cast<FLOAT>(vp.h);

	immediateContext_->RSSetViewports(1, &viewport);
}

void DX11Context::SetWorldMatrix(core::matrix4x4 const& worldMatrix)
{

}
void DX11Context::SetViewMatrix(core::matrix4x4 const& viewMatrix)
{

}
void DX11Context::SetProjectionMatrix(core::matrix4x4 const& projMatrix)
{

}

void DX11Context::DrawModel(/*CompiledModel& model*/)
{
}

void DX11Context::Clear(core::color_rgba const& color)
{
	FLOAT clearColor[4] = {static_cast<FLOAT>(color.x()), static_cast<FLOAT>(color.y()), static_cast<FLOAT>(color.z()), static_cast<FLOAT>(color.w())};
	immediateContext_->ClearRenderTargetView(defaultRenderTarget_, clearColor);
}

void DX11Context::Present()
{
	swapChain_->Present(0,0);
}

IResourceManager& DX11Context::GetResourceManager()
{
	return *this;
}

std::unique_ptr<CompiledModel> DX11Context::CompileModel(Model const& model)
{
	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(bufDesc));

	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.ByteWidth = model.GetVertexCount() * model.GetVertexSizeInBytes();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA srData;
	ZeroMemory(&srData, sizeof(srData));
	srData.pSysMem = model.GetVertexArrayPointer();

	ID3D11Buffer* pVertexBuffer = nullptr;
	THROW_IF_FAILED(device_->CreateBuffer(&bufDesc, &srData, &pVertexBuffer), "Cannot create mesh vertex buffer");

	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.ByteWidth = model.GetIndexCount() * sizeof(unsigned short);
	bufDesc.Usage = D3D11_USAGE_DEFAULT;

	ZeroMemory(&srData, sizeof(srData));
	srData.pSysMem = model.GetIndexArrayPointer();

	ID3D11Buffer* pIndexBuffer = nullptr;
	THROW_IF_FAILED(device_->CreateBuffer(&bufDesc, &srData, &pIndexBuffer), "Cannot create mesh index buffer");

	/// Potential 64-bit compatibily issue, fix later
	return std::unique_ptr<CompiledModel>(new CompiledModel(reinterpret_cast<core::uint>(pVertexBuffer), reinterpret_cast<core::uint>(pIndexBuffer), std::bind(&DX11Context::OnReleaseModel, this, std::placeholders::_1)));
}

void DX11Context::OnReleaseModel(CompiledModel const& model)
{
	ID3D11Buffer* pVertexBuffer = reinterpret_cast<ID3D11Buffer*>(model.GetVertexBufferID());
	ID3D11Buffer* pIndexBuffer = reinterpret_cast<ID3D11Buffer*>(model.GetIndexBufferID());
	
	/// Release index/vertex buffers etc
	pVertexBuffer->Release();
	pIndexBuffer->Release();
}
