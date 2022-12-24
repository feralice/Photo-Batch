#include "Mode.h"
#include "ArgumentParser.h"
#include "RenameMode.h"
#include "ConvertMode.h"
#include "ResizeMode.h"
#include "ScaleMode.h"

#include <iostream>
#include <filesystem>
#include <array>
#include <chrono>

Mode::Mode(const std::string& filter, const std::string& folder) 
	: m_Filter{filter}
	, m_Folder{folder}
{

	//m_Filter = filter;
	//m_Folder = folder;

}

const std::string& Mode::GetFilter() const {

	return m_Filter;

}

const std::string& Mode::GetFolder() const {

	return m_Folder;

}

void Mode::Run() {

	using ClockT = std::chrono::high_resolution_clock;

	ClockT::time_point startTime = ClockT::now();

	RunImpl();

	ClockT::time_point endTime = ClockT::now();

	ClockT::duration elapsedTime = endTime - startTime;

	std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds> (elapsedTime);

	std::cout <<"\n" << GetModeName() << "Tempo de execução em milissegundos: " << elapsedTimeMs.count() << " ms " << std::endl;

}



const std::string& GetInvalidChars() {

	static const std::string invalidCharacters = "\\/*?\"<>|:";
	return invalidCharacters;

}

bool hasInvalidChars(const std::string& str) {

	const bool bHasInvalidChars = str.find_first_of(GetInvalidChars()) != std::string::npos;
	
	return bHasInvalidChars;
}


std::vector<std::filesystem::path>Mode::GetFiles(const std::filesystem::path& extension) const {

	std::vector<std::filesystem::path> files;
	int numSkipFiles = 0;

	//coletar todos os arquivos que correspodem ao filtro especificado
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(GetFolder())) {

		const bool bIsFile = std::filesystem::is_regular_file(entry.path());
		const bool bMatchFilter = GetFilter().empty() || (entry.path().string().find(GetFilter()) != std::string::npos);
		const bool bMatchExtension = extension.empty() || (entry.path().extension() == extension);

		if (bIsFile && bMatchFilter && bMatchExtension) {
			files.push_back(entry.path());
		}
		else {
			numSkipFiles++;
		}

	}

	std::cout << GetModeName() << "Número de arquivos encontrados: " << files.size() << std::endl;
	std::cout << GetModeName() << "Número de arquivos ignorados: " << numSkipFiles << std::endl;

	return files;

}


std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser) {




		//ler as flags que o argumentparser identificou
		const bool bRenameMode = argParser.GetFlag(Args::Flags::Rename);
		const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
		const bool bResizeMode = argParser.GetFlag(Args::Flags::Resize);
		const bool bScaleMode = argParser.GetFlag(Args::Flags::Scale);

		//verificar se somente um modo está ativo

		// ^ é um ou exclusivo, que só é verdade quando temos apenas um elemento que é verdade 

		//entra no if se mais de um modo estiver ativo
		if (!(bRenameMode ^ bConvertMode ^ bResizeMode ^ bScaleMode)) {

			//se houver mais de um modo ativo, lança uma exceção
			throw std::invalid_argument("Somente um modo pode estar ativo");

		}

		//Validar a pasta passada como opção folder
		const std::string folder = argParser.GetOptionAs<std::string>(Args::Opts::Folder);

		if (folder.empty()) {

			throw std::invalid_argument("A pasta não pode estar em branco");
		}


		if (!std::filesystem::exists(folder)) {

			throw std::invalid_argument("A pasta não não existe");
		}

		//validar se o filtro é uma string válida
		const std::string filter = argParser.GetOptionAs<std::string>(Args::Opts::Filter);

		if (!filter.empty() && hasInvalidChars(filter)) {

			throw std::invalid_argument("O filtro não pode conter" + GetInvalidChars());

		}

		//validar o modo resize
		if (bResizeMode) {

			int width = 0;
			int height = 0;

			try {

				width = argParser.GetOptionAs<int>(Args::Opts::Width);
				height = argParser.GetOptionAs<int>(Args::Opts::Height);

			}
			catch (const std::invalid_argument& exception) {

				throw std::invalid_argument("O valor informado de height ou width não são números");

			}



			//no modo resize, o comprimento e a altura devem ser maiores do que 0

			if (width <= 0 || height <= 0) {
				throw std::invalid_argument("Width e height devem ser positivos e maiores que 0");
			}

			if (filter.empty()) {
				throw std::invalid_argument("Filtro não pode estar em branco no modo Resize");

			}

			return std::make_unique<ResizeMode>(filter, folder, width, height);
		}

		//Validar o modo scale
		if (bScaleMode) {

			float amount = 0;

			try {

				amount = argParser.GetOptionAs<float>(Args::Opts::Amount);
			}
			catch (const std::invalid_argument) {

				throw std::invalid_argument("Amount não é um número válido");
			}


			if (amount <= 0.0f) {
				throw std::invalid_argument("Amount deve ser maior do que zero");
			}

			if (filter.empty()) {
				throw std::invalid_argument("Filtro não pode estar em branco no modo Scale");

			}

			return std::make_unique<ScaleMode>(filter, folder, amount);

		}

		//validando o modo rename
		if (bRenameMode) {

			int startNumber = 0;

			try {
				startNumber = argParser.GetOptionAs<int>(Args::Opts::StartNumber);
			}
			catch (const std::invalid_argument&) {

				throw std::invalid_argument("O valor informado para start number não é um número válido");
			}


			std::string prefix = argParser.GetOptionAs<std::string>(Args::Opts::Prefix);

			if (startNumber < 0) {
				throw std::invalid_argument("StartNumber deve ser maior ou igual a zero");
			}

			if (prefix.empty() || hasInvalidChars(prefix)) {

				throw std::invalid_argument("O prefixo não pode estar vazio e nem conter esses caracteres: " + GetInvalidChars());
			}

			return std::make_unique<RenameMode>(filter, folder, prefix, startNumber);

		}

		//Validar o modo convert
		if (bConvertMode) {

			const std::string from = argParser.GetOptionAs<std::string>(Args::Opts::From);
			const std::string to = argParser.GetOptionAs<std::string>(Args::Opts::To);

			const std::array<std::string, 2> convertOptions = { "jpg","png" };

			const bool bFromValido = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions);
			const bool bToValido = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions);


			if (!bFromValido || !bToValido) {

				throw std::invalid_argument("From e To devem ser JPG ou PNG");
			}

			if (from == to) {

				throw std::invalid_argument("From e To devem ser diferentes");
			}

			const std::map<std::string, ConvertMode::Format> convertOptionsMap = {
				{"jpg",ConvertMode::Format::JPG},
				{"png",ConvertMode::Format::PNG}
			};

			return std::make_unique<ConvertMode>(filter, folder, convertOptionsMap.at(from), convertOptionsMap.at(to));

		}

	
		return nullptr;
}