////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_kandinsky.hpp ----------
/*!
\file vs_obj_kandinsky.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Kandinsky_Configure ----------
INT ObjConfig::Callback_Kandinsky_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- kandinsky
		LONG  *l = context->GetLongParameter();
		FLOAT *k = (FLOAT *)*(l + 0);

		UINT  dimension = KandinskyGetDimension(k);
		UINT  geotype   = KandinskyGetGeoType(k);
		UINT  num_element = KandinskyGetNumElements(k);

		UINT  num_vertices = 0;
		UINT  num_gfx_vertices = 0;
		UINT  num_gfx_elements = 0;

		UINT  kandinsky_index = KandinskyGetElementIndex(0,k);
		for (UINT element = 0; element < num_element; element++)
		{
			num_vertices       = KandinskyGetNumVertices(kandinsky_index, k);

			num_gfx_vertices  += KandinskyGetNumGfxVertices(geotype, num_vertices);
			num_gfx_elements  += KandinskyGetNumGfxElements(geotype, num_vertices);

			kandinsky_index    = KandinskyNextElementIndex(kandinsky_index, k);
		}


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT  type     = (UINT)pd[KANDINSKY_TYPE];
		UINT  ap_ref   = (UINT)pd[KANDINSKY_AP_REF];


	// ---- point or line ? - always wireframe 
		if ( geotype != KANDINSKY_GEOTYPE_FACE )
			type = 0;


	// ---- atlas page
		context->SetAtlasPage(0, ap_ref);


	// ---- not overide ?
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE);
		if ( !context->GetOveride() )
		{

			context->SetAlphaBlending(false);
			context->SetColourVertex(false);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(TRUE);

			switch ( type )
			{
				default:
				case 0: // wireframe
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetAlphaBlending(TRUE);
					context->SetColourVertex(TRUE);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
				case 1: // solid
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(TRUE);
					context->SetTextured(FALSE);
					break;
				case 2: // textured
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(TRUE);
					context->SetTextured(TRUE);
					break;
			}

			context->SetVisible(true);
		}


	// ---- no index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer
		context->SetVertexCount(num_gfx_vertices);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_Kandinsky_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(num_gfx_elements);
		context->SetPrimitiveType(VS_TRIANGLELIST);


		OutputDebugString("Callback_Kandinsky_Configure \n");


	// ---- end configuration profiling
		context->EndConfig();


	return VS_OK;
}


// ---------- Callback_Kandinsky_GetVertexBuffer ----------
INT ObjConfig::Callback_Kandinsky_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT  type     = (UINT)pd[KANDINSKY_TYPE];
		UINT  ap_ref   = (UINT)pd[KANDINSKY_AP_REF];
		FLOAT red      = pd[KANDINSKY_RED];
		FLOAT green    = pd[KANDINSKY_GREEN];
		FLOAT blue     = pd[KANDINSKY_BLUE];
		FLOAT x_off    = pd[KANDINSKY_XOFF];
		FLOAT y_off    = pd[KANDINSKY_YOFF];
		FLOAT z_off    = pd[KANDINSKY_ZOFF];
		FLOAT x_scale = pd[KANDINSKY_XSCALE];
		FLOAT y_scale = pd[KANDINSKY_YSCALE];
		FLOAT z_scale = pd[KANDINSKY_ZSCALE];


	// ---- diffuse colour
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} diffuse;
		diffuse.d = VS_D3DCOLOR_COLORVALUE(red, 0, 0, 1.0f);


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- kandinsky
		LONG  *l = context->GetLongParameter();
		FLOAT *k = (FLOAT *)*(l + 0);

		UINT  dimension   = KandinskyGetDimension(k);
		UINT  geotype     = KandinskyGetGeoType(k);
		UINT  num_element = KandinskyGetNumElements(k);

		UINT  kandinsky_index = KandinskyGetElementIndex(0,k);

		for (UINT element = 0; element < num_element; element++)
		{

			// ---- first
				UINT  first_index = kandinsky_index + 2;

			// ---- build....
				UINT num_vertices = KandinskyGetNumVertices(kandinsky_index, k);
				switch (geotype)
				{

					default:
					case KANDINSKY_GEOTYPE_POINT:
					case KANDINSKY_GEOTYPE_LINE:
						{
							UINT vertex_index = first_index;
							for (UINT vertex = 0; vertex < num_vertices; vertex++)
							{
								*(f + 0) = *(k + vertex_index)    * x_scale + x_off;
								*(f + 1) = *(k + vertex_index +1) * y_scale + y_off;
								*(f + 2) = *(k + vertex_index +2) * z_scale + z_off;
								*(f + 3) = diffuse.f;
								vertex_index += 3;
								f += 4;
							}
						}
						break;

					case KANDINSKY_GEOTYPE_FACE:
						{

							UINT num_triangles = num_vertices - 2;

							VsVector3 c = { 0, 0, 0 };
							if ( vertex_format & VS_NORMAL )
							{
								FLOAT *pk = k + first_index;
								VsVector3 a =
								{
									*(pk + 0) - *(pk + 3),
									*(pk + 1) - *(pk + 4),
									*(pk + 2) - *(pk + 5)
								};

								VsVector3 b =
								{
									*(pk + 6) - *(pk + 3),
									*(pk + 7) - *(pk + 4),
									*(pk + 8) - *(pk + 5)
								};

								a.Normalise();
								b.Normalise();
								c.CrossProduct(&b, &a);
							}


							for (UINT triangle = 0; triangle < num_triangles; triangle++)
							{
								UINT indices[3] =
								{
									first_index,
									first_index + (triangle+1) * dimension,
									first_index + (triangle+2) * dimension,
								};

								for (UINT vertex = 0; vertex < 3; vertex++)
								{

									UINT vertex_index = indices[vertex];
									*(f + 0) = *(k + vertex_index)     * x_scale + x_off;
									*(f + 1) = *(k + vertex_index + 1) * y_scale + y_off;
									*(f + 2) = *(k + vertex_index + 2) * z_scale + z_off;
									f += 3;

									if ( vertex_format & VS_NORMAL )
									{
										*(f + 0) = c.x;
										*(f + 1) = c.y;
										*(f + 2) = c.z;
										f += 3;
									}

									if ( vertex_format & VS_DIFFUSE )
									{
										*(f) = diffuse.f;
										f++;
									}

								}
							}
						}
						break;
				}

			// ---- next
				kandinsky_index = KandinskyNextElementIndex(kandinsky_index, k);

		}


		// ---- write data
		/*
		FLOAT *g = context->GetVertexBufferPtr();
		std::ofstream myfile("surf.csv");
		if (myfile.is_open())
		{
			for (UINT i = 0; i < 6; i++)
			{
				for (UINT j = 0; j < 24; j++)
				{
					CHAR msg[64];
					sprintf(msg, "%6.3f", *(g++));
					myfile << msg;
					myfile << ",";
				}
				myfile << "\n";
			}

			myfile.close();
		}
		*/
		OutputDebugString("Callback_Kandinsky_GetVertexBuffer \n");

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


UINT ObjConfig::KandinskyGetDimension(FLOAT k[])
{
	return (UINT)k[0];
}


UINT ObjConfig::KandinskyGetGeoType(FLOAT k[])
{
	return (UINT)k[1];
}


UINT ObjConfig::KandinskyGetNumColours(FLOAT k[])
{
	return (UINT)k[2];

}


UINT ObjConfig::KandinskyGetNumElements(FLOAT k[])
{
	return (UINT)k[3];
}


UINT ObjConfig::KandinskyGetNumVertices(UINT index, FLOAT k[])
{
	return (UINT)k[index];
}


UINT ObjConfig::KandinskyGetColourIndex(UINT id, FLOAT k[])
{
	return (id-1) *  // colour id
				4 +  // # of RGBA colour elements
				4;   // first free array element
}


UINT ObjConfig::KandinskyGetElementIndex(UINT id, FLOAT k[])
{

	UINT num_colours  = KandinskyGetNumColours(k);
	UINT num_elements = KandinskyGetNumElements(k);
	
	id = num_elements < id ? num_elements : id;

	UINT index_of_element = num_colours * 4 + 4;
	UINT element = 0;
	while ( element < id )
	{
		index_of_element = KandinskyNextElementIndex(index_of_element,k);
		element++;
	}

	return index_of_element;
}


UINT ObjConfig::KandinskyNextElementIndex(UINT index, FLOAT k[])
{

	UINT dimension = KandinskyGetDimension(k);
	UINT geotype = KandinskyGetGeoType(k);

	UINT num_vertices = (UINT)k[index];
	UINT size_of_num_vertex = 1;
	UINT size_of_colour_id = 1;

	return index + size_of_num_vertex + size_of_colour_id + num_vertices * dimension;
}


UINT ObjConfig::KandinskyGetNumGfxElements(UINT type, UINT num_vertices)
{

	UINT num_gfx_vertices = 0;
	switch (type)
	{
		default:
		case KANDINSKY_GEOTYPE_POINT:
			num_gfx_vertices = num_vertices;
			break;
		case KANDINSKY_GEOTYPE_LINE:
			num_gfx_vertices = num_vertices - 1;
			break;
		case KANDINSKY_GEOTYPE_FACE:
			num_gfx_vertices = num_vertices - 2;
			break;
	}

	return num_gfx_vertices;
}


UINT ObjConfig::KandinskyGetNumGfxVertices(UINT type, UINT num_vertices)
{
	return
		type == KANDINSKY_GEOTYPE_POINT ||
		type == KANDINSKY_GEOTYPE_LINE ? num_vertices :
		num_vertices + 2;
}
