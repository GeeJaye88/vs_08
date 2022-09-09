////////////////////////////////////////////////////////////////////////////////

// ---------- app_layout.cpp ----------
/*!
\file app_layout.cpp
\brief Implementation of the App(lication)Layout class
\author Gareth Edwards 
*/

// ---- include
#include "../header/vs_app_layout.h"


// ---- namespace
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Layout class ----------

class AppLayout::PI_AppLayout
{
	
public:

	RECT  client_rect = { 0, 0, 0, 0 };
	FLOAT client_width  = 0;
	FLOAT client_height = 0;
	FLOAT client_aspect = 0;

	FLOAT border_y = 0;
	FLOAT space_y  = 0;
	FLOAT top_y    = 0;
	FLOAT bottom_y = 0;

	FLOAT border_x = 0;
	FLOAT space_x  = 0;
	FLOAT left_x   = 0;
	FLOAT right_x  = 0;


	PI_AppLayout()
	{

		// ---- stuff
			app_dependency = NULL;

	};

	~PI_AppLayout() { };


	// ---- local
		AppDependency *app_dependency;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
/*!
\brief Constructor.
\author Gareth Edwards
*/
AppLayout::AppLayout()
{
	pi_app_layout = new PI_AppLayout;
}


// ---------- destructor ----------
/*!
\brief Destructor.
\author Gareth Edwards
*/
AppLayout::~AppLayout()
{
	;
}



// ---------- Setup ----------
/*!
\brief Setup.
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT AppLayout::Setup(AppDependency *ad)
{
	
	pi_app_layout->app_dependency = ad;
	
	SetLayout();

	return VS_OK;
}


vs_system::AppDependency *AppLayout::GetAppDependency()
{
	return pi_app_layout->app_dependency;
}


INT AppLayout::GetBorderRect(
		VsFloatRectangle *rect
	)
{

	VsFloatRectangle r = {
			pi_app_layout->left_x,
			pi_app_layout->top_y,
			pi_app_layout->right_x,
			pi_app_layout->bottom_y
		};
	*rect = r;

	return VS_OK;
}


FLOAT AppLayout::GetValue(UINT id)
{

	switch (id)
	{
		case ASPECT   : return pi_app_layout->client_aspect; break;
		case BORDER_X : return pi_app_layout->border_x; break;
		case BORDER_Y : return pi_app_layout->border_y; break;
		case SPACE_X  : return pi_app_layout->space_x;  break;
		case SPACE_Y  : return pi_app_layout->space_y;  break;
		default: return (FLOAT)-1;
	}

	return VS_OK;
}


INT AppLayout::SetLayout()
{

	vs_system::SysContext *sys_context = pi_app_layout->app_dependency->GetSysContext();

	sys_context->GetClientRect(&pi_app_layout->client_rect);

	pi_app_layout->client_width  =
		(FLOAT)(pi_app_layout->client_rect.right - pi_app_layout->client_rect.left);

	pi_app_layout->client_height =
		(FLOAT)(pi_app_layout->client_rect.bottom - pi_app_layout->client_rect.top);

	pi_app_layout->client_aspect =
		pi_app_layout->client_width / pi_app_layout->client_height;
	
	SetY();
	SetX();

	return VS_OK;
}


INT AppLayout::SetX()
{

	pi_app_layout->border_x = pi_app_layout->border_y / pi_app_layout->client_aspect;
	pi_app_layout->space_x  = pi_app_layout->space_y / pi_app_layout->client_aspect;
	pi_app_layout->left_x   = pi_app_layout->border_x;
	pi_app_layout->right_x  = 1 - pi_app_layout->border_x;

	return VS_OK;
}


INT AppLayout::SetY()
{

	vs_system::SysContext *sys_context = pi_app_layout->app_dependency->GetSysContext();

	FLOAT border_width = sys_context->GetFloat(sys_context->BORDER_WIDTH);

	pi_app_layout->border_y = border_width;
	pi_app_layout->space_y  = pi_app_layout->border_y;
	pi_app_layout->top_y    = pi_app_layout->border_y;
	pi_app_layout->bottom_y = 1.0f - pi_app_layout->border_y;

	return VS_OK;
}




////////////////////////////////////////////////////////////////////////////////
