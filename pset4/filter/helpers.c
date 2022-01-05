#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++) //loop through rows
    {
        for (int j = 0; j < width; j++) //loop through each element of each row
        {
            int r = image[i][j].rgbtRed;
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            
            int average = round(round(r + b + g) / 3);
            
            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)     //loop through each element of each row
        {
            int r = image[i][j].rgbtRed;
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            
            int sepiaRed = round((0.393 * r) + (0.769 * g) + (0.189 * b));      //use formula
            
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            
            int sepiaGreen = round((0.349 * r) + (0.686 * g) + (0.168 * b));
            
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            
            int sepiaBlue = round((0.272 * r) + (0.534 * g) + (0.131 * b));
            
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width]; 
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            aux[i][j] = image[i][j];
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = aux[i][width - j - 1];
        }
    }
    
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width]; 
    
    // create an auxiliary array, copying the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)     //loop through each element of each row
        {
            aux[i][j] = image[i][j];
        }
    }
    
    //blur any pixel touching nine others on the auxiliary array
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width; j++)
        {
            int r = round(round(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 9);
            int b = round(round(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 9);
            int g = round(round(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 9);

            aux[i][j].rgbtRed = r;
            aux[i][j].rgbtBlue = b;
            aux[i][j].rgbtGreen = g;
            
        }
    }
    
    //top row, excluding first and last
    for (int i = 0; i < 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int r = round(round(image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6);
            int b = round(round(image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6);
            int g = round(round(image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6);

            aux[i][j].rgbtRed = r;
            aux[i][j].rgbtBlue = b;
            aux[i][j].rgbtGreen = g;
            
        }
    }
    
    //bottom row, excluding first and last
    for (int i = height - 1; i < height; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int r = round(round(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed) / 6);
            int b = round(round(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue) / 6);
            int g = round(round(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen) / 6);

            
            aux[i][j].rgbtRed = r;
            aux[i][j].rgbtBlue = b;
            aux[i][j].rgbtGreen = g;
            
        }
    }
    
    //left hand side 
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            int r = round(round(image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed) / 6);
            int b = round(round(image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue) / 6);
            int g = round(round(image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen) / 6);

            aux[i][j].rgbtRed = r;
            aux[i][j].rgbtBlue = b;
            aux[i][j].rgbtGreen = g;
            
        }
    }    

    //right hand side
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = width - 1; j < width; j++)
        {
            int r = round(round(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed) / 6);
            int b = round(round(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue) / 6);
            int g = round(round(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen) / 6);

            aux[i][j].rgbtRed = r;
            aux[i][j].rgbtBlue = b;
            aux[i][j].rgbtGreen = g;
            
        }
    }
    //top left
    int rtl = round(round(image[0][0].rgbtRed + image[0][1].rgbtRed + image[1][1].rgbtRed + image[1][0].rgbtRed) / 4);
    int btl = round(round(image[0][0].rgbtBlue + image[0][1].rgbtBlue + image[1][1].rgbtBlue + image[1][0].rgbtBlue) / 4);
    int gtl = round(round(image[0][0].rgbtGreen + image[0][1].rgbtGreen + image[1][1].rgbtGreen + image[1][0].rgbtGreen) / 4);

    aux[0][0].rgbtRed = rtl;
    aux[0][0].rgbtBlue = btl;
    aux[0][0].rgbtGreen = gtl;
    
    //top right
    int rtr = round(round(image[0][width - 1].rgbtRed + image[0][width - 2].rgbtRed + image[1][width - 1].rgbtRed + image[1][width - 2].rgbtRed) / 4);
    int btr = round(round(image[0][width - 1].rgbtBlue + image[0][width - 2].rgbtBlue + image[1][width - 1].rgbtBlue + image[1][width - 2].rgbtBlue) / 4);
    int gtr = round(round(image[0][width - 1].rgbtGreen + image[0][width - 2].rgbtGreen + image[1][width - 1].rgbtGreen + image[1][width - 2].rgbtGreen) / 4);

    aux[0][width - 1].rgbtRed = rtr;
    aux[0][width - 1].rgbtBlue = btr;
    aux[0][width - 1].rgbtGreen = gtr;
    
    //bottom left
    int rbl = round(round(image[height - 1][0].rgbtRed + image[height - 1][1].rgbtRed + image[height - 2][1].rgbtRed + image[height - 2][0].rgbtRed) / 4);
    int bbl = round(round(image[height - 1][0].rgbtBlue + image[height - 1][1].rgbtBlue + image[height - 2][1].rgbtBlue + image[height - 2][0].rgbtBlue) / 4);
    int gbl = round(round(image[height - 1][0].rgbtGreen + image[height - 1][1].rgbtGreen + image[height - 2][1].rgbtGreen + image[height - 2][0].rgbtGreen) / 4);

    aux[height - 1][0].rgbtRed = rbl;
    aux[height - 1][0].rgbtBlue = bbl;
    aux[height - 1][0].rgbtGreen = gbl;
    
    //bottom right
    int rbr = round(round(image[height - 1][width - 1].rgbtRed + image[height - 1][width - 2].rgbtRed + image[height - 2][width - 1].rgbtRed + image[height - 2][width - 2].rgbtRed) / 4);
    int bbr = round(round(image[height - 1][width - 1].rgbtBlue + image[height - 1][width - 2].rgbtBlue + image[height - 2][width - 1].rgbtBlue + image[height - 2][width - 2].rgbtBlue) / 4);
    int gbr = round(round(image[height - 1][width - 1].rgbtGreen + image[height - 1][width - 2].rgbtGreen + image[height - 2][width - 1].rgbtGreen + image[height - 2][width - 2].rgbtGreen) / 4);

    aux[height - 1][width - 1].rgbtRed = rbr;
    aux[height - 1][width - 1].rgbtBlue = bbr;
    aux[height - 1][width - 1].rgbtGreen = gbr;
    
    //copy aux array onto main image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = aux[i][j];
        }
    }
    return;
}
