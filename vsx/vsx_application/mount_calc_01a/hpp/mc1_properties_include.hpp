////////////////////////////////////////////////////////////////////////////////

// ---------- mc1_properties_include.hpp ----------
/*!
\file mc1_properties_include.hpp
\brief properties: data and value
\author Gareth Edwards
\note Vanilla C++
*/



// ---- root node
	vs_system::VNode *root = dot_ini.GetRoot();


// ---- section & property nodes

	vs_system::VNode *vn_border             = root->SearchName("Border");
	vs_system::VNode *vn_border_depth       = vn_border->SearchName("Depth");
	
	vs_system::VNode *vn_colour             = root->SearchName("Colour");
	vs_system::VNode *vn_colour_value       = vn_colour->SearchName("Value");

	vs_system::VNode *vn_outline            = root->SearchName("Outline");
	vs_system::VNode *vn_outline_count      = vn_outline->SearchName("Count");
	vs_system::VNode *vn_outline_depth      = vn_outline->SearchName("Depth");
	vs_system::VNode *vn_outline_gap        = vn_outline->SearchName("Gap");

	vs_system::VNode *vn_paper              = root->SearchName("Paper");
	vs_system::VNode *vn_paper_size         = vn_paper->SearchName("Size");
	vs_system::VNode *vn_paper_measure      = vn_paper->SearchName("Measure");
	vs_system::VNode *vn_paper_report       = vn_paper->SearchName("Report");

	vs_system::VNode *vn_printer            = root->SearchName("Printer");
	vs_system::VNode *vn_printer_correction = vn_printer->SearchName("Correction");
	vs_system::VNode *vn_printer_horizontal = vn_printer->SearchName("Horizontal");
	vs_system::VNode *vn_printer_vertical   = vn_printer->SearchName("Vertical");



// ---- property values

	FLOAT border_depth       = (FLOAT)atof(vn_border_depth->GetValue());

	FLOAT colour_value       = (FLOAT)atof(vn_colour_value->GetValue());

	INT   outline_count      = vn_outline_count->GetIntFromValue();
	FLOAT outline_depth      = (FLOAT)atof(vn_outline_depth->GetValue());
	FLOAT outline_gap        = (FLOAT)atof(vn_outline_gap->GetValue());

	INT   paper_size         = vn_paper_size->GetIntFromValue();
	CHAR *paper_measure      = vn_paper_measure->GetValue();
	CHAR *paper_report       = vn_paper_report->GetValue();

	FLOAT printer_correction = (FLOAT)atof(vn_printer_correction->GetValue());
	FLOAT printer_horizontal = (FLOAT)atof(vn_printer_horizontal->GetValue());
	FLOAT printer_vertical   = (FLOAT)atof(vn_printer_vertical->GetValue());


////////////////////////////////////////////////////////////////////////////////











