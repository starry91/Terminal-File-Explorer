#include "CommandHandler.h"

void CommandHandler::copyFile(std::string file, std::string dir)
{
    FILE *from, *to;
    char ch;
    dest_file = dir + "/" + 

    /* open source file */
    if ((from = fopen(file.c_str(), "rb")) == NULL)
    {
        printf("Cannot open source file.\n");
        exit(1);
    }

    /* open destination file */
    if ((to = fopen(argv[2], "wb")) == NULL)
    {
        printf("Cannot open destination file.\n");
        exit(1);
    }

    /* copy the file */
    while (!feof(from))
    {
        ch = fgetc(from);
        if (ferror(from))
        {
            printf("Error reading source file.\n");
            exit(1);
        }
        if (!feof(from))
            fputc(ch, to);
        if (ferror(to))
        {
            printf("Error writing destination file.\n");
            exit(1);
        }
    }

    if (fclose(from) == EOF)
    {
        printf("Error closing source file.\n");
        exit(1);
    }

    if (fclose(to) == EOF)
    {
        printf("Error closing destination file.\n");
        exit(1);
    }
};