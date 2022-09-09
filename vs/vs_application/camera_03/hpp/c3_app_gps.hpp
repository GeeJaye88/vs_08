
// ---------- ProcessGpsReader ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
INT application::ProcessGpsReader()
{

	#ifdef VS_CAMERA_03_GPS

	// ---- process gps display ----
	INT result = ProcessGpsStatus();


	// ---- if gps is running ----
	if ( pi_props->pGPSReader->IsRunning() )
	{

		// local
		GPSReader::GPSData gpsd;


		// ---- tidy up: ' ' & '0' and '-' with ' '  ------------------------------------------------------------
		auto charTidyUp = [](
			CHAR *str
			)
		{
			UINT len = strlen(str);
			for (UINT i = 0; i < len; i++)
			{
				str[i] = str[i] == ' ' ? '0' :
					(str[i] == '_' ? ' ' : str[i]);
			}
			return VS_OK;
		};


		// ---- check for positioning ---- 
		if (  pi_props->pGPSReader->Pop(gpsd) )
		{
	
			do
			{

				DBMSG("GPS: " << gpsd.time.hh << ':' << gpsd.time.mm << ':' << gpsd.time.ss << '.' << gpsd.time.ms << ' ' << gpsd.lat << ' ' << gpsd.lon);
				
				/*
				printf("Lat %10.4f ( %s )   Lon: %10.4f ( %s ) \n",
						gpsd.lat,
						gpsd.slat.c_str(),
						gpsd.lon,
						gpsd.slon.c_str()
					);
				*/

				// ---- convert NMEA format string to degrees, minutes and seconds ------------------------------------------------------------
				auto toDMS = [](
						std::string str, // NMEA format, e.g. 5104.35198
						INT *deg,        // Returned degrees, e.g. 51°
						INT *min,        // Returned minutes, e.g. 4'
						DOUBLE *sec      // Returned seconds, e.g. 21.87"
					)
				{
					DOUBLE val = stod(str);
					val = val < 0 ? -val : val;
					*deg = (INT)(val / 100);
					*min = (INT)(val - (DOUBLE)*deg * 100);
					*sec = (val - (DOUBLE)*deg * 100 - (DOUBLE)*min) * 60;
					return VS_OK;
				};


				// ---- convert NMEA format string to digital GPS value ------------------------------------------------------------
				auto toDec = [](
						std::string str, // NMEA format, e.g. 5104.35198
						DOUBLE *dec      // Returned digital GPS value, e.g. 51.075°
					)
				{
					DOUBLE val = stod(str);
					INT deg = (INT)(val / 100);
					*dec = (DOUBLE)deg + (val - (FLOAT)deg * 100) / 60;
					return VS_OK;
				};


				// ---- format Longitude ----
				DOUBLE lon_dec = 0;
					result = toDec(gpsd.slon, &lon_dec);
				INT lon_deg = 0, lon_min = 0;
				DOUBLE lon_sec = 0;
					result = toDMS(gpsd.slon, &lon_deg, &lon_min, &lon_sec);
				sprintf(
						pi_props->gps_lon,
						"%2d°_%2d'_%5.2f\"_%c",
						lon_deg, lon_min, lon_sec, gpsd.ew
					);


				// ---- format Latitude ----
				DOUBLE lat_dec = 0;
					result = toDec(gpsd.slat, &lat_dec);
				INT lat_deg = 0, lat_min = 0;
				DOUBLE lat_sec = 0;
					result = toDMS(gpsd.slat, &lat_deg, &lat_min, &lat_sec);
				sprintf(
						pi_props->gps_lat,
						"%2d°_%2d'_%5.2f\"_%c",
						lat_deg, lat_min, lat_sec, gpsd.ns
					);


				// ---- format Time ----
				sprintf(
					pi_props->gps_time,
						"%2d:%2d:%2d_%3d",
						gpsd.time.hh,
						gpsd.time.mm,
						gpsd.time.ss,
						gpsd.time.ms
					);


				// ---- tidy up Lon, Lat and Time ----
				result = charTidyUp(pi_props->gps_lon);
				result = charTidyUp(pi_props->gps_lat);
				result = charTidyUp(pi_props->gps_time);


				// ---- assemble ----
				CHAR *d[4] = {
					pi_props->gps_lon,
					pi_props->gps_lat,
					pi_props->gps_time,
					""
				};


				// ---- Update ----
				pi_props->model->UpdateGpsDataStr(d);

			}
			while (  pi_props->pGPSReader->Pop(gpsd) );


			// acquire most recent date
			if ( pi_props->pGPSReader->GetDateTime(gpsd.time) )
			{

				DBMSG("Date: " << gpsd.time.yy << '-' << gpsd.time.mo << '-' << gpsd.time.dd << 'T' << gpsd.time.hh << ':' << gpsd.time.mm << ':' << gpsd.time.ss << '.' << gpsd.time.ms);

				// ---- char - format Date ----
				if ( gpsd.time.nc )
					sprintf(
						pi_props->gps_date,
						"%2d-%2d-20%2d",
						gpsd.time.dd,
						gpsd.time.mo,
						gpsd.time.yy
					);
				else
					sprintf(
						pi_props->gps_date,
						"%2d-%2d-%4d",
						gpsd.time.dd,
						gpsd.time.mo,
						gpsd.time.yy
					);


				// ---- tidy up Date ----
				INT result = charTidyUp(pi_props->gps_date);


				// ---- assemble ----
				CHAR *d[4] = {
					"", "", "",
					pi_props->gps_date,
				};


				// ---- update ----
				pi_props->model->UpdateGpsDataStr(d);

			}

		}

		else
		{

			//OutputDebugString("Pop returned false\n");
			;//printf("Pop returned false\n");

		}

	}

	#endif

	return VS_OK;
}


// ---------- ProcessGpsStatus ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017

\note Invoked each frame?

*/
INT application::ProcessGpsStatus()
{

	// ---- manage gps state ----

	switch ( pi_props->gps_mode )
	{

		case PI_Props::GPS_RUNTIME:
			{
				pi_props->gps_mode = PI_Props::GPS_TEST_DELAY;
				return VS_OK;
			}
			break;


		case PI_Props::GPS_TEST_DELAY:
			return VS_OK;


		case PI_Props::GPS_TEST_RUNNING:
			{

				#ifdef VS_CAMERA_03_GPS

				if ( pi_props->pGPSReader->IsRunning() )
				{
					pi_props->gps_mode = PI_Props::GPS_IS_RUNNING;
					pi_props->sys_output->Message("GPS: Device has LOCK.");

				}
				else
				{
					pi_props->gps_mode = PI_Props::GPS_IS_NOT_RUNNING;
					pi_props->sys_output->Message("GPS: NO device or LOCK has NOT been acquired.");
				}

				#endif

			}
			break;


		case PI_Props::GPS_IS_NOT_RUNNING:
			return VS_OK;


		case PI_Props::GPS_IS_RUNNING:

		default:
			break;
	}

	return VS_OK;
}

