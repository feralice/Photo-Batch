#pragma once
#include <string>
#include <map>

class ArgumentParser {

public:

	//registra uma flag 
	void RegisterFlag(const std::string& flag);

	//registra uma op��o
	void RegisterOption(const std::string& option);

	//retorna se a flag est� ou n�o registrada
	bool isFlagRegistered(const std::string& flag) const;

	//retorna se a op~��o est� ou n�o registrada
	bool isOptionRegistered(const std::string& flag) const;

	//obtem o valor de uma flag
	bool GetFlag(const std::string& flag) const;

	template<typename T>
	T GetOptionAs(const std::string& option) const;

	template<>
	std::string GetOptionAs(const std::string& option) const { return GetOption(option); }

	template<>
	float GetOptionAs(const std::string& option) const { return GetOptionAsFloat(option); }

	template<>
	int GetOptionAs(const std::string& option) const { return GetOptionAsInt(option); }

	//faz o parse da lista de argumentos
	void Parse(int argc, char* argv[]);

	void SetHelpMessage(const std::string& helpMessage);

	const std::string& GetHelpMessage() const;


private:

	//obter o valor de uma op��o lida durante o parse
	const std::string& GetOption(const std::string& option) const;

	//Obter o valor de uma op��o convertida em float
	float GetOptionAsFloat(const std::string& option) const;

	//converte o valor da op��o em int
	int GetOptionAsInt(const std::string& option) const;

	std::map<std::string, bool> m_Flags;
	std::map<std::string, std::string> m_Options;
	std::string m_HelpMessage;

};