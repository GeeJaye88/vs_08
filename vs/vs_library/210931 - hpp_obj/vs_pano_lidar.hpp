////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_lidar.h ----------
/*!
\file vs_pano_lidar.h
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanoLidar_Configure ----------
INT ObjConfig::Callback_PanoLidar_Configure(
		vs_system::ElmContext *context
	)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local parameter data
		FLOAT *pd = context->GetFloatParameter();
		BOOL lidar_spherical = 1;
		UINT lidar_ap_ref    = (UINT)pd[MESH_AP_REF];


	// ---- local csv lidar parameters
		LONG  *ld = context->GetLongParameter();
		FLOAT *csv_fb      = (FLOAT *) *(ld + 0);
		UINT   csv_rows    = (UINT) *(ld + 1);
		UINT   csv_columns = (UINT) *(ld + 2);
		UINT   csv_extent  = (UINT) *(ld + 3);
		UINT   csv_rpr     = (UINT) *(ld + 4);
		BOOL   csv_read    = (BOOL) *(ld + 5);

		UINT   csv_beams   = csv_rows / csv_rpr;
		UINT   csv_pitch   = csv_columns;


	// ---- local pano lidar parameters
		UINT  cels = 1;
		UINT  lidar_beams = csv_rpr;
		UINT  lidar_cols  = csv_extent / (csv_rpr * csv_columns);


	// ---- vertex
		UINT total_vertices = cels * cels * 3 * 2 * lidar_cols * lidar_beams;
		UINT total_elements = cels * cels * 2 * lidar_cols * lidar_beams;


	// ---- atlas page
		context->SetAtlasPage(0, pi_obj_pano->pano_texture_lid);


	// ---- IF -- overide -- THEN
		if (!context->GetOveride())
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(false);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(true);
			context->SetVisible(true);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer
		context->SetVertexCount(total_vertices);
		context->SetVertexFormat((UINT)(VS_XYZ | VS_TEX1));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_PanoLidar_GetVertexBuffer);


	// ---- primitive parameters
	context->SetPrimitiveBytes(
			context->GetVertexSize() *
			context->GetVertexCount()  *
			sizeof(float)
		);
	context->SetPrimitiveCount(total_elements);
	context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
	context->EndConfig();

	return VS_OK;
}


// ----------  Callback_PanoLidar_GetVertexBuffer ----------
/*!
\note arameters as per pano sphere object
*/
INT ObjConfig::Callback_PanoLidar_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- parameter data ----
		FLOAT *pd = context->GetFloatParameter();
		BOOL  lidar_spherical = 1;//(BOOL)pd[MESH_TYPE];    // 2
		UINT  lidar_ap_ref    = (UINT)pd[MESH_AP_REF];      // 3
		FLOAT lidar_y_offset  = 0.0f;


	// ---- local csv lidar parameters
		LONG  *ld = context->GetLongParameter();
		FLOAT *csv_fb      = (FLOAT *) *(ld + 0);
		UINT   csv_rows    = (UINT) *(ld + 1);
		UINT   csv_columns = (UINT) *(ld + 2);
		UINT   csv_extent  = (UINT) *(ld + 3);
		UINT   csv_rpr     = (UINT) *(ld + 4);
		BOOL   csv_read    = (BOOL) *(ld + 5);

		UINT   csv_beams   = csv_rows / csv_rpr;
		UINT   csv_pitch   = csv_columns;


	// ---- random cloud ? (no lidar csv data)
		BOOL random_cloud = TRUE;
		if ( csv_read ) random_cloud = FALSE;


	// ---- local lidar parameters

		UINT  lidar_cels  = 1;
		UINT  lidar_beams = csv_rpr;
		UINT  lidar_cols  = csv_extent / (csv_rpr * csv_columns);

		FLOAT lidar_ang_radius    = 0.1f;
		FLOAT lidar_ang_diameter  = lidar_ang_radius * 2;

		FLOAT lidar_latitude_min  = pi_obj_pano->min_angle;
		FLOAT lidar_latitude_max  = pi_obj_pano->max_angle;

		FLOAT lidar_latitude_tot  = lidar_latitude_max - lidar_latitude_min;


	// ---- local pano v
		FLOAT v_radius_min    = pi_obj_pano->min_radius;
		FLOAT v_radius_max    = pi_obj_pano->max_radius;
		FLOAT v_radius_total  = v_radius_max - v_radius_min;


	// ---- local pano parameters
		FLOAT aspect_ratio    = pi_obj_pano->aspect_ratio;
		FLOAT sphere_radius   = pi_obj_pano->sphere_radius;
		FLOAT u_flip          = U_FLIP_LOGIC;
		FLOAT v_flip          = V_FLIP_LOGIC;
		FLOAT u_centre        = pi_obj_pano->u_offset + 0.5f;
		FLOAT v_centre        = pi_obj_pano->v_offset + 0.5f;


	// ---- vertex 
		UINT  vertex_size   = context->GetVertexSize();
		UINT  vertex_format = context->GetVertexFormat();


	// ---- mesh object
		UINT  lidar_mesh_vertices = lidar_cels + 1;
		FLOAT lidar_mesh_incr = 1 / (FLOAT)lidar_cels;

		FLOAT circumference = 2 * (FLOAT)VS_PI * v_radius_max;
		FLOAT mesh_u_size   = lidar_ang_diameter / circumference * 0.01f;
		FLOAT mesh_u_incr   = mesh_u_size / (FLOAT)lidar_cels;
		FLOAT mesh_v_size   = lidar_ang_diameter / v_radius_max * 0.01f;
		FLOAT mesh_v_incr   = mesh_v_size / (FLOAT)lidar_cels;


	// ---- diffuse ----
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} white;
		white.d = VS_D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();
		FLOAT  v = 0;


	// ----- mesh buffer
		struct point
		{
			float x, y, z;
			float u, v;
		};
		point mesh[5][5];


		auto randd = []()
		{
			return (double)rand() / (RAND_MAX + 1.0);
		};


	// ================ FOR -- each column of lidar ================
		for (UINT lidar_column = 0; lidar_column<lidar_cols; lidar_column++)
		{

			// ================ FOR -- each beam of lidar ================
				for (UINT lidar_beam = 0; lidar_beam < lidar_beams; lidar_beam++)
				{

					// ================ get x & y ================
						FLOAT lidar_x = 0, lidar_y = 0;
						if ( random_cloud )
						{
							lidar_x = (FLOAT)randd() * 8.0f - 4.0f;
							lidar_y = (FLOAT)randd() * 8.0f - 4.0f;
						}
						else
						{
							lidar_x = *(csv_fb + 2);
							lidar_y = *(csv_fb + 3);
						}
						lidar_y *= -1;
						lidar_x *= -1;

					// ---- column angle
						FLOAT lidar_column_ang = (FLOAT)atan2(lidar_y, lidar_x) *  (FLOAT)VS_RADIAN;
						if ( lidar_y < 0 )
						{
							lidar_column_ang = 180.0f + lidar_column_ang;
							lidar_column_ang += 180.0f;
						}

					// ---- column rmu
						FLOAT lidar_column_rmu = lidar_column_ang / 360;

					// ---- column radius
						FLOAT xy = lidar_x*lidar_x + lidar_y*lidar_y;
						FLOAT lidar_r = sqrt(xy);

					// ---- random column rmu and angle
						// FLOAT lidar_column_rmu = (FLOAT)randd();
						// FLOAT lidar_column_ang = lidar_column_rmu * 360.0f;



					// ================ get z ================
						FLOAT lidar_z = 0;
						if ( random_cloud )
						{
							lidar_z = (FLOAT)randd() * 0.9f - 0.75f;
						}
						else
						{
							lidar_z  = *(csv_fb + 4);
							csv_fb  += csv_pitch;
						}
						FLOAT lidar_row_ang = (FLOAT)atan2(lidar_z, lidar_r) *  (FLOAT)VS_RADIAN;

					// --- lidar row rmu
						FLOAT lidar_row_rmu_ang = lidar_row_ang;
						if ( lidar_row_rmu_ang < 0 )
						{
							lidar_row_rmu_ang = 90.0f + lidar_row_rmu_ang;
						}
						else
						{
							lidar_row_rmu_ang += 90.0f;
						}
						FLOAT lidar_row_rmu = (lidar_row_rmu_ang + lidar_latitude_min) / lidar_latitude_tot;


					// ---- random row rmu & angle
						// FLOAT lidar_row_rmu = (FLOAT)randd();
						// FLOAT lidar_row_ang = lidar_row_rmu * lidar_latitude_tot + lidar_latitude_min;


					// ---- radius scalar
						FLOAT scalar_r = sqrt(xy + lidar_z*lidar_z);

						lidar_row_ang -= 7.5;


					// ================ FOR -- each row of the mesh ================
						for (UINT mesh_row = 0; mesh_row<lidar_mesh_vertices; mesh_row++)
						{

							// ---- X axis scalar
							FLOAT x_axis_scalar = (FLOAT)mesh_row * (FLOAT)lidar_mesh_incr;
	

							// ---- angle ----
								FLOAT x_rot_ang = x_axis_scalar * lidar_ang_diameter - lidar_ang_radius;
								FLOAT x_rotation_radians = ( x_rot_ang + lidar_row_ang ) * (FLOAT)VS_RADIANS_PER_DEGREE;


							// ---- 3D Rotate 0,0,1 to RY,0,RZ_1
								FLOAT y = scalar_r*sphere_radius * sin(x_rotation_radians);
								FLOAT z = scalar_r*sphere_radius * cos(x_rotation_radians);


							// ---- 2D unwrapping
								FLOAT r = lidar_row_rmu;
								v = (r * v_radius_total + v_radius_min) * 0.5f;


							// ================ FOR -- each column of the mesh ================
								for (UINT mesh_col = 0; mesh_col<lidar_mesh_vertices; mesh_col++)
								{

									point *p = &mesh[mesh_row][mesh_col];

									// ---- Y axis rotation scalar
										FLOAT y_axis_scalar = (FLOAT)mesh_col * lidar_mesh_incr;


									// ---- 3D Rotate RY,0,RZ_1 to RY,RX,RZ_2
										FLOAT y_rot_angle        = y_axis_scalar * lidar_ang_diameter - lidar_ang_radius;
										FLOAT y_rotation_radians = (y_rot_angle + lidar_column_ang) * (FLOAT)VS_RADIANS_PER_DEGREE;
										p->x =  z * sinf(y_rotation_radians);
										p->y =  y;
										p->z = -z * cosf(y_rotation_radians);


									// ---- 2D Unwrapping
										if ( vertex_format & VS_TEX1 )
										{
											FLOAT r = lidar_column_rmu;
											FLOAT a = (1 - r) * 360.0f * (FLOAT)VS_RADIANS_PER_DEGREE;
											p->u = -u_flip * v * sinf(a) * aspect_ratio + u_centre;
											p->v =  v_flip * v * cosf(a) + v_centre;
										}

								}
						}

					// ================ FOR -- each row of the mesh ================
						for (UINT mesh_row = 0; mesh_row < lidar_cels; mesh_row++)
						{

							// ================ FOR -- each column of the mesh ================
								for (UINT mesh_col = 0; mesh_col<lidar_cels; mesh_col++)
								{

									point *p00 = &mesh[mesh_row  ][mesh_col  ];
									point *p01 = &mesh[mesh_row  ][mesh_col+1];
									point *p10 = &mesh[mesh_row+1][mesh_col  ];
									point *p11 = &mesh[mesh_row+1][mesh_col+1];

									point *p[2][3] =
									{
										{ p00, p01, p10 },
										{ p10, p01, p11 }
									};

									for (UINT triangle = 0; triangle < 2; triangle++)
									{
										for (UINT vertex = 0; vertex < 3; vertex++)
										{
											*(f + 0) = p[triangle][vertex]->x;
											*(f + 1) = p[triangle][vertex]->y - lidar_y_offset;
											*(f + 2) = p[triangle][vertex]->z;
											f += 3;
											if ( vertex_format & VS_TEX1 )
											{
												*(f + 0) = p[triangle][vertex]->u;
												*(f + 1) = p[triangle][vertex]->v;
												f += 2;
											}
										}
									}

								}

						}

				}
		}

	return VS_OK;
}

