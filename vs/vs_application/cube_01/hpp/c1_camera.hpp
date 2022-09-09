////////////////////////////////////////////////////////////////////////////////

// ---------- c1_camera.hpp ----------
/*!
\file c1_camera.hpp
\brief Implementation of the Camera methods
\author Gareth Edwards
*/


// ========================================================================== //

INT Model::SetMyKamera(INT camera_index)
{

		vs_system::AppKamera *kamera = pi_model->kamera;
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

	// ---- local
		INT result = VS_OK;


	// ---- process
		INT uc_msg = toupper(msg);
		switch ( uc_msg )
		{

			// ---- turntable
				case 'P': // pause
				case '-': // dec
				case '=': // reset
				case '+': // inc
					result = SetTime(uc_msg, delta);
					
					break;

			// ---- system
				case 'I': // image
				case 'T': // texture
				case 'V': // view mode
					result = SetSystem(uc_msg, delta);
					break;

			// ---- camera
				default:
					result = SetCameraControl(uc_msg, delta, FALSE);
					break;
		}

	return result;
}


// ========================================================================== //


INT Model::SetTime(
		INT msg,
		FLOAT delta
	)
{

		switch ( msg )
		{

			// ---- pause
				case 'P':
					{
						BOOL tts = pi_model->GetTurntableStatus();
						tts = tts == TRUE ? FALSE : TRUE; 
						pi_model->SetTurntableStatus(tts);
					}
					break;

			// ---- slow down - 1 degree
				case '-':
					{
						FLOAT ttd = pi_model->GetTurntableDegrees();
						ttd -= 1;
						pi_model->SetTurntableDegrees(ttd);
					}
					break;

			// ---- reset
				case '=':
					pi_model->SetTurntableDegrees(0);
					break;

			// ---- speed up + 1 degree
				case '+':
					{
						FLOAT ttd = pi_model->GetTurntableDegrees();
						ttd += 1;
						pi_model->SetTurntableDegrees(ttd);
					}
					break;

			// ---- what ?
				default:
					break;
		}


	// ---- consume msg
		pi_model->SetCameraControlMessage(0);

	return VS_OK;
}


// ========================================================================== //


INT Model::SetSystem(
		INT msg,
		FLOAT delta
	)
{

		switch ( msg )
		{

			// ---- image capture - not used at present...
				case 'I':
					{
						INT result = pi_model->sys_context->SetDWord(vs_system::SysContext::WINDOW_TRIGGER, 666);
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

			// ---- view mode
				case 'V': 
					{
						;
					}
					break;

			// ---- what ?
				default:
					break;
		}


	// ---- consume msg
		pi_model->SetCameraControlMessage(0);

	return VS_OK;
}


// ========================================================================== //


INT Model::SetPersistantCameraControl()
{

	// ---- zero "persistant same message" counter
		pi_model->SetMsgCameraControlCount(0);

	// ---- on ?
		UINT switched_on = pi_model->GetMsgCameraControlPersistantOn();
		if ( switched_on )
		{

			// ---- persist ?
				BOOL active = pi_model->GetMsgCameraControlPersistantActive();
				if ( active )
				{
					// ---- with what ?
						UINT control_message = pi_model->GetCameraControlMessage();
						vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
						FLOAT delta = (FLOAT)sys_context->GetDouble(vs_system::SysContext::TIME_MS_DELTA);

					// ---- process
						SetCameraControl(control_message, delta, TRUE);
				}

		}

	return VS_OK;
}


// ========================================================================== //


INT Model::SetCameraControl(
		INT   message,
		FLOAT delta,
		BOOL  persistant
	)
{

	// ---- increment "persistant same message" counter
		pi_model->IncMsgCameraControlCount();



	// ---- mouse wheel ? -- swap for zoom msg, and adjust delta
		if ( message == '{' || message == '}' )
		{
			UINT target_planet_index = pi_model->GetTargetObjectId();
			if (target_planet_index == CameraTarget::NONE)
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


	// ---- kamera
		UINT ccm = pi_model->GetCameraControlMode();
		vs_system::AppKamera *kamera = pi_model->kamera;


		using namespace vs_system;


	// ---- process
		switch ( message_adj_for_target )
		{

			// ----- look at target
				//
				// note ascii order is: "0123456789"
				//
				case '0':
					{
						UINT target_sector_index = message - '0';
						pi_model->SetTargetObjectId(target_sector_index);
						pi_model->SetCameraControlMode(AppKamera::Mode::TARGET);
					}
					break;

			// ---- reset
				case 'Q':
				case 'E':
					{
						UINT switched_on = pi_model->GetMsgCameraControlPersistantOn();
						switched_on = switched_on == TRUE ? FALSE : TRUE;
						pi_model->SetMsgCameraControlPersistantOn(switched_on);
						pi_model->SetMsgCameraControlPersistantActive(FALSE);
					}
					return VS_OK;

			// ---- reset
				case 'Y':
					{
						SetupCamera();
					}
					break;

			// ---- camera update mode
				case 'Z':
					pi_model->SetCameraControlMode(AppKamera::Mode::XYZ);
					pi_model->SetTargetObjectId(CameraTarget::NONE);
					kamera->SetTargetStatus(FALSE);
					break;

				case 'X':
					pi_model->SetCameraControlMode(AppKamera::Mode::VECTOR);
					pi_model->SetTargetObjectId(CameraTarget::NONE);
					kamera->SetTargetStatus(FALSE);
					break;

				case 'C':
					pi_model->SetCameraControlMode(AppKamera::Mode::PTZ);
					pi_model->SetTargetObjectId(CameraTarget::NONE);
					kamera->SetTargetStatus(FALSE);
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

			// ---- stuff
				case 'W':
				case 'S':
				case 'A':
				case 'D':
				case 'R':
				case 'F':
					{

						// ---- NOT persistant
							//
							// note: NOT from from display method
							//
							if ( !persistant )
							{

									BOOL switched_on = pi_model->GetMsgCameraControlPersistantOn();
									if ( switched_on )
									{

											BOOL previous_control_message = pi_model->GetCameraControlMessage();
											if ( previous_control_message == message )
											{

												BOOL active = pi_model->GetMsgCameraControlPersistantActive();
												active = active == TRUE ? FALSE : TRUE;
												pi_model->SetMsgCameraControlPersistantActive(active);

												if ( !active)
												{
													pi_model->SetCameraControlMessage(message);
													return VS_OK;
												}

											}
											else
											{
												pi_model->SetMsgCameraControlPersistantActive(TRUE);
											}

									}
							}

						// ---- prevent "persistant same message" increments
							if ( pi_model->GetMsgCameraControlCount() < 2 )
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

										case AppKamera::Mode::TARGET :
											switch ( message )
											{
												case 'R': kamera->IncTargetTilt(-delta);   break;
												case 'F': kamera->IncTargetTilt( delta);   break;
												case 'A': kamera->IncTargetPan( delta);    break;
												case 'D': kamera->IncTargetPan(-delta);    break;
												case 'W': kamera->IncTargetZoom(-delta);   break;
												case 'S': kamera->IncTargetZoom(delta);    break;
											}
											break;

										default:
											break;
									}

							}

						// ---- store control message
							pi_model->SetCameraControlMessage(message);

					}
					return VS_OK;


			// ---- other stuff
				default:
					break;
		}


	// ---- consume msg
		pi_model->SetCameraControlMessage(0);

	return VS_OK;
}


// ========================================================================== //