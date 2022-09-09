////////////////////////////////////////////////////////////////////////////////


// ---------- GfxPageDimension class ----------

class GfxPageDimension
{

public:

	GfxPageDimension()
	{
		INT result = Set(100,100);
	};
	GfxPageDimension(UINT w, UINT h)
	{
		INT result = Set(w,h);
	};

	INT Set(UINT w, UINT h)
	{
		width  = w;
		height = h;
		aspect = (FLOAT)height / (FLOAT)width;
		if ( aspect < 0.000001 ) return VS_ERROR;
		return VS_OK;
	}

	UINT  width;
	UINT  height;
	FLOAT aspect;
};


////////////////////////////////////////////////////////////////////////////////

