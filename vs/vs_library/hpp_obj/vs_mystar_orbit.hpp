////////////////////////////////////////////////////////////////////////////////

// ---------- vs_mystar_orbit.hpp ----------
/*!
\file vs_mystar_orbit.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_MyStar_Orbit_Configure ----------
INT ObjConfig::Callback_MyStar_Orbit_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get parameter data
		UINT orbit_lines = (UINT)pd[ORBIT_LINES];


	// ---- overide ?
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(true);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(FALSE);
			context->SetTextured(FALSE);
			context->SetVisible(true);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- primitive type
		context->SetPrimitiveType(VS_LINESTRIP);


	// ---- vertex buffer
		context->SetVertexCount(orbit_lines + 1);
		context->SetVertexFormat((UINT)(VS_XYZ | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_MyStar_Orbit_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(orbit_lines);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_MyStar_Orbit_GetVertexBuffer ----------
INT ObjConfig::Callback_MyStar_Orbit_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  orbit_lines   = (UINT)pd[ORBIT_LINES];
		FLOAT orbit_radius  = pd[ORBIT_RADIUS];
		FLOAT orbit_red     = pd[ORBIT_RED];
		FLOAT orbit_green   = pd[ORBIT_GREEN];
		FLOAT orbit_blue    = pd[ORBIT_BLUE];
		FLOAT orbit_alpha   = 1;
		FLOAT px            = pd[ORBIT_X];
		FLOAT py            = pd[ORBIT_Y];
		FLOAT pz            = pd[ORBIT_Z];
		FLOAT orbit_degrees = pd[ORBIT_DEGREES];


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		FLOAT *f = context->GetVertexBufferPtr();
		double ang_inc = orbit_degrees / (FLOAT)orbit_lines;


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} c;


	// ---- orbit
		for (DWORD i = 0; i <= orbit_lines; i++)
		{
 
			// ---- circle
				double ang = ang_inc * (double)i;
				float u =  (float)cos(ang*VS_RADIANS_PER_DEGREE);
				float v = -(float)sin(ang*VS_RADIANS_PER_DEGREE);
				float x = u * orbit_radius;
				float z = v * orbit_radius;

				*(f + 0) = x;
				*(f + 1) = 0;
				*(f + 2) = z;
				f += 3;

			// ---- colour interpolation
				float rmu = (float)i / (float)orbit_lines;
				float or = (1 - rmu)*orbit_red   + rmu*0.2f;
				float og = (1 - rmu)*orbit_green + rmu*0.2f;
				float ob = (1 - rmu)*orbit_blue  + rmu*0.2f;
				float oa = (1 - rmu)*orbit_alpha + rmu*0.0f;
				c.d = VS_D3DCOLOR_COLORVALUE(or, og, ob, oa);

				*(f) = c.f;
				f++;
		}

	return VS_OK;
}

