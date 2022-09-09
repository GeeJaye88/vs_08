////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_kamera.hpp ----------
/*!
\file s1_model_kamera.hpp
\brief Implementation of the Camera methods
\author Gareth Edwards
*/


// ========================================================================== //

INT Model::SetMyKamera(INT kamera_index)
{

		vs_system::AppKamera *kamera = pi_model->app_kamera[kamera_index];
		VsMatrix V;
		kamera->GetViewMatrix(&V);
		pi_model->gfx->SetViewMatrix(1, &V);

	return VS_OK;
}


// ========================================================================== //

INT Model::SetMsgProc(
	INT msg,
	FLOAT delta
)
{

	// ---- img capture ?
		INT event_id = pi_model->sys_context->GetDWord(vs_system::SysContext::WINDOW_TRIGGER);
		if ( event_id != 0 )
		{
			delta = 1;
		}


	// ---- local
		INT result = VS_OK;


	// ---- process
		INT uc_msg = toupper(msg);
		switch ( uc_msg )
		{

			// ---- time
				case 'P': // pause
				case '-': // dec
				case '=': // reset
				case '+': // inc
					result = SetMsgTime(uc_msg, delta);
					break;

			// ---- layout
				case 'V': // side
				case 'B': // main
				case 'N': // main + tint
				case 'M': // main + tint + keypad
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


INT Model::SetMsgTime(
		INT msg,
		FLOAT delta
	)
{

		switch ( msg )
		{
			// ---- pause & return without consuming message !
				case 'P':
					{
						INT pause_mode = pi_model->system_param->GetPauseMode();
						pause_mode = pause_mode ? 0 : 1;
						pi_model->system_param->SetPauseMode(pause_mode);

						DOUBLE system_time_elapsed = pi_model->sys_context->GetDouble(vs_system::SysContext::TIME_MS_ELAPSED);
						DOUBLE pause_time_elapsed = pi_model->system_param->GetPauseTimeElapsed();

						if ( pause_mode )
						{
							pi_model->system_param->SetPauseTimeStart(system_time_elapsed);
						}
						else
						{
							pause_time_elapsed += system_time_elapsed - pi_model->system_param->GetPauseTimeStart();
							pi_model->system_param->SetPauseTimeElapsed(pause_time_elapsed);
						}
					}
					return VS_OK;

			// ---- slow down / 2
				case '-':
					pi_model->system_param->SetTimeScalar(pi_model->system_param->GetTimeScalar() / 2);
					break;

			// ---- reset
				case '=':
					pi_model->system_param->SetTimeScalar(1.0f);
					break;

			// ---- speed up * 2
				case '+':
					pi_model->system_param->SetTimeScalar(pi_model->system_param->GetTimeScalar() * 2);
					break;

			// ---- what ?
				default:
					break;
		}


	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_OK;
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
				break;

			case 'B':
				{
					pi_model->panels_visible = PI_Model::MAIN_COMPLETE;
					SetPanelsLayoutUpdate();
				}
				break;

			case 'N':
				{
					pi_model->panels_visible = PI_Model::TINT_COMPLETE;
					SetPanelsLayoutUpdate();
				}
				break;

			case 'M':
				{
					pi_model->panels_visible = PI_Model::KEYPAD_COMPLETE;
					SetPanelsLayoutUpdate();
				}
				break;

			default:
				break;
		}

	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_OK;
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
				break;

			default:
				break;
		}

	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_OK;
}


// ========================================================================== //


INT Model::SetMsgSystem(
		INT msg,
		FLOAT delta
	)
{

		switch ( msg )
		{

			// ---- image - pre BlackMagic: deprecated & now not used
				case 'I':
					{
						INT event_id = pi_model->sys_context->GetDWord(vs_system::SysContext::WINDOW_TRIGGER);
						if ( event_id == 0 )
						{

							INT result = pi_model->sys_context->SetDWord(
									vs_system::SysContext::WINDOW_TRIGGER,
									pi_model->img_capture_event_id
								);

							pi_model->img_capture_elapsed_ms = pi_model->sys_context->GetDouble(
									vs_system::SysContext::TIME_MS_ELAPSED
								);

							//pi_model->system_param->StoreCameraControl();
						}
						else
						{
							INT result = pi_model->sys_context->SetDWord(
									vs_system::SysContext::WINDOW_TRIGGER,
									0
								);
						}
					}
					break;

			// ---- texture
				case 'T':
					{
						BOOL textures = pi_model->sys_context->GetBool(vs_system::SysContext::TEXTURES);
							textures = textures ? FALSE : TRUE;
						INT result = pi_model->sys_context->SetBool(
									vs_system::SysContext::TEXTURES,
									textures
								);
					}
					break;

			// ---- what ?
				default:
					break;
		}


	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_OK;
}


// ========================================================================== //


INT Model::SetMsgCameraPersistantControl()
{

	// ---- zero "persistant same message" counter
		SetMsgCameraControlCount(0);


	// ---- on?
		UINT switched_on = GetMsgCameraControlPersistantOn();
		if ( switched_on )
		{

			// ---- persist ?
				BOOL active = GetMsgCameraControlPersistantActive();
				if ( active )
				{

					// ---- with what ?
						UINT control_message = GetCameraControlMessage();

					// ---- img capture ?
						FLOAT delta = 0;
						INT event_id = pi_model->sys_context->GetDWord(vs_system::SysContext::WINDOW_TRIGGER);
						if (event_id != 0)
						{
							// ---- overide actual delta to force 50fps !!!
								delta = 50;
						}
						else
						{
							vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
							delta = (FLOAT)sys_context->GetDouble(vs_system::SysContext::TIME_MS_DELTA);
						}

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
		vs_system::AppKamera *kamera = pi_model->app_kamera[pi_model->app_kamera_index];


	// ---- mouse wheel ? -- swap for zoom msg, and adjust delta
		if ( message == '{' || message == '}' )
		{
			UINT target_planet_index = pi_model->GetTargetObjectId();
			if (target_planet_index == SystemParam::CameraTarget::NONE)
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
		if ( message > '0' && message <= ';' )
		{
			message_adj_for_target = '0';
		}


		using namespace vs_system;


	// ---- process
		switch ( message_adj_for_target )
		{

			// ----- look at target
				//
				// note ascii order is: "012 ... 9"
				//
				case '0':
					{
						UINT target_planet_index = message - '0';
						pi_model->SetTargetObjectId(target_planet_index);
						pi_model->SetCameraControlMode(AppKamera::Mode::TARGET);
						SetCameraControlMessage(message);
					}
					return VS_OK;

			// ---- reset
				case 'Q':
				case 'E':
					{
						UINT switched_on = GetMsgCameraControlPersistantOn();
						switched_on = switched_on == TRUE ? FALSE : TRUE;
						SetMsgCameraControlPersistantOn(switched_on);
						SetMsgCameraControlPersistantActive(FALSE);
					}
					return VS_OK;

			// ---- system light
				case 'Z': 
					{
						UINT svm = pi_model->system_param->IncSystemLightMode();
						INT result = InitStarSystemObjects();
						SetCameraControlMessage(message);
					}
					return VS_OK;

			// ---- reset
				case 'X':
					{
						kamera->SetTargetZoomDefault();
						kamera->SetPanTiltDefault();
						kamera->SetPositionDefault();
						kamera->SetDefaultRotate();
						kamera->SetDefaultTranslate();
						SetMsgCameraControlPersistantOn(FALSE);
						SetMsgCameraControlPersistantActive(FALSE);
						INT result = Tint_Explore_UpdateControlByMsg((INT)this, 0);
					}
					return VS_OK;

			// ---- system view
				case 'C': 
					{
						UINT svm = pi_model->system_param->IncSystemViewMode();
						INT result = InitStarSystemObjects();
						SetCameraControlMessage(message);
					}
					return VS_OK;

			// ---- camera control mode
				case 'G':
					{
						INT mode = pi_model->GetCameraControlMode() + 1;
						mode = mode > AppKamera::Mode::ROTATE ?
										AppKamera::Mode::XYZ : mode;
						pi_model->SetCameraControlMode(mode);
						pi_model->SetTargetObjectId(SystemParam::CameraTarget::NONE);
						kamera->SetTargetStatus(FALSE);
						SetCameraControlMessage(message);
					}
					return VS_OK;

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

						// ---- NOT persistant
							//
							// note: NOT from from display method
							//
							if ( !persistant )
							{

								// ---- ?
									BOOL switched_on = GetMsgCameraControlPersistantOn();
									if ( switched_on )
									{

										// ---- on, then flip
											INT old_control_message = GetCameraControlMessage();
											if ( old_control_message == message )
											{
												// active (pen up/down) logic
												//
												// note: flip logic to allow multiple selections of the same action
												//
												BOOL active = GetMsgCameraControlPersistantActive();
												active = active == TRUE ? FALSE : TRUE;
												SetMsgCameraControlPersistantActive(active);
												if ( !active )
												{
													SetCameraControlMessage(message);
													return VS_OK;
												}
											}
										// ---- else on
											else
											{
												SetMsgCameraControlPersistantActive(TRUE);
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
												default:
													break;
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
												default:
													break;
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
												default:
													break;
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
												case 'A': kamera->IncRotate(AppKamera::Ordinate::Y, delta); break;
												case 'D': kamera->IncRotate(AppKamera::Ordinate::Y,-delta); break;
												case 'R': kamera->IncRotate(AppKamera::Ordinate::Z, delta); break;
												case 'F': kamera->IncRotate(AppKamera::Ordinate::Z,-delta); break;
											}
											break;

										case AppKamera::Mode::TARGET :
											{

												//INT type = s_pi_model->system_param->Type();

												INT svm = s_pi_model->system_param->GetSystemViewMode();
												FLOAT zoom_delta = delta;
												switch ( svm )
												{
													case 0:
														zoom_delta *= 0.005f;
														break;
													case 1:
														zoom_delta *= 0.25f;
														break;
													case 2:
														zoom_delta *= 2;
														break;
													default:
														break;
												}

												switch ( message )
												{
													case 'R': kamera->IncTargetTilt(-delta);   break;
													case 'F': kamera->IncTargetTilt( delta);   break;
													case 'A': kamera->IncTargetPan( delta);    break;
													case 'D': kamera->IncTargetPan(-delta);    break;
													case 'W': kamera->IncTargetZoom(-zoom_delta);   break;
													case 'S': kamera->IncTargetZoom(zoom_delta);    break;
													case 'J':
														{
															kamera->IncTargetZoom(zoom_delta /5);
															kamera->IncTargetTilt(delta);
															kamera->IncTargetPan(delta);
														}
														break;
													case 'K':
														{
															kamera->IncTargetTilt(delta);
															kamera->IncTargetPan(delta);
														}
														break;
													case 'L':
														{
															kamera->IncTargetZoom(-zoom_delta / 5);
															kamera->IncTargetTilt(delta);
															kamera->IncTargetPan(delta);
														}
														break;
													default :
														break;
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
					return VS_OK;


			// ---- other stuff
				default:
					break;
		}


	// ---- consume msg
		SetCameraControlMessage(0);

	return VS_OK;
}


// ========================================================================== //