#
# Bash Reverse Polish Notation (RPN) Calculator
#
# Accepts an single argument a RPN, evaluates it and prints the answer to stdout.
# 

# Global Varaiables
declare -a stack
stackPointer=-1
regularExp='^[0-9]+$'
replace=`echo "$1" | tr '*' 'x'`

# Stack Functions
push()
{
	stackPointer=$((stackPointer+1))
	stack[stackPointer]=$1
}

pop()
{
	stackPointer=$((stackPointer-1))
}

# Arithmetic Functions
addition()
{
	sum=$(($1 + $2))
	#return $sum
}

subtraction()
{
	difference=$(($1 - $2))
	#return $difference	
}

multiplication()
{
	product=$(($1 * $2))
	#return $product
}

division()
{
	dividen=$(($1 / $2))
	#return $dividen
}

power()
{
	exp=$(($1**$2))
	#return $exp
}

for token in $replace
do
	if [[ "$token" =~ $regularExp ]]
	then
		push $token
	else
		oper2=${stack[$stackPointer]}
		pop
		oper1=${stack[$stackPointer]}
		pop
		if [ "$token" == "+" ]
		then
			addition $oper1 $oper2
			push $sum
		elif [ "$token" == "-" ]
		then
			subtraction $oper1 $oper2
			push $difference
		elif [ "$token" == "x" ]
		then
			multiplication $oper1 $oper2
			push $product
		elif [ "$token" == "/" ]
		then
			division $oper1 $oper2
			push $dividen
		elif [ "$token" == "^" ]
		then
			power $oper1 $oper2
			push $exp
		fi
	fi
done
echo ${stack[stackPointer]}