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
enter_String: .asciiz "Enter String "
A: .asciiz "A: "
B: .asciiz "B: "
print_length: .asciiz "Length of "
print_edit_distance: .asciiz "The edit distance between the strings is: "
endl: .asciiz "\n"
StringA: .space 50
StringB: .space 50

.text
.globl main
main:
    # Print the prompt & get String
    printPrompt(enter_String, A)
    readString(StringA)

    # Print the prompt & get String
    printPrompt(enter_String, B)
    readString(StringB)

    # Print the prompt & get String
    printPrompt(print_length, A)
    la $a0, StringA
    jal strlen
    move $s0, $v0
    printIntValue($s0)

    # Print the prompt & get String
    printPrompt(print_length, B)
    la $a0, StringB
    jal strlen
    move $s1, $v0
    printIntValue($s1)

    la $a0, StringA
    move $a1, $s0
    la $a2, StringB
    move $a3, $s1
    jal levenshteinDistance
    move $s0, $v0

    # Print the edit distance string
    li $v0, PRINT_STRING
    la $a0, print_edit_distance
    syscall

    #print the result of levenshtein distance
    printIntValue($s0)

    # Exit the program
    li $v0, QUIT
    syscall

###############################################################################
#          Student defined functions will be included starting here           #
###############################################################################
.include "hw4.asm"
