# Constants
.eqv QUIT 10
.eqv PRINT_STRING 4
.eqv READ_STRING 8
.eqv READ_CHAR 12
.eqv STRING_LEN 101
.eqv PRINT_INT 1

# Macro defines
.macro printPrompt(%STRING_LABEL1, %STRING_LABEL2)
    # print the number prompt
    li $v0, PRINT_STRING
    la $a0, %STRING_LABEL1
    syscall
    # print the number prompt
    li $v0, PRINT_STRING
    la $a0, %STRING_LABEL2
    syscall
.end_macro

.macro readString (%address_label)
    # integer input from user
    li $v0, READ_STRING
    la $a0, %address_label
    li $a1, STRING_LEN
    syscall
.end_macro

.macro printIntValue(%register)
    # integer input from user
    li $v0, PRINT_INT
    move $a0, %register
    syscall

    li $v0, PRINT_STRING
    la $a0, endl
    syscall
.end_macro


.data
str_prompt_m1: .asciiz "\nEnter a 100 character string for matrix 1: "
str_prompt_m2: .asciiz "\nEnter a 100 character string for matrix 2: "
str_error:  .asciiz "ERROR string is not 100 characters!\n"
str_m1:  .asciiz "\nMatrix 1:\n"
str_m2:  .asciiz "\nMatrix 2:\n"
str_result:  .asciiz "\nResult matrix:\n"
str_result2:  .asciiz "\nResult matrix printed as a string:\n"


.text
.globl main
main:
    # set arguments
    la $a0, m1
    la $a1, str_prompt_m1
    la $a2, str_error
    jal rd100Chars

    # set arguments
    la $a0, m2
    la $a1, str_prompt_m2
    la $a2, str_error
    jal rd100Chars

    # print label
    la $a0, str_m1
    li $v0, 4
    syscall

    la $a0, m1
    jal printIntMatrix

    # print label
    la $a0, str_m2
    li $v0, 4
    syscall

    la $a0, m2
    jal printIntMatrix


    la $a0, m1
    la $a1, m2
    la $a2, result
    jal matrixMult

    # print label
    la $a0, str_result
    li $v0, 4
    syscall

    la $a0, result
    jal printIntMatrix

    # print label
    la $a0, str_result2
    li $v0, 4
    syscall

    # print the matrix as a string
    li $v0, PRINT_STRING
    la $a0, result
    syscall

    # Exit the program
    li $v0, QUIT
    syscall


###############################################################################
#          Student defined functions will be included starting here           #
###############################################################################
.include "hw4.asm"
