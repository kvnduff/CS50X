// Filter functions

#include "helpers.h"
#include <stdio.h>
#include <getopt.h>
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through rows
    for (int i = 0; i < height; i++)
    {
        // Iterate through columns
        for (int j = 0; j < width; j++)
        {
            float rgbt_avg = round(((float) image[i][j].rgbtBlue + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = rgbt_avg;
            image[i][j].rgbtGreen = rgbt_avg;
            image[i][j].rgbtRed = rgbt_avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
	//Create buffer for swap
	RGBTRIPLE(*buffer)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (buffer == NULL)
    {
        return;
    }
    // Copy original RGB values to buffer
    // Iterate through rows
    for (int i = 0; i < height; i++)
    {
        // Iterate through columns
        for (int j = 0; j < width; j++)
        {
            buffer[i][j].rgbtBlue = image[i][j].rgbtBlue;
            buffer[i][j].rgbtGreen = image[i][j].rgbtGreen;
            buffer[i][j].rgbtRed = image[i][j].rgbtRed;
		}
	}
	// Swap RGB values along x-axis
    // Iterate through rows
    for (int i = 0; i < height; i++)
    {
        // Iterate through columns
        for (int j = 0; j < width; j++)
        {
            buffer[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            buffer[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            buffer[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
		}
	}
	// Free memory for buffer
	free(buffer);
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
	//Create buffer for swap
	RGBTRIPLE(*buffer)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (buffer == NULL)
    {
        return;
    }
    // Copy original RGB values to buffer
    // Iterate through rows
    for (int i = 0; i < height; i++)
    {
        // Iterate through columns
        for (int j = 0; j < width; j++)
        {
            buffer[i][j].rgbtBlue = image[i][j].rgbtBlue;
            buffer[i][j].rgbtGreen = image[i][j].rgbtGreen;
            buffer[i][j].rgbtRed = image[i][j].rgbtRed;
		}
	}
	// Blur images according to blur formula
    // Iterate through rows
    for (int i = 0; i < height; i++)
    {
        // Iterate through columns
        for (int j = 0; j < width; j++)
        {
			// Create variables to sum rgb values
			float blue = 0;
			float green = 0;
			float red = 0;
			float counter = 0;
			// Iterate through adjacent rows
			for (int k = -1; k < 2; k++)
			{
				// Iterate through adjacent columns
				for (int l = -1; l < 2; l++)
				{
					// Check if row exists
					if (i + k < 0 || i + k >= height)
					{
						continue;
					}
					// Check if column exists
					else if (j + l < 0 || j + l >= width)
					{
						continue;
					}
					// Sum color values when pixel exists
					else
					{
						blue += buffer[i + k][j + l].rgbtBlue;
						green += buffer[i + k][j + l].rgbtGreen;
						red += buffer[i + k][j + l].rgbtRed;
						counter++;
					}
				}
			}
			// Add averages to pixels (i.e blur pixels)
			image[i][j].rgbtBlue = round(blue / counter);
			image[i][j].rgbtGreen = round(green / counter);
			image[i][j].rgbtRed = round(red / counter);
		}
	}
	// Free memory for buffer
	free(buffer);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Create buffer for swap
    RGBTRIPLE(*buffer)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (buffer == NULL)
    {
        return;
    }
    // Copy original RGB values to buffer
    // Iterate through rows
    for (int i = 0; i < height; i++)
    {
        // Iterate through columns
        for (int j = 0; j < width; j++)
        {
            buffer[i][j].rgbtBlue = image[i][j].rgbtBlue;
            buffer[i][j].rgbtGreen = image[i][j].rgbtGreen;
            buffer[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
    // Apply edge detection
    // Create two-dimensional arrays for Gx/Gy factors
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // Iterate through rows
    for (int i = 0; i < height; i++)
    {
        // Iterate through columns
        for (int j = 0; j < width; j++)
        {
            // Create variables for RGB Gx/Gy values
            float blue_x = 0;
            float green_x = 0;
            float red_x = 0;
            float blue_y = 0;
            float green_y = 0;
            float red_y = 0;
            // Iterate through adjacent rows
            for (int k = -1; k < 2; k++)
            {
                // Iterate through adjacent columns
                for (int l = -1; l < 2; l++)
                {
                    // If pixel doesn't exist then continue
                    if (i + k < 0 || i + k >= height || j + l < 0 || j + l >= width)
                    {
                        continue;
                    }
                    // If pixel exists then sum RGB values
                    else
                    {
                        blue_x += (buffer[i + k][j + l].rgbtBlue) * gx[k + 1][l + 1];
                        green_x += (buffer[i + k][j + l].rgbtGreen) * gx[k + 1][l + 1];
                        red_x += (buffer[i + k][j + l].rgbtRed) * gx[k + 1][l + 1];
                        blue_y += (buffer[i + k][j + l].rgbtBlue) * gy[k + 1][l + 1];
                        green_y += (buffer[i + k][j + l].rgbtGreen) * gy[k + 1][l + 1];
                        red_y += (buffer[i + k][j + l].rgbtRed) * gy[k + 1][l + 1];
                    }
                }
            }
            // Apply Sobel formula, round to nearest integer
            int blue = round(sqrt((blue_x * blue_x) + (blue_y * blue_y)));
            int green = round(sqrt((green_x * green_x) + (green_y * green_y)));
            int red = round(sqrt((red_x * red_x) + (red_y * red_y)));
            // Cap values at 255
            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (red > 255)
            {
                red = 255;
            }
            // Assign edge detection RGB to image pixels
            image[i][j].rgbtBlue = blue;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtRed = red;
        }
    }
    // Free memory for buffer
    free(buffer);
    return;
}





