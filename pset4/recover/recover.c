#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
    // open file argument 
    FILE *f = fopen(argv[1], "r");
   
    
    // auxiliary array to hold read contents, and number of jpegs so far
    BYTE aux[512];
    int jpegs = 0;
    
    //repeat until false
    while (fread(aux, 1, 512, f) == 512)
    {      
        
        //if aux begins with jpeg header
        if ((aux[0] == 0xff) && (aux[1] == 0xd8) && (aux[2] == 0xff) && ((aux[3] & 0xf0) == 0xe0))  // if start of jpeg
        {
            if (jpegs == 0)         // if first jpeg
            {
                char *newfile = malloc(8);              //create space for filename
                sprintf(newfile, "%03i.jpg", jpegs);    //create filename based on jpeg no.
                jpegs++;                                //increment jpeg no.
                
                FILE *img = fopen(newfile, "w");        //create pointer to file with filename
                fwrite(aux, 1, 512, img);           // write first 512 B block to new img file

                fclose(img);
                free(newfile);
            }
            else        // if not first jpeg
            {
                
                char *newfile = malloc(8);              //make space for new filename
                sprintf(newfile, "%03i.jpg", jpegs);     //create filename  
                jpegs++;
                
                FILE *img = fopen(newfile, "w");
                fwrite(aux, 1, 512, img);

                fclose(img);
                free(newfile);
            }
        }
        
        else        // first 4 B are not header, either write next 512 onto file or move onto next 512 
        {
            if (jpegs == 0)
            {
            
            }
            else
            {
                char *newfile = malloc(8);
                sprintf(newfile, "%03i.jpg", jpegs - 1);
                FILE *img = fopen(newfile, "a");
                fwrite(aux, 1, 512, img);
                fclose(img);

                free(newfile);
            }
        }
        
    }
    fclose(f);
}