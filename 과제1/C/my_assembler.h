/* 
 * my_assembler 함수를 위한 변수 선언 및 매크로를 담고 있는 헤더 파일이다. 
 * 
 */
#define MAX_INST 256
#define MAX_LINES 5000
#define MAX_OPERAND 3
#pragma warning(disable: 4996)

/*
 * instruction 목록 파일로 부터 정보를 받아와서 생성하는 구조체 변수이다.
 * 구조는 각자의 instruction set의 양식에 맞춰 직접 구현하되
 * 라인 별로 하나의 instruction을 저장한다.
 */
struct inst_unit
{
    char mnemonic_instruction[7]; //연상명령 최대 6글자
    int instruction_format;      //형식_1,2,3,4형식
    int opcode;                 //opcode_1byte안에 다 해결 됨.
    int numoperand;            //0~2
};

// instruction의 정보를 가진 구조체를 관리하는 테이블 생성
typedef struct inst_unit inst;
inst *inst_table[MAX_INST];
int inst_index;

/*
 * 어셈블리 할 소스코드를 입력받는 테이블이다. 라인 단위로 관리할 수 있다.
 */
char *input_data[MAX_LINES];
static int line_num;

/*
 * 어셈블리 할 소스코드를 토큰단위로 관리하기 위한 구조체 변수이다.
 * operator는 renaming을 허용한다.
 * nixbpe는 8bit 중 하위 6개의 bit를 이용하여 n,i,x,b,p,e를 표시한다.
 */
struct token_unit
{
	char *label;				//명령어 라인 중 label
	char *operator;				//명령어 라인 중 operator
	char *operand[MAX_OPERAND]; //명령어 라인 중 operand
	char *comment;				//명령어 라인 중 comment
	char nixbpe;				// 추후 프로젝트에서 사용된다.
};

typedef struct token_unit token;
token *token_table[MAX_LINES];
static int token_line;

/*
 * 심볼을 관리하는 구조체이다.
 * 심볼 테이블은 심볼 이름, 심볼의 위치로 구성된다.
 */
struct symbol_unit
{
	char symbol[10];
	int addr;
};
typedef struct symbol_unit symbol;
symbol sym_table[MAX_LINES];
static int symtab_line;
static int current_symtab_line;

int find_symbol_addr(char* str);
/*
* 리터럴을 관리하는 구조체이다.
* 리터럴 테이블은 리터럴의 이름, 리터럴의 위치로 구성된다.
* 추후 프로젝트에서 사용된다.
*/
struct literal_unit
{
	char literal[10];
	int addr;
};
typedef struct literal_unit literal;
literal literal_table[MAX_LINES];
static int literal_line;/*literaltable의 총 라인 수*/
static int current_littab_line;/*literaltable의 다음line*/
static int locctr;
static int literalsize;/*리터럴의 사이즈 저장하는 변수, locctr을 연산하기 위하여 사용*/
static int LTORGline;
int isliteral(char* str);
int isliteralexist(char* str);
/******************H D R T M ******************/

struct H {
    char H[60];
}; typedef struct H H;
H H_table[3];
int Hline;

struct D {
    char D[60];
};
typedef struct D D;
D D_table[3];
int Dline;

struct R {
    char R[60];
};
typedef struct R R;
R R_table[3];
int Rline;

struct T {
    char T[60];
    int len;
    int controlsection;
    int startaddr;
}; typedef struct T T;
T T_table[6];
int Tline;

struct M {
    char M[60];
    int startbyte;
    int size;
    int controlsection;
}; typedef struct M M;
M M_table[10];
int Mline;
/******************control section***********************/
struct controlsection {
    char name[10];
    int length;
    int startaddr;
    char* EXTREF[3];
    char* EXTDEF[3];
};
typedef struct controlsection control;
control control_table[5];
static int current_control_line;
static int control_line;

/********pass2 object코드를 만드는데 사용하는 BUFFER***********/
char LINEBUFFER[70];/*1~9 10~69 */
char BUFFER[50];

void docalculate(int i);
int isimmediate(char* str);
int isformat4(char* str);
void make_littab(int i);
void make_symtab(int i);
int find_symbol(char* str);

/*********pass2에서 불리는 함수*****/
void divisionoperator(int* length, int i);
void assembledirectives(int i);
void assembleLinkDirectives(int i);
void assembleinstructions(int i);
void assembleSTARTENDdirectives(int i);

/*nixbpe를 상황에 맞게 설정하는 함수*/
void addnixbpe(int line);

/*동일한 control section에 존재하는 경우 1을 리턴*/
int isinsamecontrol(char* tok);

/*literal의 size를 구하여 literalsize에 저장하는 함수*/
void sizeliteral(int i, char* XORC, char* token);
/**token_table[i]->operator에 해당하는 형식(1~4)을 찾아 리턴해주는 함수**/
int findcurrentformat(int i);
/*3형식에서 disp부분을 구하는 함수**/
void calculatedisp(int* dp, int i);
void initializeLOCCTR(int i);

/*locctr를 구하기 위해, 해당하는 값을 더하는 함수*/
void addlocctr(int i);

static char *input_file;
static char *output_file;
int init_my_assembler(void);
int init_inst_file(char *inst_file);
int init_input_file(char *input_file);
int token_parsing(char *str);
static int assem_pass1(void);


void make_symtab_output(char *file_name);
void make_literaltab_output(char *file_name);
static int assem_pass2(void);
void make_objectcode_output(char *file_name);
int search_operator(char* str);
int search_opcode(char* str);
