////////////////////////////////////////////////////////////////////////////////

// ---------- vs_heightfield.h ----------

/*!
\file vs_heightfield.h
\brief HeightField class interface.
\note Created by app_setup, an application setup utility.
*/

#pragma once


// ---- vs system
#include "../../vs_system/header/vs_inc_defs.h"


// ---- app
#include "vs_img_media.h"


// ---- windows
#include <fstream>


// ---- namespace ----
namespace vs_system
{

	// ---------- Heightfield class ----------
	class Heightfield
	{

		public:

		// ---- cdtor
			Heightfield();
			virtual ~Heightfield();


		// ---- access
			static BOOL Get(
				CHAR    *raw_file_name,
				UCHAR  **heightfield,
				INT     *num_vertices
			);

			static BOOL Get(
				UCHAR  **heightfield,
				INT     *num_vertices
			);

		// --- build
			INT Flatfield2D(
				FLOAT  *dest,
				INT     dest_stride
			);

			INT Heightfield2D(
				FLOAT  *dest,
				INT     dest_stride,
				FLOAT   dest_height_scale,
				UCHAR  *heightfield,
				INT     num_vertices
			);

		// ---- properties
			UCHAR *heightfield_data = NULL;
			INT    heightfield_extent = 0;

		private:

		// ---- private implementation
			class  PI_Heightfield; PI_Heightfield *pi_heightfield = NULL;

	};

};


////////////////////////////////////////////////////////////////////////////////
