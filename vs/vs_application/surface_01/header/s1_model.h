////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model.h ----------
/*!
\file s1_model.h
\brief Interface for the Model class.
\author Gareth Edwards
*/

#pragma once

#include "../header/s1_include.h"


namespace surface_01
{


	class SystemParam;


	// ---------- Model class ----------
	class Model
	{

	public:

		// ---- cdtor
			Model();
			virtual ~Model();


		// ---- update
			enum CameraUpdate
			{
				XYZ       = 0,
				VECTOR    = 1,
				PTZ       = 2,
				TRANSLATE = 3,
				ROTATE    = 4,
				TARGET    = 5,
			};


		// ---- target
			enum CameraTarget
			{
				NONE = 999,
				NONE_RUNTIME
			};


		// ---- mode
			enum
			{
				PARAM_GROUP_NULL    = 0,
				PARAM_GROUP_CAMERA  = 1
			};


		// ---- framework methods
			INT Setup(vs_system::AppDependency*);
			INT SetupGraphics();
			INT Display();
			INT Display(vs_system::PanelContext *, CHAR *);
			INT CleanupGraphics();
			INT Cleanup();


		// ---- pimp interface
			vs_system::AppDependency* GetAppDependency();


		// ---- exploded cube
			INT  InitExplodedCube();
			INT  InitExplodedCubeScene();
			INT  InitExplodedCubeObject();
			INT  SetupExplodedCubeDisplay();


		// ---- star system
			INT  InitStarSystem();
			INT  InitStarSystemScene();
			INT  InitStarSystemObjects();
			INT  SetupStarSystemDisplay();
			INT  GetSystemParam(SystemParam **);


		// ---- star system geometry
			BOOL IsObjectGeometry(INT);
			BOOL IsSelectedObjectGeometry(INT);


		// ---- wavefront
			INT  InitWavefront();
			INT  InitWavefrontScene();
			INT  InitWavefrontObject();
			INT  SetupWavefrontDisplay();


		// ---- shared methods
			#include "../../surface_shared/h/model_camera_methods_incl.h"
			#include "../../surface_shared/h/model_panel_methods_incl.h"


		// ---- Toolbar - required
			#include "../../surface_shared/hpp_toolbar/model_toolbar_interface.hpp"


		// ---- Tint - required
			#include "../hpp_tint/s1_tint_interface.hpp"


		// ---- Keypad - required
			#include "../../surface_shared/hpp_kpad/model_kpad_interface.hpp"


		// ---- parameter & methods included for facade copy
			vs_system::Parameter parameter;
			#include "vs/vs_library/header/vs_parameter_methods_static.h"


		// ---- atlas page
			INT InitAtlasPage();


		// ---- img capture
			INT SetImgCaptureEventId(UINT);
			INT SetImgCaptureElapsedMs(DOUBLE);


		// ---- s1_img_props.hpp
			INT GetGfxShaderParam(UINT, FLOAT *param);
			INT SetGfxShaderParam(UINT, FLOAT *param);
			INT GetImgProps(vs_system::ImgProps **);
			INT SetImgPropsShaders();
			INT ReadImgProps();
			INT SaveImgProps();


		// ---- s1_model_my_star.hpp
			INT SysObjectSelect(INT, INT);


		// --- parameter setup - "s3_surface_03_setup.hpp"
			INT SetupParamCamera(INT *);
			INT SetupParamNull();


		// ---- Keypad enumerations - required
			#include "../../surface_shared/hpp_kpad/model_kpad_enum.hpp"

	public:

		// ---- private implementation
			class  PI_Model; PI_Model *pi_model;

	};

	class  Model::PI_Model;
	static Model::PI_Model *s_pi_model;

}