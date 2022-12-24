#pragma once
#include <string>

namespace Utils {

	//transforma tudo para letras minúsculas 
	std::string ToLower(std::string str);

	//verifica se tem espaçoes em branco
	bool HasWhitespaces(const std::string& str);
}
