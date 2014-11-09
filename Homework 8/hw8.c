/*
Homework #8
name: Andrew Rosiclair
SBU ID: 109235970
*/

#include "hw8.h"

/**
* Calculates the length of a '\0' terminated string.
* (the null character is not part of the length)
* @param str Pointer to a '\0' terminated string.
* @return Returns the length of the string.
*/
size_t hw_strlen(const char *str){
	int i = 0;

	/* Handle NULL reference special case */
	if(str == NULL)
		return 0;

	while (*(str + i) != '\0')
		i++;

	return i;
}

/**
* Determines if two strings are exactly equal to each other.
* @param str1 Pointer to a '\0' terminated string.* @param str2 Pointer to a '\0' terminated string.
* @return Returns 1 if equal, else 0.
*/
int hw_strcmp(const char *str1, const char *str2){
	int i = 0;

	/* Compare string lengths first */
	if(hw_strlen(str1) != hw_strlen(str2))
		return 0;

	/* Compare each char */
	for(i = 0; i < hw_strlen(str1); i++){
		if(*(str1 + i) != *(str2 + i))
			return 0;
	}

	return 1;
}

/**
* Copies the string from src into dst.
* @param dest Buffer to copy into.
* @param src String to copy.
* @param n Non-negative maximum amount of bytes that can be copied from src.
* @return Returns the dst pointer.
*/
char* hw_strncpy(char *dst, const char *src, size_t n){
	int i;

	/* Check if n is invalid or 0 */
	if (n <= 0)
		return dst;

	/* Assign each char from src to dst */
	for(i = 0; i < n; i++){
		*(dst + i) = *(src + i);
	}

	return dst;
}

/*** Finds the index of the character c in the string.
* @param str A `\0` terminated string to search.
* @param c Character to search for.
* @return Returns the index of the first occurrence of the character
* if it exists, else -1.
*/
int hw_indexof(const char *str, char c){
	int i = 0;
	char temp;

	while((temp = *(str + i)) != c){
		if (temp == '\0') return -1;
		i++;
	}

	return i;
}

/**
* Reverse the contents of the string.
* @param str Pointer to a `\0` terminated string.
*/
void hw_reversestr(char *str){
	int i = 0, end = 0;
	char temp;

	/* get index of the end of the string */
	while(*(str + end) != '\0')
		end++;
	
	/* If the string is only 1 char, do nothing */
	if(end == 1)
		return;

	end--;

	/* Swap each char at each end */
	while (i < end){
		temp = *(str + end);
		*(str + end) = *(str + i);
		*(str + i) = temp;
		i++;
		end--;
	}
}

/**
* Determines if two strings are equal to each other ignoring the case.
* @param str1 Pointer to a `\0` terminated string.
* @param str2 Pointer to a `\0` terminated string.
* @return Returns 1 if the strings are equal, else 0.
*/
int hw_equalsIgnoreCase(const char *str1, const char *str2){
	int i;
	char c1, c2;

	/* Compare string lengths first */
	if(hw_strlen(str1) != hw_strlen(str2))
		return 0;

	for (i = 0; i < hw_strlen(str1); i++){
		c1 = *(str1 + i);
		c2 = *(str2 + i);

		/* Ensure first char is a letter */
		if ((c1 >= 'a' && c1 <= 'z') || (c1 >= 'A' && c1 <= 'Z')){
			/* Ensure second char is a letter */
			if((c2 >= 'a' && c2 <= 'z') || (c2 >= 'A' && c2 <= 'Z')){
				/* Check if either letter is equal in any case */
				if(c1 != c2 && (c1 + 32) != c2 && c1 != (c2 + 32))
					return 0;
			}
			/* Second char isn't a letter so just compare */
			if(c1 != c2)
				return 0;
		}
		/* First char isn't a letter so just compare */
		if(c1 != c2)
				return 0;
	}

	return 1;
}

/**
* Replace all characters in the pattern with another symbol.
* @param str String to replace values that match the pattern.
* @param pattern Array of characters to search and replace for.
* @param replacement Character to replace with.
*/
void hw_replaceall(char *str, const char *pattern, char replacement){
	int i = 0;
	char c;

	/* loop through each character in the string */
	while((c = *(str + i)) != '\0'){
		/* Check if we find the string character in the pattern */
		if(hw_indexof(pattern, c) != -1)
			*(str + i) = replacement;
	}
}

