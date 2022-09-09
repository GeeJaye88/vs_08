////////////////////////////////////////////////////////////////////////////////

// ---------- s3_portal_01.cpp ----------
/*!
\file s3_portal_01.cpp
\brief Implementation of a sub-application class
\author Gareth Edwards
\note
*/

#include "../header/s3_surface_03.h"


using namespace model;


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_surface_03/s3_pi.hpp"
#include "../hpp_surface_03/s3_pi_configuration.hpp"
#include "../hpp_surface_03/s3_param_interface.hpp"
#include "../hpp_surface_03/s3_scene_setup.hpp"
#include "../hpp_surface_03/s3_param_setup.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Surface_03::Surface_03()
{

	// ---- allocate and construct pimp
		pi_surface_03 = new PI_Surface_03();

	// ---- local
		INT result = VS_OK;
		INT offset = 0;

	// ---- setup target stuff
		InitTargetLoi();

	// --- sub-framework setup
		result = SetupParamAcquire(&offset);    // acquire - lighting & calculation
		result = SetupParamCamera(&offset);     // acquire - camera setup & calibration
		result = SetupParamHeightMap(&offset);  // acquire - not used
		result = SetupParamLights(&offset);     // explore - interactive lighting
		result = SetupParamMeasure(&offset);    // explore - measurement stuff
		result = SetupParamFunction(&offset);   // acquire - functional surface

	// ---- leave as zero
		result = parameter.SetParamGroup(surface_03::Model::PARAM_GROUP_NULL);

}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Surface_03::~Surface_03()
{
	delete pi_surface_03;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- framework ----------

// Function to round an int to a multiple of 4
int round4(int x) {
    return x % 4 == 0 ? x : x - x % 4 + 4;
}


void write_bmp(char *filename, char rgb[], int length, int width) {
    int height = (length / 3) / width;

    // Pad the width of the destination to a multiple of 4
    int padded_width = round4(width * 3);
    
    int bitmap_size = height * padded_width * 3;
    char *bitmap = (char *) malloc(bitmap_size * sizeof(char));
    for (int i = 0; i < bitmap_size; i++) bitmap[i] = 0;

    // For each pixel in the RGB image...
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            
            // For R, G, and B...
            for (int color = 0; color < 3; color++) {

                // Get the index of the destination image
                int index = row * padded_width + col * 3 + color;

                // Set the destination to the value of the src at row, col.
                bitmap[index] = rgb[3*(row * width + col) + (2 - color)];
            }
        }
    }

    char tag[] = { 'B', 'M' };
    int header[] = {
        0, 0, 0x36, 0x28, width, height, 0x180001, 
        0, 0, 0x002e23, 0x002e23, 0, 0
    };
    header[0] = sizeof(tag) + sizeof(header) + bitmap_size;
    FILE *fp = fopen(filename, "w+");
    fwrite(&tag, sizeof(tag), 1, fp);
    fwrite(&header, sizeof(header), 1, fp);
    fwrite(bitmap, bitmap_size * sizeof(char), 1, fp);
    fclose(fp);
    free(bitmap);
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = 0;

	// ---- store dependency pointers
		pi_surface_03->app_dependency = app_dep;
		pi_surface_03->gfx            = app_dep->GetGfxInterface();
		pi_surface_03->sys_context    = app_dep->GetSysContext();
		pi_surface_03->sys_output     = app_dep->GetSysOutput();


	// ---- BITMAP

	// ---- local
		const char* source_file = "../220801 - small_creasing_001_12_bit.bmp";
		const char* dest_12_file = "../test_001_12_bit.bmp";
		const char* dest_8_file = "../test_001_8_bit.bmp";

		#define BMP_BPP UINT16

	// ----go...
		try
		{

			// ---- file... we open it with it's constructor
				std::ifstream s_file(source_file, std::ios::binary);
				if (!s_file) throw (1);

			// ---- bitmap header & info
				BITMAPFILEHEADER* bmpHeader = nullptr;
				BITMAPINFOHEADER* bmpInfo = nullptr;

			// ==== READ ====

			// ---- header buffers
				BMP_BPP * datBuff[2] = { nullptr, nullptr };

			// ---- allocate byte memory that will hold the two headers
				datBuff[0] = new BMP_BPP[sizeof(BITMAPFILEHEADER)];
				datBuff[1] = new BMP_BPP[sizeof(BITMAPINFOHEADER)];

				s_file.read((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
				s_file.read((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

			// ---- construct the values from the buffers
				bmpHeader = (BITMAPFILEHEADER*)datBuff[0];
				bmpInfo   = (BITMAPINFOHEADER*)datBuff[1];

			// ---- check if the file is an actual BMP file
				if (bmpHeader->bfType != 0x4D42) throw (2);

			// ---- check mono
				if (bmpInfo->biPlanes != 1) throw (3);

			// ---- check 16 bits
				if (bmpInfo->biBitCount != 16) throw (4);

			// ==== PROCESS ====

			// ---- allocate pixel memory
				BMP_BPP * pixels_bpp = nullptr;
				pixels_bpp = new BMP_BPP[bmpInfo->biSizeImage];

			// ---- go to where image data starts, then read in image data
				s_file.seekg(bmpHeader->bfOffBits);
				s_file.read((char*)pixels_bpp, bmpInfo->biSizeImage);

			// ---- note RGB is BGR

			// ---- minimax
				UINT num_pixels = bmpInfo->biHeight * bmpInfo->biWidth;
				BMP_BPP mono_min = 4096;
				BMP_BPP mono_max = 0;
				for (unsigned long i = 0; i < num_pixels; i++)
				{
					mono_min = mono_min < pixels_bpp[i] ? mono_min : pixels_bpp[i];
					mono_max = mono_max > pixels_bpp[i] ? mono_max : pixels_bpp[i];
				}
				FLOAT mono_dif = (FLOAT)(mono_max - mono_min);

			// ---- stretch
				BMP_BPP stretch_min = 256;
				BMP_BPP stretch_max = 4096;
				FLOAT stretch_dif = (FLOAT)(stretch_max - stretch_min);
				for (unsigned long i = 0; i < num_pixels; i++)
				{
					FLOAT m = (FLOAT)(pixels_bpp[i] - mono_min) / mono_dif;  // range 0 < m < 1
					FLOAT h = m * stretch_dif + stretch_min;
					BMP_BPP v = (BMP_BPP)( h );
					pixels_bpp[i] = v-1;
				}

				//mono_min = 4096;
				//mono_max = 0;
				//for (unsigned long i = 0; i < num_pixels; i ++)
				//{
				//	mono_min = mono_min < pixels_bpp[i] ? mono_min : pixels_bpp[i];
				//	mono_max = mono_max > pixels_bpp[i] ? mono_max : pixels_bpp[i];
				//}

			// ---- set width and height to the values loaded from the file
				UINT w = bmpInfo->biWidth;
				UINT h = bmpInfo->biHeight;

			// ---- save original 12 bit file
				{
					// ---- create file
						std::ofstream d_file(dest_12_file, std::ios::binary);
						if (!d_file) throw (5);

					// ---- two headers
						d_file.write((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
						d_file.write((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

					// ---- pad to 66!
						union pad { UINT p; BYTE b[4] = { 255,15,0,0 }; };
						UINT  pad_size = bmpHeader->bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
						pad  *pad_buffer = new pad[pad_size / 4];
						d_file.write((char*)pad_buffer, pad_size);

					// ---- write image & close
						d_file.write((char*)pixels_bpp, bmpInfo->biSizeImage);
						d_file.close();

				}

			// ---- save new 8 bit file
				for (UINT section = 1; section <=16; section++)
				{
					// ---- create file
						std::string dest_8_name = "../sect_";
						if (section < 4) dest_8_name += "0";
						dest_8_name += std::to_string(section*256);
						dest_8_name += ".bmp";

						std::ofstream d_file(dest_8_name, std::ios::binary);
						if (!d_file) throw (5);

					// ---- initialise the two headers
						bmpInfo->biSize          = 40;
						bmpInfo->biWidth         = 1280;
						bmpInfo->biHeight        = 1024;
						bmpInfo->biPlanes        = 1;
						bmpInfo->biBitCount      = 8;
						bmpInfo->biCompression   = 0;
						bmpInfo->biSizeImage     = bmpInfo->biWidth * bmpInfo->biHeight;
						bmpInfo->biXPelsPerMeter = 0;
						bmpInfo->biYPelsPerMeter = 0;
						bmpInfo->biClrUsed       = 256;
						bmpInfo->biClrImportant  = 256;

						bmpHeader->bfType        = 0x4D42; //19788; 
						bmpHeader->bfReserved1   = 0;
						bmpHeader->bfReserved2   = 0;
						bmpHeader->bfOffBits     = 1078;
						bmpHeader->bfSize        = bmpInfo->biHeight * bmpInfo->biWidth + bmpHeader->bfOffBits;

					// ---- write two headers
						d_file.write((char*)datBuff[0], sizeof(BITMAPFILEHEADER));
						d_file.write((char*)datBuff[1], sizeof(BITMAPINFOHEADER));

					// ---- pad to 1078! with RGB LUT + 54
						UINT pad_size = bmpHeader->bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
						BYTE *pad_buffer = new BYTE[pad_size];
						for (UINT i = 0; i < 256; i++)
						{
							UINT index = i * 4;
							*(pad_buffer + index )  = i;
							*(pad_buffer + index+1) = i;
							*(pad_buffer + index+2) = i;
							*(pad_buffer + index+3) = i;
						}
						d_file.write((char*)pad_buffer, pad_size);

					// ---- allocate pixel memory
						UINT8 * pixels_8 = nullptr;
						pixels_8 = new UINT8[bmpInfo->biSizeImage];

					// section minimax
						BMP_BPP section_min = (section-1) * 256;
						BMP_BPP section_max = section_min + 256;

					// ---- copy slice from source
						UINT line_offset = 0;
						for (UINT line = 0; line < (UINT)bmpInfo->biHeight; line++)
						{
							UINT v = line % 256;
							line_offset = line * bmpInfo->biWidth;
							for (UINT pixel = 0; pixel < (UINT)bmpInfo->biWidth; pixel++)
							{
								UINT index = line_offset + pixel;

								// all
								// v = (UINT8)( (FLOAT)( pixels_bpp[index]) / 4096 * 255 );

								// for visible section, change "? section_max-1 :" to "? section_max :"
								BMP_BPP val = pixels_bpp[index] < section_min ? section_min :
									( pixels_bpp[index] > section_max ? section_max : pixels_bpp[index] - section_min);

								v = (UINT8)(val);
 								pixels_8[index] = v;
							}
						}

					// ---- write & delete image, & then close file
						d_file.write((char*)pixels_8, bmpInfo->biSizeImage);
						delete[] pixels_8;
						d_file.close();

				}

				float fred = 2;
				exit(0);

			// ----

		}
		catch (int error_num)
		{

			char *error_msg;
			switch (error_num)
			{
				case 1: error_msg = "failure to open bitmap file."; break;
				case 2: error_msg = "not a bitmap format file\n"; break;
				case 3: error_msg = "not a mono format file\n"; break;
				case 4: error_msg = "not a 16 bit format file\n"; break;
				default: error_msg = "unknown"; break;
			}

			//std::cout << "file \"" << source_file << "\": error # " << error_num  << " - " << error_msg << std::endl;

			std::cout << "file \"" << source_file << std::endl;

			std::string bert = "--> source file ";
			bert += source_file;
			bert += ":";
			bert += "\n";

			OutputDebugString(bert.c_str());

			//return error_num;
			//exit(error_num);

			float fred = 2;

			/*

			std::cout << "file \"" << source_file << std::endl;

			std::string bert = "--> source file ";
			bert += source_file;
			bert += ":";
			bert += "\n";
			
			
			*/
		}

	return VS_OK;
}


// ---------- framework / SetupGraphics ----------
/*!
\brief SetupGraphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetupGraphics(vs_system::AppKamera *kamera)
{

	// ---- local
		INT result = VS_OK;


	// ---- store param group
		INT param_group = parameter.GetParamGroup();


	// ---- scene
		result = SetupExploreCamera(0, kamera);
		result = SetupAcquireLight();
		result = SetupExploreLighting(1);
		result = SetupExploreMaterials(0);
		result = SetupExploreMaterials(1);


	// ---- restore group
		parameter.SetParamGroup(param_group);


	// ---- setup "the" display list
		result = SetupDisplayList();


	// ---- setup objects
		vs_system::GfxInterface *gfx = pi_surface_03->gfx;
		for (int i = 0; i < PI_Surface_03::MAX_GFX_OBJ_LIST; i++)
		{
			PI_Surface_03::GfxObjectItem *gfx_obj =
				&pi_surface_03->gfx_object_list[i];

			if ( gfx_obj->setup != NULL )
			{
				if ( gfx_obj->multi_element )
				{
					for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
					{
						result = gfx_obj->setup(
								i,
								this
							);
					}
				}
				else
				{
					result = gfx_obj->setup(
							gfx_obj->index,
							this
						);
				}
			}
		}


	// ---- state
		pi_surface_03->runtime = FALSE;


	// ---- update target loi
		INT tid = GetTargetLoiIndex();
		UpdateLoiObjects(tid, this);

	return VS_OK;
}


// ---------- framework / Display ----------
/*!
\brief SetupDisplay
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetupDisplay(
		vs_system::AppKamera *kamera
	)
{

	// ---- local
		INT result = 0;
		VsVector3 translate;
		FLOAT rx, ry, rz;


	// ---- get stuff
		kamera->GetTranslate(&translate);
		kamera->GetRotate(&rx, &ry, &rz);


	// ---- push
		pi_surface_03->matrix_stack->Push();


	// ---- display objects
		for (int i = 0; i < PI_Surface_03::MAX_GFX_OBJ_LIST; i++)
		{

			PI_Surface_03::GfxObjectItem *gfx_obj =
				&pi_surface_03->gfx_object_list[i];

			if ( gfx_obj->setup != NULL && gfx_obj->display != NULL )
			{

				if  ( gfx_obj->display != ObjectExpSpot_Display )
				{
					pi_surface_03->matrix_stack->Push();
					pi_surface_03->matrix_stack->Rotate(rz, 'Z');
					pi_surface_03->matrix_stack->Rotate(rx, 'X');
					pi_surface_03->matrix_stack->Rotate(ry, 'Y');
					pi_surface_03->matrix_stack->Translate(
									translate.x,
									translate.y,
									translate.z
								);

					result = gfx_obj->display(
								gfx_obj->index,
								this,
								gfx_obj->visible
							);

					pi_surface_03->matrix_stack->Pop();
				}
				else
				{
					if ( gfx_obj->multi_element )
					{
						for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
						{
							result = gfx_obj->display(
									i,
									this,
									gfx_obj->visible
								);
						}
					}
					else
					{
						result = gfx_obj->display(
								gfx_obj->index,
								this,
								gfx_obj->visible
							);
					}
				}

			}

		}

	// ---- pop
		pi_surface_03->matrix_stack->Pop();

	// ---- hmmmm....

	return VS_OK;
}


// ---------- framework / CleanupGraphics ----------
/*!
\brief CleanupGraphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::CleanupGraphics()
{
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- sub-framework ----------


// ---------- Display3DText ----------
/*!
\brief display dx 3D text
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::Display3DText(vs_system::AppKamera *kamera)
{
	
	// ---- local
		INT result = 0;
		if ( result == 0 ) return VS_OK;
		VsVector3 translate;
		FLOAT rx, ry, rz;

	// ---- get stuff
		kamera->GetTranslate(&translate);
		kamera->GetRotate(&rx, &ry, &rz);

	// ---- push
		pi_surface_03->matrix_stack->Push();

		// ---- transform
			pi_surface_03->matrix_stack->Rotate(rz, 'Z');
			pi_surface_03->matrix_stack->Rotate(rx, 'X');
			pi_surface_03->matrix_stack->Rotate(ry, 'Y');
			pi_surface_03->matrix_stack->Translate(translate.x, translate.y, translate.z);


		// ---- dx fonts
			#if defined(CD3DXFONT)


				CD3DFont *dx_font = pi_surface_03->gfx->GetDxFont(7);

				SIZE textFS;
				dx_font->GetTextExtent("Gareth", &textFS);

				DWORD color = D3DCOLOR_COLORVALUE(1, 0, 0, 0.5f);
				dx_font->Render3DText("Gareth", color, D3DFONT_FILTERED | D3DFONT_TWOSIDED);

			#endif	return VS_OK;

	// ---- pop
		pi_surface_03->matrix_stack->Pop();

	return VS_OK;
}


// ---------- SetObjConfig ----------
/*!
\brief Set pointer to object configuration object
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetObjConfig(vs_system::ObjConfig *oc)
{
	pi_surface_03->obj_config = oc;
	return VS_OK;
}


// ---------- SetImgProps ----------
/*!
\brief Set pointer to object configuration object
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetImgProps(vs_system::ImgProps *ip)
{
	pi_surface_03->img_props = ip;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupDisplayList ----------
/*!
\brief
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetupDisplayList()
{

	if ( pi_surface_03->runtime )
	{
		vs_system::GfxInterface *gfx = pi_surface_03->gfx;
		pi_surface_03->scene_dspl_list_handle = gfx->AddDisplayList("Scene", 1, 1);
	}

	return VS_OK;
}


// ---------- GetDisplayListHandle ----------
/*!
\brief
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::GetDisplayListHandle(INT *handle, CHAR *name)
{
	*handle = pi_surface_03->scene_dspl_list_handle;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


VOID Surface_03::SetRRBumpMapIndex(UINT index)
{
	pi_surface_03->rr_bump_map_index = index;
}

UINT Surface_03::GetRRBumpMapIndex()
{
	return pi_surface_03->rr_bump_map_index;
}

INT Surface_03::GetRRBumpMapData(UCHAR **data, INT *extent)
{
	*data = pi_surface_03->rr_bump_map[pi_surface_03->rr_bump_map_index]->heightfield_data;
	*extent = pi_surface_03->rr_bump_map[pi_surface_03->rr_bump_map_index]->heightfield_extent;
	return VS_OK;
}

INT Surface_03::GetTerainBumpMapData(UCHAR **data, INT *extent)
{
	*data = pi_surface_03->terrain_bump_map->heightfield_data;
	*extent = pi_surface_03->terrain_bump_map->heightfield_extent;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT Surface_03::SetVisibleObjects(INT body_id, Surface_03* p_s_03)
{

	// --- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- lambda


	// ---- ACQUIRE
		auto set_aquire = [pimp]()
		{

			// -+- fix loi stuff -+-
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SPOT_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_AXIS_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RECT_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RULE_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SECT_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_ANNO_IDX, 0);

			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_0_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_1_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_2_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_3_IDX, 0);
		};


	// ---- ANALYSIS
		auto set_explore = [pimp]()
		{
			// -+- fix loi stuff -+-
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SPOT_IDX, pimp->_configuration.target_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_AXIS_IDX, pimp->_configuration.target_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RECT_IDX, pimp->_configuration.target_ruler_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RULE_IDX, pimp->_configuration.target_ruler_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SECT_IDX, pimp->_configuration.target_section_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_ANNO_IDX, pimp->_configuration.target_visible ||
																		pimp->_configuration.target_ruler_visible ||
																		pimp->_configuration.target_section_visible);

			pimp->SetObjectVisible(PI_Surface_03::GOB_ACQ_LGT_RIG_IDX, 0);
		};


	// ---- MESH
		auto set_mesh = [pimp](BOOL visible)
		{
			pimp->SetObjectVisible(PI_Surface_03::GOB_SURF_MESH_IDX, visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_SURF_BASE_IDX, visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_SURF_EDGE_IDX, visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_SURF_AXIS_IDX, visible);
		};


	// ---- MEASURE
		auto set_loi = [pimp]()
		{
			pimp->SetObjectVisible(PI_Surface_03::GOB_SURF_BASE_IDX, 1);
			pimp->SetObjectVisible(PI_Surface_03::GOB_SURF_AXIS_IDX, 1);

			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_0_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_1_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_2_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_3_IDX, 0);

			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_0_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_1_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_2_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_3_IDX, 0);

			// -+- fix loi stuff -+-
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SPOT_IDX, pimp->_configuration.target_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_AXIS_IDX, pimp->_configuration.target_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RECT_IDX, pimp->_configuration.target_ruler_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RULE_IDX, pimp->_configuration.target_ruler_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SECT_IDX, pimp->_configuration.target_section_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_ANNO_IDX, pimp->_configuration.target_visible ||
																		pimp->_configuration.target_ruler_visible ||
																		pimp->_configuration.target_section_visible);

			pimp->SetObjectVisible(PI_Surface_03::GOB_ACQ_LGT_RIG_IDX, 0);
		};


	// ---- all on
		pimp->SetObjectsVisible();

	// ---- select off
		switch (body_id)
		{
			case TINT_BODY_ACQUIRE_LID:
			case TINT_BODY_CALIBRATE_LID:
			case TINT_BODY_FUNCTION_LID:
				set_aquire();
				set_mesh(TRUE);
				break;

			case TINT_BODY_LIGHT_LID:
			case TINT_BODY_EXPLORE_LID:
			case TINT_BODY_ENHANCE_LID:
				set_explore();
				set_mesh(TRUE);
				break;

			case TINT_BODY_MEASURE_LID:
				set_explore();
				set_mesh(FALSE);
				set_loi();
				break;

			case TINT_BODY_SETTINGS_LID:
				set_explore();
				set_mesh(TRUE);
				break;

			default:
				return VS_ERROR;
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_obj/s3_obj_heightfield.hpp"

#include "../hpp_obj/s3_obj_acq_lgt_axis.hpp"

#include "../hpp_obj/s3_obj_surf_axis.hpp"
#include "../hpp_obj/s3_obj_surf_base.hpp"
#include "../hpp_obj/s3_obj_surf_edge.hpp"
#include "../hpp_obj/s3_obj_surf_mesh.hpp"

#include "../hpp_obj/s3_obj_exp_lgt_axis.hpp"
#include "../hpp_obj/s3_obj_exp_lgt_spot.hpp"

// -+- fix loi stuff -+-

#include "../hpp_obj/s3_obj_loi_spot.hpp"
#include "../hpp_obj/s3_obj_loi_rect.hpp"
#include "../hpp_obj/s3_obj_loi_rule.hpp"
#include "../hpp_obj/s3_obj_loi_axis.hpp"
#include "../hpp_obj/s3_obj_loi_sect.hpp"
#include "../hpp_obj/s3_obj_loi_anno.hpp"


////////////////////////////////////////////////////////////////////////////////
