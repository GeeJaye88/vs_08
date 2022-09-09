////////////////////////////////////////////////////////////////////////////////

// ---------- r2_pi_panophoto.hpp ----------
/*!
\file r2_pi_panophoto.hpp
\brief
\author Gareth Edwards
*/


// ---------- Private Implementation of PI_PanophotoData struct ----------

struct PI_PanophotoData
{

	BOOL  fullsize = TRUE;
	FLOAT basis = 0.0f;
	FLOAT gamma = 1.0f;
	FLOAT sharpness = 1.0f;
	UINT  sample = 3;
	UINT  width = 1920;

	FLOAT min_angle = -40.0f;
	FLOAT max_angle = 10.0f;
	FLOAT min_radius = 0.5f;
	FLOAT max_radius = 1.0f;
	FLOAT u_offset = 0.0f;
	FLOAT v_offset = 0.0f;

	CHAR filename[VS_MAXCHARLEN];
	CHAR path[VS_MAXCHARLEN];

	vs_system::ImgBuffer annular_image;

	BOOL thread_complete = TRUE;
	BOOL button_on = TRUE;

} pi_panophoto_data;

