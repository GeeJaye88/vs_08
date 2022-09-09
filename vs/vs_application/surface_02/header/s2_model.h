////////////////////////////////////////////////////////////////////////////////

// ---------- s2_model.h ----------
/*!
\file s3_model.h
\brief Interface for the Model class.
\author Gareth Edwards
*/

#pragma once

#include "../header/s2_include.h"


namespace surface_02
{

	// ---------- Model class ----------
	class Model
	{

	public:

		// ---- cdtor
			Model();
			virtual ~Model();

		
		// ---- parameter groups
			enum
			{
				PARAM_GROUP_NULL     = 0,
				PARAM_GROUP_CAMERA   = 1, 
				PARAM_GROUP_BUMP_MAP = 2,
				PARAM_GROUP_FUNCTION = 3,
				PARAM_GROUP_ACQUIRE  = 4,
				PARAM_GROUP_LIGHTS   = 5,
			};


		// ---- kamera transform update modes
			enum CameraUpdate
			{
				XYZ       = 0,
				VECTOR    = 1,
				PTZ       = 2,
				TRANSLATE = 3,
				ROTATE    = 4,
				TARGET    = 5,
			};


		// ---- target status
			enum CameraTarget
			{
				NONE = 999,
				NONE_RUNTIME
			};


		// ---- framework methods - "s2_model.cpp"
			INT Setup(vs_system::AppDependency*);
			INT SetupGraphics();
			INT Display(vs_system::PanelContext *, CHAR *);
			INT CleanupGraphics();
			INT Cleanup();


		// ---- get Setup injected application dependencies
			vs_system::AppDependency* GetAppDependency();


		// ---- project - "s2_model_project_framework.hpp"
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
			#include "../hpp_tint/s2_tint_interface.hpp"


		// ---- Keypad - required
			#include "../../surface_shared/hpp_kpad/model_kpad_interface.hpp"


		// ---- the acquisition engine - "s2_model_acquire.hpp"


			// ---- actual ----------------------------------------------------

			// ---- imaging framework
				static INT Acquire_SetupBuffers();
				static INT Acquire_CleanupBuffers();


			// ---- scene setup
				static INT Acquire_SetupLighting();
				static INT Acquire_SetupMaterials();


			// ---- demo ----------------------------------------------------

				//  - simulate camera source acquisition by
				//    generating source test heighfields
				static INT Acquire_GetDemoHeightfield();

				//  - simulate multi lumina field acquisition
				//    from source test heighfields
				static INT Acquire_CalculateDemoSurface(VsVector3 position[]);
				static INT Acquire_CalculateDemoLuminafields();


			// ---- actual ----------------------------------------------------

			// ---- lumina field analysis
				static INT Acquire_AnalyseLuminafields();

			// ---- (re)initialise heightfield
				static INT Acquire_InitialiseHeightfield();


			// ---- compute heightfield (aka "the big one")
				static INT Acquire_CalculateHeightfield();


			// ---- store lumina field
				static INT Acquire_CopyLuminafieldToImageBuffer(
								UINT    lf_index,
								UINT    lf_width,
								UINT    lf_height,
								UINT    lf_stride,
								UINT    lf_block,
								USHORT *lf_array
							);
				static INT Acquire_CopyLuminafieldToImageBuffer(
								UINT    lf_index,
								vs_system::UnsignedShortBuffer *bu
							);
				static INT Acquire_CopyRGBAToImageBuffer(
								UINT    lf_index,
								VsByteRGBA rgba
							);

				static INT Acquire_CopyImageBufferToAtlasPage(
								UINT    ib_index,
								BOOL    view_analysis
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

}
