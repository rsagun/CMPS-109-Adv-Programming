#
# Awk Reverse Polish Notation (RPN) Calculator
#
# Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
# 

function add(a, b)
{
	return a+b;
}

function subt(a, b)
{
	return a-b;
}

function mult(a, b)
{
	return a*b;
}

function div(a, b)
{
	return a/b;
}

function expo(a, b)
{
	return a**b
}

BEGIN {
	stackPtr = -1
    expression = ARGV[1]
    split(expression, token, " ")

    for(arg in token)
    {
    	
    	if(token[arg] ~ /^[0-9]*$/)
    	{
    		stack[++stackPtr] = token[arg]
    	}
    	else
    	{
    		a = stack[stackPtr--]
    		b = stack[stackPtr--]
    		if(token[arg] == "+")
			{
				c = add(a, b)
				stack[++stackPtr] = c
			}
			else if(token[arg] == "-")
			{
				c = subt(b, a)
				stack[++stackPtr] = c
			}
			else if(token[arg] == "*")
			{
				c = mult(a, b)
				stack[++stackPtr] = c
			}
			else if(token[arg] == "/")
			{
				c = div(b, a)
				stack[++stackPtr] = c
			}
			else if(token[arg] == "^")
			{
				c = expo(b, a)
				stack[++stackPtr] = c
			}
    	}
    }
    print stack[stackPtr]
}
