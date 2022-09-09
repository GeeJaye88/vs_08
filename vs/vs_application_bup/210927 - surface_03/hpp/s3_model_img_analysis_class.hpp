////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model_img_analysis.hpp ----------
/*!
\file s3_img_atlas.hpp
\brief
\author Gareth Edwards
*/


using namespace surface_03;


// ---------- ImgAnalysisSlice ----------
	class ImgAnalysisSlice
	{

		public:

			ImgAnalysisSlice() {};
			~ImgAnalysisSlice() {};

			UINT   size        = 0;
			UINT   extent      = 0;
			BOOL   initialised = FALSE;
			BYTE  *rgba_buffer = NULL;
			FLOAT *norm_buffer = NULL;
			FLOAT *vert_buffer = NULL;
			vs_system::VsLight light;


			VOID initialise(UINT s)
			{
				if ( s != size  )
				{
					size = 0;
					extent = 0;
					if ( vert_buffer != NULL ) { delete[] vert_buffer; vert_buffer = NULL; }
					if ( norm_buffer != NULL ) { delete[] norm_buffer; norm_buffer = NULL; }
					if ( rgba_buffer != NULL ) { delete[] rgba_buffer; rgba_buffer = NULL; }
					initialised = FALSE;
				}

				if ( ! initialised )
				{
					size = s;
					extent = size * 3;
					vert_buffer = new FLOAT[extent];
					norm_buffer = new FLOAT[extent];
					rgba_buffer = new BYTE [extent];
					for (UINT i = 0; i < extent; i++)
					{
						*(vert_buffer + i) = 0;
						*(norm_buffer + i) = 0;
						*(rgba_buffer + i) = 0;
					}
					initialised = TRUE;
				}
			}


			VOID store(UINT s, FLOAT *v, FLOAT *n, BYTE *p, vs_system::VsLight *l)
			{
				initialise(s);
				for (UINT i = 0; i < extent; i++)
				{
					*(vert_buffer + i) = *(v + i);
					*(norm_buffer + i) = *(n + i);
					*(rgba_buffer + i) = *(p + i);
				}
				light = *l;
			}

	};


	// ---------- ImgAnalysisData ----------
	class ImgAnalysisData
	{

		public:

			ImgAnalysisData() {};
			~ImgAnalysisData() {};

			UINT row = 0;
			ImgAnalysisSlice data[2];
			VsColour ambient;
			VsColour diffuse;

	};