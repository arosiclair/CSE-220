# Homework #3
# Name: Andrew Rosiclair
# SBU ID: 109235970

.data
	den: .float 180	#Used for conversion to radians
	pie: .float 3.1415926535897924
	floatOne: .float 1	#Used for negative sign bit
	floatSign: .float -0	#Used for negative sign bit
	pointOne: .float 0.1	#Used for dividing by 10
	signLabel:	.asciiz "Sign:\t"
	posSign: .asciiz "0 +"
	negSign: .asciiz "1 -"
	exponentLabel: .asciiz "\nExponent:\t"
	fractionLabel: .asciiz "\nFraction:\t"
	binaryLabel: .asciiz "\nBinary Product:\t"
	.align 2
	exp: .space 1
	.align 2
	frac: .space 3
	
.text
#Converts a given angle argument from degrees to radians.
#Args: Single-prec degrees in $f12
#Returns: Single-prec Radians in $f0
toRadians:

	la $t0,pie
	la $t1,den
	mtc1 $0,$f4		#Clear these registers
	mtc1 $0,$f5
	mtc1 $0,$f6
	l.s $f4,0($t0)		#Load $f4 with the pi constant
	l.s $f5,0($t1)		#Load $f5 with the denominator, 180
	div.s $f6, $f4, $f5		#Save a single precision quotient in $f6
	mul.s $f0, $f12, $f6	#Multiply the argument by our quotient and save the result to be returned in $
	
	jr $ra		#return to the calling function
	
#Finds the arclength of a given angle in degrees and float distance
#Args: Single-prec degrees in $f12,
#		Single prec redius in $f13
#Returns: Single-prec arclength in $f0
arcLength:

	addi $sp,$sp,-4	#Shift the stack pointer down a word to fit $ra
	sw $ra,($sp)	#Store the original $ra because we will be calling another function    
	jal toRadians	#Convert the degrees in $f12 to radians in $f0
	mov.s $f12,$f0
	mul.s $f0,$f12,$f13	#Multiply the radian result from the "toRadians" call by the radius.
	
	lw $ra,($sp)	#Restore the return address we saved.
	addi $sp,$sp,4	#Reset the position of the stack pointer.	
	jr $ra			#Return to calling function

#Takes a string argument and converts it to its floating point representation.	
atof:
	
	#STACK HEADER
	addi $sp,$sp,-16	
	sw $ra,($sp)	#Save the return address for later
	sw $s0,4($sp)	#Save $s0 as we will be using it as a counter.
	sw $s1,8($sp)
	sw $s2,12($sp)	
		
	move $s0,$a0	#We'll save the address of the input over to $s0
	li $s1,0		#We'll use $s1 as a counter
					#removeNull
	
	#Get the sign bit.	
	lb $t0,0($s0)	#Check the sign-bit
	li $t2,'-'
	beq $t0,$t2,negative	#Add a sign-bit 1 if the first char is "-"
	li $s1,0		#We're going to use $s1 as a index starting at 0
	li $t0,0		#Make sure $t0 is empty
	jal exponent	#Contine to process the exponent bits.
	li $s2,0		#Another counter is needed.
	jal fraction	#Continue to process the fraction bits.
	
	#RESTORE FROM STACK HEADER
	lw $ra,($sp)	#Restore our saved registers.
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	addi $sp,$sp,16	#Restore position of stack pointer
	jr $ra	#After we finish, Jump back to the caller. The result is in $f0
	
negative:
	
	li $t0,0x80000000	#Insert the sign bit
	li $s1,1		#We're going to use $s1 as a index starting at 1
	jal exponent
	li $s2,0		#Another counter is needed.
	jal fraction	#Continue to process the fraction bits.
	
	#RESTORE FROM STACK HEADER
	#move $sp,$fp	#Restore position of stack pointer
	lw $ra,($sp)	#Restore our saved registers.
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	addi $sp,$sp,16	#Restore position of stack pointer	
	jr $ra	#After we finish, Jump back to the caller. The result is in $f0
	

#Reads in each character of the leading integer in the ascii string.
#Converts it to integer representation and inserts it into $f0	
exponent:
	
	add $t5,$s0,$s1	#t5 has address of the next char
	sll $t1,$t1,4	#Make room for the next digit
	lb $t2,($t5)	#Load the next char into $t2
	li $t3,'\n'
	beq $t2,$t3,noDecimal	#Convert the digits to float representation than finish.
	li $t3,'0'		#Load $t3 with ASCII '0' for conversion
	sub $t4,$t2,$t3	#Set $t4 to a converted digit.
	abs $t4,$t4		#In case we just tried converting a period.
	or $t1,$t1,$t4	#Insert the $t4 digit into $t1
	addi $s1,$s1,1	#Increment counter
	li $t3,'.'
	bne $t2,$t3,exponent	#repeat if the char we just inserted wasn't a period
	
	
	srl $t1,$t1,4	#The char we just inserted was a decimal so we'll shift it out
	mtc1 $t1,$f4	#Move the integer digits we have to a float
	cvt.s.w $f4,$f4	#Convert the integer to a single-prec float
	mfc1 $t1,$f4	#Move the float bits back
	andi $t1,$t1,0x7F800000	#Take the exponent bits
	or $t0,$t0,$t1	#Insert our exponent bits with the sign bit we have in $t0
	mtc1 $t0,$f0	#Move our sign bit and exponent bits into our result register.
	
	jr $ra	#Return
	
noDecimal:

	srl	$t1,$t1,4	#Reposition our integer
	mtc1 $t1,$f4	#Move the integer digits we have to a float
	cvt.s.w $f4,$f4	#Convert the integer to a single-prec float
	mfc1 $t1,$f4	#Move the float bits back
	or $t0,$t0,$t1	#Insert our float bits with the sign bit we have in $t0
	mtc1 $t0,$f0	#Move our sign bit and exponent bits into our result register.
	
	#RESTORE FROM STACK HEADER
	lw $ra,($sp)	#Restore our saved registers.
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	addi $sp,$sp,16	#Restore position of stack pointer
	jr $ra	#After we finish, Jump back to the caller. The result is in $f0
	
fraction:
	
	add $t4,$s0,$s1	#t4 has address of the next char
	addi $s2,$s2,1	#$s2 keeps track of how many places past decimal point.
	lb $t0,($t4)	#Load the next char after the decimal
	li $t3,'\n'
	beq $t0,$t3,finish	#Stop once the char we load is the new line char at the end.
	addi $s1,$s1,1	#increment index
	li $t1,'0'
	sub $t0,$t0,$t1	#Set $t0 to the integer conversion from ASCII
	mtc1 $t0,$f4	#Move the digit to a float register
	cvt.s.w $f4,$f4	#Convert the digit in $f4 to a true float value		
	li $t2,0		#Used to count how many multiplications are needed.
	j reposition	#reposition the digit to the right decimal place and jump back
	j fraction

reposition:

	l.s $f5,pointOne	#Load 0.1 into $f5
	mul.s $f4,$f4,$f5	#move over 1 place
	addi $t2,$t2,1		#increment our temp counter
	beq $t2,$s2,insert	#Stop once we've moved it over enough
	j reposition		#Repeat until we've moved enough times.

insert:
	mfc1 $t0,$f0
	andi $t1,$t0,0x80000000
	bnez $t1,negInsert	#Check if neg
	li $t0,0
	li $t1,0
	add.s $f0,$f0,$f4	#Insert the adjusted decimal
	j fraction		#Return to process next char.

negInsert:

	li $t0,0
	li $t1,0
	sub.s $f0,$f0,$f4	#Insert the adjusted decimal
	j fraction		#Return to process next char.
	
finish:

	jr $ra	#return to atof
	
print_parts:

	#STACK HEADER
	addi $sp,$sp,-8
	sw	$ra,($sp)
	sw	$s0,4($sp)
	
	mfc1 $t0,$f12	#Copy the float to $t0
	#Check sign-bit
	andi $t1,$t0,0x80000000
	bgtz $t1,neg
	bltz $t1,neg
		
	#Print sign label
	li $v0,4
	la $a0,signLabel
	syscall
	la $a0,posSign
	syscall
	
	j printExp

neg:

	#Print sign label
	li $v0,4
	la $a0,signLabel
	syscall
	la $a0,negSign
	syscall
	
	j printExp
	
printExp:

	andi $t1,$t0,0x7F800000	#Set $t1 to the 8 exponent bits.
	srl $t1,$t1,23		#Shift the exponent bits to the end.
	#sb $t1,exp		#Store the exp bits in a string label.
	
	li $s0,0		#Use $s0 as an index
	#jal printChars
	
	#Print the exponent 
	li $v0,4
	la $a0,exponentLabel
	syscall
	li $v0,35
	la $a0,($t1)
	syscall
	li $v0,11
	li $a0,'\t'
	syscall
	li $v0,1
	la $a0,($t1)
	syscall

	j printFrac	#Continue to print the fraction bits
	
printFrac:

	andi $t1,$t0,0x007FFFFF	#Set $t1 to the lower 23 bits
	sw $t1,frac
	
	#Print the fraction
	li $v0,4
	la $a0,fractionLabel
	syscall
	li $v0,35
	la $a0,($t1)
	syscall
	li $v0,11
	li $a0,'\t'
	syscall
	li $v0,1
	la $a0,($t1)
	syscall
	
	#RESTORE FROM STACK HEADER
	lw $ra,($sp)
	lw $s0,4($sp)
	addi $sp,$sp,8	#Restore the Stack Pointer position
	jr $ra	#Return to caller
	
print_binary_product:

		li $v0,4		#Print the label
		la $a0,binaryLabel
		syscall
		
		li $v0,11	#Move down a line.
		li $a0,'\n'
		syscall
		
		jr $ra
		
arcLengthS:

	#STACK HEADER
	addi $sp,$sp,-4
	sw $ra,($sp)
	
	#move $a0,$v0	#Move the input address to $a0
	li $t1,0
	jal atof		#atof will convert the radius input at $a0 to a float at $f0
	mov.s $f13,$f0	#Set the second arg to the float value of the radius
	#Degrees are in $f12 as a float
	jal arcLength	#Calculate the arcLength which is now in $f0
	
	lw $ra,($sp)	#Restore registers
	addi $sp,$sp,4
	jr $ra		#Return to the caller

product:

	#STACK HEADER
	move $fp,$sp	#Save the position of the stack pointer before call
	addi $sp,$sp,-16
	sw $ra,($sp)
	sw $s0,4($sp)
	sw $s1,8($sp)
	sw $s2,12($sp)
	
	li $t0,1
	blt $a0,$t0,zero	#Return zero if our arg < 1
	beq $a0,$t0,one		#Return the first arg on the stack
	
	li $s0,0		#We'll use $s0 as a counter
	li $s1,4		#We'll use $s1 as the length of a word
	move $s2,$a0	#Copy over the number of args to $s2
	l.s $f0,floatOne
	j multArgs

zero:

	mtc1 $0,$f0
	#RESTORE REGISTERS
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	addi $sp,$sp,16
	
	jr $ra

one:

	lw $t0,($fp)
	mtc1 $t0,$f0
	#RESTORE REGISTERS
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	addi $sp,$sp,16
	
	jr $ra

multArgs:

	beq $s2,$s0,done	#Stop if we processed enough args.
	lwc1 $f4,0($fp)
	add $fp,$fp,$s1		#Move the frame pointer up by a word
	mul.s $f0,$f0,$f4	#Multiply the total in $f0 with the float we just took
	addi $s0,$s0,1		#Increment counter
	j multArgs

done:
	
	#RESTORE REGISTERS
	lw $ra,($sp)
	lw $s0,4($sp)
	lw $s1,8($sp)
	lw $s2,12($sp)
	addi $sp,$sp,16
	
	jr $ra
	