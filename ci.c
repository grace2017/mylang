#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

static char *s_line;
static int s_line_pos;

void
CI_set_line(char *buff)
{
	s_line = buff;
	s_line_pos = 0;
}

void
CI_get_token(PToken token_p)
{
	char ch = '0';
	int line_pos = 0;

	while('\0' != s_line[s_line_pos]) {
		ch = s_line[s_line_pos];
		s_line_pos++;

		if(line_pos > BUFF_MAX_SIZE) {
			printf("%s(%d): 预读字符超过缓冲区大小 \n", __FILE__, __LINE__);

			exit(0);
		}

		if(isspace(ch)) {
			token_p->int_val = 0;
			bzero(token_p->str, BUFF_MAX_SIZE);

			//命令行直接回车，从这里结束
			if('\n' == ch) {
				token_p->type = T_LINE_END;

				return;
			}
		} else {
			token_p->str[line_pos] = ch;

			line_pos++;

			token_p->str[line_pos] = '\0';
		}
		
		if(isdigit(ch)) {
			/*
				预读 看下个字符是不是数字
					是 return
					否 continue
			*/
			ch = s_line[s_line_pos];
			if(!isdigit(ch)) {
				token_p->type = T_INT;
				sscanf(token_p->str, "%d", &token_p->int_val);
				
				return;
			} else {
				continue;
			}
		}

		if('+' == ch) {
			/*
				判断是不是++
			*/
			ch = s_line[s_line_pos];
			if('+' == ch) {
				token_p->str[line_pos] = ch;
				line_pos++;
				token_p->str[line_pos] = '\0';

				s_line_pos++;

				//判断是前++还是后++
				if(!isdigit(s_line[s_line_pos])) {
					token_p->type = T_A_ADD_ADD;
				} else {
					token_p->type = T_B_ADD_ADD;
				}
	
				return;
			} else {
				token_p->type = T_ADD;

				return;
			}
		} else if('-' == ch) {
			/*
				判断是不是--
			*/
			ch = s_line[s_line_pos];
			if('-' == ch) {
				token_p->str[line_pos] = ch;
				line_pos++;
				token_p->str[line_pos] = '\0';

				s_line_pos++;

				//判断是前--还是后--
				if(!isdigit(s_line[s_line_pos])) {
					token_p->type = T_A_SUB_SUB;
				} else {
					token_p->type = T_B_SUB_SUB;
				}
				
				return;
			} else {
				token_p->type = T_SUB;

				return;
			}
		} else if('*' == ch) {
			token_p->type = T_MUL;
			return;
		} else if('/' == ch) {
			token_p->type = T_DIV;
			return;
		} else if('%' == ch) {
			token_p->type = T_MOD;
			return;
		}

		if('(' == ch) {
			token_p->type = T_LP;
			return;	
		} else if(')' == ch) {
			token_p->type = T_RP;
			return;
		}

		token_p->int_val = 0;
		token_p->type = T_UNKNOWN;
		return;
	} /* end while */
} /* end CI_get_token */
