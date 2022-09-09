////////////////////////////////////////////////////////////////////////////////

// ---------- vs_dot_obj_elem.cpp ----------
/*!
\file vs_dot_obj_elem.cpp
\brief Implementation of the Obj classes.
\author Gareth Edwards
*/

#include "../header/vs_dot_obj_elem.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vs_system;


class DotObjElement::PI_DotObjElement
{

	public:

	// ---- cdtor
		PI_DotObjElement::PI_DotObjElement() {};
		PI_DotObjElement::~PI_DotObjElement() {};

	// ---- node
		DotObjElement  *_parent   = NULL;
		DotObjElement  *_previous = NULL;
		DotObjElement  *_next     = NULL;
		DotObjElement  *_first    = NULL;
		DotObjElement  *_last     = NULL;

	// ---- identity
		DotObjElement::TagId _tid = DotObjElement::TagId::UNDEFINED;

	// ---- data
		INT   _number_of_values = 0;
		FLOAT _value[4] = { 0, 0, 0, 0 };

};


////////////////////////////////////////////////////////////////////////////////


// ---- cdtor

	DotObjElement::DotObjElement()
	{
		pi_dot_obj_element = new PI_DotObjElement();
	}

	DotObjElement::~DotObjElement()
	{
		if ( pi_dot_obj_element != NULL )
		{
			delete pi_dot_obj_element;
			pi_dot_obj_element = NULL;
		}
	}


// ---- builder

	DotObjElement *DotObjElement::Add(TagId tid)
	{
		DotObjElement *new_node = new DotObjElement();

		if ( pi_dot_obj_element->_first == NULL )
		{
			pi_dot_obj_element->_first = pi_dot_obj_element->_last = new_node;
		}
		else
		{
			pi_dot_obj_element->_last->pi_dot_obj_element->_next = new_node;
			new_node->pi_dot_obj_element->_previous = pi_dot_obj_element->_last;
			pi_dot_obj_element->_last = new_node;
		}
		new_node->pi_dot_obj_element->_tid = tid;

		return new_node;
	}


// ---- node

	DotObjElement *DotObjElement::GetFirst()    { return pi_dot_obj_element->_first;    };
	DotObjElement *DotObjElement::GetLast()     { return pi_dot_obj_element->_last;     };
	DotObjElement *DotObjElement::GetNext()     { return pi_dot_obj_element->_next;     };
	DotObjElement *DotObjElement::GetParent()   { return pi_dot_obj_element->_parent;   };
	DotObjElement *DotObjElement::GetPrevious() { return pi_dot_obj_element->_previous; };
	DotObjElement::TagId DotObjElement::GetTagId() { return pi_dot_obj_element->_tid;   };


// ---- data

	INT DotObjElement::GetNumberOfValues()
	{
		return pi_dot_obj_element->_number_of_values;
	}
	INT DotObjElement::SetNumberOfValues(INT number_of_values)
	{
		pi_dot_obj_element->_number_of_values =
			number_of_values >= 1 && number_of_values <= 4 ?
				number_of_values : 0;
		return
			number_of_values >= 1 && number_of_values <= 4 ?
				VS_OK : VS_ERROR;
	}

	INT DotObjElement::GetIntValue(INT index)
	{
		return (INT)GetFloatValue(index);
	}
	INT DotObjElement::SetIntValue(INT index, INT value)
	{
		INT result = SetFloatValue(index, (FLOAT)value);
		return result;
	}
	FLOAT DotObjElement::GetFloatValue(INT index)
	{
		return pi_dot_obj_element->_value[index > 0 && index < 4 ? index : 0];
	}
	INT DotObjElement::SetFloatValue(INT index, FLOAT value)
	{
		pi_dot_obj_element->_value[index > 0 && index < 4 ? index : 0] = value;
		return index > 0 && index < 4 ? VS_OK : VS_ERROR;
	}
	FLOAT * DotObjElement::GetPtrToFloatValues()
	{
		return &pi_dot_obj_element->_value[0];
	}


////////////////////////////////////////////////////////////////////////////////


class DotObjUtilities::PI_DotObjUtilities
{

	public:


	// ---- data
		struct DotObjList
		{
			CHAR *_tag_name;
			DotObjElement::TagId _tag_id;
		};

		DotObjList _rosetta[9] =
		{
			{ "#",   DotObjElement::TagId::COMMENT },
			{ "v",   DotObjElement::TagId::VERTEX_GEOMETRY  },
			{ "vt",  DotObjElement::TagId::VERTEX_TEXTURE   },
			{ "vn",  DotObjElement::TagId::VERTEX_NORMAL    },
			{ "vp",  DotObjElement::TagId::VERTEX_PARAMETER },
			{ "f",   DotObjElement::TagId::ELEMENT_FACE },
			{ "l",   DotObjElement::TagId::ELEMENT_LINE },
			{ "p",   DotObjElement::TagId::ELEMENT_POINT },
			{ "?",   DotObjElement::TagId::UNDEFINED },
		};

		CHAR *_tag_label[9] =
		{
			{ "#"  },
			{ "vertices" },
			{ "texture coordinates" },
			{ "vertex normals" },
			{ "parameter space vertices" },
			{ "faces" },
			{ "lines" },
			{ "points" },
			{ "?" },
		};


	// ---- cdtor
		PI_DotObjUtilities::PI_DotObjUtilities() {};
		PI_DotObjUtilities::~PI_DotObjUtilities() {};


	// ---- tag
		DotObjElement::TagId GetTagId(
				CHAR *tag_name                // [in]
			);
		DotObjElement *GetTagList(
			DotObjElement *object,            // [in]
			DotObjElement::TagId tag_id       // [in]
		);
		CHAR *GetTagLabel(
				DotObjElement::TagId tag_id   // [in]
			);
		CHAR *GetTagName(
				DotObjElement::TagId tag_id   // [in]
			);


	// --- read
		INT Scrub
			(
				CHAR *data,                   // [in & out]
				INT  *size                    // [in & out]
			);
		INT Split
			(
				CHAR *word[],                 // [out]
				UINT *word_count,             // [out]
				CHAR  split_char,             // [in]
				CHAR *line,                   // [in]
				const INT len                 // [in]

			);

	// ---- debug stuff
		INT v__count = 0;
		INT vt_count = 0;
		INT vn_count = 0;
		INT vp_count = 0;
		INT ef_count = 0;
		INT el_count = 0;
		INT ep_count = 0;

};


// ---------- GetTagId ----------
/*!
\brief get a ".obj" format object tag id
\author Gareth Edwards
\param CHAR *tag_name [in] tag name
\return DotObjElement::TagId
*/
DotObjElement::TagId DotObjUtilities::PI_DotObjUtilities::GetTagId(
		CHAR *tag_name // [in]
	)
{

	// ---- find tag id with given name
		UINT num_type = sizeof(_rosetta) /
			sizeof(PI_DotObjUtilities::DotObjList);
		for (UINT t = 0; t<num_type; t++)
		{
			if ( strcmp(_rosetta[t]._tag_name, tag_name) == 0 ) return _rosetta[t]._tag_id;
		}

	// ---- otherwise UNDEFINED
		return _rosetta[num_type-1]._tag_id;
}


// ---------- GetTagList ----------
/*!
\brief get a object tag list
\author Gareth Edwards
\param CHAR *tag_name [in] tag name
\return DotObjElement *
*/
DotObjElement * DotObjUtilities::PI_DotObjUtilities::GetTagList(
		DotObjElement *object,       // in]
		DotObjElement::TagId  tag_id // [in]
	)
{

	// ---- check
		if ( object == NULL) return NULL;
		DotObjElement *first = object->GetFirst();
		if ( object->GetFirst() == NULL ) return NULL;

	// ---- loop
		DotObjElement *elem = first;
		while (elem)
		{
			DotObjElement::TagId tid = elem->GetTagId();
			if ( tag_id == tid ) return elem;
			elem = elem->GetNext();
		}

	// ---- find tag id list
		return NULL;
}


// ---------- GetTagLabel ----------
/*!
\brief get a ".obj" format object tag label
\author Gareth Edwards
\param DotObjElement::TagId tag_id [in] tag id
\return CHAR * tag label (e.g. "vertices")
\note returns "" if tag id not matched
*/
CHAR *DotObjUtilities::PI_DotObjUtilities::GetTagLabel(
		DotObjElement::TagId tag_id // [in]
	)
{

	// ---- find tag label with given id
		UINT num_type = sizeof(_rosetta) /
			sizeof(PI_DotObjUtilities::DotObjList);
		for (UINT t = 0; t<num_type; t++)
		{
			if ( _rosetta[t]._tag_id == tag_id ) return &_tag_label[t][0];
		}

	// ---- otherwise UNDEFINED
		return &_tag_label[num_type - 1][0];
}


// ---------- GetTagName ----------
/*!
\brief get a ".obj" format object tag name
\author Gareth Edwards
\param DotObjElement::TagId tag_id [in] tag id
\return CHAR * tag name
\note returns DotObjElement::TagId::UNDEFINED "name" if not matched
*/
CHAR *DotObjUtilities::PI_DotObjUtilities::GetTagName(
		DotObjElement::TagId tag_id // [in]
	)
{

	// ---- find tag name with given id
		UINT num_type = sizeof(_rosetta) /
			sizeof(PI_DotObjUtilities::DotObjList);
		for (UINT t = 0; t<num_type; t++)
		{
			if ( _rosetta[t]._tag_id == tag_id ) return &_rosetta[t]._tag_name[0];
		}

	// ---- otherwise UNDEFINED
		return &_rosetta[num_type-1]._tag_name[0];
}


// ---------- Scrub ----------
/*!
\brief scrub (cleanup/tidy) a ".obj" format data array 
\author Gareth Edwards
\param CHAR *data [in & out] array
\param INT  *len  [in & out] of data array
\return VS_ERROR <= INT else VS_OK, etc...
\note

Where * is any normal char...

Scrub out:
 1. multiple ' '
 2. multiple '\n'
 3. bogus '\r'
 4. " /" with '/'


*/
INT DotObjUtilities::PI_DotObjUtilities::Scrub
		(
			CHAR *data,          // [in & out]
			INT  *size           // [in & out]
		)
{

	// ---- local
		INT new_size = *size;


	// ---- scrub
		INT j = -1;
		CHAR last_last_c = 0;
		CHAR last_c = 0;
		for ( INT i = 0; i < new_size; i++ )
		{

			char this_c = data[i];
			BOOL copy = TRUE;
			switch ( this_c ) //data[i] )
			{
				case ' ':
					if ( last_c == ' ' ) copy = FALSE;
					break;
				case '\n':
					if ( last_c == '\n' ) copy = FALSE;
					break;
				case '\r':
					copy = FALSE;
					break;
				case '/':
					if ( last_c == '/' ) data[++j] = '0';
					break;
				default:
					break;
			}

			if ( copy )
				data[++j] = this_c;

			last_last_c = last_c;
			last_c = this_c;

		}
		new_size = j;


	// ---- finally terminate
		data[++j] = '\n';


	// ----- & update len
		*size = new_size;

	return VS_OK;
};


// ---------- Split ----------
/*!
\brief split a ".obj" format CHAR data line into "words"
\author Gareth Edwards
\param CHAR *word       [out] pointer to words within line
\param INT  *word_count [out] number of words
\param CHAR *line       [in]  pointer to line in data array
\param INT  *len        [in]  length of line data array
\return VS_ERROR <= INT else VS_OK, etc...
*/
INT DotObjUtilities::PI_DotObjUtilities::Split
		(
			CHAR *word[],        // [out]
			UINT *word_count,    // [out]
			CHAR  split_char,
			CHAR *line,          // [in]
			const INT len        // [in]
		)
{

	// ---- comment
		if ( line[0] == _rosetta[DotObjElement::TagId::COMMENT-1]._tag_name[0] && line[1] == split_char && len > 2 )
		{
			word[0]     = &line[0];
			word[1]     = &line[2];
			line[1]     = 0;
			line[len]   = 0;
			*word_count = 2;
			return VS_OK;
		}


	// ---- count & terminate
		INT wc = 0;
		CHAR last_c = split_char;
		for ( INT i = 0; i < len; i++ )
		{
			CHAR this_c = line[i];
			if ( this_c != split_char && last_c == split_char )
			{
				word[wc++] = &line[i];
			}
			else if ( this_c == split_char && last_c != split_char )
			{
				line[i] = 0;
			}
			last_c = this_c;
		}
		line[len] = 0;


	// ---- #
		*word_count = wc;

	return VS_OK;
};


////////////////////////////////////////////////////////////////////////////////


// ---------- CTOR ----------
/*!
\brief create a DotObjUtilities object
\author Gareth Edwards
*/
DotObjUtilities::DotObjUtilities()
{
	pi_dot_obj_utilities = new PI_DotObjUtilities();
}


// ---------- DTOR ----------
/*!
\brief destroy a DotObjUtilities object
\author Gareth Edwards
*/
DotObjUtilities::~DotObjUtilities()
{
	if ( pi_dot_obj_utilities != NULL )
	{
		delete pi_dot_obj_utilities;
		pi_dot_obj_utilities = NULL;
	}
}


// ---------- Read ----------
/*!
\brief read a ".obj" format object data file
\author Gareth Edwards
\param DotObjElement *object [in]
\param CHAR *filename [in]
\return VS_ERROR <= INT else VS_OK, etc...  
*/
INT DotObjUtilities::Read(
		DotObjElement *object, CHAR *filename
	)
{

	// ---- error
		if (object == NULL) return VS_ERROR;


	// ---- local
		INT result = VS_OK;


	// ---- create an element tag list tree
		try
		{
			result = CreateTagLists(object);
			if ( result != VS_OK ) throw result;
		}
		catch ( INT result )
		{
			return result;
		}


	// ---- read file
		using namespace std;
		ifstream file(filename, ios::in | ios::binary | ios::ate);
		if (file.is_open())
		{
			file.seekg(0, ios::end);
			streamoff size = file.tellg();
			char *data = new char[(int)size];
			file.seekg(0, ios::beg);
			file.read(data, size);
			file.close();
			try
			{

				INT int_size = (int)size;

				result = pi_dot_obj_utilities->Scrub(data, &int_size);
					if ( result != VS_OK ) throw result;

				result = ParseData(object, data, int_size);
					if ( result != VS_OK ) throw result;

			}
			catch ( INT result )
			{
				delete[] data;
				return result;
			}
			delete[] data;
		}

	return VS_OK;
}


// ---------- Report ----------
/*!
\brief report on an object
\author Gareth Edwards
\param DotObjElement *object [in]
\param CHAR *filename [in] ".obj" path/file name
\return VS_ERROR <= INT else VS_OK, etc...
*/
INT DotObjUtilities::Report(
		DotObjElement *object,
		CHAR *filename
	)
{

	// ---- check
		if ( object == NULL) return NULL;
		DotObjElement *first = object->GetFirst();
		if ( object->GetFirst() == NULL ) return NULL;


	// ---- tag list
			OutputDebugString("Object: Path/File - ");
			OutputDebugString(filename);
			OutputDebugString("\n");
		DotObjElement *tags_list = first;
		while ( tags_list )
		{

			// ---- ?
				DotObjElement *elem_list = tags_list->GetFirst();
				INT number_of_elem = 0;
				while ( elem_list )
				{
					++number_of_elem;
					elem_list = elem_list->GetNext();
				}

			// ---- tell
				DotObjElement::TagId tag_id = tags_list->GetTagId();
				CHAR *tag_label = pi_dot_obj_utilities->GetTagLabel(tag_id);
				CHAR msg[256];
				sprintf_s(msg, 256, "   %10d - %s \n", number_of_elem, tag_label);
				OutputDebugString(msg);

			tags_list = tags_list->GetNext();

		}

	return VS_OK;
}


// ---------- Write ----------
/*!
\brief write an object
\author Gareth Edwards
\param DotObjElement *object [in]
\param CHAR *filename [in] ".obj" path/file name
\return VS_ERROR <= INT else VS_OK, etc...
*/
INT DotObjUtilities::Write(
		DotObjElement *object,
		CHAR *filename
	)
{

	// ---- local
		BOOL report = FALSE;


	// ---- check
		if ( object == NULL) return NULL;
		DotObjElement *first = object->GetFirst();
		if ( object->GetFirst() == NULL ) return NULL;


	// ---- stream
		std::ofstream output_stream;
		output_stream.open(filename);
		if (!output_stream ) { // file couldn't be opened
			OutputDebugString("DotObjUtilities::Write - Error: file could not be opened \n");
			return VS_ERROR;
		}


	// ---- BIG buffer
		INT output_buffer_size = 100000;
		std::string output_buffer;
		output_buffer.reserve(output_buffer_size);


	// ---- lambda: write to BIG buffer
		auto write_output_buffer = [report, output_buffer_size](
				std::ofstream & output_stream,
				std::string & output_buffer,
				std::string buffer
			)
		{
			if ( (INT)output_buffer.length() + (INT)buffer.length() + 1 >= output_buffer_size )
			{
				if ( report )
				{
					OutputDebugString(buffer.c_str());
					OutputDebugString("\n");
				}
				output_stream << output_buffer;
				output_buffer.resize(0);
			}
			output_buffer.append(buffer);
			output_buffer.append(1, '\n');
		};


	// ---- lambda: flush & close BIG buffer
		auto flush_output_buffer = [&](
				std::ofstream & output_stream,
				std::string & output_buffer
			)
		{
			output_stream << output_buffer;
			output_stream.close();
		};


	// ---- tag
		DotObjElement *tags_list = first;
		while ( tags_list )
		{
		
			// ---- tag
				DotObjElement::TagId tag_id = tags_list->GetTagId();
				CHAR *tag_name = pi_dot_obj_utilities->GetTagName(tag_id);
				CHAR *tag_label = pi_dot_obj_utilities->GetTagLabel(tag_id);
				

			// ---- ?
				switch ( tag_id )
				{

					// ---- stuff
						case (INT)DotObjElement::TagId::COMMENT:
							break;

					// ---- vertex
						case (INT)DotObjElement::TagId::VERTEX_GEOMETRY:
						case (INT)DotObjElement::TagId::VERTEX_TEXTURE:
						case (INT)DotObjElement::TagId::VERTEX_NORMAL:
						case (INT)DotObjElement::TagId::VERTEX_PARAMETER:
							{

								// ---- loop
									CHAR c[128];
									std::string buffer;
									buffer.reserve(128);
									DotObjElement *vertex = tags_list->GetFirst();
									INT number_of_vertex = 0;
									while ( vertex )
									{

										// ---- assemble
											buffer.assign(tag_name);
											INT number_of_values = vertex->GetNumberOfValues();
											for ( INT n=0; n<number_of_values; n++)
											{
												FLOAT v = vertex->GetFloatValue(n);
												sprintf(c, " %f", v);
												buffer += c;
											}
											++number_of_vertex;
											vertex = vertex->GetNext();

										// ---- output
											write_output_buffer(output_stream, output_buffer, buffer);
									}

								// ---- annotate
									if (number_of_vertex != 0)
									{

										// --- assemble
											sprintf(c, "# %d %s\n", number_of_vertex, tag_label);
											buffer.assign(c);

										// ---- output
											write_output_buffer(output_stream, output_buffer, buffer);
									}
							}
							break;

					// ---- elements
						case (INT)DotObjElement::TagId::ELEMENT_FACE:
						case (INT)DotObjElement::TagId::ELEMENT_LINE:
						case (INT)DotObjElement::TagId::ELEMENT_POINT:
							{

								// ---- loop
									CHAR c[128];
									std::string buffer;
									buffer.reserve(128);
									DotObjElement *element = tags_list->GetFirst();
									INT number_of_elements = 0;
									while ( element )
									{

										// ---- assemble
											buffer.assign(tag_name);
											DotObjElement *parts = element->GetFirst();
											while ( parts )
											{
												buffer += " ";
												INT number_of_values = parts->GetNumberOfValues();
												for (INT n = 0; n < number_of_values; n++)
												{
													BOOL last_value = n == number_of_values-1 ? TRUE : FALSE;
													INT v = parts->GetIntValue(n);
													//
													// note: v == 0 is legal ( e.g. v/0/vn ) but must NOT be saved
													//
													if ( v == 0 ) c[0] = '\0'; else sprintf(c, "%d", v);
													if (!last_value) strcat(c, "/");
													buffer += c;
												}
												parts = parts->GetNext();
											}
											++number_of_elements;
											element = element->GetNext();

										// ---- output
											write_output_buffer(output_stream, output_buffer, buffer);

									}

								// ---- annotate
									if ( number_of_elements != 0 )
									{

										// ---- assemble
											sprintf(c, "# %d %s\n", number_of_elements, tag_label);
											buffer.assign(c);

										// ---- output
											write_output_buffer(output_stream, output_buffer, buffer);
									}
							}
							break;

					// ---- more stuff
						case (INT)DotObjElement::TagId::UNDEFINED:
						default:
							break;

				}

			tags_list = tags_list->GetNext();

		}


		// ---- output
			flush_output_buffer(output_stream, output_buffer);


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- get size of list

	// ---------- GetSizeOfIndexList ----------
	/*!
	\brief get size of ".obj" index list
	\author Gareth Edwards
	\param DotObjElement *object [in] 
	\param DotObjElement::TagId tag_id [in] list tag id
	\return INT #
	*/
	INT DotObjUtilities::GetSizeOfIndexList(
			DotObjElement *object,
			DotObjElement::TagId tag_id
		)
	{

		// ---- check * object
			if ( object == NULL) return NULL;
			DotObjElement *first = object->GetFirst();
			if ( object->GetFirst() == NULL ) return NULL;

		// ---- check tag
			switch ( tag_id )
			{
				case DotObjElement::TagId::ELEMENT_FACE:
				case DotObjElement::TagId::ELEMENT_LINE:
				case DotObjElement::TagId::ELEMENT_POINT:
					break;
				default:
					return VS_ERROR;
			}

		// ---- count
			DotObjElement *element_list = pi_dot_obj_utilities->GetTagList(object, tag_id);
			DotObjElement *element = element_list->GetFirst();
			INT number_of_element = 0;
			INT number_of_indices = 0;
			while ( element )
			{
				++number_of_element;
				DotObjElement *indice = element->GetFirst();
				while ( indice )
				{
					++number_of_indices;
					indice = indice->GetNext();
				}
				element = element->GetNext();
			}

		return number_of_indices;
	}


	// ---------- GetSizeOfVertexList ----------
	/*!
	\brief get size of ".obj" vertexlist
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\param DotObjElement::TagId tag_id [in] list tag id
	\return INT #
	*/
	INT DotObjUtilities::GetSizeOfVertexList(
			DotObjElement *object,
			DotObjElement::TagId tag_id
		)
	{

		// ---- check * object
			if ( object == NULL) return NULL;
			DotObjElement *first = object->GetFirst();
			if ( object->GetFirst() == NULL ) return NULL;

		// ---- check tag
			switch ( tag_id )
			{
				case DotObjElement::TagId::VERTEX_GEOMETRY:
				case DotObjElement::TagId::VERTEX_TEXTURE:
				case DotObjElement::TagId::VERTEX_NORMAL:
				case DotObjElement::TagId::VERTEX_PARAMETER:
					break;
				default:
					return VS_ERROR;
			}

		// ---- count
			DotObjElement *vertices_list = pi_dot_obj_utilities->GetTagList(object,tag_id);
			DotObjElement *vertex = vertices_list->GetFirst();
			INT number_of_vertices = 0;
			while ( vertex )
			{
				++number_of_vertices;
				vertex = vertex->GetNext();
			}

		return number_of_vertices;
	}


////////////////////////////////////////////////////////////////////////////////


// ---- get list


	// ---------- GetIndexBuffer ----------
	/*!
	\brief get ".obj" object index buffer
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\param DotObjElement::TagId tag_id [in] tag id
	\param INT *indices [in] indices buffer
	\param INT index_count [in] # of indices buffer
	\return VS_ERROR <= INT else VS_OK, etc...
	*/
	INT DotObjUtilities::GetIndexBuffer(
			DotObjElement *object,
			DotObjElement::TagId tag_id,
			DWORD *index_buffer,
			INT index_count
		)
	{

		// ---- check * object
			if ( object == NULL) return NULL;
			DotObjElement *first = object->GetFirst();
			if ( object->GetFirst() == NULL ) return NULL;

		// ---- check tag
			switch ( tag_id )
			{
				case DotObjElement::TagId::ELEMENT_FACE:
				case DotObjElement::TagId::ELEMENT_LINE:
				case DotObjElement::TagId::ELEMENT_POINT:
					break;
				default:
					return VS_ERROR;
			}

		// ---- get
			DotObjElement *faces_list = pi_dot_obj_utilities->GetTagList(object, tag_id);
			DotObjElement *face = faces_list->GetFirst();
			INT number_of_faces = 0;
			INT number_of_indices = 0;
			DWORD *ib = index_buffer;
			while ( face )
			{
				++number_of_faces;
				DotObjElement *part = face->GetFirst();
				while ( part )
				{
					if ( number_of_indices >= index_count ) return VS_ERROR;
					++number_of_indices;

					*(ib) = part->GetIntValue(0) - 1;
					ib++;

					part = part->GetNext();
				}
				face = face->GetNext();
			}

		return VS_OK;
	}

	
	// ---------- GetVertexBuffer ----------
	/*!
	\brief get ".obj" object vertex buffer
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\param DotObjElement::TagId tag_id [in] tag id
	\param FLOAT *indices [in] vertices buffer
	\param INT vertex_count [in] # of vertices
	\return VS_ERROR <= INT else VS_OK, etc...
	
	\note
	
	In this order:

	# float   value(s)              vs & dx pre-processor
	-------   -------------------   ---------------------
	  3       vertex geometry     - VS_XYZ     (   2 )
	  1       diffuse             - VS_DIFFUSE (  64 )
	  3       vertex normal       - VS_NORMAL  (  16 )
	  2       texture coordinates - VS_TEX1    ( 256 )

	\note diffuse defaults to white

	\note vertex normal and texture coordinates are
	initialised to zero
	
	*/
	INT DotObjUtilities::GetVertexBuffer(
			DotObjElement *object,
			DotObjElement::TagId tag_id,
			FLOAT *vertex_buffer,
			INT vertex_count,
			INT vertex_format
		)
	{

		// ---- check
			if ( object == NULL) return NULL;
			DotObjElement *first = object->GetFirst();
			if ( object->GetFirst() == NULL ) return NULL;


		// ---- default diffuse colour
			float r = 1, g = 1, b = 1, a = 1;
			union colour_union_t {
				DWORD d;
				FLOAT f;
			} diffuse;
				diffuse.d = (
							( ((DWORD)(a*255) & 0xff) << 24) |
							( ((DWORD)(r*255) & 0xff) << 16) |
							( ((DWORD)(g*255) & 0xff) << 8) |
							( ((DWORD)(b*255) & 0xff) )
						);

		// ---- get
			DotObjElement *vertices_list = pi_dot_obj_utilities->GetTagList(object, tag_id);
			DotObjElement *vertex = vertices_list->GetFirst();
			INT number_of_vertices = 0;
			FLOAT *vb = vertex_buffer;
			while ( vertex )
			{

				if ( number_of_vertices >= vertex_count ) return VS_ERROR;
				++number_of_vertices;

				// ---- geometry
					FLOAT *g = vb;
					*(vb + 0) = vertex->GetFloatValue(0);
					*(vb + 1) = vertex->GetFloatValue(1);
					*(vb + 2) = vertex->GetFloatValue(2);
					vb += 3;


				// --- diffuse
					if ( vertex_format & VS_DIFFUSE )
					{
						*(vb++) = diffuse.f;
					}


				// ---- vertex normals
					if ( vertex_format & VS_NORMAL )
					{
						*(vb    ) = 0;
						*(vb + 1) = 0;
						*(vb + 2) = 0;
						vb += 3;
					}


				// ---- texture coordinates
					if ( vertex_format & VS_TEX1 )
					{
						*(vb + 0) = 0;
						*(vb + 1) = 0;
						vb += 2;
					}


				vertex = vertex->GetNext();
			}

		return VS_OK;
	}


////////////////////////////////////////////////////////////////////////////////


// ---- triangles


	// ---------- GetNumberOfTriangles ----------
	/*!
	\brief get number of ".obj" triangles
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\return INT #
	*/
	INT DotObjUtilities::GetNumberOfTriangles(DotObjElement *object)
	{

		//---- note:
			//
			// though this could be counted when a dotobj is read
			// a dotobj could also be procedurally generated...
			//

		// ---- check
			if ( object == NULL) return NULL;
			DotObjElement *first = object->GetFirst();
			if ( object->GetFirst() == NULL ) return NULL;


		// ---- count
			DotObjElement *faces_list =
				pi_dot_obj_utilities->GetTagList(
					object,DotObjElement::TagId::ELEMENT_FACE);
			DotObjElement *face = faces_list->GetFirst();
			INT number_of_faces = 0;
			INT number_of_triangles = 0;
			while ( face )
			{
				++number_of_faces;
				DotObjElement *indice = face->GetFirst();
				INT number_of_indices = 0;
				while ( indice )
				{
					++number_of_indices;
					indice = indice->GetNext();
				}
				number_of_triangles += number_of_indices > 3 ?
					number_of_indices - 2 : 1;
				face = face->GetNext();
			}

		return number_of_triangles;
	}


	// ---------- GetTriangleIndexBuffer ----------
	/*!
	\brief get ".obj" object triangle index buffer
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\param DotObjElement::TagId tag_id [in] tag id
	\param INT *indices [in] indices buffer
	\param INT index_count [in] # of indices buffer
	\return VS_ERROR <= INT else VS_OK, etc...
	*/
	INT DotObjUtilities::GetTriangleIndexBuffer(
			DotObjElement *object,
			DWORD *index_buffer,
			INT index_total
		)
	{

		// ---- a lot can go wrong!
			try
			{

				// ---- check object
					if ( object == NULL )                  throw("object: NULL");
					DotObjElement *first = object->GetFirst();
					if ( object->GetFirst() == NULL )      throw("object: has no element lists");


				// ---- check buffer
					if ( index_buffer == NULL )           throw("index buffer: NULL");


				// ---- lists
					DotObjElement *faces_list =
						pi_dot_obj_utilities->GetTagList(
							object, DotObjElement::TagId::ELEMENT_FACE);
					if ( faces_list->GetFirst() == NULL ) throw("face element list: empty");


				// ---- local
					DWORD *ib = index_buffer;


				// ---- for each face
					DotObjElement *face = faces_list->GetFirst();
					INT index_count = 0;
					while ( face )
					{

						// ---- buffer ok?
							if ( index_count >= index_total ) throw("index buffer: overflow");


						// ---- for each indice
							DotObjElement *indice = face->GetFirst();
							while ( indice )
							{
								*(ib) = index_count;
								ib++;
								index_count++;
								indice = indice->GetNext();
							}

						face = face->GetNext();
					}

			}
			catch (CHAR * msg)
			{
				OutputDebugString(msg);
				OutputDebugString("\n");
			}

			//INT result = Write(object, "tmp.obj");

		return VS_OK;
	}


	// ---------- GetTriangleVertexBuffer ----------
	/*!
	\brief get ".obj" object triangle vertex buffer
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\param DotObjElement::TagId tag_id [in] tag id
	\param FLOAT *indices [in & out] vertices buffer
	\param INT vertex_count [in] # of vertices
	\return VS_ERROR <= INT else VS_OK, etc...
	\note see vertex info in GetVertexBuffer method
	*/
	INT DotObjUtilities::GetTriangleVertexBuffer(
			DotObjElement *object,
			FLOAT *object_param,
			FLOAT *vertex_buffer,
			INT vertex_total,
			INT vertex_format
		)
	{

		// ---- a lot can go wrong!
			try
			{

				// ---- check object
					if ( object == NULL )                  throw("object: NULL");
					DotObjElement *first = object->GetFirst();
					if ( object->GetFirst() == NULL )      throw("object: has no element lists");


				// ---- check buffer
					if ( vertex_buffer == NULL )           throw("vertex buffer: NULL");


				// ---- check only for required buffer formats
					if ( ! ( vertex_format & VS_XYZ)    )  throw("vertex format: NO VS_XYZ");
					if ( ! ( vertex_format & VS_NORMAL) )  throw("vertex format: VS_NORMAL");


				// ---- lists
					DotObjElement *faces_list =
						pi_dot_obj_utilities->GetTagList(
							object, DotObjElement::TagId::ELEMENT_FACE);
					DotObjElement *geometry_list =
						pi_dot_obj_utilities->GetTagList(
							object, DotObjElement::TagId::VERTEX_GEOMETRY);
					DotObjElement *normal_list =
						pi_dot_obj_utilities->GetTagList(
							object, DotObjElement::TagId::VERTEX_NORMAL);
					DotObjElement *texture_list =
						pi_dot_obj_utilities->GetTagList(
							object, DotObjElement::TagId::VERTEX_TEXTURE);
				

				// ---- check only for required lists
					if ( faces_list->GetFirst()    == NULL ) throw("face element list: empty");
					if ( geometry_list->GetFirst() == NULL ) throw("geometry element list: empty");


				// ---- flag only for required lists
					BOOL normal_exists   = normal_list->GetFirst()   == NULL ? FALSE : TRUE;
					BOOL texture_exists  = texture_list->GetFirst()  == NULL ? FALSE : TRUE;


				// --- param
					FLOAT  scale  = object_param[0];
					FLOAT  width  = object_param[1] * scale;
					FLOAT  height = object_param[2] * scale;
					FLOAT  depth  = object_param[3] * scale;
					FLOAT  x_off  = object_param[4];
					FLOAT  y_off  = object_param[5];
					FLOAT  z_off  = object_param[6];


				// ---- local
					FLOAT norm_x = 0, norm_y = 0, norm_z = 0;
					FLOAT *vb = vertex_buffer;
					BOOL  calculate_face_normal = normal_exists == TRUE ? FALSE : TRUE;
					INT   geometry_index = 0, normal_index = 0, texture_index = 0;
					VsVector3 n (0, 0, 0);


				// ---- local geometry lut
					INT geometry_total = 0;
					DotObjElement *geometry_indices[100000];
					DotObjElement *vertex = geometry_list->GetFirst();
					while ( vertex )
					{
						geometry_indices[geometry_total++] = vertex;
						vertex = vertex->GetNext();
					}


				// ---- local normal lut
					INT normal_total = 0;
					DotObjElement *normal_indices[100000];
					if ( normal_exists )
					{
						DotObjElement *normal = normal_list->GetFirst();
						while ( normal )
						{
							normal_indices[normal_total++] = normal;
							normal = normal->GetNext();
						}
					}


				// ---- NEED TO CHECK index values throughout !!! ---- 


				// ---- for each face
					DotObjElement *face = faces_list->GetFirst();
					INT vertex_count = 0;
					INT normal_count = 0;
					while ( face )
					{

						// ---- buffer ok?
							if ( vertex_count >= vertex_total ) throw("vertex buffer: overflow");

					
						// ---- face normal ?
							if ( calculate_face_normal )
							{

								// ---- first three vertices
									INT indices_for_normal[3];
									INT indices_count = 0;
									DotObjElement *indice = face->GetFirst();
									while ( indice && indices_count < 3)
									{
										indices_for_normal[indices_count++] = indice->GetIntValue(0) - 1;
										indice = indice->GetNext();
									}

								// ---- calculate normal
									if ( indices_count == 3 )
									{

										FLOAT *pa = geometry_indices[indices_for_normal[0]]->GetPtrToFloatValues();
										FLOAT *pb = geometry_indices[indices_for_normal[1]]->GetPtrToFloatValues();
										FLOAT *pc = geometry_indices[indices_for_normal[2]]->GetPtrToFloatValues();
										
										VsVector3 b( *(pb+0) - *(pa+0), *(pb+1) - *(pa+1), *(pb+2) - *(pa+2));
										VsVector3 c( *(pc+0) - *(pa+0), *(pc+1) - *(pa+1), *(pc+2) - *(pa+2));

										n.x = b.y * c.z - b.z * c.y;
										n.y = b.z * c.x - b.x * c.z;
										n.z = b.x * c.y - b.y * c.x;

										FLOAT q = (FLOAT)sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
										n.x /= q;
										n.y /= q;
										n.z /= q;

										//n.CrossProduct(&b, &c);
										//n.Normalise();
										//float fred = 2;
									}
									else
									{
										throw("face element: less than 3 indices");
									}


								// ---- add a vertex_normal to tag list
									DotObjElement *normal = normal_list->Add(DotObjElement::TagId::VERTEX_NORMAL);
									normal->SetFloatValue(0, n.x);
									normal->SetFloatValue(1, n.y);
									normal->SetFloatValue(2, n.z);
									normal->SetNumberOfValues(3);

								// --- set index (must be this for dotobj)
									normal_index = normal_count + 1;

								// --- store in lut
									normal_indices[normal_index - 1] = normal;

								// --- increment
									normal_total++;
									normal_count++;

							}


						// ---- for each indice
							DotObjElement *indice = face->GetFirst();
							while ( indice )
							{
					
								// ---- geometry
									INT geometry_index = indice->GetIntValue(0);
									DotObjElement *vertex = geometry_indices[geometry_index - 1];
									*(vb + 0) = vertex->GetFloatValue(0)  * width  + x_off;
									*(vb + 1) = vertex->GetFloatValue(1)  * height + y_off;
									*(vb + 2) = vertex->GetFloatValue(2)  * depth  + z_off;

								// ---- normal
									if ( normal_exists )
									{
										INT normal_index = indice->GetIntValue(2);
										DotObjElement *normal = normal_indices[normal_index - 1];
										*(vb + 3) = normal->GetFloatValue(0);
										*(vb + 4) = normal->GetFloatValue(1);
										*(vb + 5) = normal->GetFloatValue(2);
									}
									else
									{
										indice->SetIntValue(2, normal_index);
										indice->SetNumberOfValues(2);
										*(vb + 3) = n.x;
										*(vb + 4) = n.y;
										*(vb + 5) = n.z;
									}

								// ---- texture
									if ( vertex_format & VS_TEX1 )
									{
										if ( texture_exists )
										{
											INT texture_index = indice->GetIntValue(1);
											DotObjElement *texture = &texture_list[texture_index - 1];
											*(vb + 6) = texture->GetFloatValue(0);
											*(vb + 7) = texture->GetFloatValue(1);
										}
										else
										{
											*(vb + 6) = 0;
											*(vb + 7) = 0;
										}
										vb += 8;
									}
									else
									{
										vb += 6;
									}

								// ---- housekeeping
									++vertex_count;
									indice = indice->GetNext();

							}

						face = face->GetNext();
					}
			}
			catch (CHAR * msg)
			{
				OutputDebugString(msg);
				OutputDebugString("\n");
			}

		return VS_OK;
	}


	// triangle indices...
	//indices[indices_count] = indice->GetIntValue(0) - 1; // vertex
	//if ( number_of_indices >= 2 )
	//{
	//	// 0 1 2, 0 2 3, 0 3 4, etc..
	//}


////////////////////////////////////////////////////////////////////////////////


	// ---------- CreateTagLists ----------
	/*!
	\brief create an element tag list tree
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\return VS_ERROR <= INT else VS_OK, etc...
	*/
	INT DotObjUtilities::CreateTagLists(
			DotObjElement *object
		)
	{

		// ---- build tree
			UINT num_type = sizeof(pi_dot_obj_utilities->_rosetta) /
				sizeof(PI_DotObjUtilities::DotObjList);
			for ( UINT t=0; t<num_type; t++)
				object->Add(pi_dot_obj_utilities->_rosetta[t]._tag_id);

		return VS_OK;
	}


	// ---------- ParseData ----------
	/*!
	\brief parse a ".obj" format object CHAR data array
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\param CHAR *data [in] data array
	\param INT size [in] of data array
	\return VS_ERROR <= INT else VS_OK, etc...
	*/
	INT DotObjUtilities::ParseData(
			DotObjElement *object,
			CHAR    *data,
			INT      size
		)
	{

		// ---- data ?
			INT index = 0;
			int line_count = 0;
			int line_start = 0;
			int line_end = line_start;
			while ( index < size )
			{
				CHAR *c = data + index;
				switch (*c)
				{
					case '\n':
						++line_count;
					case '\r':
						{
							int len = line_end - line_start + 1;
							if ( len > 0 )
							{
								CHAR test[256];
								memcpy(test, data + line_start, len);
								test[len] = 0;

								int result = ParseLine(object, data + line_start, len);
								line_start += len + 1;
								line_end = line_start;
							}
						}
						break;
					default:
						line_end = index;
						break;
				}
				index++;
			}


		// ---- debug


		return VS_OK;
	}


	// ---------- ParseLine ----------
	/*!
	\brief parse a ".obj" format object CHAR data line 
	\author Gareth Edwards
	\param DotObjElement *object [in]
	\param CHAR *data [in] pointer to line within data array
	\param INT len [in] lenght of line within the data array
	\return VS_ERROR <= INT else VS_OK, etc...
	*/
	INT DotObjUtilities::ParseLine(
			DotObjElement *object,
			CHAR    *data,
			INT      len
		)
	{

		// ---- local
			INT result = 0;


		// ---- report
			if ( 0 )
			{
				CHAR msg[1024];
				memcpy(msg, data, len+1);
				msg[len + 1] = '\n';
				msg[len + 2] = '\0';
				OutputDebugString(msg);
			}


		// ---- add to tree
			try
			{

				// ---- split
					CHAR *word[64];
					UINT  word_count = 0;
					result = pi_dot_obj_utilities->Split
						(
							word,
							&word_count,
							' ',
							data,
							len
						);
					if ( result != VS_OK ) throw result;


				// ---- tag id
					DotObjElement::TagId tag_id = pi_dot_obj_utilities->GetTagId(word[0]);
					// ( tag_id == DotObjElement::TagId::UNDEFINED ) throw (word[0]);


				// ---- debug 
					switch ( tag_id )
					{
						case (INT)DotObjElement::TagId::VERTEX_GEOMETRY:
							pi_dot_obj_utilities->v__count++;
							break;
						case (INT)DotObjElement::TagId::VERTEX_TEXTURE:
							pi_dot_obj_utilities->vt_count++;
							break;
						case (INT)DotObjElement::TagId::VERTEX_NORMAL:
							pi_dot_obj_utilities->vn_count++;
							break;
						case (INT)DotObjElement::TagId::VERTEX_PARAMETER:
							pi_dot_obj_utilities->vp_count++;
							break;
						case (INT)DotObjElement::TagId::ELEMENT_FACE:
							pi_dot_obj_utilities->ef_count++;
							break;
						case (INT)DotObjElement::TagId::ELEMENT_LINE:
							pi_dot_obj_utilities->el_count++;
							break;
						case (INT)DotObjElement::TagId::ELEMENT_POINT:
							pi_dot_obj_utilities->ep_count++;
							break;
						default:
							break;
					}


				// ---- add
					switch ( tag_id )
					{

						// ---- stuff
							case (INT)DotObjElement::TagId::COMMENT:
								{
									DotObjElement *elem = pi_dot_obj_utilities->GetTagList(object, tag_id);
									elem->Add(tag_id);
								}
								break;

						// ---- vertex
							case (INT)DotObjElement::TagId::VERTEX_GEOMETRY:
							case (INT)DotObjElement::TagId::VERTEX_TEXTURE:
							case (INT)DotObjElement::TagId::VERTEX_NORMAL:
							case (INT)DotObjElement::TagId::VERTEX_PARAMETER:
								{
									DotObjElement *tag_list = pi_dot_obj_utilities->GetTagList(object, tag_id);
									DotObjElement *new_elem = tag_list->Add(tag_id);
									new_elem->SetNumberOfValues(word_count - 1);
									for ( UINT w = 1; w < word_count; w++ )
									{
										new_elem->SetFloatValue( w - 1, (FLOAT)atof(word[w]) );
									}
								}
								break;

						// ---- elements
							case (INT)DotObjElement::TagId::ELEMENT_FACE:
							case (INT)DotObjElement::TagId::ELEMENT_LINE:
							case (INT)DotObjElement::TagId::ELEMENT_POINT:
								{

									// ---- add elem
										DotObjElement *tag_list = pi_dot_obj_utilities->GetTagList(object, tag_id);
										DotObjElement *new_elem = tag_list->Add(tag_id);
										for (UINT w = 1; w < word_count; w++)
										{

											// ---- expand "//" to "/0/"
												INT  word_len = strlen(word[w]);
												CHAR expanded_word[256];
												CHAR last_c = '/';
												INT expanded_word_len = -1;
												for (INT i = 0; i < word_len; i++)
												{
													CHAR this_c = word[w][i];
													if ( last_c == '/' && this_c == '/')
													{
														expanded_word[++expanded_word_len] = '0';
													}
													expanded_word[++expanded_word_len] = this_c;
													last_c = this_c;
												}
												expanded_word[++expanded_word_len] = '\0';


											// ---- & split again...
												CHAR *elem_word[64];
												UINT  elem_word_count = 0;
												result = pi_dot_obj_utilities->Split
													(
														elem_word,
														&elem_word_count,
														'/',
														expanded_word,
														expanded_word_len
													);


											// ---- add elem part
												DotObjElement *new_elem_part = new_elem->Add(tag_id);
												new_elem_part->SetNumberOfValues(elem_word_count);
												for (UINT ew = 0; ew < elem_word_count; ew++)
												{
													new_elem_part->SetIntValue(ew, (INT)atoi(elem_word[ew]));
												}

										}
								}
								break;

						// ---- more stuff
							case (INT)DotObjElement::TagId::UNDEFINED:
							default:
								break;

					}

			}
			catch (INT result)
			{
				return result;
			}
			catch (CHAR *name)
			{
				OutputDebugString("ParseLine: Error - Unrecognised tag name ");
				OutputDebugString((LPSTR)name);
				OutputDebugString("\n");
				return VS_ERROR;
			}

		return VS_OK;
	}


////////////////////////////////////////////////////////////////////////////////

