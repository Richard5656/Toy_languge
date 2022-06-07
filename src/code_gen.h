class Code_gen {

public:
  typedef struct symbol {
    int offset_bp_int;
    int is_integer = 0; // 1 is yes 0 is no
  } symbol;

  std::unordered_map<std::string, symbol> sym_table;
  int label_count=0;
  std::string assembly_code = "";
  int bp_kt; // keeps track of bas pointer off

  Code_gen(Parse parser) {
    Parse::ast_node *buffer = &parser.program[0];
    while (buffer->token->type != Lex::end_main) {
      if (buffer->token->name == "print") { // handler for print
        buffer = buffer->right;
        if (buffer->token->type == Lex::int_lit) { // handles int_lit
          assembly_code += "IMM " + buffer->token->name + "\nPRI\n";
        } else if (buffer->token->type == Lex::var) { // handles int varibles will need to change if I
          if (sym_table[buffer->token->name].is_integer == 1) {
               assembly_code += "NDLDA " + std::to_string(-sym_table[buffer->token->name].offset_bp_int) + "\nPRI\n";
          }
        }
        buffer = buffer->right;
      } else if (buffer->token->name == "var") {
        symbol buffer_for_var;
        if (buffer->right->right->token->type == Lex::int_lit) {
          buffer = buffer->right;
          bp_kt++;
          buffer_for_var.offset_bp_int = bp_kt;
          buffer_for_var.is_integer = 1;
          sym_table[buffer->token->name] = buffer_for_var;
          buffer = buffer->right;
          assembly_code += "IMM " + buffer->token->name + "\nSTA " +
                           std::to_string(-bp_kt) + "\n";
          buffer = buffer->right;
        }
      }else if(buffer->token->type == Lex::var){ //handles arithmatic
      std::string operation;
      std::string assembly_buffer=std::to_string(-sym_table[buffer->token->name].offset_bp_int);
      buffer = buffer->right;

      if(buffer->token->name == "+=")operation = "ADD";
      if(buffer->token->name == "-=")operation = "SUB";
      if(buffer->token->name == "*=")operation = "MUL";
      if(buffer->token->name == "/=")operation = "DIV";
	  
	  


      buffer = buffer->right;
        
      if(buffer->token->type == Lex::int_lit){
        assembly_code += "IMM "+ buffer->token->name + "\nPSH\n";
      }else if(buffer->token->type == Lex::var){
        assembly_code += "\nNDLDA "+ std::to_string(-sym_table[buffer->token->name].offset_bp_int)+ "\nPSH\n";
      }
        assembly_code+= "NDLDA "+ assembly_buffer + "\n" + operation + "\n" +"STA " + assembly_buffer +"\n";
        
          buffer = buffer->right;
      }else if(buffer->token->name == "{" && buffer->back->token->type == Lex::while_t){
			label_count++;
			std::string stringlc = std::to_string(label_count);
			assembly_code+= "JMP _L" + stringlc +"\nLABEL L" + stringlc + "\n";
			buffer->label = "L" + std::to_string(label_count) + "\n";
        }else if(buffer->token->name == "{" && buffer->back->token->type == Lex::if_t){
			Parse::ast_node * buffer_buffer = buffer->back;
			
			std::string operation;
			std::string first_param; 
			std::string second_param;
			
			if(buffer_buffer->right->token->name == ".=")operation = "EQ 0";
			if(buffer_buffer->right->token->name == "<")operation = "LE 0";
			if(buffer_buffer->right->token->name == ">")operation = "GT 0";
			if(buffer_buffer->right->token->name == "!=")operation = "NE 0";
			label_count++;
			buffer_buffer= buffer_buffer->right;
			
			if(buffer_buffer->left->token->type == Lex::int_lit)first_param = "IMM " + buffer_buffer->left->token->name+"\n";

			if(buffer_buffer->right->token->type == Lex::int_lit)second_param = "IMM " + buffer_buffer->right->token->name+"\n";
        
			if(buffer_buffer->left->token->type == Lex::var){
      
				first_param = "NDLDA " +std::to_string(-sym_table[ buffer_buffer->left->token->name].offset_bp_int) +"\n";  
			}

			if(buffer_buffer->right->token->type == Lex::var){
      
				second_param = "NDLDA " +std::to_string(-sym_table[ buffer_buffer->right->token->name].offset_bp_int)+"\n";  
			}  
			
			
			
			
			
			
			
			
			std::string stringlc = "L"+std::to_string(label_count);
			assembly_code+=  first_param + "STA 0\n" + second_param + operation + "\nJNZ " + stringlc  + "\n";
			
			buffer->label = stringlc + "\n";
			
        }else if (buffer->token->name == "}"){
			int kt = 1;//keep track of brackets when going backwards
			Parse::ast_node * buffer_buffer = buffer->back;
        while(kt !=0){
            if(buffer_buffer->token->name == "{"){
              kt--;
            }

            if(buffer_buffer->token->name == "}" ){
              kt++;
            }
            buffer_buffer = buffer_buffer->back;
          }
        std::string label = buffer_buffer->next->label;// gets the label from {
        
        
      if(buffer_buffer->token->name == "while"){
			assembly_code+="LABEL _"+label;
          std::string operation;
          std::string first_param; 
          std::string second_param;
        if(buffer_buffer->right->token->name == ".=")operation = "EQ 0";
        if(buffer_buffer->right->token->name == "<")operation = "LE 0";
        if(buffer_buffer->right->token->name == ">")operation = "GT 0";
        if(buffer_buffer->right->token->name == "!=")operation = "NE 0";

        buffer_buffer = buffer_buffer->right;
        
    if(buffer_buffer->left->token->type == Lex::int_lit)first_param = "IMM " + buffer_buffer->left->token->name;

    if(buffer_buffer->right->token->type == Lex::int_lit)second_param = "IMM " + buffer_buffer->right->token->name;
        
    if(buffer_buffer->left->token->type == Lex::var){
      
  first_param = "NDLDA " +std::to_string(-sym_table[ buffer_buffer->left->token->name].offset_bp_int);  
      }

    if(buffer_buffer->right->token->type == Lex::var){
      
  second_param = "NDLDA " +std::to_string(-sym_table[ buffer_buffer->right->token->name].offset_bp_int);  
      }  

      assembly_code += first_param+="\nSTA 0\n" + second_param+"\n" + operation+"\n" + "JZ " + label;
    }
	
	
	if(buffer_buffer->token->name == "if"){
		assembly_code+="LABEL "+ label+"\n";
		
	}
}

        
      buffer = buffer->next;
      
    }

    assembly_code += "HLT\n";
    //std::cout << assembly_code;
  }
};