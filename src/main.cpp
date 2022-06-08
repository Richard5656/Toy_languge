#include "inter.h"
#include "lex.h"
#include "parser.h"
#include "assembler_abstract.h"
#include "assembler_direct.h"
#include <ctype.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <vector>
#include<fstream>
#include<sstream>
#include<string>
#include "code_gen.h"

int main(int argc, char **argv) {
	 if(argc < 2){
		  std::cout<<"-admp dump machine code for inerpreted lang\n-idmp machine code dump for assembly abstract\n-id interpret to abstract assembly\n-asd Assembler Direct\n-asa Assembler Abstract\n-i Interpreter\n"; exit(-1);
    }
	std::string code;
	std::ifstream fp(argv[2]);
	
	if(!fp){
		std::cout<<"File empty\n";
		exit(-1);
	}
	
	std::ostringstream ss;
    ss << fp.rdbuf();
    code = ss.str();
	
	
	

	CPU cpu(10000);
    if(strcmp(argv[1],"-i")==0){
		  Lex lexer(code);
		  Parse parser(lexer.tokens);
		  Code_gen code_gen(parser);    
		  abs_Assembler assembler(code_gen.assembly_code);
		  assembler.copy_memory(&cpu);
		  cpu.run();  
    }else if(strcmp(argv[1],"-asa")==0){  
		  abs_Assembler assembler(code);
		  assembler.copy_memory(&cpu);
		  cpu.run();  
    }else if(strcmp(argv[1],"-asd")==0){  
		  dir_Assembler assembler(code);
		  assembler.copy_memory(&cpu);
		  cpu.run();  
    }else if(strcmp(argv[1],"-id")==0){
			
		  Lex lexer(code);
		  Parse parser(lexer.tokens);
		  Code_gen code_gen(parser);
		  std::cout<<code_gen.assembly_code;
    }else if(strcmp(argv[1],"-idmp")==0){  
		  Lex lexer(code);
		  Parse parser(lexer.tokens);
		  Code_gen code_gen(parser);
		  abs_Assembler assembler(code_gen.assembly_code);
		  assembler.print_memory();
    }else if(strcmp(argv[1],"-admp")==0){  
		  abs_Assembler assembler(code);
		  assembler.print_memory();
    }
  return 0;
}