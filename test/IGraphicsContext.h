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
class SpotLight;
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
		POINT_LIGHT_0,
		POINT_LIGHT_1,
		POINT_LIGHT_2,
		POINT_LIGHT_3,
		POINT_LIGHT_MAX,
	};

	enum SpotLightIndex
	{
		SPOT_LIGHT_0,
		SPOT_LIGHT_1,
		SPOT_LIGHT_2,
		SPOT_LIGHT_3,
		SPOT_LIGHT_MAX,
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

	/// Set point light properties
	virtual void SetPointLight(PointLightIndex index, PointLight const& light) = 0;
	/// Set point light enabled flag
	virtual void SetPointLightEnabled(PointLightIndex index, bool bEnabled) = 0;
	/// Enabled query
	virtual bool IsPointLightEnabled(PointLightIndex index) = 0;

	/// Set spot light properties
	virtual void SetSpotLight(SpotLightIndex index, SpotLight const& light) = 0;
	/// Set point light enabled flag
	virtual void SetSpotLightEnabled(SpotLightIndex index, bool bEnabled) = 0;
	/// Enabled query
	virtual bool IsSpotLightEnabled(SpotLightIndex index) = 0;

	/// Commit per-frame states (such as lighting params) to device
	virtual void CommitState() = 0;

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