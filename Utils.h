#pragma once
#include <string>

namespace Utils {

	//transforma tudo para letras min�sculas 
	std::string ToLower(std::string str);

	//verifica se tem espa�oes em branco
	bool HasWhitespaces(const std::string& str);
}
