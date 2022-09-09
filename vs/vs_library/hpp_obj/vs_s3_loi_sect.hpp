////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_loi_sect.hpp ----------
/*!
\file vs_s3_loi_sect.hpp
\brief
\author Gareth Edwards
\note

*/


#pragma once


// ---------- Callback_S3_Loi_Section_Configure ----------
INT ObjConfig::Callback_S3_Loi_Section_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();
		LONG  *ld = context->GetLongParameter(); 

	// ---- get supplied and parameter data
		UINT mode      = (UINT)pd[BOX_FACE_MODE];
		UINT type      = (UINT)pd[BOX_FACE_TYPE];
		UINT ap_ref    = (UINT)pd[BOX_FACE_AP_REF];

	// ---- section - comprised of front & back lines!
		FLOAT *mesh_section_y_samples = (FLOAT*)(ld + 0);
		UINT   mesh_section_num_samples = (INT)*(ld + 1);


	// ---- # of lines
		//
		// warning: potential memory violation
		//
		//  -- max ruler width of 100mm
		//     max diagonal ruler length is 142mm
		//
		//  -- top and bottom lines:
		//     one line per unit plus extra end line is 143 lines
		//     round 143 to 150
		//     rule is double sides     
		//     so allow for 150 x 2
		//
		// --- blips max is one line per unit
		//     so allow for 150
		//
		UINT num_lines = 150 * 2 + 150;


	// ---- # of sample lines
		//
		// -- max number of samples is mesh_section_num_samples
		//    max sample lines is mesh_section_num_samples
		//    sample lines are top and bottom ( + # samples * 2 )
		//    sample lines are double sided ( + # samples * 2 )
		//
		num_lines += mesh_section_num_samples * 4;


	// ---- atlas page
		context->SetAtlasPage(0, 0);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(TRUE);
			context->SetMultiState(FALSE);

			switch ( type )
			{
				default:
				case 0:
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
			}
			context->SetVisible(TRUE);
		}

	// ---- index buffer
		context->SetIndexCount(num_lines * 2);
		context->SetIndexBufferCallback(Callback_S3_Loi_Section_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_S3_Loi_Section_GetVertexBuffer);

	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(num_lines);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_S3_Loi_Section_GetVertexBuffer ----------
INT ObjConfig::Callback_S3_Loi_Section_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();
		LONG  *ld = context->GetLongParameter();

	// ---- get supplied and parameter data
		UINT  mode    = (UINT)pd[BOX_FACE_MODE];
		UINT  type    = (UINT)pd[BOX_FACE_TYPE];
		UINT  ap_ref  = (UINT)pd[BOX_FACE_AP_REF];
		UINT  ap_sect = (UINT)pd[BOX_FACE_AP_SECT];
		FLOAT red     = pd[BOX_FACE_RED];
		FLOAT green   = pd[BOX_FACE_GREEN];
		FLOAT blue    = pd[BOX_FACE_BLUE];

	// ---- uncalculated
		FLOAT width = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT depth = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

	// ---- location
		FLOAT x_off = pd[vs_system::ObjConfig::BOX_FACE_X];
		FLOAT z_off = pd[vs_system::ObjConfig::BOX_FACE_Z];

	// ---- other parameter data
		FLOAT angle = pd[vs_system::ObjConfig::BOX_FLAG_1];
		FLOAT y_top = pd[vs_system::ObjConfig::BOX_FLAG_2];
		FLOAT y_bot = pd[vs_system::ObjConfig::BOX_FLAG_3];

		FLOAT x3 = pd[vs_system::ObjConfig::BOX_FLAG_4];
		FLOAT y3 = pd[vs_system::ObjConfig::BOX_FLAG_5];
		FLOAT x4 = pd[vs_system::ObjConfig::BOX_FLAG_6];
		FLOAT y4 = pd[vs_system::ObjConfig::BOX_FLAG_7];

		FLOAT marks_per_unit = pd[vs_system::ObjConfig::BOX_FLAG_8];

	// ---- mesh section samples
		FLOAT *mesh_section_y_samples = (FLOAT*)*(ld + 0);
		UINT   mesh_section_num_samples = (INT)*(ld + 1);


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- num lines
		UINT num_lines = context->GetPrimitiveCount();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} grey, endl;
		grey.d  = VS_D3DCOLOR_COLORVALUE( red, green, blue, 1.0f );
		endl.d = VS_D3DCOLOR_COLORVALUE(0, 1, 0, 1.0f);


	// ---- vertices
		struct vtx
		{
			float x, y, z, d;
		};


	// ---- proportional to depth
		FLOAT aw = depth * 0.025f;


	// ---- rule part flags
		BOOL rule_marks  = mode ? FALSE : TRUE;
		BOOL rule_top    = mode ? FALSE : TRUE;
		BOOL rule_bottom = TRUE;


	// ---- height of rule
		FLOAT rule_hgt = y_top - y_bot;


	// ---- offset: front (fn) & back (bn) surface normals
		FLOAT vl = ap_sect == 1 ? 0.5f : 0;
		FLOAT vh = ap_sect == 1 ? 1 : 0.5f;
		VsVector3 fn = { 0, 0, 0 };
		VsVector3 bn = { 0, 0, 0 };
		{
			VsVector3 a = { 0, rule_hgt, 0 };
			VsVector3 b = { x4 - x3, 0, y4 - y3 };
			a.Normalise();
			b.Normalise();
			fn.CrossProduct(&a, &b);
			bn.x = -fn.x; bn.y = fn.y; bn.z = -fn.z;
			FLOAT scalar = aw * 0.015f;
			fn.x *= scalar;
			fn.z *= scalar;
			bn.x *= scalar;
			bn.z *= scalar;
		}


	// ---- y sample offset to "see" surface intersect, proportional to surface size
		FLOAT y_sample_offset = 0.01f * aw;


	// ---- counter used to "clean" at end 
		UINT used_vertex_count = 0;


	// ---- marks
		if ( rule_marks)
		{

			// ---- height
				FLOAT rule_hgts[3] =
					{
						rule_hgt,
						rule_hgt * 0.4f,
						rule_hgt * 0.1f
					};


			// ---- var
				FLOAT rmu_off;
				FLOAT dd, dist;
				UINT  idist, ih, is;
				FLOAT xd, yd, zd;
				FLOAT xe, ze, re;
				FLOAT x, y, z, r;

			// ---- proportional dist of off from sect start - backslash \/
				xd = x4 - x3;
				yd = y4 - y3;
				dd = sqrt(xd*xd + yd*yd);
				xd = x_off-x3;
				yd = z_off-y3;
				rmu_off = sqrt(xd*xd + yd*yd) / dd;

			// ---- draw rule in each "direction"
	
				for (UINT d=0; d<2; d++)
				{

					// ---- end
						xe = d == 0 ? x3 : x4;
						ze = d == 0 ? y3 : y4;
						re = d == 0 ? 0.0f : 1.0f;

					// ---- # of mm width
						xd = xe - x_off;
						zd = ze - z_off;
						dist = (FLOAT)sqrt(xd*xd + zd*zd) * marks_per_unit;
						idist = (UINT)dist;

					// ---- mm
						for (UINT i = 0; i<= idist; i++)
						{

							// ---- avoid overflow
								if ( used_vertex_count+4 < vertex_count )
								{

									// ---- interpolate to calculate xz "here"
										FLOAT rmu = (FLOAT)i / idist;
										x = (1 - rmu)*x_off + rmu * xe;
										z = (1 - rmu)*z_off + rmu * ze;

									// ---- interpolate to calculate y sample "here" 
										r = (1 - rmu)*rmu_off + rmu * re;
										r = r < 0 ? 0 : (r > 1 ? 1 : r);
										is = (INT)(r * (FLOAT)mesh_section_num_samples);
										y = *(mesh_section_y_samples + is) + y_bot;

									// ---- mark size
										ih = i % 10 == 0 ? 0 : (i % 5 == 0 ? 1 : 2);

									// ---- front mark
										*(f  ) = x + fn.x;
										*(f+1) = y;
										*(f+2) = z + fn.z;
										*(f+3) = endl.f;
										f += 4;
										*(f  ) = x + fn.x;
										*(f+1) = y - rule_hgts[ih] + y_sample_offset;
										*(f+2) = z + fn.z;
										*(f+3) = endl.f;
										f += 4;

									// ---- back mark
										*(f  ) = x + bn.x;
										*(f+1) = y;
										*(f+2) = z + bn.z;
										*(f+3) = endl.f;
										f += 4;
										*(f  ) = x + bn.x;
										*(f+1) = y - rule_hgts[ih];
										*(f+2) = z + bn.z;
										*(f+3) = endl.f;
										f += 4;

									// ---- increment 
										used_vertex_count += 4;
								}

						}

				}
		}


	// ---- draw top & bottom section
		FLOAT msss = (FLOAT)(mesh_section_num_samples-1);
		FLOAT x_last = x3;
		FLOAT z_last = y3;
		colour_union_t colr = grey;
		for (UINT s=1; s<mesh_section_num_samples; s++)
		{

			// ---- interpolate
				FLOAT rmu = (FLOAT)s / msss;
				FLOAT x = (1 - rmu)*x3 + rmu * x4;
				FLOAT z = (1 - rmu)*y3 + rmu * y4;

			// ---- top
				if ( rule_top )
				{

					// ---- y
						FLOAT ys = *(mesh_section_y_samples + s - 1) + y_bot;
						FLOAT ye = *(mesh_section_y_samples + s) + y_bot;

					// ---- front
						*(f  ) = x_last + fn.x;
						*(f+1) = *(mesh_section_y_samples + s - 1) + y_bot;
						*(f+2) = z_last + fn.z;
						*(f+3) = endl.f;
						f += 4;
						*(f  ) = x + fn.x;
						*(f+1) = *(mesh_section_y_samples + s    ) + y_bot;
						*(f+2) = z + fn.z;
						*(f+3) = endl.f;
						f += 4;
				
					// ---- back
						*(f    ) = x_last + bn.x;
						*(f + 1) = *(mesh_section_y_samples + s - 1) + y_bot;
						*(f + 2) = z_last + bn.z;
						*(f + 3) = endl.f;
						f += 4;
						*(f    ) = x + bn.x;
						*(f + 1) = *(mesh_section_y_samples + s    ) + y_bot;
						*(f + 2) = z + bn.z;
						*(f + 3) = endl.f;
						f += 4;

						used_vertex_count += 4;
				}

			// ---- bottom
				if ( rule_bottom )
				{

					// ---- y
						FLOAT ys = *(mesh_section_y_samples + s - 1) + y_bot - y_sample_offset;
						FLOAT ye = *(mesh_section_y_samples + s) + y_bot - y_sample_offset;

					// ---- bottom front
						*(f  ) = x_last + fn.x;
						*(f+1) = *(mesh_section_y_samples + s - 1) + y_sample_offset;;
						*(f+2) = z_last + fn.z;
						*(f+3) = endl.f;
						f += 4;
						*(f  ) = x + fn.x;
						*(f+1) = *(mesh_section_y_samples + s    ) + y_sample_offset;;
						*(f+2) = z + fn.z;
						*(f+3) = endl.f;
						f += 4;

					// ---- bottom back
						*(f  ) = x_last + bn.x;
						*(f+1) = *(mesh_section_y_samples + s - 1) + y_sample_offset;;
						*(f+2) = z_last + bn.z;
						*(f+3) = endl.f;
						f += 4;
						*(f  ) = x + bn.x;
						*(f+1) = *(mesh_section_y_samples + s    ) + y_sample_offset;;
						*(f+2) = z + bn.z;
						*(f+3) = endl.f;
						f += 4;

						used_vertex_count += 4;
				}

			// ---- join the dots
				x_last = x;
				z_last = z;

		}


	// ---- zero the unused vertices
		for (UINT i = used_vertex_count; i < vertex_count; i++)
		{
			*(f) = 0;
			*(f + 1) = 0;
			*(f + 2) = 0;
			*(f + 3) = 0;
			f += 4;
		}

	return VS_OK;
}


// ---------- Callback_S3_Loi_Section_GetIndexBuffer ----------
INT ObjConfig::Callback_S3_Loi_Section_GetIndexBuffer(
		vs_system::ElmContext *context
	)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- num lines
		UINT num_lines = context->GetPrimitiveCount();

	// ---- buffer
		DWORD *ib = context->GetIndexBufferPtr();
		DWORD  ic = 0;

	// ----
		for (UINT line = 0; line < num_lines; line++)
		{
			*(ib+0) = ic++;
			*(ib+1) = ic++;
			ib += 2;
		}

	return VS_OK;
}
