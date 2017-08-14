#include <stdio.h>
#include <stdlib.h>

#include "god.h"

PExpression
create_int_expr(int val)
{
	PExpression p = (PExpression)calloc(1, sizeof(Expression));
	if(NULL == p) {
		perror("calloc failed");

		exit(0);
	}

	p->type = INT_EXPRESSION;
	p->u.int_val = val;

	return p;
}

/*
        创建运算表达式（加、减、乘、除、求余）

        参数一：表达式类型
        参数二：左操作数
        参数三：右操作数
*/
PExpression
create_operation_expression(
        int type,
        PExpression p1,
        PExpression p2)
{
        PExpression p = (PExpression)calloc(1, sizeof(Expression));
        if(NULL == p) {
                perror("calloc failed");

                exit(0);
        }

        p->type = type;
        p->u.binary_expression.left = p1;
        p->u.binary_expression.right = p2;

        return p;
}

void
free_expression(PExpression p, int is_free_self)
{
        if(NULL == p) return;

        free(p->u.binary_expression.left);
        p->u.binary_expression.left = NULL;

        free(p->u.binary_expression.right);
        p->u.binary_expression.right = NULL;

        if(1 == is_free_self) {
                free(p);
                p = NULL;
        }
}

void
update_result_expression(PExpression p, int result)
{
        if(NULL == p) return;

        free_expression(p, 0);

        p->type = INT_EXPRESSION;
        p->u.int_val = result;
}

void
print_expr_tree(PExpression p)
{
	if(NULL == p) {
		return;
	}

	switch(p->type) {
		case INT_EXPRESSION:
			printf("%d", p->u.int_val);
			break;
		case ADD_EXPRESSION:
			printf("(");
			print_expr_tree(p->u.binary_expression.left);
			printf(" + ");
			print_expr_tree(p->u.binary_expression.right);
			printf(")");

			break;
		case SUB_EXPRESSION:
			printf("(");
			print_expr_tree(p->u.binary_expression.left);
			printf(" - ");
			print_expr_tree(p->u.binary_expression.right);
			printf(")");

			break;
		case MUL_EXPRESSION:
			printf("(");
			print_expr_tree(p->u.binary_expression.left);			
			printf(" * ");
			print_expr_tree(p->u.binary_expression.right);			
			printf(")");

			break;
		case DIV_EXPRESSION:
			printf("(");
			print_expr_tree(p->u.binary_expression.left);			
			printf(" / ");
			print_expr_tree(p->u.binary_expression.right);			
			printf(")");

			break;
		case MOD_EXPRESSION:
			printf("(");
			print_expr_tree(p->u.binary_expression.left);			
			printf(" 求余 ");
			print_expr_tree(p->u.binary_expression.right);			
			printf(")");

			break;
		default:
			break;
	}
}

static
int
run_operation_expression(PExpression p)
{
	if(NULL == p) return 0;

        int result = 0;

        int v1 = p->u.binary_expression.left->u.int_val;
        int v2 = p->u.binary_expression.right->u.int_val;

	switch(p->type) {
		case ADD_EXPRESSION:
			result = v1 + v2;
			break;
		case SUB_EXPRESSION:
			result = v1 - v2;
			break;
		case MUL_EXPRESSION:
			result = v1 * v2;
			break;
		case DIV_EXPRESSION:
			result = v1 / v2;
			break;
		case MOD_EXPRESSION:
			result = v1 % v2;
			break;
		default:
			break;
	}

        update_result_expression(p, result);

        return result;
}

void
run_expr_tree(PExpression p, int *return_val)
{
	if(NULL == p) {
                return;
        }

        switch(p->type) {
                case ADD_EXPRESSION:
                        run_expr_tree(p->u.binary_expression.left, return_val);
                        run_expr_tree(p->u.binary_expression.right, return_val);
			*return_val = run_operation_expression(p);

                        break;
                case SUB_EXPRESSION:
                        run_expr_tree(p->u.binary_expression.left, return_val);
                        run_expr_tree(p->u.binary_expression.right, return_val);
			*return_val = run_operation_expression(p);

                        break;
                case MUL_EXPRESSION:
                        run_expr_tree(p->u.binary_expression.left, return_val);
                        run_expr_tree(p->u.binary_expression.right, return_val);
			*return_val = run_operation_expression(p);

                        break;
                case DIV_EXPRESSION:
                        run_expr_tree(p->u.binary_expression.left, return_val);
                        run_expr_tree(p->u.binary_expression.right, return_val);
			*return_val = run_operation_expression(p);

                        break;
                case MOD_EXPRESSION:
                        run_expr_tree(p->u.binary_expression.left, return_val);
                        run_expr_tree(p->u.binary_expression.right, return_val);
			*return_val = run_operation_expression(p);

                        break;
                default:
                        break;
        }
}
