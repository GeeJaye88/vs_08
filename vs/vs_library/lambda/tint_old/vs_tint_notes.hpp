////////////////////////////////////////////////////////////////////////////////

// ---------- vs_tint_notes.hpp ----------
/*!
\file vs_tint_notes.hpp
\brief 
\author Gareth Edwards
*/


/*

1. TDI

	In interface design, a tabbed document interface (TDI) or Tab is a graphical
	control element that allows multiple documents or panels to be contained within
	a single window, using tabs as a navigational widget for switching between sets
	of documents.

2. Tint.

	Is an abbreviation of Tabbed Interface version Z; the component parts of the VS TDI.

3. Create, allocate and initialise. e.g:

	pi_model->tint = new Tint();

4. Setup, e.g:

	example - code required

5. Callbacks:

	Every DM requires 5 create callback methods, and a method to return
	these to a "setup" process, e.g:

		INT Tint_Acquire_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Acquire_BuildList(TintItem []);
		static INT Tint_Acquire_GetIconInfo(INT);
		static INT Tint_Acquire_UpdateModel(INT, INT, BOOL);
		static INT Tint_Acquire_UpdateText(INT, INT);
		static INT Tint_Acquire_AddValue(INT, INT, BOOL *, FLOAT *);

	These callbacks are returned using a TintCallback struct, e.g.

		struct TintCallback
		{
			INT(*callback_build_list)   (TintItem[]);
			INT(*callback_get_icon_info)(INT);
			INT(*callback_update_model) (INT, INT);
			INT(*callback_update_text)  (INT, INT);
			INT(*callback_format_text)  (INT, INT, BOOL *, FLOAT *);
		};

	And, e.g:

		INT Model::Tint_GainMas_GetCallbacks(vs_system::TintCallback *tint_callback)
		{
			tint_callback->callback_build_list    = Tint_GainMas_BuildList;
			tint_callback->callback_get_icon_info = Tint_GainMas_GetIconInfo;
			tint_callback->callback_update_model  = Tint_GainMas_UpdateModel;
			tint_callback->callback_add_actions   = NULL;
			tint_callback->callback_update_text   = Tint_GainMas_UpdateText;
			tint_callback->callback_format_text   = Tint_GainMas_AddValue;
			return VS_OK;
		}

6. Callback methods.

	A DM is created from a list of DM items, e.g:

		INT Model::Tint_GainMas_BuildList(
				TintItem list_of_items[]
			)
		{
			.
			.
			.
			.
			return number_of_items;
		}


	A DM item is initialised using a temporary TintItem, e.g.

		TintItem temp_list_of_items[] =
		{
			{
				"High Speed",
				ELEM_GROUP_COLOUR_LID,
				vs_system::CamProps::FPS_HIGH,
				Tint::ITEM_BUTTON,
				icon_row_base + 7
			},
			.
			.
			.
			.
		}

	And then copied into the TintItem list_of_items, e.g:

		INT number_of_items = sizeof(temp_list_of_items) / sizeof(TintItem);
		for (INT i = 0; i < number_of_items; i++) list_of_items[i] = temp_list_of_items[i];


	Using the TintItem struct, e.g:

		struct TintItem
		{
			CHAR text[32];
			UINT group_id;            // group logical id
			UINT id;                  // element logical id
			UINT type;                //
			UINT icon;                // row * # icon (per row) + col
			UINT status;              // 0/1
		};



*/