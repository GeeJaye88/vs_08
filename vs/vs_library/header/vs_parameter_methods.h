////////////////////////////////////////////////////////////////////////////////

// ---------- vs_parameter_methods.h ----------
/*!
\file vs_parameter_methods.h
\brief Parameter Class methods
\author Gareth Edwards
*/


// ---- predefined indices
	INT GetParamIndexLid();
	INT GetParamIndexValue();
	INT GetParamIndexDefault();
	INT GetParamIndexIncrement();

// ---- group
	INT GetParamGroup();
	INT SetParamGroup(INT);

// ---- group properties
	INT GetParamGroupSize();
	INT SetParamGroupSize(INT);
	INT GetParamGroupIndex();
	INT SetParamGroupIndex(INT);
	INT GetParamGroupOffset();
	INT SetParamGroupOffset(INT);

// ---- array properties
	INT GetParamArrayIndex();
	INT SetParamArrayIndex(INT);

// ---- labels and values
	INT GetParamArraySize();
	INT GetParamGroupName(std::string &);
	INT SetParamArrayLabels(INT, INT, std::string[]);
	INT GetParamArrayLabels(INT, std::string[]);
	INT GetParamArrayLabels(INT, CHAR[]);
	INT GetParamArray(INT, INT, FLOAT *);
	INT GetParamArray(INT, INT, INT, FLOAT *);
	INT GetParamArrayValues(INT, INT *, INT *, FLOAT *);
	INT GetParamArrayValues(INT, INT *, INT *, INT, FLOAT *);
	INT SetParamArray(INT, INT, FLOAT *);
	INT SetParamArray(INT, INT, INT, FLOAT *);
	INT SetParamArrayValues(INT, INT, INT, FLOAT *);
	INT SetParamArrayDefaults(INT);

// ---- fast stuff
	INT Get(INT, INT, INT, INT, FLOAT *);
	INT Put(INT, INT, INT, INT, FLOAT *);

// ---- store
	INT CopyParamSet(vs_system::Parameter *);

