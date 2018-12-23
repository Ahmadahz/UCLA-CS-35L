#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char* argv[])
{
    //ERROR CHECKING
    //if no parameters were passed
    if (argc == 1)
    {
        char* errorMsg = "Error: missing operand\n";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
        exit(1);
    }
    //if only one parameter was passed
    if (argc == 2)
    {
        char* errorMsg = "Error: missing one operand\nTwo strings must be given when translating.\n";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
        exit(1);
    }
    //if more than two parameters were passed
    if (argc > 3)
    {
        char* errorMsg = "Error: extra operand\n";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
        exit(1);
    }
    
    const char* from = argv[1];
    const char* to = argv[2];
    
    int fromLength = strlen(from);
    int toLength = strlen(to);
    
    //throw an error if from and to aren't the same length
    if (fromLength != toLength)
    {
        char* errorMsg = "Error: 'from' and 'to' parameters must have the same length.\n";
        int bytesWritten = write(2, errorMsg, strlen(errorMsg));
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
                char* errorMsg = "Error: 'from' parameter must not have duplicate bytes.\n";
                int bytesWritten = write(2, errorMsg, strlen(errorMsg));
                exit(1);
            }
        }
    }
    
    while(1)
    {
        char* currChar = (char *) malloc(sizeof(char));
        ssize_t bytesRead = read(0, currChar, 1);
        
        if (bytesRead < 0)
        {
            char* errorMsg = "Error reading input.";
            int bytesWritten = write(2, errorMsg, strlen(errorMsg));
            exit(1);
        }
        
        if (bytesRead == 0) break;
        
        char charToOutput = *currChar;
        
        for (int i = 0; i < fromLength; i++)
        {
            //if the current character maps to another character
            if (charToOutput == from[i])
            {
                charToOutput = to[i];
                break;
            }
        }
        
        ssize_t bytesWritten = write(1, &charToOutput, 1);
        
        if (bytesWritten <= 0)
        {
            char* errorMsg = "Error writing output.";
            int bytesWritten = write(2, errorMsg, strlen(errorMsg));
            exit(1);
        }
    }
}
