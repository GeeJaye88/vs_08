////////////////////////////////////////////////////////////////////////////////

// ---------- vs_mystar_starfield.hpp ----------
/*!
\file vs_mystar_starfield.hpp
\brief
\author Gareth Edwards
*/


#pragma once


#define USE_ALPHA_TEXTURE


// ---------- Callback_MyStar_Starfield_Configure ----------
/*!

*/
INT ObjConfig::Callback_MyStar_Starfield_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT star_count = (UINT)pd[STARFIELD_STAR_COUNT];


	// ---- atlas page
		context->SetAtlasPages(VS_ATLAS_PAGE_GROUP_FOUR);


	// ---- overide ?
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(TRUE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(TRUE);
			context->SetSolid(TRUE);
			context->SetTextured(TRUE);
			context->SetVisible(TRUE);
		}

	#if defined (USE_ALPHA_TEXTURE)


	// ---- no multistate
		context->SetSwitchedOn(FALSE);

	// ---- alpha from texture
		context->SetAlphaTexture(TRUE);

	#else

	// ---- hijack multistate
			context->SetSwitchedOn(TRUE);

	// ---- rgba from texture
			context->SetAlphaTexture(FALSE);

	#endif


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- vertex buffer
		context->SetVertexCount(6 * star_count);
		#if defined (USE_ALPHA_TEXTURE)
		context->SetVertexFormat((UINT)(VS_XYZ | VS_DIFFUSE | VS_TEX1));
		#else
		context->SetVertexFormat((UINT)(VS_XYZ | VS_TEX1));
		#endif
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_MyStar_Starfield_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(2 * star_count);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


#include <chrono>

// ----------  Callback_MyStar_Starfield_GetVertexBuffer ----------
/*!

MK System
=========

There are seven main types of stars. In order of decreasing
temperature, they are O, B, A, F, G, K, and M. This is known
as the Morgan–Keenan (MK) system.

Other types include D, L, T & Y.

Main Sequence Stars
===================

The main sequence stars are powered by the fusion of hydrogen
into helium in their cores. About 90% of the stars in the
Universe are main-sequence stars, including our sun. They
usually range from one-tenth to 200 times the Sun’s mass.

How many stars are visible at night?
====================================

On Earth, with a clear night sky we can easily perceive
around 2,000 – 2,500 stars.

Our eyes can only see stars brighter than magnitude 6 to 6,500.

This corresponds to between about 3,500 and 4,500 stars on any
given night in a single hemisphere under perfect conditions.

The upper end on the estimates seems is about 10,000.

Note: Brown Dwarfs mass ranges from around 13 to 80 x Jupiter.

Star        Colour            Approximate Surface  Prev  Luminance     Mass (Suns)
Type                          Temperature
====        ======            ===================  ====  =========     ===========
O - B       Blue Stars        30,000 K             Rare                2.50 - 90.0
G           Yellow Dwarfs     5,200 - 7,500 K      10%   0.60 - 5.00   0.80 - 1.40
G           Orange Dwarfs     3,700 - 5,200 K      10%   0.08 - 0.60   0.45 - 0.80
K - M       Red Dwarfs        4,000 K              73%   0.08 - 0.45   0.0001 - 0.8
O - B - A   Blue Giants       10,000 - 33,000 K    Rare  10,000        2.00 - 150.00
O - B       Blue Super Giants 10,000 - 50,000 K    Rare  10,000 - 1 M  20.0 - 1,000.00
K - M       Red Giants        3,300 - 5,300 K      0.4%  100 - 1,000   0.30 - 10.00
K - M       Red Super Giants  3,500 - 4,500 K      0.4%  1 - 800 T     10.0 - 40.00
D           White Dwarfs      8,000 - 40,000 K     0.4%  0.0001 100    0.10 - 1.40
M,L,T - Y   Brown Dwarfs      300 - 2,800 K        1.0%  Very Low      0.01 - 0.08
D           Neutron Stars     600,000 K            0.7%  Very Low      1.40 - 3.20
-           Black Dwarfs      Hypothetical 

F	        Blue to White     6,000 - 7,500 K     
G	        White to Yellow   5,000 - 6,000 K     
K	        Orange to Red     3,500 - 5,000 K
M	        Red               under 3,500 K


*/
INT ObjConfig::Callback_MyStar_Starfield_GetVertexBuffer(vs_system::ElmContext *context)
{
	
	// ---- random seed stuff


	// ---- chrono random seed (not used) ....
		// - requires #include <chrono> ( see above this method )
		unsigned seed = (INT) std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);


	// ---- Mersenne twister PRNG random seed
		//  - random device class instance, source of 'true'
		//    randomness for initializing random seed
		//  - initialized with seed from previous random device instance
		std::random_device rd;
		std::mt19937 gen(rd());


	// ---- simple random lambda using rand function
		// - use srand(#) to seed
		auto random_value = [](FLOAT range)
		{
			return (FLOAT)(rand() % 10000) / (FLOAT)10000 * range;
		};


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get parameter data
		UINT  type        = (UINT)pd[STARFIELD_TYPE];
		FLOAT radius_max  =       pd[STARFIELD_RADIUS_MAX];
		FLOAT radius_min  =       pd[STARFIELD_RADIUS_MIN];
		UINT  star_count  = (UINT)pd[STARFIELD_STAR_COUNT];
		FLOAT star_radius =       pd[STARFIELD_STAR_RADIUS];


	// ---- vertex
		UINT  vertex_size    = context->GetVertexSize();
		UINT  vertex_format  = context->GetVertexFormat();


	#if defined (USE_ALPHA_TEXTURE)

	// ---- star type table info
		struct star_info
		{
			FLOAT red, green, blue;
			FLOAT size;
			INT   prev_percent;
		};

	// ---- star type table
		star_info star_type_table[8] =
		{
			/* O */ { 146, 166, 239,  1.00f,  1 },
			/* B */ { 160, 179, 237,  0.90f,  4 },
			/* A */ { 198, 210, 227,  0.80f,  8 },
			/* F */ { 255, 255, 255,  0.70f, 12 },
			/* G */ { 242, 243, 248,  0.60f, 60 },
			/* K */ { 247, 237, 228,  0.50f, 11 },
			/* M */ { 242, 201,  15,  0.45f,  3 },
			/* D */ { 239, 191, 111,  0.45f,  1 },
		};

	// ---- dword to float
		union rgba_union
		{
			DWORD d;
			FLOAT f;
		} rgba;

	// ---- star data info
		struct star_data
		{
			FLOAT f;
			FLOAT size;
		};
		star_data star_data_table[100];

	// ---- random star rgb variance
		FLOAT rgb_mean = 20;
		FLOAT rgb_stddev = rgb_mean / 2;
		std::normal_distribution<float> rgb_distribution(rgb_mean, rgb_stddev);

	// ---- random star size variance
		FLOAT size_mean = star_radius / 2;
		FLOAT size_stddev = size_mean / 2;
		std::normal_distribution<float> size_distribution(size_mean, size_stddev);


	// ---- expand star info table to star data table by prevelance percentage 


	// ---- star type
		UINT  index = 0;
		FLOAT rgb[3];
		for ( UINT st=0; st<8; st++)
		{
			// ---- star data
				UINT sd_num = star_type_table[st].prev_percent;
				for (UINT sd = 0; sd < sd_num; sd++)
				{

					// ---- random star rgb variance
						rgb[0] = ( star_type_table[st].red   + rgb_distribution(generator) ) / 255;
						rgb[1] = ( star_type_table[st].green + rgb_distribution(generator) ) / 255;
						rgb[2] = ( star_type_table[st].blue  + rgb_distribution(generator) ) / 255;
						for (UINT i = 0; i < 3; i++) rgb[i] = rgb[i] < 0 ? 0 : (rgb[i] > 1 ? 1 : rgb[i]);

					// ---- convert DWORD difuse to FLOAT difuse
						rgba.d = VS_D3DCOLOR_COLORVALUE(rgb[0], rgb[1], rgb[2], 1);
						star_data_table[index].f = rgba.f;

					// ---- random star size variance
						star_data_table[index].size = ( size_distribution(generator) + size_mean ) * star_type_table[st].size * 0.1f;

					// ---- increment star_data_table index ONLY to limit
						index = index < 99 ? index + 1 : 99;
				}
		}

	#endif


	// ---- coloured vertex
		struct vrtx
		{
			#if defined (USE_ALPHA_TEXTURE)
			FLOAT x, y, z, d, u, v;
			#else
			FLOAT x, y, z, u, v;
			#endif
		};


	// ---- star colour texture
		struct uvtex { FLOAT u, v; };
		uvtex uv[6];
		auto set_tex = [&uv](INT index)
		{
			INT   row = index / 4;
			INT   col = index - row * 4;
			FLOAT u_loc = 1 - 0.125f;
			FLOAT v_loc = 1 - 0.125f;
			FLOAT u_wid = 0.125f / 4;
			FLOAT v_wid = 0.125f / 4;
			FLOAT u_off = (FLOAT)col * u_wid + u_loc;
			FLOAT v_off = (FLOAT)row * v_wid + v_loc;
			uv[0].u = u_off;         uv[0].v = v_off;
			uv[1].u = u_off + u_wid; uv[1].v = v_off;
			uv[2].u = u_off;         uv[2].v = v_off + v_wid;
			uv[3].u = u_off + u_wid; uv[3].v = v_off;
			uv[4].u = u_off + u_wid; uv[4].v = v_off + v_wid;
			uv[5].u = u_off;         uv[5].v = v_off + v_wid;
		};


	// ---- star quad
		set_tex(0);
		FLOAT sr = star_radius;
		vrtx vs[6] =
		{
			#if defined (USE_ALPHA_TEXTURE)
			{ -sr,  sr,  0,  star_data_table[0].f, uv[0].u, uv[0].v },
			{  sr,  sr,  0,  star_data_table[0].f, uv[1].u, uv[1].v },
			{ -sr, -sr,  0,  star_data_table[0].f, uv[2].u, uv[2].v },
			{  sr,  sr,  0,  star_data_table[0].f, uv[3].u, uv[3].v },
			{  sr, -sr,  0,  star_data_table[0].f, uv[4].u, uv[4].v },
			{ -sr, -sr,  0,  star_data_table[0].f, uv[5].u, uv[5].v },
			#else
			{ -sr,  sr,  0,  uv[0].u, uv[0].v },
			{  sr,  sr,  0,  uv[1].u, uv[1].v },
			{ -sr, -sr,  0,  uv[2].u, uv[2].v },
			{  sr,  sr,  0,  uv[3].u, uv[3].v },
			{  sr, -sr,  0,  uv[4].u, uv[4].v },
			{ -sr, -sr,  0,  uv[5].u, uv[5].v },
			#endif
		};


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- starfield cubic dimensions
		FLOAT max  = radius_max * 2;
		FLOAT min  = radius_min * 2;
		FLOAT maxby2 = max / 2;


	// ---- random location within cube
		FLOAT loc_mean = maxby2;
		FLOAT loc_stddev = loc_mean / 2;
		std::normal_distribution<float> loc_distribution(loc_mean, loc_stddev);


	// ---- random dist within from min to max distance
		FLOAT dist_mean = max - min;
		FLOAT dist_stddev = dist_mean / 2;
		std::normal_distribution<float> dist_distribution(dist_mean, dist_stddev);


	// ---- for each star
		for (UINT s = 0; s < star_count; s++)
		{
			
			#if defined (USE_ALPHA_TEXTURE)

			// ---- select star data index
				INT s_d_index = s % 100;

			#else

			// ---- select main sequence star texture colour
				INT colour_index = s % 16;
				set_tex(colour_index);
			
			#endif


			// ---- even distribution within +/- radius cube
				//FLOAT x_loc = random_value(max) - maxby2;
				//FLOAT y_loc = random_value(max) - maxby2;
				//FLOAT z_loc = random_value(max) - maxby2;
				FLOAT x_loc = loc_distribution(generator) - maxby2;
				FLOAT y_loc = loc_distribution(generator) - maxby2;
				FLOAT z_loc = loc_distribution(generator) - maxby2;


			// ---- location
				//FLOAT dist = random_value(max-min) + min;
				FLOAT dist = sqrt(x_loc*x_loc + y_loc*y_loc + z_loc*z_loc);
				FLOAT d = dist_distribution(generator) + min;
				x_loc = d / dist * x_loc;
				y_loc = d / dist * y_loc;
				z_loc = d / dist * z_loc;


			//---- convert to polar coordinates
				//
				// note: angles are calculated inc with XY axis
				//
				FLOAT xd  = x_loc * x_loc;
				FLOAT yd  = y_loc * y_loc;
				FLOAT zd  = z_loc * z_loc;
				FLOAT r   = sqrt(xd + yd + zd);
				FLOAT lon = acos(x_loc / sqrt(xd + zd)) * (z_loc < 0 ? -1 : 1);
				FLOAT lat = acos( y_loc / r) - (FLOAT)VS_PI / 2;


			// ---- rotate & locate quad
				for (UINT v = 0; v < 6; v++)
				{
					#if defined (USE_ALPHA_TEXTURE)

					FLOAT xs = vs[v].x * star_data_table[s_d_index].size;
					FLOAT ys = vs[v].y * star_data_table[s_d_index].size;
					FLOAT zs = vs[v].z * star_data_table[s_d_index].size;
					FLOAT x0 = xs;
					FLOAT y0 = ys * cos(lat) - zs * sin(lat);
					FLOAT z0 = ys * sin(lat) + zs * cos(lat);
					*(f)     = z0 * cos(lon) - x0 * sin(lon) + x_loc;
					*(f + 1) = y0                            + y_loc;
					*(f + 2) = z0 * sin(lon) + x0 * cos(lon) + z_loc;
					*(f + 3) = star_data_table[s_d_index].f;
					*(f + 4) = uv[v].u;
					*(f + 5) = uv[v].v;
					f += 6;

					#else

					FLOAT x0 = vs[v].x;
					FLOAT y0 = vs[v].y * cos(lat) - vs[v].z * sin(lat);
					FLOAT z0 = vs[v].y * sin(lat) + vs[v].z * cos(lat);
					*(f)     = z0 * cos(lon) - x0 * sin(lon) + x_loc;
					*(f + 1) = y0                            + y_loc;
					*(f + 2) = z0 * sin(lon) + x0 * cos(lon) + z_loc;
					*(f + 3) = uv[v].u;
					*(f + 4) = uv[v].v;
					f += 5;

					#endif

				}
		}

	return VS_OK;
}

