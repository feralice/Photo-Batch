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


	//registra as op��es
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

	argParser.SetHelpMessage(R"([USO]: PhotoBatch --[Rename|Scale|Convert|Resize] [Op��es]
    O PhotoBatch possui 4 modos de opera��o. Somente um modo pode estar ativo.
    Para ativar um modo escolha uma das seguintes flags:
    --Rename  : Ativa o modo para renomear arquivos
    --Convert : Ativa o modo para converter arquivos
    --Resize  : Ativa o modo para redimensionar arquivos
    --Scale   : Ativa o modo para escalar arquivos
    As seguintes op��es s�o comuns a todos os modos:
    --Folder  : Especifica a pasta onde o PhotoBatch ir� atuar
    --Filter  : Especifica o filtro usado para selecionar arquivos
Op��es do modo Rename:
    No modo Rename, cada arquivo selecionado ser� renomeado para o formato
    <Prefix>_<N>.<ext>, onde <Prefix> e <N> s�o especificados atrav�s de op��es.
    <ext> � a extens�o do arquivo que permanece inalterada.
    --Prefix=<Prefix> : Prefixo usado ao renomear os arquivos
    --StartNumber=<N> : N�mero inicial a ser adicionado em cada arquivo que for renomeado
Op��es do modo Convert:
    No modo Convert, cada arquivo selecionado ser� convertido do formato <from> para
    o formato <to>. <from> e <to> n�o podem ser iguais. O nome do arquivo permanece
    inalterado.
    --From=<jpg|png|tga> : Extens�o de origem para convers�o
    --To=<jpg|png|tga>   : Extens�o de destino para convers�o
Op��es do modo Resize:
    No modo Resize, cada arquivo selecionado ser� redimensionado para as dimens�es
    especificadas nas seguintes op��es. Note que N >= 1.
    --Width=<N>  : Novo comprimento da imagem
    --Height=<N> : Nova altura da imagem
Op��es do modo Scale:
    No modo Scale, cada arquivo selecionado ser� escalado uniformemente pelo
    fator especificado pelas seguintes op��es. Note que N > 0 e ser� interpretado
    como um n�mero em ponto flutuante.
    --Amount=<N> : Fator de escala a ser aplicado em cada imagem selecionada.
Exemplos de uso:
    Renomear todos os arquivos da pasta "C:\Viagem" com a extens�o ".png" para Viagem_N.png, onde N come�a do 0
        PhotoBatch --Rename --Folder="C:\Viagem" --Filter=".png" --Prefix="Viagem_" --StartNumber=0
    Redimensionar todos os arquivos na pasta "C:\Viagem" com a extens�o ".jpg" para o tamanho 500x500
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
			std::cerr << "Para mais informa��es utilize a flag --help" << std::endl;
		}
	}


	return 0;
}