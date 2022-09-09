////////////////////////////////////////////////////////////////////////////////

// ---------- s2_kontrol.cpp ----------
/*!
\file s2_kontrol.cpp
\brief Implementation of the Kontrol class
\author Gareth Edwards
\note 
*/

#include "../header/s2_kontrol.h"


////////////////////////////////////////////////////////////////////////////////


using namespace surface_02;


// ---------- Private Implementation of PI_Props class ----------
class Kontrol::PI_Kontrol
{

public:

	// ---- cdtor
		PI_Kontrol();
		virtual ~PI_Kontrol();


	// ---- managed by vs
		vs_system::AppDependency *app_dependency;
		vs_system::AppView       *app_view;


	// ---- managed by application
		surface_02::Model        *model;


	// ---- managed by Kontrol::PI_Kontrol
		vs_system::AppKontrol    *app_kontrol;

};


////////////////////////////////////////////////////////////////////////////////


#include "../../surface_shared/hpp/kontrol_class.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- CallbackByPickElement ----------
/*!
\brief Callback by pick element event
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked from:
	GfxInterface::RenderViewPort ->
		GfxInterface::PI_Props::isPickPanel ->
			AppKontrol::CallbackPickElementFromGfxInterface ->
				AppKontrol::UpdatePickElementFromGfxInterface

*/
INT Kontrol::CallbackByPickElement(
		INT handle_app_kontrol,
		INT handle_element
	)
{

	// ---- local
		INT result = 0;
		BOOL reset_mouse = TRUE;
		vs_system::AppKontrol    *app_kontrol = (vs_system::AppKontrol *)handle_app_kontrol;
		vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();


	// ---- has to be ok.... to get to here!!!
		vs_system::ElmContext *elem_context = NULL;
		result = app_dependency->GetGfxInterface()->GetElementContext(handle_element, &elem_context);
		if ( elem_context == NULL ) return VS_ERROR;


	// ======== REQUIRED ========

		INT group_id = elem_context->GetGroupId();
		switch ( group_id )
		{

			// ======== View - REQUIRED ========

				case TOOLBAR_GROUP_ID:
				case TOOLBAR_GROUP_ID_SIDE:
				case TOOLBAR_GROUP_ID_LAYOUT:
					{
						INT elem_id = elem_context->GetId();
						switch ( elem_id )
						{
							default:
								{
									UINT model_handle = app_kontrol->GetHandleModel();
									Model *model = (Model *)model_handle;
									model->SetPanelsLayoutByElemId(elem_id);
								}
								break;
							case TOOLBAR_ELEM_ID_EXIT:
								{
									exit(0);
									return VS_ERROR;
								}
								break;
						}
					}
					break;

			// ======== Tint - REQUIRED ========

				case TINT_HEAD_2DS_LID:
					result = TintUpdateHead(
							app_kontrol,
							group_id,
							handle_element
						);
					break;


				case TINT_BODY_ACQUIRE_LID:
				case TINT_BODY_ACQUIRE_ARRAY_LID:
				case TINT_BODY_ACQUIRE_PARAM_LID:

				case TINT_BODY_FUNCTION_LID:
				case TINT_BODY_FUNCTION_PARAM_LID:

				case TINT_BODY_EXPLORE_LID:
				case TINT_BODY_EXPLORE_LID_GROUP_1:
				case TINT_BODY_EXPLORE_LID_GROUP_2:

					//
					// elem_context->GetId() is used as a tint body id,
					// and set 0 to avoid tint model update
					//
					// see UpdateTintImgText for example
					//
					result = TintUpdateBody(
							app_kontrol,
							group_id,
							handle_element
						);
					break;

		// ======== Kpad - REQUIRED ========

			case KPAD_HEAD_LID:
				result = KpadUpdateHead(
						app_kontrol,
						group_id,
						handle_element
					);
					break;

			case KPAD_BODY_INPUT_LID:
				result = KpadUpdateBody(
						app_kontrol,
						group_id,
						handle_element
					);
					break;

			default:
				reset_mouse = FALSE;
				break;
		}


	//	---- ok ?
		if ( reset_mouse )
		{
			// ---- done...
			//
			//  Reset by AppKontrol::UpdateElementFromCallback
			//
			return VS_OK;
		}


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ======== Tint - REQUIRED ========


// ---- Tint - required
#include "../hpp_tint/s2_tint_kontrol.hpp"


// ---- Keypad - required

#include "../../surface_shared/hpp_kpad/model_kpad_kontrol.hpp"


////////////////////////////////////////////////////////////////////////////////

