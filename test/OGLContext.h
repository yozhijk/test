/////////////////////////////////////////////////////
/// @file OGLContext.h
///	    OpenGL implementation of IGraphicsContext iface
///
/// @author Dmitry Kolzov
///
#ifndef OGLCONTEXT_H
#define OGLCONTEXT_H

#include <GLUT/GLUT.h>
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "OGLShaderManager.h"

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
	void SetFrustum(core::frustum const& frustum);
    
	/// Draw 3D model with default lighting / effects
	void DrawMesh(CompiledMesh const& mesh);
    
    /// Set point light properties
    void SetPointLight(PointLightIndex index, PointLight const& light);
	/// Set point light enabled flag
	void SetPointLightEnabled(PointLightIndex index, bool bEnabled);
	/// Enabled query
	bool IsPointLightEnabled(PointLightIndex index);
    
	/// Set spot light properties
	void SetSpotLight(SpotLightIndex index, SpotLight const& light);
	/// Set point light enabled flag
	void SetSpotLightEnabled(SpotLightIndex index, bool bEnabled);
	/// Enabled query
	bool IsSpotLightEnabled(SpotLightIndex index);
    
	/// Commit per-frame states (such as lighting params) to device
	void CommitState();
    
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
	std::shared_ptr<CompiledMesh> CompileMesh(Mesh const& mesh);
    
protected:
    /// Release model callback
    void OnReleaseMesh( CompiledMesh const& mesh );
    
private:
    
    OGLContext(OGLContext const&);
    OGLContext& operator = (OGLContext const&);
    
    struct PointLightData
	{
		core::vector4 vPos;
		core::vector4 vColor;
	};
    
	struct SpotLightData
	{
		core::vector4 vPos;
		core::vector4 vDir;
		core::vector4 vColor;
		core::vector4 vAngle;
	};
    
    /// Point lights
	std::vector<PointLightData> pointLights_;
	std::vector<SpotLightData>  spotLights_;
    
    OGLShaderManager shaderManager_;
    
    core::matrix4x4 worldMatrix_;
    core::matrix4x4 viewMatrix_;
    core::matrix4x4 projMatrix_;
};


#endif // OGLCONTEXT_H
