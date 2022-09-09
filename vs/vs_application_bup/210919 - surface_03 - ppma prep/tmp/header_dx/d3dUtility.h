//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// d3dUtility.h : interface for the directx tainted devParam class
//
//              : forward declaration of utility dx tainted functions and constants
//        
//////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef __d3dUtilityH__
#define __d3dUtilityH__




// ---------- include ----------

#include <d3dx9.h>
#include <string>




//////////////////////////////////////////////////////////////////////////////////////////////////




// ********* BEGIN NAMESAPCE *********




namespace d3d
{




/////////////////////////////////////////////////////////////////////////////////////////////////




// ******** DEVICE PARAMETERS ********




class devParam
{


public:


		// constructor/destructor
	devParam() {};
	~devParam() {};

	HWND					hWindow;
	D3DPRESENT_PARAMETERS	presentation;
	DWORD					width;
	DWORD					height;

};




//////////////////////////////////////////////////////////////////////////////////////////////////
	

	
	
// forward definitions


// ---------- initialise device ----------

bool initialiseD3D (
	HINSTANCE          hInstance,		// [in] application instance
	char*              name,			// [in] application title bar name
	int                width,			// [in] window (backbuffer) height
	int                height,			// [in] window (backbuffer) height
	bool               windowed,		// [in] IF true THEN windowed ELSE fullscreen
	bool               desktop,			// [in] IF true THEN use desktop ELSE use window dimensions
	D3DDEVTYPE         deviceType,		// [in] HAL or REF
	devParam*          parameters,		// [out] device parameters
	IDirect3DDevice9** device);			// [out] created device


// ---------- display loop  ----------

int displayMsgLoop(bool (*ptr_display)(double timeDelta));


// ---------- queue message window process  ----------

LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg, 
	WPARAM wParam,
	LPARAM lParam);


// ---------- release reserved memory ----------
template<class T> void Release(T t)
{
	if( t )
	{
		t->Release();
		t = 0;
	}
}


// ---------- delete allocated memory ----------
template<class T> void Delete(T t)
{
	if( t )
	{
		delete t;
		t = 0;
	}
}


// ---------- colour ----------

const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
const D3DXCOLOR     ORANGE( D3DCOLOR_XRGB(255, 128,   0) );
const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );


// ---------- lights ----------

D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);


// ---------- materials ----------

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

const D3DMATERIAL9 WHITE_MTRL   = InitMtrl(WHITE,    WHITE,    WHITE,     BLACK, 2.0f);
const D3DMATERIAL9 BLACK_MTRL   = InitMtrl(BLACK,    BLACK,    BLACK,     BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL     = InitMtrl(RED,      RED,      RED,       BLACK, 2.0f);
const D3DMATERIAL9 ORANGE_MTRL  = InitMtrl(ORANGE,   ORANGE,   ORANGE,    BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL  = InitMtrl(YELLOW,   YELLOW,   YELLOW,    BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL   = InitMtrl(GREEN,    GREEN,    GREEN,     BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL    = InitMtrl(BLUE,     BLUE,     BLUE,      BLACK, 2.0f);
const D3DMATERIAL9 CYAN_MTRL    = InitMtrl(CYAN,     CYAN,     CYAN,      BLACK, 2.0f);
const D3DMATERIAL9 MAGENTA_MTRL = InitMtrl(MAGENTA,  MAGENTA,  MAGENTA,   BLACK, 2.0f);




// ********* END NAMESPACE *********




}




//////////////////////////////////////////////////////////////////////////////////////////////////

#endif // __d3dUtilityH__