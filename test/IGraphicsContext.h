/////////////////////////////////////////////////////
/// @file IGraphicsContext.h
///	 Platform independent graphics API concept
///
/// @author Dmitry Kolzov
///
#ifndef IGRAPHICS_CONTEXT
#define IGRAPHICS_CONTEXT

#include <vector>

#include "common_types.h"

class CompiledMesh;
class PointLight;
class IResourceManager;

/////////////////////////////////////////////////////////
/// Graphics API to be used by the engine inside OnRender
/// callbacks, would probably have DX, OGL, SW backends	
///
class IGraphicsContext
{
public:
	/// Types
	enum PointLightIndex
	{
		LIGHT_0,
		LIGHT_1,
		LIGHT_2,
		LIGHT_3,
		LIGHT_MAX,
	};

public:
	/// Methods
	virtual ~IGraphicsContext() = 0;

	/// Initialize graphics API and create necessarry recources
	/// Should be called once at startup priop to any use
	virtual void Init() = 0;
	/// Resize backing buffer
	virtual void ResizeBuffer(core::ui_size const& size) = 0; 
	/// Set viewport within  [0, 0, bbheight, bbwidth] range
	virtual void SetViewport(core::ui_rect const& vp) = 0; 

	/// Set coordinate space transforms
	virtual void SetWorldMatrix(core::matrix4x4 const& worldMatrix) = 0;
	virtual void SetViewMatrix(core::matrix4x4 const& viewMatrix) = 0;
	virtual void SetFrustum(core::frustum const& frustum) = 0;

	/// Draw 3D model with default lighting / effects
	virtual void DrawMesh(CompiledMesh const& mesh) = 0;

	/// Set point light properteies
	virtual void SetPointLight(PointLightIndex index, PointLight const& light) = 0;
	/// Set point light enabled flag
	virtual void SetPointLightEnabled(PointLightIndex index, bool bEnabled) = 0;
	/// Enabled query
	virtual bool IsPointLightEnabled(PointLightIndex index) = 0;

	/// Clear canvas
	virtual void Clear(core::color_rgba const& color) = 0;
	/// Present backbuffer
	/// REQUIREMENT: This method shouldn't be called inside an engine's rendering code
	/// as it could mess up the order, let app object be responsible for presenting
	virtual void Present() = 0;

	/// Retrieve resource manager from the context
	virtual IResourceManager& GetResourceManager() = 0;
};

inline IGraphicsContext::~IGraphicsContext() {}


#endif // IGRAPHICS_CONTEXT