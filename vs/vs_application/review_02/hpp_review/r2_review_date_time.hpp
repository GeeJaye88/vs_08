////////////////////////////////////////////////////////////////////////////////

// ---------- r2_review_date_time.hpp ----------
/*!
\file r2_review_date_time.hpp
\brief
\author Gareth Edwards
*/


#ifndef REVIEW_02_REVIEW_DATE_AND_TIME
#define REVIEW_02_REVIEW_DATE_AND_TIME


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- ConvertDateTimeToMillisecond ----------
/*!
\brief Convert a datetime stack to milliseconds
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
ULONG Model::ConvertDateTimeToMillisecond(UINT(*)[DATETIME_MAX_STACK])
{

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetArchiveSaveStatus ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetArchiveSaveStatus(BOOL status)
{
	pi_model->archive_save_on_at_runtime = status;
	return VS_OK;
}


// ---------- SetStreamingStatus ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetStreamingStatus(BOOL status)
{
	pi_model->streaming_on_at_runtime = status;
	return VS_OK;
}


// ---------- SetStreamingAndOnvifStatus ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetStreamingAndOnvifStatus(BOOL status)
{
	pi_model->streaming_and_onvif_on_at_runtime = status;
	return VS_OK;
}


// ---------- SetButtonSwitch ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetButtonSwitch(UINT group_id, UINT id, BOOL value)
{
	// Local
	vs_system::ElmContext *element_context = NULL;
	UINT button_list_index[2] = { CONTEXT_BUTTON_INDEX, NAVIGATION_BUTTON_INDEX };

	// Search context and navigation button lists
	for ( UINT list=0; list<2; list++ )
	{
		UINT list_index = button_list_index[list];
		UINT items = button_list_size[list_index];

		// FOR each button
		for ( UINT element=0; element<items; element++ )
		{
			// FOR each version
			for ( UINT version=0; version<3; version++ )
			{
				// IF context THEN
				button_element_descriptor *bed = button_list[list_index] + element;
				element_context = (vs_system::ElmContext *)bed->handle_element_context[version]; // UPDATE ALL
				if ( element_context != NULL )
				{
					// IF group THEN
					if ( element_context->GetGroupId() == group_id )
					{
						// IF id THEN
						if ( element_context->GetId() == id )
						{
							bed->switched_on = value ? 1 : 0;
							element_context->SetSwitchedOn(value);
						}
					}
				}
			}
		}
	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- UpdateButtonSwitch ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::UpdateButtonSwitch(UINT group_id, UINT id, BOOL value)
{
	INT result = SetButtonSwitch(group_id, id, value);
	return result;
}


// ---------- UpdateDateTimeList ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/

//
// Note: was std::string datetime[8]
//

INT Model::UpdateDateTimeList(std::string datetime[], std::string filename)
{

	// ---- initialise YYYYMMDD_HHMMSS_MSC LUT
		UINT len       = (UINT)filename.length() - 4;
		UINT start[8]  = { 0, len-19, len-15, len-13, len-10, len-8, len-6, len-3 };
		UINT length[8] = { 1,      4,      2,      2,      2,     2,     2,     3 };


	// ---- FOR all elements in list
		UINT number_elements = button_list_size[NAVIGATION_BUTTON_INDEX]; // WAS TIMELINE_
		vs_system::ElmContext *element_context = NULL;
		for ( UINT element=0; element<number_elements; element++ )
		{

			// ---- context ? - THEN update ONLY text
				element_context = (vs_system::ElmContext *) navigation_button_list[element].handle_element_context[2];
				if ( element_context != NULL )
				{

					// ---- element id == datetime index ?
						UINT element_group_id = element_context->GetGroupId();
						UINT element_id       = element_context->GetId();
						UINT element_version =  element_context->GetVersion();
						if (
								element_version == vs_system::ElmContext::X_VERSION_TEXT &&
								element_id >= DATETIME_DECADE &&
								element_id <= DATETIME_MSEC
							)
						{

							// ---- snip text from filename
								std::string txt =
									element_id == DATETIME_DECADE ?
										"A" :
										filename.substr(start[element_id], length[element_id]);

							// ---- button element context, update button_element_descriptor and datetime
								element_context->SetText(txt);
								navigation_button_list[element].text = txt;       // NOTE: Here element is the index,
								datetime[element_id] = txt;                       // ... here element_id is the index

							// ---- exit if completed update
								if ( element_id == DATETIME_MSEC ) break;
						}
				}
		}


	return VS_OK;
}


// ---------- UpdateDateTimeList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/
INT Model::UpdateDateTimeList(UINT index)
{
	// ---- FOR all elements in list ----
	INT  result = VS_OK;
	UINT count = 0;
	BOOL switched_on = FALSE;
	UINT number_elements = button_list_size[TIMELINE_BUTTON_INDEX];
	vs_system::ElmContext *element_context = NULL;
	for (UINT element = 0; element<number_elements; element++)
	{

		// For each version
		for (UINT version = 0; version <= 2; version++)
		{

			// IF context THEN update ONLY text
			element_context = (vs_system::ElmContext *)navigation_button_list[element].handle_element_context[version];
			if (element_context != NULL)
			{

				// IF element id == datetime index THEN
				UINT element_group_id = element_context->GetGroupId();
				UINT element_id       = element_context->GetId();
				UINT element_version  = element_context->GetVersion();


				// IF index match THEN switch on ELSE off
				if ( count == index - 1 )
					result = element_context->SetSwitchedOn(TRUE);
				else
					result = element_context->SetSwitchedOn(FALSE);


				// exit if complete
				if ( element_id == DATETIME_MSEC ) return VS_OK;


				// increment
				if (version == 2)
					if (element_id >= DATETIME_DECADE && element_id <= DATETIME_SECOND)
						count++;
			}
		}
	}

	return VS_OK;
}


// ---------- UpdateTimelineList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

handle_dspl_context_button
BYTE setup_bitmask = VS_ELM_SETUP_NONE;
BYTE setup_update = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING

*/
INT Model::UpdateTimelineList(
		UINT  datetime_index,
		UINT  datetime_list[],
		BOOL  clip_active,
		LLONG clip_list[],
		UINT  items,
		UINT  stride,
		LLONG list[]
	)
{

	// ---- local
		CHAR text[32];
		vs_system::ElmContext *element_context = NULL;
		button_element_descriptor *button_list[2] = { &timeline_button_list[0], &clipline_button_list[0] };
		UINT list_id[2]     = {
				TIMELINE_BUTTON_INDEX,
				CLIPLINE_BUTTON_INDEX
			};
		INT  list_handle[2] = {
				pi_model->handle_dspl_timeline_button,
				pi_model->handle_dspl_clipline_button
			};


	// ---- HIDE ALL
		for ( UINT list_index=0; list_index<2; list_index++ )
		{
			for ( UINT element=0; element<100; element++ )
			{
				button_element_descriptor *bed = button_list[list_index] + element;
				bed->visible = 0;
				for ( UINT version = 0; version<3; version++ )
				{
					element_context = (vs_system::ElmContext *)bed->handle_element_context[version]; // UPDATE ALL
					if ( element_context != NULL ) element_context->SetVisible(FALSE);
				}
			}
		}


	// ---- loop index depends on clip ON/OFF status
		UINT max_list_index = clip_active ? 2 : 1;


	// ---- SHOW # BY CONTEXT
	for ( UINT list_index=0; list_index<max_list_index; list_index++ )
	{

		for ( UINT element=0; element<items; element++ )
		{

			for ( UINT version = 0; version<3; version++ )
			{

				// ---- IF context THEN
				button_element_descriptor *bed = button_list[list_index] + element;
				element_context = (vs_system::ElmContext *)bed->handle_element_context[version]; // UPDATE ALL
				if ( element_context != NULL )
				{

					// ---- local list values
					UINT list_offset = element * stride;
					LLONG list_id    = list[list_offset];
					LLONG list_value = list[list_offset+1];
					LLONG list_mod   = list[list_offset+2];
					LLONG list_ms    = list[list_offset+3];
					sprintf(text, "%d", (INT)list_mod);


					// ---- select on index ----
					switch ( list_index ) 
					{


						case 0: // ---------- "Timeline" ----------
							{
								// IF version == TEXT THEN set text
								if ( version == vs_system::ElmContext::X_VERSION_TEXT) element_context->SetText(text);

								// IF switched OFF THEN switch ON - ELSE switch OFF
								bed->id = (UINT)list_id;

								// NEW - was: bed->text = datetime_index == 7 ? "" : text;

								if ( datetime_index == 7 )
								{
									if ( list_value > 0 )
									{
										INT i = atoi(text);
										if (i > 100)
										{
											text[2] = '0';
											text[3] = 0;
										}
										bed->text = text;
									}
									else
										bed->text = "";
								}
								else
								{
									bed->text = text;
								}

								// "HERE"

								//bed->text = datetime_index == 7 ? "" : text;

								if ( list_value > 0 )
								{
									bed->switched_on = 1;
									bed->behaviour = vs_system::ElmContext::BEHAVIOUR_RADIO;
									bed->sub_behaviour = vs_system::ElmContext::SUB_BEHAVIOUR_RADIO_SWITCH;
									bed->visible = 1;
									bed->version_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;

									element_context->SetText(bed->text);
									element_context->SetSwitchedOn(TRUE);
									element_context->SetSwitchedOnPreviously(TRUE);
									element_context->SetId((UINT)list_id);
									element_context->SetMultiActive(TRUE, 0);
									element_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_RADIO);
									element_context->SetSubBehaviour(vs_system::ElmContext::SUB_BEHAVIOUR_RADIO_SWITCH);
									element_context->SetVisible(TRUE);
									element_context->SetBitmask(bed->version_bitmask);

									element_context->SetTextVertical(datetime_index > 4 ? TRUE : FALSE);
								}
								else
								{
									bed->switched_on = 0;
									bed->behaviour = vs_system::ElmContext::BEHAVIOUR_NONE;
									bed->visible = 1;
									bed->version_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;

									element_context->SetText(bed->text);
									element_context->SetSwitchedOn(FALSE);
									element_context->SetSwitchedOnPreviously(FALSE);
									element_context->SetMultiActive(FALSE, 0);
									element_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
									element_context->SetSubBehaviour(vs_system::ElmContext::SUB_BEHAVIOUR_NONE);
									element_context->SetVisible(TRUE);
									element_context->SetBitmask(bed->version_bitmask);

									element_context->SetTextVertical(datetime_index > 4 ? TRUE : FALSE);
								}

							}
							break;


						case 1: // ---------- "Clipline" ----------
							{
								// IF millisconds THEN
								if ( datetime_index == 7 )
								{
									// IF(this ms is ge AND le clip) THEN
									list_value = list_ms >= clip_list[0] && list_ms <= clip_list[1] ? 1 : 0;
								}
								// ELSE seconds or something else, so catch beginning and end of clip....
								else
								{
									// So, IF ( this ms is ge AND le clip ) AND ( next ms is ge AND le clip ) THEN 
									LLONG list_ms_next = list[stride + 3] - list[0 + 3] + list_ms;
									if ( list_ms >= clip_list[0] && list_ms <= clip_list[1] ||
										list_ms_next >= clip_list[0] && list_ms_next <= clip_list[1])
									{
										list_value = 1;
									}
									else
									{
										list_value = 0;
									}
								}

								// IF switched OFF THEN switch ON
								bed->id = (UINT)list_id;
								bed->text = datetime_index == 7 ? "" : text;

								// IF between IN AND OUT THEN display clip
								if ( list_value > 0 )
								{

									bed->switched_on = 1;
									bed->behaviour = vs_system::ElmContext::BEHAVIOUR_RADIO;
									bed->visible = 1;
									bed->version_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;

									element_context->SetText("");// bed->text);
									element_context->SetSwitchedOn(TRUE);
									element_context->SetSwitchedOnPreviously(TRUE);
									element_context->SetId((UINT)list_id);
									element_context->SetMultiActive(TRUE, 0);
									element_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_RADIO);
									element_context->SetVisible(TRUE);
									element_context->SetBitmask(bed->version_bitmask);

									element_context->SetTextVertical(FALSE); //  list_index > 5 ? TRUE : FALSE );
								}

							}
							break;

					}   // switch ( list_index )  

				}   // if ( element_context != NULL )

			}   // for ( UINT version = 0; version<3; version++ )

		}   // for ( UINT element=0; element<items; element++ )


		// ---- reconfig ----
		button_list_size[list_id[list_index]] = items;


		// ---- BUG BUG BUG : timeline memory leak ----
		gfx->SetConfigure(list_handle[list_index]);


	}   // for ( UINT list_index=0; list_index<max_list_index; list_index++ )

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- HighlightTimelineList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY BACKGROUND
*/
INT Model::HighlightTimelineList(
		UINT  list_index,
		UINT  list_value,
		UINT  items,
		UINT  stride,
		LLONG list[]
	)
{
	// Local
	vs_system::ElmContext *element_context = NULL;
	button_element_descriptor *button_list = &timeline_button_list[0];

	// FOR each button
	for ( UINT element=0; element<items; element++ )
	{
		// FOR each version
		// for ( UINT version = 0; version<3; version++ )
		{
			// IF context THEN
			button_element_descriptor *bed = button_list + element;
			bed->switched_on = 0;
			element_context = (vs_system::ElmContext *)bed->handle_element_context[0]; // UPDATE ONLY BACKGROUND
			if ( element_context != NULL )
			{
				// Get element "on " status
				UINT list_offset = element * stride;
				element_context->SetSwitchedOn(FALSE);
				element_context->SetSwitchedOnPreviously(FALSE);
				BOOL on = FALSE;
				switch ( list_index )
				{
					default:
						if ( list[list_offset] == list_value && list[list_offset+1] >= 0 ) on = TRUE;
						break;
					case 7:
						if ( element == list_value / 10 ) on = TRUE;
						break;
				}

				// IF "on" THEN set
				if ( on )
				{
					bed->switched_on = 1;
					element_context->SetSwitchedOn(TRUE);
					element_context->SetSwitchedOnPreviously(TRUE);
				}
			}
		}
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- UpdateContextualState ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::UpdateContextualState(
		BOOL timeline_is_created,
		UINT previous_next_state,
		UINT first_last_state
	)
{

	// ---- local ----
	vs_system::ElmContext *element_context = NULL;
	UINT button_list_index[2] = { CONTEXT_BUTTON_INDEX, NAVIGATION_BUTTON_INDEX };


	// ---- search context and navigation button lists ----
	UINT total = sizeof(button_list_index) / sizeof(UINT);
	for (UINT list = 0; list<total; list++)
	{
		UINT list_index = button_list_index[list];
		UINT items = button_list_size[list_index];


		// FOR each button
		for ( UINT element=0; element<items; element++ )
		{
			// FOR each version
			for ( UINT version = 0; version<3; version++ )
			{
				// IF context THEN
				button_element_descriptor *bed = button_list[list_index] + element;
				element_context = (vs_system::ElmContext *)bed->handle_element_context[version]; // UPDATE ALL
				if ( element_context != NULL )
				{

					// IF context buttons THEN
					UINT group_id = element_context->GetGroupId();
					if ( group_id == CONTEXT_BUTTON_GROUP_ID )
					{
						// Assume that element is "active"
						UINT id = element_context->GetId();
						BOOL active = TRUE;
						switch ( id )
						{
							case CONTEXT_PREVIOUS:
								active = previous_next_state & 1;
								break;
							case CONTEXT_NEXT:
								active = previous_next_state & 2;
								break;
							case CONTEXT_FIRST:
								active = first_last_state & 1;
								break;
							case CONTEXT_LAST:
								active = first_last_state & 2;
								break;
							default:
								break;
						}

						// IF timeline created AND active THEN
						if ( timeline_is_created && active )
						{
							element_context->SetMultiActive(TRUE, 0);
							element_context->SetBehaviour(button_list[list_index][element].behaviour);
						}
						else
						{
							element_context->SetMultiActive(FALSE, 0);
							element_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
						}

					}

					// ELSE IF navigation buttons THEN - eh!!!
					/*
					else if ( group_id == NAVIGATION_BUTTON_GROUP_ID )
					{

						BOOL on = element_context->GetSwitchedOn();
						if ( on ) 
						{
							element_context->SetSwitchedOn(TRUE);
						}
						else
						{
							element_context->SetSwitchedOn(FALSE);
						}
					}
					*/

				}
			}
		}
	}

	return VS_OK;
}


// ---------- UpdateClipState ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::UpdateClipState(
		BOOL timeline_is_created, 
		UINT clip_bitmask
	)
{

	/// ---- local test ----
	
	//
	// IF change THEN....
	//

	{

		// ---- local ----
		vs_system::ElmContext *element_context = NULL;
		UINT list_index = NAVIGATION_BUTTON_INDEX;
		UINT items = button_list_size[list_index];


		// FOR -- each button --
		for (UINT element = 0; element < items; element++)
		{

			// FOR -- each version --
			for (UINT version = 0; version < 3; version++)
			{

				// IF -- context -- THEN --
				button_element_descriptor *bed = button_list[list_index] + element;
				element_context = (vs_system::ElmContext *)bed->handle_element_context[version]; // UPDATE ALL
				if (element_context != NULL)
				{

					// IF -- navigation button group -- THEN --
					UINT group_id = element_context->GetGroupId();
					if (group_id == NAVIGATION_BUTTON_GROUP_ID)
					{

						// ---- assume that element is "active" ----
						UINT id = element_context->GetId();
						BOOL active = TRUE;
						switch (id)
						{
							case NAVIGATION_CLIP_CLEAR:
								active = clip_bitmask & 1 || clip_bitmask & 2 ? TRUE : FALSE;
								break;
							case NAVIGATION_CLIP_LOOP:
							case NAVIGATION_CLIP_ARCHIVE:
								active = clip_bitmask & 1 && clip_bitmask & 2 ? TRUE : FALSE;
								break;

								/*
								case NAVIGATION_CLIP_LOOP:
								active = clip_bitmask & 1 && clip_bitmask & 2 ? TRUE : FALSE;
								break;
								case NAVIGATION_CLIP_ARCHIVE:
								active = clip_bitmask & 1 && clip_bitmask & 2 ? TRUE : FALSE;
								break;
								*/
							default:
								break;
						}


						// IF -- timeline created -- AND -- active -- THEN --
						if (timeline_is_created && active)
						{
							element_context->SetMultiActive(TRUE, 0);
							element_context->SetBehaviour(button_list[list_index][element].behaviour);
						}
						else
						{
							element_context->SetMultiActive(FALSE, 0);
							element_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
						}

					}
				}
			}
		}
	}

	return VS_OK;
}


#endif // REVIEW_02_REVIEW_DATE_AND_TIME


