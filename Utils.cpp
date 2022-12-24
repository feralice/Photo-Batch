#include "Utils.h"

#include <algorithm>


namespace Utils {

	//transforma tudo para letras minúsculas 
	std::string ToLower(std::string str) {

		std::transform(std::begin(str), std::end(str), std::begin(str),
			[](unsigned char c) { return std::tolower(c); });

		return str;
	}

	bool HasWhitespaces(const std::string& str)
	{
		return str.find(' ') != std::string::npos;
	}

}