////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model_kamera.hpp ----------
/*!
\file s3_model_kamera.hpp
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
					{
						INT body_id = pi_model->tint_setng->GetSelectedBodyId();
						switch (body_id)
						{
							case TINT_BODY_LIGHT_LID:
							case TINT_BODY_EXPLORE_LID:
							case TINT_BODY_MEASURE_LID:
								result = SetMsgCameraControl(uc_msg, delta, FALSE);
								break;
							default:
								break;
						}
					}
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


	// ---- on ?
		UINT persistant_switched_on = GetMsgCameraControlPersistantOn();
		if ( persistant_switched_on )
		{

			// ---- active ?
				BOOL persistant_active = GetMsgCameraControlPersistantActive();
				if ( persistant_active )
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


	// ---- local shortcuts
		vs_system::AppKamera *kamera = pi_model->app_kamera[0];
		model::Surface_03 *surface_03 = pi_model->surface_03;


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
				delta = message == 'W' || message == 'S' ? delta : delta; // eh....?
			}
		}


	// ----- catch target object index msg
		INT message_adj_for_target = message;
		if (message > '0' && message <= ';')
		{
			message_adj_for_target = '0';
		}


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
						// ---- note: if max is 7, then tid_max is 6...
							UINT tid = message - '0';
							UINT tid_max = surface_03->GetTargetLoiMax() - 1;
							tid = tid > tid_max ? tid_max : tid;

						// ---- set legal target id
							pi_model->SetTargetObjectId(tid);
							pi_model->SetCameraControlMode(AppKamera::Mode::TARGET);
							surface_03->SetTargetLoiIndex(tid);
					}
					break;

			// ---- persistant
				case 'Q':
				case 'E':
					{
						UINT persistant_switched_on = GetMsgCameraControlPersistantOn();
						persistant_switched_on = persistant_switched_on == TRUE ? FALSE : TRUE;
						SetMsgCameraControlPersistantOn(persistant_switched_on);
						SetMsgCameraControlPersistantActive(FALSE);
					}
					return VS_OK;

			// ---- camera control mode
				case 'G':
					{
						INT mode = pi_model->GetCameraControlMode() + 1;
						mode = mode > AppKamera::Mode::ROTATE ?
							AppKamera::Mode::XYZ : mode;
						pi_model->SetCameraControlMode(mode);
						pi_model->SetTargetObjectId(CameraTarget::NONE);
						surface_03->SetTargetLoiIndex(CameraTarget::NONE);
						kamera->SetTargetStatus(FALSE);
					}
					break;

			// ---- mesh display mode
				case 'Z':
					{

						UINT mdm = surface_03->GetMeshDisplayMode() + 1;
						mdm = mdm > 3 ? 0 : mdm;
						surface_03->SetMeshDisplayMode(mdm);

						BOOL solid    = surface_03->GetMeshSolid();
						BOOL textured = surface_03->GetMeshTextured();
						switch (mdm)
						{
							case 0: solid = 0, textured = 0; break;
							case 1: solid = 1, textured = 0; break;
							case 2: solid = 0, textured = 1; break;
							case 3: solid = 1, textured = 1; break;
						}
						
						surface_03->SetMeshSolid(solid);
						surface_03->SetMeshTextured(textured);

						surface_03->UpdateHeightfieldObjects((INT)'K', surface_03, FALSE);
					}
					break;

			// ---- reset NOT position (RNP) (TBD: rename DRT - default rotate & translate)
				case '&':
					{
						kamera->SetDefaultRotate();
						kamera->SetDefaultTranslate();
						return VS_OK;
					}
					break;

			// ---- reset all (RAL)
				case 'X':
					{

						// ---- kamera PTZ
							kamera->SetTargetZoomDefault();
							kamera->SetPanTiltDefault();
							kamera->SetPositionDefault();

						// ---- kamera position
							kamera->SetDefaultRotate();
							kamera->SetDefaultTranslate();

						// ---- targets
							surface_03->InitTargetLoi();

						return VS_OK;
					}
					break;

			// ---- mesh density mode
				case 'C':
					{

						// ----- note: also see s3_model_kamera.hpp 

						// ---- update model
							UINT mode = surface_03->GetMeshDensityMode() + 1;
							mode = mode > surface_03->HIGH ? surface_03->LOW : mode;
							surface_03->SetMeshDensityMode(mode);

						// ---- update surface
							surface_03->SetMeshDensity(mode + 1);
							surface_03->UpdateHeightfieldObjects((INT)'K', surface_03, FALSE);
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

						// ---- NOT persistant
							//
							// note: NOT from from display method
							//
							if ( !persistant )
							{

								// ---- on ?
									BOOL persistant_switched_on = GetMsgCameraControlPersistantOn();
									if ( persistant_switched_on )
									{

											BOOL old_control_message = GetCameraControlMessage();
											if ( old_control_message == message )
											{

												BOOL persistant_active = GetMsgCameraControlPersistantActive();
												persistant_active = persistant_active == TRUE ? FALSE : TRUE;
												SetMsgCameraControlPersistantActive(persistant_active);
												if ( !persistant_active )
												{
													SetCameraControlMessage(message);
													return VS_OK;
												}
											}
											else
											{
												SetMsgCameraControlPersistantActive(TRUE);
											}

									}
							}


						// ---- prevent "persistant same message" increments
							if ( GetMsgCameraControlCount() < 2 )
							{

								// ---- process
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

												// ---- scalar proportional to surface size
													FLOAT surface_limit  = surface_03->GetAcquireSurfaceLimit();
													FLOAT surface_size   = surface_03->GetAcquireSurfaceSize();
													FLOAT surface_scalar = surface_limit / surface_size;

												// ---- temp stuff
													FLOAT zoom_scalar = 1.0f;
													FLOAT t_scalar = 0.1f;
													FLOAT r_scalar = 0.25f;

												// ---- transforms proportional to surface size
													t_scalar *= surface_size / surface_limit;

												// ---- section mode? - then msg "RF" moves target along section/profile 
													BOOL section_mode = surface_03->GetTargetLoiSectionMode();
													VsVector3 v;
													if ( section_mode )
													{

														// ---- delta & increment proportional to surface size
															UINT  loi_index = surface_03->GetTargetLoiIndex();
															FLOAT loi_rotation = surface_03->GetTargetLoiRotation(loi_index);
															FLOAT radian = -loi_rotation * (FLOAT)VS_RADIANS_PER_DEGREE;
															FLOAT s_delta = kamera->GetScalarMode() ?
																			delta * t_scalar / 10 : 
																			delta * t_scalar / 40;
															v.x = (FLOAT)cos(radian) * s_delta;
															v.z = (FLOAT)sin(radian) * s_delta;
													}


												// ---- target mode? - then msg "RFADWS" moves or rotates target section 
													BOOL target_mode = surface_03->GetTargetLoiTargetMode();
													FLOAT t_delta = delta/ surface_scalar, r_delta = 0;
													if ( target_mode )
													{

														// ---- transform & rotational delta roportional to surface size
															if ( kamera->GetScalarMode() )
															{
																t_delta = delta * t_scalar / 10;
																r_delta = delta * r_scalar / 10;
															}
															else
															{
																t_delta = delta * t_scalar / 40;
																r_delta = delta * r_scalar / 40;
															}
													}


												// ---- kamera OR target transform & section modes
													switch ( message )
													{
														case 'R':
															{
																if ( target_mode )
																{
																	if ( section_mode )
																	{
																		BOOL flip_dir = surface_03->CalculateTargetLoi_Surface_Increment(v.x, 0, v.z, persistant);
																		if ( flip_dir )
																		{
																			message = 'F';
																			INT result = Tint_Explore_UpdateControlByMsg((INT)this, message);
																		}
																	}
																	else
																	{
																		surface_03->CalculateTargetLoi_Rotation_Increment(r_delta);
																	}
																}  
																else
																	kamera->IncTargetTilt(-delta);
															}
															break;
														case 'F':
															{
																if (target_mode)
																{
																	if (section_mode)
																	{
																		BOOL flip_dir = surface_03->CalculateTargetLoi_Surface_Increment(-v.x, 0, -v.z, persistant);
																		if (flip_dir)
																		{
																			message = 'R';
																			INT result = Tint_Explore_UpdateControlByMsg((INT)this, message);
																		}
																	}
																	else
																	{
																		surface_03->CalculateTargetLoi_Rotation_Increment(-r_delta);
																	}
																}
																else
																	kamera->IncTargetTilt( delta);
															}
															break;
														case 'A':
															{
																if (target_mode)
																{
																	BOOL flip_dir = surface_03->CalculateTargetLoi_Surface_Increment(-t_delta, 0, 0, persistant);
																	if ( flip_dir )
																	{
																		message = 'D';
																		INT result = Tint_Explore_UpdateControlByMsg((INT)this, message);
																	}
																}
																else
																	kamera->IncTargetPan( delta);
															}
															break;
														case 'D':
															{
																if ( target_mode )
																{
																	BOOL flip_dir = surface_03->CalculateTargetLoi_Surface_Increment(t_delta, 0, 0, persistant);
																	if ( flip_dir )
																	{
																		message = 'A';
																		INT result = Tint_Explore_UpdateControlByMsg((INT)this, message);
																	}
																}
																else
																	kamera->IncTargetPan(-delta);
															}break;
														case 'W':
															{
																if ( target_mode )
																{
																	BOOL flip_dir = surface_03->CalculateTargetLoi_Surface_Increment(0, 0, t_delta, persistant);
																	if ( flip_dir )
																	{
																		message = 'S';
																		INT result = Tint_Explore_UpdateControlByMsg((INT)this, message);
																	}
																}
																else
																	kamera->IncTargetZoom(-delta * zoom_scalar);
															}
															break;
														case 'S':
															{
																if ( target_mode )
																{
																	BOOL flip_dir = surface_03->CalculateTargetLoi_Surface_Increment(0, 0, -t_delta, persistant);
																	if ( flip_dir )
																	{
																		message = 'W';
																		INT result = Tint_Explore_UpdateControlByMsg((INT)this, message);
																	}
																}
																else
																	kamera->IncTargetZoom(delta * zoom_scalar);
															}
															break;

														// ---- zoom away
														case 'J':
															{
																kamera->IncTargetZoom(delta * zoom_scalar);
																kamera->IncTargetPan(delta );
															}
															break;

														// ---- orbit (tilt proportional to pan) around
														case 'K':
															{

																kamera->IncTargetTilt(delta);
																kamera->IncTargetPan(delta);

																FLOAT pan       = kamera->GetTargetPan();
																FLOAT tilt_now  = kamera->GetTargetTilt();
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
																kamera->IncTargetZoom(-delta * zoom_scalar);
																kamera->IncTargetPan(delta);
															}
															break;
														default :
															break;
													}   


												// ---- really big deal ! 
													if ( target_mode )
													{

														// ---- update loi objects
															INT tid = surface_03->GetTargetLoiIndex();
															surface_03->UpdateLoiObjects(tid, surface_03);

														// ---- refresh ALL measure tab elements
															Tint_Measure_UpdateElement((INT)this, 0);
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