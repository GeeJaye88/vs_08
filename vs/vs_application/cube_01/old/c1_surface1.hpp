////////////////////////////////////////////////////////////////////////////////

// ---------- c1_surface..hpp ----------
/*!
\file c1_surface..hpp
\brief Implementation of the Model surface methods
\author Gareth Edwards
*/


double Model::GetShaderValue_WNA(SurfaceShaderData *shader_data)
{

	// ---- result
		double returned_value = 0;


	// ---- distance
		VsVector3D  *light  = shader_data->light;
		VsVector3D  *vertex = &shader_data->vertex;
		double xd = light->x - vertex->x;
		double yd = light->y - vertex->y;
		double zd = light->z - vertex->z;
		double len = sqrt(xd*xd + yd*yd + zd*zd);


	// ---- incident ?
		if ( len > 0.0f )
		{
			VsVector3D *normal = &shader_data->normal;
			double attenuation = 1 / len;
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


double Model::GetShaderSolution_WNA(
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
		double len = sqrt(xd*xd + yd * yd + zd * zd);


	// ---- unit vector pointing at light
		VsVector3D light_direction = { xd/len, yd/ len, zd/ len };


	// ---- angle required to rotate light direction vector into xy plane
		double y_rot_radians = atan(light_direction.z / light_direction.x);
		y_rot_radians = light_direction.x > 0 ?
				y_rot_radians :
					VS_PI + y_rot_radians;


	// --- rotate light direction vector +ve about y axis to xy plane
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
		double test_shader_angle_in_radians = acos(solution_data->pixel_value);
		VsVector3D solution_1 =
		{
			xy_plane.x*cos(test_shader_angle_in_radians) - xy_plane.y*sin(test_shader_angle_in_radians),
			xy_plane.x*sin(test_shader_angle_in_radians) + xy_plane.y*cos(test_shader_angle_in_radians),
			0
		};


	// --- rotate solutions 1 -ve about y axis back to light direction plane
		VsVector3D *solution = NULL;
		
		solution = solution_data->solution_1;
			solution->x = solution_1.z*sin(-y_rot_radians) + solution_1.x*cos(-y_rot_radians);
			solution->y = solution_1.y;
			solution->z = solution_1.z*cos(-y_rot_radians) - solution_1.x*sin(-y_rot_radians);


	// ---- get shader value with calculated normal solution 1
		SurfaceShaderData returned_shader_data;
		returned_shader_data.light     =  light;
		returned_shader_data.intensity =  shader_data->intensity;
		returned_shader_data.vertex    = *vertex;
		returned_shader_data.normal    = *solution_data->solution_1;
		double returned_shader_value   = GetShaderValue_WNA(&returned_shader_data);

	return returned_shader_value;
};


INT Model::SetShaderParameters_WNA()
{

	// ---- init lights
		double intensity = pi_model->surface_lights_intensity = 1;
		double radius = pi_model->surface_lights_radius = 200;

		VsVector3D surface_lights[4] =
		{
			{ -radius,  radius,  0      },
			{  0,       radius,  radius },
			{  radius,  radius,  0      },
			{  0,       radius, -radius }
		};
		for (UINT i=0; i<4; i++)
		{
			pi_model->surface_lights[i] = surface_lights[i];
		}


	// ---- init surface
		VsVector3D surface_vertex = { -37.5f, 2.5, 0 };
		VsVector3D surface_normal;
		double z_rotate = 30;
		double y_rotate =  0;
		double xd = -sin(z_rotate * VS_RADIANS_PER_DEGREE);
		double yd =  cos(z_rotate * VS_RADIANS_PER_DEGREE);
		double zd = 0;
		surface_normal.x =  xd * cos(y_rotate * VS_RADIANS_PER_DEGREE);
		surface_normal.y =  yd;
		surface_normal.z = -xd * sin(y_rotate * VS_RADIANS_PER_DEGREE);


	// ---- init shader
		pi_model->shader_data.light     = &pi_model->surface_lights[0];
		pi_model->shader_data.intensity = intensity;
		pi_model->shader_data.vertex    = surface_vertex;
		pi_model->shader_data.normal    = surface_normal;


	// ---- calculate shader value for each light
		for (UINT i = 0; i<4; i++)
		{
			pi_model->shader_data.light = &pi_model->surface_lights[i];
			pi_model->surface_pixel_value[i]  = GetShaderValue_WNA(&pi_model->shader_data);
		}


	return VS_OK;
}


double Model::GetShaderHeight_WNA()
{

	// ---- local
		VsVector3D solution_1[4];
		double y_value = 0.0f;
		double result_value = 0.0f;


	// ---- store
		static const int num_y_iteration = 41;
		struct T_Info
		{
			double value = 0;
			VsVector3D normal;
		};
		T_Info shader_info[num_y_iteration][4];
		double  shader_y[num_y_iteration];


	// ---- y value
		for (UINT y_iteration = 0; y_iteration < num_y_iteration; y_iteration++)
		{

			// ---- y
				//mid_y_value = (max_y_value + min_y_value) / 2.0f;
				y_value = (double)y_iteration - (double)(num_y_iteration-1)/2.0f;
				y_value /= 2; // for 0.5 sample


			// --- shader
				pi_model->shader_data.vertex.y = y_value;


			// ---- for each light
				for ( UINT light_index =0; light_index<1; light_index++)
				{

					// --- shader solution
						pi_model->shader_data.light = &pi_model->surface_lights[light_index];
						pi_model->solution_data.solution_1 = &solution_1[light_index];
						pi_model->solution_data.pixel_value = pi_model->surface_pixel_value[light_index];
						result_value = GetShaderSolution_WNA(
								&pi_model->shader_data,
								&pi_model->solution_data
							);

					// ---- store
						shader_info[y_iteration][light_index].value  =  result_value;
						shader_info[y_iteration][light_index].normal = *pi_model->solution_data.solution_1;
						shader_y[y_iteration] = y_value;

				}

		}

		// ---- temp - search for shader solution with least difference and return y value
			double legal_tolerance = 0.001;
			double difference_tolerance = 0.001;
			for (UINT y_iteration = 0; y_iteration < num_y_iteration; y_iteration++)
			{
				// ---- for each light
					for (UINT light_index = 0; light_index < 4; light_index++)
					{
							if (
								pi_model->surface_pixel_value[light_index] > legal_tolerance &&
								pi_model->surface_pixel_value[light_index] < (1 - legal_tolerance)
								)
							{
								if ( y_iteration == 25 )
									float fred = 1;

								double difference = fabs(
										pi_model->surface_pixel_value[light_index] * 100000 -
										shader_info[y_iteration][light_index].value
									);
								if (difference < difference_tolerance)
								{
									double height = shader_y[y_iteration];
								}
							}
					}
			}

	// no solution
		return 0;
}


INT Model::SetupSurfaceDisplay1()
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


INT Model::SetupSurfaceObjects1()
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