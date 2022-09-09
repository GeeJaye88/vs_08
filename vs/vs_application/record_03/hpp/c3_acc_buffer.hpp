////////////////////////////////////////////////////////////////////////////////


// ---------- AccBufferInitialise ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::PI_Props::AccBufferInitialise(WORD w, WORD h, WORD bpp)
{
	// Local
	BOOL create_buffer = FALSE;

	// IF NO accumulate image buffer
	if ( acc_img_buffer == NULL )
	{
		acc_img_buffer = new vs_system::ImgBuffer();
		create_buffer = TRUE;
	}
	// ELSE IF wrong dimensions
	else if ( acc_img_buffer->GetWidth() != w )
	{
		acc_img_buffer->DestroyBuffer();
		create_buffer = TRUE;
	}

	// IF zero buffer THEN
	if ( create_buffer )
	{
		acc_img_buffer->Initialise(w, h, bpp);
		acc_img_buffer->CreateBuffer();
		BYTE *acc = acc_img_buffer->GetBuffer();
		memset(acc, 1, acc_img_buffer->GetExtent());
		return VS_OK;
	}

	// Done
	return VS_ERROR;
}


// ---------- AccBufferAll ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::PI_Props::AccBufferAll(WORD w, WORD h, WORD bpp, BYTE *byte_data)
{
	// Initialise grey scale look up table
	BYTE gamma_lut[256];
	for ( UINT i = 0; i < 256; i++ )
	{
		gamma_lut[i] =
			(BYTE)round(
				255.0 *
				pow((FLOAT)i /
				255.0, 1.0 /

#				ifdef	VS_IR_ACCUMULATION_BUFFER
				((FLOAT)(pi_props->ir_gamma) / 100.0f)
#				else
				(1.0f)
#				endif

				)
			);
	}

	// TBD - Assumes all dimensions match!!!

	// FOR every pixel
	BYTE *src  = byte_data;
	UINT  pmax = w * h;
	BYTE *acc  = acc_img_buffer->GetBuffer();
	BYTE *dest = dst_img_buffer->GetBuffer();
	for ( UINT p = 0; p < pmax; p++ )
	{
		INT luminance = (INT)
			(
				(
				(FLOAT)(*src + 0) * 0.299 +
				(FLOAT)(*src + 1) * 0.587 +
				(FLOAT)(*src + 2) * 0.114
				) +

#				ifdef	VS_IR_ACCUMULATION_BUFFER
				(FLOAT)(*acc + 0) * pi_props->ir_acc_factor + 0.5
#				else
				1.0f
#				endif

			);

		BYTE max_luminance = gamma_lut[(BYTE)(luminance > 255 ? 255 : luminance)];
		memset(acc, max_luminance, 3);
		memset(dest, max_luminance, 3);
		*(dest + 3) = 0;
		acc += bpp;

#		if defined (VS_CAM_JPLY_DVP_SDK) || defined (VS_CAM_IDS_VIDEO_SDK)
		* src     = *(dest+2); // Swap R = B
		*(src+2)  = *dest;     //      B = R
#		endif

		dest += 4;
		src += bpp;
	}

	// Done
	return VS_OK;
}


// ---------- AccBufferPano ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::PI_Props::AccBufferPano(WORD w, WORD h, WORD bpp, BYTE *byte_data)
{
	// TBD - Assumes all dimensions match!!!

	// Accelerate panoramic parameters
	FLOAT u_offset = 0;
	FLOAT v_offset = 0;
	INT result = model->GetPanoUVOffset(&u_offset, &v_offset);

	// Create a luminance buffer
	vs_system::ImgBuffer lumin_src_img_buffer;
	lumin_src_img_buffer.Initialise(w, h, bpp);
	lumin_src_img_buffer.SetBuffer(byte_data);

	// Accumulate luminance into accumulate buffer
	vs_system::ImgProcess img_process;
#	ifdef	VS_IR_ACCUMULATION_BUFFER
	img_process.PanoLuminance(
			acc_img_buffer,
			0,
			v_offset,
			pi_props->ir_acc_gamma,
			pi_props->ir_acc_factor,
			pi_props->ir_acc_radius_min,
			pi_props->ir_acc_radius_max,
			&lumin_src_img_buffer
		);
#	else
	img_process.PanoLuminance(
			acc_img_buffer,
			0,
			v_offset,
			1.0f,
			1.0f,
			0.5f,
			1.0f,
			&lumin_src_img_buffer
		);
#	endif

	// FOR every pixel
	UINT  pmax = w * h;
	BYTE *src  = acc_img_buffer->GetBuffer();
	BYTE *dest = dst_img_buffer->GetBuffer();
	for ( UINT p = 0; p < pmax; p++ )
	{
		*dest = *src;              // Copy R = R 
		*(dest + 1) = *(src + 1);  //      G = G
		*(dest + 2) = *(src + 2);  //      B = B
		*(dest + 3) = 0;           // Alpha  = 0

#		if defined (VS_CAM_JPLY_DVP_SDK) || defined (VS_CAM_IDS_VIDEO_SDK)
		* src     = *(dest+2);     // Swap R = B
		*(src+2)  = *dest;         //      B = R
#		endif

		dest += 4;
		src += bpp;
	}

	// Done
	return VS_OK;
}


// ---------- FormatDestBuffer ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::PI_Props::FormatDestBuffer(WORD w, WORD h, WORD bpp, BYTE *byte_data)
{
	// Copy, add alpha and swap
	UINT  pmax = w * h;
	BYTE *src = byte_data;
	BYTE *dest = dst_img_buffer->GetBuffer();
	for ( UINT p = 0; p < pmax; p++ )
	{
		// Note: memcpy(dest, src, 3); is slower!
	   *dest = *src;              // Copy R = R
		*(dest + 1) = *(src + 1);  //      G = G
		*(dest + 2) = *(src + 2);  //      B = B
		*(dest + 3) = 0;           // Alpha  = 0

#		if defined (VS_CAM_JPLY_DVP_SDK) || defined (VS_CAM_IDS_VIDEO_SDK)
		* src     = *(dest+2);     // Swap R = B
		*(src+2)  = *dest;         //      B = R
#		endif

		dest += 4;
		src += bpp;
	}

	// Done
	return VS_OK;
}


// ---------- FormatRGBABuffer ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::PI_Props::FormatRGBABuffer(WORD w, WORD h, BYTE *byte_data)
{
	// Copy and add alpha
	UINT  pmax = w * h;
	BYTE *src = byte_data;
	BYTE *dest = dst_img_buffer->GetBuffer();
	for ( UINT p = 0; p < pmax; p++ )
	{
		*dest = *src;              // Copy R = R 
		*(dest + 1) = *(src + 1);  //      G = G
		*(dest + 2) = *(src + 2);  //      B = B
		*(dest + 3) = 0;           // Alpha  = 0
		dest += 4;
		src += 3;
	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////