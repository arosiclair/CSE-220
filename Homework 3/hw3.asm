# Homework #3
# Name: Andrew Rosiclair
# SBU ID: 109235970

#Converts a given angle argument from degrees to radians.
toRadians:
	move $f0,$a0	#Save a copy of the angle in degrees to $t0
	la $f4,PI		#Load $t1 with the pi constant
	li $f6,180
	div.s $f8,$f4,$f6	#Save a double precision quotient in $f0
	mul.s $
	
	
	

