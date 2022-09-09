////////////////////////////////////////////////////////////////////////////////

// ---------- vs_dot_obj_elem.h ----------
/*!
\file vs_dot_obj_elem.h
\brief Interface for the DotObjElement class.
\author Gareth Edwards
*/

#pragma once


// ---- vs system
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_inc_dems.h"
#include "../../vs_system/header/vs_inc_maths.h"

#include <fstream>
#include <string>
#include <vector>


// ---------- namespace ----------

namespace vs_system
{

	class DotObjElement
	{

		public:

			enum TagId
			{
				COMMENT          = 1,
				VERTEX_GEOMETRY  = 2,
				VERTEX_TEXTURE   = 3,
				VERTEX_NORMAL    = 4,
				VERTEX_PARAMETER = 5,
				ELEMENT_FACE     = 6,
				ELEMENT_LINE     = 7,
				ELEMENT_POINT    = 8,
				UNDEFINED        = 9
			};


		// ---- cdtor
			DotObjElement::DotObjElement();
			DotObjElement::~DotObjElement();

		// ---- builder
			DotObjElement *Add(TagId tid);

		// ---- node
			DotObjElement *GetFirst();
			DotObjElement *GetLast();
			DotObjElement *GetNext();
			DotObjElement *GetParent();
			DotObjElement *GetPrevious();
			TagId          GetTagId();

		// ---- data 
			INT            GetNumberOfValues();
			INT            SetNumberOfValues(INT);
			INT            GetIntValue(INT index);
			INT            SetIntValue(INT index, INT value);
			FLOAT          GetFloatValue(INT index);
			INT            SetFloatValue(INT index, FLOAT value);
			FLOAT*         GetPtrToFloatValues();

	private:

		// ---- private implementation
			class  PI_DotObjElement; PI_DotObjElement *pi_dot_obj_element;

	};


	class DotObjUtilities
	{

		public:

			enum GfxRender
			{
				RAW        = 1,
				PROCESSED  = 2,
			};


		// ---- cdtor
			DotObjUtilities::DotObjUtilities();
			DotObjUtilities::~DotObjUtilities();

		// ---- methods
			INT Read(DotObjElement *, CHAR *);
			INT Report(DotObjElement *, CHAR *);
			INT Write(DotObjElement *, CHAR *);

		// ---- get size of list
			INT GetSizeOfIndexList(DotObjElement *, DotObjElement::TagId);
			INT GetSizeOfVertexList(DotObjElement *, DotObjElement::TagId);

		// ---- list
			INT GetIndexBuffer(DotObjElement *, DotObjElement::TagId, DWORD *, INT);
			INT GetVertexBuffer(DotObjElement *, DotObjElement::TagId, FLOAT *, INT, INT);

		// ---- triangles
			INT GetNumberOfTriangles(DotObjElement *);
			INT GetTriangleIndexBuffer(DotObjElement *, DWORD *, INT);
			INT GetTriangleVertexBuffer(DotObjElement *, FLOAT*, FLOAT *, INT, INT);

		// ---- methods to be made private
			INT CreateTagLists(DotObjElement *);
			INT ParseData(DotObjElement *, CHAR *, INT);
			INT ParseLine(DotObjElement *, CHAR *, INT);
		

		private:

		// ---- private implementation
			class  PI_DotObjUtilities; PI_DotObjUtilities *pi_dot_obj_utilities;

	};

};
