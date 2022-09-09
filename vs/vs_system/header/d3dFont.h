//////////////////////////////////////////////////////////////////////////////////////////////////
//
// d3dFont.h: interface for the directx tainted CD3DFont - a texture-based font class
//
//////////////////////////////////////////////////////////////////////////////////////////////////




// ---------- NOTE : Copyright (c) 1999-2001 Microsoft Corporation. All rights reserved ----------




#ifndef _d3dFont_
#define _d3dFont_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




// ---------- include ----------

#include <tchar.h>
#include <D3D9.h>




// ---------- font creation flags ----------

#define D3DFONT_BOLD        0x0001
#define D3DFONT_ITALIC      0x0002
#define D3DFONT_ZENABLE     0x0004




// ---------- font rendering flags ----------

#define D3DFONT_CENTERED    0x0001
#define D3DFONT_TWOSIDED    0x0002
#define D3DFONT_FILTERED    0x0004




// ---------- texture-based font class ----------

class CD3DFont
{

	// font properties
    TCHAR   m_strFontName[80];            // Font properties
    DWORD   m_dwFontHeight;
    DWORD   m_dwFontFlags;

    LPDIRECT3DDEVICE9       m_pd3dDevice; // D3DDevice used for rendering
    LPDIRECT3DTEXTURE9      m_pTexture;   // d3d texture for this font
    LPDIRECT3DVERTEXBUFFER9 m_pVB;        // vertexBuffer for rendering text
    DWORD   m_dwTexWidth;                 // texture dimensions

    DWORD   m_dwTexHeight;
    FLOAT   m_fTextScale;
    FLOAT   m_fTexCoords[128-32][4];
 

    // stateblocks for setting and restoring render states
    LPDIRECT3DSTATEBLOCK9 m_pStateBlockSaved;
    LPDIRECT3DSTATEBLOCK9 m_pStateBlockDrawText;


public:


	// constructor / destructor
    CD3DFont( const TCHAR* strFontName, DWORD dwHeight, DWORD dwFlags=0L );
    ~CD3DFont();


	// 2D and 3D text drawing functions
    HRESULT DrawText( FLOAT x, FLOAT y,
						DWORD dwColor, 
						const TCHAR* strText,
						DWORD dwFlags=0L );
    HRESULT DrawTextScaled( FLOAT x, FLOAT y, FLOAT z, 
						FLOAT fXScale, FLOAT fYScale, DWORD dwColor, 
						const TCHAR* strText, DWORD dwFlags=0L );
    HRESULT Render3DText( const TCHAR* strText, DWORD dwColor, DWORD dwFlags=0L );
 

    // function to get extent of text
    HRESULT GetTextExtent( const TCHAR* strText, SIZE* pSize );


    // initialising and destroying device-dependent objects
    HRESULT InitDeviceObjects( LPDIRECT3DDEVICE9 pd3dDevice );
    HRESULT RestoreDeviceObjects();
    HRESULT InvalidateDeviceObjects();
    HRESULT DeleteDeviceObjects();


};




//////////////////////////////////////////////////////////////////////////////////////////////////




#endif // _d3dFont_

