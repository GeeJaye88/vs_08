////////////////////////////////////////////////////////////////////////////////

// ---------- e1_model.cpp ----------
/*!
\file e1_model.cpp
\brief Implementation of the Model class
\author Gareth Edwards
*/

#include "../header/e1_model.h"

using namespace examples_01;


// #define CUBE_TEST


// ---------- Private Implementation of PI_Model class ----------
class Model::PI_Model
{

public:

	// ---- cdtor
		PI_Model();
		~PI_Model();


	// ---- cubes
		INT SetupCubeDisplay();


	// ---- planets
		INT InitSystemObjectsGfx(SystemObject *);
		INT InitSystemObjects(SystemObject *);
		INT SetupSystemObjectsForDisplay(SystemObject *);


	// ---- dependency pointers managed by VS
		vs_system::AppDependency  *app_dependency;
		vs_system::GfxInterface   *gfx;
		vs_system::SysContext     *sys_context;
		vs_system::SysOutput      *sys_output;


	// ---- managed locally
		vs_system::ObjConfig      *obj_config;
		vs_system::VsMatrixStack  *matrix_stack;
		SystemObject              *system_object;
		SystemParam               *system_param;


	// ---- scene
		vs_system::AppCamera      *my_camera;
		vs_system::VsMaterial     *object;
		vs_system::VsLight        *star;


	// ---- flags
		BOOL runtime = TRUE;
		vs_system::AppCamera::AppCameraType camera_type = vs_system::AppCamera::LANDOBJECT;


	// ---- camera
		UINT view_matrix_index = 1;


	// ---- default camera 
		D3DXVECTOR3 default_camera_position  = { 0,  700, -1250 };
		D3DXVECTOR3 default_camera_direction = { 0, -0.6f, 0.8f };


	// ---- target camera 
		D3DXVECTOR3 target_camera_position  = {
				default_camera_position.x,
				default_camera_position.y,
				default_camera_position.z
			};

		D3DXVECTOR3 target_camera_direction = {
				default_camera_direction.x,
				default_camera_direction.y,
				default_camera_direction.z
			};


	// ---- target planet
		UINT  target_planet_index       = 0;
		FLOAT target_planet_dist_scalar = 1.0f;
		D3DXVECTOR3 target_planet_position = { 0,    0,     0 };
		CHAR *target_planet_name[11] = 
		{
			"",
			"Sun",
			"Mercury",
			"Venus",
			"Earth",
			"Mars",
			"Jupiter",
			"Saturn",
			"Uranus",
			"Neptune",
			"Pluto"
		};


	// ---- objects
		INT recursive_depth_count;
		INT object_index;
		INT mtl_index;

		INT handle_sphere_dsplst;
		INT handle_sphere[SystemParam::MAX_SYS_OBJECT];

		INT handle_rings_dsplst;
		INT handle_rings[SystemParam::MAX_SYS_OBJECT];

		INT handle_orbit_dsplst;
		INT handle_orbit[SystemParam::MAX_SYS_OBJECT];

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
		obj_config     = NULL;
		matrix_stack   = NULL;
		system_object  = NULL;
		system_param   = NULL;


	// ---- init objects
		recursive_depth_count = 0;
		object_index = 0;
		mtl_index = 0;

		handle_sphere_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_sphere[i] = 0;

		handle_rings_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_rings[i] = 0;

		handle_orbit_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_orbit[i] = 0;

}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::PI_Model::~PI_Model()
{

	// ---- managed locally
		if ( obj_config != NULL )    delete obj_config;
		if ( system_object != NULL ) delete system_object;
		if ( system_param != NULL )  delete system_param;
		if ( matrix_stack != NULL )  delete matrix_stack;

}


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

	// ---- store dependency pointers
		pi_model->app_dependency = app_dep;
		pi_model->gfx            = app_dep->GetGfxInterface();
		pi_model->sys_context    = app_dep->GetSysContext();
		pi_model->sys_output     = app_dep->GetSysOutput();


	// ---- allocate and construct object config
		pi_model->obj_config    = new vs_system::ObjConfig();
		pi_model->matrix_stack  = new vs_system::VsMatrixStack();
		pi_model->system_object = new SystemObject(0,"Solar System"); 
		pi_model->system_param  = new SystemParam();
		pi_model->my_camera     = new vs_system::AppCamera(pi_model->camera_type);


	// ---- read system data
		SystemIO io;
		INT result = io.Read(pi_model->system_object);


	// ---- target
		pi_model->target_planet_dist_scalar = pi_model->system_param->GetTargetDistScalar();


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
		INT result = 0;


	// ---- setup
		result = pi_model->gfx->SetupGraphics();


	// ---- planets
		#if defined(CUBE_TEST)
			result = InitCubeScene();
			hr = InitCubeObjects();
		#else

			result = InitPlanetsScene();
			result = InitSystemObjects();

			pi_model->target_planet_dist_scalar = 
				pi_model->system_param->GetTargetDistScalar();

		#endif

	return result;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display()
{

	// ---- local
		INT result = 0;


	// ---- planets
		#if defined(CUBE_TEST)
			result = SetupCubeDisplay();
		#else
			result = SetupSystemObjectsForDisplay();
		#endif

	return result;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display(
		vs_system::PanelContext *panel_gfx_context,
		CHAR *fred
	)
{

	// ---- local
		INT result = 0;
	

	// ---- get handle of current display list
		INT handle_display_list = GetPanelDisplayListHandle(panel_gfx_context);
		if ( handle_display_list == NULL ) return VS_ERROR;


	// ---- accelerate
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- camera
		FLOAT *perspective = panel_gfx_context->GetPerspective();
		perspective[0] = 40;        // v fov
		perspective[1] = 0.5f;      // near
		perspective[2] = 20000.0f;  // far


	// ---- setup and render view rectangle background
		result = gfx->SetMargin(panel_gfx_context->GetMargin());
		result = gfx->SetViewrect(panel_gfx_context->GetViewrect());
		result = gfx->RenderViewrect(panel_gfx_context, FALSE);


	// ---- setup and render view port AND selected display list
		result = gfx->SetBorder(panel_gfx_context->GetBorder());
		result = gfx->SetViewport(panel_gfx_context->GetViewport());
		result = gfx->RenderViewport(handle_display_list, panel_gfx_context);


	// ---- setup and render view rectangle foreground
		result = gfx->RenderViewrect(panel_gfx_context, TRUE);

	return result;
}


// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::CleanupGraphics()
{

	INT hr = pi_model->gfx->CleanupGraphics();

	return hr;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT Model::InitPlanetsParameters()
{
	
	// ---- local  
		INT  result = VS_OK;
		

	// ---- seed
		srand(2001);


	// ---- set system param
		result = pi_model->system_param->SetScale(0.00001f);
		result = pi_model->system_param->SetTimeScalar(1);
		result = pi_model->system_param->SetDaylenScalar(0.01f);
		result = pi_model->system_param->SetMsOffset(1000000);


	// ---- set scaling
		UINT gfx_mode = pi_model->system_param->GetGfxMode();
		FLOAT scale = 1;
		switch ( gfx_mode )
		{
			case SystemObject::NORMAL       : scale =   1.0f; break;
			case SystemObject::LARGE        : scale =  50.0f; break;
			case SystemObject::ILLUSTRATION : scale = 100.0f; break;
			default:
				break;
		}
		result = pi_model->system_param->SetObjectScale(scale);

	return VS_OK;
}


// ========================================================================== //

INT Model::InitPlanetsScene()
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_model->gfx;
		INT hr = 0;


	// ---- fog
		VS_Colour col = { 0.0f, 0.0f, 0.5f, 1.0f };
		vs_system::VsFog fog;
		hr = fog.SetColour(col);
		hr = fog.SetType(vs_system::VsFogType::VS_FOG_LINEAR);
		hr = fog.SetStart(10000);
		hr = fog.SetEnd(20000);
		hr = gfx->SetFog(&fog);
		hr = gfx->SetFogEnabled(TRUE);


	// ---- ambient light
		VS_Colour amb_col = { 0.1f, 0.1f, 0.1f, 1.0f };
		vs_system::VsAmbient amb;
		hr = amb.SetColour(amb_col);
		hr = gfx->SetAmbient(&amb);


	// ---- sun light
		VS_Colour dif = { 0.5f, 0.5f, 0.5f, 1.0f };
		VS_Vector pos = { 0.0f, 0.0f, 0.0f };
		vs_system::VsLight sun;
		hr = sun.SetType(vs_system::VS_LIGHT_POINT);
		hr = sun.SetDiffuse(dif);
		hr = sun.SetPosition(pos);
		hr = sun.SetRange(30000.0f); 
		hr = sun.SetAttLinear(0.0015f);
		hr = gfx->SetLight(0, &sun);
		hr = gfx->SetLightEnabled(0, TRUE);


	// ---- view matrix 1
		pi_model->my_camera->SetLook(&pi_model->target_camera_position, &pi_model->target_camera_direction);

		D3DXMATRIX V;
		pi_model->my_camera->GetViewMatrix(&V);
		pi_model->gfx->SetViewMatrix(1, &V);

	return VS_OK;
}

// ========================================================================== //

INT Model::InitSystemObjects()
{

	// ---- local
		INT hr = VS_OK;


	// ---- create display list
		if ( pi_model->runtime )
		{
			vs_system::GfxInterface *gfx = pi_model->gfx;
			hr = pi_model->handle_sphere_dsplst = gfx->AddDisplayList("My_3D_Sphere_01", 1, 1);
		}

		
	// ---- create objects
		pi_model->recursive_depth_count = 0;
		hr = InitPlanetsParameters();
		hr = pi_model->InitSystemObjectsGfx(pi_model->system_object);
		hr = pi_model->InitSystemObjects(pi_model->system_object);


	// ---- done
		pi_model->runtime = FALSE;

	return VS_OK;
}


// ========================================================================== //

INT Model::PI_Model::InitSystemObjectsGfx(SystemObject *sys_obj)
{

	// ---- planet
		struct gfx_planet_item
		{
			CHAR  name[64];
			FLOAT distance   = 0;
			FLOAT radius_min = 0;
			FLOAT radius_max = 0;
			FLOAT diameter   = 0;
		};


	// ----list of planets
		gfx_planet_item gfx_planet_list[] =
		{
			{ "",			0,       0,       0,       0,     },
			{ "Sun",		0,       0,       0,       1.000f },
			{ "Mercury",	0.060f,  0,       0,       0.200f },
			{ "Venus",		0.100f,  0,       0,       0.300f },
			{ "Earth",		0.140f,  0,       0.200f,  0.300f },
			{ "Mars",	    0.180f,  0,       0.200f,  0.250f },
			{ "Ceres 1",    0.280f,  0,       0,       0.050f },
			{ "Pallas 2",   0.260f,  0,       0,       0.050f },
			{ "Juno 3",		0.240f,  0,       0,       0.050f },
			{ "Vesta 4",    0.220f,  0,       0,       0.050f },
			{ "Hygiea 10",  0.300f,  0,       0,       0.050f },
			{ "MB Zone 1",  0,       0.220f,  0.240f,  0.005f },
			{ "MB Zone 2",  0,       0.250f,  0.270f,  0.005f },
			{ "MB Zone 3",  0,       0.280f,  0.300f,  0.005f },
			{ "Jupiter",	0.400f,  0.500f,  0.600f,  0.700f },
			{ "Saturn",		0.600f,  0.350f,  0.600f,  0.600f },
			{ "Uranus",		0.800f,  0.400f,  0.500f,  0.500f },
			{ "Neptune",	0.950f,  0.400f,  0.500f,  0.400f },
			{ "Pluto",		1.100f,  0,       0,       0.100f },
		};

		 
	// ---- calculate gfx info
		FLOAT ten_to_p6 = 1000000;
		FLOAT ten_to_p3 =    1000;
		INT result = 0;
		CHAR *name = sys_obj->GetName();
		INT num_tp_items = sizeof(gfx_planet_list) / sizeof(gfx_planet_item);
		for (INT i = 0; i < num_tp_items; i++)
		{
			if ( !strcmp(name, gfx_planet_list[i].name) )
			{

				// ---- distance
					result = sys_obj->Set("GfxDis",    gfx_planet_list[i].distance * ten_to_p3);


				// ---- diameter
					UINT type = ((INT)(sys_obj->Get("Type")));
					if ( type == SystemObject::AstRing )
					{
						result = sys_obj->Set("GfxRadMin", gfx_planet_list[i].radius_min * ten_to_p3);
						result = sys_obj->Set("GfxRadMax", gfx_planet_list[i].radius_max * ten_to_p3);
						result = sys_obj->Set("GfxDia",    gfx_planet_list[i].diameter * ten_to_p6);
					}
					else if (
							type == SystemObject::Star ||
							type == SystemObject::Planet ||
							type == SystemObject::GasGiant ||
							type == SystemObject::Dwarf
						)
					{
						result = sys_obj->Set("GfxDia", gfx_planet_list[i].diameter * ten_to_p6);
					}


				// ---- ring system
					BOOL ring_system = (BOOL)((INT)(sys_obj->Get("Ring")));
					if ( ring_system )
					{
						FLOAT ring_radius_min = gfx_planet_list[i].radius_min;
						FLOAT ring_radius_max = gfx_planet_list[i].radius_max;
						result = sys_obj->Set("GfxRadMin", ring_radius_min);
						result = sys_obj->Set("GfxRadMax", ring_radius_max);
					}


				// ---- moons
					UINT num_moons = ((INT)(sys_obj->Get("Moon")));
					if ( num_moons > 0 )
					{
						FLOAT radius = gfx_planet_list[i].radius_max;
						FLOAT incr = 0.1f;
						SystemObject *moon_obj = sys_obj->GetFirst();
						while ( moon_obj )
						{
							radius += incr;
							result = moon_obj->Set("GfxDis", radius);
							result = moon_obj->Set("GfxDia", moon_obj->Get("Diam") * 20);
							moon_obj = moon_obj->GetNext();
						}
					}

			}
		}


	// ---- process child objects
		SystemObject *s = sys_obj->GetFirst();
		while (s)
		{
			INT result = InitSystemObjectsGfx(s);
			s = s->GetNext();
		}


	return VS_OK;
}


// ========================================================================== //

INT Model::PI_Model::InitSystemObjects(SystemObject *sys_obj)
{

	// ---- local
		enum
		{
			HIGH_COMPLEXITY   = 0,
			MEDIUM_COMPLEXITY = 1,
			LOW_COMPLEXITY    = 2,
			BLOCK_COMPLEXITY  = 3
		};

		struct object_complexity
		{
			UINT cels;
			UINT rows;
			UINT lines;
		};

		object_complexity complexity[4] =
		{
			{ 40, 20, 720 },
			{ 20, 10, 360 },
			{ 10,  5, 180 },
			{  3,  2,   1 },
		};


	// ---- local
		INT hr = VS_OK;


	// ---- set illustration mode
		UINT gfx_mode = system_param->GetGfxMode();
		BOOL illustration_mode = FALSE;
		switch ( gfx_mode )
		{
			case SystemObject::ILLUSTRATION :
				illustration_mode = TRUE;
				break;
			case SystemObject::NORMAL:
			case SystemObject::LARGE:
			default:
				break;
		}


	// ---- increment
		recursive_depth_count++;


	// ---- scalar
		UINT   type = (UINT)sys_obj->Get("Type");
		DOUBLE object_diameter_scalar = 1;
		switch ( type )
		{
			case SystemObject::Type::UnKnown:
				object_diameter_scalar = 0;
				break;
			case SystemObject::Type::Asteroid:
				object_diameter_scalar = system_param->GetObjectScale() * 60;
				break;
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
				object_diameter_scalar = system_param->GetObjectScale() * 10;
				break;
			default:
				object_diameter_scalar = system_param->GetObjectScale();
				break;
		}


	// ---- density
		UINT geometry_density = HIGH_COMPLEXITY;
		switch ( type )
		{
			default:
			case SystemObject::Type::UnKnown:
			case SystemObject::Type::Star:
			case SystemObject::Type::GasGiant:
				break;
			case SystemObject::Type::Planet:
			case SystemObject::Type::Dwarf:
			case SystemObject::Type::Asteroid:
				geometry_density = MEDIUM_COMPLEXITY;
				break;
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
				geometry_density = BLOCK_COMPLEXITY;
				break;
			case SystemObject::Type::Moon:
				geometry_density = LOW_COMPLEXITY;
				break;
		}


	// ---- object geometry
		BOOL create_object_geometry = TRUE;
		switch ( type )
		{
			case SystemObject::Type::UnKnown:
				create_object_geometry = FALSE;
				break;
			default:
				break;
		}


	// ---- orbit geometry
		BOOL create_orbit_geometry = TRUE;
		switch ( type )
		{
			case SystemObject::Type::UnKnown:
			case SystemObject::Type::Star:
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
				create_orbit_geometry = FALSE;
				break;
			default:
				break;
		}


	// ---- parent
		SystemObject *parent_sys_obj = sys_obj->GetParent();
		UINT          parent_type = parent_sys_obj == NULL ? 0 : (UINT)parent_sys_obj->Get("Type");
		DOUBLE        parent_diameter_scalar = 1;
		switch ( parent_type )
		{
			case SystemObject::Type::UnKnown:
				parent_diameter_scalar = 0;
				break;
			default:
				parent_diameter_scalar = system_param->GetObjectScale();
				break;
		}


	// ---- who
		CHAR *name = sys_obj->GetName();


	// ---- minimum size == scaler
		FLOAT system_scalar = (FLOAT)system_param->GetGlobalScale();
		FLOAT minimum_diameter = system_scalar;
		FLOAT maximum_diameter = 100000.0f;


	// ---- object diameter
		//
		// note: can't do this on matrix stack without adding non-scale channel
		//
		FLOAT object_diameter =
				(FLOAT)(
					( illustration_mode ? sys_obj->Get("GfxDia" ) : sys_obj->Get("Diam") ) *
								object_diameter_scalar * system_scalar );

			object_diameter =
					object_diameter < minimum_diameter ? minimum_diameter :
						(object_diameter > maximum_diameter ? maximum_diameter : object_diameter );


	// ---- parent diameter
		FLOAT parent_diameter =
				parent_sys_obj == NULL ? 0 :
					(FLOAT)(
						( illustration_mode ? parent_sys_obj->Get("GfxDia") : parent_sys_obj->Get("Diam") ) *
								parent_diameter_scalar * system_scalar );

			parent_diameter =
					parent_diameter < minimum_diameter ? minimum_diameter :
						(parent_diameter > maximum_diameter ? maximum_diameter : parent_diameter);



	// ---- create object geometry
		if ( create_object_geometry )
		{

			// ---- index
				if ( runtime )
				{
					;
				}
				else
				{
					object_index = sys_obj->GetIndex();
				}


			// ---- create sphere object
				if ( runtime )
				{
					if ( type == SystemObject::Type::AstRing )
					{
						handle_sphere[object_index] = gfx->AddElement(
								handle_sphere_dsplst,
								sys_obj->GetName(),  
								1,
								1,
								obj_config->Callback_Asteroid_Configure
							);
					}
					else
					{
						handle_sphere[object_index] = gfx->AddElement(
								handle_sphere_dsplst,
								sys_obj->GetName(),
								1,
								1,
								obj_config->Callback_Sphere_Configure
							);
					}
				}
				else
				{
					gfx->SetElementConfigureBitmask(
							handle_sphere[object_index],
							VS_ELM_SETUP_GEOMETRY
						);
				}


			// ---- context
				vs_system::ElmContext *gfx_context = NULL;
				hr = gfx->GetElementContext(handle_sphere[object_index], &gfx_context);


			// ---- force geometry setup
				gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
				gfx_context->SetConfigured(FALSE);


			// ---- init sphere parameters
				FLOAT *param = gfx_context->GetFloatParameter();
				param[vs_system::ObjConfig::SPHERE_CEL_COLS] = (FLOAT)complexity[geometry_density].cels;
				param[vs_system::ObjConfig::SPHERE_CEL_ROWS] = (FLOAT)complexity[geometry_density].rows;
				param[vs_system::ObjConfig::SPHERE_TYPE]     = 2;
				param[vs_system::ObjConfig::SPHERE_AP_REF]   = sys_obj->Get("GfxPag") + 100;
				param[vs_system::ObjConfig::SPHERE_AP_SECT]  = sys_obj->Get("GfxSec");
				param[vs_system::ObjConfig::SPHERE_RADIUS]   = object_diameter / 20;
				param[vs_system::ObjConfig::SPHERE_RED]      = sys_obj->Get("GfxRed");
				param[vs_system::ObjConfig::SPHERE_GREEN]    = sys_obj->Get("GfxGre");
				param[vs_system::ObjConfig::SPHERE_BLUE]     = sys_obj->Get("GfxBlu");
				param[vs_system::ObjConfig::SPHERE_LAT_MIN]  = -90;
				param[vs_system::ObjConfig::SPHERE_LAT_MAX]  =  90;
				param[vs_system::ObjConfig::SPHERE_RAD_MIN]  =
						illustration_mode ?
							sys_obj->Get("GfxRadMin") :
								sys_obj->Get("RadMin");
				param[vs_system::ObjConfig::SPHERE_RAD_MAX]  =
						illustration_mode ?
							sys_obj->Get("GfxRadMax") :
								sys_obj->Get("RadMax");


			// ---- init sphere asteroid belt parameters
				if (type == SystemObject::Type::AstRing)
				{
					param[vs_system::ObjConfig::SPHERE_RADIUS]  =  object_diameter / 20;
					param[vs_system::ObjConfig::SPHERE_LAT_MIN] =  object_diameter / 5;
					param[vs_system::ObjConfig::SPHERE_LAT_MAX] = -object_diameter / 5;
					param[vs_system::ObjConfig::SPHERE_NUMBER]  =  60;
				}

			// ---- lighting ON
				hr = gfx_context->SetLit(TRUE);

	
			// ---- material colours
				VS_Colour obj_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
				VS_Colour obj_dif = { 1.0f, 1.0f, 1.0f, 1.0f };


			// ---- materials
				vs_system::VsMaterial obj;
				hr = obj.SetAmbient(obj_amb);
				hr = obj.SetDiffuse(obj_dif);


			// ---- sun
				if ( !strcmp(name, "Sun" ) )
				{

					// ---- sun light
						hr = gfx_context->SetLit(FALSE);
						VS_Colour obj_emi = { 1.0f, 1.0f, 0.0f, 1.0f };
						hr = obj.SetAmbient(obj_emi);
						hr = obj.SetDiffuse(obj_emi);
						hr = obj.SetEmissive(obj_emi);

					// ---- sun bright
						hr = gfx_context->SetShaded(TRUE);
						hr = gfx_context->SetShader(0);
						FLOAT param[8];
						param[0] = 2;
						param[1] = 0;
						param[2] = 0;
						gfx->SetGfxShaderParam(0, param);

				}


			// ---- set gfx materials - VS_MAX_MATERIALS
				if ( runtime )
				{
					hr = gfx->SetMaterial(mtl_index, &obj);
					hr = gfx->SetMaterial(mtl_index);
					hr = gfx_context->SetMaterial(mtl_index);
					mtl_index++;
				}


			// ---- set gfx view matrix
				hr = gfx_context->SetViewMatrix(view_matrix_index);


			// ---- create orbit geometry
				if ( parent_sys_obj != NULL && create_orbit_geometry )
				{

					// ---- create orbit object
						if ( runtime  )
						{
							handle_orbit[object_index] = gfx->AddElement(
												handle_sphere_dsplst,
												sys_obj->GetName(),
												1,
												1,
												obj_config->Callback_Orbit_Configure
											);
						}
						else
						{
							gfx->SetElementConfigureBitmask(
								handle_orbit[object_index],
									VS_ELM_SETUP_GEOMETRY
								);
						}


					// ---- orbit distance translation

						DOUBLE orbit_distance_km_10p6 =
								(DOUBLE)( illustration_mode ?
									sys_obj->Get("GfxDis") :
										sys_obj->Get("Dist") );

							if ( (UINT)sys_obj->Get("Type") == SystemObject::Type::Moon )
							{
								orbit_distance_km_10p6 *=
									system_param->GetObjectScale();

								//orbit_distance_km_10p6 += parent_diameter / 2;
							}


					// ---- context
						vs_system::ElmContext *gfx_context = NULL;
						hr = gfx->GetElementContext(handle_orbit[object_index], &gfx_context);


					/// ------ hmmmmmmmmmmmmmmmmmmmmm......
						gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
						gfx_context->SetConfigured(FALSE);


					// ---- init orbit geometry parameters
						FLOAT *param = gfx_context->GetFloatParameter();
						param[vs_system::ObjConfig::ORBIT_LINES]   = (FLOAT)complexity[geometry_density].lines;
						param[vs_system::ObjConfig::ORBIT_RADIUS]  = (FLOAT)orbit_distance_km_10p6;
						param[vs_system::ObjConfig::ORBIT_RED]     = sys_obj->Get("GfxRed");
						param[vs_system::ObjConfig::ORBIT_GREEN]   = sys_obj->Get("GfxGre");
						param[vs_system::ObjConfig::ORBIT_BLUE]    = sys_obj->Get("GfxBlu");
						param[vs_system::ObjConfig::ORBIT_X]       = 0;
						param[vs_system::ObjConfig::ORBIT_Y]       = 0;
						param[vs_system::ObjConfig::ORBIT_Z]       = 0;
						param[vs_system::ObjConfig::ORBIT_DEGREES] = type == SystemObject::Type::Asteroid ? 135.0f : 270.0f;

					// ---- lighting OFF
						hr = gfx_context->SetLit(FALSE);


					// ---- set gfx view matrix
						hr = gfx_context->SetViewMatrix(view_matrix_index);

				}


				// ---- rings ?
				BOOL rings = (BOOL)sys_obj->Get("Ring");
				if ( rings )
				{

					// ---- create rings object
						if ( runtime )
						{
							handle_rings[object_index] = gfx->AddElement(
								handle_sphere_dsplst,
								sys_obj->GetName(),
								1,
								1,
								obj_config->Callback_Rings_Configure
							);
						}
						else
						{
							gfx->SetElementConfigureBitmask(
								handle_rings[object_index],
									VS_ELM_SETUP_GEOMETRY
								);
						}


					// ---- context
						vs_system::ElmContext *gfx_context = NULL;
						hr = gfx->GetElementContext(handle_rings[object_index], &gfx_context);


					/// ------ hmmmmmmmmmmmmmmmmmmmmm......
						gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
						gfx_context->SetConfigured(FALSE);


					// ---- init rings geometry parameters
						FLOAT object_radius = object_diameter / 2;
						FLOAT *param = gfx_context->GetFloatParameter();
						param[vs_system::ObjConfig::SPHERE_CEL_COLS] = 72;
						param[vs_system::ObjConfig::SPHERE_CEL_ROWS] = 2;
						param[vs_system::ObjConfig::SPHERE_TYPE]     = 2;
						param[vs_system::ObjConfig::SPHERE_RADIUS]   = object_radius;
						param[vs_system::ObjConfig::SPHERE_AP_REF]   = sys_obj->Get("GfxPag") + 100;
						param[vs_system::ObjConfig::SPHERE_AP_SECT]  = sys_obj->Get("GfxSec");
						param[vs_system::ObjConfig::SPHERE_RED]      = sys_obj->Get("GfxRed");
						param[vs_system::ObjConfig::SPHERE_GREEN]    = sys_obj->Get("GfxGre");
						param[vs_system::ObjConfig::SPHERE_BLUE]     = sys_obj->Get("GfxBlu");

						param[vs_system::ObjConfig::SPHERE_RAD_MIN]  =
								( illustration_mode ?
										sys_obj->Get("GfxRadMin") :
											sys_obj->Get("RadMin") ) *
												(FLOAT)object_diameter_scalar;
						param[vs_system::ObjConfig::SPHERE_RAD_MAX]  =
								( illustration_mode ?
									sys_obj->Get("GfxRadMax") :
										sys_obj->Get("RadMax") ) *
												(FLOAT)object_diameter_scalar;

					// ---- lighting OFF
						hr = gfx_context->SetLit(FALSE);


					// ---- set gfx view matrix
						hr = gfx_context->SetViewMatrix(view_matrix_index);


						hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_NONE);

				}

		}


	// ---- increment object handle index
		if ( runtime )
		{
			sys_obj->SetIndex(object_index);
			object_index++;
		}

		 
	// ---- process child objects
		SystemObject *s = sys_obj->GetFirst();
		while ( s )
		{
			hr = InitSystemObjects(s);
			s = s->GetNext();
		}


	// ---- decrement
		recursive_depth_count--;

	return VS_OK;
}

// ========================================================================== //

INT Model::SetupSystemObjectsForDisplay()
{

	pi_model->recursive_depth_count = 0;
	pi_model->matrix_stack->Initialise();
	pi_model->SetupSystemObjectsForDisplay(pi_model->system_object);


	if ( pi_model->target_planet_index != 0 )
	{
		D3DXVECTOR3 pos;
		pos.x = pi_model->target_planet_position.x;
		pos.y = (  75 * pi_model->target_planet_dist_scalar) + pi_model->target_planet_position.y;
		pos.z = (-100 * pi_model->target_planet_dist_scalar) + pi_model->target_planet_position.z;
		pi_model->my_camera->SetPosition(&pos);

		D3DXMATRIX V;
		pi_model->my_camera->GetViewMatrix(&V);
		pi_model->gfx->SetViewMatrix(1, &V);
	}


	return VS_OK;
}


// ========================================================================== //

INT Model::PI_Model::SetupSystemObjectsForDisplay(SystemObject *sys_obj)
{

	// ---- local
		INT hr = VS_OK;


	// ---- set illustration mode
		UINT gfx_mode = system_param->GetGfxMode();
		BOOL illustration_mode = FALSE;
		switch ( gfx_mode )
		{
			case SystemObject::ILLUSTRATION :
				illustration_mode = TRUE;
				break;
			case SystemObject::NORMAL:
			case SystemObject::LARGE:
			default:
				break;
		}


	// ---- parent
		SystemObject *parent_sys_obj = sys_obj->GetParent();
		UINT          parent_type = parent_sys_obj == NULL ? 0 : (UINT)parent_sys_obj->Get("Type");
		DOUBLE        parent_diameter_scalar = 1;
		switch ( parent_type )
		{
			default:
			case SystemObject::Type::UnKnown:
				parent_diameter_scalar = 0;
				break;

			case SystemObject::Type::Star:
			case SystemObject::Type::Planet:
			case SystemObject::Type::GasGiant:
			case SystemObject::Type::Dwarf:
			case SystemObject::Type::Asteroid:
				parent_diameter_scalar = system_param->GetObjectScale();
				break;

			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
			case SystemObject::Type::Moon:
				break;
		}


	// ---- time - with pause ON/OFF
		DOUBLE system_time_elapsed = sys_context->GetDouble(vs_system::SysContext::TIME_ELAPSED);
		DOUBLE pause_time_start   = system_param->GetPauseTimeStart();
		DOUBLE pause_time_elapsed = system_param->GetPauseTimeElapsed();
		if ( system_param->GetPauseMode() == 1 )
		{
			system_time_elapsed = pause_time_start - pause_time_elapsed;
		}
		else
		{
			system_time_elapsed -= pause_time_elapsed;
		}

		DOUBLE hours_elapsed =
			system_time_elapsed *
				system_param->GetTimeScalar() +
					system_param->GetMsOffset();


	// ---- note: 1 ms == 1 hour (that's 3,600,000 times faster!!!)


	// ---- orbit rotation
		DOUBLE orbit_period_hours =
				(DOUBLE)( illustration_mode ? sys_obj->Get("GfxOrbPer") : sys_obj->Get("OrbPer") ) * 24;
		DOUBLE orbit_start_hours = 
				(DOUBLE)( illustration_mode ? sys_obj->Get("GfxOrbSta") : sys_obj->Get("OrbSta") ) * 24;
		DOUBLE orbit_degrees = fabs(orbit_period_hours) < 0.000001 ? 0 :
				( hours_elapsed + orbit_start_hours ) / orbit_period_hours * 360;


	// ---- orbit distance
		DOUBLE orbit_distance_km_10p6 =
			(DOUBLE)( illustration_mode ? sys_obj->Get("GfxDis") : sys_obj->Get("Dist") );

			if ( (UINT)sys_obj->Get("Type") == SystemObject::Type::Moon )
			{
				DOUBLE parent_diameter =
						parent_sys_obj == NULL ? 0 :
							(DOUBLE)( illustration_mode ? parent_sys_obj->Get("GfxDia") : parent_sys_obj->Get("Diam") ) *
								parent_diameter_scalar * system_param->GetGlobalScale();

				orbit_distance_km_10p6 *=
					system_param->GetObjectScale();
			}


	// ---- day rotation
		DOUBLE daylen_scalar    = system_param->GetDaylenScalar();
		DOUBLE day_length_hours = (DOUBLE)( illustration_mode ? sys_obj->Get("GfxDayLen") : sys_obj->Get("DayLen") );
		DOUBLE day_start_hours  =
				(DOUBLE)( illustration_mode ? sys_obj->Get("GfxDaySta") : sys_obj->Get("DaySta") );

		DOUBLE day_degrees      = fabs(day_length_hours) < 0.000001 ? 0 :
				hours_elapsed / day_length_hours * 360 ;

		       day_degrees     += fabs(day_start_hours) < 0.000001 ? 0 :  
					day_start_hours / day_length_hours * 360;

			   day_degrees     *= daylen_scalar;


	// ---- 
		CHAR *name = sys_obj->GetName();
			   

	// ---- matrix stuff


		// ---- parent world matrix
			vs_system::VsMatrix *parent_matrix = &matrix_stack->stack[matrix_stack->index];


		// ---- push orbit
			recursive_depth_count++;
			matrix_stack->Push(); 


		// ---- locate
			FLOAT x = (FLOAT)orbit_distance_km_10p6 * (FLOAT)cos(orbit_degrees * VS_RADIANS_PER_DEGREE);
			FLOAT z = (FLOAT)orbit_distance_km_10p6 * (FLOAT)sin(orbit_degrees * VS_RADIANS_PER_DEGREE);

		// ---- inclination
			DOUBLE orbit_inclination = sys_obj->Get("OrbInc");
			FLOAT yd = -z * (FLOAT)sin(orbit_inclination * VS_RADIANS_PER_DEGREE);
			FLOAT zd =  z * (FLOAT)cos(orbit_inclination * VS_RADIANS_PER_DEGREE);
			matrix_stack->Translate( x, yd, zd);


		// ---- obiquity (TBD add inclination)
			DOUBLE orbit_obliquity = !strcmp("Sun", name) ?  0 : sys_obj->Get("OrbObl");
			matrix_stack->Rotate((FLOAT)  orbit_obliquity, 'Z');


		// ---- push day degrees
			recursive_depth_count++;
			matrix_stack->Push();
			matrix_stack->Rotate((FLOAT) -day_degrees, 'Y');


	// ---- item transform now top of stack matrix
		if ( handle_sphere[sys_obj->GetIndex()] != NULL )
		{

			// ---- local
				vs_system::ElmContext *elm_context = NULL;


			// ---- store item matrix
				vs_system::VsMatrix  *this_matrix = &matrix_stack->stack[matrix_stack->index];
				hr = gfx->GetElementContext(handle_sphere[sys_obj->GetIndex()], &elm_context);
				elm_context->SetMatrixActive(TRUE);
				elm_context->SetMatrix(this_matrix);


			// ---- store rings matrix 
				hr = gfx->GetElementContext(handle_rings[sys_obj->GetIndex()], &elm_context);
				elm_context->SetMatrixActive(TRUE);
				elm_context->SetMatrix(this_matrix);


			// ---- target ?
				if ( target_planet_index != 0 )
				{
					if ( !strcmp(elm_context->GetName().c_str(), target_planet_name[target_planet_index]) )
					{
						target_planet_position.x = this_matrix->m[3][0];
						target_planet_position.y = this_matrix->m[3][1];
						target_planet_position.z = this_matrix->m[3][2];
					}
				}


			// ---- orbit - use and then loose the parent matrix
				SystemObject *parent = sys_obj->GetParent();
				if ( parent != NULL && handle_orbit[sys_obj->GetIndex()] != NULL )
				{

					matrix_stack->Push();

						vs_system::VsMatrix *this_matrix = &matrix_stack->stack[matrix_stack->index];

						vs_system::VsMatrixOp mop;
						mop.Copy(this_matrix, parent_matrix);

						DOUBLE orbit_inclination = sys_obj->Get("OrbInc");

						matrix_stack->Rotate( (FLOAT)  orbit_inclination, 'X');
						matrix_stack->Rotate( (FLOAT) -orbit_degrees, 'Y');
						hr = gfx->GetElementContext(handle_orbit[sys_obj->GetIndex()], &elm_context);
						elm_context->SetMatrixActive(TRUE);
						elm_context->SetMatrix(this_matrix);

					matrix_stack->Pop();

				}

		}


		// ---- pop day degrees
			recursive_depth_count--;
			matrix_stack->Pop();


	// ---- recursively display child objects
		SystemObject *s = sys_obj->GetFirst();
		while (s)
		{
			hr = SetupSystemObjectsForDisplay(s);
			s = s->GetNext();
		}


	// ---- pop orbit
		recursive_depth_count--;
		matrix_stack->Pop();


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========================================================================== //

INT Model::SetupCubeDisplay()
{

	pi_model->recursive_depth_count = 0;
	pi_model->matrix_stack->Initialise();
	pi_model->SetupCubeDisplay();
	return VS_OK;
}

// ========================================================================== //

INT Model::PI_Model::SetupCubeDisplay()
{

	for (int i = 0; i < 12; i++)
	{

		// ---- push
			matrix_stack->Push();


		// ---- time
			DOUBLE time_elapsed_ms =
				sys_context->GetDouble(vs_system::SysContext::TIME_ELAPSED) +
					system_param->GetMsOffset();


		// ---- rotation
			DOUBLE rot_degrees = time_elapsed_ms / 100;


		// ---- matrix stuff
			vs_system::VsMatrix  *m = &matrix_stack->stack[matrix_stack->index];
			matrix_stack->Rotate(    (FLOAT)-rot_degrees, 'Y');
			matrix_stack->Translate( (FLOAT) 0, 0, 0);
			matrix_stack->Rotate(    (FLOAT) 0.0f, 'Y');


		// ---- sphere - world matrix
			vs_system::ElmContext *elm_context = NULL;
			INT hr = gfx->GetElementContext(handle_sphere[i], &elm_context);
			elm_context->SetMatrixActive(TRUE);
			elm_context->SetMatrix(m);
			elm_context->SetViewMatrix(view_matrix_index);


		// ---- pop
			matrix_stack->Pop();
		
	}

	return VS_OK;
}

// ========================================================================== //

INT Model::InitCubeObjects()
{

	// ---- create display list
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- create display list ?
		if ( pi_model->runtime )
		{
			pi_model->handle_sphere_dsplst = gfx->AddDisplayList("My_3D_Sphere_01", 1, 1);
		}


	// ---- for each face
		for (int i = 0; i < 12; i ++)
		{

			// ---- create quad ?
				if ( pi_model->runtime )
				{
					pi_model->handle_sphere[i] = gfx->AddElement(
							pi_model->handle_sphere_dsplst,
							"",
							1,
							1,
							pi_model->obj_config->Callback_CubeFace_Configure
						);
				}


			// ---- context
				vs_system::ElmContext *gfx_context = NULL;
				INT hr = gfx->GetElementContext(pi_model->handle_sphere[i], &gfx_context);


			// ---- force geometry setup
				gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
				gfx_context->SetConfigured(FALSE);


			// ---- init sphere parameters
				FLOAT *param = gfx_context->GetFloatParameter();
				param[vs_system::ObjConfig::CUBEFACE_MODE]    = (float)i;
				param[vs_system::ObjConfig::CUBEFACE_TYPE]    = 2;
				param[vs_system::ObjConfig::CUBEFACE_AP_REF]  = 108;
				param[vs_system::ObjConfig::CUBEFACE_RED]     = 1;
				param[vs_system::ObjConfig::CUBEFACE_GREEN]   = 1;
				param[vs_system::ObjConfig::CUBEFACE_BLUE]    = 1;
				param[vs_system::ObjConfig::CUBEFACE_SCALE]   = 150.0f;
				param[vs_system::ObjConfig::CUBEFACE_EXPLODE] = 0.5f;


			// ---- lighting and materials
				hr = gfx_context->SetLit(TRUE);
				hr = gfx_context->SetMaterial(i);

				hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CCW);

		}


	// ---- done
		pi_model->runtime = FALSE;

	return VS_OK;
}

// ========================================================================== //

INT Model::InitCubeScene()
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_model->gfx;
		INT hr = 0;


	// ---- ambient
		VS_Colour amb_col = { 0.5f, 0.5f, 0.5f, 1.0f };
		vs_system::VsAmbient amb;
		hr = amb.SetColour(amb_col);
		hr = gfx->SetAmbient(&amb);


	// ---- directional main light
		VS_Colour sun_amb = {  0.2f, 0.2f, 0.2f, 1.0f };
		VS_Colour sun_dif = {  1.0f, 1.0f, 1.0f, 1.0f };
		VS_Colour sun_spc = {  1.0f, 1.0f, 1.0f, 1.0f };
		VS_Vector sun_dir = {  1.0f,-1.0f, 1.0f };
		vs_system::VsLight sun;
		hr = sun.SetType(vs_system::VS_LIGHT_DIRECTIONAL);
		hr = sun.SetAmbient(sun_amb);
		hr = sun.SetDiffuse(sun_dif);
		hr = sun.SetSpecular(sun_spc);
		hr = sun.SetDirection(sun_dir);
		hr = gfx->SetLight(0, &sun);
		hr = gfx->SetLightEnabled(0, TRUE);


	// ---- directional fill from right to back
		VS_Colour lgt_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
		VS_Colour lgt_dif = { 0.5f, 0.5f, 0.5f, 1.0f };
		VS_Colour lgt_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
		VS_Vector lgt_dir = {-1.0f,-1.0f,-1.0f };
		vs_system::VsLight lgt;
		hr = lgt.SetType(vs_system::VS_LIGHT_DIRECTIONAL);
		hr = lgt.SetAmbient(lgt_amb);
		hr = lgt.SetDiffuse(lgt_dif);
		hr = lgt.SetSpecular(lgt_spc);
		hr = lgt.SetDirection(lgt_dir);
		hr = gfx->SetLight(1, &lgt);
		hr = gfx->SetLightEnabled(1, FALSE);


	// ---- point radience from origin
		VS_Colour top_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
		VS_Colour top_dif = { 0.5f, 0.5f, 0.5f, 1.0f };
		VS_Colour top_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
		VS_Vector top_pos = { 0.0f, 0.0f, 0.0f };
		vs_system::VsLight top;
		hr = top.SetType(vs_system::VS_LIGHT_POINT);
		hr = top.SetAmbient(top_amb);
		hr = top.SetDiffuse(top_dif);
		hr = top.SetSpecular(top_spc);
		hr = top.SetPosition(top_pos);
		hr = top.SetRange(1000);
		hr = top.SetAttLinear(0.01f);
		hr = gfx->SetLight(2, &top);
		hr = gfx->SetLightEnabled(2, TRUE);


	// ---- spot left to front
		VS_Colour spot_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
		VS_Colour spot_dif = { 1.0f, 1.0f, 1.0f, 1.0f };
		VS_Colour spot_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
		VS_Vector spot_pos = {-800.0f, 800.0f,-800.0f };
		VS_Vector spot_dir = { 1.0f,-1.0f, 1.0f };
		vs_system::VsLight spot;
		hr = spot.SetType(vs_system::VS_LIGHT_SPOT);
		hr = spot.SetAmbient(spot_amb);
		hr = spot.SetDiffuse(spot_dif);
		hr = spot.SetSpecular(spot_spc);
		hr = spot.SetPosition(spot_pos);
		hr = spot.SetDirection(spot_dir);
		hr = spot.SetRange(2000);
		hr = spot.SetAttConstant(0);
		hr = spot.SetAttLinear(0.001f);
		hr = spot.SetAttQuadratic(0);
		hr = spot.SetOuter(20 * (float)VS_RADIANS_PER_DEGREE);
		hr = spot.SetInner(10* (float)VS_RADIANS_PER_DEGREE);
		hr = spot.SetFalloff(1.0f);
		hr = gfx->SetLight(3, &spot);
		hr = gfx->SetLightEnabled(3, TRUE);


	// ---- materials for each cube face
		VS_Colour c[12] =
		{
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f },
		};
		for (int i = 0; i < 12; i++)
		{
			VS_Colour obj_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
			VS_Colour obj_dif = { c[i].red, c[i].green, c[i].blue, 1.0f };
			VS_Colour obj_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
			vs_system::VsMaterial obj;
			hr = obj.SetAmbient(obj_amb);
			hr = obj.SetDiffuse(obj_dif);
			hr = obj.SetSpecular(obj_spc);
			hr = obj.SetPower(40);
			hr = gfx->SetMaterial(i, &obj);
		}


	// ---- view matrix 1
		D3DXVECTOR3 pos = { 0, 0, -1000 };
		D3DXVECTOR3 dir = { 0, 0, 1 };
		pi_model->my_camera->SetLook(&pos, &dir);

		D3DXMATRIX V;
		pi_model->my_camera->GetViewMatrix(&V);
		gfx->SetViewMatrix(1, &V);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT Model::SetCamera(INT action, FLOAT delta)
{

	// ---- setup
		action = toupper(action);
		#if defined(CUBE_TEST)
			float scalar = 5;
			float ang_div = 16.0f;
		#else
			float scalar = 0.2f;
			float ang_div = 16.0f;
		#endif


	// ---- increment
		UINT gfx_mode = pi_model->system_param->GetGfxMode();
		BOOL illustration_mode = FALSE;
		FLOAT imin = 0.01f;
		FLOAT incr = 0.01f;
		switch ( gfx_mode )
		{
			case SystemObject::ILLUSTRATION:
				incr = 0.05f;
				break;
			case SystemObject::NORMAL:
				incr = 0.01f;
				break;
			case SystemObject::LARGE:
			default:
				incr = 0.05f;
				break;
		}


	// ---- process
		vs_system::AppCamera *camera = pi_model->my_camera;
		switch ( action )
		{

			case 'P':
				{
					INT pause_mode = pi_model->system_param->GetPauseMode();
						pause_mode = pause_mode ? 0 : 1;
					pi_model->system_param->SetPauseMode(pause_mode);

					DOUBLE system_time_elapsed = pi_model->sys_context->GetDouble(vs_system::SysContext::TIME_ELAPSED);
					DOUBLE pause_time_elapsed = pi_model->system_param->GetPauseTimeElapsed();

					if ( pause_mode )
					{
						pi_model->system_param->SetPauseTimeStart(system_time_elapsed);
					}
					else
					{
						pause_time_elapsed += system_time_elapsed - pi_model->system_param->GetPauseTimeStart();
						pi_model->system_param->SetPauseTimeElapsed(pause_time_elapsed);
					}

				}
				break;

			case '-':
				pi_model->system_param->SetTimeScalar(pi_model->system_param->GetTimeScalar()/2);
				return VS_OK;

			case '=':
				pi_model->system_param->SetTimeScalar(1.0f);
				return VS_OK;

			case '+':
				pi_model->system_param->SetTimeScalar(pi_model->system_param->GetTimeScalar() * 2);
				return VS_OK;

			case 'T':
				{
					BOOL textures = pi_model->sys_context->GetBool(vs_system::SysContext::TEXTURES);
						textures = textures ? FALSE : TRUE;
					INT result = pi_model->sys_context->SetBool(
							vs_system::SysContext::TEXTURES,
							textures
						);
				}
				break;

			case 'V':
				{
					pi_model->camera_type = pi_model->camera_type == vs_system::AppCamera::LANDOBJECT ?
						vs_system::AppCamera::AIRCRAFT :
							vs_system::AppCamera::LANDOBJECT;
					camera->SetCameraType(pi_model->camera_type);
				}
				break;

			case 'C': 
				{
					#if defined(CUBE_TEST) 
						;
					#else

						UINT gfx_mode = pi_model->system_param->IncGfxMode();

						INT result = InitSystemObjects();

						pi_model->target_planet_dist_scalar =
							pi_model->system_param->GetTargetDistScalar();


					#endif
					return VS_OK;
				}
				break;

			default:
				{

					INT tmp_action = action;
					if ( action > '0' && action <= ':' )
					{
						tmp_action = '0'; 
					}

					switch ( tmp_action )
					{

						// ----- at
							case '0':
								{
									pi_model->target_planet_index = action - '0';
									if ( action == '0' )
									{

										pi_model->target_planet_position.x = 0;
										pi_model->target_planet_position.y = 0;
										pi_model->target_planet_position.z = 0;

										pi_model->target_planet_dist_scalar = 
											pi_model->system_param->GetTargetDistScalar();

										pi_model->my_camera->SetLook(
											&pi_model->target_camera_position,
											&pi_model->target_camera_direction
											);

									}

									CHAR msg[VS_MAXCHARLEN];
									sprintf(msg, " +++ Choose %s \n", pi_model->target_planet_name[pi_model->target_planet_index]);
									OutputDebugString(msg);

								}
								break;

							case 'Z':
								pi_model->target_planet_dist_scalar = 1.0f;
								break;

							case 'X':
								{
									pi_model->target_planet_dist_scalar = pi_model->system_param->GetTargetDistScalar();

									pi_model->target_camera_position = {
											pi_model->default_camera_position.x,
											pi_model->default_camera_position.y,
											pi_model->default_camera_position.z
									};

									pi_model->target_camera_direction = {
											pi_model->default_camera_direction.x,
											pi_model->default_camera_direction.y,
											pi_model->default_camera_direction.z
									};

									D3DXVECTOR3 right = { 1.0f, 0.0f, 0.0f };
										camera->SetRight(&right);

									D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
										camera->SetUp(&up);

									camera->SetLook(
											&pi_model->target_camera_position,
											&pi_model->default_camera_direction
										);
								}
								break;

							case 'Q':
								pi_model->target_planet_dist_scalar -= incr;
								pi_model->target_planet_dist_scalar =
									pi_model->target_planet_dist_scalar < imin ?
									imin :
											pi_model->target_planet_dist_scalar;
								break;

							case 'E':
								pi_model->target_planet_dist_scalar += incr;
								break;


						// ---- update pos
							case 'W': camera->Walk(   scalar * delta); break;
							case 'S': camera->Walk(  -scalar * delta); break;
							case 'A': camera->Strafe(-scalar * delta); break;
							case 'D': camera->Strafe( scalar * delta); break;
							case 'R': camera->Fly(    scalar * delta); break;
							case 'F': camera->Fly(   -scalar * delta); break;

						// ---- rotate
							case 'I': camera->Pitch( scalar / ang_div * delta); break;
							case 'K': camera->Pitch(-scalar / ang_div * delta); break;
							case 'J': camera->Yaw(  -scalar / ang_div * delta); break;
							case 'L': camera->Yaw(   scalar / ang_div * delta); break;

						// ---- rotate 
							case 'N': camera->Roll( scalar / ang_div * delta); break;
							case 'M': camera->Roll(-scalar / ang_div * delta); break;

						// ---- nada
							default:
								break;
					}
				}
			break;
		}


	// ---- update the view matrix representing the cameras new position/orientation.
		D3DXMATRIX V;
		camera->GetViewMatrix(&V);
		pi_model->gfx->SetViewMatrix(1, &V);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetDisplayListHandle ----------
/*!
\brief Setup a display list panel handle
\author Gareth Edwards
\return
*/
INT Model::GetPanelDisplayListHandle(
		vs_system::PanelContext *panel_gfx_context
	)
{

	// ---- local
		INT hr = 0;


	// ---- get current index of display list
		UINT index_display_list;
		hr = panel_gfx_context->GetDisplayListIndex(&index_display_list);


	// ---- get display list handle
		INT handle_display_list;
		hr = panel_gfx_context->GetDisplayListHandle(index_display_list, &handle_display_list);


	// ---- no display list handle ? - then set one -
		if ( handle_display_list <= 0 )
		{

			// ---- get display list name
				CHAR name_display_list[VS_MAXCHARLEN];
				hr = panel_gfx_context->GetDisplayListName(index_display_list, name_display_list);


			// ---- get display list handle, and id's...
				handle_display_list = pi_model->gfx->GetHandleDisplayList(name_display_list);
				UINT group_id = pi_model->gfx->GetElementGroupId(handle_display_list);
				UINT id = pi_model->gfx->GetElementGroupId(handle_display_list);


			// ---- get panel name
				CHAR name_panel[VS_MAXCHARNAME];
				hr = panel_gfx_context->GetName(name_panel);
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
				hr = panel_gfx_context->SetDisplayListHandle(
								index_display_list,
								handle_display_list
							);
				hr = panel_gfx_context->SetGroupId(group_id);

		}

	return handle_display_list;
}


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
		INT hr = VS_OK;


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
			VS_FloatRectangle fr;
		};


	// ---- initialise panel rects
		pane_rect pane_rect_lut[lut_number] =
		{
			{ "My_3D_Panel", x_left, y_top, x_right, y_bottom }
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
				vs_system::PanelContext *gfx_context = panel->GetPanelContext();
				VS_FloatRectangle *vr = gfx_context->GetViewrect();
				*vr = pane_rect_lut[0].fr;
				hr = gfx_context->GetName(name);
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




