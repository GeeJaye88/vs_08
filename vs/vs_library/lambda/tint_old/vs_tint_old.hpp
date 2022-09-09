////////////////////////////////////////////////////////////////////////////////

// ---------- vs_tint.hpp ----------
/*!
\file vs_tint.hpp
\brief application include files.
\author Gareth Edwards
*/

#pragma once

using namespace review_02;


// ---------- TintItem ----------


class TintItem
{

public:

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


	// ---- description
		CHAR  text[32];
		UINT  group_id;  // group logical id
		UINT  elem_id;   // element logical id


	// ---- details
		UINT  type;      // ITEM_, see below... 
		UINT  icon;      // ICON_, see below... OR row * # icon (per row) + col
		UINT  status;    // 0/1
		FLOAT x_off;     // 0 (false) or 1, 2, .... # spaces
	

	// ---- atlas
		UINT  ap_group;  // atlas page group
		UINT  ap_row;    // atlas page row
		UINT  ap_col;    // atlas page col


	// ---- line conditions
		BOOL  column_feed;


	// ---- line conditions
		BOOL  line_feed;   // lf
		FLOAT line_incr;   // lf scalar increment (e.g. SPACE_HEIGHT * line_incr)


	// ---- indiv properties
		INT SetXOff(FLOAT o)      { x_off       = o;       return VS_OK; }
		INT SetElemId(UINT e_id)  { elem_id     = e_id;    return VS_OK; }
		INT SetColumnLF(UINT cf)  { column_feed = cf;      return VS_OK; }
		INT SetGroupId(UINT g_id) { group_id    = g_id;    return VS_OK; }
		INT SetIcon(UINT i_id)    { icon        = i_id;    return VS_OK; }
		INT SetLF(BOOL lf)        { line_feed   = lf;      return VS_OK; }
		INT SetLFI(FLOAT li)      { line_incr   = li;      return VS_OK; }
		INT SetText(CHAR *c)      { strcpy(text, c);       return VS_OK; }
		INT SetType(UINT t)       { type        = t;       return VS_OK; }
		INT SetStatus(BOOL s)     { status      = (INT)s;  return VS_OK; }


	// ---- atlas
		INT SetAtlas(UINT group,UINT row,UINT col)
		{
			ap_group = group;
			ap_row   = row;
			ap_col   = col;
			return VS_OK;
		}

	// ---- head and body
		INT Set(CHAR  *c, UINT   g_id, UINT   e_id, UINT   t, UINT   i_id)
		{ 
			strcpy(text, c);
			group_id = g_id;
			elem_id  = e_id;
			type     = t;
			icon     = i_id;
			return VS_OK;
		}

	// ---- set item 
		INT Set(CHAR  *c, UINT   g_id, UINT   e_id)
		{ 
			strcpy(text, c);
			group_id = g_id;
			elem_id  = e_id;
			return VS_OK;
		}

	// ---- set item with element id
		INT Set(CHAR  *c, UINT   g_id)
		{ 
			strcpy(text, c);
			group_id = g_id;
			elem_id  = 0;
			return VS_OK;
		}
};


// ---------- TintCallback ----------

struct TintCallback
{
	INT(*callback_build_list)   (TintItem[]);
	INT(*callback_get_icon_info)(INT);
	INT(*callback_update_model) (INT, INT);
	INT(*callback_update_text)  (INT, INT);
	INT(*callback_add_actions)  (INT, INT, FLOAT *);
	INT(*callback_add_text)     (INT, INT, BOOL *, BOOL*, CHAR *);
	INT(*callback_add_value)    (INT, INT, BOOL *, FLOAT *, CHAR *);
};


// ---------- Tint ----------

class Tint
{

public:

	enum
	{
		TAB_ATLAS_PAGE_ID  = 1,
		TAB_ATLAS_PAGE_COL = 2,
		TAB_ATLAS_PAGE_ROW = 3,

		ICON_ATLAS_PAGE_ID = 4,

		TINT_XOFFSET   = 12,
		HEAD_YOFFSET   = 14,
		BODY_YOFFSET   = 15,

		COL_HEIGHT     = 16,
		COL_WIDTH    = 17,

		SPACE_WIDTH    = 4,
		SPACE_HEIGHT   = 5,
		SPACE_MARGIN   = 11,

		TEXT_WIDTH     = 6,
		TEXT_HEIGHT    = 7,
		ICON_WIDTH     = 8,
		ICON_HEIGHT    = 9,
		ITEM_SCALAR    = 10,

		ICONS_PER_ROW  = 8,

		ITEM_TEXT      = 1,
		ITEM_CLUNK     = 2,
		ITEM_CLICK     = 4,
		ITEM_RADIO     = 8,
		ITEM_SWITCH    = 16,
		ITEM_TABS      = 32,
		ITEM_CHANNEL   = 64,

	};


	// ---- constructor -----------------------------------------------------
	Tint()
	{
		INT result = Initialise(NULL, NULL, NULL);
	}


	// ---- destructor ------------------------------------------------------
	~Tint()
	{
		;
	}


	// ----------------------------------------------------------------------
	INT Initialise(
			vs_system::GfxInterface *gfx,
			vs_system::ObjConfig    *obj,
			FLOAT(*get_config_param)(INT)
		)
	{

		_inj_gfx_interface = gfx;
		_inj_obj_config = obj;

		_x_tab_off = 0;
		_y_tab_off = 0;

		_selected_body_id = 0;

		_use_columns = FALSE;
		_num_columns = 3;

		_head_handle = NULL;

		for (INT j = 0; j < _max_body; j++)
		{
			_body_handle[j] = 0;
			for (INT i = 0; i < _max_body_elem; i++)
			{
				_body_handle_elem[j][i] = 0;
			}

			strcpy(_body_name[j], "");
			_body_id[j] = -1;
		}

		_callback_get_config_param = get_config_param;

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT AddHead(
			CHAR *head_name,
			UINT  head_lid
		)
	{

		TintItem list_of_items[_max_body];
		INT number_of_items = 0;
		for ( INT index = 0; index < _max_body; index++ )
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


		INT result = BuildDisplayList(
				head_name,
				_head_handle,
				list_of_items,
				number_of_items
			);

		return _head_handle;
	}


	// ----------------------------------------------------------------------
	INT AddBody(
			INT    handle_ass_obj,
			CHAR  *body_name,
			UINT   body_id,
			TintCallback *tint_callback
		)
	{

		for ( INT index = 0; index < _max_body; index++ )
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
	INT BuildBody(
			INT    handle_ass_obj,
			CHAR  *body_name,
			UINT   body_id,
			INT    index,
			TintCallback *tint_callback
		)
	{

		strcpy(_body_name[index], body_name);
		_body_handle_ass_obj[index] = handle_ass_obj;
		_body_id[index]             = body_id;
		_callback_for_body[index]   = *tint_callback;


		TintItem list_of_items[_max_body_elem];
		INT number_of_items = _callback_for_body[index].callback_build_list(list_of_items);


		_body_handle[index] = _inj_gfx_interface->AddDisplayList(body_name);


		INT result = BuildDisplayList(
				_body_name[index],
				_body_handle[index],
				list_of_items,
				number_of_items
			);

		return result;
	}


	// ----------------------------------------------------------------------
	INT BuildDisplayList(
			CHAR       *head_or_body_name, 
			INT         handle,
			TintItem    list_of_items[],
			INT         number_of_items
		)
	{

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
						ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
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
					else if ( item->type == ITEM_CHANNEL )
					{
						behaviour = ElmContext::BEHAVIOUR_CHANNEL;
					}
					else if (item->type == ITEM_CLUNK)
					{
						behaviour = ElmContext::BEHAVIOUR_CLUNK;
					}
				// ---- add
					button_list[button_count++] =
						{
							"S", "S", item->group_id, item->elem_id,
							ElmContext::TYPE_ICON, behaviour, item->status,
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
						ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_CLICK, 0,
						x_id, y_icon, small_icon_width, small_icon_incr,
						VS_ATLAS_PAGE_GROUP_ONE, 0, 0, 3, 0, 1, 1
					};
				x_id += small_icon_width + space_width;
				button_list[button_count++] =
					{
						"I", "I", item->group_id, item->elem_id + 2,
						ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_CLICK, 0,
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
				INT   atlas_col = _callback_for_body[mod_body_index].callback_get_icon_info(TAB_ATLAS_PAGE_COL);
				INT   atlas_row = _callback_for_body[mod_body_index].callback_get_icon_info(TAB_ATLAS_PAGE_ROW);

				button_list[button_count++] =
					{
						"T", "T", item->group_id, item->elem_id,
						ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, (UINT)mod_selected,
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
					if ( item->type == ITEM_TEXT )
					{
						// ---- next line and force lf
							y_prev   = y_prev + text_incr + space_height;
							y_height = y_prev;
							add_text = FALSE;
					}

				// ---- icon ....
					else
					{

						// ---- y inc to icon location
							y_icon = y_now + ( add_text ? text_incr : 0 ) + space_height;;


						// ---- item
							switch (item->type)
							{
								case ITEM_CLICK:
								case ITEM_CLUNK:
								case ITEM_RADIO:
								case ITEM_SWITCH:
								case ITEM_CHANNEL:
									convert_item_button_into_button(item);
									break;
								//case ITEM_INCDEC:
								//	convert_item_incdec_into_button(item);
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


				// ---- last tabs item? - set exit y height... 
					if ( item->type == ITEM_TABS )
					{
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
					element_count
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
	INT BuildDisplayList(
			INT  handle,
			vs_system::ElmContext::ElmButton button_list[],
			UINT button_count,
			UINT element_count
		)
	{

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
			INT item_counter = 0;
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

						// ---- add to display list
							INT button_handle = _inj_gfx_interface->AddElement(
									handle,
									button_list[ib].name,
									button_list[ib].group_id,
									button_list[ib].id,
									&_inj_obj_config->Callback_Button_Configure
								);


						// ---- initialise
							ElmContext *elm = NULL;
							INT get_element_result = _inj_gfx_interface->GetElementContext(button_handle, &elm);
							if ( get_element_result != NULL )
							{

								// ---- set element font stuff
									result = elm->SetFontName("Context");
									result = elm->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);


								// ---- "text" ?
									
									if ( button_list[ib].type == ElmContext::TYPE_TEXT  )
									{

										// store handle
										INT selected_body_index = GetBodyIndex(_selected_body_id);
										_body_handle_elem[selected_body_index][button_list[ib].id] = button_handle;

										// set name | name/value
										result = UpdateText(
														button_list[ib].group_id,
														button_list[ib].id
													);
									}
									else
									{
										result = elm->SetText(button_list[ib].text);
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

							item_counter++;

							

					}   // next element

			}   // next button


			// ---- store #
				INT selected_body_index = GetBodyIndex(_selected_body_id);
				_body_total_items[selected_body_index] = item_counter;


		return VS_OK;

	}


	// ----------------------------------------------------------------------
	INT UpdateStatus(
			INT   elem_id,
			BOOL  status
		)
	{

		INT result = VS_OK;
		INT body_handle = GetBodyHandle(_selected_body_id);
		INT elem_handle = _inj_gfx_interface->GetFirst(body_handle);
		while ( elem_handle != 0 )
		{
			vs_system::ElmContext *elm_context = NULL;
			result = _inj_gfx_interface->GetElementContext(elem_handle, &elm_context);
			if (elm_context->GetId() == (UINT)elem_id)
			{
				elm_context->SetSwitchedOn(status);
			}
			elem_handle = _inj_gfx_interface->GetNext(elem_handle);
		}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT UpdateRadio(
			INT elem_id
		)
	{

		INT result = VS_OK;
		INT body_handle = GetBodyHandle(_selected_body_id);
		INT elem_handle = _inj_gfx_interface->GetFirst(body_handle);
		while ( elem_handle != 0 )
		{
			vs_system::ElmContext *elm_context = NULL;
			result = _inj_gfx_interface->GetElementContext(elem_handle, &elm_context);
			if (elm_context->GetId() == (UINT)elem_id)
			{
				elm_context->SetSwitchedOn(TRUE);
			}
			else
			{
				elm_context->SetSwitchedOn(FALSE);
			}
			elem_handle = _inj_gfx_interface->GetNext(elem_handle);
		}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT UpdateAllText()
	{

		INT result = 0;
		INT selected_body_index = GetBodyIndex(_selected_body_id);


		for (INT body = 0; body < _max_body; body++)
		{
			if ( _body_handle[body] != 0 )
			{

				SelectBody(_body_id[body]);

				INT total_items = _body_total_items[body];
				for (INT index = 0; index < total_items; index++)
				{
					result = UpdateText(0, index);

					INT handle_elm = _body_handle_elem[body][index];
					vs_system::ElmContext *elm_context = NULL;
					result = _inj_gfx_interface->GetElementContext(handle_elm, &elm_context);
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
	INT UpdateText(
			INT   group_id_not_used,
			INT   elem_id
		)
	{

		// ---- local
			INT result = VS_OK;


		// --- handle
			INT selected_body_index = GetBodyIndex(_selected_body_id);
			INT handle_elm = _body_handle_elem[selected_body_index][elem_id];


		// ---- check context ok... ?
			vs_system::ElmContext *elm_context = NULL;
			result = _inj_gfx_interface->GetElementContext(handle_elm, &elm_context);
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
					CHAR  text[VS_MAXCHARNAME];


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


				// ---- format value
					CHAR value_text[VS_MAXCHARNAME];
					if ( add_value )
					{

						CHAR formated_value[16];

						if (*suffix == ' ' || *suffix == '0')
						{
							sprintf(formated_value, "%d", (INT)value);
						}
						else
						{
							switch (*suffix)
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

						sprintf(value_text, "%s %s%s%s",
								elm_context->GetName().c_str(),
								add_text ? text : "",
								formated_value,
								suffix
							);

						result = elm_context->SetText(value_text);

					}
					else
					{
						sprintf(value_text, "%s%s",
								elm_context->GetName().c_str(),
								add_text ? text : ""
							);
						result = elm_context->SetText(value_text);
					}

			}

		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT GetBodyHandle(
			const CHAR *body_name
		)	
	{
		for ( INT index = 0; index < _max_body; index++ )
		{
			
			if ( _body_handle[index] != 0 && strcmp(body_name, _body_name[index]) == 0 )
			{
				return _body_handle[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT GetBodyHandle(
			const UINT body_id
		)	
	{
		for ( INT index = 0; index < _max_body; index++ )
		{
			
			if ( _body_id[index] == body_id )
			{
				return _body_handle[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	FLOAT GetBodyHeight(
			const CHAR *body_name
		)	
	{
		for ( INT index = 0; index < _max_body; index++ )
		{
			
			if ( _body_handle[index] != 0 && strcmp(body_name, _body_name[index]) == 0 )
			{
				return _body_height[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT GetBodyId(
			CHAR  *body_name
		)
	{
		for ( INT index = 0; index < _max_body; index++ )
		{
			if ( _body_handle[index] != 0 && strcmp(body_name, _body_name[index]) == 0 )
			{
				return _body_id[index];
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT GetBodyIndex(
			const UINT body_id
		)
	{
		for (INT index = 0; index < _max_body; index++)
		{
			if ( _body_handle[index] != 0 && body_id == _body_id[index] )
			{
				return index;
			}
		}
		return VS_ERROR;
	}


	// ----------------------------------------------------------------------
	INT GetBodyName(
			const INT body_handle,
			CHAR *body_name
		)
	{
		for (INT index = 0; index < _max_body; index++)
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
	INT GetHeadHandle()
	{

		return _head_handle;
	}


	// ----------------------------------------------------------------------
	FLOAT GetHeadHeight()
	{

		return _head_height;
	}


	// ----------------------------------------------------------------------
	INT GetHeadName(
			CHAR *head_name
		)
	{
		strcpy(head_name, _head_name);
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT GetSelectedBodyId()
	{
		return _selected_body_id;
	}


	// ----------------------------------------------------------------------
	INT GetTabOffset(
			FLOAT *x,
			FLOAT *y
		)
	{
		*x = _x_tab_off;
		*y = _y_tab_off;
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT SelectBody(
			const INT body_id
		)
	{
		for (INT index = 0; index < _max_body; index++)
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
	INT SetCallbackGetBuildInfo(
		FLOAT(*func)(INT)
		)
	{
		_callback_get_config_param = func;
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT SetColumns(BOOL use, UINT num)
	{
		_use_columns = use;
		_num_columns = num;
		return VS_OK;
	}


	// ----------------------------------------------------------------------
	INT SetSelectedBody(
			const UINT id
		)
	{
		for ( INT index = 0; index < _max_body; index++ )
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
	INT SetTabOffset(
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

	INT AddEnclose(
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

				FLOAT lal = (FLOAT)_inj_obj_config->DBZ_TEXT_CAPTION_ALIGNED_LEFT;

				// ---- add ----
				INT handle_elm = _inj_gfx_interface->AddElement(
						handle,
						"",
						0,
						0,
						&_inj_obj_config->Dbz_Callback_Configure_Text
					);

				vs_system::ElmContext *elm = NULL;
				result = _inj_gfx_interface->GetElementContext(handle_elm, &elm);

				// ---- set elm ----
				elm->SetText(label);
				elm->SetRGBA(0.15f, 0.3f, 1.0f, 1.0f);

				// ---- set db ----
				FLOAT *p = elm->GetFloatParameter();
				result = _inj_obj_config->Dbz_SetDefault(p);
				result = _inj_obj_config->Dbz_SetArt(p, _inj_obj_config->DBZ_ART_LEFT);
				result = _inj_obj_config->Dbz_SetConfig(p, lal);
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

				// enclose
				INT handle_elm = _inj_gfx_interface->AddElement(
						handle,
						"",
						0,
						0,
						&_inj_obj_config->Dbz_Callback_Configure_Rect
					);

				vs_system::ElmContext *elm = NULL;
				result = _inj_gfx_interface->GetElementContext(handle_elm, &elm);


				// set elm param
				// FIX - was elm->SetRGBA(1, 1, 1, 0.25f);
				elm->SetRGBA(0.15f, 0.3f, 1.0f, 1.0f);


				// set db param
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


private:


	// ---- injected
		vs_system::GfxInterface *_inj_gfx_interface = NULL;
		vs_system::ObjConfig    *_inj_obj_config    = NULL;


	// ---- tab
		FLOAT _x_tab_off = 0, _y_tab_off = 0;


	// ---- max
		static const INT _max_body       = 8;
		static const INT _max_body_elem  = 256;
		static const INT _max_body_items = _max_body_elem / 4;


	// --- selected
		UINT  _selected_body_id = 0;


	// ---- columns
		BOOL _use_columns = 0;
		UINT _num_columns = 3;


	// ---- head
		INT   _head_handle = 0;
		FLOAT _head_height = 0;
		CHAR  _head_name[VS_MAXCHARNAME];


	// ---- body
		INT   _body_handle         [_max_body];
		INT   _body_handle_ass_obj [_max_body];
		INT   _body_handle_elem    [_max_body][_max_body_elem];
		CHAR  _body_name           [_max_body][VS_MAXCHARNAME];
		UINT  _body_id             [_max_body];
		FLOAT _body_height         [_max_body];
		UINT  _body_total_items    [_max_body];


	// ---- colour
		FLOAT _r_min = 0.10f, _g_min = 0.30f, _b_min = 1.0f;
		FLOAT _r_max = 0.05f, _g_max = 0.15f, _b_max = 0.5f;


	// ---- callback
		FLOAT (*_callback_get_config_param)(INT);
		TintCallback _callback_for_body[_max_body];

};
