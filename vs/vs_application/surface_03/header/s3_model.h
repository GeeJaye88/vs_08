////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model.h ----------
/*!
\file s3_model.h
\brief Interface for the Model class.
\author Gareth Edwards
*/

#pragma once

#include "../header/s3_include.h"


namespace surface_03
{

	// ---------- Model class ----------
	class Model
	{

	public:

		// ---- cdtor
			Model(); 
			virtual ~Model();

		
		// ---- mode
			enum
			{
				PARAM_GROUP_NULL     = 0,
				PARAM_GROUP_CAMERA   = 1, 
				PARAM_GROUP_BUMP_MAP = 2,
				PARAM_GROUP_FUNCTION = 3,
				PARAM_GROUP_ACQUIRE  = 4,
				PARAM_GROUP_LIGHTS   = 5,
				PARAM_GROUP_MEASURE  = 6,
			};


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


		// ---- framework methods
			INT Setup(vs_system::AppDependency*);
			INT SetupGraphics();
			INT Display(vs_system::PanelContext *, CHAR *);
			INT CleanupGraphics();
			INT Cleanup();


		// ---- pimp interface
			vs_system::AppDependency* GetAppDependency();


		// ---- project
			INT ProjectSetup();
			INT ProjectSetupGraphics();
			INT ProjectSetupDisplay();
			INT ProjectCleanupGraphics();


		// ---- shared methods
			#include "../../surface_shared/h/model_camera_methods_incl.h"
			#include "../../surface_shared/h/model_panel_methods_incl.h"


		// ---- Toolbar - required
			#include "../../surface_shared/hpp_toolbar/model_toolbar_interface.hpp"


		// ---- Tint - required
			#include "../hpp_tint/s3_tint_interface.hpp"


		// ---- Keypad - required
			#include "../../surface_shared/hpp_kpad/model_kpad_interface.hpp"


		// ---- img ( s3_model_img_process.hpp )


			// ---- framework
				static INT AcquireSetup();
				static INT AcquireCleanup();

			// ---- demo acquitision
				static INT AcquireDemoSetLights();
				static INT AcquireDemoSetMaterials();
				static INT AcquireDemoCalculateLuminaFields();
				static INT AcquireDemoCalculateHeightfield();

			// ---- acquitision
				static INT AcquireAnalyseLuminaFields();
				static INT AcquireFlatHeightField();
				static INT AcquireCalculateHeightField();

				static INT AcquireGetImageDimensions(
								UINT  *width,
								UINT  *height,
								UINT  *bytes
							);
				static INT AcquireStoreLuminaField(
								UINT   acquire_img_index,
								UINT   width,
								UINT   height,
								UINT   bytes,
								BYTE  *byte_data,
								INT    timestamp[]
							);
				static INT AcquireCopyLuminaFieldToAtlasPage(
								UINT acquire_img_index,
								BOOL view_analysis_img
							);


		// ---- parameter - static methods included for facade copy
			#include "vs/vs_library/header/vs_parameter_methods_static.h"


		// ---- sub-framework
			INT Model::SetupPanels(
					vs_system::AppDependency *,
					vs_system::AppView *
				);


		// ---- Keypad enumerations - required
			#include "../../surface_shared/hpp_kpad/model_kpad_enum.hpp"

	public:

		// ---- private implementation
			class  PI_Model; PI_Model *pi_model;


	};


// ----------------------------------------------------------------------


	class  Model::PI_Model;
	static Model::PI_Model *s_pi_model;


// ----------------------------------------------------------------------

	#include "../hpp/s3_model_img_analysis_class.hpp"

}
