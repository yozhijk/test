/////////////////////////////////////////////////////
/// @file DX11Context.h
///	 DX11 implementation of IGraphicsContext iface
///
/// @author Dmitry Kolzov
///

#ifndef DX11CONTEXT_H
#define DX11CONTEXT_H

#include "common_types.h"

#include <windows.h>
#include <d3d11.h>
#include <atlbase.h>
#include <vector>

#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "DX11ShaderManager.h"

class Mesh;
class CompiledMesh;

/////////////////////////////////////////////////////
/// DX11Context, currently holding roles of the context
/// and resource manager simultaneously, should probably
/// be splitted
class DX11Context : public IGraphicsContext, IResourceManager
{
public: 
	DX11Context(HWND hWnd);

	/// IGraphicsContext overrides

	/// Initialize graphics API and create necessarry recources
	/// Should be called once at startup priop to any use
	void Init();
	/// Resize backing buffer
	void ResizeBuffer( core::ui_size const& size ); 
	/// Set viewport within  [0,0,bbheight, bbwidth] range
	void SetViewport( core::ui_rect const& vp ); 

	/// Set coordinate space transforms
	void SetWorldMatrix(core::matrix4x4 const& worldMatrix);
	void SetViewMatrix(core::matrix4x4 const& viewMatrix);
	void SetFrustum(core::frustum const& frustum);

	/// Set point light properteies
	void SetPointLight(PointLightIndex index, PointLight const& light);
	/// Set point light enabled flag
	void SetPointLightEnabled(PointLightIndex index, bool bEnabled);
	/// Enabled query
	bool IsPointLightEnabled(PointLightIndex index);

	/// Commit per-frame states (such as lighting params) to device
	void CommitState();

	/// Draw 3D model with default lighting / effects
	void DrawMesh(CompiledMesh const& mesh);
	/// Clear canvas
	void Clear(core::color_rgba const& color);
	/// Present backbuffer
	void Present();
	/// Retrieve resource manager
	IResourceManager& GetResourceManager();

	/// IResourceManager overrides
	/// Transform model into API-friendly form (would look into AssImp)
	std::shared_ptr<CompiledMesh> CompileMesh(Mesh const& mesh);

protected:
	/// CompiledModel release callback
	void OnReleaseMesh(CompiledMesh const& mesh);

private:
	DX11Context(DX11Context const&);
	DX11Context& operator = (DX11Context const&);

	struct TransformData
	{
		core::matrix4x4 mWorld;
		core::matrix4x4 mWorldViewProj;
	};

	struct PointLightData
	{
		core::vector4 vPos;
		core::vector4 vColor;
	};

	/// Feature level
	D3D_FEATURE_LEVEL featureLevel_;
	/// Direct3D device
	CComPtr<ID3D11Device> device_;
	/// Swap chain 
	CComPtr<IDXGISwapChain> swapChain_;
	/// Immediate context
	CComPtr<ID3D11DeviceContext> immediateContext_;
	/// Default render target
	CComPtr<ID3D11RenderTargetView> defaultRenderTarget_;
	CComPtr<ID3D11DepthStencilView> defaultDepthBuffer_;
	/// Transforms constant buffer
	CComPtr<ID3D11Buffer> transformCB_;
	// Point lights constant buffer
	CComPtr<ID3D11Buffer> pointLightCB_;
	/// Raster state
	CComPtr<ID3D11RasterizerState> rasterizerState_;
	/// Depth & stencil state
	CComPtr<ID3D11DepthStencilState> dsState_;;

	/// Window handle
	HWND hWnd_;

	/// Transform matrices
	core::matrix4x4 worldMatrix_;
	core::matrix4x4 viewMatrix_;
	core::matrix4x4 projMatrix_;

	/// Point lights
	std::vector<PointLightData> pointLights_;

	/// Shader cache
	DX11ShaderManager shaderCache_;
};

#endif