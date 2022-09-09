////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_heightfield.hpp ----------
/*!
\file s3_obj_heightfield.h
\brief 
\author Gareth Edwards
*/

// ---------- UpdateHeighfieldObjects ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_03::UpdateHeightfieldObjects(
		INT index,
		Surface_03 *p_s_03,
		BOOL source_object
	)
{

	// ---- source object ? - zap target y ord
		if ( source_object )
		{
			p_s_03->ObjectLoiSpot_Reset(p_s_03);
			p_s_03->ObjectLoiAxis_Reset(p_s_03);
		}
		
	// ---- update mesh
		p_s_03->ObjectSurfMesh_Update(0, p_s_03);
		p_s_03->ObjectSurfEdge_Update(0, p_s_03);

	// ---- update loi
		p_s_03->ObjectLoiSpot_Update(p_s_03);
		p_s_03->ObjectLoiRect_Update(p_s_03);
		p_s_03->ObjectLoiRule_Update(p_s_03);
		p_s_03->ObjectLoiSect_Update(p_s_03);
		p_s_03->ObjectLoiAxis_Update(p_s_03);

	return VS_OK;
}



