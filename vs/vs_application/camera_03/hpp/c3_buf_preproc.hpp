////////////////////////////////////////////////////////////////////////////////

// ---------- c3_buf_preproc.hpp ----------
/*!
\file c3_buf_preproc.hpp
\brief 
\author Gareth Edwards
*/


// ---------- PreGPUProcess ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT PI_Props::PreGPUProcess(WORD w, WORD h, WORD bpp, BYTE *byte_data)
{

#	ifdef VS_CAM_IDS_VIDEO_SDK

	UINT  total = w * h;
	BYTE *src = byte_data;
	BYTE *dest = dst_img_buffer->GetBuffer();
	for (UINT p = 0; p<total; p++)
	{
		*dest       = *src;        // Copy R = R
		*(dest + 1) = *(src + 1);  //      G = G
		*(dest + 2) = *(src + 2);  //      B = B
		*(dest + 3) = 0;           // Alpha  = 0
		dest += 4;
		src += bpp;
	}

#	else

	// hey - wtf - this is the same as above - hmmmmm... 
	UINT  total = w * h;
	BYTE *src = byte_data;
	BYTE *dest = dst_img_buffer->GetBuffer();
	for (UINT p = 0; p<total; p++)
	{
		*dest = *src;              // Copy R = R
		*(dest + 1) = *(src + 1);  //      G = G
		*(dest + 2) = *(src + 2);  //      B = B
		*(dest + 3) = 0;           // Alpha  = 0
		dest += 4;
		src += bpp;
	}

#	endif
	
	return VS_OK;
}
