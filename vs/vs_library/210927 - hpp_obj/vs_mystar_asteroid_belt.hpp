////////////////////////////////////////////////////////////////////////////////

// ---------- vs_mystar_asteroid_belt.hpp ----------
/*!
\file vs_mystar_asteroid_belt.hpp
\brief
\author Gareth Edwards


// ---- init sphere asteroid belt parameters

if ( type == SystemObject::Type::AstRing )
{
param[vs_system::ObjConfig::ASTEROID_BELT_DIVISION] = 2;
param[vs_system::ObjConfig::ASTEROID_BELT_TYPE]     = 2;
param[vs_system::ObjConfig::ASTEROID_BELT_AP_REF] = 0;//sys_obj->Get("GfxPag") + 100;
param[vs_system::ObjConfig::ASTEROID_BELT_AP_SECT]  = sys_obj->Get("GfxSec");
param[vs_system::ObjConfig::ASTEROID_BELT_RADIUS]   = object_diameter / 20;
param[vs_system::ObjConfig::ASTEROID_BELT_RED]      = sys_obj->Get("GfxRed");
param[vs_system::ObjConfig::ASTEROID_BELT_GREEN]    = sys_obj->Get("GfxGre");
param[vs_system::ObjConfig::ASTEROID_BELT_BLUE]     = sys_obj->Get("GfxBlu");
param[vs_system::ObjConfig::ASTEROID_BELT_RAD_MIN]  =
illustration_mode ?
sys_obj->Get("GfxRadMin") :
sys_obj->Get("RadMin");
param[vs_system::ObjConfig::ASTEROID_BELT_RAD_MAX]  =
illustration_mode ?
sys_obj->Get("GfxRadMax") :
sys_obj->Get("RadMax");
param[vs_system::ObjConfig::ASTEROID_BELT_NUMBER]   =  10;
}



*/


#pragma once


// ---------- Tetrahedron ----------
/*!

\brief testing tetrahedron recursion

\note 4 to the power of max_rec + 1, e.g. num_tri = pow(4, max_rec + 1)

*/
INT ObjConfig::GetTetrahedronTriangles(INT max_rec, INT *cur_rec, INT *num_tri)
{

	if ( *cur_rec < max_rec )
	{
		*cur_rec += 1;
		for (UINT i = 0; i < 4; i++)
		{
			GetTetrahedronTriangles(max_rec, cur_rec, num_tri);
		}
		*cur_rec -= 1;
	}
	else
	{
		*num_tri += 4;
	}

	return VS_OK;
}


INT ObjConfig::GetTetrahedronFaceVertices(INT index, FLOAT s, FLOAT d, FLOAT *face_pnt)
{

	// ---- test index 
		if ( index < 0 || index > 3 ) return VS_ERROR;


	// ---- included angle
		FLOAT rad_120 = 120.0f * (FLOAT)VS_RADIANS_PER_DEGREE;


	// ---- 1A : top
		FLOAT x0 = 0, y0 = 1, z0 = 0;
	
	// ---- 2B : 0, -y, +z
		FLOAT x1 =   0;
		FLOAT y1 =  y0 * cosf(rad_120);
		FLOAT z1 =  y0 * sinf(rad_120);
	
	// ---- 3C : +x, -y, -z
		FLOAT x2 =  z1 * sinf(rad_120);
		FLOAT y2 =  y1;
		FLOAT z2 =  z1 * cosf(rad_120);
	
	// ---- 4D : -x, -y, -z
		FLOAT x3 = -x2;
		FLOAT y3 =  y2;
		FLOAT z3 =  z2;


	// ---- VS_XYZ | VS_DIFFUSE| VS_NORMAL
		FLOAT tet_pnt[4][7] =
		{
			{ x0*s, y0*s, z0*s, d, x0, y0, z0 },
			{ x1*s, y1*s, z1*s, d, x1, y1, z1 },
			{ x2*s, y2*s, z2*s, d, x2, y2, z2 },
			{ x3*s, y3*s, z3*s, d, x3, y3, z3 },
		};


	// ---- face indices
		UINT tet_ind[4][3] =
		{
			{  0,  1,  3 },
			{  1,  2,  3 },
			{  0,  3,  2 },
			{  1,  0,  2 }
		};


	// ---- get
		FLOAT *pf = face_pnt;
		for (INT i = 0; i < 3; i++)
		{
			for (UINT j = 0; j < 7; j++)
			{
				INT k = tet_ind[index][i];
				*pf = tet_pnt[k][j];
				pf++;
			}
		}

	return VS_OK;
}


// ---------- Callback_MyStar_Asteroid_Belt_Configure ----------

INT ObjConfig::Callback_MyStar_Asteroid_Belt_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT asteroid_division = (UINT)( pd[ASTEROID_BELT_DIVISION] < 0 ? 0 : pd[ASTEROID_BELT_DIVISION] );
		UINT asteroid_number   = (UINT)( pd[ASTEROID_BELT_NUMBER]   < 1 ? 1 : pd[ASTEROID_BELT_NUMBER]);


	// ---- number of triangles
		INT  num_tri = (INT)pow(4, asteroid_division + 1);


	// ---- NO atlas page
		context->SetAtlasPage(0, 0);


	// IF -- not overide -- THEN
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE | VS_NORMAL);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(TRUE);
			context->SetMultiState(FALSE);
			context->SetSolid(TRUE);
			context->SetTextured(FALSE);
			context->SetVisible(TRUE);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer
		context->SetVertexCount(num_tri * 3 * asteroid_number);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_MyStar_Asteroid_Belt_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(FLOAT)
			);
		context->SetPrimitiveCount(num_tri * asteroid_number);
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ----------  Callback_MyStar_Asteroid_Belt_GetVertexBuffer ----------
/*!

*/
INT ObjConfig::Callback_MyStar_Asteroid_Belt_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT  asteroid_division = (UINT)pd[ASTEROID_BELT_DIVISION];
		FLOAT asteroid_red      = pd[SPHERE_RED];
		FLOAT asteroid_green    = pd[SPHERE_GREEN];
		FLOAT asteroid_blue     = pd[SPHERE_BLUE];
		FLOAT asteroid_radius   = pd[SPHERE_RADIUS];
		FLOAT asteroid_rad_min  = pd[SPHERE_RAD_MIN];
		FLOAT asteroid_rad_max  = pd[SPHERE_RAD_MAX];
		UINT  asteroid_number   = (UINT)pd[SPHERE_NUMBER];


	// ---- vertex
		UINT  vertex_size   = context->GetVertexSize();
		UINT  vertex_format = context->GetVertexFormat();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} dif;
		dif.d = VS_D3DCOLOR_COLORVALUE(asteroid_red, asteroid_green, asteroid_blue, 1.0f);
		FLOAT d = (FLOAT)dif.d;

		  
	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- store
		FLOAT face_vertices[3][7]; 
		for (INT face = 0; face < 4; face++)
		{
			// --- get face
				INT result = GetTetrahedronFaceVertices(face, asteroid_radius, d, &face_vertices[0][0]);
				memcpy((VOID *)f, (VOID *)face_vertices, 21 * sizeof(FLOAT));
				f += 21;
		}

	return VS_OK;
}

