////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_heightfield.hpp ----------
/*!
\file s2_obj_heightfield.h
\brief 
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


// ---------- UpdateHeighfieldObjects ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

\note 04-02-21

      NOT updating the surface unless required is a good idea but should
      be flagged by the various change mechanisms and effected at a much
	  lower level.

*/
INT Surface_02::UpdateHeightfieldObjects(
		INT index,
		Surface_02 *p_s_02,
		BOOL change_surface_version
	)
{

	// ---- change ? - reset target loi
		if ( change_surface_version )
		{
			p_s_02->ObjectLoiSpot_Reset(p_s_02);
			p_s_02->ObjectLoiAxis_Reset(p_s_02);
		}
		

	// ---- update surface
		p_s_02->ObjectSurfMesh_Update(0, p_s_02);
		p_s_02->ObjectSurfEdge_Update(0, p_s_02);
		// p_s_02->ObjectSurfMesh_ChangeTexture(0, p_s_02);
		p_s_02->ObjectSurfQuad_Update(0, p_s_02);


	// ---- update loi
		p_s_02->ObjectLoiSpot_Update(p_s_02);
		p_s_02->ObjectLoiRect_Update(p_s_02);
		p_s_02->ObjectLoiRule_Update(p_s_02);
		p_s_02->ObjectLoiAxis_Update(p_s_02);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
