/* 
 * my_assembler �Լ��� ���� ���� ���� �� ��ũ�θ� ��� �ִ� ��� �����̴�. 
 * 
 */
#define MAX_INST 256
#define MAX_LINES 5000
#define MAX_OPERAND 3
#pragma warning(disable: 4996)

/*
 * instruction ��� ���Ϸ� ���� ������ �޾ƿͼ� �����ϴ� ����ü �����̴�.
 * ������ ������ instruction set�� ��Ŀ� ���� ���� �����ϵ�
 * ���� ���� �ϳ��� instruction�� �����Ѵ�.
 */
struct inst_unit
{
    char mnemonic_instruction[7]; //������ �ִ� 6����
    int instruction_format;      //����_1,2,3,4����
    int opcode;                 //opcode_1byte�ȿ� �� �ذ� ��.
    int numoperand;            //0~2
};

// instruction�� ������ ���� ����ü�� �����ϴ� ���̺� ����
typedef struct inst_unit inst;
inst *inst_table[MAX_INST];
int inst_index;

/*
 * ����� �� �ҽ��ڵ带 �Է¹޴� ���̺��̴�. ���� ������ ������ �� �ִ�.
 */
char *input_data[MAX_LINES];
static int line_num;

/*
 * ����� �� �ҽ��ڵ带 ��ū������ �����ϱ� ���� ����ü �����̴�.
 * operator�� renaming�� ����Ѵ�.
 * nixbpe�� 8bit �� ���� 6���� bit�� �̿��Ͽ� n,i,x,b,p,e�� ǥ���Ѵ�.
 */
struct token_unit
{
	char *label;				//��ɾ� ���� �� label
	char *operator;				//��ɾ� ���� �� operator
	char *operand[MAX_OPERAND]; //��ɾ� ���� �� operand
	char *comment;				//��ɾ� ���� �� comment
	char nixbpe;				// ���� ������Ʈ���� ���ȴ�.
};

typedef struct token_unit token;
token *token_table[MAX_LINES];
static int token_line;

/*
 * �ɺ��� �����ϴ� ����ü�̴�.
 * �ɺ� ���̺��� �ɺ� �̸�, �ɺ��� ��ġ�� �����ȴ�.
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
* ���ͷ��� �����ϴ� ����ü�̴�.
* ���ͷ� ���̺��� ���ͷ��� �̸�, ���ͷ��� ��ġ�� �����ȴ�.
* ���� ������Ʈ���� ���ȴ�.
*/
struct literal_unit
{
	char literal[10];
	int addr;
};
typedef struct literal_unit literal;
literal literal_table[MAX_LINES];
static int literal_line;/*literaltable�� �� ���� ��*/
static int current_littab_line;/*literaltable�� ����line*/
static int locctr;
static int literalsize;/*���ͷ��� ������ �����ϴ� ����, locctr�� �����ϱ� ���Ͽ� ���*/
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

/********pass2 object�ڵ带 ����µ� ����ϴ� BUFFER***********/
char LINEBUFFER[70];/*1~9 10~69 */
char BUFFER[50];

void docalculate(int i);
int isimmediate(char* str);
int isformat4(char* str);
void make_littab(int i);
void make_symtab(int i);
int find_symbol(char* str);

/*********pass2���� �Ҹ��� �Լ�*****/
void divisionoperator(int* length, int i);
void assembledirectives(int i);
void assembleLinkDirectives(int i);
void assembleinstructions(int i);
void assembleSTARTENDdirectives(int i);

/*nixbpe�� ��Ȳ�� �°� �����ϴ� �Լ�*/
void addnixbpe(int line);

/*������ control section�� �����ϴ� ��� 1�� ����*/
int isinsamecontrol(char* tok);

/*literal�� size�� ���Ͽ� literalsize�� �����ϴ� �Լ�*/
void sizeliteral(int i, char* XORC, char* token);
/**token_table[i]->operator�� �ش��ϴ� ����(1~4)�� ã�� �������ִ� �Լ�**/
int findcurrentformat(int i);
/*3���Ŀ��� disp�κ��� ���ϴ� �Լ�**/
void calculatedisp(int* dp, int i);
void initializeLOCCTR(int i);

/*locctr�� ���ϱ� ����, �ش��ϴ� ���� ���ϴ� �Լ�*/
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
