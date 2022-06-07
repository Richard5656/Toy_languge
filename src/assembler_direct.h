// direct assembler no abstractions for jumps
#include <assert.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>



class dir_Assembler {
public:
  std::vector<int> memory;
  dir_Assembler(std::string code) {
    lexer(code);
    for (int i = 0; i < memory.size(); i++) {
      std::cout << memory[i] << "\n";
    }
  };

  int kt_mem = 0; // keep track of memeory
  std::string keep_track = "";

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
      kt_mem++;
      keep_track = "";
    }
  }

  void lexer(std::string code) {

    for (int i = 0; i < code.length(); i++) {
        if (code[i] == ';') {            
          while (code[i] != '\n'){
            i++;
          }
          i++;
        }



      
      if (code[i] != ' ' && code[i] != '\n') {
        keep_track += code[i];
        match_operation(keep_track);

        // numerical handling
        if (isdigit(code[i])||code[i]=='-') {
          int is_neg = 1;// - 1 = true
          if(code[i] == '-'){
            i++;
            is_neg = -1;
          }
          while (isdigit(code[i])) {
            i++;
            keep_track += code[i];
          }
          memory.push_back(atoi(keep_track.c_str()) * is_neg);
          keep_track = "";
        }
      }
    }
  }
    void copy_memory(CPU *cpu) {
    for (int i = 0; i < memory.size(); i++)
      cpu->memory[i] = (memory[i]);
  };
};