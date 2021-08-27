#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define MAX 100



typedef struct
{
	int stack_item[MAX];
	int top;
	
}STACK;

void initStack(STACK *s);

int isFull(STACK  *s);

int isEmpty(STACK  *s );

int push(STACK *s, int *x );

int pop(STACK *s, int *x );

int peek(STACK *s, int *x );

int operation_reader(STACK* operand_s, STACK* operator_s ,char *op, int lenght);

int operator_or_operand_coding(int x);

char* decimal_converter(int* result, char* operand, int* count);

int operation_exacution(int op1, int op2, int operand);

void showStacks(STACK* operator_s, STACK* operand_s);

void RemoveSpaces(char* source);

int main()
{
	char operation[MAX];
	int result, leng;

	STACK *operand_stack;
	operand_stack = (STACK*)malloc(sizeof(STACK));
	initStack(operand_stack);

	STACK *operator_stack;
	operator_stack = (STACK*)malloc(sizeof(STACK));
	initStack(operator_stack);

	printf("Please enter your desired operation = ");
	scanf("%[^\t\n]s", &operation);

	RemoveSpaces(operation);

	leng = strlen(operation);

	operation[leng] = '%'; //strinin sonunu berlirlemek icin koydum.

	

	result = operation_reader(operand_stack, operator_stack , &operation[0], leng);

	printf("The result is = %d\n", result);


	system("PAUSE");

	return 0;
}
//STACK initialize.
void initStack(STACK *s) {
	s->top = 0;
}

//This function controls if a stack is full or not.
//If it is full , function returns integer value of 1;
//If it is not full , function returns integer value of 0;

int isFull(STACK  *s) {
	if (s->top == MAX)
		return 1;
	else
		return 0;
}

//This function controls if a stack is empty or not.
//If it is empty , function returns integer value of 1;
//If it is empty , function returns integer value of 0;

int isEmpty(STACK  *s) {
	if (s->top == 0)
		return 1;
	else
		return 0;
}

//PUSH
//This function pushes an integer value inside a stack.

int push(STACK *s, int *x) {
		if (isFull(s)==1)
			return 0;
		else {
			s->stack_item[s->top] = *x;
			s->top++;
			return 1;
		}
	
}

//POP
//This function pops an integer value from a stack and returns it using call by reference.

int pop(STACK *s, int *x) {
	if (isEmpty(s)==1)
		return 0;
	else {
		*x = s->stack_item[s->top - 1];
		s->stack_item[s->top - 1] = 0;
		s->top--;
		return 1;
	}
}

//PEEK
//This function doesn't change anything on any stack. 
//Function only reads the top value of a stack and returns it using call by reference.

int peek(STACK *s, int *x) {
	if (isEmpty(s)==1)
		return 0;
	else {
		*x = s->stack_item[s->top - 1];
		return 1;
	}
}

//OPERATION_READER
//Function uses stack functions and does some special operations to find result of given operation.
//This function takes a mathematical operation and returns an integer value which carries result of the given operation.

int operation_reader(STACK* operand_s, STACK* operator_s, char *op , int lenght) {

	int counter = 0, flag = 0, x = 0, converted = 0, peek_control = 0, op1 = 0, op2 = 0, operator_int = 0, result = 0 ,flag2 = 0;
	int tf;

	showStacks(operator_s , operand_s);

	while (lenght != counter)
	{
		
		x = *op - '0'; //First we obtain integer value of *op which type is char.
		flag = operator_or_operand_coding(x); //We send obtained integer value to coding function so we can know if it is operand or operator.
		

		if (flag == 0)	//flag == 0 means our char is a number so its an operand.
		{
			flag2 = 1;
			converted = 0;
			op = decimal_converter(&converted, op, &counter);	//we send our char pointer to find the operands decimal equivalent.
			push(operand_s, &converted );	//we push our operand in to operand stack.
			showStacks(operator_s, operand_s);	

		}
		else {	//if flag != 0 means our char is an operator .

			if (isEmpty(operator_s)) {		//if operator stack is empty we should push our operator in it withour any control.

				tf = push(operator_s, &x);	//pushes the integer value of operator to operator stackç
				counter++;					
				op++;
				showStacks(operator_s, operand_s);

			}
			else {			//if the operator stack is not empty we how some other controls.

				

				tf = peek(operator_s, &peek_control);	//first we should obtain the top element in the operator stack.
				if (operator_or_operand_coding(peek_control) > flag || operator_or_operand_coding(peek_control) == 1) {
					
					if (operator_or_operand_coding(peek_control) == 1 && (x == -3) && flag2 == 0) {
						int temp = 0;
						push(operand_s, &temp);
					}
					flag2 = 0;

					/*I used coding function both for controlling if the char is operand or operator and for operators operation priorities.
					I control if the new coming operator is more prior than the one on top of the stack or if the new coming operator is "(".  */

					tf = push(operator_s, &x);	//we push the operator.
					counter++;
					op++;
					showStacks(operator_s, operand_s);

				}
				else {
					if (flag == 4) {		//if flag == 4 it means that we obtained ')' .
						
						printf(" ')' obtained. Program will exacute all the opertaions between first '(' and last entered ')'. Then it will pop '(' . \n\n ");
						Sleep(2500);
						op++;
						counter++;
						do {
							
							tf = pop(operator_s, &operator_int);	//we obtain operator from stack.
							tf = pop(operand_s, &op2);				//we obtain second operand .
							tf = pop(operand_s, &op1);				//we obtain first operand.
							result = operation_exacution(op1, op2, operator_int);	//we send operation elements to this function so it returns the result.
							printf("%d %c %d = %d\n\n", op1, operator_int + '0', op2, result);	//prints the operation.
							Sleep(2500);
							showStacks(operator_s, operand_s);
							tf = push(operand_s, &result);			//we push the result back into the operand stack.
							tf = peek(operator_s, &operator_int);	//we obtain the new top element of the operator stack for  controls.
							printf("Result = %d will get pushed back in Operand Stack.\n\n", result);
							Sleep(2500);
							showStacks(operator_s, operand_s);

						} while (operator_int != -8);	// -8 is the ASCII equevelant of '('. We will repeat the codes above until we reach  to '('.
						
						tf = pop(operator_s, &operator_int);	//after we reach to '(' we pop it .
						showStacks(operator_s, operand_s);
					}
				    else   //if our new operator is not more prior than the one on the top of the stack and obtained operator is not ')' we do some operations down below.
					{
						do {	
							
							showStacks(operator_s, operand_s);	
							tf = pop(operand_s, &op2);				//we obtain second operand .
							tf = pop(operand_s, &op1);				//we obtain first operand.
							tf = pop(operator_s, &operator_int);	//we obtain operator from stack.
							result = operation_exacution(op1, op2, operator_int); // we send elements for an operaton exacution.
							printf("%d %c %d = %d\n\n", op1, operator_int + '0', op2, result);
							Sleep(2500);
							showStacks(operator_s, operand_s);
							tf = push(operand_s, &result);		//we push the result back in toý operand stack.
							printf("Result = %d will get pushed back in Operand Stack.\n\n", result);
							Sleep(2500);
							showStacks(operator_s, operand_s);
							tf = peek(operator_s, &peek_control);

							//we will continue this operation till we have a more prior operator.

						} while ((operator_or_operand_coding(peek_control) <= flag && operator_or_operand_coding(peek_control) != 1 && isEmpty(operator_s) == 0));
						tf = push(operator_s, &x);
						counter++;
						op++;
					}
				}
			}
		}
		
	}

	while (operator_s->top != 0) {	//here we will do operations until we deplete the operator stack.
		
		tf = pop(operator_s, &operator_int);	//we obtain the operator.
		tf = pop(operand_s, &op2);	//we obtain second operand .
		tf = pop(operand_s, &op1);	//we obtain first operand.
		result = operation_exacution(op1, op2, operator_int);	// we send elements for an operaton exacution.
		printf("%d %c %d = %d", op1, operator_int + '0', op2, result);
		Sleep(1500);
		showStacks(operator_s, operand_s);
		tf = push(operand_s, &result);
		printf("Result = %d will get pushed back in Operand Stack.", result);
		Sleep(1500);
		showStacks(operator_s, operand_s);

	}

	return result;
}

//OPERATOR_OR_OPERAND_CODING
//I use this function for two diffrent jobs.
//First i use it for to decide if the obtained char is an operand or an operator.
//Secondly i use it for giving priority numbers.

int operator_or_operand_coding(int x) {



	switch (x) {

	case -5:
		return 3; //'+' coded as 3 
		break;

	case -3:
		return 3; //'-' coded as 3
		break;

	case -6:
		return 2; //'*' coded as 2
		break;

	case -1:
		return 2; //'/' coded as 2
		break;

	case -8:
		return 1; //'(' coded as 1
		break;

	case -7:
		return 4; // ')' coded as 4
		break;

	case -11:
		return -1;//-11 means converted '%' which means our string ended.
		break;

	default:
		return 0; // 0 means char is a number.
		break;

	}

}

//DECIMAL_CONVERTER
//This function converts and char operand to integer.

char* decimal_converter(int* result, char* str, int* count) {

	int flag = 0, tmp;
	float basamak = 1;
	char* temp = str;
	*result = 0;
	temp++;
	tmp = *temp - '0';
	flag = operator_or_operand_coding(tmp);
	while ((flag == 0)) {
		basamak *= 10;
		temp++;
		tmp = *temp - '0';
		flag = operator_or_operand_coding(tmp);
	}

	while (basamak >= 1) {	//do while duzeltmesi yapip float degiskeni integer yapabilirsin.
		tmp = *str - '0';
		tmp *= basamak;
		*result += tmp;
		basamak /= 10;
		str++;
		(*count)++;
	}

	return str;
}

//OPERATION_ECACUTION
//This function exacutes the mathematical operation if elements given correctly.

int operation_exacution(int op1, int op2, int operand) {

	switch (operand) {

	case -5:
		return (op1 + op2);
		break;

	case -3:
		return (op1 - op2);
		break;

	case -6:
		return (op1*op2);
		break;

	case -1:
		return (op1 / op2);
		break;

	default:
		return 0; 
		break;

	}

}

//SHOWSTACKS
//This function shows strings for a period of time.

void showStacks(STACK* operator_s , STACK* operand_s) {

	int i;
	char converted;

	system("CLS");
	printf("----Operand STACK!----\n\n");


	for (i = (operand_s->top - 1); i >= 0; i--) {

		printf("\t|--------|\n");
		printf("\t| %4d   |\n", operand_s->stack_item[i]);

	}
	printf("\t|________|\n");
	printf("___________________\n");
	printf("----Operator STACK!----\n\n");

	for (i = (operator_s->top - 1); i >= 0; i--) {
		converted = operator_s->stack_item[i] + '0';
		printf("\t|-----|\n");
		printf("\t|  %c  |\n", converted);

	}
	printf("\t|_____|\n\n");
	Sleep(1500);

}
//This function removes the spaces.
void RemoveSpaces(char* source)
{
	char* i = source;
	char* j = source;
	while (*j != 0)
	{
		*i = *j++;
		if (*i != ' ')
			i++;
	}
	*i = 0;
}

