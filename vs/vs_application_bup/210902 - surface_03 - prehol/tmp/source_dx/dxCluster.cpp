//////////////////////////////////////////////////////////////////////////////////////////////////
//
// dxCluster.cpp: implementation of the directx tainted cluster class
//
//////////////////////////////////////////////////////////////////////////////////////////////////




// ---------- include standard library for srand, RAND_MAX, etc. ----------

#include <stdlib.h>




// ---------- include ----------

#include "../../rtge/header/gvUtility.h"
#include "../../sets/header/dxCluster.h"




// ---------- forward declaration ----------

class transform;




/////////////////////////////////////////////////////////////////////////////////////////////////




// ********** CLUSTER **********




// ---------- framework : constructor : instantiated by load thread ----------
//
// Note: Set data focii:
//
//       1. _defaultData - node* to default node structure
//       2. _userData    - node* to the 'cluster' node
//       3. _clusterData  - node* to the 'data' node
//       4. _listName    - char* to the list name
//       5. _listType    - char* to the list type
//
//       Then:
//
//       1. Set node type      ( see node setType method )
//       2. Create connections ( see node connect method )
//       3. Sort all lists     ( see node sort method    )
//       4. Cluster all lists  ( see node cluster method )
//

dx::cluster::cluster (gv::node* data)
{

	initialise ();


	//---------- part 1 : data ----------


	// get default data
	_defaultData = this->getDefaultData();


	// set userdata & if no data then default
	_userData = data->getPtr("cluster");
	if (data == 0)
	{
		_userData = _defaultData;
	}


	// default names
	_listName = "";
	_listType = "";


	// NOTE : ASSUME A WELL FORMED DATA FILE


	// set focii
	_clusterData   = data->getPtr("data");
	_clusterData   = _clusterData->first;
	_currentFocus = _clusterData;


	// names
	_listName = _clusterData->name;
	_listType = _clusterData->first->name;


	// set previous and mouse focus
	_previousFocus = _currentFocus->parent;
	_mouseFocus    = 0;


	//---------- part 2 : format data and connect ----------


	// recursively set ALL node types - 'r'oot, 'n'ode, 't'wig & 'l'eaf
	_userData->parent->setType();


	// if there is data
	if (_clusterData)
	{

		// connect ONLY cluster data	- add new lists of lists of lists
		_clusterData->connect(_clusterData);


		// sort ONLY cluster data - 
		_clusterData->parent->sort();


		// cluster ONLY cluster data - 'b'ranch
		//
		// Note: parent + next sibling + last sibling + 9 children = 12
		//       objects, which at current size and radius seems confortable
		//
		_clusterData->cluster (9,9," - ");

	}


	//---------- part 3 : initialise properties ----------


	// seed
	_seed  = atoi(_userData->getValueOrDefault("seed",_defaultData) );

}




// ---------- framework : destructor ----------

dx::cluster::~cluster ()
{
	// nothing to delete or release
}




// ********** BEGIN FRAMEWORK METHODS **********




// ---------- framework : cleanup ----------

bool dx::cluster::cleanup ()
{


	// delete object_3d_D objects
	d3d::Delete<object_3D_D*>(_grid);


	// delete object_3D_N objects
	d3d::Delete<object_3D_N*>(_up);
	d3d::Delete<object_3D_N*>(_previous);
	d3d::Delete<object_3D_N*>(_next);
	d3d::Delete<object_3D_N*>(_pentagon);
	d3d::Delete<object_3D_N*>(_triangle);
	d3d::Delete<object_3D_N*>(_disk);
	d3d::Delete<object_3D_N*>(_hexagon);
	d3d::Delete<object_3D_N*>(_box);
	d3d::Delete<object_3D_N*>(_spoke);


	// ok
	return true;
}




// ---------- framework : cleanup dx ----------

bool dx::cluster::cleanupDX ()
{

	// delete textures
	d3d::Delete<LPDIRECT3DTEXTURE9>(_tplaneTexture);


	// release dx objects
	_grid     ->releaseDX();
	_up       ->releaseDX();
	_previous ->releaseDX();
	_next     ->releaseDX();
	_pentagon ->releaseDX();
	_triangle ->releaseDX();
	_disk     ->releaseDX();
	_hexagon  ->releaseDX();
	_box      ->releaseDX();
	_spoke    ->releaseDX();


	// delete fonts
	d3d::Delete<CD3DFont*>(_font);
	d3d::Delete<CD3DFont*>(_font3d);
	d3d::Delete<CD3DFont*>(_fontSub);


	// ok
	return true;
}




// ---------- framework : display ----------
//
// cluster
// ------
//
// Part 1 : housekeeping
//      2 : get required matrices and calculate world transform
//      3 : if not visible then cull this set
//      4 : draw grid
//      5 : set scaling
//      6 : update cluster
//      7 : update pick ray
//      8 : for each object
//          1 - transform
//          2 - if not visible then cull this object
//          3 - mouse over scale, open and inspect flip 
//          4 - draw object
//          5 - draw arrow
//          6 - draw arrow text
//          7 - draw spokes
//          8 - initialise draw text 
//          9 - draw text
//      9 : tidy up
//
// Note: Needs a lot of tidying up - but at present it runs fast enough - and
//       as is - it has a clear structure
//
//     : todo - move object declarations and initialisation to after the
//       the set either has, or has not, been culled
//
//     : todo - implement the arguments bag - one structure that will contain
//       all the info passed to each sets implementaion of this method
//
//     : todo - pass the device parameters (e.g. height, width)
//
//     : todo - cleanup all hardcoded parameters
//

bool dx::cluster::display (dx::stage* stage)
{



	// ---------- part 1 : messages ----------


	// IF there is a plinth THEN plinth = true
	gv::message* m = stage->messageBox->get(3);
	bool plinth = false;
	if (m!=0)
	{
		plinth = m->data.boolValue;
	}



	// ---------- part 2 : IF cluster not within field of view THEN return ----------

	
	// test bounding box against fov
	bool withinFOV = stage->boundingBoxWithinFOV (&stage->viewMatrix, &_clusterTransform, -5.0, 5.0, -5.0, 5.0, -0.5, 0.5);
	if (!withinFOV){
		return true;
	}


	// ---------- part 3 : unpack the stage ----------


	// shortcuts to stage properties
	IDirect3DDevice9* device     = stage->device;
	gv::keyboard*     keyboard   = stage->keyboard;
	gv::lens*         lens       = stage->lens;
	gv::mouse*        mouse      = stage->mouse;
	dx::ray*          ray        = stage->ray;
	gv::time*         time       = stage->time;
	char*             path       = stage->path;


	
	// ---------- part 4 : local variables ----------


	// cluster only scaling
	float  scaling = 1.5;


	// font - draw sub text state
	bool   arrowText;
	bool   drawText;
	bool   drawSubText;


	// font - texture sizes (extent)
	SIZE  textFS, subTextFS;


	// font - matrix
	D3DXMATRIX fontTransform;


	// utility transform
	D3DXMATRIX rotateX;
	D3DXMATRIX rotateY;
	D3DXMATRIX rotateZ;
	D3DXMATRIX translateXYZ;
	D3DXMATRIX scaleXYZ;


	// object scaling transform
	D3DXMATRIX scalingTransform;


	// specific object transforms
	D3DXMATRIX objectMatrix;
	D3DXMATRIX spokeTransform;
	D3DXMATRIX arrowTransform;


	// resulting object transform
	D3DXMATRIX objectTransform;
	D3DXMATRIX inspectTransform;
	D3DXMATRIX tempTransform;


	// ---------- part 5 : update the various state flags ----------

	
	// update states
	updateState (stage);


	// ---------- part 6 : draw grid ----------


	// default turntable transform
	device->SetTransform(D3DTS_WORLD, &_clusterTransform);


	// draw grid
	if (_gridVisible)
	{
		_grid->draw(device);
	}


	// ---------- part 7 : set scalar ----------


	// scaling transform
	D3DXMatrixScaling       ( &tempTransform, scaling, scaling, scaling );
	D3DXMatrixMultiply      ( &scalingTransform, &tempTransform, &_clusterTransform );


	// ---------- part 8 : update node cluster ----------


	// update the cluster
	_totalObjects = update (mouse, time);

 
	// ---------- part 9 : update pick ray ----------


	// update the cluster via the pick ray
	updatePickRay (ray, _totalObjects, &scalingTransform, scaling);


	// ---------- part 10: for each object transform, draw, annotate ----------


	// utility text buffers
	char  subText[100];
	char  text[100];


	// font view space z depth cue
	float fontDepthCueMin       = atof(stage->prop->pixelFog[PROP_FOG_START]->value)-30;
	float fontDepthCueMax       = fontDepthCueMin + 15;
	float fontDepthCueTitleMin  = fontDepthCueMin - 15;
	float fontDepthCueTitleMax  = fontDepthCueMax + 50;
	float fontDepthCueDiff      = fontDepthCueMax      - fontDepthCueMin;
	float fontDepthCueTitleDiff = fontDepthCueTitleMax - fontDepthCueTitleMin;


	// multi-sampling
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);

	// for each object
	for (int object=0; object<_totalObjects; object++)
	{


		// ---------- part 10.1 : transform ----------


		// local transform
		D3DXMatrixTranslation( &objectMatrix, _sphere[object]->getX(), _sphere[object]->getY(), _sphere[object]->getZ());
		D3DXMatrixMultiply   ( &objectTransform, &objectMatrix, &scalingTransform );


		// locus into view space
		D3DXVECTOR3 tlocus = D3DXVECTOR3( objectTransform.m[3][0], objectTransform.m[3][1], objectTransform.m[3][2]);
		D3DXVec3TransformCoord(&tlocus, &tlocus, &stage->viewMatrix);


		// ---------- part 10.2 : if not visible then cull this object ----------


		// IF object is in front of the camera THEN draw ELSE cull
		//
		// Note: this block of code with camera at origin looking left
		//       and viewing 5 cluster data sets with only 10 visible nodes 
		//       and a fps overlay increases the frame rate from 120 to 176
		//
		if (tlocus.z > 0.0)
		{

			// or each edge of the viewport
			float x  =  tlocus.x;
			float y  =  tlocus.y;
			float z  =  tlocus.z;
			float r  = _sphere[object]->getRadius() * scaling;


			// test bounding sphere against fov
			withinFOV = stage->boundingSphereWithinFOV (x,y,z,r);


			// THEN if within
			if (withinFOV)
			{

				// distance to object
				float d = sqrt( x*x + y*y + z*z );


				// distance == alpha value
				float fontAlpha      = d > fontDepthCueMax ? 0.0 : d < fontDepthCueMin ? 1.0 : 1.0 - ((d-fontDepthCueMin)/fontDepthCueDiff);
				float titleFontAlpha = 1.0 - fontAlpha;
				if (d > fontDepthCueTitleMin)
				{
					titleFontAlpha = d > fontDepthCueTitleMax ? 0.0 : 
						d < fontDepthCueTitleMin ? titleFontAlpha : 1.0 - ((d-fontDepthCueTitleMin)/fontDepthCueTitleDiff);
				}


				// is there a plinth
				if (plinth)
				{
					float oy = objectTransform.m[3][1];
					fontAlpha      = oy > -4.5 ? fontAlpha :      oy < -5.5 ? 0.0 : fontAlpha      * (oy+5.5);
					titleFontAlpha = oy > -4.5 ? titleFontAlpha : oy < -5.5 ? 0.0 : titleFontAlpha * (oy+5.5);
				}


				// colour alpha == alpha value
				_fontNavigationColour.a  = fontAlpha;
				_fontNormalColour.a      = fontAlpha;
				_fontTitleColour.a       = titleFontAlpha;


				// ---------- part 10.3 : mouse over scale, open and inspect flip ----------


				// IF this is mouse focus THEN scale
				if (_node[object] == _mouseFocus && ! _open.running)
				{

					// IF rayFocus ('X' key pressed - e'X'amine) THEN update ray vectors
					if (_rayFocus)
					{

						// set local ray focus false
						_rayFocus = false;


						// set stage ray focus true
						ray->focus = true;


						// initialise ray axis


						D3DXVECTOR3 tsphere = D3DXVECTOR3( _sphere[object]->getX(), _sphere[object]->getY(), _sphere[object]->getZ() );
						D3DXVec3TransformCoord(&tsphere, &tsphere, &scalingTransform);
						ray->focusAxis.position = tsphere;

						D3DXVECTOR3 torigin = D3DXVECTOR3( 0.0, 0.0, 0.0);
						D3DXVec3TransformCoord(&torigin, &torigin, &scalingTransform);

						D3DXVECTOR3 tright = D3DXVECTOR3( 1.0, 0.0, 0.0);
						D3DXVec3TransformCoord(&tright, &tright, &scalingTransform);
						ray->focusAxis.right = tright-torigin;

						D3DXVECTOR3 tup = D3DXVECTOR3( 0.0, 1.0, 0.0);
						D3DXVec3TransformCoord(&tup, &tup, &scalingTransform);
						ray->focusAxis.up = tup - torigin;

						D3DXVECTOR3 tlook = D3DXVECTOR3( 0.0, 0.0, 1.0);
						D3DXVec3TransformCoord(&tlook, &tlook, &scalingTransform);
						ray->focusAxis.look = tlook - torigin;

						D3DXVECTOR3 tnormal = D3DXVECTOR3( 0.0, 0.0, -1.0);
						D3DXVec3TransformCoord(&tnormal, &tnormal, &scalingTransform);
						ray->focusAxis.normal = tnormal - torigin;

					}


					// scale
					if (_mouseFocus == _currentFocus)
						D3DXMatrixScaling    ( &scaleXYZ, 1.65f, 1.65f, 1.0f);
					else
						D3DXMatrixScaling    ( &scaleXYZ, 1.15f, 1.15f, 1.0f);

					D3DXMatrixMultiply   ( &objectTransform, &scaleXYZ, &objectTransform );

				}

				// IF this is current focus THEN scale
				else if (_node[object] == _currentFocus)
				{
					D3DXMatrixScaling    ( &scaleXYZ, 1.3f, 1.3f, 1.0f);
					D3DXMatrixMultiply   ( &objectTransform, &scaleXYZ, &objectTransform );
				}


				// rotate by TIMER_FLIP during open
				D3DXMatrixRotationY  ( &rotateY, _open.TIMER_FLIP * RADIAN);
				D3DXMatrixMultiply   ( &objectTransform, &rotateY, &objectTransform );


				// IF object is inspect focus && inspect is running THEN
				//     rotate by TIMER_FLIP during inspect and
				//     update world transform
				// ELSE
				//     update world transform
				//
				if (_node[object] == _inspect.focus)
				{
					D3DXMatrixRotationY  ( &rotateY, _inspect.TIMER_FLIP * RADIAN);
					D3DXMatrixMultiply   ( &inspectTransform, &rotateY, &objectTransform );
					device->SetTransform ( D3DTS_WORLD, &inspectTransform);
				}
				else
				{
					device->SetTransform ( D3DTS_WORLD, &objectTransform);
				}


				// ---------- part 10.4 : draw object ----------


				// refereced object && type of object - 'r'oot, 'n'ode, 'b'ranch, 't'wig or 'l'eaf
				gv::node* nodeRef;


				// IF there is a link THEN use link ELSE use node 
				if(_node[object]->link)
				{
					nodeRef = _node[object]->link;
				}
				else
				{
					nodeRef = _node[object];
				}
				char nodeType = nodeRef->type;


				// draw object 'r'oot, 'n'ode, 'b'ranch, 't'wig or 'l'eaf
				switch (nodeType)
				{


				// ***** draw 'r'oot, 'n'ode or 'b'ranch objects as discs *****
				//

				default :
					_disk->draw(device);
					break;


				// ***** draw twig object *****
				//
				// ALSO - IF _inspect.focus OR _mouseFocus THEN draw image texture plane
				//
				// NOTE - the _inspect state must be reset when a new cluster is opened
				//

				case 't':
					_hexagon->draw(device);


					// is this image visible
					bool thisImageVisible;


					// IF images visible then ignore inspect mode THEN images are always visible
					if (_imageVisible)
					{
						thisImageVisible = true;
					}

					// ELSE
					//    IF object == inspect object OR object == mouse focus THEN image is visible
					else
					{
						thisImageVisible = _node[object] == _inspect.focus || _node[object] == _mouseFocus;
					}


					// if picture visible
					if (thisImageVisible)
					{

						// ---------- if picture visible : texture ----------

						LPDIRECT3DTEXTURE9 _tempTexture = 0;


						// IF there is not an icon THEN create one
						if (!nodeRef->icon)
						{
							nodeRef->icon = new gv::icon();
						}


						// IF there is an icon THEN get texture
						gv::icon* icon = nodeRef->icon;
						if (!icon->large)
						{

							// get image filename
							char* filename = nodeRef->getValue("Image");


							// IF there is a media filename THEN create texture
							if (filename)
							{

								// build texture projectr path and filename
								char* paf = stage->projectPAF( "images/", filename);


								// create etexture
								D3DXCreateTextureFromFile( stage->device, paf, &_tempTexture );
								icon->large = int(_tempTexture);

							}

							// ELSE default
							else
							{
								icon->large = int(_tplaneTexture);
							}

						}


						// get texture
						_tempTexture = LPDIRECT3DTEXTURE9(icon->large);


						// ---------- if picture visible : draw object ----------


						// default size and rotation
						float tpaul = 0.5f;
						float tsize = _imageVisible ? 0.4 : tpaul;
						float trot  = 180.0;


						// IF this object is the inspect focus THEN animate ELSE zap text
						if (_node[object] == _inspect.focus)
						{
							tsize = _inspect.state ? _inspect.mu * tpaul : _inspect.imu * tpaul;
							trot  = _inspect.TIMER_FLIP;
						}
						else
						{
							fontAlpha = 0.0;
						}


						// update plane
						_tplane->update(device, -tsize, tsize,  tsize, -tsize, 0.0);


						// part 3 : draw 'front'
						D3DXMatrixRotationY   ( &rotateY, (trot+180) * RADIAN);
						D3DXMatrixMultiply    ( &inspectTransform, &rotateY,      &objectTransform );
						D3DXMatrixRotationZ   ( &rotateZ, float(180.0 * RADIAN) );
						D3DXMatrixMultiply    ( &inspectTransform, &rotateZ,      &inspectTransform );
						D3DXMatrixTranslation ( &translateXYZ, 0.0, 0.0,  0.07f);
						D3DXMatrixMultiply    ( &inspectTransform, &translateXYZ, &inspectTransform );
						device->SetTransform  ( D3DTS_WORLD, &inspectTransform);
						_tplane->draw(device, _tempTexture);


						// part 4 : draw 'back'
						D3DXMatrixRotationY   ( &rotateY, trot * RADIAN);
						D3DXMatrixMultiply    ( &inspectTransform, &rotateY,      &objectTransform );
						D3DXMatrixRotationZ   ( &rotateZ, float(180.0 * RADIAN) );
						D3DXMatrixMultiply    ( &inspectTransform, &rotateZ,      &inspectTransform );
						D3DXMatrixTranslation ( &translateXYZ, 0.0, 0.0,  -0.07f);
						D3DXMatrixMultiply    ( &inspectTransform, &translateXYZ, &inspectTransform );
						device->SetTransform  ( D3DTS_WORLD, &inspectTransform);
						_tplane->draw(device, _tempTexture);

					}
					break;

				// ***** draw 'l'eaf objects as boxes *****
				//

				case 'l':
					_box->draw(device);
					break;
				}


				// ---------- part 10.7 : draw spokes ----------


				// IF this is the focus object THEN draw spokes
				if (object == _totalObjects-1)
				{

					// draw spokes
					DWORD maxSpokes = _totalObjects-1;
					float inc =   0.0;
					float ang =   90.0;
					if (maxSpokes > 1)
					{
					inc = 360.0 / float(maxSpokes);
					}
					for (DWORD a=0; a<=maxSpokes; a++)
					{

						// align with object
						D3DXMatrixMultiply   ( &spokeTransform, &objectMatrix, &scalingTransform);

						// rotate to point 
						D3DXMatrixRotationZ  ( &rotateY,ang * RADIAN);
						D3DXMatrixMultiply   ( &spokeTransform, &rotateY, &spokeTransform);

						// translate to scale
						D3DXMatrixTranslation( &translateXYZ,  _open.TIMER_SCALE * 1.0, 0.0, 0.0);
						D3DXMatrixMultiply   ( &spokeTransform, &translateXYZ, &spokeTransform);

						// scale
						D3DXMatrixScaling    ( &scaleXYZ, _open.TIMER_SCALE, 1.0, 1.0);
						D3DXMatrixMultiply   ( &spokeTransform, &scaleXYZ, &spokeTransform);

						// draw
						device->SetTransform(D3DTS_WORLD, &spokeTransform);
						_spoke->draw(device);

						// increment angle
						ang += inc;
					}

				} // ENDIF focus object


				// ---------- part 10.5 : draw arrow ----------


				// transform & draw arrows
				switch (_object[object])
				{
				case 'u' :
					strcpy (text,"UP");
					arrowText = true;
					D3DXMatrixTranslation ( &translateXYZ, 0.0, _open.TIMER_SCALE * 0.5, 0.0);
					D3DXMatrixMultiply    ( &arrowTransform, &translateXYZ, &objectTransform );
					device->SetTransform  ( D3DTS_WORLD, &arrowTransform);
					_up->draw(device);
					break;
				case 'p' :
					strcpy (text,"LAST");
					arrowText = true;
					D3DXMatrixRotationY   ( &rotateY, - _open.TIMER_FLIP * RADIAN);
					D3DXMatrixMultiply    ( &arrowTransform, &rotateY, &objectTransform );
					D3DXMatrixRotationZ   ( &rotateZ,float(-90.0 * RADIAN));
					D3DXMatrixMultiply    ( &arrowTransform, &rotateZ, &arrowTransform );
					D3DXMatrixTranslation ( &translateXYZ, 0.0, _open.TIMER_SCALE * 0.5, 0.0);
					D3DXMatrixMultiply    ( &arrowTransform, &translateXYZ, &arrowTransform );
					device->SetTransform  ( D3DTS_WORLD, &arrowTransform);
					_previous->draw(device);
					break;
				case 'n' :
					strcpy (text,"NEXT");
					arrowText = true;
					D3DXMatrixRotationY   ( &rotateY, - _open.TIMER_FLIP * RADIAN);
					D3DXMatrixMultiply    ( &arrowTransform, &rotateY, &objectTransform );
					D3DXMatrixRotationZ   ( &rotateZ, float(90.0 * RADIAN));
					D3DXMatrixMultiply    ( &arrowTransform, &rotateZ, &arrowTransform );
					D3DXMatrixTranslation ( &translateXYZ, 0.0, _open.TIMER_SCALE * 0.5, 0.0);
					D3DXMatrixMultiply    ( &arrowTransform, &translateXYZ, &arrowTransform );
					device->SetTransform  ( D3DTS_WORLD, &arrowTransform);
					_next->draw(device);
					break;
				default:
					arrowText = false;
					break;
				}


				// ---------- part 10.6 : draw arrow text ----------


				// IF arrow text
				if (arrowText)
				{

					// IF draw 3d font THEN transform & draw 3d text
					_font->GetTextExtent(text, &textFS);
					if (_draw3dFont)
					{

						if (fontAlpha > 0.0)
						{
							for (int i=0; i<2; i++)
							{
								switch (_object[object])
								{
								case 'u' :
									{
										D3DXMatrixRotationY   ( &rotateY, float(i)*180.0*RADIAN);
										D3DXMatrixScaling     ( &scaleXYZ,
																FONT_3D_NAVIG_SCALE,
																FONT_3D_NAVIG_SCALE,
																FONT_3D_NAVIG_SCALE );
										D3DXMatrixTranslation ( &translateXYZ,
																-float(textFS.cx/2)*FONT_3D_NORMAL_TRANS_S,
																-float(textFS.cy/2-FONT_3D_NORMAL_TRANS_Y)*FONT_3D_NORMAL_TRANS_S + 36.0,
																- (_sphere[object]->getRadius() * scaling) );
										D3DXMatrixMultiply    ( &tempTransform,  &rotateY,       &scaleXYZ );
										D3DXMatrixMultiply    ( &tempTransform,  &translateXYZ,  &tempTransform );
										D3DXMatrixMultiply    ( &tempTransform,  &tempTransform, &arrowTransform );
									}
									break;
								case 'p' :
									{
										float angle = float(i)*180.0;
										float xoff = i == 0 ? 45.0 : -45.0;
										D3DXMatrixRotationZ   ( &rotateZ, float(90.0 * RADIAN) );
										D3DXMatrixRotationY   ( &rotateY, angle * RADIAN);
										D3DXMatrixTranslation ( &translateXYZ,
																-float(textFS.cx/2) * FONT_3D_NORMAL_TRANS_S + xoff,
																-float(textFS.cy/2-FONT_3D_NORMAL_TRANS_Y) * FONT_3D_NORMAL_TRANS_S - 3.5,
																- (_sphere[object]->getRadius() * scaling) );
										D3DXMatrixScaling     ( &scaleXYZ,
																FONT_3D_NAVIG_SCALE,
																FONT_3D_NAVIG_SCALE,
																FONT_3D_NAVIG_SCALE);
										D3DXMatrixMultiply    ( &tempTransform,  &rotateZ,       &scaleXYZ );
										D3DXMatrixMultiply    ( &tempTransform,  &rotateY,       &tempTransform );
										D3DXMatrixMultiply    ( &tempTransform,  &translateXYZ,  &tempTransform );
										D3DXMatrixMultiply    ( &tempTransform,  &tempTransform, &arrowTransform );
									}
									break;
								case 'n' :
									{
										float angle = float(i)*180.0;
										float xoff = i == 0 ? -45.0 : 45.0;
										D3DXMatrixRotationZ   ( &rotateZ, float(-90.0 * RADIAN) );
										D3DXMatrixRotationY   ( &rotateY, angle * RADIAN);
										D3DXMatrixTranslation ( &translateXYZ,
																-float(textFS.cx/2) * FONT_3D_NORMAL_TRANS_S + xoff,
																-float(textFS.cy/2-FONT_3D_NORMAL_TRANS_Y) * FONT_3D_NORMAL_TRANS_S - 3.5,
																- (_sphere[object]->getRadius() * scaling) );
										D3DXMatrixScaling     ( &scaleXYZ,
																FONT_3D_NAVIG_SCALE,
																FONT_3D_NAVIG_SCALE,
																FONT_3D_NAVIG_SCALE);
										D3DXMatrixMultiply    ( &tempTransform,  &rotateZ,       &scaleXYZ );
										D3DXMatrixMultiply    ( &tempTransform,  &rotateY,       &tempTransform );
										D3DXMatrixMultiply    ( &tempTransform,  &translateXYZ,  &tempTransform );
										D3DXMatrixMultiply    ( &tempTransform,  &tempTransform, &arrowTransform );
									}
								break;
								default :
									break;
								}
								_nav3DMaterial.Diffuse.a = fontAlpha;
								device->SetMaterial   ( &_nav3DMaterial);
								device->SetTransform  ( D3DTS_WORLD, &tempTransform);
								_font3d->Render3DText ( text,0);
							}
						}
					}


					// ELSE draw 2d text
					else
					{
						if (fontAlpha > 0.0)
						{
							D3DXVECTOR3 tempPos = D3DXVECTOR3( 0.0f, 0.7f, 0.0f);
							D3DXVECTOR3 temp    = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
							D3DXVec3Project ( &temp,&tempPos,&stage->viewPort,&stage->projMatrix,&stage->viewMatrix,&arrowTransform);
							_font->DrawText(int ( temp.x)-(textFS.cx/2), int(temp.y)-(textFS.cy/2), _fontNavigationColour, text, 0);
						}
					}


				} // ENDIF arrowText


				// ---------- part 10.8 : initialise draw text ----------


				// text & texture
				drawText    = true;
				drawSubText = false;


				// build text
				switch (nodeType)
				{

				case 'l' : //'l'eaf
					strncpy(text, _node[object]->name,99);
					sprintf(subText, "( %s )", text);
					_fontSub->GetTextExtent(subText, &subTextFS);
					strncpy(text, _node[object]->value,99);
					_font->GetTextExtent(text, &textFS);
					drawSubText = true;
					break;

				case 't' : // 't'wig
					strncpy(text, _node[object]->name,99);
					_font->GetTextExtent(text, &textFS);
					sprintf (subText,"( %s )",_listType);
					_fontSub->GetTextExtent(subText, &subTextFS);
					drawSubText = true;
				break;

				default  :// or a 'r'oot, 'n'ode, or 'b'ranch
					strncpy(text, _node[object]->name,99);
					_font->GetTextExtent(text, &textFS);
					break;

				}


				// ---------- part 10.9 : draw text ----------


				// IF text
				if (drawText)
				{

					// IF 3d text
					if (_draw3dFont)
					{

						// IF visible
						if (fontAlpha > 0.0)
						{

							// THEN draw text on each side of an object
							for (int i=0; i<2; i++)
							{

								// flip the coin
								float angle =  float(i)*180.0f;
								float yxtra =  0.0;
								float zxtra = -5.0;
								float axtra = fontAlpha;


								// if flip side of inspect item
								if (i && _inspect.focus == _node[object])
								{
									if (_inspect.state)
									{
										yxtra = (float)_inspect.mu * 60.0;
										axtra = (float)_inspect.imu * fontAlpha;
									}
									else
									{
										yxtra = (float)_inspect.imu * 60.0;
										axtra = (float)_inspect.mu * fontAlpha;
									}
								}


								// transform text
								D3DXMatrixRotationY   ( &rotateY, angle * (float)RADIAN);
								D3DXMatrixTranslation ( &translateXYZ,
										-float(textFS.cx/2)*FONT_3D_NORMAL_TRANS_S,
										-float(textFS.cy/2-FONT_3D_NORMAL_TRANS_Y)*FONT_3D_NORMAL_TRANS_S - yxtra,
										zxtra );
								D3DXMatrixScaling     ( &scaleXYZ, FONT_3D_NORMAL_SCALE, FONT_3D_NORMAL_SCALE, FONT_3D_NORMAL_SCALE);
								D3DXMatrixMultiply    ( &fontTransform,  &rotateY,       &scaleXYZ );
								D3DXMatrixMultiply    ( &fontTransform,  &translateXYZ,  &fontTransform );
								D3DXMatrixMultiply    ( &fontTransform,  &fontTransform, &objectTransform );


								// draw text
								_text3DMaterial.Diffuse.a = axtra;
								device->SetMaterial   ( &_text3DMaterial);
								device->SetTransform  ( D3DTS_WORLD, &fontTransform);
								_font3d->Render3DText ( text,0);


								// IF there is sub text THEN draw
								if (drawSubText)
								{
									D3DXMatrixRotationY   ( &rotateY, angle * (float)RADIAN);
									D3DXMatrixTranslation ( &translateXYZ,
											-float(subTextFS.cx/2)*FONT_3D_SUB_TRANS_S,
											-float(subTextFS.cy/2-FONT_3D_SUB_TRANS_Y)*FONT_3D_SUB_TRANS_S  - yxtra,
											zxtra );
									D3DXMatrixScaling     ( &scaleXYZ, float(FONT_3D_SUB_SCALE), float(FONT_3D_SUB_SCALE), float(FONT_3D_SUB_SCALE));
									D3DXMatrixMultiply    ( &fontTransform,  &rotateY,       &scaleXYZ );
									D3DXMatrixMultiply    ( &fontTransform,  &translateXYZ,  &fontTransform );
									D3DXMatrixMultiply    ( &fontTransform,  &fontTransform, &objectTransform );
									_text3DMaterial.Diffuse.a = axtra;
									device->SetMaterial   ( &_text3DMaterial);
									device->SetTransform  ( D3DTS_WORLD, &fontTransform);
									_font3d->Render3DText ( subText,0);
								}
							}


						} // ENDIF visible


						// IF current object == current focus && title font visible THEN draw title name
						if (_currentFocus == _node[object] && titleFontAlpha > 0.0)
						{

							// THEN draw title name on each side of an object
							_font->GetTextExtent(_listName, &textFS);
							for (int i=0; i<2; i++)
							{
								// flip the coin
								float angle = float(i)*180.0f;


								// transform text
								D3DXMatrixRotationY   ( &rotateY, angle * (float)RADIAN);
								D3DXMatrixTranslation ( &translateXYZ,
										-float(textFS.cx/2)*FONT_3D_TITLE_TRANS_S,
										-float(textFS.cy/2-FONT_3D_TITLE_TRANS_Y)*FONT_3D_TITLE_TRANS_S,
										- (_sphere[object]->getRadius() * scaling) );
								D3DXMatrixScaling     ( &scaleXYZ, FONT_3D_TITLE_SCALE, FONT_3D_TITLE_SCALE, FONT_3D_TITLE_SCALE);
								D3DXMatrixMultiply    ( &fontTransform,  &rotateY,       &scaleXYZ        );
								D3DXMatrixMultiply    ( &fontTransform,  &translateXYZ,  &fontTransform   );
								D3DXMatrixMultiply    ( &fontTransform,  &fontTransform, &objectTransform );


								// draw text
								_text3DMaterial.Diffuse.a = titleFontAlpha;
								device->SetMaterial   ( &_text3DMaterial);
								device->SetTransform  ( D3DTS_WORLD, &fontTransform);
								_font3d->Render3DText ( _listName,0);

							}
						}
					}


					// ELSE must be 2d text
					else
					{

						// IF visible
						if (fontAlpha > 0.0)
						{

							// THEN project a vector (sphere centre) from object space into screen space
							D3DXVECTOR3 temp = D3DXVECTOR3( 0.0, 0.0, 0.0);
							D3DXVec3Project ( &temp,&_fontPos,&stage->viewPort,&stage->projMatrix,&stage->viewMatrix,&objectTransform);


							// animate the inspect item text
							float yxtra = _inspect.focus == _node[object] ? 120.0f : 0.0f;
							yxtra = _inspect.state ? (float)_inspect.mu * yxtra : (float)_inspect.imu * yxtra ;


							// THEN set material and draw text
							device->SetMaterial ( &_text2DMaterial);
							_font->DrawText(int ( temp.x)-((float)textFS.cx/2), int(temp.y)-((float)textFS.cy/2)-7+yxtra, _fontNormalColour, text, 0);
							if (drawSubText)
							{
								_fontSub->DrawText(int ( temp.x)-((float)subTextFS.cx/2), int(temp.y)-((float)subTextFS.cy/2)+10+yxtra, _fontNormalColour, subText, 0);
							}

						}


						// IF current object == current focus && title font visible THEN draw title name
						if (_currentFocus == _node[object] && titleFontAlpha > 0.0)
						{

							// THEN project a vector (sphere centre) from object space into screen space
							D3DXVECTOR3 temp = D3DXVECTOR3( 0.0, 0.0, 0.0);
							D3DXVec3Project ( &temp,&_fontPos,&stage->viewPort,&stage->projMatrix,&stage->viewMatrix,&objectTransform);


							// THEN set material and draw 
							strcpy(text, _listName);
							_font->GetTextExtent(text, &textFS);
							device->SetMaterial ( &_text2DMaterial);
							_font->DrawText(int ( temp.x)-((float)textFS.cx/2), int(temp.y)-((float)textFS.cy/2)-7, _fontTitleColour, text, 0);

						}


					} // ENDIF 3d text


				} // ENDIF draw text


			} // ENDIF within


		} // ENDIF object is in front of the camera THEN draw ELSE cull


	} // end for each object


	// ---------- part 11 : tidy up ----------


	// always zap ray focus
	_rayFocus = false;


	// ok
	return true;
}




// ---------- framework : setup - called by load thread ----------

bool dx::cluster::setup (int id)
{

	// store id
	_id = id;


	// seed set
	srand(_seed);


	// initialise set stack
	for ( int i=0; i<MAX_CLUSTER_OBJECTS; i++)
	{
		_node[i]   = 0;
		_object[i] = 0;
		_sphere[i] = new dx::sphere (0.5);
	}


	// ok
	return true;
}




// ---------- framework : setp dx - called when load thread has exited ----------

bool dx::cluster::setupDX (dx::stage* stage)
{

	
	// ---------- part 1 : fonts ----------


	// assign font objects
	_font    = stage->font[0];
	_fontSub = stage->font[1];
	_font3d  = stage->font[2];


	// font - color
	_fontNavigationColour = D3DXCOLOR( 1.0, 0.8f, 0.0,  1.0);
	_fontNormalColour     = D3DXCOLOR( 1.0, 1.0,  1.0,  1.0);
	_fontTitleColour      = D3DXCOLOR( 1.0, 1.0,  1.0,  1.0);


	// font - position
	_fontPos = D3DXVECTOR3( 0.0, 0.0, 0.0);
	_projPos = D3DXVECTOR3( 0.0, 0.0, 0.0);


	// ---------- part 2 : fonts - materials ----------


	// material for 2d text
	_text2DMaterial = d3d::WHITE_MTRL;
	_text2DMaterial.Ambient  = d3d::BLACK;
	_text2DMaterial.Diffuse  = d3d::WHITE;
	_text2DMaterial.Specular = d3d::WHITE;
	_text2DMaterial.Emissive = d3d::WHITE * 0.5f;
	_text2DMaterial.Power    = 5.0f;


	// material for 3d white text
	_text3DMaterial = d3d::WHITE_MTRL;
	_text3DMaterial.Ambient  = d3d::BLACK;
	_text3DMaterial.Diffuse  = d3d::WHITE;
	_text3DMaterial.Specular = d3d::WHITE;
	_text3DMaterial.Emissive = d3d::WHITE;
	_text3DMaterial.Power    = 5.0f;


	// material for 3d navigation text
	_nav3DMaterial = d3d::ORANGE_MTRL;
	_nav3DMaterial.Ambient  = d3d::BLACK;
	_nav3DMaterial.Diffuse  = d3d::ORANGE;
	_nav3DMaterial.Specular = d3d::WHITE;
	_nav3DMaterial.Emissive = d3d::WHITE;
	_nav3DMaterial.Power    = 5.0f;



	// ---------- part 3 : objects ----------


	// grid
	_grid     = new dx::object_3D_D  (stage->device,"grid");


	// objects
	_up       = new dx::object_3D_N  (stage->device,"up");
	_previous = new dx::object_3D_N  (stage->device,"up");
	_next     = new dx::object_3D_N  (stage->device,"up");
	_pentagon = new dx::object_3D_N  (stage->device,"pentagon");
	_triangle = new dx::object_3D_N  (stage->device,"triangle");
	_disk     = new dx::object_3D_N  (stage->device,"disk");
	_hexagon  = new dx::object_3D_N  (stage->device,"hexagon");
	_box      = new dx::object_3D_N  (stage->device,"box");
	_spoke    = new dx::object_3D_N  (stage->device,"spoke");


	// texture plane
	_tplane   = new dx::objectRectangle_3D_NT (stage->device,"plane");


	// build texture project path and filename 
	char* paf = stage->projectPAF( "textures/", "cube_512.jpg");


	// load texture
	_tplaneTexture = 0;
	D3DXCreateTextureFromFile( stage->device, paf, &_tplaneTexture );


	// ---------- part 4 : create focus cluster ----------


	// object to display
	_totalObjects = 0;


	// update stack with first cluster
	_totalObjects =	updateStack();


	// ok
	return true;
}




// ---------- framework : set z order ----------

bool dx::cluster::zIndex (dx::stage* stage)
{

	// ---------- part 1 : create the cluster transform ----------

	
	// utility transform
	D3DXMATRIX rotateX;
	D3DXMATRIX rotateY;
	D3DXMATRIX rotateZ;
	D3DXMATRIX translateXYZ;


	// pose this set
	_clusterPose.setParameters( _currentPose, _targetPose, float(_pose.mu), _id);


	// transform
	D3DXMatrixTranslation   ( &translateXYZ, _clusterPose.getChannel(3), _clusterPose.getChannel(4), _clusterPose.getChannel(5));
	D3DXMatrixRotationX     ( &rotateX, _clusterPose.getChannel(0)*(float)RADIAN  );
	D3DXMatrixRotationY     ( &rotateY, _clusterPose.getChannel(1)*(float)RADIAN  );
	D3DXMatrixMultiply      ( &_clusterTransform, &translateXYZ, &stage->worldMatrix );
	D3DXMatrixMultiply      ( &_clusterTransform, &rotateX, &_clusterTransform );
	D3DXMatrixMultiply      ( &_clusterTransform, &rotateY, &_clusterTransform );


	// ---------- part 2 : get the minimum z ----------


	// get minimum z
	float minimumZ = stage->boundingBoxZCentroid (&stage->viewMatrix, &_clusterTransform, -5.0, 5.0, -5.0, 5.0, -0.1f, 0.1f);


	// set stage zIndex value
	stage->zIndex = minimumZ;


	// ok
	return true;

}




// ********** END FRAMEWORK METHODS **********




// ---------- cluster : default data ----------

gv::node* dx::cluster::getDefaultData()
{
	gv::node* def = new gv::node("default");
		def->add("seed",  "1");

	// ok
	return (def);
}




// ---------- display : update ----------

//
// NOTE : maximum cluster objects defined by MAXclusterOBJECTS in dxcluster.h
//

DWORD dx::cluster::update (gv::mouse* mouse, gv::time* time)
{

	// ---------- part 1 : update _inspect ----------


	// if double click & a 't'wig then start inspect timer
	if (mouse->getMouseDoubleClick())
	{

		// if key matched tag mouse double clicked to be reset when this frame is completed
		mouse->tagMouseDoubleClick();


		// (re)start inspect
		if (!_inspect.running && _mouseFocus && _mouseFocus->type =='t')
		{

			// if same focus then TIMER_FLIP state
			int state;
			if (_inspect.focus == _mouseFocus)
			{
				state = 0;
			}
			else
			{
				state = 1;
			}
			_inspect.start(_mouseFocus);
			_inspect.state = state;

		}
	}


	// ---------- part 2 : if open timer running then update ----------


	// if open timer is running
	if (_open.running)
	{

		// start - then stack new cluster & prep all for animation
		if (_open.setup)
		{
			_totalObjects   = updateStack();
			_open.setup = false; 

			// ALSO - reset inspect to ensure that no images are visible when mouseFocus
			_inspect.state = 0;

		}


		// update timer
		_open.update (time);


		// interpolate
		_open.TIMER_FLIP  = _open.mu * 180.0;
		_open.TIMER_SCALE = _open.mu;


		// re-position
		for (DWORD i=0; i<_totalObjects; i++)
		{
			_sphere[i]->move( (float)_open.imu, (float)_open.mu);
		}


		// if time > open timer length then end open timer
		if (_open.time >= _open.length)
		{

			// set previous focus == current focus
			_previousFocus  = _currentFocus;


			// if _ghost focus is active
			if (!_ghost.running)
			{

				// end open timer
				_open.end();
				_open.length = 1.5;


				// if ghost mode
				if (_ghostMode)
				{
					_ghost.start();
				}
			}
		}
	}


	// ---------- part 3 : if inspect timer running then update ----------


	// if inspect timer is running
	if (_inspect.running)
	{

		// time
		_inspect.update (time);


		// flip
		_inspect.TIMER_FLIP  = _inspect.state ? _inspect.mu * 180.0 : _inspect.imu * 180.0;


		// if time > inspect timer length then end inspect timer
		if (_inspect.time >= _inspect.length)
		{
			_inspect.end();
			if (!_inspect.state)
			{
				_inspect.focus = 0;
			}
		}
	}


	// ---------- part 3A : if pause timer running then update ----------

	// IF pause timer running THEN
	if (_pause.running)
	{
		// if setup then assign new focus & start an open timer
		if (_pause.setup)
		{
			_pause.setup = false;
		}
		_pause.update(time);
		if (_pause.time >= _pause.length)
		{
			_pause.end();
			_ghostMode = true;
			_pauseMode = false;
		}
	}
	else
	{
		if (_pauseMode)
		{
			_pause.length = (float)_id * 0.5;
			_pause.start();
		}
	}


	// ---------- part 3B : if ghost timer running then update ----------

	// IF ghost timer running THEN
	if (_ghost.running)
	{

		// if setup then assign new focus & start an open timer
		if (_ghost.setup)
		{
			int newFocus  = int( float(rand())/(float(RAND_MAX)+1)*float(_totalObjects-2) + 0.5);
			_previousFocus = _currentFocus;
			if (_node[newFocus]->link)
			{
				_currentFocus = _node[newFocus]->link;
			}
			else
			{
				_currentFocus = _node[newFocus];
			}
			_open.start();
			_ghost.setup = false;
		}


		// if time > ghost timer length then end ghost timer
		//
		// NOTE : the open timer initiated in setup above can only re-start
		//        a ghost timer when this ghost timer has completed - thus
		//        providing a pause between each ghost timer
		//
		_ghost.update(time);
		if (_ghost.time >= _ghost.length)
		{
			_ghost.end();
		}

	}

	
	// if ghost mode then start
	else
	{
		if (_ghostMode)
		{
			_ghost.start();
		}
	}


	// ---------- part 3 : if pose timer running then update ----------


	// if inspect timer is running
	if (_pose.running)
	{
		_pose.update(time);
		if (_pose.time >= _pose.length)
		{
			_pose.end();
			_currentPose = _targetPose;
		}
	}


	// done
	return _totalObjects;

}




// ---------- update : pick ray ----------

//
// NOTE : Original implemenation only updated the pick ray when ray->picking property was
//        set true by left mouse click. This was changed to allow continual picking of the
//        mouse focus.
//

void dx::cluster::updatePickRay (dx::ray* ray, int totalObjects, D3DXMATRIX* worldTransform, float scaling)
{

	// nothing picked
	ray->index = -1;
	ray->pickZ = 9999999.0;


	// pick
	if (ray != 0)
	{
		//if (ray->picking) // removed to allow continual pick update
		//{

			// for each sphere
			for (int i=0; i<totalObjects; i++)
			{

				// transform location
				D3DXVECTOR3 s = D3DXVECTOR3( _sphere[i]->getX(), _sphere[i]->getY(), _sphere[i]->getZ());
				D3DXVec3TransformCoord(&s, &s, worldTransform);


				// ray
				D3DXVECTOR3 v = ray->origin - s;
				float b = 2.0f * D3DXVec3Dot(&ray->direction, &v);
				float r = _sphere[i]->getRadius() * scaling;
				float c = D3DXVec3Dot(&v, &v) - (r*r);


				// find the discriminant
				float discriminant = (b * b) - (4.0f * c);


				// test for imaginary number
				if( discriminant < 0.0f )
				{
					;
				}
				else
				{
					// if a solution is >= 0, then there is a intersection with the sphere
					discriminant = sqrtf(discriminant);
					float s0 = (-b + discriminant) / 2.0f;
					float s1 = (-b - discriminant) / 2.0f;
					if( s0 >= 0.0f || s1 >= 0.0f )
					{
						if (s1 < ray->pickZ)
						{
							ray->index = i;
							ray->pickZ     = s1;
						}
					}
				}
			}
		//}
	}


	// if there is a ray/sphere intersection
	if (ray->index >= 0)
	{

		// assign mouse focus
		_mouseFocus = _node[ray->index];


		// set ray focus
		ray->focusNode = _mouseFocus;


		// if this new focus is not the same as the current focus
		if (ray->picking && _mouseFocus->type != 'r' && _mouseFocus != _currentFocus)
		{

			// re-assign if this node is linked
			if (_mouseFocus->link != 0)
			{
				_mouseFocus = _mouseFocus->link;
			}


			// zap previous focus
			_previousFocus = 0;


			// set current focus
			_currentFocus  = _mouseFocus;


			// start open timer
			_open.start();

		}


	// set mouse focus == null
	} else {
		_mouseFocus    = 0;
	}

}




// ---------- setupDX & update : update the sphere/node stack ----------

DWORD dx::cluster::updateStack ()
{


	// stack builds from current top
	DWORD i;
	DWORD stackTop = 0;


	// ---------- part 1 : find current (new) focus in old cluster then reset stack ----------


	// find - either a visible or referenced link
	float xc=0.0, yc=0.0, zc=0.0;
	int found = -1;
	for (i=0; i<_totalObjects; i++)
	{
		if ( _node[i] == _currentFocus || (_node[i]->link != 0 && _node[i]->link == _currentFocus) )
		{
			found = i;
			continue;
		}

	}
	if (found >= 0)
	{
		xc = _sphere[found]->getX();
		yc = _sphere[found]->getY();
		zc = _sphere[found]->getZ();
	}
	_totalObjects = 0;


	// ---------- part 2 : build new cluster ----------


	// stack parent - if not a root node!
	if (_currentFocus != _clusterData && _currentFocus->parent != 0)
	{
		_object[stackTop] = 'u';
		_node[stackTop++] = _currentFocus->parent;
	}


	// stack previous sibling
	if (_currentFocus->previous != 0)
	{
		if (_currentFocus->previous->visible)
		{
			_object[stackTop] = 'p';
			_node[stackTop++] = _currentFocus->previous;
		}
	}


	// stack children
	gv::node* child = _currentFocus->first;
	while (child != 0)
	{
		if (child->visible)
		{
			_object[stackTop] = 'c';
			_node[stackTop++] = child;
		}
		child = child->next;
	}

	
	// stack next sibling
	if (_currentFocus->next != 0)
	{
		if (_currentFocus->next->visible)
		{
			_object[stackTop] = 'n';
			_node[stackTop++] = _currentFocus->next;
		}
	}


	// ---------- part 3 : open new cluster from original ----------
	double inc = 360.0/double(stackTop);
	double ang = -90.0;
	for (i=0; i<stackTop; i++)
	{
		double rdn = ang * RADIAN;
		float x =  _orbitRadius * float(cos(rdn));
		float y = -_orbitRadius * float(sin(rdn));
		_sphere[i]->setMove ( xc, yc, zc, x,  y, 0.0);
		ang += inc;
	}


	// add focus - opens from self to origin
	_sphere[stackTop]->setMove ( xc, yc, zc, 0.0, 0.0, 0.0);
	_object[stackTop] = 'f';
	_node[stackTop++] = _currentFocus;


	// ok
	return stackTop;
}




// ---------- get display state flags from cluster boss ----------

void dx::cluster::updateState (dx::stage* stage)
{

	// get message
	gv::message* m = stage->messageBox->get(2);


	// read message

	if (!strcmp(m->title,"selectedPose"))
	{
		{
			if (!_pose.running)
			{
				if ( m->data.intValue != _currentPose)
				{
					_targetPose = m->data.intValue;
					_pose.start();
				}
			}
		}
	}

	else if (!strcmp(m->title,"draw3dFont"))
	{
		_draw3dFont = m->data.boolValue;
	}

	else if (!strcmp(m->title,"gridVisible"))
	{
		_gridVisible = m->data.boolValue;
	}

	else if (!strcmp(m->title,"ghostMode"))
	{
		_pauseMode = m->data.boolValue;
		_ghostMode = false;
	}

	else if (!strcmp(m->title,"imageVisible"))
	{
		_imageVisible = m->data.boolValue;
	}

	else if (!strcmp(m->title,"rayFocus"))
	{
		_rayFocus = m->data.boolValue;
	}


}




/////////////////////////////////////////////////////////////////////////////////////////////////




// ********** CLUSTER POSE **********




// ---------- constructor ----------

#define SCA 0.8669f
#define HX1 (  8.65125f * SCA )
#define HZ1 (  5.0f * SCA )
#define HZ2 ( 10.0f * SCA )

dx::clusterPose::clusterPose ()
{

	// channel     -c-    -l-    -r-    -u-    -d-
	float rx[] = {
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0, -90.0,  90.0,
				   0.0,   0.0,   0.0, -90.0,  90.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0
				};
	float ry[] = {
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0, -90.0,  90.0,   0.0,   0.0,
				   0.0, -90.0,  90.0,   0.0,   0.0,
				   0.0,  60.0, -60.0, -60.0,  60.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0
				};
	float rz[] = {
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0
				};

	// channel     -c-    -l-    -r-    -u-    -d-
	float tx[] = {
				   0.0, -10.0,  10.0,   0.0,   0.0,
				   0.0,  -5.0,   5.0,   0.0,   0.0,
				   0.0,   5.0,  -5.0,   0.0,   0.0,
				   0.0,   HX1,  -HX1,   HX1,  -HX1,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0
				};
	float ty[] = {
				   0.0,   0.0,   0.0,  10.0, -10.0,
				   0.0,   0.0,   0.0,   5.0,  -5.0,
				   0.0,   0.0,   0.0,  -5.0,   5.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   0.0,   0.0,   0.0,   0.0,   0.0
				};
	float tz[] = {
				   0.0,   0.0,   0.0,   0.0,   0.0,
				   5.0,   0.0,   0.0,   0.0,   0.0,
				  -5.0,   0.0,   0.0,   0.0,   0.0,
				   HZ2,   HZ1,   HZ1,  -HZ1,  -HZ1,
				   0.0,   5.0,  -5.0,   2.5,  -2.5,
				   0.0,  15.0, -15.0,   7.5,  -7.5
				};


	// array of pointer to an array
	float* data[] = { &rx[0], &ry[0], &rz[0], &tx[0], &ty[0], &tz[0]};


	// array of pointer to a string
	char*  name[] = { "xr",   "yr",   "zr",   "xt",   "yt",   "zt"};


	// number of channels
	_channels = sizeof(data)/sizeof(float);


	// length of a channel	
	int channelLength = sizeof(rx)/sizeof(float);


	// length of group within a channel	
	_length = channelLength / _channels;


	// set parameters
	setParameters (0,0,0.0,0);


	// create channels 
	for (int i=0; i<_channels; i++)
	{
		_t[i] = new gv::channel(name[i], data[i], channelLength);
	}

}




// ---------- destructor ----------

dx::clusterPose::~clusterPose ()
{
	for (int i=0; i<sizeof(_t); i++)
	{
		d3d::Delete<gv::channel*>(_t[i]);
	}
}




//////////////////////////////////////////////////////////////////////////////////////////////////
