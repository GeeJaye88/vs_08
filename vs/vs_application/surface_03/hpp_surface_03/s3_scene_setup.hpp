////////////////////////////////////////////////////////////////////////////////

// ---------- s3_scene_setup.hpp ----------
/*!
\file s3_scene_setup.hpp
\brief surface_03 scene (camera, lights, materials) methods
\author Gareth Edwards
*/


// ---------- SetupExploreCamera ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetupExploreCamera(UINT mode, vs_system::AppKamera *kamera)
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_surface_03->gfx;
		INT result = VS_OK;
		INT hr = 0;


	// ---- select
		switch( mode )
		{
			case 1:	
				break;
	
			default:

				if ( pi_surface_03->runtime )
				{

					// ---- setup proportional to size of surface
						FLOAT dist = GetAcquireSurfaceSize() * 2.0f;


					// ---- pan & tilt
						FLOAT pan  = pi_surface_03->_configuration.explore_kamera.pan;
						FLOAT p2r  = pan * (FLOAT)VS_RADIANS_PER_DEGREE;
						FLOAT tilt = pi_surface_03->_configuration.explore_kamera.tilt;
						FLOAT t2r  = tilt * (FLOAT)VS_RADIANS_PER_DEGREE;


					// ---- target
						FLOAT d = dist* pi_surface_03->_configuration.explore_kamera.dist_scalar;
						FLOAT xt =  0;
						FLOAT yt =  d * sinf(t2r);
						FLOAT zt = -d * cosf(t2r);


					// ---- lookat
						FLOAT xl = zt * sin(p2r) + xt * cos(p2r);
						FLOAT yl = yt;
						FLOAT zl = zt * cos(p2r) - xt * sin(p2r);


					// ---- kamera proportional value
						FLOAT kpv = dist / 1000.0f; // LIMIT ?


					// ---- target
						kamera->SetTargetIncr(kpv * 0.05f);
						kamera->SetTargetZoom(dist * 0.6f);

					// ---- change
						kamera->SetTargetChangeZoomPower(1.5f);
						kamera->SetTargetChangeZoomDenom(20.0f);

					// ---- change
						kamera->SetTargetZoomMinimum(dist * 0.25f);
						kamera->SetTargetZoomMaximum(dist * 2.0f);

					// ---- overall increment NOT proportional to surface
						kamera->SetScalarIncr(0.2f);
						kamera->SetScalarRotation(TRUE);
						kamera->SetScalarMode(TRUE);

					// ---- move increment
						{
							FLOAT incr = 0.25f;
							kamera->SetPositionIncr(kpv * incr);
							kamera->SetTranslateIncr(kpv * incr);
							kamera->SetZoomIncr(kpv * incr / 2);
						}

					// ---- rotational increment - NOT proportional to surface
						kamera->SetPanTiltIncr(0.01f);
						kamera->SetRotateIncr(0.025f);

					// ---- set view
						kamera->SetPanTilt(pan, tilt);
						kamera->SetPosition(xl, yl, zl);

					// ---- kamera (view) matrix
						VsMatrix V;
						kamera->GetViewMatrix(&V);
						gfx->SetViewMatrix(1, &V);

					// ---- housekeeping - set default & state
						kamera->SaveDefaults();
						kamera->SaveState();



				}
				break;
		}

	return VS_OK;
}


INT Surface_03::SetupAcquireLight()
{

	// ---- get aquire param
		INT current_param_group = parameter.GetParamGroup();
		parameter.SetParamGroup(surface_03::Model::PARAM_GROUP_ACQUIRE);
		INT acquire_param_group_index = parameter.GetParamGroupIndex();
		INT acquire_param_group_size  = parameter.GetParamGroupSize();
		INT acquire_param_array_index = parameter.GetParamArrayIndex();
		INT acquire_param_array_size  = parameter.GetParamArraySize();
		FLOAT *acquire_param_array    = new FLOAT[acquire_param_array_size];


	// ---- get aquire light parameters
		INT result = parameter.GetParamArray(0, 9, acquire_param_array);


	// ---- store
		SetAcquireLightParam(
				*(acquire_param_array + 1),
				*(acquire_param_array + 2),
				*(acquire_param_array + 3),
				*(acquire_param_array + 4)
			);


	// ---- reset
		parameter.SetParamGroup(current_param_group);


	// ---- cleanup
		delete[] acquire_param_array;

	return VS_OK;
}


// ---------- SetupExploreLighting ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

Attenuation

Attenuation requires some basic algebra skills to fully understand.

Basically, attenuation controls how illuminated an object will be
based on its distance from the light source. In other words, it
controls how a light's intensity will decrease as it travels
through space.

The attenuation function:

Atten = 1 / (att0 + att1 * d + att2 * d2)

Atten

Atten is short for attenuation. It is a number between 1.0 and 0.0
which indicates the intensity of light. 1.0 is full intensity, while
0.0 is no intensity (no light). For each vertex within the light's range,
the Atten is applied, along with the vertex's normal, to determine
how much illumination there is.

d

d in this function represents distance. This refers to the distance
between the vertex and the light. In the function it is multiplied
by att1 and att2, which are constants I'll go over next.

att0

There are three constant values in this equation, att0, att1 and att2.
If you look, you will see that att0 is not multiplied by anything.
This makes it a constant modifier. If you place a number in only this
variable, you get a constant amount of light. This means there is no
attenuation at all. For example, if you put 0.5 in this value, you
will get a half-lit light that will extend all the way to the maximum
range of the light.

att1

This is the second constant value. If used by itself, it is an inverse
function, meaning that the light will dissipate more slowly as the
distance increases. This is probably the most realistic type of light.
Usually you can get away with just setting this to 1.0 and the other
two values to 0.0. However, because of varying types of lights,
this doesn't always work out.

att2

This is the third constant value. If used by itself, it is an inverse
square function, meaning that the light will not only dissipate more
slowly as the distance increases, but the dissipation will be very
rapid at first, then sharply slow down. This type of attenuation has
the effect of, say, a campfire at night. It is very bright around
the campfire. But if you walk fifty feet away, you can still see
objects lit by the fire, but very dimly. If you walk a hundred feet
away, you'll still be able to see the light, and it probably won't
get that much darker.

Using the Equation

When building a point light, you will want to look over the attenuation
function and find out what values you want in each variable. You can
combine them to get all kinds of different effects. You do this by
plugging in each att value and seeing how brightly the light will show
at various distances. This will give you some prediction on how the
light will perform.

There is another thing to look out for, and that is having the Range
of the light suddenly cut off an area where the light is still bright.
Remember that Range determines how far Direct3D will calculate the
attenuation. If the maximum range of the light is still quite bright
with attenuation, it can make things look a bit unrealistic.

If you position the Range correctly, you won't even notice where
the edge of the light is.

Note that setting the range or attenuation for a directional light has no effect.

*/
INT Surface_03::SetupExploreLighting(UINT mode)
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_surface_03->gfx;
		INT result = VS_OK;
		INT hr = 0;


	// ---- param
		parameter.SetParamGroup(surface_03::Model::PARAM_GROUP_LIGHTS);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- select
		switch ( mode )
		{
			case 2: // colour light
			case 1: // white light
				{

					// ---- light - white or coloured
						VsColour col[2][4] = 
						{
							{
								{ 1.00f, 1.00f, 1.00f, 1.00f },
								{ 1.00f, 1.00f, 1.00f, 1.00f },
								{ 1.00f, 1.00f, 1.00f, 1.00f },
								{ 1.00f, 1.00f, 1.00f, 1.00f } 
							},
							{
								{ 0.00f, 1.00f, 0.00f, 1.00f },
								{ 1.00f, 0.50f, 0.00f, 1.00f },
								{ 1.00f, 0.00f, 1.00f, 1.00f },
								{ 0.00f, 0.50f, 1.00f, 1.00f }
							},
						};


					// ---- 4 * point illumination
						VsColour  light_amb_col = { 0, 0, 0, 1 };
						VsColour  light_spc_col = { 1, 1, 1, 1 };
						vs_system::VsLight light_point;
						hr = light_point.SetType(vs_system::VS_LIGHT_POINT);

						FLOAT *v = new FLOAT[param_array_size];
						for (UINT i = 0; i < 4; i++)
						{

							// ---- light parameters
								result = parameter.GetParamArray(i, param_array_size, v);
								FLOAT param_intensity = v[1];
								FLOAT param_dist      = v[2];
								FLOAT param_range     = v[3];
								FLOAT param_atten     = v[4];
								FLOAT param_azimuth   = v[5];
								FLOAT param_elevation = v[6];
								FLOAT param_height    = v[7];

							// ---- position

								FLOAT t2r   = param_azimuth   * (FLOAT)VS_RADIANS_PER_DEGREE;
								FLOAT phi   = param_elevation + (FLOAT)i * 90.0f + 90.0f;
								FLOAT p2r   = phi             * (FLOAT)VS_RADIANS_PER_DEGREE;

								// y' = y*cos q - z*sin q
								// z' = y*sin q + z*cos q
									FLOAT xt =  0;
									FLOAT yt =  param_dist * sinf(t2r);
									FLOAT zt = -param_dist * cosf(t2r);

								// z' = z*cos q - x*sin q
								// x' = z*sin q + x*cos q
									FLOAT zl = zt * cos(p2r) - xt * sin(p2r);
									FLOAT xl = zt * sin(p2r) + xt * cos(p2r);

							// ---- store light location
								pi_surface_03->_configuration.explore_location[i] = { xl, yt + param_height, zl };
								hr = light_point.SetPosition(pi_surface_03->_configuration.explore_location[i]);

							// ---- spec and ambient have no effect
								hr = light_point.SetAmbient(light_amb_col);
								hr = light_point.SetSpecular(light_spc_col);

							// ---- diffuse
								VsColour intensity = col[pi_surface_03->_configuration.explore_light_enabled.colour ? 1 : 0][i];
								intensity.red   *= param_intensity;
								intensity.green *= param_intensity;
								intensity.blue  *= param_intensity;
								hr = light_point.SetDiffuse(intensity);

							// ---- cuts illumination at a specific dist
								hr = light_point.SetRange(param_range);

							// ---- Atten = 1 / (att0 + att1 * d + att2 * d2)
								hr = light_point.SetAttLinear(param_atten / 100.0f); // att1

							// ---- store light (to colourise light object)
								pi_surface_03->_configuration.explore_light[i] = light_point;

							// ---- set
								hr = gfx->SetLight(i, &light_point);
								hr = gfx->SetLightEnabled(i, pi_surface_03->_configuration.explore_light_enabled.state[i]);
						}


						// ---- ambient light param
							result = parameter.GetParamArray(4, param_array_size, v);
							BOOL  enabled   = (BOOL)((INT)v[0]);
							FLOAT intensity = v[1];

							// wrong ambient...
							//pi_surface_03->_configuration.acquire_ambient_param.intensity = intensity;
							//

						// ---- ambient
							VsColour amb_col = { intensity, intensity, intensity, 1.0f };
							vs_system::VsAmbient amb;
							hr = amb.SetColour(amb_col);
							hr = gfx->SetAmbient(&amb);
							hr = gfx->SetAmbientEnabled(enabled);

						delete[] v;

				}
				break;
	
			default:
				{
	
					// ---- light values
						FLOAT amb_val = 0.2f;
						FLOAT sun_val = 0.7f;
						FLOAT dif_val = 0.2f;
						FLOAT spe_val = 0.4f;

					// ---- ambient
						VsColour amb_col = { amb_val, amb_val, amb_val, 1.0f };
						vs_system::VsAmbient amb;
						hr = amb.SetColour(amb_col);
						hr = gfx->SetAmbient(&amb);

					// ---- directional main light
						VsColour  sun_amb = { amb_val, amb_val, amb_val, 1.0f };
						VsColour  sun_dif = { sun_val, sun_val, sun_val, 1.0f };
						VsColour  sun_spc = { spe_val, spe_val, spe_val, 1.0f };
						VsVector3 sun_dir = {  1.0f,-1.0f, 1.0f };
						vs_system::VsLight sun;
						hr = sun.SetType(vs_system::VS_LIGHT_DIRECTIONAL);
						hr = sun.SetAmbient(sun_amb);
						hr = sun.SetDiffuse(sun_dif);
						hr = sun.SetSpecular(sun_spc);
						hr = sun.SetDirection(sun_dir);
						hr = gfx->SetLight(0, &sun);
						hr = gfx->SetLightEnabled(0, TRUE);

					// ---- directional fill from right to back
						VsColour  lgt_dif = { dif_val, dif_val, dif_val, 1.0f };
						VsColour  lgt_spc = { spe_val, spe_val, spe_val, 1.0f };
						VsVector3 lgt_dir = {-1.0f,-1.0f,-1.0f };
						vs_system::VsLight lgt;
						hr = lgt.SetType(vs_system::VS_LIGHT_DIRECTIONAL);
						hr = lgt.SetDiffuse(lgt_dif);
						hr = lgt.SetSpecular(lgt_spc);
						hr = lgt.SetDirection(lgt_dir);
						hr = gfx->SetLight(1, &lgt);
						hr = gfx->SetLightEnabled(1, FALSE);

					// ---- point radience from origin
						VsColour  top_dif = { dif_val, dif_val, dif_val, 1.0f };
						VsColour  top_spc = { spe_val, spe_val, spe_val, 1.0f };
						VsVector3 top_pos = { 0.0f, 0.0f, 0.0f };
						vs_system::VsLight top;
						hr = top.SetType(vs_system::VS_LIGHT_POINT);
						hr = top.SetDiffuse(top_dif);
						hr = top.SetSpecular(top_spc);
						hr = top.SetPosition(top_pos);
						hr = top.SetRange(1000);
						hr = top.SetAttLinear(0.01f);
						hr = gfx->SetLight(2, &top);
						hr = gfx->SetLightEnabled(2, FALSE);

					// ---- spot left to front
						VsColour  spot_dif = { dif_val, dif_val, dif_val, 1.0f };
						VsColour  spot_spc = { spe_val, spe_val, spe_val, 1.0f };
						VsVector3 spot_pos = {-800.0f, 800.0f,-800.0f };
						VsVector3 spot_dir = { 1.0f,-1.0f, 1.0f };
						vs_system::VsLight spot;
						hr = spot.SetType(vs_system::VS_LIGHT_SPOT);
						hr = spot.SetDiffuse(spot_dif);
						hr = spot.SetSpecular(spot_spc);
						hr = spot.SetPosition(spot_pos);
						hr = spot.SetDirection(spot_dir);
						hr = spot.SetRange(2000);
						hr = spot.SetAttConstant(0);
						hr = spot.SetAttLinear(0.001f);
						hr = spot.SetAttQuadratic(0);
						hr = spot.SetOuter(20 * (float)VS_RADIANS_PER_DEGREE);
						hr = spot.SetInner(10* (float)VS_RADIANS_PER_DEGREE);
						hr = spot.SetFalloff(1.0f);
						hr = gfx->SetLight(3, &spot);
						hr = gfx->SetLightEnabled(3, TRUE);

				}
				break;
		}


	// ---- param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ---------- SetupExploreMaterials ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

RGB to Lumaconversion formulae
==============================

Perceived brightness?

Luminance?

Luminance (standard for certain colour spaces):
	(0.2126*R + 0.7152*G + 0.0722*B) [1]

Luminance (perceived option 1):
	(0.299*R + 0.587*G + 0.114*B) [2]

Luminance (perceived option 2, slower to calculate):
		sqrt( 0.241*R^2 + 0.691*G^2 + 0.068*B^2 ) → sqrt( 0.299*R^2 + 0.587*G^2 + 0.114*B^2 )

Photometric/digital ITU BT.709:

	Y = 0.2126 R + 0.7152 G + 0.0722 B

Digital ITU BT.601 (gives more weight to the R and B components):

	Y = 0.299 R + 0.587 G + 0.114 B

If you are willing to trade accuracy for perfomance, there are two approximation formulas for this one:

	Y = 0.33 R + 0.5 G + 0.16 B

	Y = 0.375 R + 0.5 G + 0.125 B

These can be calculated quickly as

	Y = (R+R+B+G+G+G)/6

	Y = (R+R+R+B+G+G+G+G)>>3

*/

INT Surface_03::SetupExploreMaterials(UINT index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		vs_system::GfxInterface *gfx = pimp->gfx;


	// ---- material colour
		VsColour colour[5] =
		{
			{ 0.0f, 1.0f, 0.0f, 0 },
			{ 1.0f, 0.5f, 0.0f, 0 },
			{ 1.0f, 0.0f, 1.0f, 0 },
			{ 0.0f, 0.5f, 5.0f, 0 },
			{ 0.5f, 0.5f, 5.0f, 0 }
		};
		VsColour colour_off = { 0.2f, 0.2f, 0.2f, 0 };
		VsColour zero = { 0, 0, 0, 0 };


	// ---- scalar proportional to size of surface
		FLOAT scalar = ( p_s_03->GetAcquireSurfaceLimit() * 2 ) / p_s_03->GetAcquireSurfaceSize();


	// ---- light colour as per _configuration
		VsColour light_colour = {
			pimp->_configuration.explore_light[index].diffuse.red   * scalar,
			pimp->_configuration.explore_light[index].diffuse.green * scalar,
			pimp->_configuration.explore_light[index].diffuse.blue  * scalar,
			0
		};
		light_colour.red   = light_colour.red   > 1 ? 1 : light_colour.red;
		light_colour.green = light_colour.green > 1 ? 1 : light_colour.green;
		light_colour.blue  = light_colour.blue  > 1 ? 1 : light_colour.blue;


	// ---- light luminance ( note: no scalar )
		FLOAT luma = (
				(light_colour.red * 2) +
				(light_colour.green * 3) +
				light_colour.blue) / 6;
		luma *= scalar;
		//luma = luma > 1 ? 1 : ( luma < 0.2f ? 0.2f : luma);
		VsColour white = { luma, luma, luma, 0 };


	// ---- light flags
		BOOL light_enabled    = pimp->_configuration.explore_light_enabled.state[index];
		BOOL specular_enabled = pimp->_configuration.explore_light_enabled.specular;
		BOOL colour_enabled   = pimp->_configuration.explore_light_enabled.colour;


	// ---- get material
		UINT gfx_index = index + 1;
		vs_system::VsMaterial mtl;
		gfx->GetMaterial(gfx_index, &mtl);


	// ---- set
		INT result = VS_OK;
		result = mtl.SetAmbient(zero);
		result = mtl.SetDiffuse(zero);
		result = mtl.SetSpecular(zero);
		result = mtl.SetPower(1);


	// ---- update emmisive 
		result = mtl.SetEmissive(
			light_enabled ?
				(colour_enabled ? light_colour : white) : colour_off);


	// ---- set material
		result = gfx->SetMaterial(gfx_index, &mtl);

	return VS_OK;
}


// ---------- SetupExploreMaterials ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note 22 colours with indices from 0 to 21

8 colour pallette - from 0 to 4
   - lights are 0 to 3
   - ambient is 4
   - RGB emissive are 5 to 7

16 colour pallette - from 6 to 21

*/
INT Surface_03::SetupExploreMaterials(UINT mode)
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_surface_03->gfx;
		INT result = VS_OK;


	// ---- select
		switch(mode)
		{

			case 1:
				{

					// ---- init material colours
						vs_system::VsMaterial mtl;

						VsColour zero     = { 0, 0, 0, 0 };
						VsColour ambient  = { 1, 1, 1, 0 };
						VsColour emissive = zero;
						VsColour specular = { 0.1f, 0.1f, 0.1f, 0 };
						VsColour diffuse  = { 1, 1, 1, 0 };

						BOOL specular_enabled =
							pi_surface_03->_configuration.explore_light_enabled.specular;
						
					// ---- surface
						result = mtl.SetAmbient(ambient);
						result = mtl.SetDiffuse(diffuse);
						result = mtl.SetEmissive(emissive);
						result = mtl.SetSpecular(specular_enabled ? specular : emissive);
						result = mtl.SetPower(4);
						result = gfx->SetMaterial(0, &mtl);

					// ---- lights
						//
						// 0: green    ( 0.0 1.0 0.0 )
						// 1: orange   ( 1.0 0.5 0.0 )
						// 2: magenta  ( 1.0 0.0 1.0 )
						// 3: cyan     ( 0.0 0.5 1.0 )
						// 4: ambient
						//
						for ( UINT i=0; i<5; i++)
						{
							SetupExploreMaterials(i, this);
						}

					// ---- 5: red emissive
						VsColour red = { 0.8f, 0.2f, 0.0f, 0.0f };
						result = mtl.SetAmbient(zero);
						result = mtl.SetDiffuse(zero);
						result = mtl.SetSpecular(zero);
						result = mtl.SetEmissive(red);
						result = mtl.SetPower(1);
						result = gfx->SetMaterial(5, &mtl);

					// ---- 6: green emissive
						VsColour green = { 0.0f, 0.8f, 0.2f, 0.0f };
						result = mtl.SetAmbient(zero);
						result = mtl.SetDiffuse(zero);
						result = mtl.SetSpecular(zero);
						result = mtl.SetEmissive(green);
						result = mtl.SetPower(1);
						result = gfx->SetMaterial(6, &mtl);

					// ---- 7: dark green emissive
						VsColour darkgreen = { 0.0f, 0.35f, 0.1f, 0.0f };
						result = mtl.SetAmbient(zero);
						result = mtl.SetDiffuse(zero);
						result = mtl.SetSpecular(zero);
						result = mtl.SetEmissive(darkgreen);
						result = mtl.SetPower(1);
						result = gfx->SetMaterial(7, &mtl);

					// ---- 8: blue  emissive
						VsColour blue = { 0.0f, 0.2f, 0.8f, 0.0f };
						result = mtl.SetAmbient(zero);
						result = mtl.SetDiffuse(zero);
						result = mtl.SetSpecular(zero);
						result = mtl.SetEmissive(blue);
						result = mtl.SetPower(1);
						result = gfx->SetMaterial(8, &mtl);

				}
				break;

			default:
				{
					//
					// 16 colour pallette - indices from 10 to 25 
					//
					VsColour ambient  = { 0.2f, 0.2f, 0.2f, 0 };
					VsColour emissive = { 0, 0, 0, 0 };
					VsColour specular = { 0, 0, 0, 0 };

					VsColour diffuse[16] =
					{
						{ 0.00f, 0.00f, 0.00f, 1.0f }, // 10 black
						{ 1.00f, 1.00f, 1.00f, 1.0f }, // 11 white
						{ 0.87f, 0.87f, 0.87f, 0.8f }, // 12 light grey
						{ 0.50f, 0.50f, 0.50f, 1.0f }, // 13 grey
						{ 0.38f, 0.38f, 0.38f, 1.0f }, // 14 dark grey
						{ 1.00f, 0.00f, 0.00f, 1.0f }, // 15 red
						{ 1.00f, 0.38f, 0.87f, 1.0f }, // 16 pink
						{ 0.63f, 0.12f, 1.00f, 1.0f }, // 17 purple
						{ 0.31f, 0.87f, 1.00f, 1.0f }, // 18 light blue
						{ 0.00f, 0.12f, 1.00f, 1.0f }, // 19 blue
						{ 0.38f, 1.00f, 0.50f, 1.0f }, // 20 yellow blue
						{ 0.00f, 0.75f, 0.00f, 1.0f }, // 21 green
						{ 1.00f, 0.87f, 0.12f, 1.0f }, // 22 yellow
						{ 1.00f, 0.63f, 0.06f, 1.0f }, // 23 orange
						{ 0.63f, 0.50f, 0.38f, 1.0f }, // 24 brown
						{ 1.00f, 0.87f, 0.63f, 1.0f }, // 25 pale pink
					};

					for (int i = 0; i < 16; i++)
					{
						VsColour amb = ambient;
						VsColour dif = {
								diffuse[i].red,
								diffuse[i].green,
								diffuse[i].blue,
								diffuse[i].alpha
							};
						VsColour emi = emissive;
						VsColour spc = specular;
						vs_system::VsMaterial mtl;
						result = mtl.SetAmbient(amb);
						result = mtl.SetDiffuse(dif);
						result = mtl.SetEmissive(emi);
						result = mtl.SetSpecular(spc);
						result = mtl.SetPower(4);
						result = gfx->SetMaterial(i+10, &mtl);
					}

				}
				break;

		}

	return VS_OK;
}

