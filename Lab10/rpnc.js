/*
 * Node.js / JavaScript Reverse Polish Notation (RPN) Calculator
 *
 * Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
 * 
 */

function add(a, b)
{
	return a+b;
}

function sub(a, b)
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

function exp(a, b)
{
	return Math.pow(a, b);
}

var i;
var stack = [];
var expression = process.argv[2];
var args = expression.split(" ");
for(i = 0; i < args.length; ++i)
{
	if(isNaN(args[i]))
	{
		this.a = Number(stack.pop());
		this.b = Number(stack.pop());
		// console.log('a = ', this.a);
		// console.log('b = ', this.b);
		if(args[i] == '+')
		{
			this.c = add(this.a, this.b);
		}
		else if(args[i] == '-')
		{
			this.c = sub(this.b, this.a);
		}
		else if(args[i] == '*')
		{
			this.c = mult(this.a, this.b);
		}
		else if(args[i] == '/')
		{
			this.c = div(this.b, this.a);
		}
		else if(args[i] == '^')
		{
			this.c = exp(this.b, this.a);
		}
		// console.log('c = ' + this.c);
		stack.push(this.c);
	}
	else
	{
		stack.push(args[i])
		// console.log(stack);
	}
}
stack.pop();
var result = stack.pop();
console.log(result);