////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_obj_star_system.hpp ----------
/*!
\file s1_model_obj_star_system.hpp
\brief Implementation of the Object System methods
\author Gareth Edwards
*/


// ========================================================================== //


INT Model::InitStarSystem()
{

	// ---- local
		INT result = 0;


	// ---- set config src directories 
		pi_model->config_files->SetSrcDirectory(1, "csv");
		pi_model->config_files->SetSrcDirectory(2, "csv");


	// ---- set config src files 
		pi_model->config_files->SetSrcFilename(0, "star_system.hdl");
		pi_model->config_files->SetSrcFilename(1, "star_system.csv");
		pi_model->config_files->SetSrcFilename(2, "stars.csv");


	// ---- setup config files
		pi_model->config_files->Setup(pi_model->app_dependency, "Observant Simulations", "surface_01");


	// ---- system param
		CHAR solar_system_hdl_filename[VS_MAXCHARPATHLEN];
		pi_model->config_files->GetAppFilename(0, solar_system_hdl_filename);
		pi_model->system_param->Read(solar_system_hdl_filename);
		//pi_model->system_param->Write();


	// ---- system data
		CHAR solar_system_csv_filename[VS_MAXCHARPATHLEN];
		pi_model->config_files->GetAppFilename(1, solar_system_csv_filename);
		result = pi_model->system_io->Read(pi_model->system_object, solar_system_csv_filename);
		//result = pi_model->system_io->Write();();

	return VS_OK;
}


// ========================================================================== //


INT Model::InitStarSystemScene()
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_model->gfx;
		INT hr = 0;


	// ---- fog
		VsColour col = { 0.0f, 0.0f, 0.0f, 0.0f };
		vs_system::VsFog fog;
		hr = fog.SetColour(col);
		hr = fog.SetType(vs_system::VsFogType::VS_FOG_LINEAR);
		hr = fog.SetStart(3000);
		hr = fog.SetEnd(8000);
		hr = gfx->SetFog(&fog);
		hr = gfx->SetFogEnabled(TRUE);


	// ---- ambient light
		VsColour amb_col = { 0.1f, 0.1f, 0.1f, 1.0f };
		vs_system::VsAmbient amb;
		hr = amb.SetColour(amb_col);
		hr = gfx->SetAmbient(&amb);


	// ---- sun light
		VsColour dif = { 1.0f, 1.0f, 1.0f, 1.0f };
		VsVector3 pos = { 0.0f, 0.0f, 0.0f };
		vs_system::VsLight sun;
		hr = sun.SetType(vs_system::VS_LIGHT_POINT);
		hr = sun.SetDiffuse(dif);
		hr = sun.SetPosition(pos);
		hr = sun.SetRange(30000.0f); 
		hr = sun.SetAttLinear(0.0015f);
		hr = gfx->SetLight(0, &sun);
		hr = gfx->SetLightEnabled(0, TRUE);


	// ---- kamera
		if ( pi_model->runtime )
		{
			for (INT i = 0; i < pi_model->num_app_camera; i++)
			{
				vs_system::AppKamera *kamera = pi_model->app_kamera[i];

				// ---- target
					kamera->SetTargetIncr(0.1f);
					kamera->SetTargetZoom(200.0f);

				// ---- increment
					kamera->SetScalarIncr(0.5f);
					kamera->SetScalarRotation(TRUE);
					kamera->SetScalarMode(TRUE);
					kamera->SetPositionIncr(0.5f);
					kamera->SetPanTiltIncr(0.01f);
					kamera->SetRotateIncr(0.02f);
					kamera->SetTranslateIncr(1.0f);
					kamera->SetZoomIncr(1.0f);

				// ---- set view
					kamera->SetPanTilt(0, 35);
					kamera->SetPosition(0, 700, -1250);

				// ---- default & state
					kamera->SaveDefaults();
					kamera->SaveState();

			}
			INT result = SetMyKamera(pi_model->app_kamera_index);
		}

	return VS_OK;
}


// ========================================================================== //



INT Model::InitStarSystemObjects()
{

	// ---- local
		INT hr = VS_OK;


	// ---- add display lists ( in draw sequence )
		if ( pi_model->runtime )
		{
			vs_system::GfxInterface *gfx = pi_model->gfx;
			hr = pi_model->handle_starfield_dsplst = gfx->AddDisplayList("Starfield", 1, 1);
			hr = pi_model->handle_asteroid_dsplst  = gfx->AddDisplayList("Asteroid",  1, 1);
			hr = pi_model->handle_sphere_dsplst    = gfx->AddDisplayList("Sphere",    1, 1);
			hr = pi_model->handle_cube_dsplst      = gfx->AddDisplayList("Cube",      1, 1);
			hr = pi_model->handle_dotobj_dsplst    = gfx->AddDisplayList("DotObj",    1, 1);
			hr = pi_model->handle_orbit_dsplst     = gfx->AddDisplayList("Orbit",     1, 1);
			hr = pi_model->handle_ring_dsplst      = gfx->AddDisplayList("Ring",      1, 1);
			hr = pi_model->handle_halo_dsplst      = gfx->AddDisplayList("Halo",      1, 1);
		}


	// ---- init (create) objects
		pi_model->recursive_depth_count = 0;
		hr = pi_model->InitStarSystemObjectsGfx(pi_model->system_object);
		hr = pi_model->InitStarSystemObjects(pi_model->system_object);


	// ---- runtime - end one time init
		pi_model->runtime = FALSE;

	return VS_OK;
}


// ========================================================================== //

INT Model::PI_Model::InitStarSystemObjectsGfx(SystemObject *sys_obj)
{

	// --- reconfigure ?
		UINT configure = ((INT)(sys_obj->Get("GfxCon")));
		if ( configure )
		{

			// ---- required type
				UINT system_object_type = ((INT)(sys_obj->Get("Type")));
				if (
						system_object_type == SystemObject::AstRing    ||
						system_object_type == SystemObject::Star       ||
						system_object_type == SystemObject::Planet     ||
						system_object_type == SystemObject::GasGiant   ||
						system_object_type == SystemObject::Dwarf      ||
						system_object_type == SystemObject::Asteroid   ||
						system_object_type == SystemObject::Starfield
					)
				{
					
					// ---- calculate gfx info
						FLOAT ten_to_p6 = 1000000;
						FLOAT ten_to_p3 = 1000;
						INT   result = 0;


					// ---- get
						FLOAT distance = sys_obj->Get("GfxDis");
						result = sys_obj->Set("GfxDis", distance * ten_to_p3);


					// 
						FLOAT diameter = sys_obj->Get("GfxDia");
						result = sys_obj->Set("GfxDia", diameter * ten_to_p6);


					// ---- radii
						FLOAT radius_min = sys_obj->Get("GfxRadMin");
						FLOAT radius_max = sys_obj->Get("GfxRadMax");


					// ---- TBD - main belt
						UINT type = ((INT)(sys_obj->Get("Type")));
						if ( type == SystemObject::AstRing || type == SystemObject::Starfield )
						{
							result = sys_obj->Set("GfxRadMin", radius_min * ten_to_p3);
							result = sys_obj->Set("GfxRadMax", radius_max * ten_to_p3);
						}


					// ---- moons
						UINT num_moons = ((INT)(sys_obj->Get("Moon")));
						if ( num_moons > 0 )
						{
							FLOAT radius = diameter;
							FLOAT incr = 0.1f;
							SystemObject *moon_obj = sys_obj->GetFirst();
							while ( moon_obj )
							{
								radius += incr;
								result = moon_obj->Set("GfxDis", radius);
								result = moon_obj->Set("GfxDia", moon_obj->Get("Diam") * 20); // param
								moon_obj = moon_obj->GetNext();
							}
						}

				}


				// ---- configured...
					INT r = sys_obj->Set( "GfxCon", 0.0f);

		}

	// ---- process child objects
		SystemObject *s = sys_obj->GetFirst();
		while (s)
		{
			INT result = InitStarSystemObjectsGfx(s);
			s = s->GetNext();
		}


	return VS_OK;
}


// ========================================================================== //


INT Model::PI_Model::InitStarSystemObjects(SystemObject *sys_obj)
{

	// ---- local 
		enum GeometryComplexity
		{
			HIGH   = 0,
			MEDIUM = 1,
			LOW    = 2,
			BLOCK  = 3
		};

		struct ObjectGeometry
		{
			UINT cels;
			UINT rows;
			UINT lines;
		};

		ObjectGeometry object_geometry[4] =
		{
			{ 80, 40, 720 },
			{ 30, 15, 360 },
			{ 10,  5, 180 },
			{  5,  3,   1 },
		};


	// ---- local 
		INT result = VS_OK;


	// ---- gfx version ?
		BOOL gfx_version = system_param->GetGfxVersion();


	// ---- increment
		recursive_depth_count++;


	// ---- type
		UINT system_object_type = (UINT)sys_obj->Get("Type");


	// ---- diameter - ok
		DOUBLE object_diameter_scalar = system_param->GetObjectDiameterScalar(system_object_type);


	// ---- complexity
		UINT level_of_complexity = GeometryComplexity::HIGH;
		switch ( system_object_type )
		{
			default:
			case SystemObject::Type::UnKnown:
			case SystemObject::Type::Star:
			case SystemObject::Type::GasGiant:
			case SystemObject::Type::Planet:
				break;
			case SystemObject::Type::Dwarf:
			case SystemObject::Type::Asteroid:
				level_of_complexity = GeometryComplexity::MEDIUM;
				break;
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
			case SystemObject::Type::Starfield:
				level_of_complexity = GeometryComplexity::BLOCK;
				break;
			case SystemObject::Type::Moon:
				{
					FLOAT diameter = sys_obj->Get("Diam");
					if ( diameter > 1000 )
					{
						level_of_complexity = GeometryComplexity::MEDIUM;
					}
					else
					{
						level_of_complexity = GeometryComplexity::LOW;
					}
				}
				break;
		}


	// ---- unknown geometry
		BOOL create_object_geometry = TRUE;
		switch ( system_object_type )
		{
			case SystemObject::Type::UnKnown:
				create_object_geometry = FALSE;
				break;
			default:
				break;
		}


	// ---- orbit geometry
		BOOL create_orbit_geometry = TRUE;
		switch ( system_object_type )
		{
			case SystemObject::Type::UnKnown:
			case SystemObject::Type::Star:
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
			case SystemObject::Type::Starfield:
				create_orbit_geometry = FALSE;
				break;
			default:
				break;
		}


	// ---- halo geometry
		BOOL create_halo_geometry = TRUE;
		switch ( system_object_type )
		{
			case SystemObject::Type::UnKnown:
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Starfield:
				create_halo_geometry = FALSE;
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
		CHAR *object_name = sys_obj->GetName();


	// ---- minimum size == scaler
		FLOAT system_scalar    = (FLOAT)system_param->GetGlobalScale();
		FLOAT minimum_diameter = system_scalar;
		FLOAT maximum_diameter = 100000.0f;


	// ---- object diameter
		//
		// note: can't do this on matrix stack without adding non-scale channel
		//
		FLOAT object_diameter =
				(FLOAT)(
					( gfx_version ? sys_obj->Get("GfxDia" ) : sys_obj->Get("Diam") ) *
								object_diameter_scalar * system_scalar );
			object_diameter =
					object_diameter < minimum_diameter ? minimum_diameter :
						(object_diameter > maximum_diameter ? maximum_diameter : object_diameter );


	// ---- parent diameter
		FLOAT parent_diameter =
				parent_sys_obj == NULL ? 0 :
					(FLOAT)(
						( gfx_version ? parent_sys_obj->Get("GfxDia") : parent_sys_obj->Get("Diam") ) *
								parent_diameter_scalar * system_scalar );

			parent_diameter =
					parent_diameter < minimum_diameter ? minimum_diameter :
						(parent_diameter > maximum_diameter ? maximum_diameter : parent_diameter);


	// ---- create geometry
			if ( create_object_geometry )
			{

				// ---- index
					if (runtime)
					{
						;
					}
					else
					{
						object_index = sys_obj->GetIndex();
					}


				////////////////////////////////////////////////////////////////////////////////


				// ---- create

					if ( runtime )
					{
							switch ( system_object_type )
							{
								case SystemObject::Type::AstRing:
									{
										handle_asteroid[object_index] = gfx->AddElement(
												handle_asteroid_dsplst,
												object_name,
												SYS_OBJECT_GROUP_ID,
												object_index,
												obj_config->Callback_MyStar_Asteroid_Configure
											);
									}
									break;
								case SystemObject::Type::Starfield:
									{
										handle_starfield[object_index] = gfx->AddElement(
												handle_starfield_dsplst,
												object_name,
												SYS_OBJECT_GROUP_ID,
												object_index,
												obj_config->Callback_MyStar_Starfield_Configure
											);
									}
									break;
								default:
									{
	
										handle_sphere[object_index] = gfx->AddElement(
												handle_sphere_dsplst,
												object_name,
												SYS_OBJECT_GROUP_ID,
												object_index,
												obj_config->Callback_MyStar_Sphere_Configure
											);

										handle_cube[object_index] = gfx->AddElement(
												handle_cube_dsplst,
												object_name,
												SYS_OBJECT_GROUP_ID,
												object_index,
												obj_config->Callback_Box_Configure
											);

										handle_dotobj[object_index] = gfx->AddElement(
												handle_dotobj_dsplst,
												object_name,
												SYS_OBJECT_GROUP_ID,
												object_index,
												obj_config->Callback_DotObj_Configure
											);

									}
									break;
							}

					}
					else
					{
						INT  handle = NULL;
						switch ( system_object_type )
						{
							case SystemObject::Type::AstRing:
								handle = handle_asteroid[object_index];
								break;
							case SystemObject::Type::Starfield:
								handle = handle_starfield[object_index];
								break;
							default:
								{
									switch ( object_geometry_version )
									{
										case ObjectGeometryVersion::SPHERE:
											handle = handle_sphere[object_index];
											break;
										case ObjectGeometryVersion::CUBE:
											handle = handle_cube[object_index];
											break;
										case ObjectGeometryVersion::DOTOBJ:
											handle = handle_dotobj[object_index];
											break;
										default:
											break;
									}
								}
								break;

						}

						gfx->SetElementConfigureBitmask(
									handle,
									VS_ELM_SETUP_GEOMETRY
								);

					}


			// ---- number of object types
				INT max_object_geometry_version = ObjectGeometryVersion::MAX;
				switch ( system_object_type )
				{
					case SystemObject::Type::AstRing:
					case SystemObject::Type::Starfield:
						max_object_geometry_version = 1;
						break;
					default:
						break;
				}


			// ---- context
				vs_system::ElmContext *gfx_context = NULL;


			// ---- for each object type
				for (INT version = 1; version <= max_object_geometry_version; version++)
				{
					
					//
					// Note:
					//    Sun textured & NOT lit
					//    Planets textured & lit
					//
					//    Both have materials associated with them...
					//

					// ---- get graphics context
						switch ( system_object_type )
						{
							case SystemObject::Type::AstRing:
								result = gfx->GetElementContext(handle_asteroid[object_index], &gfx_context);
								break;
							case SystemObject::Type::Starfield:
								result = gfx->GetElementContext(handle_starfield[object_index], &gfx_context);
								break;
							default:
								switch ( version )
								{
									case ObjectGeometryVersion::SPHERE:
										result = gfx->GetElementContext(handle_sphere[object_index], &gfx_context);
										break;
									case ObjectGeometryVersion::CUBE:
										result = gfx->GetElementContext(handle_cube[object_index], &gfx_context);
										break;
									case ObjectGeometryVersion::DOTOBJ:
										result = gfx->GetElementContext(handle_dotobj[object_index], &gfx_context);
										break;
									default:
										float fred = 2;
										break;
								}
								break;
						}


					// ---- force geometry setup
						//
						// this does not work ...
						// result = gfx_context->SetConfigured(FALSE);
						// 
						// ....does nothing
						//
						// result = gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
						// result = gfx_context->SetConfigured(FALSE);


					// ---- TBD properties
						result = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_CLICK);
						result = gfx_context->SetPick3D(
							system_object_type != SystemObject::Type::AstRing &&
								system_object_type != SystemObject::Type::Starfield ?
									TRUE : FALSE );
						result = gfx_context->SetTextureMirror(TRUE);

	
					// ---- init sphere parameters
						FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];


					// ---- fog & lighting ON
						result = gfx_context->SetFog( strcmp("Sun", object_name) != 0 );
						result = gfx_context->SetLit(TRUE);

						if (!strcmp(object_name, "Sun"))
						{
							float fred = 2;
						}

					// ---- init sphere asteroid belt parameters

						switch ( system_object_type )
						{

							case SystemObject::Type::AstRing:
								{

								// --- display ONLY in grapfix mode
										gfx_context->SetDisplay(gfx_version);

									// ---- same as sphere
										param[vs_system::ObjConfig::SPHERE_CEL_COLS] = (FLOAT)object_geometry[level_of_complexity].cels;
										param[vs_system::ObjConfig::SPHERE_CEL_ROWS] = (FLOAT)object_geometry[level_of_complexity].rows;
										param[vs_system::ObjConfig::SPHERE_TYPE]     = 2;
										param[vs_system::ObjConfig::SPHERE_AP_REF]   = sys_obj->Get("GfxPag") + 100;
										param[vs_system::ObjConfig::SPHERE_AP_SECT]  = sys_obj->Get("GfxSec");
										param[vs_system::ObjConfig::SPHERE_RADIUS]   = object_diameter / 20; // param
										param[vs_system::ObjConfig::SPHERE_RED]      = sys_obj->Get("GfxRed");
										param[vs_system::ObjConfig::SPHERE_GREEN]    = sys_obj->Get("GfxGre");
										param[vs_system::ObjConfig::SPHERE_BLUE]     = sys_obj->Get("GfxBlu");
										param[vs_system::ObjConfig::SPHERE_LAT_MIN]  = -90;
										param[vs_system::ObjConfig::SPHERE_LAT_MAX]  =  90;

										param[vs_system::ObjConfig::SPHERE_RAD_MIN]  =
												gfx_version ?
													sys_obj->Get("GfxRadMin") :
														sys_obj->Get("RadMin");
										param[vs_system::ObjConfig::SPHERE_RAD_MAX]  =
												gfx_version ?
													sys_obj->Get("GfxRadMax") :
														sys_obj->Get("RadMax");

									// ---- asteroid ring
										param[vs_system::ObjConfig::SPHERE_RADIUS] = object_diameter / 35; // param
										param[vs_system::ObjConfig::SPHERE_NUMBER] = 800; // param
										result = gfx_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);

									// ---- cull
										result = gfx_context->SetCullMode(vs_system::ElmContext::CULL_NONE);

									// ---- properties
										result = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
										result = gfx_context->SetPick3D(FALSE);

								}
								break;

							case SystemObject::Type::Starfield:
								{

									// ---- size NOT affected by mode
										UINT svm = system_param->GetSystemViewMode();
											system_param->SetSystemViewMode(SystemParam::SystemViewMode::GRAPHIC_SIZE);
											DOUBLE object_diameter_scalar = system_param->GetObjectDiameterScalar(system_object_type);
											FLOAT object_diameter = sys_obj->Get("Diam") * (FLOAT)object_diameter_scalar * system_scalar;
										system_param->SetSystemViewMode(svm);

									// ---- param ( note: object_object_diameter used to calculate radii )
										param[vs_system::ObjConfig::STARFIELD_TYPE]        = 1;
										param[vs_system::ObjConfig::STARFIELD_RADIUS_MAX]  = object_diameter * 2;
										param[vs_system::ObjConfig::STARFIELD_RADIUS_MIN]  = object_diameter * 1.9f;
										param[vs_system::ObjConfig::STARFIELD_STAR_COUNT]  = 10000;
										param[vs_system::ObjConfig::STARFIELD_STAR_RADIUS] = object_diameter / 150; // was 100
										result = gfx_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);

									// ---- interaction
										result = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
										result = gfx_context->SetPick3D(FALSE);

									// ---- flog & lighting OFF
										result = gfx_context->SetFog(FALSE);
										result = gfx_context->SetLit(FALSE);

									// ---- cull
										result = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CW);
								}
								break;

							default:
								{

									if ( version == ObjectGeometryVersion::CUBE )
									{
										// ---- scalar
											FLOAT scale = object_diameter / 20;

										// ---- parameters
											param[vs_system::ObjConfig::BOX_FACE_MODE]    = 0;
											param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 2;
											param[vs_system::ObjConfig::BOX_FACE_AP_REF]  = sys_obj->Get("GfxPag") + 100;
											param[vs_system::ObjConfig::BOX_FACE_AP_SECT] = sys_obj->Get("GfxSec");
											param[vs_system::ObjConfig::BOX_FACE_RED]     = 1;
											param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 1;
											param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 1;
											param[vs_system::ObjConfig::BOX_FACE_SCALE]   = scale;
											param[vs_system::ObjConfig::BOX_FACE_EXPLODE] = 0.0f;
											param[vs_system::ObjConfig::BOX_FACE_WIDTH]   = 1;
											param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  = 1;
											param[vs_system::ObjConfig::BOX_FACE_DEPTH]   = 1;
											gfx_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);

										// ---- cull
											result = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CW);
									}
									else if ( version == ObjectGeometryVersion::DOTOBJ )
									{
										// --- ?
											if ( runtime )
											{
												// ---- create
													vs_system::DotObjElement *object = new DotObjElement();

												// ---- read
													CHAR *filename = "";
													if (IsDebuggerPresent())
													{
														filename = "..\\object\\teapot\\teapot.obj";
													}
													else
													{
														filename = "object\\teapot\\teapot.obj";
													}
													vs_system::DotObjUtilities dot_obj_utilities;
													INT result = dot_obj_utilities.Read(object, filename);

												// ---- store object pointer
													LONG *l = gfx_context->GetLongParameter();
													*l = (LONG)object;
											}

										// ---- scalar
											FLOAT scale = object_diameter / 40;

										// ---- parameters
											FLOAT *param = gfx_context->GetFloatParameter();
											using namespace vs_system;
											param[ObjConfig::DOTOBJ_MODE]    =  DotObjUtilities::GfxRender::RAW;
											param[ObjConfig::DOTOBJ_TYPE]    =  2;
											param[ObjConfig::DOTOBJ_AP_REF]  =  sys_obj->Get("GfxPag") + 100;
											param[ObjConfig::DOTOBJ_AP_SECT] =  sys_obj->Get("GfxSec");
											param[ObjConfig::DOTOBJ_RED]     =  1.0f;
											param[ObjConfig::DOTOBJ_GREEN]   =  1.0f;
											param[ObjConfig::DOTOBJ_BLUE]    =  1.0f;
											param[ObjConfig::DOTOBJ_SCALE]   =  scale;
											param[ObjConfig::DOTOBJ_WIDTH]   =  1.0f;
											param[ObjConfig::DOTOBJ_HEIGHT]  =  1.25f;
											param[ObjConfig::DOTOBJ_DEPTH]   =  1.0f;
											param[ObjConfig::DOTOBJ_XOFF]    =  0.0f;
											param[ObjConfig::DOTOBJ_YOFF]    = -scale * 2;
											param[ObjConfig::DOTOBJ_ZOFF]    =  0.0f;
											result = gfx_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);

										// ---- cull
											result = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CCW);

									}
									else // ObjectType::SPHERE
									{

										// ---- parameters
											param[vs_system::ObjConfig::SPHERE_CEL_COLS] = (FLOAT)object_geometry[level_of_complexity].cels;
											param[vs_system::ObjConfig::SPHERE_CEL_ROWS] = (FLOAT)object_geometry[level_of_complexity].rows;
											param[vs_system::ObjConfig::SPHERE_TYPE]     = 2;
											param[vs_system::ObjConfig::SPHERE_AP_REF]   = sys_obj->Get("GfxPag") + 100;
											param[vs_system::ObjConfig::SPHERE_AP_SECT]  = sys_obj->Get("GfxSec");
											param[vs_system::ObjConfig::SPHERE_RADIUS]   = object_diameter / 20; // param
											param[vs_system::ObjConfig::SPHERE_RED]      = sys_obj->Get("GfxRed");
											param[vs_system::ObjConfig::SPHERE_GREEN]    = sys_obj->Get("GfxGre");
											param[vs_system::ObjConfig::SPHERE_BLUE]     = sys_obj->Get("GfxBlu");
											param[vs_system::ObjConfig::SPHERE_LAT_MIN]  = -90;
											param[vs_system::ObjConfig::SPHERE_LAT_MAX]  =  90;

											/* asteoid belt & planet rings only.... ?

											param[vs_system::ObjConfig::SPHERE_RAD_MIN]  =
													gfx_version ?
														sys_obj->Get("GfxRadMin") :
															sys_obj->Get("RadMin");
											param[vs_system::ObjConfig::SPHERE_RAD_MAX]  =
													gfx_version ?
														sys_obj->Get("GfxRadMax") :
															sys_obj->Get("RadMax");

											*/

											result = gfx_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);

										// ---- cull
											result = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CCW);
									}
									break;
								}
						}

	
					// ---- material colours
						VsColour obj_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
						VsColour obj_dif = { 1.0f, 1.0f, 1.0f, 1.0f };


					// ---- materials
						vs_system::VsMaterial obj;
						result = obj.SetAmbient(obj_amb);
						result = obj.SetDiffuse(obj_dif);


					// ---- emissive material for sun
						if ( !strcmp(object_name, "Sun" ) )
						{
							result = gfx_context->SetLit(FALSE);
							VsColour obj_emi = { 1.0f, 1.0f, 0.0f, 1.0f };
							result = obj.SetAmbient(obj_emi);
							result = obj.SetDiffuse(obj_emi);
							result = obj.SetEmissive(obj_emi);
						}


					// ---- shaders
						/*
						if ( !strcmp(object_name, "Sun") )
						{
							hr = gfx_context->SetShaded(TRUE);
							hr = gfx_context->SetShaderIndex(0);
							if ( runtime )
							{
								FLOAT shader_param[8];
								hr = img_props->GetProperty(vs_system::ImgProps::GFX_SHADER, 1, vs_system::ImgProps::BRIGHTNESS, &shader_param[0]);
								hr = img_props->GetProperty(vs_system::ImgProps::GFX_SHADER, 1, vs_system::ImgProps::CONTRAST,   &shader_param[1]);
								hr = img_props->GetProperty(vs_system::ImgProps::GFX_SHADER, 1, vs_system::ImgProps::SATURATION, &shader_param[2]);
								gfx->SetGfxShaderParam(0, shader_param);
							}
						}
						*/

					// ---- set gfx materials - VS_MAX_MATERIALS
						if ( runtime )
						{
							result = gfx->SetMaterial(mtl_index, &obj);
							result = gfx->SetMaterial(mtl_index);
							result = gfx_context->SetMaterialIndex(mtl_index);
						}


					// ---- set gfx view matrix (as not using default 0)
						result = gfx_context->SetViewMatrix(gfx_view_matrix_index);

				}


			// ---- increment materials index
				if ( runtime )
				{
					mtl_index++; // <--- BIG DEAL !!!
				}


			////////////////////////////////////////////////////////////////////////////////


			// ---- halo
				//
				// NOT textured & NOT lit... 
				//
				if ( create_halo_geometry )
				{

					if ( runtime )
					{
						handle_halo[object_index] = gfx->AddElement(
							handle_halo_dsplst,
							object_name,
							SYS_OBJECT_GROUP_ID,
							object_index,
							obj_config->Callback_MyStar_Halo_Configure
						);
					}
					else
					{
						gfx->SetElementConfigureBitmask(
							handle_halo[object_index],
							VS_ELM_SETUP_GEOMETRY
						);
					}


					// ---- context
						vs_system::ElmContext *gfx_context = NULL;
						result = gfx->GetElementContext(handle_halo[object_index], &gfx_context);


					/// ------ hmmmmmmmmmmmmmmmmmmmmm......
						result = gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
						result = gfx_context->SetConfigured(FALSE);


					// ---- properties
						result = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
						result = gfx_context->SetDisplay(FALSE);
						result = gfx_context->SetPick3D(FALSE);
						

					// ---- init orbit geometry parameters
						FLOAT *param = gfx_context->GetFloatParameter();
						param[vs_system::ObjConfig::HALO_EDGES]   = 90;
						param[vs_system::ObjConfig::HALO_RED]     = sys_obj->Get("GfxRed");
						param[vs_system::ObjConfig::HALO_GREEN]   = sys_obj->Get("GfxGre");
						param[vs_system::ObjConfig::HALO_BLUE]    = sys_obj->Get("GfxBlu");
						param[vs_system::ObjConfig::HALO_ALPHA]   = 1;
						param[vs_system::ObjConfig::HALO_X]       = 0;
						param[vs_system::ObjConfig::HALO_Y]       = 0;
						param[vs_system::ObjConfig::HALO_Z]       = 0;
						param[vs_system::ObjConfig::HALO_DEGREES] = 360;
						param[vs_system::ObjConfig::HALO_RAD_MIN] = object_diameter / 15;
						param[vs_system::ObjConfig::HALO_RAD_MAX] = object_diameter / 12;

					// ---- lighting OFF
						result = gfx_context->SetFog(TRUE);
						result = gfx_context->SetLit(FALSE);


					// ---- set gfx view matrix (as not using default 0)
						result = gfx_context->SetViewMatrix(gfx_view_matrix_index);

				}


			////////////////////////////////////////////////////////////////////////////////


			// ---- orbit
				//
				// NOT textured & NOT lit... 
				//
				if ( parent_sys_obj != NULL && create_orbit_geometry )
				{

					// ---- create orbit object
						if ( runtime )
						{
							handle_orbit[object_index] = gfx->AddElement(
												handle_orbit_dsplst,
												object_name,
												SYS_OBJECT_GROUP_ID,
												object_index,
												obj_config->Callback_MyStar_Orbit_Configure
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
								(DOUBLE)( gfx_version ?
									sys_obj->Get("GfxDis") :
										sys_obj->Get("Dist") );

							if ( (UINT)sys_obj->Get("Type") == SystemObject::Type::Moon )
							{
								orbit_distance_km_10p6 *=
									system_param->GetObjectScale();
							}


					// ---- context
						vs_system::ElmContext *gfx_context = NULL;
						result = gfx->GetElementContext(handle_orbit[object_index], &gfx_context);


					/// ------ hmmmmmmmmmmmmmmmmmmmmm......
						result = gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
						result = gfx_context->SetConfigured(FALSE);


					// ---- properties
						result = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
						result = gfx_context->SetPick3D(FALSE);


					// ---- init orbit geometry parameters
						FLOAT *param = gfx_context->GetFloatParameter();
						param[vs_system::ObjConfig::ORBIT_LINES]   = (FLOAT)object_geometry[level_of_complexity].lines;
						param[vs_system::ObjConfig::ORBIT_RADIUS]  = (FLOAT)orbit_distance_km_10p6;
						param[vs_system::ObjConfig::ORBIT_RED]     = sys_obj->Get("GfxRed");
						param[vs_system::ObjConfig::ORBIT_GREEN]   = sys_obj->Get("GfxGre");
						param[vs_system::ObjConfig::ORBIT_BLUE]    = sys_obj->Get("GfxBlu");
						param[vs_system::ObjConfig::ORBIT_X]       = 0;
						param[vs_system::ObjConfig::ORBIT_Y]       = 0;
						param[vs_system::ObjConfig::ORBIT_Z]       = 0;
						param[vs_system::ObjConfig::ORBIT_DEGREES] = system_object_type == SystemObject::Type::Asteroid ? 135.0f : 270.0f;


					// ---- lighting OFF
						result = gfx_context->SetFog(TRUE);
						result = gfx_context->SetLit(FALSE);


					// ---- set gfx view matrix (as not using default 0)
						result = gfx_context->SetViewMatrix(gfx_view_matrix_index);

				}


			////////////////////////////////////////////////////////////////////////////////


			// ---- rings ?
				//
				// Textured & NOT lit... 
				//
				BOOL create_rings_geometry = (BOOL)sys_obj->Get("Ring");
				if ( create_rings_geometry )
				{

					// ---- create rings object
						if ( runtime )
						{
							handle_ring[object_index] = gfx->AddElement(
									handle_ring_dsplst,
									object_name,
									SYS_OBJECT_GROUP_ID,
									object_index,
									obj_config->Callback_MyStar_Ring_System_Configure
								);
						}
						else
						{
							result = gfx->SetElementConfigureBitmask(
									handle_ring[object_index],
									VS_ELM_SETUP_GEOMETRY
								);
						}


					// ---- context
						vs_system::ElmContext *gfx_context = NULL;
						result = gfx->GetElementContext(handle_ring[object_index], &gfx_context);


					/// ------ hmmmmmmmmmmmmmmmmmmmmm......
						result = gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
						result = gfx_context->SetConfigured(FALSE);


					// ---- properties
						result = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
						result = gfx_context->SetPick3D(FALSE);


					// ---- init rings geometry parameters
						FLOAT object_radius = object_diameter / 2;
						FLOAT *param = gfx_context->GetFloatParameter();
						param[vs_system::ObjConfig::RING_SYSTEM_COLS]     = (FLOAT)object_geometry[level_of_complexity].cels * 2;
						param[vs_system::ObjConfig::RING_SYSTEM_ROWS]     = 1;
						param[vs_system::ObjConfig::RING_SYSTEM_TYPE]     = 2;
						param[vs_system::ObjConfig::RING_SYSTEM_AP_REF]   = sys_obj->Get("GfxRngPag") + 100;
						param[vs_system::ObjConfig::RING_SYSTEM_AP_SECT]  = sys_obj->Get("GfxRngSec");
						param[vs_system::ObjConfig::RING_SYSTEM_RED]      = sys_obj->Get("GfxRed");
						param[vs_system::ObjConfig::RING_SYSTEM_GREEN]    = sys_obj->Get("GfxGre");
						param[vs_system::ObjConfig::RING_SYSTEM_BLUE]     = sys_obj->Get("GfxBlu");

						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MIN]  =
								( gfx_version ?
										sys_obj->Get("GfxRadMin") :
											sys_obj->Get("RadMin") ) *
												(FLOAT)object_diameter_scalar;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MAX]  =
								( gfx_version ?
									sys_obj->Get("GfxRadMax") :
										sys_obj->Get("RadMax") ) *
												(FLOAT)object_diameter_scalar;


						//if (strcmp(object_name, "Uranus") == 0)
						//{
						//	sys_output->Message("Uranus ", (INT)object_diameter);
						//}

						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MIN_0] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MAX_0] = 1.0f;

						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MIN_1] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MAX_1] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MIN_2] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MAX_2] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MIN_3] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MAX_3] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MIN_4] = 0.0f;
						param[vs_system::ObjConfig::RING_SYSTEM_RAD_MAX_4] = 0.0f;


					// ---- set render states
						result = gfx_context->SetFog(TRUE);
						result = gfx_context->SetLit(FALSE);
						result = gfx_context->SetTextureMirror(TRUE);
						result = gfx_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


					// ---- set gfx view matrix (as not using default 0)
						result = gfx_context->SetViewMatrix(gfx_view_matrix_index);
						
				}

		}


	// ---- increment object index
		if ( runtime )
		{
			result = sys_obj->SetIndex(object_index);
			object_index++;
		}


	// ---- recurse
		switch (system_object_type)
		{
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Starfield:
				return VS_OK;

			default:
				{
					SystemObject *s = sys_obj->GetFirst();
					while (s)
					{
						result = InitStarSystemObjects(s);
						s = s->GetNext();
					}
				}
				break;
		}

	// ---- decrement
		recursive_depth_count--;

	return VS_OK;
}


// ========================================================================== //


INT Model::SetupStarSystemDisplay()
{

	// ---- kamera
		INT mki = pi_model->app_kamera_index;
		vs_system::AppKamera *kamera = pi_model->app_kamera[mki];


	// ---- setup
		{
			// ---- local
				INT result = 0;
				VsVector3 translate;
				FLOAT rx, ry, rz;

			// ---- setup matrix stack & transform values
				pi_model->recursive_depth_count = 0;
				pi_model->matrix_stack->Initialise();
				kamera->GetTranslate(&translate);
				kamera->GetRotate(&rx, &ry, &rz);

			// ---- push, transform, setup & pop
				pi_model->matrix_stack->Push();

				pi_model->matrix_stack->Rotate(rz, 'Z');
				pi_model->matrix_stack->Rotate(rx, 'X');
				pi_model->matrix_stack->Rotate(ry, 'Y');
				pi_model->matrix_stack->Translate(
								translate.x,
								translate.y,
								translate.z
							);

				pi_model->SetupStarSystemDisplay(pi_model->system_object);
				pi_model->matrix_stack->Pop();

		}


	// ---- kamera
		if ( kamera->HasBeenUpdated() )
		{
			INT result = SetMyKamera(mki);
			kamera->SetHasBeenUpdated(FALSE);
		}


	return VS_OK;
}


// ========================================================================== //


INT Model::PI_Model::SetupStarSystemDisplay(SystemObject *sys_obj)
{

	// ---- local
		INT result = VS_OK;


	// ---- gfx version ?
		BOOL gfx_version = system_param->GetGfxVersion();


	// ---- parent
		SystemObject *parent_sys_obj = sys_obj->GetParent();
		UINT   parent_type = parent_sys_obj == NULL ? 0 : (UINT)parent_sys_obj->Get("Type");
		DOUBLE parent_diameter_scalar = 1;
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

			case SystemObject::Type::Moon:
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
			case SystemObject::Type::Starfield:
				break;
		}


	// ---- event_id
		INT event_id = sys_context->GetDWord(vs_system::SysContext::WINDOW_TRIGGER);


	// ---- time - with pause ON/OFF
		DOUBLE system_time_elapsed =
			event_id != 0 ?
				img_capture_elapsed_ms += 1 :
					sys_context->GetDouble(vs_system::SysContext::TIME_MS_ELAPSED);


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
				(DOUBLE)( gfx_version ? sys_obj->Get("GfxOrbPer") : sys_obj->Get("OrbPer") ) * 24;
		DOUBLE orbit_start_hours = 
				(DOUBLE)( gfx_version ? sys_obj->Get("GfxOrbSta") : sys_obj->Get("OrbSta") ) * 24;
		DOUBLE orbit_degrees = fabs(orbit_period_hours) < 0.000001 ? 0 :
				( hours_elapsed + orbit_start_hours ) / orbit_period_hours * 360;


	// ---- orbit distance
		DOUBLE orbit_distance_km_10p6 =
			(DOUBLE)( gfx_version ? sys_obj->Get("GfxDis") : sys_obj->Get("Dist") );

			if ( (UINT)sys_obj->Get("Type") == SystemObject::Type::Moon )
			{
				DOUBLE parent_diameter =
						parent_sys_obj == NULL ? 0 :
							(DOUBLE)( gfx_version ? parent_sys_obj->Get("GfxDia") : parent_sys_obj->Get("Diam") ) *
								parent_diameter_scalar * system_param->GetGlobalScale();

				orbit_distance_km_10p6 *=
					system_param->GetObjectScale();
			}


	// ---- day rotation
		DOUBLE daylen_scalar    = system_param->GetDayLenScalar();
		DOUBLE day_length_hours = (DOUBLE)( gfx_version ? sys_obj->Get("GfxDayLen") : sys_obj->Get("DayLen") );
		DOUBLE day_start_hours  =
				(DOUBLE)( gfx_version ? sys_obj->Get("GfxDaySta") : sys_obj->Get("DaySta") );

		DOUBLE day_degrees      = fabs(day_length_hours) < 0.000001 ? 0 :
				hours_elapsed / day_length_hours * 360 ;

		       day_degrees     += fabs(day_start_hours) < 0.000001 ? 0 :  
					day_start_hours / day_length_hours * 360;

			   day_degrees     *= daylen_scalar;


	// ---- matrix stuff


	// ---- set parent world matrix
		VsMatrix *parent_matrix = &matrix_stack->stack[matrix_stack->index];


	// ============ PUSH ORBIT ============
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


			// ---- obliquity (TBD add inclination)
				CHAR *object_name = sys_obj->GetName();
				DOUBLE orbit_obliquity = !strcmp("Sun", object_name) ?  0 : sys_obj->Get("OrbObl");
				matrix_stack->Rotate((FLOAT)  orbit_obliquity, 'Z');


			// ============ PUSH DAY DEGREES ============
				recursive_depth_count++;
				matrix_stack->Push();
				matrix_stack->Rotate((FLOAT) -day_degrees, 'Y');


					// ---- locate
						VsMatrix  *sys_obj_matrix = &matrix_stack->stack[matrix_stack->index];
						VsVector3 sys_obj_vector = { sys_obj_matrix->m[3][0] , sys_obj_matrix->m[3][1] , sys_obj_matrix->m[3][2] };


					// ---- get object handle
						UINT system_object_type = ((INT)(sys_obj->Get("Type")));
						INT  object_index = sys_obj->GetIndex();
						BOOL has_geometry_version = FALSE;
						INT  handle = NULL;
						switch ( system_object_type )
						{
							case SystemObject::Type::AstRing:
								handle = handle_asteroid[object_index];
								break;
							case SystemObject::Type::Starfield:
								handle = handle_starfield[object_index];
								break;
							default:
								{
									has_geometry_version = TRUE;
									switch ( object_geometry_version )
									{
										case ObjectGeometryVersion::SPHERE:
											handle = handle_sphere[object_index];
											break;
										case ObjectGeometryVersion::CUBE:
											handle = handle_cube[object_index];
											break;
										case ObjectGeometryVersion::DOTOBJ:
											handle = handle_dotobj[object_index];
											break;
										default:
											break;
									}
								}
								break;
						}


					// ---- object ? 
						if ( handle != NULL )
						{

							// ---- local
								vs_system::ElmContext *elm_context = NULL;


							// ---- object matrix
								result = gfx->GetElementContext(handle, &elm_context);
								elm_context->SetUseMatrix(TRUE);
								elm_context->SetMatrix(sys_obj_matrix);


							// ---- geometry ?
								if ( has_geometry_version )
								{

									// ---- rings matrix 
										result = gfx->GetElementContext(handle_ring[object_index], &elm_context);
										elm_context->SetUseMatrix(TRUE);
										elm_context->SetMatrix(sys_obj_matrix);


									// ---- target (or view mode changed) ?
										UINT target_id = GetTargetObjectId();
										if ( target_id != SystemParam::CameraTarget::NONE )
										{

											// ---- zoom ?
												if ( !strcmp(elm_context->GetName().c_str(), target_planet_name[target_id]) )
												{

													// ---- kamera
														vs_system::AppKamera *kamera = app_kamera[app_kamera_index];

													// ---- system
														FLOAT system_scalar = (FLOAT)system_param->GetGlobalScale();

													// ---- object
														UINT type = sys_obj == NULL ? 0 : (UINT)sys_obj->Get("Type");
														DOUBLE object_diameter_scalar = system_param->GetObjectDiameterScalar(type);

														FLOAT object_diameter =
															(FLOAT)(
																(gfx_version ? sys_obj->Get("GfxDia") : sys_obj->Get("Diam")) *
																	object_diameter_scalar * system_scalar
																);

													// ---- new target ?
														INT  target_id_prev = GetTargetObjectIdPrev();
														BOOL new_target = target_id != target_id_prev;

													// ---- runtime - set target zoom limits
														if ( target_id_prev == SystemParam::CameraTarget::NONE_RUNTIME )
														{
															kamera->SetTargetZoomMinimum(object_diameter *  0.3f);
															kamera->SetTargetZoomMaximum(object_diameter *  3.0f);
														}

													// ---- runtime ? - catch new target
														new_target = target_id_prev == SystemParam::CameraTarget::NONE_RUNTIME ? FALSE : new_target;

													// ---- new view ?
														BOOL new_view_mode = system_param->GetSystemViewMode();
														BOOL new_view_mode_changed = system_param->GetSystemViewModeChanged();

													// ---- new target
														if ( new_target || new_view_mode_changed)
														{

															//if ( strcmp(object_name, "Sun") == 0 )
															//	s_pi_model->sys_output->Message(object_name, "", object_diameter_scalar);

															// ---- zoom
																kamera->SetTargetZoomMinimum(object_diameter *  0.3f);
																kamera->SetTargetZoomMaximum(object_diameter *  10.0f);

															// ---- change
																kamera->SetTargetChangeZoomPower(1.5f);
																kamera->SetTargetChangeZoomDenom(40.0f);


															// ---- change speed
																if ( new_view_mode != 2 )
																{
																	kamera->SetTargetChangeSpeed(1);
																}
																else
																{
																	kamera->SetTargetChangeSpeed(1.5f);
																}

																if (new_view_mode_changed)
																{
																	kamera->SetTargetZoomMinMax();
																}

															// ---- update
																system_param->SetSystemViewModeChanged(FALSE);

															// ---- target
																SetTargetObjectId(target_id);

														}

													// ---- position
														kamera->SetTargetPosition(&sys_obj_vector, new_target);

												}
										}


									// ---- orbit ? - use parent matrix...
										SystemObject *parent = sys_obj->GetParent();
										if ( parent != NULL && handle_orbit[object_index] != NULL )
										{

											// ============ PUSH ORBIT ============
												matrix_stack->Push();

													VsMatrix *orbit_matrix = &matrix_stack->stack[matrix_stack->index];
													vs_system::VsMatrixOp mop;
													mop.Copy(orbit_matrix, parent_matrix);
													DOUBLE orbit_inclination = sys_obj->Get("OrbInc");
													matrix_stack->Rotate( (FLOAT)  orbit_inclination, 'X');
													matrix_stack->Rotate( (FLOAT) -orbit_degrees, 'Y');
													result = gfx->GetElementContext(handle_orbit[object_index], &elm_context);
													result = elm_context->SetUseMatrix(TRUE);
													result = elm_context->SetMatrix(orbit_matrix);

											// ============ POP ORBIT ============
												matrix_stack->Pop();

										}
								}
						}


				// ============ POP DAY DEGREES ============
					recursive_depth_count--;
					matrix_stack->Pop();


				//
				// ============ BILLBOARDING HALO ============
				//

				// ---- is has geometry && is halo ?
					if ( has_geometry_version && handle_halo[object_index] != NULL )
					{

						// ---- context
							vs_system::ElmContext *elm_context = NULL;
							result = gfx->GetElementContext(handle_halo[object_index], &elm_context);


						// ---- display ?
							BOOL display = elm_context->GetDisplay();
							if ( display) 
							{

								// ============ PUSH HALO ============
									matrix_stack->Push();

										VsMatrix *halo_matrix = &matrix_stack->stack[matrix_stack->index];

										VsVector3 kamera_position;
											app_kamera[app_kamera_index]->GetPosition(&kamera_position);

										FLOAT pos_x = sys_obj_vector.x,
											pos_y = sys_obj_vector.y,
											pos_z = sys_obj_vector.z;

										VsVector3 look;
											look.x = kamera_position.x - pos_x;
											look.y = kamera_position.y - pos_y;
											look.z = kamera_position.z - pos_z;
											FLOAT len = look.Normalise();

										VsVector3 up;
											app_kamera[app_kamera_index]->GetUp(&up);

										VsVector3 right;
											right.CrossProduct(&look, &up);
											len = right.Normalise();

										up.CrossProduct(&look, &right);
											len = up.Normalise();

										FLOAT *t = &halo_matrix->m[0][0];

										*(t+ 0) = right.x; *(t+ 1) = right.y; *(t+ 2)  = right.z; *(t+ 3) = 0;
										*(t+ 4) = up.x;    *(t+ 5) = up.y;    *(t+ 6)  = up.z;    *(t+ 7) = 0;
										*(t+ 8) = look.x;  *(t+ 9) = look.y;  *(t+ 10) = look.z;  *(t+11) = 0;
										*(t+12) = pos_x;   *(t+13) = pos_y;   *(t+ 14) = pos_z;   *(t+15) = 1;

										result = elm_context->SetUseMatrix(TRUE);
										result = elm_context->SetMatrix(halo_matrix);

								// ============ POP HALO ============
									matrix_stack->Pop();
							}
					}


				// ---- recurse
					SystemObject *s = sys_obj->GetFirst();
					while (s)
					{
						result = SetupStarSystemDisplay(s);
						s = s->GetNext();
					}


		// ============ POP ORBIT ============
			recursive_depth_count--;
			matrix_stack->Pop();


	return VS_OK;
}


// ========================================================================== //


BOOL Model::IsObjectGeometry(INT handle_display_list)
{
	INT handle_geometry_display_list[3] =
		{
			pi_model->handle_sphere_dsplst,
			pi_model->handle_cube_dsplst,
			pi_model->handle_dotobj_dsplst
		};

	for (UINT i = 0; i < 3; i++)
	{
		if ( handle_display_list == handle_geometry_display_list[i] )
			return TRUE;
	}

	return FALSE;

}


BOOL Model::IsSelectedObjectGeometry(INT handle_display_list)
{
	INT handle_geometry_display_list[3] =
		{
			pi_model->handle_sphere_dsplst,
			pi_model->handle_cube_dsplst,
			pi_model->handle_dotobj_dsplst
		};

	for (UINT i = 0; i < 3; i++)
	{
		if ( handle_display_list == handle_geometry_display_list[i] )
		{
			if ( pi_model->object_geometry_version == i + 1 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}


	// ---- exclude unwanted geometry versions
	/*
	switch ( pi_model->object_geometry_version )
	{
		case PI_Model::CUBE:
			if ( handle_display_list == pi_model->handle_sphere_dsplst || handle_display_list == pi_model->handle_dotobj_dsplst )
				return VS_OK;
			break;
		case PI_Model::DOTOBJ:
			if ( handle_display_list == pi_model->handle_sphere_dsplst || handle_display_list == pi_model->handle_cube_dsplst )
				return VS_OK;
			break;
		case PI_Model::SPHERE:
			if ( handle_display_list == pi_model->handle_cube_dsplst || handle_display_list == pi_model->handle_dotobj_dsplst )
				return VS_OK;
			break;
	}
	*/
// ========================================================================== //
