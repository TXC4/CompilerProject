#include "Lex.h"
#include "Parser.h"
#include "CodeGen.h"

int main()
{
	lex();
	parse();
	generateCode();
}