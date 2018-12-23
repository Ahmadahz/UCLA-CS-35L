#include <stdio.h>
#include <stdlib.h>

//for syscalls
#include <unistd.h>

//for strlen
#include <string.h>

//for fstat
#include <sys/types.h>
#include <sys/stat.h>

//for toupper
#include <ctype.h>

int frobcmp(char const * a, char const * b);

int generalCmp(const void * a, const void * b);

//global variable to track if f flag is passed
int fflag = 0;
int isRegularFile = 0;

//global variable to check if the last

int main(int argc, char* argv[])
{
    //f flag
    if (argc == 2)
    {
        char* flagParameter = "-f";
        if (strcmp(argv[1], flagParameter) == 0)
        {
            fflag = 1;
        }
        else //invalid parameter
        {
            char* errorMsg = "Invalid option.";
            int bytesWritten = write(2, errorMsg, strlen(errorMsg));
            exit(1);
        }
    }
    else if (argc > 2)
    {
        char* errorMsg = "Too many parameters passed.";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
        exit(1);
    }
    
    struct stat fileInfo;
    int fstatStatus = fstat(0, &fileInfo);
    
    //error checking
    if (fstatStatus < 0)
    {
        char* errorMsg = "Failure executing fstat command.";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
        exit(1);
    }
    
    if (S_ISREG(fileInfo.st_mode))
    {
        isRegularFile = 1;
    }
    
    size_t fileSize = fileInfo.st_size;
    
    //use a pointer to a pointer as an array of strings
    char ** arrayOfWords = (char **) malloc(sizeof(char *));
    int currentWordPosition = 0;
    
    char* fileInput;

    if (isRegularFile == 1)
    {
        fileInput = (char *) malloc(fileSize * sizeof(char));
        int position = 0;
        ssize_t bytesRead = read(0, fileInput, fileSize + 1);
        
        if (bytesRead < 0)
        {
            char* errorMsg = "Error reading input.";
            int bytesWritten = write(2, errorMsg, strlen(errorMsg));
            exit(1);
        }
        
        int charCount = 0;
        
        for (size_t i = 0; i < fileSize; i++)
        {
            //for the beginning of the word
            if (i == 0)
            {
                if (fileInput[0] != ' ')
                {
                    charCount++;
                }
            }

            //end with a space
            if (i == fileSize - 1 && fileInput[i] != ' ')
            {
                char* temp = realloc(fileInput, (fileSize + 1) * sizeof(char));
                if (temp == NULL)
                {
                    char* errorMsg = "Error allocating memory.";
                    int bytesWritten = write(2, errorMsg, strlen(errorMsg));
                    exit(1);
                }
                
                fileInput = temp;
                
                fileSize++;
                fileInput[fileSize - 1] = ' ';
                break;
            }
            
            if (fileInput[i] == ' ')
            {
                while (fileInput[i] == ' ' && i < fileSize)
                {
                    i++;
                }
                if (i < fileSize)
                {
                    charCount++;
                }
            }
        }
        
        arrayOfWords = (char **) malloc(charCount * sizeof(char*));
        
        int addedToArray = 0;
        for (size_t i = 0; i < fileSize; i++)
        {
            if (addedToArray == 0 && fileInput[i] != ' ')
            {
                arrayOfWords[currentWordPosition] = &fileInput[i];
                addedToArray = 1;
                currentWordPosition++;
            }
            else if (addedToArray == 1 && fileInput[i] == ' ')
            {
                addedToArray = 0;
            }
        }
    }
    ////////////////////////////////////////END OF IF REGULAR FILE///////////////////////////////////
    
    ssize_t currentBytesRead;
    ssize_t nextBytesRead;
    
    //get the current character from stdin
    char * currentChar = (char *) malloc(sizeof(char));
    currentBytesRead = read(0, currentChar, 1);
    
    if (currentBytesRead < 0)
    {
        char* errorMsg = "Error reading input.";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
        exit(1);
    }
    
    //check for leading spaces
    if (currentBytesRead > 0)
    {
        while (*currentChar == ' ')
        {
            currentChar = NULL;
            currentChar = (char *) malloc(sizeof(char));
            currentBytesRead = read(0, currentChar, 1);
            
            if (currentBytesRead < 0)
            {
                char* errorMsg = "Error reading input.";
                int bytesWritten = write(2, errorMsg, strlen(errorMsg));
                exit(1);
            }
        }
    }
    
    //we also want to get the next one to check for extra spaces
    //or the end of the input
    char * nextChar = (char *) malloc(sizeof(char));
    nextBytesRead = read(0, nextChar, 1);
    
    if (nextBytesRead < 0)
    {
        char* errorMsg = "Error reading input.";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
        exit(1);
    }

    //the current word
    char * currentWord = (char *) malloc(sizeof(char));
    int currentLetterPosition = 0;
    
    ////////////////////////////////////////////////START OF WHILE LOOP/////////////////////////
    while (!feof(stdin) && currentBytesRead > 0)
    {
        int dontIncrementLetter = 0;
        
        currentWord[currentLetterPosition] = *currentChar;
        
        char * wordWithMoreSpace = realloc(currentWord, (currentLetterPosition + 2) * sizeof(char));
        
        //error allocating memory
        if (wordWithMoreSpace == NULL)
        {
            free(currentWord);
            char* errorMsg = "Error allocating memory.";
            int bytesWritten = write(2, errorMsg, strlen(errorMsg));
            exit(1);
        }
        else
        {
            currentWord = wordWithMoreSpace;
        }
     
        //if we've reached the end of the current word
        if (*currentChar == ' ')
        {
            //add the current word to the array of words
            arrayOfWords[currentWordPosition] = currentWord;
            
            char ** wordsWithMoreSpace = realloc(arrayOfWords, (currentWordPosition + 2) * sizeof(char*));
            
            //error allocating more memory
            if (wordsWithMoreSpace == NULL)
            {
                free(arrayOfWords);
                char* errorMsg = "Error allocating more memory.";
                int bytesWritten = write(2, errorMsg, strlen(errorMsg));
                exit(1);
            }
            else
            {
                arrayOfWords = wordsWithMoreSpace;
                currentWordPosition++;
                
                //reset the currentWord
                currentLetterPosition = 0;
                dontIncrementLetter = 1;
                currentWord = NULL;
                currentWord = (char *) malloc(sizeof(char));
            }
            
            //treat multiple spaces like one
            if (*nextChar == ' ')
            {
                while (*nextChar == ' ')
                {
                    nextChar = NULL;
                    nextChar = (char *) malloc(sizeof(char));
                    nextBytesRead = read(0, nextChar, 1);
                    
                    if (nextBytesRead < 0)
                    {
                        char* errorMsg = "Error reading input.";
                        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
                        exit(1);
                    }
                }
            }
        }
        
        if (dontIncrementLetter == 0)
        {
            currentLetterPosition++;
        }
        
        *currentChar = *nextChar;
        
        nextChar = NULL;
        nextChar = (char *) malloc(sizeof(char));
        nextBytesRead = read(0, nextChar, 1);
        
        if (nextBytesRead < 0)
        {
            char* errorMsg = "Error reading input.";
            int bytesWritten = write(2, errorMsg, strlen(errorMsg));
            exit(1);
        }
        
        //empty input
        if (nextBytesRead == 0)
        {
            break;
        }
    }   //////////////////////////////////END OF WHILE LOOP////////////////////////////////////////
    
    //if there's no space at the end
    if (currentLetterPosition > 0 && currentWord[currentLetterPosition - 1] != ' ')
    {
        currentWord[currentLetterPosition] = *currentChar;
        
        currentLetterPosition++;
        
        if (*currentChar != ' ')
        {
            currentWord[currentLetterPosition] = ' ';
        }
        
        arrayOfWords[currentWordPosition] = currentWord;
        currentWordPosition++;
    }
    
    //function pointer to the general compare function
    int (* functionPtr) (const void *, const void *) = &generalCmp;
    
    //pass the function pointer as a parameter to qsort, and sort the words
    qsort(arrayOfWords, currentWordPosition, sizeof(char*), functionPtr);
    
    //output the words
    size_t i;
    for (i = 0; i < currentWordPosition; i++)
    {
        size_t j = 0;
        long wordLength = 0;
        while (1)
        {
            if (j == strlen(arrayOfWords[i]))
            {
                break;
            }
            
            wordLength++;
            char charToOutput = arrayOfWords[i][j];
            
            if (charToOutput == ' ')
            {
                break;
            }
            j++;
        }
        
        ssize_t bytesWritten = write(1, arrayOfWords[i], wordLength);
        
        if (bytesWritten <= 0)
        {
            char* errorMsg = "Error writing output.";
            int bytesWritten = write(2, errorMsg, strlen(errorMsg));
            exit(1);
        }
    }

    free(arrayOfWords);
    return 0;
}

int frobcmp(char const * a, char const * b)
{
    while (*a != ' ' && *b != ' ')
    {
        if (*a == ' ') return -1;
        if (*b == ' ') return 1;
        
        char frobA = *a ^ 42;
        char frobB = *b ^ 42;
        
        //if the f option was passed
        if (fflag == 1)
        {
            frobA = toupper((unsigned char) frobA);
            frobB = toupper((unsigned char) frobB);
        }
        
        if (frobA < frobB) return -1;
        if (frobA > frobB) return 1;
        
        a++;
        b++;
    }
    
    //if we finish the while loop w/o returning, then they're equal
    return 0;
}

int generalCmp(const void * a, const void * b)
{
    return frobcmp(* (const char**) a, * (const char**) b);
}
