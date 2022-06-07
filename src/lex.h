#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>
class Lex {

public:
  enum {
    while_t,
    if_t,
    open_brace,
    closed_brace,
    open_bracket,
    closed_bracket,
    equal,
    double_equal,
    less_than,
    greater_than,
    equal_equal,
    less_than_or_equal_to,
    greater_than_or_equal_to,
    not_yet,
    int_lit,
    str_lit,
    semi_col,
    print,
    main,
    end_main,
    var,
    signify_var //$
  };
  std::string constants = "0123456789";
  typedef struct token {
    std::string name;
    int type;
  } token;

  std::string key_words[6] = {"while", "int", "str", "print", "main"};
  std::string identifiers[13] = {"(",  ")",  "{",  "}",  ">",  "<",
                                 ">=", "<=", "==", "+=", "-=", "="};

  token match(std::string possible_token) {
    token token_to_be_returned;
    token_to_be_returned.name = possible_token;
    if (possible_token == "while") {
      token_to_be_returned.type = while_t;
      return token_to_be_returned;
    } else if (possible_token == "var") {
      token_to_be_returned.type = 103; // code for intlit start
      return token_to_be_returned;
    } else if (possible_token == "(") {
      token_to_be_returned.type = open_brace;
      return token_to_be_returned;
    } else if (possible_token == ")") {
      token_to_be_returned.type = closed_brace;
      return token_to_be_returned;
    } else if (possible_token == "{") {
      token_to_be_returned.type = open_bracket;
      return token_to_be_returned;
    } else if (possible_token == "}") {
      token_to_be_returned.type = closed_bracket;
      return token_to_be_returned;
    } else if (possible_token == "<") {
      token_to_be_returned.type = less_than;
      return token_to_be_returned;
    } else if (possible_token == ">") {
      token_to_be_returned.type = greater_than;
      return token_to_be_returned;
    } else if (possible_token == ".=") {
      token_to_be_returned.type = equal_equal;
      return token_to_be_returned;
    }else if (possible_token == "!=") {
      token_to_be_returned.type = equal_equal;
      return token_to_be_returned;
    } else if (possible_token == "=") {
      token_to_be_returned.type = equal;
      return token_to_be_returned;
    } else if (possible_token == "+=") {
      token_to_be_returned.type = equal;
      return token_to_be_returned;
    } else if (possible_token == "-=") {
      token_to_be_returned.type = equal;
      return token_to_be_returned;
    } else if (possible_token == "*=") {
      token_to_be_returned.type = equal;
      return token_to_be_returned;
    } else if (possible_token == "/=") {
      token_to_be_returned.type = equal;
      return token_to_be_returned;
    } else if (possible_token == "print") {
      token_to_be_returned.type = print;
      return token_to_be_returned;
    }else if (possible_token == "if") {
      token_to_be_returned.type = if_t;
      return token_to_be_returned;
    } else if (possible_token == ";") {
      token_to_be_returned.type = semi_col;
      return token_to_be_returned;
    } else if (possible_token == "$") {
      token_to_be_returned.type = 104;
      return token_to_be_returned;
    } else if (possible_token == "main") {
      token_to_be_returned.type = main;
      return token_to_be_returned;
    } else if (possible_token == "\"") {
      token_to_be_returned.type = 101; // code for stringlit start
      return token_to_be_returned;
    } else if (isdigit(possible_token[0])) {
      token_to_be_returned.type = 102; // code for intlit start
      return token_to_be_returned;
    } else {
      token_to_be_returned.type = not_yet;
      return token_to_be_returned;
    }
  }

  void append_tok(token to_psh) { tokens.push_back(to_psh); }

  std::vector<token> tokens;

  Lex(std::string code) {
    int counter = 0;         // counts through the code
    std::string store_token; // stores characters to build up tokens
    for (int i = 0; i < code.size(); i++) {
      if (code[i] == '	' ||code[i] == ' ' || code[i] == '\n') {
        continue;
      }

      store_token += code[i];
      token match_buf = match(store_token);
      if (match_buf.type != not_yet && match_buf.type != 101 &&
          match_buf.type != 102 && match_buf.type != 103 &&
          match_buf.type != 104) {
        append_tok(match_buf);
        store_token = "";
      } else if (match_buf.type == 101) { // for string handling
        i++;
        while (code[i] != '"') {
          store_token += code[i];
          i++;

          if (i >= code.size()) {
            std::cout << "Sir there seems to be a problem with your string "
                         "literals\n";
            exit(-1);
          }
        }

        store_token += code[i];
        append_tok(match(store_token));

        store_token = "";

      } else if (match_buf.type == 102) { // for string handling
        i++;
        while (isdigit(code[i])) {
          store_token += code[i];
          i++;

          if (i >= code.size()) {
            std::cout
                << "Sir there seems to be a problem with your int litetals\n";
            exit(-1);
          }
        }

        token tok_buffer;
        tok_buffer.name = store_token;
        tok_buffer.type = int_lit;
        append_tok(tok_buffer);
        store_token = "";
        i--;
      } else if (match_buf.type == 103) { // varible handling
        token tok_buffer;
        tok_buffer.name = "var";
        tok_buffer.type = var;
        append_tok(tok_buffer);
        // keep going

        store_token = "";
        i++;
        while (code[i] != '=') {
          if (code[i] != ' ') {
            store_token += code[i];
          }
          i++;
          if (i >= code.size()) {
            std::cout
                << "Sir there seems to be a problem with your varible names\n";
            exit(-1);
          }
        }

        tok_buffer.name = store_token;
        tok_buffer.type = var;
        append_tok(tok_buffer);
        store_token = "";
      } else if (match_buf.type == 104) {
        token tok_buffer;

        store_token = "";
        i++;
        while (code[i] != ';' && code[i] != ' ' && code[i] != ')' &&
               code[i] != '+' && code[i] != '-' && code[i] != '/' &&
               code[i] != '*'&&
               code[i] != '<'
          &&
               code[i] != '>'
          &&
               code[i] != '.'
          &&
               code[i] != '!') {
          if (code[i] != ' ') {
            store_token += code[i];
          }
          i++;
        }

        tok_buffer.name = store_token;
        tok_buffer.type = var;
        append_tok(tok_buffer);
        store_token = "";
        i--;
      }
    }
  }
};