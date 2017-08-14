#include <stdio.h>
#include <stdlib.h>
#include "token.h"

static Token s_token;
static int exist_token;

static char *tokenTypeStr[] = {
	"整数",
	"加",
	"减",
	"乘",
	"除",
	"求余",
	"行结束",
	"未知",
	"前加加",
	"后加加",
	"前减减",
	"后减减",
	"左小括号",
	"右小括号"
};

static int parse_expr();

static
void
get_token(PToken p)
{
	if(1 == exist_token) {
		*p = s_token;
		
		exist_token = 0;
	} else {
		CI_get_token(p);
	}
}

/*
	将预读的token保存起来，以供后续使用
*/
static
void
unget_token(PToken p)
{
	s_token = *p;
	exist_token = 1;
}

static
int
parse_primary_expr()
{
	Token token = {0};
	int result = 0;

	get_token(&token);
	if(T_LINE_END == token.type) {
		printf("运行结束 \n");

		exit(1);
	} else if(T_UNKNOWN == token.type) {
		printf("未知标记：%s \n", token.str);

		exit(1);
	} else if(T_LP == token.type) {
		result = parse_expr();		

		get_token(&token);
		if(T_RP != token.type) {
			printf("缺少 ) \n");

			exit(0);
		}
	} else {
		result = token.int_val;
	}

	return result;
}

static
int 
parse_twice_expr()
{
	Token token = {0};
        int v1 = 0;
        int v2 = 0;

        v1 = parse_primary_expr();
        while(1) {
                get_token(&token);
                if(T_MUL == token.type) {
                        v2 = parse_primary_expr();

                        v1 *= v2;
                } else if(T_DIV == token.type) {
			v2 = parse_primary_expr();

                        v1 /= v2;
		} else if(T_MOD == token.type) {
			v2 = parse_primary_expr();

                        v1 %= v2;
		} else {
			unget_token(&token);

                        break;
                }
        } /* end while */

        return v1;
}

static
int
parse_expr()
{
	Token token = {0};
	int v1 = 0;
	int v2 = 0;

	v1 = parse_twice_expr();
	while(1) {
		get_token(&token);
	        if(T_ADD == token.type) {
        	        v2 = parse_twice_expr();

			v1 += v2;
        	} else if(T_SUB == token.type) {
                	v2 = parse_twice_expr();

                	v1 -= v2;
		} else {
			unget_token(&token);

			break;
		}	
	} /* end while */

	return v1;
}

/*
	一次性解析一行中的所有token
*/
static
void
parse_line()
{
	Token token = {0};
	while(1) {
		CI_get_token(&token);
		if(T_LINE_END == token.type) {
			printf("解析结束 \n");

			exit(1);	
		} else {
			printf("%s %s \n", tokenTypeStr[token.type], token.str);	
		}
	}
}

void
YU_parse_line(char *buff)
{
	int result = 0;

	CI_set_line(buff);	

	//parse_line();

	result = parse_expr();
	printf("结果：%d \n", result);
}
