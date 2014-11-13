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
void insertReplacement(char *str, const char *replacement, int numBytes, int index);
int checkFindStrEC(const char *str, const char* find_str);

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

	/* Assign each char from src to result */
	for(i = 0; i < n; i++)
		*(dst + i) = *(src + i);

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
	int i = 0, j = 0, length = hw_strlen(str), numTokens = 1, copyIndex;
	char **tokens;

	if(str == NULL)
		return NULL;

	/* Skip initial delims if they're there */
	for(i = 0; str[i] == c; i++);
	/* Ignore trailing delims if they're there */
	for(j = (length - 1); str[j] == c; j--);
	
	/* Count the number of tokens */
	while(i < j){
		if(str[i] == c){
			numTokens++;
			/* Skip contiguous delims */
			while(str[i] == c)
				i++;
		}
		i++;
	}

	/* Allocate enough space for tokens and the string */
	tokens = (char **)malloc((numTokens + 2)*sizeof(char*));
	tokens[numTokens + 1] = (char*)malloc((length + 1)*sizeof(char));
	if(tokens == NULL) return NULL;
	if (tokens[numTokens + 1] == NULL) return NULL;

	copyIndex = numTokens + 1;

	/* Copy the original str to the bottom of our tokens array */
	hw_strncpy(tokens[copyIndex], str, (length + 1));

	/* Skip initial delims and set first token reference */
	for(i = 0; tokens[copyIndex][i] == c; i++);
	j = 0; 
	tokens[j] = &tokens[copyIndex][i];
	j++;
	/* replace delims in the copy with null chars and set pointers */
	while(tokens[copyIndex][i] != '\0'){
		/* check if char is a delim */
		if(tokens[copyIndex][i] == c){
			/* insert null chars */
			while(tokens[copyIndex][i] == c){
				tokens[copyIndex][i] = '\0';
				i++;
			}
			/* set next token pointer */
			if(tokens[copyIndex][i] != '\0')
				tokens[j] = &tokens[copyIndex][i];
			j++;
		}else
			i++;
	}

	/* Handle special case where only delims were passed */
	if(tokens[0][0] == '\0'){
		tokens[0] = NULL;
	}
	/* null terminate the tokens */
	tokens[j] = NULL;

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
	int findLength, replaceLength, newLength, i = 0, j = 0, numReplacements = 0;
	char c;

	if(find_str == NULL || replace_str == NULL)
		return 0;

	findLength = hw_strlen(find_str);
	replaceLength = hw_strlen(replace_str);

	/* Count the number of replacements */
	while((c = str[i]) != '\0'){
		/* Check if c is the first char of find_str and then verify it is the string we are looking for*/
		if(c == *find_str && checkFindStr((str + i), find_str)){
				numReplacements++;
		}
		i++;
	}

	/* Calculate effective length of new string */
	newLength = hw_strlen(str) - numReplacements*findLength + numReplacements*replaceLength + 1;

	/* Allocate space for our newly modified str */
	*dst = (char *)malloc(newLength*sizeof(char));
	if(dst == NULL) return 0;

	i = 0;
	j = 0;
	/* Loop through str again, insert replace_str where necessary */
	while((c = str[i]) != '\0'){
		/* insert the replacement instead */
		if(c == *find_str && checkFindStr((str + i), find_str)){
			hw_strncpy((*dst + j), replace_str, replaceLength);
			i += findLength;
			j += replaceLength;
		}else{
			/* Other wise just copy the character */
			dst[0][j] = str[i];
			i++;
			j++;
		}

	}
	
	/* Null terminate */
	dst[0][j] = '\0';
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

void insertReplacement(char *str, const char *replacement, int numBytes, int index){
	int length = hw_strlen(replacement), i;

	/* Allocate more space for the replacement string */
	str = (char *)realloc(str, numBytes + length*sizeof(char));
	/* assert(str != NULL); */
	/* Insert the chars of replacement into the string */
	for(i = 0; i < length; i++){
		*(str + index + i) = *(replacement + i);
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
	int k = 0, l = 0, m = 0, n = 0, numTokens = 1, length = hw_strlen(str);
	char **tokens, *copy, *result;

	/* Do nothing if null references were passed */
	if (str == NULL || delimiters == NULL)
		return;
	if (i < 0 || j < 0)
		return;

	/* Skip initial delims if they're there */
	for(k = 0; hw_indexof(delimiters, str[k]) != -1; k++);
	/* Ignore trailing delims if they're there */
	for(l = (length - 1); hw_indexof(delimiters, str[l]) != -1; l--);

	/* Count the number of tokens */
	while(k < l){
		if(hw_indexof(delimiters, str[k]) != -1){
			numTokens++;
			/* Skip contiguous delims */
			while(hw_indexof(delimiters, str[k]) != -1)
				k++;
		}
		k++;
	}

	/* Validate i and j */
	if(i >= numTokens || j >= numTokens)
		return;

	/* Allocate space for token array */
	tokens = (char **)malloc(numTokens*sizeof(char*));
	if(tokens == NULL) return;

	/* Allocate space for copy */
	copy = (char *)malloc((length + 1)*sizeof(char));
	if(copy == NULL) return;

	/*Copy input str to copy*/
	copy = hw_strncpy(copy, str, (length + 1));

	/* skip initial delims, set first token pointer */
	for(k = 0; hw_indexof(delimiters, copy[k]) != -1; k++);
	tokens[n] = &copy[k];
	n++;

	/* Tokenize the rest of copy */
	while(k < length){
		/* check if char is one of the delims */
		if(hw_indexof(delimiters, copy[k]) != -1){
			/* insert null chars */
			while(hw_indexof(delimiters, copy[k]) != -1){
				copy[k] = '\0';
				k++;
			}
			/* set next token reference */
			if(copy[k] != '\0')
				tokens[n] = &copy[k];
			n++;
		}else
			k++;
	}

	/* allocate space for the result */
	result = (char *)malloc((length + 1)*sizeof(char));
	if (result == NULL) return;
	/* insert any intial delims */
	for(k = 0; hw_indexof(delimiters, str[k]) != -1; k++)
		result[k] = str[k];
	m = k; /* m is index for original str */

	/* insert each token and the following delims from orig str*/
	for(l = 0; l < numTokens; l++){
		/* handle swap conditions */
		if(l == i){
			/* copy token j into result instead */
			hw_strncpy(&result[k], tokens[j], hw_strlen(tokens[j]));
			k += hw_strlen(tokens[j]);
			m += hw_strlen(tokens[l]);
		}else if(l == j){
			/* copy token i into result instead */
			hw_strncpy(&result[k], tokens[i], hw_strlen(tokens[i]));
			k += hw_strlen(tokens[i]);
			m += hw_strlen(tokens[l]);
		}else{
			hw_strncpy(&result[k], tokens[l], hw_strlen(tokens[l]));
			k += hw_strlen(tokens[l]);
			m += hw_strlen(tokens[l]);
		}

		/* insert the delims following the token */
		while(hw_indexof(delimiters, str[m]) != -1 && str[m] != '\0'){
			result[k] = str[m];
			m++;
			k++;
		}
	}

	/* copy our result into our original str */
	hw_strncpy(str, result, length);

}

int ec_findAndReplace(char **dst, const char *str, const char *find_str, const char* replace_str){
	int findLength, replaceLength, newLength, i = 0, j = 0, numReplacements = 0;
	char c;

	if(find_str == NULL || replace_str == NULL)
		return 0;

	findLength = hw_strlen(find_str);
	replaceLength = hw_strlen(replace_str);

	/* Count the number of replacements */
	while((c = str[i]) != '\0'){
		/* Check if c is the first char of find_str and then verify it is the string we are looking for*/
		if((c == *find_str || *find_str == '*') && checkFindStrEC((str + i), find_str)){
				numReplacements++;
		}
		i++;
	}

	/* Calculate effective length of new string */
	newLength = hw_strlen(str) - numReplacements*findLength + numReplacements*replaceLength + 1;

	/* Allocate space for our newly modified str */
	*dst = (char *)malloc(newLength*sizeof(char));
	if(dst == NULL) return 0;

	i = 0;
	j = 0;
	/* Loop through str again, insert replace_str where necessary */
	while((c = str[i]) != '\0'){
		/* insert the replacement instead */
		if((c == *find_str || *find_str == '*') && checkFindStrEC((str + i), find_str)){
			hw_strncpy((*dst + j), replace_str, replaceLength);
			i += findLength;
			j += replaceLength;
		}else{
			/* Other wise just copy the character */
			dst[0][j] = str[i];
			i++;
			j++;
		}

	}
	
	/* Null terminate */
	dst[0][j] = '\0';
	return numReplacements;

}

int checkFindStrEC(const char *str, const char* find_str){
	int findLength = hw_strlen(find_str);
	int i;

	/* Verify each char in str matches find_str */
	for(i = 0; i < findLength; i++){
		/* Do not compare against '*' chars in find_str */
		if(str[i] != ' ' && find_str[i] == '*') 
			continue;
		if(*(str + i) != *(find_str + i))
			return 0;
	}

	return 1;
}