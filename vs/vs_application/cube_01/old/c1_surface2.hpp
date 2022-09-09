////////////////////////////////////////////////////////////////////////////////

// ---------- c1_surface..hpp ----------
/*!
\file c1_surface..hpp
\brief Implementation of the Model surface methods
\author Gareth Edwards
*/


double Model::GetShaderValue2(SurfaceShaderData *shader_data)
{

	// ---- result
		double returned_value = 0;


	// ---- distance
		VsVector3D  *light  = shader_data->light;
		VsVector3D  *vertex = &shader_data->vertex;
		double xd = light->x - vertex->x;
		double yd = light->y - vertex->y;
		double zd = light->z - vertex->z;
		double dist = sqrt(xd*xd + yd*yd + zd*zd);


	// ---- incident ?
		if ( dist > 0.0f )
		{
			VsVector3D *normal = &shader_data->normal;
			double attenuation = shader_data->intensity / (dist * dist);
			double dva = 0.0;
			xd = xd * attenuation; dva += normal->x * xd;
			yd = yd * attenuation; dva += normal->y * yd;
			zd = zd * attenuation; dva += normal->z * zd;
			if ( dva > 0.00001f )
			{
				returned_value = dva > 1 ? 1 : dva;
			}
		}


	// ---- clamp
		returned_value = returned_value < 0 ? 0 : (returned_value > 1 ? 1 : returned_value);

	return returned_value;

};


double Model::GetShaderValueUsingCalculatedNormal2(
		SurfaceShaderData   *shader_data,
		SurfaceSolutionData *solution_data
	)
{

	// ---- distance
		VsVector3D  *light = shader_data->light;
		VsVector3D  *vertex = &shader_data->vertex;
		double xd = light->x - vertex->x;
		double yd = light->y - vertex->y;
		double zd = light->z - vertex->z;
		double dist = sqrt(xd*xd + yd * yd + zd * zd);


	// ---- unit vector pointing at light
		VsVector3D light_direction = { xd/dist, yd/dist, zd/dist };


	// ---- angle required to rotate light direction vector into xy plane
		double y_rot_radians = atan(light_direction.z / light_direction.x);
		y_rot_radians = light_direction.x > 0 ?
				y_rot_radians :
					VS_PI + y_rot_radians;


	// --- rotate light direction vector +ve about y axis
		// x' = z*sin q + x*cos q
		// z' = z*cos q - x*sin q
		VsVector3D xy_plane =
		{
			light_direction.z*sin(y_rot_radians) + light_direction.x*cos(y_rot_radians),
			light_direction.y,
			light_direction.z*cos(y_rot_radians) - light_direction.x*sin(y_rot_radians),
		};


	// --- caluclated normal solution 1: rotate light direction vector +ve about z axis
		// x' = x*cos q - y*sin q
		// y' = x*sin q + y*cos q
		double value = solution_data->pixel_value;
		double pixel_value = acos(value);
		VsVector3D solution_1 =
		{
			xy_plane.x*cos(pixel_value) - xy_plane.y*sin(pixel_value),
			xy_plane.x*sin(pixel_value) + xy_plane.y*cos(pixel_value),
			0
		};
		double aa = acos(0.998f);
		double bb = acos(0.95f);
		double cc = acos(0.9f);
		float fred = 2;




	// --- calculated normal solution 2: rotate light direction vector -ve about z axis
		/*
		VsVector3D solution_2 =
		{
			xy_plane.x*cos(-pixel_value) - xy_plane.y*sin(-pixel_value),
			xy_plane.x*sin(-pixel_value) + xy_plane.y*cos(-pixel_value),
			0
		};
		*/

	// --- rotate solutions 1 and 2 -ve about y axis into light direction plane
		VsVector3D *solution = NULL;
		
		solution = solution_data->solution_1;
			solution->x = solution_1.z*sin(-y_rot_radians) + solution_1.x*cos(-y_rot_radians);
			solution->y = solution_1.y;
			solution->z = solution_1.z*cos(-y_rot_radians) - solution_1.x*sin(-y_rot_radians);

		/*
		solution = solution_data->solution_2;
			solution->x = solution_2.z*sin(-y_rot_radians) + solution_2.x*cos(-y_rot_radians);
			solution->y = solution_2.y;
			solution->z = solution_2.z*cos(-y_rot_radians) - solution_2.x*sin(-y_rot_radians);
		*/

	// ---- get shader value with calculated normal solution 1
		SurfaceShaderData returned_shader_data;
		returned_shader_data.light     =  light;
		returned_shader_data.intensity =  shader_data->intensity;
		returned_shader_data.vertex    = *vertex;
		returned_shader_data.normal    = *solution_data->solution_1;
		double returned_shader_value   = GetShaderValue2(&returned_shader_data);

	return returned_shader_value;
};


INT Model::InitSurfaceSceneParam2()
{

	// ---- init lights
		double intensity = pi_model->surface_lights_intensity = 50;
		double radius = pi_model->surface_lights_radius = 200;

		VsVector3D surface_lights[4] =
		{
			{ -radius, radius,     0 },
			{ -radius, radius+50,  0 },
			{ -radius, radius+100, 0 },
			{ -radius, radius+150, 0 }
		};
		for (UINT i=0; i<4; i++)
		{
			pi_model->surface_lights[i] = surface_lights[i];
		}


	// ---- test: init vec
		VsVector3D surface_vertex = { -37.5f, 2.5f, 0 };


	// ---- test: init normal
		VsVector3D surface_normal;
		double z_rotate = 30;
		double y_rotate =  0;
		double xd = -sin(z_rotate * VS_RADIANS_PER_DEGREE);
		double yd =  cos(z_rotate * VS_RADIANS_PER_DEGREE);
		double zd = 0;
		surface_normal.x =  xd * cos(y_rotate * VS_RADIANS_PER_DEGREE);
		surface_normal.y =  yd;
		surface_normal.z = -xd * sin(y_rotate * VS_RADIANS_PER_DEGREE);


	// ---- test: calculate shaded values
		pi_model->shader_data.light     = &pi_model->surface_lights[0];
		pi_model->shader_data.intensity = intensity;
		pi_model->shader_data.vertex    = surface_vertex;
		pi_model->shader_data.normal    = surface_normal;

		for (UINT i = 0; i<4; i++)
		{
			pi_model->shader_data.light = &pi_model->surface_lights[i];
			pi_model->surface_pixel_value[i]  = GetShaderValue2(&pi_model->shader_data);
		}


	return VS_OK;
}


INT Model::InitSurfaceBufferParam2()
{

	// ---- "matched" light solution vectors
		VsVector3D solution_1[4];
		VsVector3D solution_2[4];


	// ---- y iteration
		double min_y_value = -10.0f;
		double max_y_value =  10.0f;
		double mid_y_value =   0.0f;
		double result_y    =   0.0f;


	// ---- shader iteration
		double surface_shader_value = 0;
		double min_test_value = 0.0f;
		double max_test_value = 1.0f;
		double mid_test_value = 0.0f;
		double result_value   = 0.0f;


	// ---- iteratively calculate y value for a given surface shader
		//  value solution, during which two "matched" surface normal
		//  solutions and a dot product are derived
		//
		//  select y value range depending on value and
		//  sense (+/- clockwise) of the dot product  
		//

		static const int num_y_iteration = 41;

		struct T_Info
		{
			double target_value = 0;
			double found_value = 0;
			double result_value = 0;
			VsVector3D normal;
		};

		T_Info shader_info[num_y_iteration][4];
		double  shader_y[num_y_iteration];

		for (UINT y_iteration = 0; y_iteration < num_y_iteration; y_iteration++)
		{

			// ---- y
				//mid_y_value = (max_y_value + min_y_value) / 2.0f;
				mid_y_value = (double)y_iteration - (double)(num_y_iteration-1)/2.0f;
				mid_y_value /= 2;


			// ---- set y
				pi_model->shader_data.vertex.y = mid_y_value;


			// ---- light sequence : left & right then up & down
				UINT light_sequence_index[4] = { 0, 2, 1, 3 };
				UINT light_index = 0;


			// ---- for each pair of lights
				for (UINT sequence_index = 0; sequence_index<4; sequence_index++)
				{

					// --- init light
						light_index = light_sequence_index[sequence_index];
						pi_model->shader_data.light = &pi_model->surface_lights[light_index];


					// ---- init solution store
						pi_model->solution_data.solution_1 = &solution_1[light_index];
						pi_model->solution_data.solution_2 = &solution_2[light_index];


					// ---- iteratively calculate non attenuated surface shader value
						//  for a give shader value, and from this derive surface normal
						//  solutions 1 & 2
						//
						//  note: min of surface shader dot product value (e.g. 0.3334) 
						//        and max shader dot product value PI/2 (90 degrees)
						//
						surface_shader_value = pi_model->surface_pixel_value[light_index];
						min_test_value = surface_shader_value;
						max_test_value = 1.01f; 
						mid_test_value = 0.0f;
						result_value   = 0.0f;
						for (UINT value_iteration = 0; value_iteration < 20; value_iteration++)
						{

							// ---- test mid value
								mid_test_value = (max_test_value + min_test_value) / 2.0f;
								pi_model->solution_data.pixel_value = mid_test_value;

								if (value_iteration == 19)
								{
									pi_model->solution_data.pixel_value = surface_shader_value;
									float wilma = 1;
								}

								result_value = GetShaderValueUsingCalculatedNormal2(
										&pi_model->shader_data,
										&pi_model->solution_data
									);

							// --- update minimax
								if ( result_value < surface_shader_value)
								{
									max_test_value = mid_test_value;
								}
								else
								{
									min_test_value = mid_test_value;
								}
						}

						shader_info[y_iteration][light_index].target_value = surface_shader_value;
						shader_info[y_iteration][light_index].result_value = result_value;
						shader_info[y_iteration][light_index].found_value  = mid_test_value;
						shader_info[y_iteration][light_index].normal       = *pi_model->solution_data.solution_1;

				}
				shader_y[y_iteration] = mid_y_value;

				double fred = 2;
		}


		// ---- save
			std::ofstream myfile("surf.csv");
			if (myfile.is_open())
			{

				CHAR msg[128];

				{
					sprintf(msg, "Y, ");
					myfile << msg;
					for (UINT l = 0; l < 4; l++)
					{
						sprintf(msg, "TV, RV, FV, XN, YN, ZN");
						myfile << msg;
						myfile << ",";
					}
					myfile << "\n";
				}

				for (UINT y = 0; y < num_y_iteration; y++)
				{
					sprintf(msg, "%6.4f, ", shader_y[y]);
					myfile << msg;
					for (UINT l = 0; l < 4; l++)
					{
						sprintf(msg, "%8.6f, %8.6f, %8.6f, %8.6f, %8.6f, %8.6f",
								shader_info[y][l].target_value,
								shader_info[y][l].result_value,
								shader_info[y][l].found_value,
								shader_info[y][l].normal.x,
								shader_info[y][l].normal.y,
								shader_info[y][l].normal.z
							);
						myfile << msg;
						myfile << ",";
					}
					myfile << "\n";
				}


				myfile.close();
			}
		

		double fred = 2;

	return VS_OK; 
}


INT Model::SetupSurfaceDisplay2()
{

	// ---- local
		vs_system::VsMatrixStack *vms = pi_model->matrix_stack;
		vms->Initialise();


	// ---- time
		double rot_degrees =
			pi_model->sys_context->GetDouble(vs_system::SysContext::TIME_MS_ELAPSED)
			/ 20;


	// ---- push
		vms->Push();


	// ---- matrix stuff
		VsMatrix  *m = &vms->stack[vms->index];
		vms->Rotate( (FLOAT)-rot_degrees, 'Y');
		vms->Translate( 0, 0, 0);
		vms->Rotate( 0.0f, 'Y');


	// ---- pop
		vms->Pop();

	return VS_OK;
}


INT Model::SetupSurfaceObjects2()
{

	// ---- create display list
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- create display list ?
		if ( pi_model->handle_dspl == NULL )
		{
			pi_model->handle_dspl = gfx->AddDisplayList("Scene", 1, 1);
		}


	return VS_OK;
}