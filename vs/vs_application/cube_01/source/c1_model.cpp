////////////////////////////////////////////////////////////////////////////////

// ---------- c1_model.cpp ----------
/*!
\file c1_model.cpp
\brief Implementation of the Model class
\author Gareth Edwards
\note
*/

#include "../header/c1_model.h"


using namespace cube_01;

// ---------- Private Implementation of PI_Model class ----------
class Model::PI_Model
{

public:

	// ---- cdtor
		PI_Model();
		~PI_Model();


	// ---- dependency pointers managed by VS
		vs_system::AppDependency  *app_dependency;
		vs_system::GfxInterface   *gfx;
		vs_system::SysContext     *sys_context;
		vs_system::SysOutput      *sys_output;


	// ---- managed locally
		vs_system::ObjConfig      *obj_config;
		vs_system::VsMatrixStack  *matrix_stack;
		vs_system::AppKamera      *kamera;


	// ---- camera - control

		INT   GetMsgCameraControlCount();
		VOID  IncMsgCameraControlCount();
		VOID  SetMsgCameraControlCount(INT count);

		UINT  GetCameraControlMode();
		VOID  SetCameraControlMode(UINT);

		UINT  GetCameraControlMessage();
		VOID  SetCameraControlMessage(UINT);

		BOOL  GetMsgCameraControlPersistantActive();
		VOID  SetMsgCameraControlPersistantActive(BOOL);

		BOOL  GetMsgCameraControlPersistantOn();
		VOID  SetMsgCameraControlPersistantOn(BOOL);


	// ---- camera (XYZ, VECTOR, PTZ or TARGET)
		UINT  kamera_control_mode                = vs_system::AppKamera::Mode::XYZ;
		UINT  camera_control_message_count       = 0;
		UINT  camera_control_message             = 0;
		BOOL  camera_control_persist_active      = 0;
		BOOL  camera_control_persist_switched_on = FALSE;
		UINT  target_object_id                   = CameraTarget::NONE;
		BOOL  camera_turntable_status            = FALSE;
		FLOAT camera_turntable_degrees           = 0;


	// ---- camera - target
		UINT  GetTargetObjectId();
		VOID  SetTargetObjectId(UINT);


	// ---- camera - turntable
		BOOL  GetTurntableStatus();
		VOID  SetTurntableStatus(BOOL);
		FLOAT GetTurntableDegrees();
		VOID  SetTurntableDegrees(FLOAT);


	// ---- time
		DOUBLE time_elapsed_ms_last = 0;
		UINT   fps = 0;


	// ---- scene objects
		vs_system::VsMaterial     *object;
		vs_system::VsLight        *star;


	// ---- scene properties
		FLOAT scene_dimension = 100;


	// ---- show flags
		BOOL  show_cube = FALSE;
		BOOL  show_mesh = TRUE;
		BOOL  show_surf = FALSE;


	// ---- elements
		INT   handle_dspl = 0;

		// ---- cube
			static const INT number_of_cube_elem = 12;
			INT   handle_cube_elem[number_of_cube_elem];

		// ---- mesh
			INT   handle_mesh_elem = NULL;

		// ---- surface
			static const INT number_of_surf_elem = 4;
			INT   handle_surf_elem[number_of_surf_elem];


	// ---- mesh properties
		FLOAT *mesh_surface_y_array = NULL;
		UINT   mesh_surface_y_stride = 0;
		FLOAT  mesh_surface_y_scalar = 1;


	// ---- surf properties
		VsVector3D surf_lights[4];
		double     surf_lights_radius;
		double     surf_lights_intensity;
		FLOAT     *surf_kandinsky_array[number_of_surf_elem];


	// ---- stuff
		INT  gfx_view_matrix_index = 1;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::PI_Model::PI_Model()
{

	// ---- dependency pointers managed by VS
		app_dependency = NULL;
		gfx            = NULL;
		sys_context    = NULL;
		sys_output     = NULL;


	// ---- managed locally
		kamera         = NULL;
		obj_config     = NULL;
		matrix_stack   = NULL;


	// ---- element arrays
		for (int i = 0; i < number_of_cube_elem; i++)
		{
			handle_cube_elem[i] = NULL;
		}
		for (int i = 0; i < number_of_surf_elem; i++)
		{
			handle_surf_elem[i] = NULL;
		}


	// ---- data arrays
		for (int i = 0; i < number_of_surf_elem; i++)
		{
			surf_kandinsky_array[i] = NULL;
		}

}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::PI_Model::~PI_Model()
{

	// ---- managed locally
		if ( kamera != NULL)         delete kamera;
		if ( obj_config != NULL )    delete obj_config;
		if ( matrix_stack != NULL )  delete matrix_stack;

}


////////////////////////////////////////////////////////////////////////////////


// ---------- camera - control

	INT  Model::PI_Model::GetMsgCameraControlCount()                   { return camera_control_message_count; }
	VOID Model::PI_Model::IncMsgCameraControlCount()                   { camera_control_message_count++; }
	VOID Model::PI_Model::SetMsgCameraControlCount(INT count)          { camera_control_message_count = count; }

	UINT Model::PI_Model::GetCameraControlMode()                       { return kamera_control_mode; }
	VOID Model::PI_Model::SetCameraControlMode(UINT ccm)               { kamera_control_mode = ccm;  }
	
	UINT Model::PI_Model::GetCameraControlMessage()                    { return camera_control_message; }
	VOID Model::PI_Model::SetCameraControlMessage(UINT m)              { camera_control_message = m;    }

	BOOL Model::PI_Model::GetMsgCameraControlPersistantActive()        { return camera_control_persist_active; }
	VOID Model::PI_Model::SetMsgCameraControlPersistantActive(BOOL p)  { camera_control_persist_active = p;    }

	BOOL Model::PI_Model::GetMsgCameraControlPersistantOn()            { return camera_control_persist_switched_on; }
	VOID Model::PI_Model::SetMsgCameraControlPersistantOn(BOOL s)      { camera_control_persist_switched_on = s;    }


// ---------- camera - target
	UINT Model::PI_Model::GetTargetObjectId()
		{
			return target_object_id;
		}
	VOID Model::PI_Model::SetTargetObjectId(UINT toi)
		{
			target_object_id = toi;
			if ( toi == CameraTarget::NONE )
			{
				;
			}
		}


// ---------- camera - turntable
	BOOL  Model::PI_Model::GetTurntableStatus()              { return camera_turntable_status;  }
	VOID  Model::PI_Model::SetTurntableStatus(BOOL tts)      { camera_turntable_status = tts;   }
	FLOAT Model::PI_Model::GetTurntableDegrees()             { return camera_turntable_degrees; }
	VOID  Model::PI_Model::SetTurntableDegrees(FLOAT ttd)    { camera_turntable_degrees = ttd;  }


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief allocate and construct private properties
\author Gareth Edwards
*/
Model::Model()
{
	pi_model = new PI_Model();
}


// ---------- Destructor ----------
/*!
\brief delete private properties
\author Gareth Edwards
*/
Model::~Model()
{
	if ( pi_model != NULL) delete pi_model;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = 0;


	// ---- store dependency pointers
		pi_model->app_dependency = app_dep;
		pi_model->gfx            = app_dep->GetGfxInterface();
		pi_model->sys_context    = app_dep->GetSysContext();
		pi_model->sys_output     = app_dep->GetSysOutput();


	// ---- allocate and construct object config
		pi_model->kamera        = new vs_system::AppKamera();
		pi_model->obj_config    = new vs_system::ObjConfig();
		pi_model->matrix_stack  = new vs_system::VsMatrixStack();


	// ---- mesh 
		result = SetupMeshYArray();


	// ---- surface 
		result = SetupSurfKArray();


	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief Setup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupGraphics()
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_model->gfx;
		INT hr = 0, result = 0;


	// ---- pi
		result = pi_model->gfx->SetupGraphics();


	// ---- scene
		result = SetupScene();
		result = SetupCamera();
		result = SetupCubeObjects();
		result = SetupMeshObjects();
		result = SetupSurfObjects();

	return result;
}



// ---------- SetupDisplay ----------
/*!
\brief SetupDisplay
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupDisplay()
{

	// ---- local
		INT result = 0;


	// ---- kamera target ?
		UINT target_sector_index = pi_model->GetTargetObjectId();
		if ( target_sector_index != CameraTarget::NONE )
		{
			VsVector3 v = { 0, 0, 0 };
			if ( target_sector_index > 0 )
			{
				FLOAT width = 10;
				FLOAT depth = 10;
				UINT target_sector_id = target_sector_index - 1;
				UINT row = target_sector_id / 3;
				UINT col = target_sector_id % 3;
				v.x = width / 3.0f * (FLOAT)col - width * 0.5f;
				v.z = depth / 3.0f * (FLOAT)row - depth * 0.5f;
			}
			pi_model->kamera->SetTargetPosition(&v, FALSE);
		}


	// ---- kamera update ?
		if (pi_model->kamera->HasBeenUpdated())
		{
			VsMatrix V;
			pi_model->kamera->GetViewMatrix(&V);
			pi_model->gfx->SetViewMatrix(1, &V);
			pi_model->kamera->SetHasBeenUpdated(FALSE);
		}


	// ---- turntable
		if (pi_model->GetTurntableStatus())
		{
			DOUBLE time_delta_ms = pi_model->sys_context->GetDouble(vs_system::SysContext::TIME_MS_DELTA);
			FLOAT ttd = pi_model->GetTurntableDegrees();
			ttd += (FLOAT)time_delta_ms / 100.0f;
			pi_model->SetTurntableDegrees(ttd);
		}


	// ---- push & turntable
		vs_system::VsMatrixStack *vms = pi_model->matrix_stack;
		vms->Initialise();

		pi_model->matrix_stack->Push();
		pi_model->matrix_stack->Rotate((FLOAT)-pi_model->GetTurntableDegrees(), 'Y');


	// ---- scene
		result = SetupCubeDisplay();
		result = SetupMeshDisplay();
		result = SetupSurfDisplay();


	// ---- pop
		pi_model->matrix_stack->Pop();

	return result;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display(
		vs_system::PanelContext *panel_context,
		CHAR *fred
	)
{

	// ---- local
		INT result = 0;
	

	// ---- get handle of current display list
		INT handle_display_list = GetPanelDisplayListHandle(panel_context);
		if ( handle_display_list == NULL ) return VS_ERROR;


	// ---- kamera
		result = SetPersistantCameraControl();


	// ---- set gfx_interface properties view matrix 0
		FLOAT *perspective = panel_context->GetPerspective();
		perspective[0] = 40;            // v fov
		perspective[1] = 0.1f;          // near
		perspective[2] = 100000.0f;     // far


	// ---- accelerate
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- setup and render view rectangle background
		result = gfx->SetMargin(panel_context->GetMargin());
		result = gfx->SetViewrect(panel_context->GetViewrect());
		result = gfx->RenderViewrect(panel_context, FALSE);


	// ---- setup and render view port AND selected display list
		result = gfx->SetBorder(panel_context->GetBorderRect());
		result = gfx->SetViewport(panel_context->GetViewport());
		result = gfx->RenderViewport(handle_display_list, panel_context);


	// ---- setup and render view rectangle foreground
		result = gfx->RenderViewrect(panel_context, TRUE);

	return result;
}


// ---------- GetDisplayListHandle ----------
/*!
\brief Setup a display list panel handle
\author Gareth Edwards
\return
*/
INT Model::GetPanelDisplayListHandle(
		vs_system::PanelContext *panel_context
	)
{

	// ---- local
		INT result = 0;


	// ---- get current index of display list
		UINT index_display_list;
		result = panel_context->GetDisplayListIndex(&index_display_list);


	// ---- get display list handle
		INT handle_display_list;
		result = panel_context->GetDisplayListHandle(index_display_list, &handle_display_list);


	// ---- no display list handle ? - then set one -
		if ( handle_display_list <= 0 )
		{

			// ---- get display list name
				CHAR name_display_list[VS_MAXCHARLEN];
				result = panel_context->GetDisplayListName(index_display_list, name_display_list);


			// ---- get display list handle, and id's...
				handle_display_list = pi_model->gfx->GetHandleDisplayList(name_display_list);
				UINT group_id = pi_model->gfx->GetElementGroupId(handle_display_list);
				UINT id = pi_model->gfx->GetElementGroupId(handle_display_list);


			// ---- get panel name
				CHAR name_panel[VS_MAXCHARNAME];
				result = panel_context->GetName(name_panel);
				if (name_panel == NULL) strcpy(name_panel, "UNKNOWN");


			// ---- report
				CHAR msg[VS_MAXCHARLEN];
				sprintf(msg, "+++ set panel %s display list %s handle [%d] and gid %d\n",
					name_panel,
						name_display_list,
						handle_display_list,
						group_id
					);
				OutputDebugString(msg);


			// ---- set
				result = panel_context->SetDisplayListHandle(
								index_display_list,
								handle_display_list
							);
				result = panel_context->SetGroupId(group_id);

		}

	return handle_display_list;
}


// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::CleanupGraphics()
{
	INT result = pi_model->gfx->CleanupGraphics();
	return result;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Cleanup()
{

	if ( pi_model->mesh_surface_y_array != NULL)
	{
		delete[] pi_model->mesh_surface_y_array;
		pi_model->mesh_surface_y_array = NULL;
	}

	for (INT i = 0; i < pi_model->number_of_surf_elem; i++)
	{
		if (pi_model->surf_kandinsky_array[i] != NULL)
		{
			delete[] pi_model->surf_kandinsky_array[i];
			pi_model->surf_kandinsky_array[i] = NULL;
		}
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupFrameset ----------
/*!
\brief Uppdate frameset
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::SetupFrameset(
		vs_system::AppDependency *app_dependency,
		vs_system::AppView *app_view
	)
{

	// ---- local
		INT result = VS_OK;


	// ---- accelerate
		vs_system::SysContext *sys_context = app_dependency->GetSysContext();


	// ---- setup

		// get client rectangle
		RECT client_rect;
		sys_context->GetClientRect(&client_rect);

		// set width and height
		FLOAT width = (FLOAT)(client_rect.right - client_rect.left);
		FLOAT height = (FLOAT)(client_rect.bottom - client_rect.top);
		FLOAT aspect = width / height;

		// get border width scalar
		FLOAT border_width = sys_context->GetFloat(sys_context->BORDER_WIDTH);


	// ---- Y

		// set y proportional
		FLOAT y_bar_prop   = 0.050f + 0.02f;
		FLOAT y_space_prop = 0.004f + 0.001f;
		FLOAT y_border     = y_space_prop * border_width;

		// set y bounds
		FLOAT y_top    = y_border;
		FLOAT y_bottom = 1.0f - (y_space_prop* border_width);


	// ---- X

		// set x proportional
		FLOAT x_space_prop = y_space_prop / aspect;
		FLOAT x_border     = x_space_prop * border_width;

		// set x bounds
		FLOAT x_left  = x_border;
		FLOAT x_right = 1 - x_left;


	// ---- setup panel lut

		// set # of panes
		const int lut_number = 1;

		// panel rectangle
		struct pane_rect {
			CHAR *name;
			VsFloatRectangle fr;
		};


	// ---- initialise panel rects
		pane_rect pane_rect_lut[lut_number] =
		{
			{ "3D", x_left, y_top, x_right, y_bottom }
		};


	// ---- find and assign panel rects
		// note: defaults to lut[0] if no match
		CHAR name[VS_MAXCHARNAME];
		vs_system::AppFrame *frame = app_view->GetFirstFrame();
		while (frame)
		{
			vs_system::AppPanel *panel = frame->GetFirstPanel();
			while (panel)
			{
				// set rect
				vs_system::PanelContext *panel_context = panel->GetPanelContext();
				VsFloatRectangle *vr = panel_context->GetViewrect();
				*vr = pane_rect_lut[0].fr;
				INT result = panel_context->GetName(name);
				for (UINT i = 0; i<lut_number; i++)
				{
					if (
							pane_rect_lut[i].name != 0 &&
								!strcmp(pane_rect_lut[i].name, name)
						)
					{
						*vr = pane_rect_lut[i].fr;
						break;
					}
				}
				panel = panel->GetNext();
			}
			frame = frame->GetNext();
		}


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


#include "../hpp/c1_camera.hpp"
#include "../hpp/c1_setup.hpp"
#include "../hpp/c1_cube.hpp"
#include "../hpp/c1_mesh.hpp"
#include "../hpp/c1_surf.hpp"
