////////////////////////////////////////////////////////////////////////////////

// ---------- model_template.hpp ----------
/*!
\file model_templats.hpp
\brief Templates
\author Gareth Edwards
*/

#pragma once


#include <string>
#include <sstream>
#include <iostream>

//! Create a custom convert function, remove the tailing zeros if necessary.  
template<typename T>
std::string tostring(const T &n)
{
	std::ostringstream oss;
	oss << n;
	std::string s = oss.str();
	int dotpos = s.find_first_of('.');
	if (dotpos != std::string::npos) {
		int ipos = s.size() - 1;
		while (s[ipos] == '0' && ipos>dotpos) {
			--ipos;
		}
		s.erase(ipos + 1, std::string::npos);
	}
	return s;
}



