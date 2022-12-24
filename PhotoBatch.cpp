#include <iostream>
#include <iomanip>


#include "Mode.h"
#include "ArgumentParser.h"

#include <stb_image.h>


int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "pt_BR");
	setlocale(LC_NUMERIC, "en_US");

	ArgumentParser argParser;

	//registra as flags
	argParser.RegisterFlag(Args::Flags::Rename);
	argParser.RegisterFlag(Args::Flags::Convert);
	argParser.RegisterFlag(Args::Flags::Resize);
	argParser.RegisterFlag(Args::Flags::Scale);


	//registra as opções
	argParser.RegisterOption(Args::Opts::Filter);
	argParser.RegisterOption(Args::Opts::Folder);
	argParser.RegisterOption(Args::Opts::Height);
	argParser.RegisterOption(Args::Opts::Width );
	argParser.RegisterOption(Args::Opts::Amount);
	argParser.RegisterOption(Args::Opts::Prefix);
	argParser.RegisterOption(Args::Opts::StartNumber);
	argParser.RegisterOption(Args::Opts::From);
	argParser.RegisterOption(Args::Opts::To);



	argParser.Parse(argc, argv);

	//std::cout << std::boolalpha << "Rename: " << argParser.GetFlag(Args::Flags::Rename) << std::endl;
	//std::cout << std::boolalpha << "Convert: " << argParser.GetFlag(Args::Flags::Convert) << std::endl;
	//std::cout << std::boolalpha << "Resize: " << argParser.GetFlag(Args::Flags::Resize) << std::endl;
	//std::cout << std::boolalpha << "Scale: " << argParser.GetFlag(Args::Flags::Scale) << std::endl;

	//std::cout << "Folder :" << argParser.GetOptionAs<std::string>("folder") << std::endl;
	//std::cout << "Amount (float) :" << argParser.GetOptionAs<float>("amount") << std::endl;
	//std::cout << "Amount (int) :" << argParser.GetOptionAs<int>("amount") << std::endl;

	argParser.SetHelpMessage(R"([USO]: PhotoBatch --[Rename|Scale|Convert|Resize] [Opções]
    O PhotoBatch possui 4 modos de operação. Somente um modo pode estar ativo.
    Para ativar um modo escolha uma das seguintes flags:
    --Rename  : Ativa o modo para renomear arquivos
    --Convert : Ativa o modo para converter arquivos
    --Resize  : Ativa o modo para redimensionar arquivos
    --Scale   : Ativa o modo para escalar arquivos
    As seguintes opções são comuns a todos os modos:
    --Folder  : Especifica a pasta onde o PhotoBatch irá atuar
    --Filter  : Especifica o filtro usado para selecionar arquivos
Opções do modo Rename:
    No modo Rename, cada arquivo selecionado será renomeado para o formato
    <Prefix>_<N>.<ext>, onde <Prefix> e <N> são especificados através de opções.
    <ext> é a extensão do arquivo que permanece inalterada.
    --Prefix=<Prefix> : Prefixo usado ao renomear os arquivos
    --StartNumber=<N> : Número inicial a ser adicionado em cada arquivo que for renomeado
Opções do modo Convert:
    No modo Convert, cada arquivo selecionado será convertido do formato <from> para
    o formato <to>. <from> e <to> não podem ser iguais. O nome do arquivo permanece
    inalterado.
    --From=<jpg|png|tga> : Extensão de origem para conversão
    --To=<jpg|png|tga>   : Extensão de destino para conversão
Opções do modo Resize:
    No modo Resize, cada arquivo selecionado será redimensionado para as dimensões
    especificadas nas seguintes opções. Note que N >= 1.
    --Width=<N>  : Novo comprimento da imagem
    --Height=<N> : Nova altura da imagem
Opções do modo Scale:
    No modo Scale, cada arquivo selecionado será escalado uniformemente pelo
    fator especificado pelas seguintes opções. Note que N > 0 e será interpretado
    como um número em ponto flutuante.
    --Amount=<N> : Fator de escala a ser aplicado em cada imagem selecionada.
Exemplos de uso:
    Renomear todos os arquivos da pasta "C:\Viagem" com a extensão ".png" para Viagem_N.png, onde N começa do 0
        PhotoBatch --Rename --Folder="C:\Viagem" --Filter=".png" --Prefix="Viagem_" --StartNumber=0
    Redimensionar todos os arquivos na pasta "C:\Viagem" com a extensão ".jpg" para o tamanho 500x500
        PhotoBatch --Resize --Folder="C:\Viagem" --Filter=".jpg" --Width=500 --Height=500
    Converter todos os arquivos da pasta "C:\Viagem" de "png" para "jpg"
        PhotoBatch --Convert --Folder="C:\Viagem" --From=png --To=jpg
    Reduzir o pela metade o tamanho de todos os arquivos "png" da pasta "C:\Viagem"
        PhotoBatch --Scale --Folder="C:\Viagem" --Filter=".png" --Amount=0.5)");


	// Verifica se a flag Help foi ativada.
	if (argParser.GetFlag(Args::Flags::Help))
	{
		std::cout << argParser.GetHelpMessage() << std::endl;
	}
	else
	{
		try
		{
			if (std::unique_ptr<Mode> PhotoBatchMode = CreateMode(argParser)) {

				PhotoBatchMode->Run();
			}
			

		}
		catch (const std::exception& exception)
		{
			std::cerr <<  exception.what() << std::endl;
			std::cerr << "Para mais informações utilize a flag --help" << std::endl;
		}
	}


	return 0;
}