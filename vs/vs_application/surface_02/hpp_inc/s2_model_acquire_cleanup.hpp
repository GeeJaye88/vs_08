////////////////////////////////////////////////////////////////////////////////

// ---------- s2_model_acquire_cleanup.hpp ----------
/*!
\file s2_model_acquire_cleanup.hpp
\brief 
\author Gareth Edwards
*/


// ==== LOCK ====


// ---- copy
	memcpy(
			(VOID *)(ext_dest_buffer_char + tile_offset),
			(VOID *)dest_buffer,
			tile_extent
		);


// ==== UNLOCK ====


// ---- cleanup
	delete[] src_buffer;
	delete[] dest_buffer;