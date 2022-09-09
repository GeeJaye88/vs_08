//////////////////////////////////////////////////////////////////////////////////////////////////
//
// dxCluster.h: interface for the directx tainted cluster class.
//
//////////////////////////////////////////////////////////////////////////////////////////////////




#ifndef _dxCluster_
#define _dxCluster_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000




// ---------- set specific definitions ----------

#define MAX_CLUSTER_OBJECTS  20




// ---------- includes directx9 ----------

#include <d3dx9.h>




// ---------- include ----------

#include "../../rtge/header/d3dUtility.h"
#include "../../rtge/header/d3dFont.h"
#include "../../rtge/header/dxObjects.h"
#include "../../rtge/header/gvLibrary.h"
#include "../../rtge/header/dxStage.h"




// ---------- string substitution ----------


#define FONT_3D_NORMAL_SCALE     0.012f
#define FONT_3D_NORMAL_TRANS_S   0.41f
#define FONT_3D_NORMAL_TRANS_Y   9.0f

#define FONT_3D_NAVIG_SCALE      0.018f

#define FONT_3D_TITLE_SCALE      0.060f
#define FONT_3D_TITLE_TRANS_S    0.41f
#define FONT_3D_TITLE_TRANS_Y    0.0f

#define FONT_3D_SUB_SCALE        0.012
#define FONT_3D_SUB_TRANS_S      0.61f
#define FONT_3D_SUB_TRANS_Y     -9.0f




//////////////////////////////////////////////////////////////////////////////////////////////////




// ********** BEGIN NAMESPACE **********

namespace dx
{




//////////////////////////////////////////////////////////////////////////////////////////////////




// ********** CLUSTER POSE **********


#define MAX_POSES 6

class clusterPose
{

	int      _channels;          // number of channels
	int      _length;	         // group length within a channel
	int      _currentOffset;     // current offset in channel
	int      _targetOffset;      // target offset in channel
	float    _imu, _mu;          // interpolant
	bool     _calculate;         // if true then interpolate

	gv::channel* _t[MAX_POSES];  // channels


public:


	// constructor /destructor
	clusterPose ();
	virtual ~clusterPose ();


	// set parameters
	void setParameters (int currentPose, int targetPose, float mu, int id)
	{
		_currentOffset = currentPose * _length + id;
		_targetOffset  = targetPose * _length + id;
		_imu           = 1.0-mu;
		_mu            = mu;
		_calculate     = _currentOffset == _targetOffset ? false : true;
	};


	// get a value from a channel
	float getChannel (int channel)
	{
		if (_calculate)
			return ( _imu*_t[channel]->getF(_currentOffset) + _mu*_t[channel]->getF(_targetOffset) );

		return _t[channel]->getF(_currentOffset);
	};

};




//////////////////////////////////////////////////////////////////////////////////////////////////




// ********** CLUSTER **********


class cluster  
{

	// id
	DWORD         _id;


	// data
	gv::node*     _clusterData;
	gv::node*     _defaultData;
	gv::node*     _userData;


	// list name and type
	char*         _listName;
	char*         _listType;


	// focii within data
	gv::node*     _currentFocus;
	gv::node*     _previousFocus;
	gv::node*     _mouseFocus;


	// font
	CD3DFont*     _font;
	CD3DFont*     _font3d;
	CD3DFont*     _fontSub;


	// font - color
	D3DXCOLOR     _fontNavigationColour;
	D3DXCOLOR     _fontNormalColour;
	D3DXCOLOR     _fontTitleColour;


	// font - position
	D3DXVECTOR3   _fontPos;
	D3DXVECTOR3   _projPos;


	// font materials
	D3DMATERIAL9  _text2DMaterial;
	D3DMATERIAL9  _text3DMaterial;
	D3DMATERIAL9  _nav3DMaterial;


	// cluster transform
	D3DXMATRIX    _clusterTransform;


	// cluster poses
	clusterPose   _clusterPose;


	// orbit
	float         _orbitRadius;


	// timers
	gv::timer     _open;
	gv::timer     _pose;
	gv::timer     _pause;
	gv::timer     _ghost;
	gv::timer     _inspect;


	// pose id
	int           _currentPose;
	int           _targetPose;


	// display states
	bool          _draw3dFont;
	bool          _pauseMode;
	bool          _ghostMode;
	bool          _gridVisible;
	bool          _imageVisible;
	bool          _tumble;
	bool          _rayFocus;


	// misc
	int           _seed;


	// stacks
	DWORD         _totalObjects;


	// grid
	object_3D_D*  _grid;


	// stacks
	gv::node*     _node     [MAX_CLUSTER_OBJECTS];
	dx::sphere*   _sphere   [MAX_CLUSTER_OBJECTS];
	char          _object   [MAX_CLUSTER_OBJECTS];


	// resource - arrow objects
	object_3D_N*  _up;
	object_3D_N*  _previous;
	object_3D_N*  _next;


	// resource - objects & matrix
	object_3D_N*  _box;
	object_3D_N*  _disk;
	object_3D_N*  _hexagon;
	object_3D_N*  _pentagon;
	object_3D_N*  _triangle;
	object_3D_N*  _spoke;


	// texture
	LPDIRECT3DTEXTURE9  _tplaneTexture ;


	// plane
	objectRectangle_3D_NT* _tplane;


public:


	// constructor /destructor
	cluster (gv::node*);
	virtual ~cluster ();


	void initialise ()
	{
		// TBF - causes a crash
		// memset(this,0,sizeof(cluster));

		_id              = 0;

		_clusterData      = 0;
		_defaultData     = 0;
		_userData        = 0;

		_currentFocus    = 0;
		_previousFocus   = 0;
		_mouseFocus      = 0;

		_draw3dFont      = false;			// NOTE - 2d text is the default

		_font            = 0;
		_fontSub         = 0;
		_font3d          = 0;

		_ghostMode       = false;
		_pauseMode       = false;
		_gridVisible     = true;			// NOTE - visible grid is the default
		_imageVisible    = true;			// NOTE - visible images is the default
		_tumble          = false;
		_rayFocus        = false;

		_orbitRadius     = 2.0;

		_open.length     = 1.5;
		_open.running    = true;
		_pause.length    = 0.5;
		_ghost.length    = 2.5;
		_pose.length     = 2.5;
		_inspect.length  = 1.0;

		_currentPose     = 0;
		_targetPose      = 0;

		_seed            = 100;

	}


	// get default data
	gv::node* getDefaultData();


	// framework
	bool cleanup     ();
	bool cleanupDX   ();
	bool display     (dx::stage*);
	bool setup       (int);
	bool setupDX     (dx::stage*);
	bool zIndex      (dx::stage*);


	// display update methods
	DWORD  update        (gv::mouse*, gv::time*);
	DWORD  updateStack   ();
	void   updatePickRay (dx::ray*, int, D3DXMATRIX*, float);
	void   updateState   (dx::stage*);
};




//////////////////////////////////////////////////////////////////////////////////////////////////




// ********** END NAMESPACE **********




}




//////////////////////////////////////////////////////////////////////////////////////////////////




#endif // _dxCluster_
