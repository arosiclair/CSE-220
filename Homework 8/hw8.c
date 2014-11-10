/*
Homework #8
name: Andrew Rosiclair
SBU ID: 109235970
*/

#include "hw8.h"
#include <assert.h>

/* Function Prototypes */
int isALetter(char c);

/**
* Calculates the length of a '\0' terminated string.
* (the null character is not part of the length)
* @param str Pointer to a '\0' terminated string.
* @return Returns the length of the string.
*/
size_t hw_strlen(const char *str){
	int i = 0;

	/* Handle NULL reference special case */
	if(str != NULL){
		while (*(str + i) != '\0')
			i++;
	}

	return i;
}

/**
* Determines if two strings are exactly equal to each other.
* @param str1 Pointer to a '\0' terminated string.* @param str2 Pointer to a '\0' terminated string.
* @return Returns 1 if equal, else 0.
*/
int hw_strcmp(const char *str1, const char *str2){
	int i = 0;

	if(str1 == NULL || str2 == NULL)
		return 0;

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

	/* Return NULL if either of the strings are NULL */
	if(dst == NULL || src == NULL)
		return NULL;

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

	if(str == NULL)
		return -1;

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

	/* if str is NULL, do nothing */
	if(str == NULL)
		return;
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

	if(str1 == NULL || str2 == NULL)
		return 0;

	/* Compare string lengths first */
	if(hw_strlen(str1) != hw_strlen(str2))
		return 0;

	/* Loop through each char for each string */
	for (i = 0; i < hw_strlen(str1); i++){
		c1 = *(str1 + i);
		c2 = *(str2 + i);

		/* Ensure first char is a letter */
		if (isALetter(c1)){
			/* Ensure second char is a letter */
			if(isALetter(c2)){
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

int isALetter(char c){
	if(c >= 'a' && c <= 'z')
		return 1;
	else if(c >= 'A' && c <= 'Z')
		return 1;
	else
		return 0;
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

	/* Do nothing if any of the parameters are NULL */
	if(str == NULL || pattern == NULL)
		return;
	/* loop through each character in the string */
	while((c = *(str + i)) != '\0'){
		/* Check if we find the string character in the pattern */
		if(hw_indexof(pattern, c) != -1)
			*(str + i) = replacement;
	}
}

/**
* Searches a string for '\t' characters and replaces them with spaces.
* @param str A '\0' terminated string to search tabs for.
* @param tabsize The number of spaces to replace a tab with.
* @return A dynamically allocated '\0' terminated string
* If the operation fails it should return NULL.
*/
char* hw_expandtabs(const char *str, size_t tabsize){
	char *result = (char *)malloc(sizeof(char));
	char c;
	/* numBytes will keep track of the size of our result array */
	/* numElements will keep track of the capacity of the array */
	int i = 0, j, numElements = 1, numBytes = sizeof(char);

	/* Ensure malloc did not fail */
	assert(result != NULL);

	while((c = *(str + i)) != '\0'){
		if(c == '\t'){
			/* Attempt to allocate more space for a tab */
			result = (char *) realloc(result, numBytes + tabsize * sizeof(char));
			/* Add spaces at the end of the array */
			for (j = 0; j < tabsize; j++)
				*(result + numElements + j) = ' ';

			/* Update number of elements and bytes*/
			numElements += tabsize;
			numBytes += tabsize * sizeof(char);
		}else{
			/* allocate more space to insert char */
			result = (char *) realloc(result, numBytes + sizeof(char));
			*(result + numElements) = c;

			/* Update num of elements and bytes */
			numElements += 1;
			numBytes += sizeof(char);
		}

		i++;
	}

	return result;
}

/**
* Splits on a character c and stores the pointers to the each separate
* token in the return value.
* @param str String to split.
* @param c Character to search the string and split on.
* @return Returns a 2D array containing the split tokens.
* If the operation fails it should return NULL.
*/
char** hw_split(const char *str, char c){
	int i, length = hw_strlen(str), numTokens = 0;
	char **tokens;

	for(i = 0; i < length; i++){
		/* Check if we reached the splitting char */
		if(*(str + i) == c){
			
		}
	} 

}


