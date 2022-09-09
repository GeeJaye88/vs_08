////////////////////////////////////////////////////////////////////////////////

// ---------- vs_parameter_methods.h ----------
/*!
\file vs_parameter_methods.h
\brief Parameter Class methods
\author Gareth Edwards
*/


// ---- predefined indices
	static INT GetParamIndexLid();
	static INT GetParamIndexValue();
	static INT GetParamIndexDefault();
	static INT GetParamIndexIncrement();

// ---- group
	static INT GetParamGroup();
	static INT SetParamGroup(INT);

// ---- group properties
	static INT GetParamGroupSize();
	static INT SetParamGroupSize(INT);
	static INT GetParamGroupIndex();
	static INT SetParamGroupIndex(INT);
	static INT GetParamGroupOffset();
	static INT SetParamGroupOffset(INT);

// ---- array properties
	static INT GetParamArrayIndex();
	static INT SetParamArrayIndex(INT);

// ---- labels and values
	static INT GetParamArraySize();
	static INT GetParamGroupName(std::string &);
	static INT GetParamArray(INT, INT, FLOAT *);
	static INT GetParamArray(INT, INT,INT,  FLOAT *);
	static INT GetParamArrayLabels(INT, std::string[]);
	static INT GetParamArrayLabels(INT, CHAR[]);
	static INT GetParamArrayValues(INT, INT *, INT *, FLOAT *);
	static INT GetParamArrayValues(INT, INT *, INT *, INT, FLOAT *);
	static INT SetParamArray(INT, INT, FLOAT *);
	static INT SetParamArray(INT, INT, INT, FLOAT *);
	static INT SetParamArrayLabels(INT, INT, std::string[]);
	static INT SetParamArrayValues(INT, INT, INT, FLOAT *);
	static INT SetParamArrayDefaults(INT);
