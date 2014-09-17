# Homework #1
# Name: Andrew Rosiclair
# SBU ID: 109235970

.text
.globl main
main:

	la $a0,prompt	#Load the prompt string for printing
	
	li $v0,4		#Print the prompt
	syscall
	
	li $v0,5		#Read the entered integer
	syscall
	
	la $t0,($v0)	#Load the result into a temp address
	b twos			#call the twos label

twos:	#--------------------------TWO'S COMPLEMENT REPRESENTATIONS---------------------------------

	li $v0,4		
	la $a0,twosLabel
	syscall
	li $v0,1		#Print integer value
	la $a0,($t0)
	syscall
	li $v0,4		
	la $a0,tab
	syscall
	li $v0,34		#Print hex representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,35		#Print binary representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,1
	la $a0, ($t0)
	syscall			#Print the integer representation again.
	
	b ones
	
ones:	#--------------------------ONE'S COMPLEMENT REPRESENTATIONS---------------------------------

	la $a0,onesLabel	#Print one's complement representation
	li $v0, 4	
	syscall
	li $v0,1		#Print integer value in One's complement
	la $a0, ($t0)
	syscall
	li $v0,4		
	la $a0,tab
	syscall
	
	bltz $t0,onesNeg	#Branch to either onesNeg or onesPos depending on if the input is positive or negative
	b onesPos
	
onesNeg:
	addi $t0,$t0,-1	#decrement the input
	li $v0,34		#Print hex representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,35		#Print binary representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,1
	la $a0, ($t0)
	syscall			#Print the 1's complement integer representation again.
	addi $t0,$t0,1	#increment input
	
	b signMag
	
onesPos:
	
	li $v0,34		#Print hex representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,35		#Print binary representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,100
	la $a0, ($t0)
	syscall			#Print the 1's complement integer representation again.
	
	b signMag
	
signMag:	#--------------------------SIGNED MAGNITUDE REPRESENTATIONS---------------------------------

	la $a0,signLabel	#Print the Signed Magnitude label
	li $v0,4
	syscall
	li $v0,1		#Print integer
	la $a0, ($t0)
	syscall
	li $v0,4		
	la $a0,tab
	syscall
	addi $t1,$t0,-1 #decrement the input and store it in $t1
	neg $t1,$t1		#Negate the new integer
	
	bltz $t0,signNeg	#Branch to handle both positive and negative inputs.
	b signPos
	
signNeg:

	ori $t1,$t1,10000000000000000000000000000000	#Flip the first bit
	la $a0,($t1)
	li $v0,34		#Print hex repesentation
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,35		#Print binary representation
	la $a0,($t1)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,101		#Print SM integer representation
	la $a0,($t0)
	syscall
	
	b negTwos

signPos:

	la $a0,($t1)
	li $v0,34		#Print hex repesentation
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,35		#Print binary representation
	la $a0,($t1)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,101		#Print SM integer representation
	la $a0,($t1)
	syscall
	
	b negTwos
	
negTwos:

	li $v0,4	#Print Negative Two's Label
	la $a0,negTwosLabel
	syscall
	li $v0,1	#Negate the input value and print
	neg $t0,$t0	
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall	
	li $v0,34	#Print the hex representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall	
	li $v0,35	#Print the binary representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,1 #Print the integer again
	la $a0,($t0)
	syscall
	
	b sixBitTwos
	
sixBitTwos:
	
	li $v0,4	#16-bit twos complement label
	la $a0,sixteenTwos
	syscall
	li $v0,102	#Print 16-bit two's integer representation
	la $a0,($t0)
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,34	#Print the hex representation
	la $a0,($t0)
	syscall
	
	li $v0,10		#Exit
	syscall
	
	
.data
	prompt: .asciiz "Enter an integer.\n"
	tab: .asciiz "\t"
	
	#Numerical format labels
	twosLabel: .asciiz "\n2's complement:\t"
	onesLabel: .asciiz "\n1's complement:\t"
	signLabel: .asciiz "\nSign Magnitude:\t"
	negTwosLabel: .asciiz "\nNeg 2's complement: "
	sixteenTwos: .asciiz "\n16-bit 2's comp:\t"
