////////////////////////////////////////////////////////////////////////////////

// ---------- param_cleanup.hpp ----------
/*!
\file param_cleanup.hpp
\brief Cleanup getting mesh param
\author Gareth Edwards
*/

// ---- tidy
	delete[] param_array_increments;
	delete[] param_array_values;
	delete[] param_array_labels;
