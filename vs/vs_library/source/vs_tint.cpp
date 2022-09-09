////////////////////////////////////////////////////////////////////////////////

// ---------- vs_tint.cpp ----------
/*!
\file vs_tint.cpp
\brief Implementation of the T(abbed) int(erface) classes
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_tint.h"


using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


	// ---------- TintItem ----------


	// ---- cdtor
		TintItem::TintItem()
		{

			// ---- description
				text[0]  = 0;
				group_id = 0;
				elem_id  = 0; 

			// ---- details
				type     = 0;
				icon     = 0;
				status   = 0;
				x_off    = 0;

			// ---- atlas
				ap_group = 0;
				ap_row   = 0;
				ap_col   = 0;

			// ---- column conditions
				column_feed = 0;

			// ---- line conditions
				line_feed = 0;
				line_incr = 1;

		}


	// ---- set

		INT TintItem::SetXOff(FLOAT o)      { x_off       = o;       return VS_OK; }
		INT TintItem::SetElemId(UINT e_id)  { elem_id     = e_id;    return VS_OK; }
		INT TintItem::SetColumnLF(UINT cf)  { column_feed = cf;      return VS_OK; }
		INT TintItem::SetGroupId(UINT g_id) { group_id    = g_id;    return VS_OK; }
		INT TintItem::SetIcon(UINT i_id)    { icon        = i_id;    return VS_OK; }
		INT TintItem::SetLF(BOOL lf)        { line_feed   = lf;      return VS_OK; }
		INT TintItem::SetLFI(FLOAT li)      { line_incr   = li;      return VS_OK; }
		INT TintItem::SetText(CHAR *c)      { strcpy(text, c);       return VS_OK; }
		INT TintItem::SetType(UINT t)       { type        = t;       return VS_OK; }
		INT TintItem::SetStatus(BOOL s)     { status      = (INT)s;  return VS_OK; }

		// ---- atlas
			INT TintItem::SetAtlas(UINT group,UINT row,UINT col)
			{
				ap_group = group;
				ap_row   = row;
				ap_col   = col;
				return VS_OK;
			}


		// ---- head and body
			INT TintItem::Set(CHAR  *c, UINT   g_id, UINT   e_id, UINT   t, UINT   i_id)
			{ 
				strcpy(text, c);
				group_id = g_id;
				elem_id  = e_id;
				type     = t;
				icon     = i_id;
				return VS_OK;
			}


		// ---- set item
			INT TintItem::Set(CHAR  *c, UINT   g_id, UINT   e_id)
			{ 
				strcpy(text, c);
				group_id = g_id;
				elem_id  = e_id;
				return VS_OK;
			}


		// ---- set item with element id
			INT TintItem::Set(CHAR  *c, UINT   g_id)
			{ 
				strcpy(text, c);
				group_id = g_id;
				elem_id  = 0;
				return VS_OK;
			}


////////////////////////////////////////////////////////////////////////////////


	// ----------------------------------------------------------------------
	Tint::Tint()
	{
		INT result = Initialise(NULL, NULL, NULL);
	}
	Tint::~Tint()
	{
		;
	}


	// ----------------------------------------------------------------------
	INT Tint::Initialise(
			vs_system::GfxInterface *gfx,
			vs_system::ObjConfig    *obj,
			FLOAT(*get_config_param)(INT)
		)
	{

		// ---- injected
			_inj_gfx_interface = gfx;
			_inj_obj_config = obj;


		// ---- layout
			_x_tab_off = 0;
			_y_tab_off = 0;

			_selected_body_id = 0;

			_use_columns = FALSE;
			_num_columns = 3;


		// ---- names
			strcpy(_head_font_name, "");
			strcpy(_body_font_name, "");
			_build_font_name = _head_font_name;


		// ---- head
			_head_handle = NULL;
			strcpy(_head_name, "");
			for (INT i = 0; i < MAX_BODY_ITEM; i++)
			{
				_head_item_elm_handle[i] = 0;
			}
			_head_dspl_elm_count = 0;
			for (INT i = 0; i < MAX_BODY_ELM; i++)
			{
				_head_dspl_elm_handle[i] = 0;
			}
			

		// ---- body
			for (INT j = 0; j < MAX_BODY; j++)
			{
				_body_handle[j] = 0;
				_body_handle_ass_obj[j] = 0;
				for (INT i = 0; i < MAX_BODY_ITEM; i++)
				{
					_body_item_elm_handle[j][i] = 0;
				}

				_body_dspl_elm_count[j] = 0;
				for (INT i = 0; i < MAX_BODY_ELM; i++)
				{
					_body_dspl_elm_handle[j][i] = 0;
				}

				strcpy(_body_name[j], "");
				_body_id[j] = -1;
			}


		// ---- callback
			_callback_get_config_param = get_config_param;

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::AddHead(
			CHAR *head_name,
			UINT  head_lid
		)
	{

		TintItem list_of_items[MAX_BODY];
		INT number_of_items = 0;
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_handle[index] != 0 )
			{
				list_of_items[index].Set(
						_body_name[index],
						head_lid,
						_body_id[index],
						Tint::ITEM_TABS,
						0
					);
				number_of_items++;
			}
		}


		strcpy(_head_name, head_name);
		_head_handle = _inj_gfx_interface->AddDisplayList(head_name);


		_build_font_name = _head_font_name;

		INT result = BuildDisplayList(
				head_name,
				_head_handle,
				list_of_items,
				number_of_items,
				TRUE
			);

		return _head_handle;
	}


	// ----------------------------------------------------------------------
	INT Tint::AddBody(
			INT    handle_ass_obj,
			CHAR  *body_name,
			UINT   body_id,
			TintCallback *tint_callback
		)
	{

		_build_font_name = _body_font_name;

		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_handle[index] == 0 )
			{
				INT result = BuildBody(
						handle_ass_obj,
						body_name,
						body_id,
						index,
						tint_callback
					);
				return _body_handle[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::BuildBody(
			INT           handle_ass_obj,
			CHAR         *body_name,
			UINT          body_id,
			INT           index,
			TintCallback *tint_callback
		)
	{

		strcpy(_body_name[index], body_name);
		_body_handle_ass_obj[index] = handle_ass_obj;
		_body_id[index]             = body_id;
		_callback_for_body[index]   = *tint_callback;


		TintItem list_of_items[MAX_BODY_ITEM];
		INT number_of_items = _callback_for_body[index].callback_build_list(list_of_items);


		_body_handle[index] = _inj_gfx_interface->AddDisplayList(body_name);


		INT result = BuildDisplayList(
				_body_name[index],
				_body_handle[index],
				list_of_items,
				number_of_items,
				FALSE
			);

		return result;
	}


	// ----------------------------------------------------------------------
	INT Tint::BuildDisplayList(
			CHAR       *head_or_body_name, 
			INT         handle,
			TintItem    list_of_items[],
			INT         number_of_items,
			BOOL        is_head
		)
	{

		// ---- for enum
			using namespace vs_system;


		// --- local ----
			INT result = VS_OK;
			INT body_id    = 0;
			INT body_index = 0;


		// ---- note: head body_id and body_index set
			if ( handle == _head_handle )
			{
				;
			}
			else
			{
				body_id    = GetBodyId(head_or_body_name);
				body_index = GetBodyIndex(body_id);
				result     = SetSelectedBody(body_id);
			}


		// ---- initialise "callback" layout parameters

			// ---- proportional to icon dimensions
				FLOAT space_width  = _callback_get_config_param(SPACE_WIDTH);   // e.g. 0.10f;
				FLOAT space_height = _callback_get_config_param(SPACE_HEIGHT);  // e.g. 0.05f;
				FLOAT space_margin = _callback_get_config_param(SPACE_MARGIN);  // e.g. 0.05f;

			// ---- proportional to panel height
				FLOAT text_width   = _callback_get_config_param(TEXT_WIDTH);    // e.g. 0.035f;
				FLOAT text_height  = _callback_get_config_param(TEXT_HEIGHT);   // e.g. 0.035f;
				FLOAT icon_width   = _callback_get_config_param(ICON_WIDTH);    // e.g. 0.06f;
				FLOAT icon_height  = _callback_get_config_param(ICON_HEIGHT);   // e.g. 0.06f;
				FLOAT column_incr  = 1 / (FLOAT)_num_columns;

			// ---- proportional to icon dimensions
				FLOAT item_scalar  = _callback_get_config_param(ITEM_SCALAR);   // e.g. 0.75f;


		// --- calc
			space_width  *= icon_width;
			space_height *= icon_height;
			space_margin *= icon_width;


		// ---- small icon proportional to icon dimensions * scalar
			FLOAT small_icon_width = icon_width  * item_scalar;
			FLOAT small_icon_incr  = icon_height * item_scalar;


		// ---- prep amplify
			ElmContext::ElmButton button_list[64];
			UINT  button_count = 0;
			UINT  element_count= 1;


		// ---- local stuff
			FLOAT text_incr  = text_height*0.85f;
			FLOAT icon_incr  = icon_height;
			FLOAT x_now  = _x_tab_off;
			FLOAT x_col  = 0;
			FLOAT y_now  = _y_tab_off;
			FLOAT y_prev = y_now;
			FLOAT y_icon = y_now;


		// ---- text ------------------------------------------------------------
			auto convert_item_text_into_button = [&](TintItem *item)
			{
				button_list[button_count] =
					{
						"T", "T", item->group_id, item->elem_id,
						ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0, 0,
						x_now, y_now, text_width, text_height,
						0, 0, 0, 0, 0, 1, 1
					};
				strcpy(button_list[button_count].name, item->text);
				strcpy(button_list[button_count].text, item->text);
				button_count++;
			};


		// ---- buttons ------------------------------------------------------------
			auto convert_item_button_into_button = [&](TintItem *item)
			{
				// ---- local
					INT mod_body_id = item->elem_id;
					INT mod_body_index = GetBodyIndex(mod_body_id);

				// ---- atlas
					UINT icon_group = item->ap_group;
					UINT icon_row = item->ap_row;
					UINT icon_col = item->ap_col;

				// ---- select behaviour
					UINT behaviour = ElmContext::BEHAVIOUR_CLICK;
					UINT group_id = item->group_id;
					if ( item->type == ITEM_RADIO )
					{
						behaviour = ElmContext::BEHAVIOUR_RADIO;
					}
					else if ( item->type == ITEM_SWITCH )
					{
						behaviour = ElmContext::BEHAVIOUR_SWITCH;
					}
					else if ( item->type == ITEM_RADIO_LOCAL )
					{
						behaviour = ElmContext::BEHAVIOUR_RADIO_LOCAL;
					}
					else if (item->type == ITEM_CLUNK)
					{
						behaviour = ElmContext::BEHAVIOUR_CLUNK;
					}
					else if (item->type == ITEM_NULL)
					{
						behaviour = ElmContext::BEHAVIOUR_NULL;
					}

				// ---- add
					button_list[button_count++] =
						{
							"S", "S", item->group_id, item->elem_id,
							ElmContext::TYPE_ICON, behaviour, 0, item->status,
							x_now, y_icon, icon_width, icon_height,
							icon_group, 0, 0, icon_col, icon_row, 1, 1
						};
			};


		// ---- incdec ------------------------------------------------------------
			auto convert_item_incdec_into_button = [&](TintItem *item)
			{
				UINT icon_row = item->icon / ICONS_PER_ROW;
				UINT icon_col = item->icon - icon_row * ICONS_PER_ROW;

				FLOAT x_id = x_now;
				button_list[button_count++] =
					{
						"D", "D", item->group_id, item->elem_id + 1,
						ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_CLICK, 0, 0,
						x_id, y_icon, small_icon_width, small_icon_incr,
						VS_ATLAS_PAGE_GROUP_ONE, 0, 0, 3, 0, 1, 1
					};
				x_id += small_icon_width + space_width;
				button_list[button_count++] =
					{
						"I", "I", item->group_id, item->elem_id + 2,
						ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_CLICK, 0, 0,
						x_id, y_icon, small_icon_width, small_icon_incr,
						VS_ATLAS_PAGE_GROUP_ONE, 0, 0, 4, 0, 1, 1
					};
			};


		// ---- tabs ------------------------------------------------------------
			auto convert_item_tabs_into_button = [&](TintItem *item, INT ii)
			{
				FLOAT x_loc = ( space_width + icon_width  ) * (FLOAT)ii + x_now;

				INT   mod_body_id    = item->elem_id;
				BOOL  mod_selected   = mod_body_id == _selected_body_id ? TRUE : FALSE;
				INT   mod_body_index = GetBodyIndex(mod_body_id);

				INT   atlas_page_id = _callback_for_body[mod_body_index].callback_get_icon_info(TAB_ATLAS_PAGE_ID);
				INT   atlas_col     = _callback_for_body[mod_body_index].callback_get_icon_info(TAB_ATLAS_PAGE_COL);
				INT   atlas_row     = _callback_for_body[mod_body_index].callback_get_icon_info(TAB_ATLAS_PAGE_ROW);

				button_list[button_count++] =
					{
						"T", "T", item->group_id, item->elem_id,
						ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0, (UINT)mod_selected,
						x_loc, _y_tab_off, icon_width, icon_height,
						(UINT)atlas_page_id, 0, 0, (UINT)atlas_col, (UINT)atlas_row, 1, 1
					};
			};


		// ---- set exit y height
			FLOAT y_height_max = 0;
			FLOAT y_height = 0;
			BOOL  add_text = TRUE;


		// ---- amplify list of items into a button list
			for ( INT ii = 0; ii<number_of_items; ii++ )
			{

				// ---- init local stuff
					TintItem *item = &list_of_items[ii];


				// ---- y locate with column feed ?
					if ( _use_columns && item->column_feed )
					{
						y_height = 0;
						y_now    = _y_tab_off;
						y_prev   = y_now;
						x_col   += column_incr;
					}
					else
					{
						y_now = y_prev;
					}


				// ---- x locate
					x_now = x_col + _x_tab_off + item->x_off * icon_width + space_margin;


				// ---- text
					if ( item->type == Tint::ITEM_TABS )
					{
						add_text = FALSE;
					}
					else
					{

						INT len = strlen(item->text);
						if ( len )
						{
							add_text = TRUE;
							convert_item_text_into_button(item);

						}
					}


				// ---- text?

					// no line feed ? AND next item text ?
					UINT next_item_type = 0;
					TintItem *next_item = NULL;
					if ( !item->line_feed && ii != number_of_items - 1)
					{
						next_item = &list_of_items[ii+1];
						next_item_type = next_item->type;
					}

					// allow multiple text items on one line
					if ( item->type == ITEM_TEXT && next_item_type != ITEM_TEXT )
					{

						// ---- next line and force lf
							y_prev   = y_prev + text_incr + space_height;
							y_height = y_prev;
							add_text = FALSE;

						// ---- lf
							if ( item->line_feed )
							{

								// ---- next line
								y_prev = y_prev + (space_height * item->line_incr);

								// ---- set exit y height...
								y_height = y_prev;

							}
					}

				// ---- icon ....
					else
					{

						// ---- y inc to icon location
							y_icon = y_now + ( add_text ? text_incr : 0 ) + space_height;;


						// ---- item
							switch (item->type)
							{
								case ITEM_NULL:
								case ITEM_CLICK:
								case ITEM_CLUNK:
								case ITEM_RADIO:
								case ITEM_SWITCH:
								case ITEM_RADIO_LOCAL:
									convert_item_button_into_button(item);
									break;
								case ITEM_TABS:
									convert_item_tabs_into_button(item, ii);
									break;
								default:
									break;
							}


						// ---- lf
							if ( item->line_feed )
							{

								// ---- next line
									y_prev =  y_prev +
										( add_text ? text_incr : 0 ) +
											icon_height + (space_height * item->line_incr);

								// ---- set exit y height...
									y_height = y_prev;

								// ---- reset add_text ?
									if ( add_text ) add_text = FALSE;

							}
					}


				// ---- tabs item? - set exit y height... 
					if ( item->type == ITEM_TABS )
					{
						// ---- last ?
							if ( ii == number_of_items - 1 )
							{
								y_height = y_now + icon_incr + space_height;
							}
					}

				// ---- update y_height_max
					y_height_max = y_height_max < y_height ? y_height : y_height_max;

			}


		// ---- amplify the button list into a display list
			result = BuildDisplayList(
					handle,
					button_list,
					button_count,
					element_count,
					is_head
				);


		// ---- pad height
			y_height_max += space_height * 2;


		// ---- set head or body height ----
			if ( handle == _head_handle )
			{
				_head_height = y_height_max;
			}
			else
			{
				_body_height[body_index] = y_height_max;
			}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::BuildDisplayList(
			INT          handle,
			vs_system::ElmContext::ElmButton button_list[],
			UINT         button_count,
			UINT         element_count,
			BOOL         is_head
		)
	{

		// ---- for enum
			using namespace vs_system;


		// ---- local
			INT result = VS_OK;


		// ---- element lut
			UINT element_version_lut[3] =
			{
				ElmContext::VERSION_BACKGROUND,
				ElmContext::VERSION_ICON,
				ElmContext::VERSION_OUTLINE,
			};


		// ---- for each button - amplify list into display list of gfx elements
			INT elem_count = 0;
			FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];
			for ( UINT ib = 0; ib<button_count; ib++ )
			{

				// ---- type == icon THEN amplify # of elements
					element_count = button_list[ib].type == ElmContext::TYPE_ICON ? 3 : 1;


				// ---- type == text THEN version[0] == text ELSE version[0] == background
					element_version_lut[0] =
						button_list[ib].type == ElmContext::TYPE_TEXT ?
							ElmContext::VERSION_TEXT :
								ElmContext::VERSION_BACKGROUND;

				// ---- for each element
					for ( UINT ie = 0; ie<element_count; ie++ )
					{

						#define FRED

						// ---- add to display list
							INT button_handle = _inj_gfx_interface->AddElement(
									handle,
									button_list[ib].name,
									button_list[ib].group_id,
									#if defined(FRED)
									button_list[ib].id,
									#else
									elem_count,
									#endif
									&_inj_obj_config->Callback_Button_Configure
								);

						// ---- initialise
							ElmContext *elm = NULL;
							INT get_element_result = _inj_gfx_interface->GetElementContext(button_handle, &elm);
							if ( get_element_result != NULL )
							{

								// ---- set element font stuff
									result = elm->SetFontName(_build_font_name);
									result = elm->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);


								// ---- "text" ?
									INT selected_body_index = is_head ? 0 : GetBodyIndex(_selected_body_id);
									if ( button_list[ib].type == ElmContext::TYPE_TEXT  )
									{

										#if defined(FRED)

										//
										// TBD: dangerous usage of button_list[ib].id as an index!!
										//
										// 191115: Actually - bloody good idea.... allows user reference
										//         user reference by char tokens, e.g. 'p'..
										//

										// store "item" handle
										if ( is_head )
										{
											_head_item_elm_handle[button_list[ib].id] = button_handle;
										}
										else
										{
											_body_item_elm_handle[selected_body_index][button_list[ib].id] = button_handle;
										}

										// set name | name/value
										result = UpdateText(
														button_list[ib].group_id,
														button_list[ib].id
													);
										#else

										// store "item" handle
										if ( is_head )
										{
											_head_item_elm_handle[elem_count] = button_handle;
										}
										else
										{
											_body_item_elm_handle[selected_body_index][elem_count] = button_handle;
										}

										// set name | name/value
										result = UpdateText(
														button_list[ib].group_id,
														elem_count
													);

										#endif
									}
									else
									{
										result = elm->SetText(button_list[ib].text);
									}


								// ---- store "element" handle
									if ( is_head )
									{
										_head_dspl_elm_handle[elem_count] = button_handle;
									}
									else
									{
										_body_dspl_elm_handle[selected_body_index][elem_count] = button_handle;
									}

									
									if ( button_list[ib].behaviour == ElmContext::BEHAVIOUR_NULL )
									{
										result = elm->SetMultiActive(FALSE, 0);
									}


								// ---- set element stuff
									
									result = elm->SetBehaviour(button_list[ib].behaviour);
									result = elm->SetVersion(element_version_lut[ie]);
									result = elm->SetRGBA(1, 1, 1, 0.2f);
									result = elm->SetSwitchedOn((BOOL)button_list[ib].status);


								// ---- param list
									result = _inj_obj_config->ButtonSetDPT(
											param,
											vs_system::ElmContext::VIEWPORT_HEIGHT
										);
									result = _inj_obj_config->ButtonSetLocation(
											param,
											button_list[ib].x, button_list[ib].y,
											button_list[ib].width, button_list[ib].height
										);
									result = _inj_obj_config->ButtonSetTexture(
											param,
											button_list[ib].apg,
											button_list[ib].bg_col,   button_list[ib].bg_row,
											button_list[ib].icon_col, button_list[ib].icon_row,
											button_list[ib].dim_col,  button_list[ib].dim_row,
											0.01f
										);
									result = elm->SetFloatParameter(vs_system::ObjConfig::MAX_BUTTON_PARAM_USED, param);

							}

							elem_count++;


					}   // next element

			}   // next button


		// ---- store # elements
			if ( is_head )
			{
				_head_dspl_elm_count = elem_count;
			}
			else
			{
				INT selected_body_index = GetBodyIndex(_selected_body_id);
				_body_dspl_elm_count[selected_body_index] = elem_count;
			}

		return VS_OK;

	}


	// ----------------------------------------------------------------------
	INT Tint::UpdateStatus(
			INT   elem_id,
			BOOL  status
		)
	{

		INT result = VS_OK;
		INT body_handle = GetBodyHandle(_selected_body_id);
		INT elm_handle = _inj_gfx_interface->GetFirst(body_handle);
		while (elm_handle != 0 )
		{
			vs_system::ElmContext *elm_context = NULL;
			result = _inj_gfx_interface->GetElementContext(elm_handle, &elm_context);
			if (elm_context->GetId() == (UINT)elem_id)
			{
				elm_context->SetSwitchedOn(status);
			}
			elm_handle = _inj_gfx_interface->GetNext(elm_handle);
		}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::UpdateRadio(
			INT elem_id
		)
	{

		INT result = VS_OK;
		INT body_handle = GetBodyHandle(_selected_body_id);
		INT elm_handle = _inj_gfx_interface->GetFirst(body_handle);
		while (elm_handle != 0 )
		{
			vs_system::ElmContext *elm_context = NULL;
			result = _inj_gfx_interface->GetElementContext(elm_handle, &elm_context);
			if (elm_context->GetId() == (UINT)elem_id)
			{
				elm_context->SetSwitchedOn(TRUE);
			}
			else
			{
				elm_context->SetSwitchedOn(FALSE);
			}
			elm_handle = _inj_gfx_interface->GetNext(elm_handle);
		}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::UpdateAllText()
	{

		INT result = 0;
		INT selected_body_index = GetBodyIndex(_selected_body_id);


		for (INT body = 0; body < MAX_BODY; body++)
		{
			if ( _body_handle[body] != 0 )
			{

				SelectBody(_body_id[body]);

				INT total_items = _body_dspl_elm_count[body];
				for (INT index = 0; index < total_items; index++)
				{
					result = UpdateText(0, index);

					INT elm_handle = _body_item_elm_handle[body][index];
					vs_system::ElmContext *elm_context = NULL;
					result = _inj_gfx_interface->GetElementContext(elm_handle, &elm_context);
					if ( result != NULL )
					{
						elm_context->SetBitmask(VS_ELM_SETUP_ALL);
						elm_context->SetConfigured(FALSE);
					}

				}

			}
		}

		_selected_body_id = selected_body_index;

		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::UpdateText(
			INT   group_id_not_used,
			INT   elem_id
		)
	{

		// ---- local
			INT result = VS_OK;


		// --- handle
			INT selected_body_index = GetBodyIndex(_selected_body_id);
			INT elm_handle = _body_item_elm_handle[selected_body_index][elem_id];


		// ---- check context ok... ?
			vs_system::ElmContext *elm_context = NULL;
			result = _inj_gfx_interface->GetElementContext(elm_handle, &elm_context);
			if ( result != NULL )
			{
			
				// ---- associated object
					INT handle_ass_obj = (INT)_body_handle_ass_obj[selected_body_index];


				// ---- action ?
					if ( _callback_for_body[selected_body_index].callback_add_actions != NULL )
					{
						FLOAT *float_param = elm_context->GetFloatParameter();
						float_param += vs_system::ObjConfig::MAX_BUTTON_PARAM_USED;
						result = _callback_for_body[selected_body_index].callback_add_actions(
								handle_ass_obj,
								elem_id,
								float_param
							);
						elm_context->SetActions(result);
					}


				// ----local element context ok
					BOOL  rep_text  = FALSE;
					BOOL  add_text  = FALSE;
					BOOL  add_value = FALSE;
					FLOAT value = 0;
					CHAR  text[VS_MAXCHARLEN];


				// ---- get text ?
					strcpy(text, elm_context->GetName().c_str());
					if ( _callback_for_body[selected_body_index].callback_add_text != NULL )
					{
						result = _callback_for_body[selected_body_index].callback_add_text(
								handle_ass_obj,
								elem_id,
								&rep_text,
								&add_text,
								text
							);
						if ( rep_text )
						{
							std::string name;
							name.assign(text);
							elm_context->SetName(name);
						}
					}


				// ---- get value ?
					CHAR suffix[VS_MAXCHARNAME];
					strcpy(suffix, "");
					if (_callback_for_body[selected_body_index].callback_add_value != NULL)
					{
						result = _callback_for_body[selected_body_index].callback_add_value(
								handle_ass_obj,
								elem_id,
								&add_value,
								&value,
								suffix
							);
					}


				// ---- format value (needs to be big)
					CHAR value_text[VS_MAXCHARLEN];
					if ( add_value )
					{

						CHAR formated_value[16];

						if ( *suffix == ' ' || *suffix == '0' )
						{
							sprintf(formated_value, "%d", (INT)value);
						}
						else
						{
							switch ( *suffix )
							{
								case '1':
									sprintf(formated_value, "%5.1f", value);
									break;
								case '2':
									sprintf(formated_value, "%5.2f", value);
									break;
								case '3':
									sprintf(formated_value, "%5.3f", value);
									break;
								default:
									sprintf(formated_value, "%5.4f", value);
									break;
							}
						}
						*suffix = ' ';

						snprintf(value_text, VS_MAXCHARLEN, "%s %s%s%s",
								elm_context->GetName().c_str(),
								add_text ? text : "",
								formated_value,
								suffix
							);

						result = elm_context->SetText(value_text);

					}
					else
					{
						snprintf(value_text, VS_MAXCHARLEN, "%s%s",
								elm_context->GetName().c_str(),
								add_text ? text : ""
							);
						result = elm_context->SetText(value_text);
					}

			}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetHeadDsplElmCount(
		)	
	{
		return _head_dspl_elm_count;
	}
	

	// ----------------------------------------------------------------------
	INT Tint::GetHeadDsplElmHandles(
			INT *elem_handles
		)	
	{
		INT elem_count = _head_dspl_elm_count;
		for ( INT e=0; e< elem_count; e++ )
		{
			elem_handles[e] = _head_dspl_elm_handle[e];
			vs_system::ElmContext *h = (vs_system::ElmContext *)elem_handles[e];
		}
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	CHAR * Tint::GetBodyFontName()
	{
		return _body_font_name;
	}

	
	// ----------------------------------------------------------------------
	INT Tint::GetBodyDsplElmCount(
			const UINT body_id
		)	
	{
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_id[index] == body_id )
			{
				return _body_dspl_elm_count[index];
			}
		}
		return VS_ERROR;
	}
	

	// ----------------------------------------------------------------------
	INT Tint::GetBodyDsplElmHandles(
			const UINT body_id,
			INT *elem_handles
		)	
	{
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_id[index] == body_id )
			{
				INT elem_count = _body_dspl_elm_count[index];
				for ( INT e=0; e< elem_count; e++ )
				{
					elem_handles[e] = _body_dspl_elm_handle[index][e];
					vs_system::ElmContext *h = (vs_system::ElmContext *)elem_handles[e];
				}
				return VS_OK;
			}
		}
		return VS_ERROR;
	}


// ---------- GetBodyDsplElmContext ----------
/*-
\brief  get body display list element contexts
\author Gareth Edwards

\param  UINT   - body id
\param  UINT   - body element count
\param  CHAR[] - body element id char (e.g. 'a')

\param  INT[]  - [out] body element id indices
\return ElmContext *[] - [out] body element context

\return - VS_ERROR <= 0 < VS _OK

Function
========

Initialises a body element id index and element context arrays.

These provide access by index to all elements in a
display list created when a tint item (TintItrem) list
is data amplified into a display list.

The cunning part is that the index is the original tint
item char id (e.g. CHAR 'a' id == INT 97 index).

Usgae example:

	Random acccess to element context by using element char id as index
	===================================================================

	Given INT element_id, an element id (e.g. 'a')...

	UINT number_of_element_ids = sizeof(tint_lighting_properties.element_lid_char) / sizeof(CHAR);
	for (UINT i = 0; i < number_of_element_ids; i++)
	{
		if ( tint_lighting_properties.element_lid_char[i] == (CHAR)element_id )
		{

			....

			for (UINT i = 0; i < number_of_element_ids; i++)
			{ 
				{
					tint_lighting_properties.element_id_context[i]->SetText(str_mesh_param_name.c_str());
				}
			}
		}
	}

	Simples!

*/
	INT Tint::GetBodyDsplElmContext(
		UINT   body_lid,
		INT    num_element_lid,
		CHAR   element_lid[],

		INT    element_id_index[],
		vs_system::ElmContext  *element_context[]

		)
	{

		// ---- local
			INT    result = VS_OK;


		// ---- get # of elements in a display list
			UINT body_dspl_elem_count = GetBodyDsplElmCount(body_lid);


		// ---- get display list element handles
			INT body_dspl_elem_handles[MAX_BODY_ELM];
			result = GetBodyDsplElmHandles(body_lid, body_dspl_elem_handles);


		// ---- get display list element contexts
			vs_system::ElmContext *body_dspl_element_context[MAX_BODY_ELM];
			result = _inj_gfx_interface->GetElementContexts(
					body_dspl_elem_count,
					body_dspl_elem_handles, 
					body_dspl_element_context
				);
		

		// ---- set ONLY first instance of matched element id contexts
			UINT number_of_element_ids = num_element_lid;
			UINT last_id = 0;
			for ( UINT e = 0; e< body_dspl_elem_count; e++ )
			{
				UINT gid = body_dspl_element_context[e]->GetGroupId();
				UINT  id = body_dspl_element_context[e]->GetId();

				if ( id != last_id )
				{
					for ( UINT i = 0; i < number_of_element_ids; i++ )
					{
						if ( id == element_lid[i] )
						{
							element_context[i]  = body_dspl_element_context[e];
							element_id_index[i] = e;
							break;
						}
					}
				}

				last_id = id;
			}

		return VS_OK;
	}


	// ----------------------------------------------------------------------

	INT Tint::GetBodyElemDsplInfo(
			UINT  body_lid,
			INT   num_element_lid,
			CHAR  element_lid[],
			CHAR  group_lid[],

			TintElemDsplInfo element_dspl_info[],
			UINT hash_element_lid_to_dspl_index[]
		)
	{

		// ---- local
			INT result = VS_OK;


		// ---- get # of elements in a display list
			INT body_dspl_elem_count = GetBodyDsplElmCount(body_lid);


		// ---- get display list element handles
			INT body_dspl_elem_handles[MAX_BODY_ELM];
			result = GetBodyDsplElmHandles(body_lid, body_dspl_elem_handles);


		// ---- get display list element contexts
			vs_system::ElmContext *body_dspl_element_context[MAX_BODY_ELM];
			result = _inj_gfx_interface->GetElementContexts(
					body_dspl_elem_count,
					body_dspl_elem_handles,
					body_dspl_element_context
				);
		

		// ---- init element display info
			UINT elem = 0;
			UINT number_of_element_ids = num_element_lid;

			for ( INT e = 0; e < body_dspl_elem_count; e++ )
			{
				UINT id = body_dspl_element_context[e]->GetId();
				{
					for ( UINT i = 0; i < number_of_element_ids; i++ )
					{
						if ( id == element_lid[i] )
						{
							element_dspl_info[elem].lid       = id;
							element_dspl_info[elem].group_lid = group_lid[i];
							element_dspl_info[elem].handle    = body_dspl_elem_handles[e];
							element_dspl_info[elem].context   = body_dspl_element_context[e];
							elem++;
						}
					}
				}
			}


		// ---- init element lid to element display list index hash table
			UINT edi_start = 0;
			for (UINT i = 0; i < (UINT)num_element_lid; i++)
			{
				CHAR lid = element_lid[i];
				for (UINT edi = edi_start; edi < (UINT)body_dspl_elem_count; edi++)
				{
					if (element_dspl_info[edi].lid == lid)
					{
						hash_element_lid_to_dspl_index[lid] = edi;
						edi_start = edi;
						break;
					}
				}
			}

		return body_dspl_elem_count;
	}


	// ----------------------------------------------------------------------

	#include <string>
	#include <sstream>
	#include <iostream>

	//! Create a custom convert function, remove the tailing zeros if necessary.  
	template<typename T>
	std::string ToTidyString(const T &n)
	{
		std::ostringstream oss;
		oss << n;
		std::string s = oss.str();
		int dotpos = s.find_first_of('.');
		if ( dotpos != std::string::npos )
		{
			int ipos = s.size() - 1;
			while ( s[ipos] == '0' && ipos>dotpos+1 )
			{
				--ipos;
			}
			s.erase(ipos + 1, std::string::npos);
		}
		return s;
	}

	INT Tint::ElemDsplTextTidy(
			INT id,
			CHAR *label,
			FLOAT value,
			CHAR *text
		)
	{

		// ! "NA" ? - label and return (multi state flag) TRUE
		if ( strcmp(label, "NA") != 0 )
		{
			sprintf(text, "%s: %f",label,value );
			std::string s = ToTidyString(text);
			strcpy(text, s.c_str());
		}
		// else zap label and return (multi state flag) FALSE
		else
		{
			strcpy(text, "");
			return FALSE;
		}

		return TRUE;
	}


	// ----------------------------------------------------------------------

	INT Tint::ElemDsplSwitch(
			UINT  body_lid,
			UINT  param_lid,
			UINT  elem_lid,
			BOOL *multi_state
		)
	{

		// ---- get display list element handles
			INT elem_handles[MAX_BODY_ELM];
			INT num_elem = GetBodyDsplElmCount(body_lid);
			GetBodyDsplElmHandles(body_lid, elem_handles);


		// ---- set element state, multi state, multi state active & behaviour
			for (INT e = 0; e < num_elem; e++)
			{
				vs_system::ElmContext *elem_context = NULL;
				_inj_gfx_interface->GetElementContext(elem_handles[e], &elem_context);

				INT group_id = elem_context->GetGroupId();
				if ( group_id == param_lid )
				{
					INT lid = elem_context->GetId();
					if ( lid == elem_lid )
					{
						elem_context->SetState(*multi_state);
						elem_context->SetMultiState(*multi_state);
						elem_context->SetMultiActive(*multi_state, 0);
						INT behaviour = *multi_state ?
							vs_system::ElmContext::BEHAVIOUR_RADIO :
								vs_system::ElmContext::BEHAVIOUR_NULL;
						elem_context->SetBehaviour(behaviour);
					}
				}
			}

		return VS_OK;
	}


	// ----------------------------------------------------------------------

	INT Tint::ElemDsplInfoSwitch(
			INT  element_dspl_count,
			TintElemDsplInfo element_dspl_info[],
			INT  group_lid,
			INT  element_lid,
			BOOL state
		)
	{

		// ---- local
			INT result = VS_OK;


		// --- ignore ZERO groups
			if ( group_lid == 0 ) return VS_ERROR;


		// ---- set
			if ( element_lid == 0 )
			{
				for (int e = 0; e < element_dspl_count; e++)
				{
					if ( group_lid == element_dspl_info[e].group_lid )
						element_dspl_info[e].context->SetSwitchedOn(state);
				}
			}
			else
			{
				for (int e = 0; e < element_dspl_count; e++)
				{
					if (
							group_lid == element_dspl_info[e].group_lid &&
							element_lid == element_dspl_info[e].lid
						)
					{
						element_dspl_info[e].context->SetSwitchedOn(state);
					}
				}
			}

		return VS_OK;
	}


	// ----------------------------------------------------------------------

	INT Tint::ElemDsplInfoSetText(
			INT  element_dspl_count,
			TintElemDsplInfo element_dspl_info[],
			INT  group_lid,
			INT  element_lid,
			CHAR *text
		)
	{

		// ---- local
			INT result = VS_ERROR;


		// --- ignore ZERO groups
			if ( group_lid == 0 ) return VS_ERROR;


		// ---- set
			std::string str_text(text);
			if ( element_lid == 0 )
			{
				for (int e = 0; e < element_dspl_count; e++)
				{
					if ( group_lid == element_dspl_info[e].group_lid )
						result = element_dspl_info[e].context->SetText(str_text);
				}
			}
			else
			{
				for (int e = 0; e < element_dspl_count; e++)
				{
					if (
							group_lid == element_dspl_info[e].group_lid &&
							element_lid == element_dspl_info[e].lid
						)
					{
						result = element_dspl_info[e].context->SetName(str_text);
						result = element_dspl_info[e].context->SetText(str_text);
					}
				}
			}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetBodyHandle(
			const CHAR *body_name
		)	
	{
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_handle[index] != 0 && strcmp(body_name, _body_name[index]) == 0 )
			{
				return _body_handle[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetBodyHandle(
			const UINT body_id
		)	
	{
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_id[index] == body_id )
			{
				return _body_handle[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	FLOAT Tint::GetBodyHeight(
			const CHAR *body_name
		)	
	{
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_handle[index] != 0 && strcmp(body_name, _body_name[index]) == 0 )
			{
				return _body_height[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetBodyId(
			CHAR  *body_name
		)
	{
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_handle[index] != 0 && strcmp(body_name, _body_name[index]) == 0 )
			{
				return _body_id[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetBodyIndex(
			const UINT body_id
		)
	{
		for (INT index = 0; index < MAX_BODY; index++)
		{
			if ( _body_handle[index] != 0 && body_id == _body_id[index] )
			{
				return index;
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetBodyName(
			const INT body_handle,
			CHAR *body_name
		)
	{
		for (INT index = 0; index < MAX_BODY; index++)
		{

			if ( _body_handle[index] == body_handle )
			{
				strcpy(body_name, _body_name[index]);
				return VS_OK;
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	CHAR * Tint::GetHeadFontName()
	{
		return _head_font_name;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetHeadHandle()
	{

		return _head_handle;
	}


	// ----------------------------------------------------------------------
	FLOAT Tint::GetHeadHeight()
	{

		return _head_height;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetHeadName(
			CHAR *head_name
		)
	{
		strcpy(head_name, _head_name);
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetSelectedBodyId()
	{
		return _selected_body_id;
	}


	// ----------------------------------------------------------------------
	INT Tint::GetTabOffset(
			FLOAT *x,
			FLOAT *y
		)
	{
		*x = _x_tab_off;
		*y = _y_tab_off;
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::SelectBody(
			const INT body_id
		)
	{
		for (INT index = 0; index < MAX_BODY; index++)
		{
			if (_body_handle[index] != 0 && body_id == _body_id[index] )
			{
				_selected_body_id = _body_id[index];
				return VS_OK;
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::SetCallbackGetBuildInfo(
		FLOAT(*func)(INT)
		)
	{
		_callback_get_config_param = func;
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::SetColumns(BOOL use, UINT num)
	{
		_use_columns = use;
		_num_columns = num;
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::SetSelectedBody(
			const UINT id
		)
	{
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_id[index] == id )
			{
				_selected_body_id = _body_id[index];
				return VS_OK;
			}
		}
		return VS_ERROR;
	}

	
	// ----------------------------------------------------------------------
	INT Tint::SetBodyFont(CHAR *font_name)
	{
		INT len = strlen(font_name);
		if ( len == 0 || len >= VS_MAXCHARNAME ) return VS_ERROR;
		strcpy(_body_font_name, font_name);
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::SetHeadFont(CHAR *font_name)
	{
		INT len = strlen(font_name);
		if (len == 0 || len >= VS_MAXCHARNAME) return VS_ERROR;
		strcpy(_head_font_name, font_name);
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::SetTabOffset(
			const FLOAT x,
			const FLOAT y
		)
	{

		//
		// Note: offset is proportional to icon height
		//

		_x_tab_off = x;
		_y_tab_off = y;
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT Tint::SetDisplayListsConfigure()
	{

		if (_head_handle != 0)
		{
			_inj_gfx_interface->SetDisplayListConfigure(_head_handle, VS_ELM_SETUP_ALL);
		}
		
		for ( INT index = 0; index < MAX_BODY; index++ )
		{
			if ( _body_id[index] != 0 )
			{
				_inj_gfx_interface->SetDisplayListConfigure(_body_handle[index], VS_ELM_SETUP_ALL);
			}
		}

		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT Tint::AddEnclose(
			INT handle_display_list
		)
	{

		// ---- local
			INT result = VS_OK;


		// ---- lambda ------------------------------------------------------------
			auto add_label = [&](
					INT   handle,
					CHAR *label,
					FLOAT x_off,
					FLOAT y_off
				)
			{

				// ---- add
					INT handle_elm = _inj_gfx_interface->AddElement(
							handle,
							"",
							0,
							0,
							&_inj_obj_config->Dbz_Callback_Configure_Text
						);

					vs_system::ElmContext *elm = NULL;
					result = _inj_gfx_interface->GetElementContext(handle_elm, &elm);


				// ---- set elm
					elm->SetFontName(_head_font_name);
					elm->SetFontAlignment(0);
					elm->SetText(label);
					elm->SetRGBA(0.15f, 0.3f, 1.0f, 1.0f);


				// ---- set db
					//
					// note: Dbz_SetConfig == 0 to overide Dbz_ assignment of font name
					//
					FLOAT *p = elm->GetFloatParameter();
					result = _inj_obj_config->Dbz_SetDefault(p);
					result = _inj_obj_config->Dbz_SetArt(p, _inj_obj_config->DBZ_ART_LEFT);
					result = _inj_obj_config->Dbz_SetConfig(p, 0);
					result = _inj_obj_config->Dbz_SetVisible(p, 1);
					result = _inj_obj_config->Dbz_SetScale(p, 0, 0.1f);
					result = _inj_obj_config->Dbz_SetTranslate(p, x_off, y_off);
			};


		// ---- lambda ------------------------------------------------------------
			auto add_enclose = [&](
					INT   handle,
					FLOAT x_off,
					FLOAT y_off,
					FLOAT width,
					FLOAT height,
					FLOAT radius,
					FLOAT m_rgb
				)
			{

				// ---- enclose
					INT handle_elm = _inj_gfx_interface->AddElement(
							handle,
							"",
							0,
							0,
							&_inj_obj_config->Dbz_Callback_Configure_Rect
						);

					vs_system::ElmContext *elm = NULL;
					result = _inj_gfx_interface->GetElementContext(handle_elm, &elm);


				// ---- set elm param
					// FIX - was elm->SetRGBA(1, 1, 1, 0.25f);
					elm->SetRGBA(0.15f, 0.3f, 1.0f, 1.0f);


				// ---- set db param
					FLOAT *p = elm->GetFloatParameter();
					result =_inj_obj_config->Dbz_SetDefault(p);
					result =_inj_obj_config->Dbz_SetArt(p,_inj_obj_config->DBZ_ART_LEFT);
					result =_inj_obj_config->Dbz_SetConfig(p, 0);
					result =_inj_obj_config->Dbz_SetVisible(p, 1);
					result =_inj_obj_config->Dbz_SetScale(p, 1.0f, 1.0f);
					result =_inj_obj_config->Dbz_SetTranslate(p, x_off, y_off);

					result =_inj_obj_config->Dbz_SetRectDimension(p, width, height);
					result =_inj_obj_config->Dbz_SetRectQuad(p, 60, radius + 0.006f, radius);

					result =_inj_obj_config->Dbz_SetRectRGBMax(p, _r_max, _g_max, _b_max);
					result =_inj_obj_config->Dbz_SetRectRGBMin(p, _r_min, _g_min, _b_min);

			};


		// ---- get name
			CHAR name[VS_MAXCHARNAME];
			BOOL head = FALSE;
			INT head_handle = GetHeadHandle();
			if ( handle_display_list == head_handle )
			{
				head = TRUE;
				result = GetHeadName(name);
			}
			else
			{
				result = GetBodyName(handle_display_list, name);
			}

	
		// ---- NEED to EXIT if not OK


		// ---- local
			FLOAT x_tab_off  = 0;
			FLOAT y_tab_off  = 0;
			FLOAT height     = 0;
			FLOAT radius     = 0.05f;
			FLOAT m_rgb      = 0.2f;


		// ---- offset
			//
			// Note: tab offset is proportional to icon height
			//
			result = GetTabOffset(&x_tab_off, &y_tab_off);


		// ---- width & x offset
			FLOAT width =  0.95f;
			FLOAT x_off = -0.5f + x_tab_off;
			FLOAT y_off =  y_tab_off;


		// ---- height & then update y offset
			if ( head )
			{
				height = ( GetHeadHeight() - y_off ) * 2;
			}
			else
			{
				height = ( GetBodyHeight(name) - y_off ) * 2;
			}
			height = height < radius * 2 ? radius * 2 : height;
			y_off = height / 2 - 1 + y_off * 2;


		// ---- adjust to "fit" radii
			y_off -= radius/4;
			height += radius;


		// ---- just do it....
			add_enclose(
					handle_display_list,
					x_off,
					y_off,
					width,
					height,
					radius,
					m_rgb
				);

		// ---- and name it....
			add_label(
					handle_display_list,
					name,
					x_off - width  / 2,
					y_off - height / 2 - x_tab_off
				);

		return VS_OK;
	}

