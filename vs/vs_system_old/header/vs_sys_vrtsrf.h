////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_vrtsrf.h ----------
/*!
\file vs_sys_vrtsrf.h
\brief Interface for the VirtualSurface class
\author Gareth Edwards
*/

#pragma once


// ---------- VirtualSurface class ----------
class VirtualSurface
{

public:

	VirtualSurface()
	{
		#ifdef VS_GFX_FLAG
		gfx_interface = app_dependency.GetGfxInterface();
		sys_input     = app_dependency.GetSysInput();
		#endif

		sys_context   = app_dependency.GetSysContext();
		sys_output    = app_dependency.GetSysOutput();
	};

	~VirtualSurface()
	{
		;
	};

	vs_system::AppDependency  app_dependency;

	#ifdef VS_GFX_FLAG
	vs_system::AppInterface   app_interface;
	vs_system::GfxInterface  *gfx_interface;
	vs_system::SysInput      *sys_input;
	#else
	vsx_system::AppInterface   app_interface;
	#endif

	vs_system::SysContext    *sys_context;
	vs_system::SysOutput     *sys_output;


} virtual_surface;
