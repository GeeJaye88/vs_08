////////////////////////////////////////////////////////////////////////////////

// ---------- kpad_1_callback.hpp ----------
/*!
\file kpad_1_callback.hpp
\brief
\author Gareth Edwards
*/

//#define KPAD_OUTPUT_DEBUG_STRING

// -------- Functionality and Parameters for Tabbed Keypad Input --------

class Model::Kpad_Input_Param
{

public:

	// ---- enum's

	// ---- cdtor
		Kpad_Input_Param::Kpad_Input_Param()
		{
			;
		}
		Kpad_Input_Param::~Kpad_Input_Param()
		{
			;
		}


	// ---- element context
		static const INT num_elements = 24;
		BOOL init_dspl_elm_context_list = TRUE;
		INT  element_id_index[num_elements];
		vs_system::ElmContext *element_id_context[num_elements];

	// ---- codex
		CHAR element_lid_char[num_elements] =
		{
			'G', 'N', 'V',
			'C', 'R', 'S',
			'>', 'c', 'x',
			'7', '8', '9',
			'4', '5', '6',
			'1', '2', '3',
			's', '0', '.',
			'-', '*', '+'
		};
		CHAR group_lid_char[num_elements] =
		{
			'T', 'T', 'T',
			'M', 'M', 'M',
			'C', 'C', 'C',
			'N', 'N', 'N',
			'N', 'N', 'N',
			'N', 'N', 'N',
			'N', 'N', 'N',
			'I', 'I', 'I'
		};

} kpad_input_param;



// -------- Keypad Input for a Float * --------

class Model::Kpad_Input_Float
{

	// ---- enum
		private:
			enum
			{
				CHAR_GROUP_ID = 1,
				CHAR_NAME_ID  = 2,
				CHAR_MAX_LEN  = 32
			};


	// ---- cdtor
		public:
			Kpad_Input_Float::Kpad_Input_Float()
			{
				strcpy(_group, "Group not Set");
				strcpy(_name,  "Name not Set");
				strcpy(_error, "Error");
				strcpy(_input, "0");
				_value = NULL;
			}
			Kpad_Input_Float::~Kpad_Input_Float()
			{
				;
			}


	// ---- STUFF

		public:

			CHAR *GetGroup()      { return _group; }
			CHAR *GetName()       { return _name;  }
			CHAR *GetValue()      { return _input; }
			FLOAT GetFloatValue() { return _value; }
			FLOAT GetIncrement()  { return _increment; }
			BOOL  GetIncrMultplier() { return _incr_x10; };

			INT   GetParamGroup()      { return _param_group;       }
			INT   GetParamGroupIndex() { return _param_group_index; }
			INT   GetParamArrayIndex() { return _param_array_index; }
			INT   GetHandleModel()     { return _handle_model;      }

			INT SetGroup(CHAR *group)  { return SetText(CHAR_GROUP_ID, group); }
			INT SetName(CHAR *name)    { return SetText(CHAR_NAME_ID, name);   }
			INT SetValue(FLOAT value)  { return SetFloat(value); }
			INT SetIncrement(FLOAT increment) { _increment = increment; return VS_OK; }
			INT SetIncrMultplier(BOOL incr_x10) { _incr_x10 = _incr_x10; return VS_OK; }

			INT SetParamGroup(INT param_group)      { _param_group       = param_group;  return VS_OK; }
			INT SetParamGroupIndex(INT group_index) { _param_group_index = group_index;  return VS_OK; }
			INT SetParamArrayIndex(INT param_index) { _param_array_index = param_index;  return VS_OK; }
			INT SetHandleModel(INT handle_model)    { _handle_model      = handle_model; return VS_OK; }

		private:
			INT SetText(UINT id, CHAR *text)
			{

				#if defined(KPAD_OUTPUT_DEBUG_STRING)
				OutputDebugString("Kpad: SetText\n");
				#endif

				if ( text == NULL ) return VS_ERROR;
				INT len = strlen(text);
				len = len >= CHAR_MAX_LEN ? CHAR_MAX_LEN - 1 : len;
				CHAR *dest = id == CHAR_GROUP_ID ? _group :
						(id == CHAR_NAME_ID ? _name : _error);
				memcpy((VOID *)dest, (VOID *)text, (size_t)len);
				*(dest + len) = 0;
				return VS_OK;
			}

		INT SetFloat(FLOAT v)
			{

				#if defined(KPAD_OUTPUT_DEBUG_STRING)
				OutputDebugString("Kpad: SetFloat\n");
				#endif

				_value = v;
				sprintf_s(_input, CHAR_MAX_LEN, "%f", _value);
				std::string s = tostring(_input);
				strcpy(_input, s.c_str());

				INT len = strlen(_input);
				if (len > 1 && len < CHAR_MAX_LEN-1 && _input[len + 1] == '.')
				{
					_input[len] = '0';
					_input[len+1] = 0;
				}
				return VS_OK;
			}

		INT Tidy(CHAR *c)
		{

			#if defined(KPAD_OUTPUT_DEBUG_STRING)
			OutputDebugString("Kpad: Tidy\n");
			#endif

			INT len = strlen(_input);
			INT dec_point_pos = 0;
			while (c[dec_point_pos] != '.' && dec_point_pos + 1 < len) dec_point_pos++;
			if ( len != dec_point_pos ) return VS_OK;

			/*
			INT toplen = len - 1;
			CHAR last_char = c[toplen];
			while (INT i = toplen; i > 1; i--)
			{
				if ( c[])
			}
			if (len > 1 && len < CHAR_MAX_LEN - 1 && _input[len + 1] == '.')
			{
				_input[len] = '0';
				_input[len + 1] = 0;
			}
			*/
			return VS_OK;
		}


	// ---- FLOAT
		public:

			INT Update(CHAR group, CHAR action)
			{

				switch (group)
				{

					case 'I':
						{

							switch (action)
							{
								case '-':
									// .... catch later in main class
									break;
								case '*':
									_incr_x10 = _incr_x10 ? FALSE : TRUE;
									break;
								case '+':
									// .... catch later in main class
									break;
								default: break;
							}
						}
						break;

					case 'M':
						{
							switch (action)
							{
								case 'C':
									strcpy(_store, "");
									break;
								case 'R':
									{
										std::string s = tostring(_store);
										strcpy(_input, s.c_str());
									}
									break;
								case 'S':
									strcpy(_store, _input);
									break;
								default: break;
							}
						}
						break;

					case 'C':
						{
							switch (action)
							{
								case '>':
									// .... catch later in main class
									break;
								case 'c':
									{
										strcpy(_input, "0");
										_value = 0;
									}
									break;
								case 'x':
									{
										INT len = strlen(_input);
										if ( len == 1 )
											strcpy(_input, "0");
										else
											*(_input + len -1) = 0;
									}
									break;
								default: break;
							}
						}
						break;

					case 'N':
						{
							switch (action)
							{
								case 's':
									{
										_value = (FLOAT)atof(_input);
										_value *= -1;
										sprintf_s(_input, CHAR_MAX_LEN, "%f", _value);
										std::string s = tostring(_input);
										strcpy(_input, s.c_str());
									}
									break;
								case '.':
									{
										BOOL point = FALSE;
										INT len = strlen(_input);
										for (INT i = 0; i < len; i++)
										{
											if ( *(_input + i) == '.' )
											{
												point = TRUE;
												break;
											}
										}
										if ( !point ) strcat_s(_input, ".");
									}
									break;
								default:
									if ( action >= '0' && action <= '9' )
									{
										INT len = strlen(_input);
										if ( (_input[0] == '0' ) && (len == 1) )
										{
											len = 0;
										}
										else
										{
											len = len >= CHAR_MAX_LEN ? CHAR_MAX_LEN - 1 : len;
										}
										*(_input + len++) = action;
										*(_input + len) = 0;
									}
									break;
							}
						}
						break;

					default:
						break;
				}

				_value = (FLOAT)atof(_input);

				/*
				CHAR ods[128];
				sprintf(ods, " -+- %c %c : %s\n", group, action, _input);
				OutputDebugString(ods);
				*/

				#if defined(KPAD_OUTPUT_DEBUG_STRING)
				OutputDebugString("Kpad: Update\n");
				#endif

				return VS_OK;
			};


	// ---- properties
		private:

			// ---- displayed text
				CHAR   _group[CHAR_MAX_LEN]  = "";
				CHAR   _name[CHAR_MAX_LEN]   = "";
				CHAR   _error[CHAR_MAX_LEN]  = "";
				CHAR   _input[CHAR_MAX_LEN]  = "";
				CHAR   _store[CHAR_MAX_LEN]  = "";

			// ---- value
				FLOAT  _value  = 0;
				FLOAT _increment = 0;
				BOOL  _incr_x10 = FALSE;

			// ---- param group
				INT    _param_group = 0;
				INT    _param_group_index = 0;
				INT    _param_array_index = 0;
				INT    _handle_model = 0;

} kpad_input_float;


// ----------------------------------------------------------------------
INT Model::Kpad_Input_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Kpad_Input_BuildList;
	tint_callback->callback_get_icon_info  = Kpad_Input_GetIconInfo;
	tint_callback->callback_update_model   = Kpad_Input_UpdateModel;
	tint_callback->callback_update_element = Kpad_Input_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Kpad_Input_AddText;
	tint_callback->callback_add_value      = Kpad_Input_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Kpad_Input_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr = 10;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = KPAD_BODY_INPUT_LID;
		UINT noi = 0;


	// ---- build
		{

			// ---- scope tint def & enu
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- 
				TINT_ADD_ITEM(kpad_input_float.GetGroup())
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   5, 0, 0, 0, TRUE, 2)
				TINT_ADD_ITEM(kpad_input_float.GetName())
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   5, 0, 0, 0, TRUE, 2)
				TINT_ADD_ITEM(kpad_input_float.GetValue())
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   5, 0, 0, 0, TRUE, 2)

				// mc mr ms
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_CLICK,     5, 1, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 1, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXL(Tint::ITEM_CLICK,   5, 1, 2, 2.2f, TRUE)

				// > c x
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_CLICK,     5, 2, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 2, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXL(Tint::ITEM_CLICK,   5, 2, 2, 2.2f, TRUE)

				// 7 8 9
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_CLICK,     5, 3, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 3, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXL(Tint::ITEM_CLICK,   5, 3, 2, 2.2f, TRUE)

				// 4 5 6
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_CLICK,     5, 4, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 4, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXL(Tint::ITEM_CLICK,   5, 4, 2, 2.2f, TRUE)

				// 1 2 3
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_CLICK,     5, 5, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 5, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXL(Tint::ITEM_CLICK,   5, 5, 2, 2.2f, TRUE)

				// > 0 .
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_CLICK,     5, 6, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 6, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK,  5, 6, 2, 2.2f, TRUE, 8)

				// + d -
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_CLICK,     4, 5, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_SWITCH,   4, 5, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK,  4, 5, 2, 2.2f, TRUE, 12)

			// ---- set total number of items
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item id's
			for (UINT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)kpad_input_param.element_lid_char[i];
			}

	return noi;
}


// ----------------------------------------------------------------------
INT Model::Kpad_Input_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 0;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Kpad_Input_UpdateModel(
		INT  handle_model,
		INT  element_index
	)
{

	#if defined(KPAD_OUTPUT_DEBUG_STRING)
	OutputDebugString("Kpad_Input_UpdateModel\n");
	#endif

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- unpack element
		result = Kpad_Update_SetElementList(handle_model, 0);


	// ---- process

		UINT number_of_element_ids = sizeof(kpad_input_param.element_lid_char) / sizeof(CHAR);
		for ( UINT i = 0; i < number_of_element_ids; i++)
		{
			if ( kpad_input_param.element_lid_char[i] == (CHAR)element_index )
			{
				// ---- increment multiplier
					BOOL  incr_double = kpad_input_float.GetIncrMultplier();
					FLOAT incr_mult  = incr_double ? 10.0f : 1.0f;

				CHAR group = kpad_input_param.group_lid_char[i];
				CHAR action = (CHAR)element_index;
				kpad_input_float.Update(group, action);

				BOOL set_parameter = FALSE;
				if ( action == '>' )
				{
					set_parameter = TRUE;
				}
				else if ( action == '-' )
				{
					FLOAT value = kpad_input_float.GetFloatValue();
					value -= incr_mult * kpad_input_float.GetIncrement();
					kpad_input_float.SetValue(value);
					Kpad_SetParameter(handle_model);
					set_parameter = TRUE;
				}
				else if ( action == '+' )
				{
					FLOAT value = kpad_input_float.GetFloatValue();
					value += incr_mult * kpad_input_float.GetIncrement();
					kpad_input_float.SetValue(value);
					set_parameter = TRUE;
				}

				if ( set_parameter )
				{
					Kpad_SetParameter(handle_model);
				}

			}
		}

		kpad_input_param.element_id_context[2]->SetText(kpad_input_float.GetValue());

	return VS_ELM_SETUP_NONE;
}


// ----------------------------------------------------------------------
INT  Model::Kpad_Input_UpdateElement(
		INT handle_model,
		INT element_index 
	)
{

	#if defined(KPAD_OUTPUT_DEBUG_STRING)
	OutputDebugString("Kpad_Input_UpdateElement\n");
	#endif

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- unpack element
		result = Kpad_Update_SetElementList(handle_model, 0);

		if ( element_index == (INT)'P' )
		{
			kpad_input_param.element_id_context[0]->SetText(kpad_input_float.GetGroup());
			kpad_input_param.element_id_context[1]->SetText(kpad_input_float.GetName());
			kpad_input_param.element_id_context[2]->SetText(kpad_input_float.GetValue());
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Kpad_Input_AddText(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	kpad_input_float.SetHandleModel(handle_obj_config);

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Kpad_Input_AddValue(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	*add_value = FALSE;
	*value     = 0;
	sprintf(suffix, "2");

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Kpad_Update_SetElementList(
		INT  handle_model,
		INT  nut_used
	)
{

	// ---- ONLY if NOT "initialised"
		if ( kpad_input_param.init_dspl_elm_context_list )
		{

			#if defined(KPAD_OUTPUT_DEBUG_STRING)
			OutputDebugString("Kpad_Update_SetElementList\n");
			#endif

			// ---- local
				INT   result = VS_OK;
				Model *model = (Model *)handle_model;
				vs_system::GfxInterface *gfx_interface = s_pi_model->app_dependency->GetGfxInterface();


			// ---- get # of elements
				UINT elem_count = s_pi_model->kpad_setng->GetBodyDsplElmCount(KPAD_BODY_INPUT_LID);


			// ---- get element handles
				INT  elem_handles[s_pi_model->kpad_setng->MAX_BODY_ELM];
				result = s_pi_model->kpad_setng->GetBodyDsplElmHandles(KPAD_BODY_INPUT_LID, elem_handles);


			// ---- get element contexts
				vs_system::ElmContext *element_context[s_pi_model->tint_setng->MAX_BODY_ELM];
				result = gfx_interface->GetElementContexts(
						elem_count,
						elem_handles,
						element_context
					);


			// ---- set ONLY first instance of matched element id contexts
				UINT number_of_element_ids = sizeof(kpad_input_param.element_lid_char) / sizeof(CHAR);
				UINT last_id = 0;
				for ( UINT e = 0; e<elem_count; e++ )
				{
					UINT gid = element_context[e]->GetGroupId();
					UINT  id = element_context[e]->GetId();

					if ( id != last_id )
					{
						for ( UINT i = 0; i < number_of_element_ids; i++ )
						{
							if ( id == kpad_input_param.element_lid_char[i] )
							{
								kpad_input_param.element_id_context[i] = element_context[e];
								kpad_input_param.element_id_index[i] = e;
								break;
							}
						}
					}

					last_id = id;
				}

			// ---- set "initialised"
				kpad_input_param.init_dspl_elm_context_list = FALSE;

		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Kpad_UpdateByParameter(
		INT handle_model,
		INT param_group
	)
{

	// ---- local
		INT result = VS_OK;
		
		
	// ---- defined ?
		if ( param_group != 0 )
		{

			#if defined(KPAD_OUTPUT_DEBUG_STRING)
			OutputDebugString("Kpad_UpdateByParameter\n");
			#endif


			// ---- select group
				result = SetParamGroup(param_group);


			// ---- param
				#include "../../surface_shared/hpp_param/param_setup.hpp"


			// ---- no change ?
				//
				// 14/11/20 - commented out to allow for s2 source param store swapping... 
				//            where the "copied" param set has the same group id, 
				//            group and array indices. 
				/*
				INT prev_param_group       = kpad_input_float.GetParamGroup();
				INT prev_param_group_index = kpad_input_float.GetParamGroupIndex();
				INT prev_param_array_index = kpad_input_float.GetParamArrayIndex();
				if (
						prev_param_group       == param_group && 
						prev_param_group_index == param_group_index &&
						prev_param_array_index == param_array_index
					)
					{
						OutputDebugString("Kpad_Get : no change\n");
						//return VS_OK;
					}
				*/

			// ---- string to char
				CHAR char_mesh_param_group_name[VS_MAXCHARNAME];
				strcpy(char_mesh_param_group_name, param_group_name.c_str());
				CHAR char_mesh_param_array_label[VS_MAXCHARNAME];
				strcpy(char_mesh_param_array_label, param_array_labels[param_array_index].c_str());

				/*
				CHAR msg[128];
				sprintf(
					msg,
					"Kpad_Get : %d - %d %d - %s - %s - %f \n",
					param_group,
					GetParamGroupIndex(),
					GetParamArrayIndex(),
					char_mesh_param_group_name,
					char_mesh_param_array_label, 
					param_array_values[param_array_index]
				);
				OutputDebugString(msg);
				*/

			// ---- set kpad
				kpad_input_float.SetGroup(char_mesh_param_group_name);
				kpad_input_float.SetName(char_mesh_param_array_label);
				kpad_input_float.SetValue(param_array_values[param_array_index]);
				kpad_input_float.SetIncrement(param_array_increments[param_array_index]);
				kpad_input_float.SetParamGroup(param_group);
				kpad_input_float.SetParamGroupIndex(param_group_index);
				kpad_input_float.SetParamArrayIndex(param_array_index);

				INT handle_model = kpad_input_float.GetHandleModel();
				if ( handle_model != 0 )
					result = Kpad_Input_UpdateElement( handle_model, 'P');

			// ---- param
				#include "../../surface_shared/hpp_param/param_cleanup.hpp"

		}
		else
		{
				kpad_input_float.SetGroup("KeyPad");
				kpad_input_float.SetName("Select Parameter");
				kpad_input_float.SetValue(0.0);
				kpad_input_float.SetParamGroup(Model::PARAM_GROUP_NULL);
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Kpad_UpdateByMessage(INT handle_model, INT msg)
{

	#if defined(KPAD_OUTPUT_DEBUG_STRING)
	OutputDebugString("Kpad_UpdateByMessage\n");
	#endif

	// ---- open/visible ?
		BOOL open = s_pi_model->GetKeypadStatus();
		//if ( !open ) return VS_OK;


	// ---- only '[' (minus) and ']' (plus)
		switch ( msg )
		{
			case '[':
			case ']':
				break;
			default:
				return VS_OK;
		}


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- param
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- update sign of increment
		FLOAT incr = (FLOAT)(msg == '[' ? -1 : 1);


	// ---- update kpad value
		FLOAT value = kpad_input_float.GetFloatValue();
		value += incr * kpad_input_float.GetIncrement();
		kpad_input_float.SetValue(value);


	// ---- update kpad text
		result = Kpad_Input_UpdateElement(handle_model, 'P');


	// ---- update current tint parameter value
		result = Kpad_SetParameter(handle_model);


	// ---- mesh param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}
