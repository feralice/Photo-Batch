#include "RenameMode.h"
#include <iostream>
#include <filesystem>
#include <vector>

RenameMode::RenameMode(const std::string& filter, const std::string& folder, const std::string& prefix, int startNumber)
	: Mode{ filter,folder }
	, m_Prefix{ prefix }
	, m_StartNumber{ startNumber }

{

}

const std::string& RenameMode::GetModeName() const {

	static const std::string RenameModeName = "[Renomear]: ";
	return RenameModeName;

}

void RenameMode::RunImpl() {

	std::cout << GetModeName() << "Modo           : Renomear" << std::endl;;
	std::cout << GetModeName() << "Filtro         : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Diretório      : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Prefixo        : " << m_Prefix << std::endl;
	std::cout << GetModeName() << "Número Inicial : " << m_StartNumber << std::endl;

	//Iniciar o processo de renomear os arquivos

	//AbsoluteZero.jpg -> prefixoNOME.jpg
	int currentNumber = m_StartNumber;


	for (const std::filesystem::path& filepath : GetFiles()) {

		const std::filesystem::path extension = filepath.extension();
		const std::string newFileName = m_Prefix + std::to_string(currentNumber) + extension.string();

		std::filesystem::path newFilePath = filepath;
		newFilePath.replace_filename(newFileName);

		try {
			std::filesystem::rename(filepath, newFilePath);

			std::cout << GetModeName() << filepath << " -> " << newFilePath << std::endl;
		}
		catch (const std::exception& ex) {

			std::cout << GetModeName() << "Erro ao renomear \"" << filepath << "\":" << ex.what() << std::endl;
		}

		currentNumber++;

	}


}