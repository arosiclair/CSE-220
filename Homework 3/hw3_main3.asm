# hw3_main3.asm
# Do NOT modify this file.
# This file is NOT part of your homework 3 submission.

.data
pi: .float 3.1415926535897924
prompt_float: .asciiz "\nPlease enter a number: "
another: .asciiz "\nAnother (Y/N)? "
productmsg: .asciiz "\nThe product of the sequence is: "
input: .space 50
incorrect: .asciiz "\nBad input. Try again"

# Constants
.eqv QUIT 10
.eqv PRINT_STRING 4
.eqv READ_FLOAT 6
.eqv READ_CHAR 12
.eqv READ_STRING 8
.eqv PRINT_FLOAT 2

.text
.globl _start

####################################################################
# This is the "main" of your program; Everything starts here.
####################################################################

_start:
	# Ask the user for a list of values
	li $s1, 0

product_input_loop:

	# increment the counter
	addi $s1, $s1, 1

	# Ask the user for a float value
	# print the prompt
	li $v0, PRINT_STRING
	la $a0, prompt_float
	syscall

	# Get the user input
	li $v0, READ_STRING
	la $a0, input
	addi $a1, $zero, 49
	syscall

	# Parse the input
	la $a0, input
	jal atof

	# store the parsed float from the atof function on stack
	addi $sp, $sp, -4
	s.s $f0, 0($sp)

product_input_again:
	# Ask the user if they want to enter another value
	# print the prompt
	li $v0, PRINT_STRING
	la $a0, another
	syscall

	# Get the user input
	li $v0, READ_CHAR
	syscall

	# Check for N
	li $t2, 'N'
	beq $v0, $t2, product_input_done

	# Check for Y
	li $t2, 'Y'
	beq $v0, $t2, product_input_loop

	# Else bad input tell user
	li $v0, PRINT_STRING
	la $a0, incorrect
	syscall
	j product_input_again

product_input_done:
	# save the value to adjust stack pointer by
	sll $s0, $s1, 2

	# Move the amount of items from the stack into it
	move $a0, $s1
	jal product

	# adjust stack pointer back to the correct position
	add $sp, $sp, $s0

	# Print out the product
	la $a0, productmsg
	mov.s $f12, $f0
	jal print_msg_float

	# Exit the program
	li $v0, QUIT
	syscall

####################################################################
# End of MAIN program
####################################################################


####################################################################
# Function to print out a string and a float value
# void print_msg_float (string address, float value)
#
# Parameters passed:
# $a0 = address of msg to print
# $f12 = value to print
####################################################################

print_msg_float:

	# Store the return address on the stack
	addi $sp, $sp, -4
	sw $ra, 0($sp)

	# Print message
	li $v0, PRINT_STRING

	# $a0 already has the string address
	syscall

	# Print float
	li $v0, PRINT_FLOAT

	# $f12 already has the float value
	syscall

	# Print newline
	li $v0, 11
	li $a0, '\n'
	syscall

	# get back the return address
	lw $ra, 0($sp)
	addi $sp, $sp, 4

	jr $ra

#################################################################
# Student defined functions will be included starting here
#################################################################

.include "hw3.asm"
