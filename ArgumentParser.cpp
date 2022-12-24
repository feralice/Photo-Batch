#include "ArgumentParser.h"
#include "Utils.h"


void ArgumentParser::RegisterFlag(const std::string& flag) {

	if (!flag.empty() && !Utils::HasWhitespaces(flag)) //se a flag não for vazia 
	{
		m_Flags[flag] = false; //quando registramos uma flag ela é automaticamente falsa, 
		//ela só passa a ser verdadeira se chamamos no parse e ela estiver no argv do parse
	}

}

void ArgumentParser::RegisterOption(const std::string& option) {

	if (!option.empty() && !Utils::HasWhitespaces(option)) {
		m_Options[option] = "";
	}

}

bool ArgumentParser::isFlagRegistered(const std::string& flag) const {

	if (!flag.empty()) {
		return m_Flags.count(flag) == 1;
	}

	return false;

}

bool ArgumentParser:: isOptionRegistered(const std::string& flag) const {

	if (!flag.empty()) {
		return m_Options.count(flag) == 1;
	}

	return false;

}


bool ArgumentParser::GetFlag(const std::string& flag) const {

	if (!flag.empty()) {
		auto flagIt = m_Flags.find(flag);
		if (flagIt != std::end(m_Flags)) {
			return flagIt->second;
		}
	}

	return false;
}


const std::string& ArgumentParser::GetOption(const std::string& option) const {

	if (!option.empty()) {
		auto optionIt = m_Options.find(option);
		if (optionIt != std::end(m_Options)) {
			return optionIt->second;
		}
	}

	static std::string EmptyOption = "";
	return EmptyOption;

}

float ArgumentParser::GetOptionAsFloat(const std::string& option) const {

	const std::string& optionValue = GetOption(option);


	if (!optionValue.empty()) {
		return std::stof(optionValue);
	}

	return -1;
}

int ArgumentParser::GetOptionAsInt(const std::string& option) const {

	const std::string& optionValue = GetOption(option);


	if (!optionValue.empty()) {
		return std::stoi(optionValue);
	}

	return -1;

}

void ArgumentParser::SetHelpMessage(const std::string& helpMessage)
{
	m_HelpMessage = helpMessage;
}

const std::string& ArgumentParser::GetHelpMessage() const
{
	return m_HelpMessage;
}

void ArgumentParser::Parse(int argc,  char* argv[]) {

	if (argc > 1 && argv!=nullptr) {
		for (int i = 1; i < argc; i++) {

			std::string arg = Utils::ToLower(argv[i]);

			//ver se tem o prefixo --nome
			//Para isso, o argumento preecisa ter 3 caracteres
			//Os "--" e pelo menos um terceiro caracter
			if (arg.length() >= 3 && arg[0] == '-' && arg[1] == '-') {

				//iremos tirar os dois tracinhos do comeco, para ficar mais "elegante"

				//ex: ANTES: --rename
				arg = arg.substr(2); // retira os dois tracinhos iniciais
				//DEPOIS: rename

				if (arg.find_first_of('=') != std::string::npos) {

					//é uma opção

					const size_t equalSignPos = arg.find('='); //acha em que posição está o =

					if (equalSignPos != std::string::npos) {

						//dividindo a opção em chave e valor
						std::string optionName = arg.substr(0, equalSignPos); //pega apenas o nome
						std::string optionValue = arg.substr(equalSignPos + 1); //pega o sinal de igual até o final

						auto optionIt = m_Options.find(optionName);
						if (optionIt != std::end(m_Options)) {
							//achamos uma opção registrada
							optionIt->second = optionValue;
						}

					}
						

				}
				else {

					//é uma flag

					auto flagIt = m_Flags.find(arg);

					if (flagIt != std::end(m_Flags)) {

						//achamos uma flag registrada
						flagIt->second = true;

					}

				}

			}

		}
	}

}