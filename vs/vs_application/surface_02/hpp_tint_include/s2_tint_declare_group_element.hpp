// ---- properties
	//
	// note: only element and group size/array names include "item"
	//       as I deemed this sufficient usage of this term...
	//

// ---- element display list information
INT  element_dspl_count = 0;
vs_system::TintElemDsplInfo element_dspl_info[vs_system::Tint::MAX_BODY_ELM];


// ---- element display list table
UINT hash_element_lid_to_dspl_index[vs_system::Tint::MAX_BODY_ELM];


// ---- element hash tables
CHAR hash_element_lid_to_group_lid[vs_system::Tint::MAX_BODY_ITEM];
CHAR hash_element_lid_to_index[vs_system::Tint::MAX_BODY_ITEM];


// ---- element item logical id's
UINT num_element_item_lid = 0;
CHAR element_item_lid[vs_system::Tint::MAX_BODY_ITEM];


// ---- group item logical id's
UINT num_group_item_lid = 0;
CHAR group_item_lid[vs_system::Tint::MAX_BODY_ITEM];