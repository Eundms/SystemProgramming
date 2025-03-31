/*
 * ȭ�ϸ� : my_assembler_00000000.c
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ���ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 * ���� ������ ���Ǵ� ���ڿ� "00000000"���� �ڽ��� �й��� �����Ѵ�.
 */

/*
 *
 * ���α׷��� ����� �����Ѵ�.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// ���ϸ��� "00000000"�� �ڽ��� �й����� ������ ��.
#include "my_assembler_.h"

/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ������� ������ �޾Ƽ� ���ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ������� ����
 * ��ȯ : ���� = 0, ���� = < 0
 * ���� : ���� ������� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�.
 *		   ���� �߰������� �������� �ʴ´�.
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[])
{
    if (init_my_assembler() < 0)
    {
        printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
        return -1;
    }

    if (assem_pass1() < 0)
    {
        printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
        return -1;
    }
    // make_opcode_output("output_00000000");

    make_symtab_output("symtab_.txt");
    make_literaltab_output("literaltab_.txt");

    if (assem_pass2() < 0)
    {
        printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
        return -1;
    }

    make_objectcode_output("output_");

    return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�.
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ���ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ�
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���.
 * ----------------------------------------------------------------------------------
 */
int init_my_assembler(void)
{
    int result;

    if ((result = init_inst_file("inst.data")) < 0)
        return -1;
    if ((result = init_input_file("input.txt")) < 0)
        return -1;
    return result;
}

/* ----------------------------------------------------------------------------------
 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)��
 *        �����ϴ� �Լ��̴�.
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0
 * ���� : ���� ������� ������ �����Ӱ� �����Ѵ�. ���ô� ������ ����.
 *
 *	===============================================================================
 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
 *	===============================================================================
 *
 * ----------------------------------------------------------------------------------
 */
int init_inst_file(char *inst_file)
{
    printf("------------------inst_file------------------\n");
    printf("NAME\tFORM\tOPCODE\tOPERATOR\n");
    FILE *file;
    int errno;
    file = fopen(inst_file, "r");
    if (file == NULL)
    {
        return -1;
    }
    char mnemonic_instruction[7];
    int instruction_format;
    int opcode;
    int numoperand;

    for (int i = 0; i < 59; i++)
    {
        fscanf(file, "%s\t%d\t%x\t%d\n", &mnemonic_instruction, &instruction_format, &opcode, &numoperand);
        /*inst_table�� ����*/
        inst_table[i] = (inst *)malloc(sizeof(inst));
        strcpy(inst_table[i]->mnemonic_instruction, mnemonic_instruction);
        inst_table[i]->instruction_format = instruction_format;
        inst_table[i]->opcode = opcode;
        inst_table[i]->numoperand = numoperand;

        printf("%s\t%d\t%x\t%d\n", inst_table[i]->mnemonic_instruction, inst_table[i]->instruction_format, inst_table[i]->opcode, inst_table[i]->numoperand);
        inst_index++;
    }
    return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : ������� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�.
 * �Ű� : ��������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0
 * ���� : ���δ����� �����Ѵ�.
 *
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{
    printf("=================input_data==================\n");
    printf("LENGTH\tLINE\tLABEL\tOPERATOR\n\n");

    FILE *file;
    int errno;

    file = fopen(input_file, "r");
    if (file == NULL)
    {
        return -1;
    }
    char oneline[1000];

    while (fgets(oneline, 1000, file) != NULL)
    {
        input_data[line_num] = (char *)malloc(sizeof(char) * (strlen(oneline)));
        /*input_data�� ����*/
        strcpy(input_data[line_num], oneline);
        printf("%d\t%s", line_num + 1, input_data[line_num]);

        line_num++;
    }

    printf("\n�ҽ��ڵ� �� ���� ��: %d\n", line_num);
    fclose(file);
    return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�.
 *        �н� 1�� ���� ȣ��ȴ�.
 * �Ű� : �Ľ��� ���ϴ� ���ڿ�
 * ��ȯ : �������� = 0 , ���� < 0
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�.
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
    printf("[%04X]token_parsing:%s", locctr, str);
    token_table[token_line] = (token *)malloc(sizeof(token));
    token_table[token_line]->label = NULL;
    token_table[token_line]->operator= NULL;
    token_table[token_line]->operand[0] = NULL;
    token_table[token_line]->operand[1] = NULL;
    token_table[token_line]->operand[2] = NULL;
    token_table[token_line]->comment = NULL;
    token_table[token_line]->nixbpe = 0;

    char *token = strtok(str, "\t\n");
    int opcodeline;
    /*opcode+START/END/RESW/RESB/BYTE/WORD/LTORG/EXTDEF/EXTREF/CSECT/EQU/ORG �̾��� ���*/
    if (((opcodeline = search_operator(token)) >= 0 && opcodeline < 59) || strcmp(token, "EXTDEF") == 0 || strcmp(token, "EXTREF") == 0 || strcmp(token, "CSECT") == 0 || strcmp(token, "EQU") == 0 || strcmp(token, "ORG") == 0 || strcmp(token, "LTORG") == 0 || strcmp(token, "START") == 0 || strcmp(token, "END") == 0 || strcmp(token, "RESW") == 0 || strcmp(token, "RESB") == 0 || strcmp(token, "BYTE") == 0 || strcmp(token, "WORD") == 0)
    { // opcode�� ���
        token_table[token_line]->label = NULL;
        token_table[token_line]->operator= token;
        if (opcodeline >= 0 && opcodeline < 59)
        {
        }
    }
    /*���̾��� ���*/
    else
    {
        /* LABEL����*/
        token_table[token_line]->label = token; /////////////////////////////////���⼭ ���� ����� ���� CLOOP�� ...//////////////////////////////////////
        /*OPERATOR ����*/
        token = strtok(NULL, "\t\n");
        if (token != NULL)
        {
            token_table[token_line]->operator= token;
            /*opcode�� ��� {operand ������ ���� ","�� �������� ���� operand�� �����ؾ� ��.}*/
        }
    }
    if (token != NULL)
    {
        if ((opcodeline = search_operator(token)) < 59 && opcodeline >= 0)
        {
            /*���ɾ ������ �ִ� operand ������ ���� operand ����*/
            int i = 0;
            for (i = 0; i < inst_table[opcodeline]->numoperand; i++)
            {
                token = strtok(NULL, ",\t\n");
                token_table[token_line]->operand[i] = token;
            }
            /*����, �ѹ��� �ڸ� token��*/
            token = strtok(NULL, "\t\n");
            if (token != NULL)
            {
                /*x�̸�,*/
                if (!strcmp(token, "X"))
                {
                    token_table[token_line]->operand[i] = token;
                    token_table[token_line]->nixbpe += 8; // x=1
                    /*comment �̸�,*/
                    if (token != NULL)
                    {
                        token = strtok(NULL, "\t\n");
                        token_table[token_line]->comment = token;
                    }
                }
                else
                {
                    /*comment�̸�,*/
                    token_table[token_line]->comment = token;
                }
            }
        }
        else
        {

            /*�� ���� ��� {START / END / RESW / RESB / BYTE / WORD / LTORG / EXTDEF / EXTREF / CSECT / EQU / ORG}*/
            if (strcmp(token, "EXTDEF") == 0 || strcmp(token, "EXTREF") == 0)
            {
                int i = 0;
                while (token != NULL)
                {
                    token = strtok(NULL, ",\n");
                    if (token == NULL)
                    {
                        break;
                    }
                    token_table[token_line]->operand[i] = token;
                    i++;
                }
            }
            else
            {
                token = strtok(NULL, "\t\n");
                if (token != NULL)
                {
                    token_table[token_line]->operand[0] = token;
                }
                token = strtok(NULL, "\t\n");
                if (token != NULL)
                {
                    token_table[token_line]->comment = token;
                }
            }
        }
    }
    addnixbpe(token_line);

    return errno;
}
void addnixbpe(int line)
{
    int opcodeline;
    if (token_table[line]->operator!= NULL)
    {
        opcodeline = search_operator(token_table[line]->operator);
        if (opcodeline >= 0 && opcodeline < 59)
        {
            if (inst_table[opcodeline]->instruction_format == 3 && isformat4(token_table[line]->operator))
            {
                token_table[line]->nixbpe += 0b000001; /*e*/
            }
            if (token_table[line]->operand[0] != NULL)
            {
                if (strstr(token_table[line]->operand[0], "#") != NULL)
                {
                    token_table[line]->nixbpe += 0b010000;
                } /*i*/
                if (strstr(token_table[line]->operand[0], "@") != NULL)
                {
                    token_table[line]->nixbpe += 0b100000;
                } /*n*/
            }
            if (token_table[line]->operand[0] == NULL || isimmediate(token_table[line]->operand[0]) != 1 && strstr(token_table[line]->operand[0], "@") == NULL && inst_table[opcodeline]->instruction_format != 1 && inst_table[opcodeline]->instruction_format != 2)
            {
                token_table[line]->nixbpe += 0b110000; /*ni*/
            }
            if (inst_table[search_operator(token_table[line]->operator)]->instruction_format == 3 && isformat4(token_table[line]->operator) == 0)
            { /*3����*/
                if (token_table[line]->operand[0] != NULL && isimmediate(token_table[line]->operand[0]) != 1)
                {
                    token_table[line]->nixbpe += 0b000010;
                }
            }
        }
    }
}

/* ----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�.
 * �Ű� : ��ū ������ ���е� ���ڿ�
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0
 * ���� :
 *
 * ----------------------------------------------------------------------------------
 */
int isformat4(char *str)
{
    if (str != NULL)
    {
        if (strstr(str, "+") != NULL)
        {
            return 1;
        } // true
        else
        {
            return 0;
        } // false
    }
    return -1;
}

int search_operator(char *str)
{
    /* add your code here +JSUB�� ���� +�� ������!*/
    int inst_tableline = 0;

    char temp[5];
    if (isformat4(str) == 1)
    { /*+�� �پ������� format 4*/
        inst_table[inst_tableline]->instruction_format = 4;
        /*+���� ���ڿ� ���� ��*/
        for (int i = 1; i < (int)strlen(str); i++)
        {
            temp[i - 1] = str[i];
            if (i == strlen(str) - 1)
            {
                temp[i] = '\0';
            }
        }
        while (inst_tableline < inst_index)
        {
            if (!strcmp(temp, inst_table[inst_tableline]->mnemonic_instruction))
            {
                break;
            }
            inst_tableline++;
        }
    }
    else
    {
        while (inst_tableline < inst_index)
        {
            if (!strcmp(str, inst_table[inst_tableline]->mnemonic_instruction))
            {
                break;
            }
            inst_tableline++;
        }
    }

    return inst_tableline;
}

/* ----------------------------------------------------------------------------------
 * ���� : ������� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
 *		   �н�1������..
 *		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
 *		   ���̺��� �����Ѵ�.
 *
 * �Ű� : ����
 * ��ȯ : ���� ���� = 0 , ���� = < 0
 * ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
 *	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
 *
 * -----------------------------------------------------------------------------------
 */

static int assem_pass1(void)
{
    for (int i = 0; i < line_num; i++)
    {
        token_line = i;
        token_parsing(input_data[i]);
        initializeLOCCTR(i); /*  START/CSECT�� ��� �ʱ�ȭ& �� ������ ������ locctr���� '�� ���α׷��� ����' �̹Ƿ� ����.*/
        make_symtab(i);      /*symbol���� �˻��ؼ� symtab�� ����*/
        docalculate(i);
        addlocctr(i);
        make_littab(i);

        printf("\n\n");
    }
    symtab_line = current_symtab_line;
    literal_line = current_littab_line;
    control_line = current_control_line;
    current_symtab_line = 0;
    current_littab_line = 0;
    current_control_line = 0;
    return 0;
}
void make_littab(int i)
{
    if (token_table[i]->operand[0] != NULL && isliteral(token_table[i]->operand[0]) == 1)
    {
        char literal[20];
        strcpy(literal, token_table[i]->operand[0]);
        char *token = strtok(literal, "'="); /*X*/
        char XORC[5];
        strcpy(XORC, token);

        token = strtok(NULL, "'"); /*05*/
        if (isliteralexist(token) == 0)
        {
            strcpy(literal_table[current_littab_line].literal, token);
            sizeliteral(i, XORC, token);

            current_littab_line++;
        }
    }
}
void sizeliteral(int i, char *XORC, char *token)
{

    if (strcmp(XORC, "X") == 0)
    {
        if (strlen(token) % 2 == 0)
        {
            literalsize = literalsize + strlen(token) / 2;
        }
        else
        {
            literalsize = literalsize + strlen(token) / 2 + 1;
        }
    }
    else if (strcmp(XORC, "C") == 0)
    {
        literalsize = literalsize + strlen(token);
    }
}
void addlocctr(int i)
{
    if (token_table[i]->operator!= NULL)
    {
        int opcodeline = search_operator(token_table[i]->operator);

        /*opcode�� �ִ� ���ɾ��ΰ��*/
        if (opcodeline < 59 && opcodeline >= 0)
        {
            if (inst_table[opcodeline]->instruction_format == 2)
            {
                locctr += 2;
            }
            else if (inst_table[opcodeline]->instruction_format == 1)
            {
                locctr += 1;
            }
            else if (inst_table[opcodeline]->instruction_format == 3)
            {
                if (isformat4(token_table[i]->operator) == 1)
                {
                    locctr += 4;
                }
                else
                {
                    locctr += 3;
                }
            }
            else
            {
            }
        }
        /*LTORG�ΰ�� LTORGline<= <currrent_literal_line, */
        else if (strcmp(token_table[i]->operator, "LTORG") == 0)
        {
            while (1)
            {
                if (LTORGline >= current_littab_line)
                {
                    break;
                }

                literal_table[LTORGline].addr = locctr;
                LTORGline++;
            }
            locctr = locctr + literalsize;
            literalsize = 0;
        }
        else if (strcmp(token_table[i]->operator, "CSECT") == 0)
        {
            control_table[current_control_line].startaddr = locctr;
        }
        /*(ORG) {locctr�� Ư�������� �ʱ�ȭ}*/
        else if (strcmp(token_table[i]->operator, "ORG") == 0)
        {
            locctr = 0;
        }
        /*(BYTE / WORD / RESB / RESW) {�ڿ� �ִ� operandȮ���ؼ� �׸�ŭ locctr�� +�ؾ��Ѵ�}*/
        else if (strcmp(token_table[i]->operator, "BYTE") == 0)
        {
            char operand[20];
            strcpy(operand, token_table[i]->operand[0]);

            char *token = strtok(operand, "'");

            if (strcmp(token, "X") == 0)
            {
                token = strtok(NULL, "'");
                if (strlen(token) % 2 == 0)
                {
                    locctr = locctr + strlen(token) / 2;
                }
                else
                {
                    locctr = locctr + strlen(token) / 2 + 1;
                }
            }
            else if (strcmp(token, "C") == 0)
            {
                token = strtok(NULL, "'");
                locctr = locctr + strlen(token);
            }
        }
        else if (strcmp(token_table[i]->operator, "WORD") == 0)
        {
            locctr += 3;
        }
        else if (strcmp(token_table[i]->operator, "RESB") == 0)
        {
            locctr = locctr + atoi(token_table[i]->operand[0]);
        }
        else if (strcmp(token_table[i]->operator, "RESW") == 0)
        {
            locctr = locctr + atoi(token_table[i]->operand[0]) * 3;
        }
        /*(EXTDEF/EXTREF) {���� ���α׷� CONTORL SECTION�� �ִ��� ���ο� ����
                       �ּҸ� ���� ���� ���ǰų� , �ƴѰ� �����Ǳ⿡ ��� ���� �־�������� ������}*/
        else if (strcmp(token_table[i]->operator, "EXTDEF") == 0)
        {
            for (int k = 0; k < 3; k++)
            {
                if (token_table[i]->operand[k] == NULL)
                {
                    break;
                }
                control_table[current_control_line].EXTDEF[k] = token_table[i]->operand[k];
            }
        }
        else if (strcmp(token_table[i]->operator, "EXTREF") == 0)
        {
            for (int k = 0; k < 5; k++)
            {
                if (token_table[i]->operand[k] == NULL)
                {
                    break;
                }
                control_table[current_control_line].EXTREF[k] = token_table[i]->operand[k];
            }
        }
        /*(EQU)         {�ƹ��͵� �ϸ� �ȵ�.}*/
        else if (strcmp(token_table[i]->operator, "EQU") == 0)
        {
        }
        else if (strcmp(token_table[i]->operator, "END") == 0)
        {
            while (1)
            {
                if (LTORGline >= current_littab_line)
                {
                    break;
                }
                literal_table[LTORGline].addr = locctr;
                LTORGline++;
            }
            locctr = locctr + literalsize;
            literalsize = 0;

            control_table[current_control_line].length = locctr;
        }
    }
}
/*  1. START/CSECT�� ��� �ʱ�ȭ
    2. �� ������ ������ locctr���� '�� ���α׷��� ����' �̹Ƿ� ����
    �Ű�: ���� token_table�� ���ι�ȣ
*/
void initializeLOCCTR(int i)
{
    if (token_table[i]->label != NULL && token_table[i]->operator!= NULL)
    {
        if (strcmp(token_table[i]->operator, "START") == 0)
        {
            locctr = atoi(token_table[0]->operand[0]);
            strcpy(control_table[current_control_line].name, token_table[i]->label);
        }
        else if (strcmp(token_table[i]->operator, "CSECT") == 0)
        {

            control_table[current_control_line].length = locctr;

            current_control_line++;
            locctr = 0;
            strcpy(control_table[current_control_line].name, token_table[i]->label);
        }
    }
}

void docalculate(int i)
{

    if (token_table[i]->operand[0] != NULL && strstr(token_table[i]->operand[0], "-") != NULL)
    {
        char *token;
        char str[30];
        char tok1[15];
        char tok2[15];
        strcpy(str, token_table[i]->operand[0]);
        token = strtok(str, "-");
        strcpy(tok1, token);
        token = strtok(NULL, "-\n");
        strcpy(tok2, token);
        sym_table[find_symbol(token_table[i]->label)].addr = find_symbol_addr(tok1) - find_symbol_addr(tok2);

        /*���� control section�� �ִٸ�,*/
        int indiffcontrol = 0;
        indiffcontrol += isinsamecontrol(tok1);
        indiffcontrol += isinsamecontrol(tok2);
        if (indiffcontrol == 0)
        {
            sprintf(token_table[i]->operand[0], "%X", sym_table[find_symbol(token_table[i]->label)].addr);
        }
    }
}

int isinsamecontrol(char *tok)
{
    int indiffcontrol = 0;

    for (int j = 0; j < 3; j++)
    {
        if (control_table[current_control_line].EXTREF[j] != NULL)
        {
            if (strcmp(control_table[current_control_line].EXTREF[j], tok) == 0)
            {
                indiffcontrol++;
            }
        }
    }

    return indiffcontrol;
}

int find_symbol(char *str)
{
    int symbol_line_addr = -1;
    int line = current_symtab_line;
    if (current_symtab_line < symtab_line)
    {
        line = symtab_line;
    }

    for (int i = 0; i <= line; i++)
    {
        if (strcmp(sym_table[i].symbol, str) == 0)
        {
            symbol_line_addr = i;
            break;
        }
    }
    return symbol_line_addr;
}

int find_symbol_addr(char *str)
{
    int addr = -1;
    int line = current_symtab_line;
    if (current_symtab_line < symtab_line)
    {
        line = symtab_line;
    }
    for (int i = 0; i < line; i++)
    {
        if (strcmp(sym_table[i].symbol, str) == 0)
        {
            addr = sym_table[i].addr;
        }
    }
    return addr;
}

int find_literal(char *str)
{
    int literal_line_addr = -1;
    int line = current_littab_line;
    if (current_littab_line < literal_line)
    {
        line = literal_line;
    }
    for (int i = 0; i < line; i++)
    {
        if (strcmp(literal_table[i].literal, str) == 0)
        {
            literal_line_addr = i;
            break;
        }
    }
    return literal_line_addr;
}

void make_symtab(int i)
{
    if (token_table[i]->label != NULL && strcmp(token_table[i]->label, ".") != 0)
    {
        strcpy(sym_table[current_symtab_line].symbol, token_table[i]->label);
        sym_table[current_symtab_line].addr = locctr;
        current_symtab_line++;
    }
}
/* ----------------------------------------------------------------------------------
 * ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
 *        ���⼭ ��µǴ� ������ ���ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 5��) �̴�.
 * �Ű� : ������ ������Ʈ ���ϸ�
 * ��ȯ : ����
 * ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
 *        ȭ�鿡 ������ش�.
 *        ���� ���� 4�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
 * -----------------------------------------------------------------------------------
 */
// void make_opcode_output(char *file_name)
// {
// 	/* add your code here */

// }

/* ----------------------------------------------------------------------------------
 * ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
 *        ���⼭ ��µǴ� ������ SYMBOL�� �ּҰ��� ����� TABLE�̴�.
 * �Ű� : ������ ������Ʈ ���ϸ�
 * ��ȯ : ����
 * ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
 *        ȭ�鿡 ������ش�.
 *
 * -----------------------------------------------------------------------------------
 */
void make_symtab_output(char *file_name)
{
    printf("\n--------------------SYMTAB----------------------\n");
    FILE *file;
    file = fopen(file_name, "wt");
    for (int i = 0; i < symtab_line; i++)
    {
        fprintf(file, "%s\t%04X\n", sym_table[i].symbol, sym_table[i].addr);
        printf("[%d]symbol: %s locctr:%04X\n", i, sym_table[i].symbol, sym_table[i].addr);
    }
}

/* ----------------------------------------------------------------------------------
 * ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
 *        ���⼭ ��µǴ� ������ LITERAL�� �ּҰ��� ����� TABLE�̴�.
 * �Ű� : ������ ������Ʈ ���ϸ�
 * ��ȯ : ����
 * ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
 *        ȭ�鿡 ������ش�.
 *
 * -----------------------------------------------------------------------------------
 */
void make_literaltab_output(char *file_name)
{
    printf("\n--------------------LITTAB----------------------\n");

    FILE *file;
    file = fopen(file_name, "w");
    for (int i = 0; i < literal_line; i++)
    {
        fprintf(file, "%s\t%04X\n", literal_table[i].literal, literal_table[i].addr);
        printf("[%d]literal: %s locctr:%04X\n", i, literal_table[i].literal, literal_table[i].addr);
    }
}
int isliteral(char *str)
{
    if (str != NULL)
    {
        if (strstr(str, "=") != NULL)
        {
            return 1;
        } // true
        else
        {
            return 0;
        } // false
    }
    return 0;
}
int isliteralexist(char *str)
{
    int i;
    for (i = 0; i < current_littab_line; i++)
    {
        if (strcmp(str, literal_table[i].literal) == 0)
        {
            break;
        }
    }
    if (i >= current_littab_line)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/* ----------------------------------------------------------------------------------
 * ���� : ������� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
 *		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
 *		   ������ ���� �۾��� ����Ǿ� ����.
 *		   1. ������ �ش� ������� ���ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
 * �Ű� : ����
 * ��ȯ : �������� = 0, �����߻� = < 0
 * ���� :
 * -----------------------------------------------------------------------------------
 */
int search_opcode(char *str)
{
    if (search_operator(str) >= 0 && search_operator(str) < 59)
    {
        return inst_table[search_operator(str)]->opcode;
    }
    return -1;
}

static int assem_pass2(void)
{
    locctr = 0;

    int length = 0; /*�ڵ� ����*/
    int controls = 0;

    for (int i = 0; i <= token_line; i++)
    {
        if (token_table[i]->operator!= NULL)
        {
            divisionoperator(&length, i);
        }

        if (token_table[i]->operator!= NULL && strcmp(token_table[i]->operator, "LTORG") == 0)
        {

            T_table[Tline].len = strlen(LINEBUFFER);
            strcpy(T_table[Tline].T, LINEBUFFER);

            Tline++;
            T_table[Tline].startaddr = locctr;
            T_table[Tline].len = strlen(BUFFER);
            strcpy(T_table[Tline].T, BUFFER); // printf("(%d:%s)", T_table[Tline].len, T_table[Tline].T);
            Tline++;

            length = 0;
            memset(LINEBUFFER, 0, sizeof(LINEBUFFER));
        }
        if (token_table[i]->operator!= NULL &&(strcmp(token_table[i]->operator, "START") == 0 || strcmp(token_table[i]->operator, "CSECT") == 0))
        {

            if (strcmp(token_table[i]->operator, "CSECT") == 0)
            {

                if (length != 0)
                {
                    T_table[Tline].len = length;
                    strcpy(T_table[Tline].T, LINEBUFFER);
                    Tline++;
                }
            }

            length = 0;
            memset(LINEBUFFER, 0, sizeof(LINEBUFFER));
        }
        if (token_table[i]->operator!= NULL &&(strcmp(token_table[i]->operator, "BYTE") == 0 || strcmp(token_table[i]->operator, "WORD") == 0))
        {
            if (length + strlen(BUFFER) > 60)
            {
                T_table[Tline].len = length;
                strcpy(T_table[Tline].T, LINEBUFFER);
                Tline++;
                T_table[Tline].startaddr = locctr;
                length = 0;
                length += strlen(BUFFER);
                memset(LINEBUFFER, 0, sizeof(LINEBUFFER));
                strcat(LINEBUFFER, BUFFER);
            }
            else
            {

                length += strlen(BUFFER);
                strcat(LINEBUFFER, BUFFER);
            }
        }
        if (token_table[i]->operator!= NULL && strcmp(token_table[i]->operator, "END") == 0)
        {

            // T������ �ڸ���
            if (length + strlen(BUFFER) > 60)
            {
                T_table[Tline].len = length;

                strcpy(T_table[Tline].T, LINEBUFFER);
                Tline++;
                T_table[Tline].startaddr = locctr;
                length = 0;
                length += strlen(BUFFER);
                memset(LINEBUFFER, 0, sizeof(LINEBUFFER));
                strcat(LINEBUFFER, BUFFER);
            }
            else
            {
                length += strlen(BUFFER);
                strcat(LINEBUFFER, BUFFER);
                T_table[Tline].len = length;
                strcpy(T_table[Tline].T, LINEBUFFER);
                Tline++;
            }
        }
        if (token_table[i]->operator!= NULL && search_opcode(token_table[i]->operator) > -1 && strcmp(token_table[i]->operator, "START") != 0 && strcmp(token_table[i]->operator, "END") != 0 && strcmp(token_table[i]->operator, "CSECT") != 0 && strcmp(token_table[i]->operator, "EXTREF") != 0 && strcmp(token_table[i]->operator, "EXTDEF") != 0 && strcmp(token_table[i]->operator, "EQU") != 0 && strcmp(token_table[i]->operator, "RESW") != 0 && strcmp(token_table[i]->operator, "RESB") != 0)
        {
            // T������ �ڸ���
            if (length + strlen(BUFFER) > 60)
            {
                T_table[Tline].len = length;

                strcpy(T_table[Tline].T, LINEBUFFER);
                Tline++;
                T_table[Tline].startaddr = locctr;
                length = 0;
                length += strlen(BUFFER);
                memset(LINEBUFFER, 0, sizeof(LINEBUFFER));
                strcat(LINEBUFFER, BUFFER);
            }
            else
            {
                length += strlen(BUFFER);
                strcat(LINEBUFFER, BUFFER);
            }
        }

        if (token_table[i]->operator!= NULL && strcmp(token_table[i]->operator, "CSECT") == 0)
        {
            controls = controls + 1;
        }

        T_table[Tline].controlsection = controls;
        addlocctr(i);
    }
    printf("\n-------------------OBJECT CODE------------------------\n");
    /*���!========================================================================*/
    int d = 0;
    int r = 0;
    for (int h = 0; h < Hline; h++)
    {
        printf("%s", H_table[h].H);
        int t = 0;
        int startaddr = 0;

        if (d < Dline)
        {
            printf("%s", D_table[d].D);
            d++;
        }
        if (r < Rline)
        {
            printf("%s", R_table[r].R);
            r++;
        }

        while (1)
        {
            if (t >= Tline)
            {
                break;
            }
            if (h == T_table[t].controlsection)
            {
                printf("T%06X%02X%s\n", T_table[t].startaddr, T_table[t].len / 2, T_table[t].T);
                startaddr = startaddr + T_table[t].len;
            }
            else
            {
                startaddr = 0;
            }
            t++;
        }

        for (int m = 0; m < Mline; m++)
        {
            if (h == M_table[m].controlsection)
            {
                printf("M%06X%02X%s\n", M_table[m].startbyte, M_table[m].size, M_table[m].M);
            }
        }
        if (h == Hline - 1)
        {
            printf("E\n");
        }
    }
    /*���!========================================================================*/
}
void divisionoperator(int *length, int i)
{ /*length�� object�ڵ�� ��ȯ�Ȱ��� ����*/
    if (token_table[i]->label != NULL && find_symbol(token_table[i]->label) != -1)
    {
        current_symtab_line = find_symbol(token_table[i]->label);
    }

    if (strcmp(token_table[i]->operator, "START") == 0 || strcmp(token_table[i]->operator, "END") == 0)
    {
        assembleSTARTENDdirectives(i);
    }
    else if (search_operator(token_table[i]->operator) >= 0 && search_operator(token_table[i]->operator) < 59)
    {
        assembleinstructions(i); /*(��)*/
    }
    else if (strcmp(token_table[i]->operator, "BYTE") == 0 || strcmp(token_table[i]->operator, "WORD") == 0)
    {
        assembledirectives(i); /*���� �Ҵ��ؼ� 6�ڸ�16������ ��Ÿ������*/
    }
    else if (strcmp(token_table[i]->operator, "EXTDEF") == 0 || strcmp(token_table[i]->operator, "EXTREF") == 0 || strcmp(token_table[i]->operator, "CSECT") == 0)
    {
        assembleLinkDirectives(i);
    }
    else if (strcmp(token_table[i]->operator, "LTORG") == 0 || i == token_line)
    {
        /*LTORG������ ���Դ� literal�� �� �Ҵ��ϸ鼭 ���������*/

        char minibuffer[20];
        memset(BUFFER, 0, sizeof(BUFFER));

        if (literal_table[current_littab_line].addr <= locctr)
        {
            for (int j = 0; j < strlen(literal_table[current_littab_line].literal); j++)
            {
                sprintf(minibuffer, "%X", *(literal_table[current_littab_line].literal + j));
                strcat(BUFFER, minibuffer);
                memset(minibuffer, 0, sizeof(minibuffer));
            }

            current_littab_line++;
        }
    }
    else if (strcmp(token_table[i]->operator, "RESB") == 0)
    {
        memset(BUFFER, 0, sizeof(BUFFER));
    }
    else if (strcmp(token_table[i]->operator, "RESW") == 0)
    {
        memset(BUFFER, 0, sizeof(BUFFER));
    }
    else
    {
        memset(BUFFER, 0, sizeof(BUFFER));
    }
}
void assembledirectives(int i)
{
    memset(BUFFER, 0, sizeof(BUFFER));

    // ����, ���۷����� �ִٸ�, ��� �Ұ� ���߿� �����ؾ���.
    // ����, -�� �ִٸ�, �̰Ŵ� control section�� �ִ��� ���ο� ���� �ٲ�� �ϴµ�~ pass1���� ������� �� ������ ���� ���� �� �ϳ�/ pass1���� ��� �ȵǴ� �� �ϳ� ����
    if (token_table[i]->operand[0] != NULL)
    {
        if (strstr(token_table[i]->operand[0], "-") != NULL)
        { /*������ ���°��*/
            if (strcmp(token_table[i]->operator, "WORD") == 0)
            {
                sprintf(BUFFER, "%06X", 0);
                char str[30];
                char tok1[15];
                char tok2[15];
                strcpy(str, token_table[i]->operand[0]);
                char buf[20];

                char *token = strtok(str, "-");
                strcpy(tok1, token);
                token = strtok(NULL, "\n");
                strcpy(tok2, token);
                memset(buf, 0, sizeof(buf));
                strcat(buf, "+");
                strcat(buf, tok1);
                strcpy(M_table[Mline].M, buf);
                M_table[Mline].size = 6;
                M_table[Mline].controlsection = T_table[Tline].controlsection;
                M_table[Mline].startbyte = T_table[Tline - 1].len / 2 + strlen(LINEBUFFER) / 2;

                Mline++;

                memset(buf, 0, sizeof(buf));
                strcat(buf, "-");
                strcat(buf, tok2);
                strcpy(M_table[Mline].M, buf);
                M_table[Mline].size = 6;
                M_table[Mline].controlsection = T_table[Tline].controlsection;
                M_table[Mline].startbyte = T_table[Tline - 1].len / 2 + strlen(LINEBUFFER) / 2;
                Mline++;
            }
            else
            {
                sprintf(BUFFER, "%02X", 0);
                char str[30];
                char tok1[15];
                char tok2[15];
                strcpy(str, token_table[i]->operand[0]);
                char buf[20];

                char *token = strtok(str, "-");
                strcpy(tok1, token);
                token = strtok(NULL, "\n");
                strcpy(tok2, token);
                memset(buf, 0, sizeof(buf));
                strcat(buf, "+");
                strcat(buf, tok1);
                strcpy(M_table[Mline].M, buf);
                M_table[Mline].startbyte = T_table[Tline - 1].len / 2 + strlen(LINEBUFFER) / 2;
                M_table[Mline].controlsection = T_table[Tline].controlsection;
                Mline++;

                memset(buf, 0, sizeof(buf));
                strcat(buf, "-");
                strcat(buf, tok2);
                strcpy(M_table[Mline].M, buf);
                M_table[Mline].startbyte = T_table[Tline - 1].len / 2 + strlen(LINEBUFFER) / 2;
                M_table[Mline].controlsection = T_table[Tline].controlsection;
                Mline++;
            }
        }
        else
        { /*�ƴ� ���*/
            if (strcmp(token_table[i]->operator, "WORD") == 0)
            {
                sprintf(BUFFER, "%06X", 0);
            }
            else
            {
                char operand[20];
                strcpy(operand, token_table[i]->operand[0]);
                char *token = strtok(operand, "'");
                if (strcmp(token, "X") == 0)
                {
                    token = strtok(NULL, "'");
                    sprintf(BUFFER, "%0*s", strlen(token), token);
                }
                else if (strcmp(token, "C") == 0)
                {
                    token = strtok(NULL, "'");
                }
            }
        }
    }
}
void assembleLinkDirectives(int i)
{
    if (strcmp(token_table[i]->operator, "EXTDEF") == 0)
    {
        memset(BUFFER, 0, sizeof(BUFFER));
        strcat(BUFFER, "D");
        char minibuffer[15];
        for (int j = 0; j < 3; j++)
        {
            if (control_table[current_control_line].EXTDEF[j] != NULL)
            {

                sprintf(minibuffer, "%s%06X", control_table[current_control_line].EXTDEF[j], find_symbol_addr(control_table[current_control_line].EXTDEF[j]));
                strcat(BUFFER, minibuffer);
            }
        }
        strcat(BUFFER, "\n");
        strcpy(D_table[Dline].D, BUFFER);
        Dline++;
    }
    else if (strcmp(token_table[i]->operator, "EXTREF") == 0)
    {
        memset(BUFFER, 0, sizeof(BUFFER));
        strcat(BUFFER, "R");
        char minibuffer[15];
        for (int j = 0; j < 3; j++)
        {
            if (control_table[current_control_line].EXTREF[j] != NULL)
            {
                sprintf(minibuffer, "%s", control_table[current_control_line].EXTREF[j]);
                strcat(BUFFER, minibuffer);
                memset(minibuffer, 0, sizeof(minibuffer));
            }
        }
        strcat(BUFFER, "\n");
        strcpy(R_table[Rline].R, BUFFER);
        Rline++;
    }
    else if (strcmp(token_table[i]->operator, "CSECT") == 0)
    {
        locctr = 0;
        current_control_line++;
        memset(BUFFER, 0, sizeof(BUFFER));

        char TEMPBUFFER[60];
        memset(TEMPBUFFER, 0, sizeof(TEMPBUFFER));

        strcat(BUFFER, "E");
        if (current_control_line == 1)
        {
            char abuf[10];
            sprintf(abuf, "%06X", control_table[0].startaddr);
            strcat(BUFFER, abuf);
        }
        strcat(BUFFER, "\n");

        sprintf(TEMPBUFFER, "\nH%s\t%06X%06X\n", control_table[current_control_line].name, control_table[current_control_line].startaddr, control_table[current_control_line].length);
        strcat(BUFFER, TEMPBUFFER);
        strcpy(H_table[Hline].H, BUFFER);
        Hline++;
    }
}

void assembleSTARTENDdirectives(int i)
{
    if (strcmp(token_table[i]->operator, "START") == 0)
    {
        memset(BUFFER, 0, sizeof(BUFFER));
        sprintf(BUFFER, "H%s\t%06X%06X\n", sym_table[current_symtab_line].symbol, control_table[current_control_line].startaddr, control_table[current_control_line].length);
        strcpy(H_table[Hline].H, BUFFER);
        Hline++;
    }
    else if (strcmp(token_table[i]->operator, "END") == 0)
    {

        memset(BUFFER, 0, sizeof(BUFFER));

        if (literal_table[current_littab_line].addr <= locctr)
        {
            sprintf(BUFFER, "%s", literal_table[current_littab_line].literal);
        }
    }
}

void assembleinstructions(int i)
{
    /*���ɾ�*/
    if (token_table[i]->operator!= NULL && search_opcode(token_table[i]->operator) != -1)
    {
        int opcode = search_opcode(token_table[i]->operator);
        int nixbpe1 = token_table[i]->nixbpe;
        if (nixbpe1 / 32 == 1)
        {
            nixbpe1 = nixbpe1 - 32;
            opcode = opcode + 2;
        }
        if (nixbpe1 / 16 == 1)
        {
            nixbpe1 = nixbpe1 - 16;
            opcode = opcode + 1;
        }

        if (search_operator(token_table[i]->operator) >= 0 && search_operator(token_table[i]->operator) < 59)
        {
            if (inst_table[search_operator(token_table[i]->operator)]->instruction_format == 3 && isformat4(token_table[i]->operator) == 0)
            { /*3����*/
                int numofoperand = inst_table[search_operator(token_table[i]->operator)]->numoperand;
                int dp = 0; /*disp���*/
                calculatedisp(&dp, i);
                memset(BUFFER, 0, sizeof(BUFFER));
                sprintf(BUFFER, "%02X%01X%03X", opcode, nixbpe1, dp);
            }
            else if (inst_table[search_operator(token_table[i]->operator)]->instruction_format == 3 && isformat4(token_table[i]->operator) == 1)
            { /*4����*/
                /*address���->�Ұ�*/
                memset(BUFFER, 0, sizeof(BUFFER));
                int address = 0;
                sprintf(BUFFER, "%02X%01X%05X", opcode, nixbpe1, address);
                int t = 1;
                int startaddress = strlen(LINEBUFFER) / 2;
                while (1)
                {
                    if (Tline < t || T_table[Tline - t].controlsection != T_table[Tline].controlsection)
                    {
                        break;
                    }
                    startaddress = startaddress + T_table[Tline - t].len / 2;
                    t++;
                }
                M_table[Mline].controlsection = T_table[Tline].controlsection;
                char tempstr[50];
                memset(tempstr, 0, sizeof(tempstr));
                strcat(tempstr, "+");
                strcat(tempstr, token_table[i]->operand[0]);
                strcpy(M_table[Mline].M, tempstr);
                M_table[Mline].startbyte = startaddress + 1;
                M_table[Mline].size = 5;
                Mline++;
            }
            else if (isformat4(token_table[i]->operator) == 1)
            { /*1����*/
                memset(BUFFER, 0, sizeof(BUFFER));
                sprintf(BUFFER, "%02X%01X", opcode, nixbpe1);
            }
            else if (inst_table[search_operator(token_table[i]->operator)]->instruction_format == 2)
            { /*2����*/
                memset(BUFFER, 0, sizeof(BUFFER));
                sprintf(BUFFER, "%02X", opcode);
                for (int k = 0; k < inst_table[search_operator(token_table[i]->operator)]->numoperand; k++)
                {
                    if (strcmp(token_table[i]->operand[k], "A") == 0)
                    {
                        strcat(BUFFER, "0");
                    }
                    else if (strcmp(token_table[i]->operand[k], "X") == 0)
                    {
                        strcat(BUFFER, "1");
                    }
                    else if (strcmp(token_table[i]->operand[k], "L") == 0)
                    {
                        strcat(BUFFER, "2");
                    }
                    else if (strcmp(token_table[i]->operand[k], "B") == 0)
                    {
                        strcat(BUFFER, "3");
                    }
                    else if (strcmp(token_table[i]->operand[k], "S") == 0)
                    {
                        strcat(BUFFER, "4");
                    }
                    else if (strcmp(token_table[i]->operand[k], "T") == 0)
                    {
                        strcat(BUFFER, "5");
                    }
                    else if (strcmp(token_table[i]->operand[k], "F") == 0)
                    {
                        strcat(BUFFER, "6");
                    }
                    else if (strcmp(token_table[i]->operand[k], "PC") == 0)
                    {
                        strcat(BUFFER, "8");
                    }
                    else if (strcmp(token_table[i]->operand[k], "SW") == 0)
                    {
                        strcat(BUFFER, "9");
                    }
                }
                for (int k = 0; k < 2 - inst_table[search_operator(token_table[i]->operator)]->numoperand; k++)
                {
                    strcat(BUFFER, "0");
                }
            }
        }
    }
}
int findcurrentformat(int i)
{
    int currentformat;
    if (inst_table[search_operator(token_table[i]->operator)]->instruction_format == 3)
    {
        if (isformat4(token_table[i]->operator) == 1)
        {
            currentformat = 4;
        }
        else
        {
            currentformat = 3;
        }
    }
    else if (inst_table[search_operator(token_table[i]->operator)]->instruction_format == 2)
    {
        currentformat = 2;
    }
    else
    {
        currentformat = 1;
    }
    return currentformat;
}
void calculatedisp(int *dp, int i)
{ /*3�����ΰ��1)#    2)=C'EOF'    3)symbol 3-1)���� ��� 3-2)���� ����*/
    if (token_table[i]->operand[0] != NULL)
    {
        if (isimmediate(token_table[i]->operand[0]) == 1)
        { /*1)#*/
            char str[30];
            strcpy(str, token_table[i]->operand[0]);
            char *token = strtok(str, "#");
            int a = atoi(token);
            *dp = a;
        }
        if (isliteral(token_table[i]->operand[0]) == 1)
        { /*2)=C'EOF'*/
            char str[30];
            strcpy(str, token_table[i]->operand[0]);
            char *token = strtok(str, "='");
            token = strtok(NULL, "'");
            int currentformat = findcurrentformat(i);
            *dp = literal_table[find_literal(token)].addr - (locctr + currentformat);
        }
        if (find_symbol(token_table[i]->operand[0]) != -1)
        {
            int currentformat = findcurrentformat(i);
            *dp = find_symbol_addr(token_table[i]->operand[0]) - (locctr + currentformat);
            *dp = *dp & 0x0000FFF;
        }
    }
}

int isimmediate(char *str)
{ /*1: �ٸ� control section�� ����*/
    if (strstr(str, "#") != NULL)
    {
        return 1;
    } /*null�� �ƴ϶�� immediate*/
    else
    {
        return 0;
    }
}
/* ----------------------------------------------------------------------------------
 * ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
 *        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
 * �Ű� : ������ ������Ʈ ���ϸ�
 * ��ȯ : ����
 * ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
 *        ȭ�鿡 ������ش�.
 *
 * -----------------------------------------------------------------------------------
 */
void make_objectcode_output(char *file_name)
{
    FILE *file;
    file = fopen(file_name, "wt");

    int d = 0;
    int r = 0;
    for (int h = 0; h < Hline; h++)
    {
        fprintf(file, "%s", H_table[h].H);
        int t = 0;
        int startaddr = 0;

        if (d < Dline)
        {
            fprintf(file, "%s", D_table[d].D);
            d++;
        }
        if (r < Rline)
        {
            fprintf(file, "%s", R_table[r].R);
            r++;
        }

        while (1)
        {
            if (t >= Tline)
            {
                break;
            }
            if (h == T_table[t].controlsection)
            {
                fprintf(file, "T%06X%02X%s\n", T_table[t].startaddr, T_table[t].len / 2, T_table[t].T);
                startaddr = startaddr + T_table[t].len;
            }
            else
            {
                startaddr = 0;
            }
            t++;
        }

        for (int m = 0; m < Mline; m++)
        {
            if (h == M_table[m].controlsection)
            {
                fprintf(file, "M%06X%02X%s\n", M_table[m].startbyte, M_table[m].size, M_table[m].M);
            }
        }
        if (h == Hline - 1)
        {
            fprintf(file, "E\n");
        }
    }
    fclose(file);
}
