////////////////////////////////////////////////////////////////////////////////

// ---------- vs_image_roi.hpp ----------
/*!
\file vs_image_roi.hpp
\brief
\author Gareth Edwards
*/


// ---------- ImageRoi class ----------
class ObjConfig::ImageRoi
{

	struct Roi
	{
		FLOAT u, v, z, a;
	};

	Roi    _roi[MAX_PANO_PTF_LUT];
	Roi    _roi_def[MAX_PANO_PTF_LUT];
	FLOAT  _roi_margin; 
	UINT   _selected_id;

public:

	// ---- constructor /destructor ----
	ImageRoi();
	virtual ~ImageRoi();


	// ---- methods ----
	INT Initialise();
	INT Reset();
	INT Read(CHAR *);
	INT Write();
	INT Configure(vs_system::VNode *);
	INT GetProperty(UINT, UINT, UINT, FLOAT *);
	INT SetProperty(UINT, UINT, UINT, FLOAT);

	 
	// ---- config file ----
	CHAR   config_paf_hdl[VS_MAXCHARPATHLEN];
	VNode* config_hdl;


};

 
////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ObjConfig::ImageRoi::ImageRoi()
{
	INT result = Initialise();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ObjConfig::ImageRoi::~ImageRoi()
{
	;
}


// ---------- Initialise ----------
/*!
\brief Initialise parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ImageRoi::Initialise()
{
	Reset();
	return VS_OK;
}


// ---------- Reset ----------
/*!
\brief Reset parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ImageRoi::Reset()
{

	_roi_margin  = 0;
	_selected_id = 0;

	Roi tmp_roi = { 0.5f, 0.5f, 1.0f, 1.3333f };
	for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
	{
		_roi[i] = tmp_roi;
	}

	return VS_OK;
}


// ---------- Read ----------
/*!
\brief Read parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ImageRoi::Read(CHAR *paf)
{

	// Replace '\' with '/'
	UINT len_paf = strlen(paf);
	for (UINT i = 0; i<len_paf; i++) paf[i] = paf[i] == '\\' ? '/' : paf[i];

	// IF file exists THEN
	if (GetFileAttributes(paf) != INVALID_FILE_ATTRIBUTES)
	{
		// Read
		vs_system::VNodeIO vio;
		vs_system::VNode *root = vio.Read(paf);


		// IF read HDL ok THEN
		if ( root != NULL )
		{

			INT result = VS_OK;
			if ( config_hdl != NULL ) result = config_hdl->Delete();
			result = Reset();
			result = Configure(root);
			strcpy(config_paf_hdl, paf);

			// note: config_hdl set == root in Configure method

			// ---- report ----
			{
				printf("Read Image config file from:\n");
				printf("   %s\n", config_paf_hdl);
			}

			return VS_OK;
		}

		// ELSE read HDL ok FAILED
		else
		{

			// ---- report ----
			{
				printf("VS_VNODE_READFILEFAILED...\n\n");
				printf("FAILED to read image config file from:\n");
				printf("   %s\n", config_paf_hdl);
			}

			return VS_VNODE_READFILEFAILED;
		}

	}

	// ELSE file does NOT exist
	else
	{

		// ---- report----
		{
			printf("VS_VNODE_OPENFILEFAILED...\n\n");
			printf("FAILED to read image config file from:\n");
			printf("   %s\n", config_paf_hdl);
		}

		return VS_VNODE_OPENFILEFAILED;
	}


	// ---- report ----
	{
		printf("File does not exist...\n\n");
		printf("FAILED to read image config file from:\n");
		printf("   %s\n", config_paf_hdl);
	}

	return VS_ERROR;
}


// ---------- Write ----------
/*!
\brief Write parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ImageRoi::Write()
{

	// IF -- file exists -- THEN -- overwrite
	if ( GetFileAttributes(config_paf_hdl) != INVALID_FILE_ATTRIBUTES )
	{

		// IF -- data -- THEN
		if ( config_hdl != NULL )
		{


			// IF -- (backwards compatble) PTZ OR PTV data -- THEN
				vs_system::VNode *c = NULL;
				if ( c = config_hdl->SearchName("UVZ") )
				{

					vs_system::VNode *p = NULL;
					CHAR name[VS_MAXCHARNAME];
					CHAR value[VS_MAXCHARNAME];

					if ( p = config_hdl->SearchName("Selected") )
					{
						sprintf(value, "%d", _selected_id);
						p->SetValue(value);
					}

					for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
					{
						sprintf(name, "%d", i + 1); 
						if ( p = c->SearchName(name) )
						{
							vs_system::VNode *q = NULL;
							if ( q = p->SearchName("U") ) { sprintf(value, "%f", _roi[i].u); q->SetValue(value); }				
							if ( q = p->SearchName("V") ) { sprintf(value, "%f", _roi[i].v); q->SetValue(value); }
							if ( q = p->SearchName("Z") ) { sprintf(value, "%f", _roi[i].z); q->SetValue(value); }
							if ( q = p->SearchName("A") ) { sprintf(value, "%f", _roi[i].a); q->SetValue(value); }
						}
					}
				}
		}

		// ---- write image config file ----
			vs_system::VNodeIO vio;
			INT result = vio.Write(config_paf_hdl, config_hdl);


		// ---- report ----
			if (result == VS_OK)
			{
				printf("Saved image config file to:\n");
				printf("   %s\n", config_paf_hdl);
			}
			else
			{
				printf("FAILED to save image config file to:\n");
				printf("   %s\n", config_paf_hdl);
			}


		// IF -- write -- NOT -- successful -- THEN -- return error
			if ( result != VS_OK ) return VS_ERROR;

	}


	// ---- default ----
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			_roi_def[i] = _roi[i];
		}


	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure parameters
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::ImageRoi::Configure(vs_system::VNode *hdl)
{

	config_hdl = hdl;
	if ( config_hdl != NULL )
	{

		vs_system::VNode *p = NULL;
		if (p = config_hdl->SearchName("Selected")) _selected_id = p->GetIntFromValue();

		if ( config_hdl->SearchName("UVZ") )
		{
			CHAR name[VS_MAXCHARNAME];
			for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
			{
				sprintf(name, "%d", i + 1);
				if (p = config_hdl->SearchName(name))
				{
					vs_system::VNode *q = NULL;
					if ( q = p->SearchName("U") ) _roi[i].u = q->GetFloatFromValue();
					if ( q = p->SearchName("V") ) _roi[i].v = q->GetFloatFromValue();
					if ( q = p->SearchName("Z") ) _roi[i].z = q->GetFloatFromValue();
					if ( q = p->SearchName("A") ) _roi[i].a = q->GetFloatFromValue();
				}
			}
		}
	}

	return VS_OK;
}


#define IMAGE_ROI    10
#define IMAGE_ROI_U  11
#define IMAGE_ROI_V  12
#define IMAGE_ROI_Z  13
#define IMAGE_ROI_A  14


// ---------- GetProperty ----------
/*!
\brief Get property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ImageRoi::GetProperty(
		UINT group,
		UINT mode,
		UINT id,
		FLOAT *value
	)
{

	switch ( group )
	{

		case IMAGE_ROI:
		{
			if ( mode >= 0 && mode < MAX_PANO_PTF_LUT )
			{
				switch ( id )
				{
					case IMAGE_ROI_U : *value = _roi[mode].u; return VS_OK;
					case IMAGE_ROI_V : *value = _roi[mode].v; return VS_OK;
					case IMAGE_ROI_Z : *value = _roi[mode].z; return VS_OK;
					case IMAGE_ROI_A : *value = _roi[mode].a; return VS_OK;
					default          :
						;
				}
			}
		}
		break;

		default:
			break;
	}

	return VS_ERROR;
}


// ---------- SetProperty ----------
/*!
\brief Set property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ImageRoi::SetProperty(
		UINT  group,
		UINT  mode,
		UINT  id,
		FLOAT value
	)
{

	switch ( group )
	{
		case IMAGE_ROI:
		{
			if ( mode >= 0 && mode < MAX_PANO_PTF_LUT )
			{
				switch ( id )
				{
					case IMAGE_ROI_U : _roi[mode].u = value; return VS_OK;
					case IMAGE_ROI_V : _roi[mode].v = value; return VS_OK;
					case IMAGE_ROI_Z : _roi[mode].z = value; return VS_OK;
					case IMAGE_ROI_A : _roi[mode].a = value; return VS_OK;
					default          :
						;
				}
			}
		}
		break;

		default:
			break;

	}

	return VS_ERROR;
}
