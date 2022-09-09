////////////////////////////////////////////////////////////////////////////////

// ---------- vs_elm_lam.h ----------
/*!
\file vs_elm_lam.h
\brief Interface for the "L"ight "A"nd "M" aterial classes
\author Gareth Edwards
*/

#pragma once


#include "../header/vs_inc_defs.h"
#include "../header/vs_inc_dems.h"
#include "vs_inc_maths.h"


namespace vs_system
{

	class VsAmbient
	{
		public:

			VsAmbient::VsAmbient()
				{
					ZeroMemory(this, sizeof(VsAmbient));
				};

			VsAmbient::~VsAmbient()
				{
					;
				};

			VsColour colour;

			INT GetColour(VsColour *c)   { *c = colour; return VS_OK; }
			INT SetColour(VsColour  c)   { colour = c; return VS_OK; }

	};


	typedef enum VsFogType
	{
		VS_FOG_NONE   = 0,
		VS_FOG_EXP    = 1,
		VS_FOG_EXP2   = 2,
		VS_FOG_LINEAR = 3
	} vs_fog_type;


	class VsFog
	{

		public:

			VsFog::VsFog()
			{
				ZeroMemory(this, sizeof(VsFog));
			};

			VsFog::~VsFog()
			{ 
				;
			};

			VsFogType table_mode;      // type
			VsColour colour;          // color
			FLOAT start;               // start (for both vertex and pixel)
			FLOAT end;                 // end (ditto)
			FLOAT density;             // density
			BOOL  range_enable;        // on or off - only for vertex shading

			INT SetType(VsFogType t)
			{
				table_mode = t;
				return VS_OK;
			}

			INT SetColour(VsColour c) {
				colour = c;
				return VS_OK; }
			INT SetStart(FLOAT s)      { start = s;        return VS_OK; }
			INT SetEnd(FLOAT e)        { end = e;          return VS_OK; }
			INT SetDensity(FLOAT d)    { density = d;      return VS_OK; }
			INT SetRangeEnable(BOOL r) { range_enable = r; return VS_OK; }

	};


	typedef enum VsLightType
	{
		VS_LIGHT_POINT       = 1,
		VS_LIGHT_SPOT        = 2,
		VS_LIGHT_DIRECTIONAL = 3,
	} vs_light_type;


	class VsLight
	{

		public:

			VsLight::VsLight()
				{
					ZeroMemory(this, sizeof(VsLight));
				};

			VsLight::~VsLight()
				{
					;
				};

			VsLightType type;             // type of light source
			VsColour    diffuse;          // diffuse color of light
			VsColour    specular;         // specular color of light
			VsColour    ambient;          // ambient color of light
			VsVector3   position;         // position in world space
			VsVector3   direction;        // direction in world space
			FLOAT       range;            // cutoff range
			FLOAT       falloff;          // falloff
			FLOAT       attenuation0;     // constant attenuation
			FLOAT       attenuation1;     // linear attenuation
			FLOAT       attenuation2;     // quadratic attenuation
			FLOAT       theta;            // inner angle of spotlight cone
			FLOAT       phi;              // outer angle of spotlight cone

			INT SetType(VsLightType t)
			{
				type = t;
				return VS_OK;
			}

			INT SetDiffuse(VsColour d)    { diffuse      = d; return VS_OK; }
			INT SetSpecular(VsColour s)   { specular     = s; return VS_OK; }
			INT SetAmbient(VsColour a)    { ambient      = a; return VS_OK; }
			INT SetPosition(VsVector3 v)  { position     = v; return VS_OK; }
			INT SetDirection(VsVector3 v) { direction    = v; return VS_OK; }
			INT SetRange(FLOAT r)         { range        = r; return VS_OK; }
			INT SetFalloff(FLOAT f)       { falloff      = f; return VS_OK; }
			INT SetAttConstant(FLOAT c)   { attenuation0 = c; return VS_OK; }
			INT SetAttLinear(FLOAT l)     { attenuation1 = l; return VS_OK; }
			INT SetAttQuadratic(FLOAT q)  { attenuation2 = q; return VS_OK; }
			INT SetInner(FLOAT i)         { theta        = i; return VS_OK; }
			INT SetOuter(FLOAT o)         { phi          = o; return VS_OK; }

	};


	class VsMaterial
	{
		public:

			VsMaterial::VsMaterial()
				{
					ZeroMemory(this, sizeof(VsMaterial));
				};

			VsMaterial::~VsMaterial()
				{
					;
				};

			VsColour   diffuse;          // Diffuse color RGBA
			VsColour   ambient;          // Ambient color RGB
			VsColour   specular;         // Specular 'shininess'
			VsColour   emissive;         // Emissive color RGB
			FLOAT      power;            // Sharpness if specular highlight

			INT SetDiffuse(VsColour d)   { diffuse  = d; return VS_OK; }
			INT SetAmbient(VsColour a)   { ambient  = a; return VS_OK; }
			INT SetSpecular(VsColour s)  { specular = s; return VS_OK; }
			INT SetEmissive(VsColour e)  { emissive = e; return VS_OK; }
			INT SetPower(FLOAT p)        { power    = p; return VS_OK; }

	};

}