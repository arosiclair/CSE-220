#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main () { 
     char str1[10]; 
     char str2[] = "AAAAAAAAAAAAAAAAAAAAAAAAAA"; 
      
     strncpy (str1, str2, sizeof(str2)); 
     printf(str1);
}  