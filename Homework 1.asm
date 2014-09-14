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

	#--------------------------TWO'S COMPLEMENT REPRESENTATIONS---------------------------------
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
	
	#--------------------------ONE'S COMPLEMENT REPRESENTATIONS---------------------------------
	la $a0,onesLabel	#Print one's complement representation
	li $v0, 4	
	syscall
	li $v0,1		#Print integer value in One's complement
	la $a0, ($t0)
	syscall
	li $v0,4		
	la $a0,tab
	syscall
	#bltz $t0,decrement
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
	#bltz $t0,increment
	
	#--------------------------SIGNED MAGNITUDE REPRESENTATIONS---------------------------------
	la $a0,signLabel	#Print the Signed Magnitude label
	li $v0,4
	syscall
	li $v0,1		#Print integer
	la $a0, ($t0)
	syscall
	li $v0,4		
	la $a0,tab
	syscall
	neg $t0,$t0		#Negate the integer
	la $a0,($t0)
	li $v0,34		#Print hex repesentation
	syscall
	li $v0,4
	la $a0,tab
	syscall
	li $v0,35		#Print binary representation
	syscall
	li $v0,4
	la $a0,tab
	syscall
	neg $t0,$t0		#Negate the integer again
	li $v0,101		#Print SM integer representation
	la $a0,($t0)
	syscall
	
	li $v0,10		#Exit
	syscall
	
decrement:
	addi $t0,$t0,-1
increment:
	addi $t0,$t0,1

.data
	prompt: .asciiz "Enter an integer.\n"
	tab: .asciiz "\t"
	
	#Numerical format labels
	twosLabel: .asciiz "\n2's complement:\t"
	onesLabel: .asciiz "\n1's complement:\t"
	signLabel: .asciiz "\nSign Magnitude:\t"
	negTwosLabel: .asciiz "\nNeg 2's complement:\t"
	sixteenTwos: .asciiz "\n16-bit 2's comp:\t"