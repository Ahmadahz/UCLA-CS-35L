#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkForInputError();
void checkForOutputError();

int main(int argc, const char* argv[])
{
    //ERROR CHECKING
    //if no parameters were passed
    if (argc == 1)
    {
        fprintf(stderr, "Error: missing operand\n");
        exit(1);
    }
    //if only one parameter was passed
    if (argc == 2)
    {
        fprintf(stderr, "Error: missing operand after '%s'\nTwo strings must be given when translating.\n", argv[1]);
        exit(1);
    }
    //if more than two parameters were passed
    if (argc > 3)
    {
        fprintf(stderr, "Error: extra operand '%s'\n", argv[3]);
        exit(1);
    }
    
    const char* from = argv[1];
    const char* to = argv[2];
    
    int fromLength = strlen(from);
    int toLength = strlen(to);
    
    //throw an error if from and to aren't the same length
    if (fromLength != toLength)
    {
        fprintf(stderr, "Error: 'from' and 'to' parameters must have the same length.\n");
        exit(1);
    }
    
    //throw an error if from has any duplicate bites
    for (int i = 0; i < fromLength; i++)
    {
        const char currChar = from[i];
        
        for (int j = i + 1; j < fromLength; j++)
        {
            //if from has a duplicate bytes
            if (currChar == from[j])
            {
                fprintf(stderr, "Error: 'from' parameter must not have duplicate bytes.\n");
                exit(1);
            }
        }
    }
    
    int currChar = getchar();
    checkForInputError();
    
    while(!feof(stdin))
    {
        char charToOutput = currChar;
        for (int i = 0; i < fromLength; i++)
        {
            //if the current character maps to another character
            if (currChar == from[i])
            {
                charToOutput = to[i];
                break;
            }
        }
        
        putchar(charToOutput);
        checkForOutputError();
        
        currChar = getchar();
        checkForInputError();
    }
}

void checkForInputError()
{
    if (ferror(stdin) != 0)
    {
        fprintf(stderr, "Error reading input.");
        exit(1);
    }
}

void checkForOutputError()
{
    if (ferror(stdout) != 0)
    {
        fprintf(stderr, "Error writing output.");
        exit(1);
    }
}
