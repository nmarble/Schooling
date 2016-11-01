/* 
 * File:   fileSort.c
 * Author: Nicholas Marble
 * Created on February 22, 2015, 11:10 AM
 * Used to resort txt files by line or by word
 * Exit codes: 
 *  1: Invalid arguments 
 *  2: Help Message displayed
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Function to display the help message
void helpMsg() {
    printf("Usage: fileSort [OPTION]... -L SOURCE\n");
    printf("sort SOURCE with OPTION format \n");
    printf("Mandatory arguments: \n");
    printf(" -L%50s\n", "Reverses file line by line");   
    printf(" -W%50s\n", "Reverses file word by word");   
    printf(" -help%46s\n", "Displays this help screen");
}

//Function to reverse the text file line by line
void reverseByLine (char * file) {
    char lines[biggestLine][totalLines];     //Holds all the lines and characters
    
    int lineNumber = 0;         //Tracks what line number you are on
    int index = 0;              //Used to index all inputed characters
    int currentChar = 0;        //Tracks the current character of the input
    int charMax = 0;            //Hold the biggest line length
    
    //Loops through all the inputed characters
    while (file[index] != EOF && file[index]) {
        //Checks for the end of line
        if (file[index] != '\n') {
            //Store a character in the array for later
            lines[currentChar][lineNumber] = file[index];
            //Increase the biggest line value
            if (currentChar > charMax) charMax = currentChar; 
            currentChar++;
        }else { 
            lineNumber++;            
            currentChar = 0;
        }
        index++;
    }
    
    //Loops back through all lines that were stored
    while (lineNumber >= 0) {
        lineNumber--;
        for (int index = 0; index < charMax; index++) {
            printf("%c", lines[index][lineNumber]);
        }
        printf("\n");
    }
}

//Function to reverse the text file word by word
void reverseByWord (char * file) {
    char word[biggestLine][totalLines];              //Hold all the words for current line
    int wordNum = 0;                //Tracks current word
    int index = 0;                  //Used to index all inputed characters
    int currentChar = 0;            //Tracks the current character of the input
    int charMax = 0;                //The total number of characters
    
    memset(word, 0, sizeof(word));  //Sets all values to 0
    
    //Loops through all of input
    while (file[index] != EOF && file[index]) {
        //Checks for a space
        if (file[index] == ' '){
            currentChar = 0;
            wordNum++;
        }else if (file[index] == '\n'){ //At end of line do this
            //Loops back through all saved words
            while (wordNum >= 0) {
                for(currentChar = 0; word[currentChar][wordNum] != 0; currentChar++) {
                    printf("%c", word[currentChar][wordNum]);
                }
                printf(" ");
                wordNum--;
            }
            //Sets all values back to 0
            wordNum = 0;
            memset(word, 0, sizeof(word));
            printf("\n");
        }else {                         //Add character to the word here
            word[currentChar][wordNum] = file[index];
            currentChar++;
        }
        index++;
    } 
}

//Function to get everything from standard in
char * getFile() {
    int bufferSize = 100;                   //Space for memory
    char *buffer = malloc(bufferSize);      //Current memory holder
    int length = 0;                         //Current Size
    int c;                                  //Current Character from input
    int lineSize = 0;
    
    //Loops until end of file
    while ((c = getchar()) != EOF)
    {
        if (c == '\n') {
            totalLines++;
            lineSize = 0;
        }else {
            lineSize++;
            if (lineSize > biggestLine) biggestLine++;
        }
        
        if (length == bufferSize)
        {
            bufferSize *= 2;
            buffer = realloc(buffer, bufferSize);
        }
        buffer[length++] = c;
    }
    totalLines++;
    return buffer;
}
int totalLines = 0;
int biggestLine = 0;
//Main function for program
int main(int argc, char** argv) {
    //Check for help argument
    if (argc == 2 && strcmp(argv[1], "-help") == 0) {
        helpMsg();
        exit(2);
    }else if (argc == 2){
        //Checks if the file is valid to open
        if (strcmp(argv[1], "-L") == 0 || strcmp(argv[1], "-l") == 0) {     //Argument for line by line        
            reverseByLine(getFile());
            exit(0);
        }else if (strcmp(argv[1], "-W") == 0 || strcmp(argv[1], "-w") == 0) {     //Argument for word by word   
            reverseByWord(getFile());
            exit(0);
        }else {
            printf("Invalid arguments try -help\n");//If no valid arguments are found
            exit(1);
        }
    }else {
        printf("Invalid arguments try -help\n");    //Not the correct amount of arguments
    }
    return (EXIT_SUCCESS);
}