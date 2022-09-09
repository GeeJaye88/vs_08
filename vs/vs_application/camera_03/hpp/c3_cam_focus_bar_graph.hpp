////////////////////////////////////////////////////////////////////////////////

// ---------- c3_cam_focus_bar_graph.hpp ----------
/*!
\file c3_cam_focus_bar_graph.hpp
\brief 
\author Gareth Edwards
*/


// ---------- FocusBuffer ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK

*/
INT PI_Props::UpdateFocusBarGraph(
		WORD width,
		WORD height,
		WORD bpp,
		BYTE *byte_data
	)
{

	// ---- return
		if ( !focus_on ) return VS_OK;


	// ---- local
		INT result = VS_OK;


	// ---- derived
		FLOAT hby2 = (FLOAT)height / 2;
		FLOAT wby2 = (FLOAT)width / 2;


	// ---- pano
		FLOAT u_offset, v_offset;
		model->GetObjConfig()->GetPanoUVOffset(&u_offset, &v_offset);
		u_offset *= 2;
		v_offset *= 2;


	// ---- lambda ------------------------------------------------------------
		auto rotit = [&](FLOAT sina, FLOAT cosa, FLOAT y, FLOAT *rx, FLOAT *ry)
		{
			*rx = sina * y + wby2 + (u_offset * wby2);
			*ry = cosa * y + hby2 + (v_offset * hby2);
		};


	// ---- camera focus properties
		vs_system::CamProps *cam_props;
		model->GetCamProps(&cam_props);


	// ---- IF -- NOT focus -- THEN -- return
		FLOAT display = 0;
		{
			result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::DISPLAY,
					&display
				);
		}
		if ( display < 1 ) return VS_OK;


	// ---- get camera properties param
		FLOAT focus_min = 0;
		FLOAT focus_max = 0;
		FLOAT rotate = 0;
		FLOAT sector = 0;
		FLOAT sector_by2 = 0;
		{
			result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::MIN,
					&focus_min
				);
			result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::MAX,
					&focus_max
				);
			result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::ROTATE,
					&rotate
				);
			result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::SECTOR,
					&sector
				);
			sector_by2 = sector / 2;
		}


	// ---- get camera graphs ----
		vs_system::CamGraphs *cam_graphs;
		model->GetCamGraphs(&cam_graphs);


	// ---- get camera graphs - float buffer
		vs_system::FloatBuffer *float_buffer = NULL;
		{
			result = cam_graphs->GetFloatBuffer(
					vs_system::CamGraphs::FOCUS,
				 	&float_buffer
				);
		}
		UINT   cam_graph_rows    = float_buffer->GetRows();
		UINT   cam_graph_columns = float_buffer->GetColumns(); 
		FLOAT *cam_graph_buffer  = float_buffer->GetBuffer();


	// ---- set camera focus param
		UINT num_sample = 2;


	// ---- interpolate focus minimax to provide contextual handles

		/*
			------- focus_min_wh ( e.g. -10% using interpolant of -0.1 ) * hby2
			   |
			------- focus_min * hby2
			   |
			   |
			   |
			   |
			   |
			   |
			   |
			------- focus_max * hby2
			   |
			------- focus_max_wh ( e.g. +10% using interpolant of 1.1 ) * hby2

		*/


	// ---- interpolate focus min/max w(ith) h(andles)
		FLOAT int_min =  -0.1f;
		FLOAT int_max =   1.1f;
		FLOAT focus_min_wh = (1 - int_min) * focus_min + int_min * focus_max;
		FLOAT focus_max_wh = (1 - int_max) * focus_min + int_max * focus_max;
		focus_min_wh = focus_min_wh < 0.0f ? 0.0f : focus_min_wh;
		focus_max_wh = focus_max_wh > 1.0f ? 1.0f : focus_max_wh;


	// ---- minimax
		FLOAT ymin = focus_min_wh * hby2;
		FLOAT ymax = focus_max_wh * hby2;
		UINT i_y_min = (UINT)(ymin + 0.5);
		UINT i_y_max = (UINT)(ymax + 0.5);
		UINT i_y_num = i_y_max - i_y_min;


	// ---- set minimax ymin
		UINT  min_num_rows = cam_graph_columns * num_sample;
		if ( i_y_num < min_num_rows )
		{
			ymin = ymax - (FLOAT)min_num_rows;
			i_y_min = (UINT)(ymin + 0.5);
			i_y_num = i_y_max - i_y_min;
		}


	// ---- initialise accumulation buffer
		if ( focus_accum_run_time )
		{
			focus_accum_run_time = FALSE;
			focus_accum_buf_index = 0;
			focus_accum_buf_size  = 1024;
			focus_accum_buf_max   = 12;
			focus_accum_buffer->Initialise(focus_accum_buf_size, focus_accum_buf_max, 1);
				focus_accum_buffer->CreateBuffer();
				focus_accum_buffer->Set(0);
			focus_average_buffer->Initialise(focus_accum_buf_size, 1, 1);
				focus_average_buffer->CreateBuffer();
				focus_average_buffer->Set(0);
			focus_difference_buffer->Initialise(focus_accum_buf_size, 1, 1);
				focus_difference_buffer->CreateBuffer();
				focus_difference_buffer->Set(0);
		}


	// ---- luminace
		FLOAT *accum_buffer = focus_accum_buffer->GetBuffer(focus_accum_buf_index);
		INT stride = width * bpp;
		for ( UINT spoke = 0; spoke < cam_graph_rows; spoke++)
		{
			FLOAT mu    = cam_graph_rows == 1 ? 0.5f : (FLOAT)spoke / (cam_graph_rows - 1);
			FLOAT rot   = (1 - mu) * (-sector_by2) + mu * sector_by2 - rotate;
			FLOAT theta = rot* (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT sina  = -sin(theta);
			FLOAT cosa  =  cos(theta);
			FLOAT rx=0, ry=0;
			for (UINT row = 0; row < i_y_num; row++)
			{
				FLOAT y = (FLOAT)( row + i_y_min );
				rotit(sina, cosa, y, &rx, &ry);

				// TBD - catch out of bounds....

				INT   i  = (INT)(rx + 0.5)*bpp + (INT)(ry + 0.5)*stride;
				BYTE  r  = *(byte_data + i);
				BYTE  g  = *(byte_data + i + 1);
				BYTE  b  = *(byte_data + i + 2);
				accum_buffer[row] = (FLOAT)(r + r + g + g + g + b) / 6;
			}
		}
		focus_accum_buf_index = 
			focus_accum_buf_index + 1 > (focus_accum_buf_max - 1) ?
				0 : focus_accum_buf_index + 1;


	// ---- average
		FLOAT *average_buffer = focus_average_buffer->GetBuffer();
		focus_average_buffer->Set(0);
		for (UINT av = 0; av < focus_accum_buf_max; av++)
		{
			FLOAT *accum_buffer = focus_accum_buffer->GetBuffer(av);
			for (UINT row = 0; row < i_y_num; row++)
			{
				average_buffer[row] += accum_buffer[row];
			}
		}
		FLOAT f_accum_buf_max = (FLOAT)focus_accum_buf_max;
		for (UINT row = 0; row < i_y_num; row++)
		{
			average_buffer[row] /= f_accum_buf_max;
		}


	// ---- difference
		FLOAT *difference_buffer = focus_difference_buffer->GetBuffer();
		FLOAT max = 0;
		for (UINT row = 1; row < i_y_num; row++)
		{
			difference_buffer[row] = (average_buffer[row] - average_buffer[row - 1]);
			// ok - fabs the max value WHILST normalising....
			FLOAT fab = fabsf(difference_buffer[row]);
			max = fab > max ? fab : max;
		}
		difference_buffer[0] = difference_buffer[1];


	// ---- normalise difference
		FLOAT scale = 1.0f / max;
		for (UINT row = 0; row < i_y_num; row++)
		{
			// ok - .... BUT fabs the stored value here.....
			difference_buffer[row] = fabs(difference_buffer[row]) * scale;
			difference_buffer[row] = pow(difference_buffer[row], 2);
		}


	// ---- sample bars buffer from difference_buffer
		BOOL blah = FALSE;
		if (blah)
		{
			UINT  bar_prev = 0;
			FLOAT bar_incr = (FLOAT)cam_graph_columns / (FLOAT)i_y_num;
			FLOAT lacc  = 0; 
			UINT  lint  = 0;
			for (UINT row = 0; row < i_y_num; row++)
			{
				lacc += fabs(difference_buffer[row]) * 0.5f;
				lint++;
	
				UINT bar_index = (UINT)((FLOAT)row * bar_incr);
				if ( bar_prev != bar_index )
				{
					cam_graph_buffer[bar_prev] = lacc / (FLOAT)lint;
					bar_prev = bar_index;
					lacc = 0;
					lint = 0;
				}
			}
			if (lint != 0)
			{
				cam_graph_buffer[bar_prev] = lacc / (FLOAT)lint;
			}
		}


	// ---- sample bars buffer from difference_buffer
		{
			FLOAT diff_incr = (FLOAT)i_y_num / (FLOAT)cam_graph_columns;
			FLOAT bar_incr = (FLOAT)cam_graph_columns / (FLOAT)i_y_num;
			for (UINT cam_col = 0; cam_col < cam_graph_columns; cam_col++)
			{
				FLOAT dif_begin   = diff_incr * cam_col;
				FLOAT dif_end     = dif_begin + diff_incr;
				UINT  i_dif_begin = (UINT)dif_begin;
				UINT  i_dif_end   = (UINT)dif_end;
				 INT  l_dif_begin = (UINT)dif_begin + 1;
				 INT  l_dif_end   = (UINT)dif_end - 1;

				FLOAT b = dif_begin - (FLOAT)i_dif_begin;
				FLOAT e = dif_end   - (FLOAT)i_dif_end;

				FLOAT l_acc_b = (1 - b) * difference_buffer[i_dif_begin];
				FLOAT l_acc_e = difference_buffer[i_dif_end] *  e;
				FLOAT l_acc   = i_dif_begin == i_dif_end ?
								l_acc_e - l_acc_b:
									l_acc_b + l_acc_e;

				for (INT dif_col = l_dif_begin; dif_col <= l_dif_end; dif_col++)
				{
					l_acc += difference_buffer[dif_col];
				}
				cam_graph_buffer[cam_col] = l_acc * 0.25f;

				float fred = 2;
			}
		}


	// ---- paint
		for (FLOAT ang = -0.01f; ang < 0.01; ang += 0.02f)
		{
			FLOAT theta = (ang - rotate) * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT sina = -sin(theta);
			FLOAT cosa = cos(theta);
			FLOAT rx = 0, ry = 0;
			for (UINT iy = i_y_min; iy < i_y_max; iy++)
			{
				FLOAT y  = (FLOAT)iy;
				rotit(sina, cosa, y, &rx, &ry);
				INT   i  = (INT)(rx + 0.5)*bpp + (INT)(ry + 0.5)*stride;
				*(byte_data + i + 0) = 255;
				*(byte_data + i + 1) = 0;
				*(byte_data + i + 2) = 0;
			}
		}


	// ---- paint
		float mu[] =
			{
				0.0f,
				((focus_min*hby2) - ymin) / (ymax - ymin),
				((focus_max*hby2) - ymin) / (ymax - ymin),
				1.0f
			};
		for (FLOAT ang = -2; ang < 2; ang+=0.025f)
		{
			FLOAT theta = (ang - rotate) * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT sina = -sin(theta);
			FLOAT cosa = cos(theta);
			FLOAT rx = 0, ry = 0;
			for (UINT iy = 0; iy < 4; iy++)
			{
				FLOAT y  = (1 - mu[iy]) * ymin + mu[iy] * ymax;
				rotit(sina, cosa, y, &rx, &ry);
				INT   i  = (INT)(rx + 0.5)*bpp + (INT)(ry + 0.5)*stride;
				*(byte_data + i + 0) = 255;
				*(byte_data + i + 1) = 0;
				*(byte_data + i + 2) = 0;
			}
		}


	return VS_OK;
}
