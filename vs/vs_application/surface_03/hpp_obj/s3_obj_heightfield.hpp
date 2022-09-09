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
\param INT (source_action) 
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

 0  - Nothing    - don't update surface
'K' - Kamera     - update surface
'A' - Acquire    - update surface
'E' - Explore    - update surface
'M' - Measure    - don't update surface
'F' - Functional - update surface

*/
INT Surface_03::UpdateHeightfieldObjects(
		INT source_action,
		Surface_03 *p_s_03,
		BOOL source_object
	)
{

	//char msg[128];
	//sprintf(msg, "-> UHO %d\n",action);
	//OutputDebugString(msg);

	// ---- source object ? - zap target y ord
		if ( source_object )
		{
			p_s_03->ObjectLoiSpot_Reset(p_s_03);
			p_s_03->ObjectLoiAxis_Reset(p_s_03);
		}
		
	// ---- update mesh objects
		switch ( source_action )
		{
			case 'K': // kamera
			case 'A': // acquire
			case 'E': // explore
			case 'F': // functional
				{
					p_s_03->ObjectSurfMesh_Update(0, p_s_03);
					p_s_03->ObjectSurfEdge_Update(0, p_s_03);
				}
				break;

			case  0 : // nothing
			case 'M': // measure
			default:
				break;
		}

	// ---- update loi objects
		p_s_03->UpdateLoiObjects(source_action, p_s_03);

	return VS_OK;
}


// ---------- UpdateLoiObjects ----------
/*!
\brief
\author Gareth Edwards
\param INT (source_action) not used
\return INT - ERROR <=0 < VS_OK

\note invoked by UpdateHeightfieldObjects (see above) at setup
      and when surface mode selected or param is updated

\note invoked by Model::Tint_Explore_UpdateModel when a loi is selected

*/
INT Surface_03::UpdateLoiObjects(
		INT source_action,
		Surface_03 *p_s_03
	)
{

	// ---- update -+- fix loi stuff -+-
		p_s_03->ObjectLoiSpot_Update(p_s_03);
		p_s_03->ObjectLoiRect_Update(p_s_03);
		p_s_03->ObjectLoiRule_Update(p_s_03);
		p_s_03->ObjectLoiSect_Update(p_s_03);
		p_s_03->ObjectLoiAxis_Update(p_s_03);
		p_s_03->ObjectLoiAnno_Update(p_s_03);

	return VS_OK;
}



