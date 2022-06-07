#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>




class Parse {
public:
  union body;

  union lit;

  struct ast_node;

  typedef struct ast_node {
    Lex::token *token;
    std::string label;//used in code generation only and not in parsing
    struct ast_node *left = NULL;
    struct ast_node *right = NULL;
    struct ast_node *next = NULL;
    struct ast_node *back = NULL;
  } ast_node;

  std::vector<Lex::token> tokens;

  Parse(std::vector<Lex::token> tokens_a) {
    tokens = tokens_a;



     main_parse();
    


    // for debug

    /*

    for (int i = 0; i < tokens_a.size(); i++) {
      std::cout << tokens_a[i].name << "\n";
    }

for(int i =0; i<20; i++){
  std::cout << "Name: "<<program[i].token->name<< " Next: "<<program[i].next <<
" Right: "<<program[i].right << " Left: "<<program[i].left<<" Current: "<<
&program[i]<<"\n";
  }



      ast_node *buffer = &program[0];
  while (buffer->token->type != Lex::end_main) {
    std::cout << buffer->token->name<<"\n";
    if (buffer->right != NULL) {
      buffer = buffer->right;
    } else if (buffer->next != NULL){
      buffer = buffer->next;
    }else if (buffer->left != NULL){
      buffer = buffer->left;
    }
  }



*/
  }

  ast_node program[2000];
  int program_iter = 0;

  int iter_tokens = 0; // keeps track of position in tokens
  int upperlimit = tokens.size();

  void match(std::string matcher) {
    if (matcher != tokens[iter_tokens].name) {
      std::cout << "Yeah I was expecting " << matcher << "but got "
                << tokens[iter_tokens].name << "\n";
      exit(-1);
    } else {
      iter_tokens++;
    }
  }

  void print_parse() {
    // parse key word print
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].next = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;
    match("(");
    // parse what is inside the print
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;
    match(")");
    // parses the ;
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter].back = &program[program_iter-1]; //back
    match(";");
    program_iter++;
  }

  void var_parse() {
    // parses the keyword var
    tokens[iter_tokens].type = Lex::var;
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].next = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;
    
    // parses the name of var
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;
    // parses the value
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;

    // parses the ;
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter].back = &program[program_iter-1]; //back
    match(";");
    program_iter++;
  }

  void varible_operations() {
    // parses the name of var
    tokens[iter_tokens].type = Lex::var;
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].next = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;
    // parses the operation
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;
    // parses the value or varible being operated on to the previous one
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens++;

    // parses the ;
    program[program_iter - 1].right = &program[program_iter];
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter].back = &program[program_iter-1]; //back
    match(";");
    program_iter++;
  }

  void while_if_parse(){

    //parse first operand
    iter_tokens+=1;
    match("(");
    iter_tokens-=2;

    iter_tokens+=2;
    program[program_iter].back = &program[program_iter-1]; //back
    program[program_iter++].token = &tokens[iter_tokens];
    iter_tokens-=2;

    //parse second operand
    iter_tokens+=4;
    program[program_iter].back = &program[program_iter-1]; //back
    program[program_iter++].token = &tokens[iter_tokens];

    iter_tokens-=4;

    //parse into tree structure through logic operation
    iter_tokens+=3;
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter].left=&program[program_iter-2];
    program[program_iter].right=&program[program_iter-1];
    program[program_iter].back = &program[program_iter-1]; //back
    iter_tokens-=3;

    iter_tokens+=1;
    match("(");
    iter_tokens-=2;
    //adds a branch into the while statement
    program_iter++;
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 4].next = &program[program_iter];
    program[program_iter].right = &program[program_iter-1];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    iter_tokens+=6;
  }

  
  void block_parse(std::string what_to_name) {

    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].next = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back
    program_iter++;
    match("{");

    int end_loc = iter_tokens; // location of the end of the block;
    int kt = 1;                // keep track of bracket.
    while (kt != 0) {
      // need to add error checking
      end_loc++;
      if (tokens[end_loc].name == "{") {
        kt += 1;
      }
      if (tokens[end_loc].name == "}") {
        kt -= 1;
      }
    }

    while (iter_tokens != end_loc) {

      if (tokens[iter_tokens].name == "print") {
        print_parse();
      } else if (tokens[iter_tokens].name == "var") {
        var_parse();
      } else if (tokens[iter_tokens].name == "{") {
        block_parse("anon");
        program_iter++;
      } else if (tokens[iter_tokens].name == "}") {
        break;
      } else if (tokens[iter_tokens].type == Lex::var) {
        varible_operations();
      } else if (tokens[iter_tokens].name == "while") {
        while_if_parse();
      }else if (tokens[iter_tokens].name == "if") {
        while_if_parse();
      } else {
        std::cout << "They are not girls \"" << tokens[iter_tokens].name
                  << "\": " << iter_tokens << "\n";
        exit(-1);
      }
    }
    program[program_iter].token = &tokens[iter_tokens];
    program[program_iter - 1].next = &program[program_iter];
    program[program_iter].back = &program[program_iter-1]; //back

    match("}");
  }

  void main_parse() {
    program[program_iter].token = &tokens[iter_tokens];
    program_iter++;
    match("main");
    block_parse("main");
    program[program_iter].token->type = Lex::end_main;
  }
};