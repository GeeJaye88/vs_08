////////////////////////////////////////////////////////////////////////////////

// ---------- model_panel_methods_incl.h ----------
/*!
\file model_panel_methods_incl.h
\brief include for the Model panel methods
\author Gareth Edwards
*/


// ---- panel ( s1_model_get_panel.hpp ) 
	INT GetPanelDisplayListHandle(vs_system::PanelContext *);


// ---- panel  ( s[...]_model_panels_set_layout.hpp )

	//  invoked by SetPanelsLayoutMode
	INT SetPanelsLayoutDisplayStatus(vs_system::AppDependency *, vs_system::AppView *);
	INT SetPanelsLayoutRectBounds(vs_system::AppLayout *, VsFloatRectangle[2]);

	// invoked by NONE
	INT SetPanelsLayoutKpadStatus(BOOL);

	// invoked by Model::SetMsgLayout
	INT SwapPanelsLayoutAlignedSide();

	// invoked by Kontrol::CallbackByPickElement ( [app space name]\source\[app prefix]_kontrol.cpp ) 
	INT SetPanelsLayoutByElemId(UINT);

	// invoked by Kontrol::CallbackByPickPanel ( shared/hpp/kontrol_class.hpp ) 
	INT SetPanelsLayoutByCorner();  

	// invoked by Model::SetMsgLayout
	//            Model::SetMsgLayout[KpadStatus/AlignedSide/Mode]
	INT SetPanelsLayoutUpdate();


////////////////////////////////////////////////////////////////////////////////
