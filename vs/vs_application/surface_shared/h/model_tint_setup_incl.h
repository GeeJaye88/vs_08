

	// ======== Tint - REQUIRED ========


	// ---- allocate and construct object
		pi_model->tint_setng = new vs_system::Tint();
		pi_model->kpad_setng = new vs_system::Tint();
	

	// ======== KeyPad - REQUIRED ========

	// ---- initialise
		Kpad_UpdateByParameter((INT)this, 0);


	// ======== Tint - REQUIRED ========

	// ---- setup
		result = TintSetup();
		result = KpadSetup();


	// ======== Tint - REQUIRED ========

	// ---- fonts
		struct font_info
		{
			std::string name;
			std::string type;
		};
		font_info font_list[] =
		{
			{ pi_model->tint_setng->GetBodyFontName(), "Roboto"    }, // "Tint Head"
			{ pi_model->tint_setng->GetHeadFontName(), "Roboto Lt" }, // "Tint Body"
			{ pi_model->kpad_setng->GetBodyFontName(), "Roboto"    }, // "Kpad Head"
			{ pi_model->kpad_setng->GetHeadFontName(), "Roboto Lt" }  // "Kpad Body"
		};
		INT total_fonts = sizeof(font_list) / sizeof(font_info);
		for (INT i = 0; i<total_fonts; i++)
		{
			INT font_ok = pi_model->gfx->SetFont(
					i + 1,
					font_list[i].name,
					font_list[i].type,
					20,
					1
				);
		}
