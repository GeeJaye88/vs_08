////////////////////////////////////////////////////////////////////////////////

// ---------- s2_model_acquire_setup.hpp ----------
/*!
\file s2_model_acquire_setup.hpp
\brief 
\author Gareth Edwards
*/



// ==== LOCK ====


// ----- extern context
	UINT ext_line_start   = img_data_transfer->line_start;
	UINT ext_line_height  = img_data_transfer->line_height;


// ---- extern parameters
	FLOAT ext_param_array_values[9];
	memcpy((VOID *)ext_param_array_values, (VOID *)img_data_transfer->param_array_values, sizeof(FLOAT) * 9);


// ---- extern src
	vs_system::ImgBuffer *ext_src_img_buffer = img_data_transfer->src_img_buffer;
	UINT ext_src_buffer_width  = ext_src_img_buffer->GetWidth();
	UINT ext_src_buffer_pitch  = ext_src_img_buffer->GetPitch();
	UCHAR *ext_src_buffer_char = ext_src_img_buffer->GetBuffer();


// --- local tile
	UINT tile_height  = ext_line_height;
	UINT tile_extent  = tile_height    * ext_src_buffer_pitch;
	UINT tile_offset  = ext_line_start * ext_src_buffer_pitch;


// ---- local src
	UCHAR *src_buffer = new UCHAR[tile_extent];
	memcpy(
			(VOID *)src_buffer,
			(VOID *)(ext_src_buffer_char + tile_offset),
			tile_extent
		);


// ---- local dest
	UCHAR *ext_dest_buffer_char = img_data_transfer->dest_buffer_char;
	UCHAR *dest_buffer = new UCHAR[tile_extent];
	memcpy(
			(VOID *)dest_buffer,
			(VOID *)(ext_dest_buffer_char + tile_offset),
			tile_extent
		);


// ==== UNLOCK ====