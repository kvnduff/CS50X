// Recovers jpg files from forensic card

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // If no input file, return
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open input file
    char *file_name = argv[1];
    FILE *file_pointer = fopen(file_name, "r");

    // If can't open file, return
    if (file_pointer == NULL)
    {
        printf("Error: could not open %s.\n", file_name);
        return 2;
    }

    // Initialize variables
    const int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE];
    int counter = 0;
    FILE *image_pointer = NULL;
    char image_name [8];

    // Write images to output files, repeat until card finished
    while (fread(&buffer, 1, BLOCK_SIZE, file_pointer) == 512)
    {
        // Check buffer for new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If not first JPEG then close last
            if (!(counter == 0))
            {
                fclose(image_pointer);
            }
            // Initialize image
            sprintf(image_name, "%03i.jpg", counter);
            image_pointer = fopen(image_name, "a");
            counter++;
        }
        // Write to file
        if (!(counter == 0))
        {
            fwrite(&buffer, 1, BLOCK_SIZE, image_pointer);
        }
    }
    fclose(file_pointer);
    fclose(image_pointer);
    return 0;
}