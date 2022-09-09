////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_annular.hpp ----------
/*!
\file vs_pano_annular.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanoAnnular_Configure ----------
INT ObjConfig::Callback_PanoAnnular_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
		context->BeginConfig();


	// ---- local
		FLOAT *p = context->GetFloatParameter();


	// ---- atlas page
		context->SetAtlasPage(0, pi_obj_pano->pano_texture_lid);


	// ---- overide -- ?
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(false);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(TRUE);
			context->SetVisible(true);
		}


	// ---- index buffer ----
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer
		context->SetVertexCount( 6 );
		context->SetVertexFormat((UINT)( VS_XYZRHW | VS_TEX1 ));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_PanoAnnular_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
			context->GetVertexSize() *
			context->GetVertexCount()  *
			sizeof(float)
			);
		context->SetPrimitiveCount( 2 );
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_PanoAnnular_GetVertexBuffer ----------
INT ObjConfig::Callback_PanoAnnular_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- vertex
		FLOAT *sp = context->GetGfxFloatParameter();
		FLOAT left   = sp[VS_PANEL_LEFT];
		FLOAT top    = sp[VS_PANEL_TOP];
		FLOAT right  = sp[VS_PANEL_RIGHT];
		FLOAT bottom = sp[VS_PANEL_BOTTOM];
		FLOAT width  = right - left - 1;
		FLOAT height = bottom - top - 1;


	// ---- pano parameters
		FLOAT aspect_ratio = pi_obj_pano->aspect_ratio;
		FLOAT u_flip       = U_FLIP_LOGIC;
		FLOAT v_flip       = V_FLIP_LOGIC;
		FLOAT v_radius_max = pi_obj_pano->max_radius;


	// ---- geometry scalar to "frame" full annular ROI
		FLOAT scalar = 1 / ( v_radius_max  > 1.0f ? v_radius_max : 1.0f );


	// ---- U
		FLOAT u_centre = pi_obj_pano->u_offset;
		FLOAT u_left  = 0;
		FLOAT u_right = 1;
		if ( u_flip )
		{
			u_left  = 1;
			u_right = 0;
		}


	// ---- V
		FLOAT v_centre = pi_obj_pano->v_offset + 0.5f;


	// ---- "Pan" source vertices
		FLOAT prop_width_by2 = (bottom - top) * 1 / aspect_ratio / 2;
		FLOAT x_centre = (right + left) / 2;
		struct source { FLOAT x, y; };
		source s[4] =
		{
			{ x_centre - prop_width_by2, top },
			{ x_centre + prop_width_by2, top },
			{ x_centre - prop_width_by2, bottom },
			{ x_centre + prop_width_by2, bottom }
		};
		FLOAT ox = width/2 + left;
		FLOAT oy = height/2 + top;


	// ---- pan 2d vertex buffers ? --  use pan value ELSE zero
		FLOAT theta =
			pi_obj_pano->pan_vertex_buffer ?
				pi_obj_pano->pan * (FLOAT)VS_RADIANS_PER_DEGREE :
				-(0.25f + pi_obj_pano->pan_offset) * (FLOAT)VS_RADIANS_PER_DEGREE;


	// ---- rotate source xy struct array
		for ( UINT i=0; i<4; i++ )
		{
			FLOAT px = s[i].x-ox;
			FLOAT py = s[i].y-oy;
			s[i].x = ( cos(theta) * px - sin(theta) * py ) + ox;
			s[i].y = sin(theta) * px + cos(theta) * py + oy;
		}


	// ----- XY centre
		FLOAT xcen = 0;
		FLOAT ycen = 0;
		for (UINT i = 0; i<4; i++)
		{
			xcen += s[i].x;
			ycen += s[i].y;
		}
		xcen /= 4;
		ycen /= 4;
		for (UINT i = 0; i<4; i++)
		{
			s[i].x = (s[i].x-xcen)*scalar + xcen;
			s[i].y = (s[i].y-ycen)*scalar + ycen;
		}


	//  ---- vertices
		struct vertex { FLOAT x, y, u, v; };
		vertex v[6] = 
		{
			{ s[0].x, s[0].y, u_centre + u_right, v_centre - v_flip * 0.5f }, // left,  top
			{ s[1].x, s[1].y, u_centre + u_left,  v_centre - v_flip * 0.5f }, // right, top
			{ s[2].x, s[2].y, u_centre + u_right, v_centre + v_flip * 0.5f }, // left,  bottom
			{ s[2].x, s[2].y, u_centre + u_right, v_centre + v_flip * 0.5f }, // left,  bottom
			{ s[1].x, s[1].y, u_centre + u_left,  v_centre - v_flip * 0.5f }, // right, top
			{ s[3].x, s[3].y, u_centre + u_left,  v_centre + v_flip * 0.5f }  // right, bottom
		};

	// ---- buffer pointer
		FLOAT *f = context->GetVertexBufferPtr();

    // ---- each vertex...
		FLOAT zed = 0.99f;
		FLOAT rhw = 1;
		UINT  vertex_size = context->GetVertexSize();
		UINT  vertex_count = context->GetVertexCount();
		for (UINT index=0; index<vertex_count; index++)
		{
			*(f)   = v[index].x;
			*(f+1) = v[index].y;
			*(f+2) = zed;
			*(f+3) = rhw;
			*(f+4) = v[index].u;
			*(f+5) = v[index].v;
			f += vertex_size;
		}

	return VS_OK;
}
