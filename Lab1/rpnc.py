#
# Python3 Reverse Polish Notation (RPN) Calculator
#
# Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
#

import sys

# Arithmetic Functions
def addition(oper1, oper2):
	return oper1+oper2
	
def subtraction(oper1, oper2):
	return oper1-oper2
	
def multiplication(oper1, oper2):
	return oper1*oper2
	
def division(oper1, oper2):
	return oper1/oper2
	
def power(oper1, oper2):
	return oper1**oper2
	
# Main Function / Code Start <----------	
stack = []
tokenList = sys.argv[1].split()
for token in tokenList:
	# Operand Found
	if token.isdigit():
		stack.append(token)
	else:
		# Operator Found
		oper2 = int(stack.pop())
		oper1 = int(stack.pop())
		if token == '+':
			result = addition(oper1, oper2)
			stack.append(result)
		elif token == '-':
			result = subtraction(oper1, oper2)
			stack.append(result)
		elif token == '*':
			result = multiplication(oper1, oper2)
			stack.append(result)
		elif token == '/':
			result = division(oper1, oper2)
			stack.append(result)
		elif token == '^':
			result = power(oper1, oper2)
			stack.append(result)
result = stack.pop()
print(result)