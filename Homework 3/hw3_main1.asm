# hw3_main1.asm
# Do NOT modify this file.
# This file is NOT part of your homework 3 submission.

.data
pi: .float 3.1415926535897924
prompt_d2r: .asciiz "Degrees To Radians: "
radians: .asciiz "Radians: "
prompt_radius: .asciiz "Enter a radius: "
arclength: .asciiz "Arc length s: "


# Constants
.eqv QUIT 10
.eqv PRINT_STRING 4
.eqv PRINT_FLOAT 2

.text
.globl _start

####################################################################
# This is the "main" of your program; Everything starts here.
####################################################################

_start:
  li $v0, 1

  # print degrees to radians prompt
  li $v0, PRINT_STRING
  la $a0, prompt_d2r
  syscall

  # Get the user input: float degrees
  li $v0, 6
  syscall

  # move the value to the argument of toRadians
  # save the degree value entered in the fp saved registers
  # call the function toRadians
  mov.s $f12, $f0
  mov.s $f20, $f0
  jal toRadians         # To be defined in your hw3.asm

  # Print the result of toRadians
  la $a0, radians
  mov.s $f12, $f0
  jal print_msg_float

  # print degrees to radians prompt
  li $v0, PRINT_STRING
  la $a0, prompt_radius
  syscall

  # Get the user input: float radius
  li $v0, 6
  syscall

  # The degrees are saved in the $f20 register, move to $f12
  mov.s $f12, $f20
  # Move the radius to the fp argument register
  mov.s $f13, $f0
  jal arcLength         # To be defined in your hw2.asm

  # Print the result of arcLength
  la $a0, arclength
  mov.s $f12, $f0
  jal print_msg_float

  # Exit the program
  li $v0, QUIT
  syscall

####################################################################
# Function to print out a string and a float value
# void print_msg_float (string address, float value)
#
# Parameters passed:
# $a0 = address of msg to print
# $f12 = value to print
####################################################################

print_msg_float:

  # Allocate space on the stack to hold the value
  # Store the argument $a0 on the stack to preserve the value
  addi $sp, $sp, -4
  sw $a0, 0($sp)

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

  # Return the $a0 register to original value
  # Move the stack back to its initial position
  lw $a0, 0($sp)
  addi $sp, $sp, 4

  jr $ra


#################################################################
# Student defined functions will be included starting here
#################################################################

.include "hw3.asm"
