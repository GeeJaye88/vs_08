// ---- init lid arrays

// zero ALL then copy element element item lid char
num_element_item_lid = sizeof(element_char);
memset((VOID *)element_item_lid, 0, sizeof(element_item_lid));
memcpy((VOID *)element_item_lid, (VOID *)element_char, num_element_item_lid);

// zero ALL then copy group group lid char
num_group_item_lid = sizeof(group_char);
memset((VOID *)group_item_lid, 0, sizeof(group_item_lid));
memcpy((VOID *)group_item_lid, (VOID *)group_char, num_group_item_lid);


// ---- init element lid hash tables
for (UINT i = 0; i < num_element_item_lid; i++)
{
	hash_element_lid_to_group_lid[(INT)element_item_lid[i]] = group_item_lid[i];
	hash_element_lid_to_index[(INT)element_item_lid[i]] = i;
}