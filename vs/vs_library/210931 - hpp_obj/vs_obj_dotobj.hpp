////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_dotobj.hpp ----------
/*!
\file vs_obj_dotobj.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_DotObj_Configure ----------

INT ObjConfig::Callback_DotObj_Configure(vs_system::ElmContext *context)
{

	// ---- local
		INT result = VS_OK;


	// ---- retrieve a dotobj object pointer
		LONG *l = context->GetLongParameter();
		DotObjElement *object = (DotObjElement *)(l[0]);
		vs_system::DotObjUtilities dot_obj_utilities;


	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- parameter data
		FLOAT *pd = context->GetFloatParameter();
		UINT   mode   = (UINT)pd[DOTOBJ_MODE];
		UINT   type   = (UINT)pd[DOTOBJ_TYPE];
		UINT   ap_ref = (UINT)pd[DOTOBJ_AP_REF];


	// ---- atlas page
		context->SetAtlasPage(0, ap_ref);


	// ---- not overide ?
		UINT format = (UINT)(VS_XYZ | VS_NORMAL);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			switch ( type )
			{
				default:
				case 0: // wireframe
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
				case 1: // solid
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(TRUE);
					context->SetTextured(FALSE);
					break;
				case 2: // solid + texture
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(TRUE);
					context->SetTextured(ap_ref == 0 ? FALSE : TRUE);
					break;
			}

			context->SetVisible(TRUE);
		}


	// ---- so processed or raw
		if ( mode == DotObjUtilities::GfxRender::PROCESSED )
		{

			// ---- index buffer
				UINT number_of_triangles = dot_obj_utilities.GetNumberOfTriangles(object);
				context->SetIndexCount(number_of_triangles * 3);
				context->SetIndexBufferCallback(&Callback_DotObj_GetIndexBuffer);


			// ---- vertex buffer (yes! ... same as # of indices )
				context->SetVertexCount(
								dot_obj_utilities.GetSizeOfIndexList(
									object, DotObjElement::TagId::ELEMENT_FACE)
							);
				context->SetVertexFormat(format);
				context->SetVertexSize(context->GetVertexFormat());
				context->SetVertexBufferCallback(&Callback_DotObj_GetVertexBuffer);


			// ---- primitive parameters
				context->SetPrimitiveBytes(
						context->GetVertexSize() *
						context->GetVertexCount()  *
						sizeof(FLOAT)
					);
				context->SetPrimitiveCount(number_of_triangles);
				context->SetPrimitiveType(VS_TRIANGLELIST);

		}
		else // DotObjUtilities::GfxRender::RAW
		{

			// ---- index buffer
				UINT number_of_triangles = dot_obj_utilities.GetNumberOfTriangles(object);
				context->SetIndexCount(number_of_triangles * 3);
				context->SetIndexBufferCallback(&Callback_DotObj_GetIndexBuffer_Teapot);
			

			// ---- # of vertices in vertex list
				UINT number_of_vertices = dot_obj_utilities.GetSizeOfVertexList(object, DotObjElement::TagId::VERTEX_GEOMETRY);


			// ---- vertex buffer
				context->SetVertexCount(number_of_vertices);
				context->SetVertexFormat(format);
				context->SetVertexSize(context->GetVertexFormat());
				context->SetVertexBufferCallback(&Callback_DotObj_GetVertexBuffer_Teapot);


			// ---- primitive parameters
				context->SetPrimitiveBytes(context->GetVertexSize() *context->GetVertexCount()  *sizeof(FLOAT));
				context->SetPrimitiveCount(number_of_triangles);
				context->SetPrimitiveType(VS_TRIANGLELIST);

		}


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ----------  Callback_DotObj_GetVertexBuffer ----------
/*!

*/
INT ObjConfig::Callback_DotObj_GetVertexBuffer(vs_system::ElmContext *context)
{
	
	// ---- parameter data
		FLOAT *pd     = context->GetFloatParameter();
		UINT   mode    = (UINT)pd[DOTOBJ_MODE];
		UINT   type    = (UINT)pd[DOTOBJ_TYPE];

		UINT   ap_ref  = (UINT)pd[DOTOBJ_AP_REF];
		UINT   ap_sect = (UINT)pd[DOTOBJ_AP_SECT];

		FLOAT  red     = pd[DOTOBJ_RED];
		FLOAT  green   = pd[DOTOBJ_GREEN];
		FLOAT  blue    = pd[DOTOBJ_BLUE];

		FLOAT  scale   = pd[DOTOBJ_SCALE];
		FLOAT  width   = pd[DOTOBJ_WIDTH]  * scale;
		FLOAT  height  = pd[DOTOBJ_HEIGHT] * scale;
		FLOAT  depth   = pd[DOTOBJ_DEPTH]  * scale;
		FLOAT  x_off   = pd[DOTOBJ_XOFF];
		FLOAT  y_off   = pd[DOTOBJ_YOFF];
		FLOAT  z_off   = pd[DOTOBJ_ZOFF];


	// ---- vertex
		INT  vertex_count  = context->GetVertexCount();
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_format = context->GetVertexFormat();
		UINT indices_count = context->GetPrimitiveCount() * 3;

	// ---- get vertex buffer
		FLOAT *vb = context->GetVertexBufferPtr();


	// ---- retrieve a dotobj object pointer
		LONG *l = context->GetLongParameter();
		DotObjElement *object = (DotObjElement *)(l[0]);
		vs_system::DotObjUtilities dot_obj_utilities;


	// --- fill vertex buffer
		//
		// size == one per index, as per indices_count
		//

		INT result = dot_obj_utilities.GetTriangleVertexBuffer(
				object,
				&pd[DOTOBJ_SCALE],
				vb,
				vertex_count,
				vertex_format
			);

	return VS_OK;
}


// ---------- Callback_DotObj_GetIndexBuffer ----------
INT ObjConfig::Callback_DotObj_GetIndexBuffer(vs_system::ElmContext *context)
{

	// ---- indices
		INT index_count  = context->GetIndexCount();


	// ---- retrieve a dotobj object pointer
		LONG *l = context->GetLongParameter();
		DotObjElement *object = (DotObjElement *)(l[0]);
		vs_system::DotObjUtilities dot_obj_utilities;


	// ---- fill index buffer
		DWORD *ib  = context->GetIndexBufferPtr();
		INT result = dot_obj_utilities.GetTriangleIndexBuffer(
				object,
				ib,
				index_count
			);

	return VS_OK;
}


// ----------  Callback_DotObj_GetVertexBuffer_Teapot ----------
/*!

*/
INT ObjConfig::Callback_DotObj_GetVertexBuffer_Teapot(vs_system::ElmContext *context)
{

	// ---- parameter data
		FLOAT *pd     = context->GetFloatParameter();
		UINT   mode    = (UINT)pd[DOTOBJ_MODE];
		UINT   type    = (UINT)pd[DOTOBJ_TYPE];
		UINT   ap_ref  = (UINT)pd[DOTOBJ_AP_REF];
		UINT   ap_sect = (UINT)pd[DOTOBJ_AP_SECT];
		FLOAT  red     = pd[DOTOBJ_RED];
		FLOAT  green   = pd[DOTOBJ_GREEN];
		FLOAT  blue    = pd[DOTOBJ_BLUE];
		FLOAT  scale   = pd[DOTOBJ_SCALE];
		FLOAT  width   = pd[DOTOBJ_WIDTH]  * scale;
		FLOAT  height  = pd[DOTOBJ_HEIGHT] * scale;
		FLOAT  depth   = pd[DOTOBJ_DEPTH]  * scale;
		FLOAT  x_off   = pd[DOTOBJ_XOFF];
		FLOAT  y_off   = pd[DOTOBJ_YOFF];
		FLOAT  z_off   = pd[DOTOBJ_ZOFF];


	// ---- vertex
		INT  vertex_count  = context->GetVertexCount();
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_format = context->GetVertexFormat();


	// ---- retrieve a dotobj object pointer
		LONG *l = context->GetLongParameter();
		DotObjElement *object = (DotObjElement *)(l[0]);
		vs_system::DotObjUtilities dot_obj_utilities;


	// ---- fill vertex buffer
		FLOAT *vb = context->GetVertexBufferPtr();


	// --- get
		INT result = dot_obj_utilities.GetVertexBuffer(
				object,
				DotObjElement::TagId::VERTEX_GEOMETRY,
				vb,
				vertex_count,
				vertex_format
			);


	// ---- diffuse colour
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} diffuse;
		diffuse.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, 1.0f);


	// ---- update
		FLOAT x = 1, y = 1, z = 1;
		FLOAT m[3] = { width, height, depth };
		FLOAT n[3] = { x_off, y_off, z_off };
		vb = context->GetVertexBufferPtr();
		for (INT v = 0; v < vertex_count; v++)
		{

			for (INT o = 0; o < 3; o++)                             // ---- VS_XYZ
			{
				*(vb + o) = *(vb + o) * m[o] + n[o];
			}
			FLOAT *g = vb;                                          // ---- set *g for VS_NORMAL
			vb += 3;                                                // ---- increment buffer pointer


			if ( vertex_format & VS_DIFFUSE )                       // ---- VS_DIFFUSE
			{
				*(vb++) = diffuse.f;
			}


			if ( vertex_format & VS_NORMAL )                        // ---- VS_NORMAL projected from origin...
			{
				x = *(g);                                           // ---- requires *g set in VS_XYZ
				y = *(g + 1);
				z = *(g + 2);
				FLOAT xd = x * x;
				FLOAT yd = y * y;
				FLOAT zd = z * z;
				FLOAT dd = (FLOAT)sqrt(xd*xd + yd * yd + zd + zd);  // ---- len
				FLOAT q = 1 / dd;                                   // ---- quotient
				*(vb    ) = x * q;                                  // ---- calc unit normal
				*(vb + 1) = y * q;
				*(vb + 2) = z * q;
				g = vb;                                             // ----  set *g for VS_TEX1
				vb += 3;                                            // ---- increment buffer pointer
			}


			if ( vertex_format & VS_TEX1 )                          // ---- VS_TEX1 - from cartesian normals to polar coordinates
			{
				FLOAT x = *(g);                                     // ---- requires *g set in VS_NORMAL
				FLOAT y = *(g + 1);
				FLOAT z = *(g + 2);

				FLOAT theta = (FLOAT)fabs(x) < 0.0001f ?            // ---- angle in YY
						0.0f : ( atan(y / x) * (FLOAT)VS_RADIAN );
				theta = 90.0f + theta;

				FLOAT zeta  = (FLOAT)fabs(z) < 0.0001f ?            // ---- angle in XZ
						0.0f : ( atan(x / z) * (FLOAT)VS_RADIAN );
				zeta = 90.0f + zeta;

				*(vb + 0) = theta / 180.0f * 0.48f + 0.01f;         // ---- U

				FLOAT vx  = ap_sect == 2 ? 0 : 0.5f;                // ---- V
				*(vb + 1) = 1 - (zeta  / 180.0f * 0.48f + 0.01f ) - vx;

				vb += 2;                                            // ---- increment buffer pointer
			}

		}

	return VS_OK;
}


// ---------- Callback_DotObj_GetIndexBuffer_Teapot ----------
INT ObjConfig::Callback_DotObj_GetIndexBuffer_Teapot(vs_system::ElmContext *context)
{

	// ---- indices
		INT index_count  = context->GetIndexCount();


	// ---- retrieve a dotobj object pointer
		LONG *l = context->GetLongParameter();
		DotObjElement *object = (DotObjElement *)(l[0]);
		vs_system::DotObjUtilities dot_obj_utilities;


	// ---- fill index buffer
		DWORD *ib  = context->GetIndexBufferPtr();
		INT result = dot_obj_utilities.GetIndexBuffer(
				object,
				DotObjElement::TagId::ELEMENT_FACE,
				ib,
				index_count
			);

	return VS_OK;
}

