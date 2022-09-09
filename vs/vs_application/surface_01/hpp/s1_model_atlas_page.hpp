////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_atlas_page.hpp ----------
/*!
\file s1_model_atlas_page.hpp
\brief Implementation of the atlas page methods
\author Gareth Edwards
*/


// ========================================================================== //


INT Model::InitAtlasPage()
{

	// ---- local
		INT result = VS_OK;


	#if defined(S1_EXPLODED_CUBE) || defined(S1_WAVEFRONT_OBJ)


	// ---- filename
		BOOL development = pi_model->sys_context->GetBool(vs_system::SysContext::DEVELOPMENT);
		CHAR *filename = "";
		if (development)
			filename = "..\\pix\\baboon.jpg";
		else
			filename = "pix\\baboon.jpg";


	// ---- get image size
		vs_system::ImgJpegLib img_jpeglib;
		vs_system::ImgBuffer img_buffer_3;
		img_jpeglib.GetHeaderInfo(&img_buffer_3, filename);


	// ---- get texture logical id
		pi_model->user_atlas_page_logical_id = pi_model->gfx->GetAtlasPageLogicalId();


	// ---- setup texture
		std::string s = filename;
		result = pi_model->gfx->PageSetup(pi_model->user_atlas_page_logical_id, s, &img_buffer_3);


	// ---- read 3 byte image
		result = img_buffer_3.CreateBuffer();
		result = img_jpeglib.Read(&img_buffer_3, filename);


	// --- convert to 4 byte
		UINT height = img_buffer_3.GetHeight();
		UINT width = img_buffer_3.GetWidth();
		vs_system::ImgBuffer img_buffer_4;
		img_buffer_4.Initialise(width, height, 4);
		img_buffer_4.CreateBuffer();
		BYTE *src_3  = (BYTE *)img_buffer_3.GetBuffer();
		BYTE *dest_4 = (BYTE *)img_buffer_4.GetBuffer();
		for (UINT y = 0; y<height; y++)
		{
			for (UINT x = 0; x<width; x++)
			{
				*(dest_4    ) = *(src_3 + 2);
				*(dest_4 + 1) = *(src_3 + 1);
				*(dest_4 + 2) = *(src_3 + 0);
				*(dest_4 + 3) = 255;
				dest_4 += 4;
				src_3 += 3;
			}
		}


	// ---- add texture
		result = pi_model->gfx->AddPageToAtlasFromImgBuffer(
				pi_model->user_atlas_page_logical_id,
				&img_buffer_4,
				FALSE
			);


	// ---- cleanup
		img_buffer_3.DestroyBuffer();
		img_buffer_4.DestroyBuffer();


	#endif

	return VS_OK;
}


// ========================================================================== //