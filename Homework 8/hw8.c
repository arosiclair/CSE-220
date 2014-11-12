/*
Homework #8
name: Andrew Rosiclair
SBU ID: 109235970
*/

#include "hw8.h"
#include <assert.h>
#include <ctype.h>

/* Function Prototypes */
int isALetter(char c);
int checkFindStr(const char *str, const char* find_str);
void insertReplacement(char *str, const char *replacement, int numBytes);
void insertToken(char *result, int index, char **tokens, int token, const char *delimiters);

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
	char *result = (char *)malloc(sizeof(char));

	/* Return NULL if either of the strings are NULL */
	if(dst == NULL || src == NULL)
		return NULL;

	/* Assign each char from src to result */
	for(i = 0; i < n; i++){
		/* allocate more space in result */
		result = realloc(result, (i + 1)*sizeof(char));
		*(result + i) = *(src + i);
	}
	/* Null terminate */
	result = realloc(result, (i + 1)*sizeof(char));
	*(result + i) = '\0';
	dst = result;

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
	unsigned char c1, c2;

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
				if(c1 != c2 && toupper(c1) != c2 && c1 != toupper(c2))
					return 0;
			}
			/* Second char isn't a letter so just compare */
			else if(c1 != c2)
				return 0;
		}
		/* First char isn't a letter so just compare */
		else if(c1 != c2)
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
		i++;
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
	/* numElements will keep track of the number of chars */
	int i = 0, j, numElements = 0, numBytes = 0;

	/* Do nothing if any null parameters */
	if(str == NULL)
		return NULL;
	
	/* Ensure malloc did not fail */
	if(result == NULL) return NULL;

	/* Analyze each char of the input string */
	while((c = *(str + i)) != '\0'){
		if(c == '\t'){
			/* alloc more space */
			result = (char *) realloc(result, numBytes + tabsize * sizeof(char));
			if(result == NULL) return NULL;
			/* Add spaces at the end of the array */
			for (j = 0; j < tabsize; j++)
				*(result + numElements + j) = ' ';

			/* Update number of elements and bytes*/
			numElements += tabsize;
			numBytes += tabsize * sizeof(char);
		}else{
			/* allocate more space to insert non-tab char */
			result = (char *) realloc(result, numBytes + sizeof(char));
			if(result == NULL) return NULL;
			*(result + numElements) = c;

			/* Update num of elements and bytes */
			numElements++;
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
	int i = 0, j = 0, length = hw_strlen(str), numTokens = 0, numBytes = sizeof(char);
	char *copy = (char *)malloc(length*sizeof(char));
	/* Ensure realloc didn't fail */
	if(copy == NULL) return NULL; 
	char **tokens = (char **)malloc(sizeof(char*));
	/* Ensure realloc didn't fail */
	if(tokens == NULL) return NULL;

	if(str == NULL)
		return NULL;

	/* Set the initial reference for tokens */
	*tokens = copy;
	numTokens++;
	/* Copy the string and look for splitting chars */
	while(i < length){
		/* Check if we reached a splitting char */
		if(*(str + i) == c){
			/* Insert a null terminator to split the copy */
			*(copy + j) = '\0';
			/* Allocate space for another token */
			tokens = (char **)realloc(tokens, (numTokens + 1)*sizeof(char*));
			/* Ensure realloc didn't fail */
			if(tokens == NULL) return NULL;
			/* Set a reference for the next token */
			*(tokens + numTokens) = copy + j + 1;
			/*Skip any additional delim chars*/
			while(*(str +i) == c)
				i++;
			j++;
			numTokens++;
			numBytes += 2*sizeof(char);
		}else{
			/* Not a splitting char so we just insert it into the copy */
			*(copy + j) = *(str + i);
			numBytes += sizeof(char);
			i++;
			j++;
		}
	}

	/* Allocate space for null terminating token */
	tokens = (char **)realloc(tokens, (numTokens + 1)*sizeof(char*));
	/* Ensure realloc didn't fail */
	if(tokens == NULL) return NULL;
	*(tokens + numTokens) = NULL;

	return tokens;
}

/**
* Find and replace all occurrences of find_str in str with replace_str.
Note find_str
* and replace_str can be of varying lengths, and not necessarily the same
length.
* @param str A '\0' character terminated string
* @param find_str A '\0' character terminated string to search for
* @param replace_str A '\0' character terminated string to replace with
* @param dst Place to store new string.
* @return Returns the number of find_str's replaced
*/
int hw_findAndReplace(char **dst, const char *str, const char *find_str, const char* replace_str){
	int findLength, replaceLength, i = 0, numBytes = 0, numReplacements = 0;
	char *copy = (char *)malloc(sizeof(char)), c;

	if(find_str == NULL || replace_str == NULL)
		return 0;

	findLength = hw_strlen(find_str);
	replaceLength = hw_strlen(replace_str);

	/* Loop through each char in the input string */
	while((c = *(str + i)) != '\0'){
		/* Check if c is the first char of find_str and then verify it is the string we are looking for*/
		if(c == *find_str && checkFindStr((str + i), find_str)){
				/* Insert the replacement */
				insertReplacement((copy + i), replace_str, numBytes);
				numBytes += replaceLength*sizeof(char);
				i += findLength;
				numReplacements++;
				continue;
		}else{
			/* we did not find the desired string so we just insert char */
			copy = (char *)realloc(copy, numBytes + sizeof(char));
			assert(copy != NULL);
			*(copy + i) = *(str + i);
			numBytes += sizeof(char);
			i++;
		}
	}
	/* Reassign str to our copy and return the number of replacements */
	str = copy;
	return numReplacements;

}



int checkFindStr(const char *str, const char* find_str){
	int findLength = hw_strlen(find_str);
	int i;

	/* Verify each char in str matches find_str */
	for(i = 0; i < findLength; i++){
		if(*(str + i) != *(find_str + i))
			return 0;
	}

	return 1;
}

void insertReplacement(char *str, const char *replacement, int numBytes){
	int length = hw_strlen(replacement), i;

	/* Allocate more space for the replacement string */
	str = (char *)realloc(str, numBytes + length*sizeof(char));
	assert(str != NULL);
	/* Insert the chars of replacement into the string */
	for(i = 0; i < length; i++){
		*(str + i) = *(replacement + i);
	}
}

/**
* Split str into tokens, where each token is delimited by any of the
characters given
* in the delimiter string. Swap the i-th indexed token with the j-thindexed token
* within the string, assuming the indexes start with 0. Parameter str
should not be
* physically tokenized, meaning is should be still printable as a single
string.
* @param str A '\0' character terminated string
* @param i The first index of the token to swap
* @param j The second index of the token to swap
* @param delimiter Array of characters which should be considered
delimiters
*/
void hw_swapTokens(char *str, size_t i, size_t j, const char
*delimiters){
	return;
	int k = 0, l = 0, index = 0, numTokens = 0, length;
	char **tokens = (char **)malloc(sizeof(char)), *result, c;

	/* Do nothing if null references were passed */
	if (str == NULL || delimiters == NULL)
		return;

	length = hw_strlen(str);
	if(i < 0 || i >= length)
		return;
	if(j < 0 || j >= length)
		return;

	/* Set initial pointer for first token */
	*(tokens) = str;
	numTokens++;
	/* Loop through each char in the string and set references for tokens */
	while((c = *(str + k)) != '\0'){
		/* Check if sthe char is a delimter */
		if(hw_indexof(delimiters, c) != -1){
			/* Find the next non delimeter char */
			while(hw_indexof(delimiters, *(str + k)) != -1 && *(str + k) != '\0')
				k++;
			/* Set a reference for the next token */
			*(tokens + numTokens) = (str + i);
			numTokens++;
			i++;
		}else
			i++;
	}

	/* Build the resultant string */
	for(l = 0; l < numTokens; l++){
		/* Check if this is a token that needs to be swapped */
		if(l == i || l == j){

		}
		/* Insert token */
		insertToken(result, index, tokens, l, delimiters);
	}
}

void insertToken(char *result, int index, char **tokens, int token, const char *delimiters){
	int i = 0;
	char c;

	/* Insert the token into the result including the following delim chars */
	while((*(tokens + token) + i) != (*(tokens + token + 1))){
		c = *(*(tokens + token) + i);
		/* Insert the token char into our result */
		*(result + index) = c;
		index++;
		i++;
	}


}

int ec_findAndReplace(char **dst, const char *str, const char *find_str, const char* replace_str){
	return 0;
}