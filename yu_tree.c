#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "mygod.h"

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

static PExpression parse_expr();

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
	if(NULL != p) {
		s_token = *p;
	}	

	exist_token = 1;
}

static
PExpression
parse_primary_expr()
{
	Token token = {0};
	PExpression expr_p = NULL;

	get_token(&token);
	if(T_LINE_END == token.type) {
		printf("运行结束 \n");

		exit(1);
	} else if(T_UNKNOWN == token.type) {
		printf("未知标记：%s \n", token.str);

		exit(1);
	} else if(T_LP == token.type) {
		expr_p = parse_expr();		

		get_token(&token);
		if(T_RP != token.type) {
			printf("缺少 ) \n");

			exit(0);
		}
	} else {
		expr_p = create_int_expr(token.int_val);
	}

	return expr_p;
}

static
PExpression 
parse_twice_expr()
{
	Token token = {0};
        PExpression expr_p1 = NULL;
        PExpression expr_p2 = 0;

        expr_p1 = parse_primary_expr();
        while(1) {
                get_token(&token);
                if(T_MUL == token.type) {
                        expr_p2 = parse_primary_expr();

			expr_p1 = create_operation_expression(T_MUL, expr_p1, expr_p2);
                } else if(T_DIV == token.type) {
                        expr_p2 = parse_primary_expr();

			expr_p1 = create_operation_expression(T_DIV, expr_p1, expr_p2);
		} else if(T_MOD == token.type) {
                        expr_p2 = parse_primary_expr();

			expr_p1 = create_operation_expression(T_MOD, expr_p1, expr_p2);
		} else {
			unget_token(&token);

                        break;
                }
        } /* end while */

        return expr_p1;
}

static
PExpression
parse_expr()
{
	Token token = {0};
	PExpression expr_p1 = 0;
	PExpression expr_p2 = 0;

	expr_p1 = parse_twice_expr();
	while(1) {
		get_token(&token);
	        if(T_ADD == token.type) {
        	        expr_p2 = parse_twice_expr();

			expr_p1 = create_operation_expression(T_ADD, expr_p1, expr_p2);
        	} else if(T_SUB == token.type) {
        	        expr_p2 = parse_twice_expr();

			expr_p1 = create_operation_expression(T_SUB, expr_p1, expr_p2);
		} else {
			unget_token(&token);

			break;
		}	
	} /* end while */

	return expr_p1;
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
	PExpression expr_p = NULL;
	int result = 0;

	CI_set_line(buff);	

	//防止上次解析的预读对后面的干扰
	exist_token = 0;

#ifdef __DEBUG
	parse_line();
#else
	expr_p = parse_expr();

	print_expr_tree(expr_p);

	run_expr_tree(expr_p, &result);
	
	free(expr_p);

	printf(" = %d \n", result);
#endif
}
