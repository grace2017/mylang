#ifndef __TOKEN_H_
#define __TOKEN_H_

#define BUFF_MAX_SIZE 0x100

typedef enum 
{
	T_INT,
	T_ADD,
	T_SUB,
	T_MUL,
	T_DIV,
	T_MOD,
	T_LINE_END,	//行结束token
	T_UNKNOWN,	//未知token
	T_B_ADD_ADD,	//前++
	T_A_ADD_ADD,	//后++
	T_B_SUB_SUB,	//前--
	T_A_SUB_SUB,	//后--
	T_LP,
	T_RP
}TokenType;

typedef struct
{
	TokenType	type;
	int		int_val;
	char		str[BUFF_MAX_SIZE];
}Token, *PToken;

#endif
