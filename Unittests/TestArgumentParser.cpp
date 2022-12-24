#include <gtest/gtest.h>
#include<ArgumentParser.h>
#include <array>

TEST(TestArgumentParser, RegisterFlag) {

	//cria um obj
	ArgumentParser argParser;

	//Verificar o estado antes da operação
	EXPECT_FALSE(argParser.isFlagRegistered("MyFlag"));

	//registrar a flag
	//chamar a função sendo testada
	argParser.RegisterFlag("MyFlag");


	//verificar o estado depois da operação
	EXPECT_TRUE(argParser.isFlagRegistered("MyFlag"));
}

TEST(TestArgumentParser, RegisterFlag_Whitespace) {

	ArgumentParser argParser;

	argParser.RegisterFlag("My flag");

	EXPECT_FALSE(argParser.isFlagRegistered("My flag"));

}

TEST(TestArgumentParser, RegisterOption) {

	ArgumentParser argParser;

	argParser.RegisterOption("MyOption");
	EXPECT_TRUE(argParser.isOptionRegistered("MyOption"));
}

TEST(TestArgumentParser, RegisterOption_Whitespace) {

	ArgumentParser argParser;

	argParser.RegisterOption("My Option");
	EXPECT_FALSE(argParser.isOptionRegistered("My Options"));
}

TEST(TestArgumentParser, Parse) {

	ArgumentParser argParser;

	std::array<const char*, 5> args =
	{
		"TestArgumentParser",
		"--flag",
		"--string=AaBbCcDd",
		"--number=42",
		"--float=4.2"
	};

	argParser.RegisterFlag("flag");
	argParser.RegisterOption("string");
	argParser.RegisterOption("number");
	argParser.RegisterOption("float");

	argParser.Parse(static_cast<int>(args.size()), args.data());

	EXPECT_FALSE(argParser.GetFlag("unregisteredFlag"));
	EXPECT_TRUE(argParser.GetFlag("flag"));
	EXPECT_EQ(argParser.GetOptionAs<std::string>("string"), "aabbccdd");
	EXPECT_EQ(argParser.GetOptionAs<int>("number"), 42);
	EXPECT_EQ(argParser.GetOptionAs<float>("float"), 4.2f);


}

TEST(TestArgumentParser, Parse_InvalidArguments) {

	ArgumentParser argParser;

	argParser.Parse(0, nullptr);
	argParser.Parse(42, nullptr);

}