// abstract assembler abstractions for jumps
#include <assert.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
class abs_Assembler {
public:
  int kt_mem = 0; // keep track of memeory
  std::string keep_track = "";
  std::unordered_map<std::string, int>
      jmem; // stores labels and their memory locations to be jumped too.
  std::vector<int> memory;
  abs_Assembler(std::string code) {
    count_labels(code);
    lexer(code);


  };

  void match_operation(std::string pos_tok) {
    int add_to_mem = -1;
    if (pos_tok == "HLT")
      add_to_mem = CPU::HLT;
    if (pos_tok == "IMM")
      add_to_mem = CPU::IMM;
    if (pos_tok == "JMP")
      add_to_mem = CPU::JMP;
    if (pos_tok == "STA")
      add_to_mem = CPU::STA;
    if (pos_tok == "ADD")
      add_to_mem = CPU::ADD;
    if (pos_tok == "SUB")
      add_to_mem = CPU::SUB;
    if (pos_tok == "PSH")
      add_to_mem = CPU::PSH;
    if (pos_tok == "PRI")
      add_to_mem = CPU::PRI;
    if (pos_tok == "PRC")
      add_to_mem = CPU::PRC;
    if (pos_tok == "MUL")
      add_to_mem = CPU::MUL;
    if (pos_tok == "DIV")
      add_to_mem = CPU::DIV;
    if (pos_tok == "POP")
      add_to_mem = CPU::POP;
    if (pos_tok == "LDA")
      add_to_mem = CPU::LDA;
    if (pos_tok == "JNZ")
      add_to_mem = CPU::JNZ;
    if (pos_tok == "JZ")
      add_to_mem = CPU::JZ;
    if (pos_tok == "CALL")
      add_to_mem = CPU::CALL;
    if (pos_tok == "ADJ")
      add_to_mem = CPU::ADJ;
    if (pos_tok == "RET")
      add_to_mem = CPU::RET;
    if (pos_tok == "LDAO")
      add_to_mem = CPU::LDAO;
    if (pos_tok == "STAO")
      add_to_mem = CPU::STAO;
    if (pos_tok == "LE")
      add_to_mem = CPU::LE;
    if (pos_tok == "GT")
      add_to_mem = CPU::GT;
    if (pos_tok == "EQ")
      add_to_mem = CPU::EQ;
    if (pos_tok == "NE")
      add_to_mem = CPU::NE;
    if (pos_tok == "NDLDA")
      add_to_mem = CPU::NDLDA;
    if (pos_tok == "NDSTA")
      add_to_mem = CPU::NDSTA;
    if (add_to_mem != -1) {
      memory.push_back(add_to_mem);
      keep_track = "";
    }
  }

  void match_operation_count(std::string pos_tok) {
    int add_to_mem = -1;
    if (pos_tok == "HLT")
      add_to_mem = CPU::HLT;
    if (pos_tok == "IMM")
      add_to_mem = CPU::IMM;
    if (pos_tok == "STA")
      add_to_mem = CPU::STA;
    if (pos_tok == "ADD")
      add_to_mem = CPU::ADD;
    if (pos_tok == "SUB")
      add_to_mem = CPU::SUB;
    if (pos_tok == "PSH")
      add_to_mem = CPU::PSH;
    if (pos_tok == "PRI")
      add_to_mem = CPU::PRI;
    if (pos_tok == "PRC")
      add_to_mem = CPU::PRC;
    if (pos_tok == "MUL")
      add_to_mem = CPU::MUL;
    if (pos_tok == "DIV")
      add_to_mem = CPU::DIV;
    if (pos_tok == "POP")
      add_to_mem = CPU::POP;
    if (pos_tok == "LDA")
      add_to_mem = CPU::LDA;
    if (pos_tok == "ADJ")
      add_to_mem = CPU::ADJ;
    if (pos_tok == "RET")
      add_to_mem = CPU::RET;
    if (pos_tok == "LDAO")
      add_to_mem = CPU::LDAO;
    if (pos_tok == "STAO")
      add_to_mem = CPU::STAO;
    if (pos_tok == "LE")
      add_to_mem = CPU::LE;
    if (pos_tok == "GT")
      add_to_mem = CPU::GT;
    if (pos_tok == "EQ")
      add_to_mem = CPU::EQ;
    if (pos_tok == "NE")
      add_to_mem = CPU::NE;
    if (pos_tok == "NDLDA")
      add_to_mem = CPU::NDLDA;
    if (pos_tok == "NDSTA")
      add_to_mem = CPU::NDSTA;
  
    if (add_to_mem != -1) {
      kt_mem++;
      keep_track = "";
    }
  }

  void count_labels(std::string code) {
	  
	  
    for (int i = 0; i < code.length(); i++) {

      if (code[i] == ';') {
        while (code[i] != '\n') {
          i++;
        }
      }

      if (code[i] != ' ' && code[i] != '\n'&& code[i] != '\t'&& code[i] != '\r') {
        keep_track += code[i];
        if (keep_track == "LABEL") {
          keep_track = "";
          while (code[i] != '\n') {
            i++;
            if (code[i] != ' ') {
              keep_track += code[i];
            }
          }
          jmem[keep_track] = kt_mem;
          keep_track = "";
        }

        int is_type_of_jump = 0;
        if (keep_track == "JNZ")
          is_type_of_jump = 1;
        if (keep_track == "JZ")
          is_type_of_jump = 1;
        if (keep_track == "CALL")
          is_type_of_jump = 1;
        if (keep_track == "JMP")
          is_type_of_jump = 1;

        if (is_type_of_jump == 1) {
          keep_track = "";
          while (code[i] != '\n') {
            i++;
          }
          kt_mem += 2;
          keep_track = "";
        }

        match_operation_count(keep_track);

        // numerical handling
        if (isdigit(code[i])) {
          while (isdigit(code[i])) {
            i++;
          }
          keep_track = "";
          kt_mem++;
        }
      }
    }
    keep_track = "";
  }

  void lexer(std::string code) {
	 int digit_flag = 1;//keeps the Jump detection from detecting digits 2,13,14,15  1 for not digit 0 for is digit
	 
	 
    for (int i = 0; i < code.length(); i++) {
		
      if (code[i] == ';') {
        while (code[i] != '\n') {
          i++;
        }
      }
      if (code[i] != ' ' && code[i] != '\n'&& code[i] != '\t'&& code[i] != '\r') {	

		keep_track += code[i];

        int is_type_of_jump;
        if (memory.size() != 0) {
          is_type_of_jump = ((memory[memory.size() - 1] == CPU::JMP) +
                            (memory[memory.size() - 1] == CPU::JZ) +
                            (memory[memory.size() - 1] == CPU::JNZ) +
                            (memory[memory.size() - 1] == CPU::CALL))*digit_flag;
        }

        if (is_type_of_jump == 1) {
          while (code[i] != '\n') {
            i++;
            if (code[i] != ' ') {
              keep_track += code[i];
            }
          }
          memory.push_back(jmem[keep_track]);
          keep_track = "";
        }

        if (keep_track == "LABEL") {
          while (code[i] != '\n') {
            i++;
          }
          keep_track = "";
        }
		digit_flag =1;
		match_operation(keep_track);
        // numerical handling
        if (isdigit(code[i])||code[i]=='-') {
          int is_neg = 1;// -1 = true
          if(code[i] == '-'){
            i++;
            is_neg = -1;
          }
		  keep_track="";
		  
			  while (isdigit(code[i])) {
				keep_track += code[i];
				i++;
			  }
			  memory.push_back(atoi(keep_track.c_str()) * is_neg);
			  keep_track="";
			  digit_flag=0;
			}
		}
	}
}
  void copy_memory(CPU *cpu) {
    for (int i = 0; i < memory.size(); i++)
      cpu->memory[i] = (memory[i]);
  };
   void print_memory() {
    for (int i = 0; i < memory.size(); i++){

      std::cout<< std::to_string(memory[i])+" ";
	}
  };
};
