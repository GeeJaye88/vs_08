////////////////////////////////////////////////////////////////////////////////

// ---------- c3_mon_buffer.hpp ----------
/*!
\file c3_mon_buffer.hpp
\brief 
\author Gareth Edwards
*/


// ---------- MonitorBuffer ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK

RGB -> Luma conversion formula.

Photometric/digital ITU BT.709:

Y = 0.2126 R + 0.7152 G + 0.0722 B

Digital ITU BT.601 (gives more weight to the R and B components):

Y = 0.299 R + 0.587 G + 0.114 B

If you are willing to trade accuracy for perfomance, there are two
approximation formulas for this one:

Y = 0.33 R + 0.5 G + 0.16 B

Y = 0.375 R + 0.5 G + 0.125 B

These can be calculated quickly as:

Y = (R+R+B+G+G+G) / 6

Y = (R+R+R+B+G+G+G+G) >> 3

*/
INT PI_Props::MonitorBuffer(WORD w, WORD h, WORD bpp, BYTE *byte_data)
{
	if ( !monitor_on ) return VS_OK;

	// ---- IF null OR dimension changed THEN (re-)create ----
	BOOL create_monitor_samples_lut = FALSE;
	UINT samples_size =
		(w / monitor_sample_incr) *
		(h / monitor_sample_incr);
	if ( monitor_samples_lut == NULL )
	{
		create_monitor_samples_lut = TRUE;
	}
	else if ( samples_size != monitor_samples_size )
	{
		if ( monitor_samples_lut != NULL ) delete [] monitor_samples_lut;
		create_monitor_samples_lut = TRUE;
	}


	// ---- IF create THEN allocate and intialise ----
	if ( create_monitor_samples_lut )
	{
		monitor_samples_size = samples_size;
		monitor_samples_lut = new BYTE[monitor_samples_size];
		for (UINT i = 0; i < monitor_samples_size; i++)
		{
			monitor_samples_lut[i] = 0;
		}
	}

	// ---- sample offset ----
	UINT sample_offset = monitor_sample_incr/2;


	// ---- reset bucket ----
	for (UINT i = 0; i < 16; i++)
	{
		monitor_prev_samples_lut[i] = monitor_next_samples_lut[i];
		monitor_next_samples_lut[i] = 0;
	}


	// lumin change
	INT monitor_threshold_dif = (INT)monitor_threshold;


	// ---- reset ----
	monitor_total_samples_changed = 0;
	monitor_total_samples = 0;


	// ---- sample into buckets by lumin index ----
	BYTE *byte_src = byte_data;
	BYTE  r, g, b;
	DWORD h_max = h - monitor_sample_incr;
	DWORD w_max = w - monitor_sample_incr;


	for (UINT y = sample_offset; y < h_max; y += monitor_sample_incr)
	{
		UINT y_offset = bpp * w * y;
		for (UINT x = sample_offset; x < w_max; x += monitor_sample_incr)
		{

			// assign
			UINT offset = y_offset + bpp * x;
			r = *(byte_src + offset);
			g = *(byte_src + offset + 1);
			b = *(byte_src + offset + 2);

			// calculate fast lumin bucket index
			UINT lumin = ((r + r + g + g + g + b) / 6);

			// update
			INT lumin_dif = abs((INT)( lumin - monitor_samples_lut[monitor_total_samples] ) ) ;
			if ( lumin_dif > monitor_threshold_dif)
			{
				monitor_total_samples_changed++;
			}
			monitor_samples_lut[monitor_total_samples] = lumin;

			// calculate lumin bucket index
			lumin = lumin / 16;

			// catch rounding
			lumin = lumin > 15 ? 15 : lumin;

			// increment bucket
			monitor_next_samples_lut[lumin]++;
			byte_src += bpp;

			// increment total samples
			monitor_total_samples++;
		}
	}

	// ---- calc max ----
	LONG max = 0;
	for (UINT i = 0; i < 16; i++)
	{
		max = (LONG)monitor_next_samples_lut[i] > max ? (LONG)monitor_next_samples_lut[i] : max;
	}


	// ---- copy next lut into prev lut ----
	for (UINT i = 0; i < 16; i++)
	{
		monitor_prev_display_lut[i] = monitor_next_display_lut[i];
	}


	// ---- normalise display lut ----
	for (UINT i = 0; i < 16; i++)
	{
		monitor_next_display_lut[i] = (FLOAT)monitor_next_samples_lut[i] / (FLOAT)max;
	}


	return VS_OK;
}


// ---------- MonitorAlert ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT PI_Props::MonitorAlert()
{

	// ---- calculate %'s ----
	UINT total_prev_samples_changed = 0;
	FLOAT percent_dif[16];
	for (UINT i = 0; i < 16; i++)
	{
		FLOAT percent_sample_next = (FLOAT)monitor_next_samples_lut[i] / (FLOAT)monitor_total_samples;
		FLOAT percent_sample_prev = (FLOAT)monitor_prev_samples_lut[i] / (FLOAT)monitor_total_samples;
		percent_dif[i] = percent_sample_next - percent_sample_prev;
	}


	// ---- check ----
	UINT total_percent_changed = (UINT)( (FLOAT)monitor_total_samples_changed / (FLOAT)monitor_total_samples * 100 );
	if ( total_percent_changed > monitor_threshold )
	{
		//FLOAT difference = abs((FLOAT)(monitor_prev_display_lut[i] - monitor_next_display_lut[i]));
		//if ( difference > monitor_threshold )
		// if ( percent_dif[i] > monitor_threshold )
		{
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);
			char f[VS_MAXCHARLEN];
			sprintf(f, "Monitor:-%d%%-exceeded-change-threshold-of-%d%%-at-Time-%2d:%2d:%2d:%3d-and-%d-IPS.",
					total_percent_changed,
					(INT)monitor_threshold,
					SystemTime.wHour,
					SystemTime.wMinute,
					SystemTime.wSecond,
					SystemTime.wMilliseconds,
					images_per_second
				);
			WORD len_filename = (WORD)strlen(f);
			for (WORD i = 0; i<len_filename; i++) f[i] = f[i] == ' ' ? '0' : f[i];
			for (WORD i = 0; i<len_filename; i++) f[i] = f[i] == '-' ? ' ' : f[i];
			sys_output->Message(f);

			/*
			strcpy(f, "Histogram % delta change: ");
			for (UINT i = 0; i < 16; i++)
			{
				CHAR tmp[16];
				sprintf(tmp, "%d:%d ", i, (INT)(percent_dif[i]*100) );
				strcat(f, tmp);
			}
			sys_output->Message(f);
			*/

			return VS_ERROR;
		}
	}

	return VS_OK;
}

