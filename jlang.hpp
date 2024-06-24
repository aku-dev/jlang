#pragma once

#include <string>

namespace jlang {
	
	using std::string;
	
	struct SArgument
	{
		string s_name;
		string s_notice;
		bool b_on;
	};

	string strtolower(const string& str) 
	{ 
		string result = ""; 
	  
		for (char ch : str) { 
			result += tolower(ch); 
		}
	  
		return result; 
	}
}