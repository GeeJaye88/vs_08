////////////////////////////////////////////////////////////////////////////////

// ---------- s2_model_kamera.hpp ----------
/*!
\file s2_model_kamera.hpp
\brief Implementation of the Kamera methods
\author Gareth Edwards
*/


// ========================================================================== //

INT Model::SetMyKamera(INT camera_index)
{

		vs_system::AppKamera *kamera = pi_model->app_kamera[0];
		VsMatrix V;
		kamera->GetViewMatrix(&V);
		pi_model->gfx->SetViewMatrix(1, &V);

	return VS_OK;
}


// ========================================================================== //
/*!

\note
	- invoked from:
		-> [surface_02]::application::Display()
			-> [surface_02]::Kontrol::Update()
				-> AppKontrol::InvokeInputCallbacks()
					-> AppKontrol::PI_AppKontrol::InvokeInputCallbacks()
						-> AppKeyboard::InvokeInputCallbacks()
							-> AppKeyboard::PI_AppKeyboard::InvokeInputCallbacks()
								-> [surface_02]::Kontrol::CallbackByKeyboard()

\note - return value is ignored!

\note - SetCameraControlMessage(0) should not be invoked !!!

*/
INT Model::SetMsgProc(
	INT msg,
	FLOAT delta
)
{

	// ---- local
		INT result = VS_OK;


	// ---- process
		INT uc_msg = toupper(msg);
		switch ( uc_msg )
		{
			// ---- layout
				case 'V':
				case 'B':
				case 'N':
				case 'M':
					result = SetMsgLayout(uc_msg);
					break;

			// ---- keypad
				case '[': // minus
				case ']': // plus
					result = SetMsgKeyPad(uc_msg, delta);
					break;

			// ---- system
				case 'T': // texture
					result = SetMsgSystem(uc_msg, delta);
					break;

			// ---- camera
				default:
					result = SetMsgCameraControl(uc_msg, delta, FALSE);
					break;
		}

	return result;
}


// ========================================================================== //


INT Model::SetMsgLayout(
	INT   msg
)
{
		switch ( msg )
		{

			case 'V':
				{
					SwapPanelsLayoutAlignedSide();
				}
				return VS_OK;

			case 'B':
				{
					pi_model->panels_visible = PI_Model::MAIN_COMPLETE;
					SetPanelsLayoutUpdate();
				}
				return VS_OK;

			case 'N':
				{
					pi_model->panels_visible = PI_Model::TINT_COMPLETE;
					SetPanelsLayoutUpdate();
				}
				return VS_OK;

			case 'M':
				{
					pi_model->panels_visible = PI_Model::KEYPAD_COMPLETE;
					SetPanelsLayoutUpdate();
				}
				return VS_OK;

			default:
				break;
		}

	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_ERROR;
}


// ========================================================================== //


INT Model::SetMsgKeyPad(
		INT msg,
		FLOAT delta
	)
{

		switch ( msg )
		{
			case '[':
			case ']':
				{
					INT result = Kpad_UpdateByMessage((INT)this, msg);
				}
				return VS_OK;

			default:
				break;
		}

	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_ERROR;
}


// ========================================================================== //


INT Model::SetMsgSystem(
		INT msg,
		FLOAT delta
	)
{

		switch ( msg )
		{

			case 'T':
				{
					BOOL textures = pi_model->sys_context->GetBool(vs_system::SysContext::TEXTURES);
						textures = textures ? FALSE : TRUE;
					INT result = pi_model->sys_context->SetBool(
								vs_system::SysContext::TEXTURES,
								textures
							);
				}
				return VS_OK;

			default:
				break;
		}


	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_ERROR;
}


// ========================================================================== //


INT Model::SetMsgCameraPersistantControl()
{

	// ---- zero "persistant same message" counter
		SetMsgCameraControlCount(0);


	// ---- on ?
		UINT switched_on = GetMsgCameraControlPersistantOn();
		if ( switched_on )
		{

			// ---- persist ?
				BOOL active = GetMsgCameraControlPersistantActive();
				if ( active )
				{

					// ---- with what ?
						UINT control_message = GetCameraControlMessage();
						vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
						FLOAT delta = (FLOAT)sys_context->GetDouble(vs_system::SysContext::TIME_MS_DELTA);
						
					// ---- process
						SetMsgCameraControl(control_message, delta, TRUE);

				}

		}

	return VS_OK;
}


// ========================================================================== //


INT Model::SetMsgCameraControl(
		INT   message,
		FLOAT delta,
		BOOL  persistant
	)
{

	// ---- increment "persistant same message" counter
		IncMsgCameraControlCount();


	// ---- local
		vs_system::AppKamera *kamera = pi_model->app_kamera[0];


	// ---- mouse wheel ? -- swap for zoom msg, and adjust delta
		if ( message == '{' || message == '}' )
		{
			UINT target_planet_index = pi_model->GetTargetObjectId();
			if ( target_planet_index == CameraTarget::NONE )
			{
				message = message == '{' ? 'W' : message == '}' ? 'S' : message;
			}
			else
			{
				message = message == '{' ? 'W' : message == '}' ? 'S' : message;
				delta = message == 'W' || message == 'S' ? delta : delta;
			}
		}


	// ----- catch target object index msg
		INT message_adj_for_target = message;
		if ( message > '1' && message <= '7' )
		{
			message_adj_for_target = '1';
		}


		using namespace vs_system;


	// ---- process
		switch ( message_adj_for_target )
		{

			// ----- look at target"1234567"
				//
				case '1':
					{
						UINT tid = message - '1';
						pi_model->SetTargetObjectId(tid);
						pi_model->SetCameraControlMode(AppKamera::Mode::TARGET);
						pi_model->surface_02->SetTargetLoiIndex(tid);
					}
					break;

			// ---- persistant
				case 'Q':
				case 'E':
					{
						UINT switched_on = GetMsgCameraControlPersistantOn();
						switched_on = switched_on == TRUE ? FALSE : TRUE;
						SetMsgCameraControlPersistantOn(switched_on);  // PHI
						SetMsgCameraControlPersistantActive(FALSE);    // KAPPA
					}
					break;

			// ---- camera control mode
				case 'G':
					{
						INT mode = pi_model->GetCameraControlMode() + 1;
						mode = mode > AppKamera::Mode::ROTATE ?
							AppKamera::Mode::XYZ : mode;
						pi_model->SetCameraControlMode(mode);
						pi_model->SetTargetObjectId(CameraTarget::NONE);
						pi_model->surface_02->SetTargetLoiIndex(CameraTarget::NONE);
						kamera->SetTargetStatus(FALSE);
					}
					break;

			// ---- mesh display mode
				case 'Z':
					{
						model::Surface_02 *surface_02 = s_pi_model->surface_02;

						UINT mdm = surface_02->GetMeshDisplayMode() + 1;
						mdm = mdm > 3 ? 0 : mdm;
						surface_02->SetMeshDisplayMode(mdm);

						BOOL solid    = surface_02->GetMeshSolid();
						BOOL textured = surface_02->GetMeshTextured();
						switch (mdm)
						{
							case 0: solid = 0, textured = 0; break;
							case 1: solid = 1, textured = 0; break;
							case 2: solid = 0, textured = 1; break;
							case 3: solid = 1, textured = 1; break;
						}
						
						surface_02->SetMeshSolid(solid);
						surface_02->SetMeshTextured(textured);

						surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);
					}
					break;

			// ---- reset NOT position (RNP)
				case '&':
					{
						kamera->SetTargetZoomDefault();
						kamera->SetPanTiltDefault();
						kamera->SetDefaultRotate();
						kamera->SetDefaultTranslate();
					}
					break;

			// ---- reset all (RAL)
				case 'X':
					{
						kamera->SetTargetZoomDefault();
						kamera->SetPanTiltDefault();
						kamera->SetPositionDefault();
						kamera->SetDefaultRotate();
						kamera->SetDefaultTranslate();
						SetMsgCameraControlPersistantOn(FALSE);     // PHI
						SetMsgCameraControlPersistantActive(FALSE); // KAPPA
						INT result = Tint_Explore_UpdateControlByMsg((INT)this, 0);
					}
					break;

			// ---- mesh density mode
				case 'C':
					{

						// ----- note: also see s2_model_kamera.hpp 

							model::Surface_02 *surface_02 = s_pi_model->surface_02;

						// ---- update model
							UINT mode = surface_02->GetMeshDensityMode() + 1;
							mode = mode > surface_02->HIGH ? surface_02->LOW : mode;
							surface_02->SetMeshDensityMode(mode);

						// ---- update surface
							surface_02->SetMeshDensity(mode + 1);
							surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);
					}
					break;

			// ---- stuff
				case 'W':
				case 'S':
				case 'A':
				case 'D':
				case 'R':
				case 'F':
				case 'J':
				case 'K':
				case 'L':
					{

						// ---- NOT persistant ?
							//
							// note: NOT invoked from tint model update (e.g. Tint_Explore_UpdateModel)
							//
							if ( !persistant )
							{

								// ---- selected ?
									BOOL switched_on = GetMsgCameraControlPersistantOn();
									if ( switched_on )
									{

											BOOL old_control_message = GetCameraControlMessage();
											if ( old_control_message == message )
											{

												BOOL active = GetMsgCameraControlPersistantActive();
												active = active == TRUE ? FALSE : TRUE;
												SetMsgCameraControlPersistantActive(active); // KAPPA
												if ( !active )
												{
													SetCameraControlMessage(message);
													return VS_OK;
												}
											}
											else
											{
												SetMsgCameraControlPersistantActive(TRUE); // KAPPA
											}

									}
							}


						// ---- prevent "persistant same message" increments
							if ( GetMsgCameraControlCount() < 2 )
							{

								// ---- control
									UINT kamera_control_mode = pi_model->GetCameraControlMode();
									switch ( kamera_control_mode )
									{

										case AppKamera::Mode::XYZ :
											switch ( message )
											{
												case 'W': kamera->IncPosOrd(AppKamera::Ordinate::Z, delta); break;
												case 'S': kamera->IncPosOrd(AppKamera::Ordinate::Z,-delta); break;
												case 'A': kamera->IncPosOrd(AppKamera::Ordinate::X,-delta); break;
												case 'D': kamera->IncPosOrd(AppKamera::Ordinate::X, delta); break;
												case 'R': kamera->IncPosOrd(AppKamera::Ordinate::Y, delta); break;
												case 'F': kamera->IncPosOrd(AppKamera::Ordinate::Y,-delta); break;
											}
											break;

										case AppKamera::Mode::VECTOR :
											switch ( message )
											{
												case 'R': kamera->IncPosDir(AppKamera::Direction::UP,    delta); break;
												case 'F': kamera->IncPosDir(AppKamera::Direction::UP,   -delta); break;
												case 'A': kamera->IncPosDir(AppKamera::Direction::RIGHT,-delta); break;
												case 'D': kamera->IncPosDir(AppKamera::Direction::RIGHT, delta); break;
												case 'W': kamera->IncPosDir(AppKamera::Direction::LOOK,  delta); break;
												case 'S': kamera->IncPosDir(AppKamera::Direction::LOOK, -delta); break;
											}
											break;
						
										case AppKamera::Mode::PTZ :
											switch ( message )
											{
												case 'R': kamera->IncTilt(-delta); break;
												case 'F': kamera->IncTilt( delta); break;
												case 'A': kamera->IncPan(-delta);  break;
												case 'D': kamera->IncPan( delta);  break;
												case 'W': kamera->IncPosDir(AppKamera::Direction::LOOK,  delta); break;
												case 'S': kamera->IncPosDir(AppKamera::Direction::LOOK, -delta); break;
											}
											break;

										case AppKamera::Mode::TRANSLATE:
											switch ( message )
											{
												case 'W': kamera->IncTranslate(AppKamera::Ordinate::Z, delta); break;
												case 'S': kamera->IncTranslate(AppKamera::Ordinate::Z,-delta); break;
												case 'A': kamera->IncTranslate(AppKamera::Ordinate::X,-delta); break;
												case 'D': kamera->IncTranslate(AppKamera::Ordinate::X, delta); break;
												case 'R': kamera->IncTranslate(AppKamera::Ordinate::Y, delta); break;
												case 'F': kamera->IncTranslate(AppKamera::Ordinate::Y,-delta); break;
											}
											break;

										case AppKamera::Mode::ROTATE:
											switch ( message )
											{
												case 'W': kamera->IncRotate(AppKamera::Ordinate::X, delta); break;
												case 'S': kamera->IncRotate(AppKamera::Ordinate::X,-delta); break;
												case 'A': kamera->IncRotate(AppKamera::Ordinate::Y,-delta); break;
												case 'D': kamera->IncRotate(AppKamera::Ordinate::Y, delta); break;
												case 'R': kamera->IncRotate(AppKamera::Ordinate::Z, delta); break;
												case 'F': kamera->IncRotate(AppKamera::Ordinate::Z,-delta); break;
											}
											break;

										case AppKamera::Mode::TARGET :
											{

												// ----target  mode ?
													model::Surface_02 *surface_02 = s_pi_model->surface_02;
													BOOL target_transform = surface_02->GetTargetLoiTranslate();
													FLOAT t_delta = delta/100, r_delta = 0;

													if ( target_transform )
													{
														BOOL high_speed = kamera->GetScalarMode();
														if ( high_speed )
														{
															t_delta = delta / 200;
															r_delta = delta / 80;
														}
														else
														{
															t_delta = delta / 600;
															r_delta = delta / 240;
														}
														t_delta /= 4;
													}

												// ---- kamera
													switch (message)
													{
														case 'R':
															{
																if ( target_transform )
																	surface_02->SetTargetLoiIncr(0, r_delta, 0);
																else
																	kamera->IncTargetTilt(-delta * 2);
															}
															break;
														case 'F':
															{
																if (target_transform)
																	surface_02->SetTargetLoiIncr(0, -r_delta, 0);
																else
																	kamera->IncTargetTilt( delta * 2);
															}
															break;
														case 'A':
															{
																if ( target_transform )
																	surface_02->SetTargetLoiIncr(-t_delta, 0, 0);
																else
																	kamera->IncTargetPan( delta * 2);
															}
															break;
														case 'D':
															{
																if ( target_transform )
																	surface_02->SetTargetLoiIncr(t_delta, 0, 0);
																else
																	kamera->IncTargetPan(-delta*2);
															}break;
														case 'W':
															{
																if ( target_transform )
																	surface_02->SetTargetLoiIncr(0, 0, t_delta);
																else
																	kamera->IncTargetZoom(-delta);
															}
															break;
														case 'S':
															{
																if ( target_transform )
																	surface_02->SetTargetLoiIncr(0, 0, -t_delta);
																else
																	kamera->IncTargetZoom(delta);
															}
															break;

														// ---- zoom away
														case 'J':
															{
																kamera->IncTargetZoom(delta / 5);
																kamera->IncTargetPan(delta);
															}
															break;

														// ---- orbit (tilt proportional to pan) around
														case 'K':
															{

																kamera->IncTargetTilt(delta);
																kamera->IncTargetPan(delta);

																FLOAT pan       = kamera->GetTargetPan();
																FLOAT advance   = 0.6f;
																FLOAT amplitude = 40.0f;
																FLOAT offset    = 15.0f;
																FLOAT radians   = (advance * pan - 90) / 360 * 360 * (FLOAT)VS_RADIANS_PER_DEGREE;
																FLOAT tilt      = sin(radians) * (amplitude / 2) + (amplitude / 2 + offset);

																kamera->SetTargetTilt(tilt);

															}
															break;

														// ---- zoom into
														case 'L':
															{
																kamera->IncTargetZoom(-delta / 5);
																kamera->IncTargetPan(delta);
															}
															break;
														default :
															break;
													}

													if ( target_transform )
													{
														surface_02->ObjectLoiSpot_Update(surface_02);
														surface_02->ObjectLoiRect_Update(surface_02);
														surface_02->ObjectLoiRule_Update(surface_02);
														surface_02->ObjectLoiAxis_Update(surface_02);
													}

											}
											break;

										default:
											break;
									}

							}

						// ---- store control message
							SetCameraControlMessage(message);

					}
					return VS_OK;


			// ---- camera increment
				case 'I':
					{
						INT result = Tint_Explore_UpdateSpeed();
					}
					break;


			// ---- other stuff
				default:
					return VS_ERROR;
		}

	return VS_OK;
}


// ========================================================================== //