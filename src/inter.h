#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>
#define AmountOfMemory 40000
#include <iostream>
class CPU{
public:
std::string test = "Hello world\n";
    /*
    STACK section
    _________________
    HEAP section
    __________________
    TEXT  section
    __________________
    */
int ax = 0;
    int memory[AmountOfMemory];
int  ktsmp = 0; //keep_track_of how many call_stack_memory_address_pointers // so its practically a stack
    int * memory_address_holder[(int)(AmountOfMemory/2)];

    enum {
HLT,//0
IMM,//1
JMP,//2
STA,//3
ADD,//4
SUB,//5
PSH,//6
PRI,//7
PRC,//8
MUL,//9
DIV,//10
POP,//11
LDA,//12
JNZ,//13
JZ,//14
CALL,//15
ADJ,//16
RET,//17
LDAO,//18
STAO,//19
LE,//20
GT,//21
EQ,//22
NE,//23
NDLDA,//24
NDSTA//25
};
    // stack pointer, base pointer, program counter
    int *pc  = &memory[0];
int *sp = &memory[AmountOfMemory-1-1];
int *bp = sp;
CPU(int main_off){
sp-=main_off;
}
int run(){
            while(*pc != HLT){
//std::cout<<*pc<<std::endl; //debug code
switch(*pc){
            case IMM:{ ax = *++pc;}break;
            case PSH:{*--sp = ax; ax=0;}break;
            case ADD:{ax = *sp++ + ax;}break;
            case SUB:{ax =  ax-*sp++;}break;
            case MUL:{ax = *sp++ * ax;}break;
            case DIV:{ax = ax / *sp++;}break;
            case POP:{ax = *sp++;}break;
            case PRI:{std::cout<<ax<<"\n";} break;
            case PRC:{std::cout<<(char)ax;} break;
            case JMP:{pc=(int*)(&memory[0]+(int)*(pc+1))-1;}break;
            case JZ:{if(ax==0){pc=(int*)(&memory[0]+(int)*(pc+1))-1;}else{pc++;}}break;
            case JNZ:{if(ax!=0){pc=(int*)(&memory[0]+(int)*(pc+1))-1;}else{pc++;}}break;
            case LDA:{ax = *(bp+ *++pc);*(bp+ *pc)=0;} break;
            case STA:{*(bp+ *++pc)=ax; ax =0;} break;
            case NDLDA: {ax = *(bp+ *++pc);} break;
            case NDSTA:{*(bp+ *++pc)=ax;} break;
            case LDAO:{ax = *(bp+*(bp+ *++pc));*(bp+*(bp+ *++pc))=0;}break;
            case STAO:{*(bp+*(bp+ *++pc))=ax; ax =0;} break;
            case CALL:{*--sp = ktsmp++; memory_address_holder[ktsmp]=bp;*--sp = ktsmp++;memory_address_holder[ktsmp]=(pc+1);bp=sp;pc=(int*)(&memory[0]+(int)*(pc+1))-1;} break;
            case ADJ:{sp+=*++pc;}break;
            case RET:{pc = memory_address_holder[ktsmp--];sp++;bp= memory_address_holder[ktsmp--];sp++;}break;
            case LE:{ax = (ax<*(bp+ *++pc));}break;
            case GT:{ax = (ax>*(bp+ *++pc));}break;
            case EQ:{ax = (ax!=*(bp+ *++pc));}break;
            case NE:{ax = (ax==*(bp+ *++pc));}break;
			//case NOP:{*pc++;}break;
//case HLT:{exit(0);std::cout<<"\n*sucess*\n";}break;
            default:{std::cout<<"Invalid opcode now terminating " << *pc <<"\n";exit(1);}break;
                }
pc++;
//std::cout << ax << "\n";
            }
            return 0;
        }
};
