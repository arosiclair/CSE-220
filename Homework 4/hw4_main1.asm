# Constants
.eqv QUIT 10
.eqv PRINT_STRING 4
.eqv READ_INT 5
.eqv READ_CHAR 12

# Macro defines
.macro printPrompt(%STRING_LABEL)
    # save the old value of $a0 and $v0
    addiu $sp, $sp, -8
    sw $a0, 0($sp)
    sw $v0, 4($sp)
    # print the number prompt
    li $v0, PRINT_STRING
    la $a0, %STRING_LABEL
    syscall
    # load the old value of $a0 and $v0
    lw $a0, 0($sp)
    lw $v0, 4($sp)
    addiu $sp, $sp, 8
.end_macro

.macro readIntValue
    # integer input from user
    li $v0, READ_INT
    syscall
.end_macro

.data
str_enter_number:   .asciiz "\nEnter a number: "
str_enter_divisor:  .asciiz "Enter divisor: "
str_enter_base:     .asciiz "Enter base: "
str_remainder:      .asciiz "The remainder is: "
str_repeat_prompt:  .asciiz "\nRepeat (Y/N)?"

.text
.globl main
main:
    # Print the number prompt & get value
    printPrompt(str_enter_number)
    readIntValue()
    move $a0, $v0   # save value argument for remainder

    # Print the divisor prompt & get value
    printPrompt(str_enter_divisor)
    readIntValue()
    move $a1, $v0   # set divisor argument for remainder

    # Call the remainder function
    jal remainder

    # Print the remainder prompt
    printPrompt(str_remainder)

    # print the result
    move $a0, $v0
    li $v0, 1
    syscall

    # Print the number prompt & get value
    printPrompt(str_enter_number)
    readIntValue()
    move $a0, $v0   # set value argument for remainder

    # Print the base prompt & get value
    printPrompt(str_enter_base)
    readIntValue()
    move $a1, $v0   # set value argument for remainder

    # Call the print base fuction
    jal printBase

    # Print the Repeat (Y/N)
input_again:
    printPrompt(str_repeat_prompt)

    # Get the user input
    li $v0, READ_CHAR
    syscall

    # Check for N
    li $t2, 'N'
    beq $v0, $t2, done

    # Check for Y
    li $t2, 'Y'
    beq $v0, $t2, main

    j input_again


done:   # Exit the program
    li $v0, QUIT
    syscall


###############################################################################
#          Student defined functions will be included starting here           #
###############################################################################
.include "hw4.asm"
