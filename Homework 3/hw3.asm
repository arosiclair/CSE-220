# Homework #3
# Name: Andrew Rosiclair
# SBU ID: 109235970

#Converts a given angle argument from degrees to radians.
toRadians:

	la $t0,pi
	la $t1,den
	l.s $f4,0($t0)		#Load $f5 with the pi constant
	l.s $f6,0($t1)		#Load $f6 with the denominator, 180
	div.s $f8, $f4, $f6		#Save a single precision quotient in $f0
	mul.s $f0, $f12, $f8	#Multiply the argument by our quotient and save the result to be returned in $
	
	jr $ra		#return to $ra
	
#Finds the arclength of a given angle in degrees and float distance
arcLength:

	la $fp,($sp)
	addi $sp,$sp,-4	#Shift the stack pointer down a word to fit $ra
	sw $ra,($fp)	#Store the original $ra because we will be calling another function    
	jal toRadians	#Convert the degrees in $f12 to radians in $f0
	
	mul.s $f0,$f0,$f13	#Multiply the radian result from the "toRadians" call by the second argument.
	
	lw $ra,($fp)	#Restore the return address we saved.
	jr $ra		#Return to $ra


.data
	den: .float 180
	
	

