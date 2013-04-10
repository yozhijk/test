/////////////////////////////////////////////////////
/// @file DX11Context.h
///	    DX11 implementation of IGraphicsContext iface
///
/// @author Dmitry Kolzov
///

#ifndef DX11CONTEXT_H
#define DX11CONTEXT_H

#include "common_types.h"

#include <windows.h>
#include <d3d11.h>
#include <atlbase.h>

#include "IGraphicsContext.h"
#include "IResourceManager.h"

/////////////////////////////////////////////////////
/// DX11Context, currently holding roles of the context
///	and resource manager simultaneously, should probably
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
	void SetProjectionMatrix(core::matrix4x4 const& projMatrix);

	/// Draw 3D model with default lighting / effects
	void DrawModel( /*CompiledModel& model*/ );
	/// Clear canvas
	void Clear(core::color_rgba const& color);
	/// Present backbuffer
	void Present();
	/// Retrieve resource manager
	IResourceManager& GetResourceManager();

	/// IResourceManager overrides
	/// Transform model into API-friendly form (would look into AssImp)
	std::unique_ptr<CompiledModel> CompileModel(Model const& model);

protected:
	/// CompiledModel release callback
	void OnReleaseModel(CompiledModel const& model);

private:
	DX11Context(DX11Context const&);
	DX11Context& operator = (DX11Context const&);

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

	 /// Window handle
	 HWND hWnd_;
};

#endif