/////////////////////////////////////////////////////
/// @file OGLContext.h
///	    OpenGL implementation of IGraphicsContext iface
///
/// @author Dmitry Kolzov
///
#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H

#include "IGraphicsContext.h"
#include "IResourceManager.H"

struct Window;
class Mesh;
class CompiledMesh;

class OGLContext : public IGraphicsContext, IResourceManager
{
public:
    OGLContext();
    ~OGLContext();
    
    /// IGraphicsContext overrides
    
	/// Initialize graphics API and create necessarry recources
	/// Should be called once at startup priop to any use
    void Init();
	/// Resize backing buffer
	void ResizeBuffer(core::ui_size const& size);
	/// Set viewport within  [0,0,bbheight, bbwidth] range
	void SetViewport(core::ui_rect const& vp);
    
	/// Set coordinate space transforms
	void SetWorldMatrix(core::matrix4x4 const& worldMatrix);
	void SetViewMatrix(core::matrix4x4 const& viewMatrix);
	void SetProjectionMatrix(core::matrix4x4 const& projMatrix);
    
	/// Draw 3D model with default lighting / effects
	void DrawMesh(CompiledMesh const& mesh);
    
	/// Clear canvas
	void Clear(core::color_rgba const& color);
	/// Present backbuffer
	/// REQUIREMENT: This method shouldn't be called inside an engine's rendering code
	/// as it could mess up the order, let app object be responsible for presenting
	void Present();
    
	/// Retrieve resource manager from the context
	IResourceManager& GetResourceManager();
    
    /// IResourceManager overrides
    /// Transform model into API-friendly form (would look into AssImp)
	std::unique_ptr<CompiledMesh> CompileMesh(Mesh const& mesh);
    
protected:
    /// Release model callback
    void OnReleaseMesh( CompiledMesh const& mesh );
};


#endif // OGLCONTEXT_H
