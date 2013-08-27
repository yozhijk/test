#include "DX11Context.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CompiledMesh.h"
#include "Mesh.h"

#ifdef _DEBUG
#include "utils.h"
#endif

#include <stdexcept>
#include "utils.h"

#define THROW_IF_FAILED(x,m) if((x)!=S_OK) throw runtime_error(m)

using namespace std;
using namespace core;

DX11Context::DX11Context(HWND hWnd) 
	:hWnd_(hWnd)
	,pointLights_(POINT_LIGHT_MAX)
	,spotLights_(SPOT_LIGHT_MAX)
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
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_10_0};
	UINT numLevels = sizeof(featureLevels)/sizeof(D3D_FEATURE_LEVEL);

	DWORD flags = 0;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Attempt to create device and swap chain
	THROW_IF_FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, numLevels, D3D11_SDK_VERSION,
		&swapChainDesc, &swapChain_, &device_, &featureLevel_, &immediateContext_ ), "Couldn't create device and swap chain");

	// Create transforms constant buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(TransformData);
	bufferDesc.StructureByteStride = sizeof(TransformData);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	THROW_IF_FAILED(device_->CreateBuffer(&bufferDesc, nullptr, &transformCB_), "Failed to create transforms constant buffer");

	// Create point lights constant buffer
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(PointLightData) * pointLights_.size();
	bufferDesc.StructureByteStride = sizeof(PointLightData);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	THROW_IF_FAILED(device_->CreateBuffer(&bufferDesc, nullptr, &pointLightCB_), "Failed to create point lights constant buffer");

	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = sizeof(SpotLightData) * spotLights_.size();
	bufferDesc.StructureByteStride = sizeof(SpotLightData);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	THROW_IF_FAILED(device_->CreateBuffer(&bufferDesc, nullptr, &spotLightCB_), "Failed to create point lights constant buffer");

	D3D11_RASTERIZER_DESC rasterDesc = 
	{
		D3D11_FILL_SOLID, //FillMode
		D3D11_CULL_NONE, //CullMode
		0x0, //FrontCounterClockwise
		0x0, //DepthBias
		0.f, //DepthBiasClamp
		0.f, //SlopeScaledDepthBias
		0x1, //DepthClipEnable this should be turned on after transforms are fixed
		0x0, //ScissorEnable
		0x0, //MultisampleEnable
		0x0  //AntialiasedLineEnable
	};

	THROW_IF_FAILED(device_->CreateRasterizerState(&rasterDesc, &rasterizerState_), "Failed to create rasterizer state");

	D3D11_DEPTH_STENCIL_DESC dsDesc = 
	{
		0x1, //DepthEnable
		D3D11_DEPTH_WRITE_MASK_ALL, //DepthWriteMask
		D3D11_COMPARISON_LESS_EQUAL, //DepthFunc
		0x1, //StencilEnable
		'\xFF', //StencilReadMask
		'\xFF', //StencilWriteMask
		{
			D3D11_STENCIL_OP_KEEP, //StencilFailOp
			D3D11_STENCIL_OP_KEEP, //StencilDepthFailOp
			D3D11_STENCIL_OP_REPLACE, //StencilPassOp
			D3D11_COMPARISON_ALWAYS  //StencilFunc
		}, //FrontFace
		{
			D3D11_STENCIL_OP_KEEP, //StencilFailOp
			D3D11_STENCIL_OP_KEEP, //StencilDepthFailOp
			D3D11_STENCIL_OP_REPLACE, //StencilPassOp
			D3D11_COMPARISON_ALWAYS  //StencilFunc
		}  //BackFace
	};

	THROW_IF_FAILED(device_->CreateDepthStencilState(&dsDesc, &dsState_), "Failed to create depth state");;

	// Create default render target and depth stencil
	ResizeBuffer(ui_size(bbWidth, bbHeight));

	// Set viewport to cover the whole backbuffer
	SetViewport(ui_rect(0, 0, bbWidth, bbHeight));
}

void DX11Context::ResizeBuffer(ui_size const& size)
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
	depthTextureDesc.Height = size.h;
	depthTextureDesc.Width = size.w;
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

/// Set viewport within  [0,0, bbheight, bbwidth] range
void DX11Context::SetViewport(ui_rect const& vp)
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

void DX11Context::SetWorldMatrix(matrix4x4 const& worldMatrix)
{
	worldMatrix_ = worldMatrix;
}

void DX11Context::SetViewMatrix(matrix4x4 const& viewMatrix)
{
	viewMatrix_ = viewMatrix;
}

void DX11Context::SetFrustum(frustum const& frustum)
{
	projMatrix_ = perspective_proj_fovy_matrix_lh_dx(frustum.fovy, frustum.aspect, frustum.nr, frustum.fr);
}

void DX11Context::CommitState()
{
	// Update point lights constant buffer
	immediateContext_->UpdateSubresource(pointLightCB_, D3D11CalcSubresource(0, 0, 1), nullptr, &pointLights_[0], 0, 0);
	// Update spot lights constant buffer
	immediateContext_->UpdateSubresource(spotLightCB_, D3D11CalcSubresource(0, 0, 1), nullptr, &spotLights_[0], 0, 0);

	// Set shaders & state
	immediateContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext_->IASetInputLayout(shaderCache_.GetShaderProgram("simple", device_).GetInputLayout());
	immediateContext_->VSSetShader(shaderCache_.GetShaderProgram("simple", device_).GetVertexShader(), nullptr, 0);
	immediateContext_->VSSetConstantBuffers(0, 1, &transformCB_.p);
	immediateContext_->PSSetShader(shaderCache_.GetShaderProgram("simple", device_).GetPixelShader(), nullptr, 0);
	immediateContext_->PSSetConstantBuffers(1, 1, &pointLightCB_.p);
	immediateContext_->PSSetConstantBuffers(2, 1, &spotLightCB_.p);
	immediateContext_->RSSetState(rasterizerState_);
	immediateContext_->OMSetDepthStencilState(dsState_, 0);
}

void DX11Context::DrawMesh(CompiledMesh const& mesh)
{
	// Set constant buffers
	TransformData transformData;
	transformData.mWorld = worldMatrix_;
	transformData.mWorldViewProj = projMatrix_ * viewMatrix_ *  worldMatrix_ ;

	immediateContext_->UpdateSubresource(transformCB_, D3D11CalcSubresource(0, 0, 1), nullptr, &transformData, 0, 0);

	ID3D11Buffer* pVertexBuffer = reinterpret_cast<ID3D11Buffer*>(mesh.GetVertexBufferID());
	ID3D11Buffer* pIndexBuffer = reinterpret_cast<ID3D11Buffer*>(mesh.GetIndexBufferID());

	UINT stride = mesh.GetVertexSizeInBytes();
	UINT offset = 0;
	immediateContext_->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	immediateContext_->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	immediateContext_->DrawIndexed(mesh.GetIndexCount(), 0, 0);
}

void DX11Context::Clear(color_rgba const& color)
{
	FLOAT clearColor[4] = {static_cast<FLOAT>(color.x()), static_cast<FLOAT>(color.y()), static_cast<FLOAT>(color.z()), static_cast<FLOAT>(color.w())};
	immediateContext_->ClearRenderTargetView(defaultRenderTarget_, clearColor);

	/// move to ClearDepth method
	immediateContext_->ClearDepthStencilView(defaultDepthBuffer_, D3D11_CLEAR_DEPTH, 1.0, 0);
}

void DX11Context::Present()
{
	swapChain_->Present(0,0);
}

IResourceManager& DX11Context::GetResourceManager()
{
	return *this;
}

shared_ptr<CompiledMesh> DX11Context::CompileMesh(Mesh const& mesh)
{
	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(bufDesc));

	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.ByteWidth = mesh.GetVertexCount() * mesh.GetVertexSizeInBytes();
	bufDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA srData;
	ZeroMemory(&srData, sizeof(srData));
	srData.pSysMem = mesh.GetVertexArrayPointer();

	ID3D11Buffer* pVertexBuffer = nullptr;
	THROW_IF_FAILED(device_->CreateBuffer(&bufDesc, &srData, &pVertexBuffer), "Cannot create mesh vertex buffer");

	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.ByteWidth = mesh.GetIndexCount() * sizeof(unsigned short);
	bufDesc.Usage = D3D11_USAGE_DEFAULT;

	ZeroMemory(&srData, sizeof(srData));
	srData.pSysMem = mesh.GetIndexArrayPointer();

	ID3D11Buffer* pIndexBuffer = nullptr;
	THROW_IF_FAILED(device_->CreateBuffer(&bufDesc, &srData, &pIndexBuffer), "Cannot create mesh index buffer");

	/// Potential 64-bit compatibily issue, fix later
	return make_shared<CompiledMesh>(reinterpret_cast<uint>(pVertexBuffer), reinterpret_cast<uint>(pIndexBuffer), mesh.GetIndexCount(), mesh.GetVertexSizeInBytes(), bind(&DX11Context::OnReleaseMesh, this, placeholders::_1));
}

void DX11Context::OnReleaseMesh(CompiledMesh const& mesh)
{
	ID3D11Buffer* pVertexBuffer = reinterpret_cast<ID3D11Buffer*>(mesh.GetVertexBufferID());
	ID3D11Buffer* pIndexBuffer = reinterpret_cast<ID3D11Buffer*>(mesh.GetIndexBufferID());

	/// Release index/vertex buffers etc
	pVertexBuffer->Release();
	pIndexBuffer->Release();
}

void DX11Context::SetPointLight(PointLightIndex index, PointLight const& light)
{
	assert (index < POINT_LIGHT_MAX);
	pointLights_[index].vPos = light.GetPosition();
	pointLights_[index].vColor = light.GetColor();
}

void DX11Context::SetPointLightEnabled(PointLightIndex index, bool bEnabled)
{
	assert (index < POINT_LIGHT_MAX);
	pointLights_[index].vPos.w() = bEnabled?1.f:0.f;
}

bool DX11Context::IsPointLightEnabled(PointLightIndex index)
{
	assert (index < POINT_LIGHT_MAX);
	return pointLights_[index].vPos.w() > 0.f;
}

void DX11Context::SetSpotLight(SpotLightIndex index, SpotLight const& light)
{
	assert (index < SPOT_LIGHT_MAX);
	spotLights_[index].vPos = light.GetPosition();
	spotLights_[index].vDir = light.GetDirection();
	spotLights_[index].vColor = light.GetColor();
	spotLights_[index].vAngle.x() = light.GetInnerAngle();
	spotLights_[index].vAngle.y() = light.GetOuterAngle();
}

void DX11Context::SetSpotLightEnabled(SpotLightIndex index, bool bEnabled)
{
	assert (index < SPOT_LIGHT_MAX);
	spotLights_[index].vPos.w() = bEnabled?1.f:0.f;
}

bool DX11Context::IsSpotLightEnabled(SpotLightIndex index)
{
	assert (index < SPOT_LIGHT_MAX);
	return spotLights_[index].vPos.w() > 0.f;
}
