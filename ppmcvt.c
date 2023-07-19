#include "pbm.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// Define Option Structure
// Mode -> transform mode
// arg -> "green" or "red" or "blue"
// parameter -> int value for grayscale, thumbnail, nup
struct Option{
        char Mode;
        char *arg;
        int parameter;
        char *infile_name;
        char *outfile_name;
};

// BITMAP CONVERTION
void toBitmap( struct Option option ){
    // Assign input and output files
    PPMImage *input = read_ppmfile(option.infile_name);
    PBMImage *output = new_pbmimage(input->width, input->height);

    // Loop through each matrix element. If average of RGB is larger than max/2 then while, else black
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            double avg = (input->pixmap[0][i][j] + input->pixmap[1][i][j] + input->pixmap[2][i][j]) / 3.0;
            if (avg < input->max/2.0){
                output->pixmap[i][j] = 1;
            }else{
                output->pixmap[i][j] = 0;
            }
        }
    }
    // write pbm
    write_pbmfile(output, option.outfile_name);
    // free memory
    del_ppmimage(input);
    del_pbmimage(output);
}

// GRAYSCALE CONVERTION
void toGrayScale( struct Option option ){
    // Assign input and output files
    PPMImage *input = read_ppmfile(option.infile_name);
    PGMImage *output = new_pgmimage(input->width, input->height, option.parameter);

    // Loop through each matrix element. We take the average of RGB and scale the number into gray scale
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            double avg = (input->pixmap[0][i][j] + input->pixmap[1][i][j] + input->pixmap[2][i][j]) / 3.0;
            output->pixmap[i][j] = (avg/input->max)*option.parameter;
        }
    }
    // write pgm
    write_pgmfile(output, option.outfile_name);
    // free memory
    del_ppmimage(input);
    del_pgmimage(output);
}

// ISOLATE PARTICULAR COLOR
void isolate( struct Option option ){
    // Assign input and output files
    PPMImage *input = read_ppmfile(option.infile_name);
    PPMImage *output = new_ppmimage(input->width, input->height, input->max);
    // assgin all 0 to output. Create a black ppm
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            output->pixmap[0][i][j] = 0;
        }
    }
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            output->pixmap[1][i][j] = 0;
        }
    }
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            output->pixmap[2][i][j] = 0;
        }
    }
    // Compare option argument with RGB. Put original value into output pixmap
    int cmp = strcmp("red", option.arg);
    if(cmp==0){
        for (int i = 0; i < input->height; i++){
            for (int j = 0; j < input->width; j++){
                output->pixmap[0][i][j] = input->pixmap[0][i][j];
            }
        }
    }
    cmp = strcmp("green", option.arg);
    if(cmp==0){
        for (int i = 0; i < input->height; i++){
            for (int j = 0; j < input->width; j++){
                output->pixmap[1][i][j] = input->pixmap[1][i][j];
            }
        }
    }
    cmp = strcmp("blue", option.arg);
    if(cmp==0){
        for (int i = 0; i < input->height; i++){
            for (int j = 0; j < input->width; j++){
                output->pixmap[2][i][j] = input->pixmap[2][i][j];
            }
        }
    }
    // write pgm
    write_ppmfile(output, option.outfile_name);
    // free memory
    del_ppmimage(input);
    del_ppmimage(output);
}

// REMOVE PARTICULAR COLOR
void removeCol( struct Option option ){
    // Assign input and output files
    PPMImage *input = read_ppmfile(option.infile_name);
    PPMImage *output = new_ppmimage(input->width, input->height, input->max);
    // Copy all matrices to output
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            output->pixmap[0][i][j] = input->pixmap[0][i][j];
        }
    }
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            output->pixmap[1][i][j] = input->pixmap[1][i][j];
        }
    }
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            output->pixmap[2][i][j] = input->pixmap[2][i][j];
        }
    }

    // Compare option argument with RGB. Assign all 0s to that one particular matrix 
    int cmp = strcmp("red", option.arg);
        if(cmp==0){
            for (int i = 0; i < input->height; i++){
                for (int j = 0; j < input->width; j++){
                    output->pixmap[0][i][j] = 0;
                }
            }
        }
    cmp = strcmp("green", option.arg);
        if(cmp==0){
            for (int i = 0; i < input->height; i++){
                for (int j = 0; j < input->width; j++){
                    output->pixmap[1][i][j] = 0;
                }
            }
        }
    cmp = strcmp("blue", option.arg);
        if(cmp==0){
            for (int i = 0; i < input->height; i++){
                for (int j = 0; j < input->width; j++){
                    output->pixmap[2][i][j] = 0;
                }
            }
        }
    // write pgm
    write_ppmfile(output, option.outfile_name);
    // free memory
    del_ppmimage(input);
    del_ppmimage(output);
}

// SEPIA TRANSFORM
void sepia( struct Option option ){
    // Assign input and output files
    PPMImage *input = read_ppmfile(option.infile_name);
    PPMImage *output = new_ppmimage(input->width, input->height, input->max);
    int max = 0;
    // Applying Transformation. 
    for (int i = 0; i < input->height; i++){
        for (int j = 0; j < input->width; j++){
            output->pixmap[0][i][j] = (double)((0.393*input->pixmap[0][i][j])+ (0.769*input->pixmap[1][i][j]) + (0.189*input->pixmap[2][i][j]));
            output->pixmap[1][i][j] = (double)((0.349*input->pixmap[0][i][j])+ (0.686*input->pixmap[1][i][j]) + (0.168*input->pixmap[2][i][j]));
            output->pixmap[2][i][j] = (double)((0.272*input->pixmap[0][i][j])+ (0.534*input->pixmap[1][i][j]) + (0.131*input->pixmap[2][i][j]));
            // Keeping track of the maximum value of the pixmap. Put the maximum value into PPMImage
            if (output->pixmap[0][i][j] >= output->pixmap[1][i][j] && output->pixmap[0][i][j] >= output->pixmap[2][i][j] && output->pixmap[0][i][j] >= max){
                max = output->pixmap[0][i][j];
            }
            else if (output->pixmap[1][i][j] >= output->pixmap[0][i][j] && output->pixmap[1][i][j] >= output->pixmap[2][i][j] && output->pixmap[1][i][j] >= max){
                max = output->pixmap[1][i][j];
            }
            else if (output->pixmap[2][i][j] >= output->pixmap[0][i][j] && output->pixmap[2][i][j] >= output->pixmap[1][i][j] && output->pixmap[2][i][j] >= max){
                max = output->pixmap[2][i][j];
            }
        }
    }
    output->max = max;
    // write pgm
    write_ppmfile(output, option.outfile_name);
    // free memory
    del_ppmimage(input);
    del_ppmimage(output);
}

// MIRROR TRANSFORM
void mirror( struct Option option ){
    // Assign input and output files
    PPMImage *input = read_ppmfile(option.infile_name);
    // Copying the first half to second half
    for (int c = 0; c < 3; c++){
        for (int i = 0; i < input->height; i++){
            for (int j = 0; j < input->width / 2; j++){
                input->pixmap[c][i][input->width-1-j] = input->pixmap[c][i][j];
            }
        }
    }
    // Write ppm
    write_ppmfile(input, option.outfile_name);
    // free memory
    del_ppmimage(input);
}

// THUMBNAIL
void thumbnail( struct Option option ){
    // Assign input and output files. Output should already be scaled
    PPMImage *input = read_ppmfile(option.infile_name);
    PPMImage *output = new_ppmimage(input->width/option.parameter, input->height/option.parameter, input->max);
    // Assign the RGB matrices by adding the nth element in input
    for (int c = 0; c < 3; c++){
        for (int i = 0; i < input->height/option.parameter; i++){
            for (int j = 0; j < input->width/option.parameter; j++){
                output->pixmap[c][i][j] = input->pixmap[c][i*option.parameter][j*option.parameter];
            }
        }
    }
    // write ppm
    write_ppmfile(output, option.outfile_name);
    // free memory
    del_ppmimage(input);
    del_ppmimage(output);
}

// NUP TRANSFORM
void nup( struct Option option ){
    // Assign input and output files. Thumbnail is the image that we are going to tile.
    PPMImage *input = read_ppmfile(option.infile_name);
    PPMImage *output = new_ppmimage(input->width, input->height, input->max);
    PPMImage *thumbnail = new_ppmimage(input->width/option.parameter, input->height/option.parameter, input->max);
    // Do thumbnail first
    for (int c = 0; c < 3; c++){
        for (int i = 0; i < input->height/option.parameter; i++){
            for (int j = 0; j < input->width/option.parameter; j++){
                thumbnail->pixmap[c][i][j] = input->pixmap[c][i*option.parameter][j*option.parameter];
            }
        }
    }
    // Pile thumbnail into output file
    for (int c = 0; c < 3; c++){
        // a represents rows count
        for (int a = 0; a < option.parameter; a++){
            // b represents columns count
            for (int b = 0; b < option.parameter; b++){
                for (int i = 0; i < input->height/option.parameter; i++){
                    for (int j = 0; j < input->width/option.parameter; j++){
                        output->pixmap[c][a*thumbnail->height + i][b*thumbnail->width + j] = thumbnail->pixmap[c][i][j];
                    }
                }
            }
        }
    }
    // write ppm
    write_ppmfile(output, option.outfile_name);
    // free memory
    del_ppmimage(input);
    del_ppmimage(output);
    del_ppmimage(thumbnail);
}

// MAIN FUNCTION
int main( int argc, char *argv[] )
{
    int opt;
    struct Option option;
    // outflag is used to see if there is an out option
    int outflag = 0;
    // transformflag is used record the times of transformations
    int transformflag = 0;
    // Read in infile name. Last parameter is the input file name
    // strcpy(&option.infile_name, &argv[argc - 1]);
    option.infile_name = argv[argc-1];
    // default where no option is specified, Mode = 'b'
    if (argc == 4){
        option.Mode = 'b';
    }
    // use getopt to parse the argument
    while((opt = getopt(argc, argv, "bg:i:r:smt:n:o:")) != -1){
        // valid input always have 4 5 or 6 arguments
        if (argc != 4 && argc != 5 && argc != 6){
            printf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
            exit(0);
        }
        // define each option
        switch(opt){
            case 'b':
                printf("b\n");
                option.Mode = 'b';
                transformflag++;
                break;
            case 'g':
                printf("g\n");
                option.Mode = 'g';
                // detect input error
                if (atoi(optarg) < 1 || atoi(optarg) > 65535){
                    printf("Error: Invalid max grayscale pixel value: %s; must be less than 65,536\n", optarg);
                    exit(0);
                }
                option.parameter = atoi(optarg);
                transformflag++;
                break;
            case 'i':
                printf("i\n");
                option.Mode = 'i';
                // check for input error: input not one of RGB
                int cmp = strcmp(optarg, "green");
                if (cmp!=0){
                    cmp = strcmp(optarg, "red");
                    if (cmp!=0){
                        cmp = strcmp(optarg, "blue");
                        if (cmp!=0){
                            printf("Error: Invalid channel specification: %s; should be 'red', 'green' or 'blue'\n", optarg);
                            exit(0);
                        }
                    }
                }
                // strcpy(&option.arg, &optarg);
                option.arg = optarg;
                transformflag++;
                break;
            case 'r':
                printf("r\n");
                option.Mode = 'r';
                // check for input error: input not one of RGB
                cmp = strcmp(optarg, "green");
                if (cmp!=0){
                    cmp = strcmp(optarg, "red");
                    if (cmp!=0){
                        cmp = strcmp(optarg, "blue");
                        if (cmp!=0){
                            printf("Error: Invalid channel specification: %s; should be 'red', 'green' or 'blue'\n", optarg);
                            exit(0);
                        }
                    }
                }
                // strcpy(&option.arg, &optarg);
                option.arg = optarg;
                transformflag++;
                break;
            case 's':
                printf("s\n");
                option.Mode = 's';
                transformflag++;
                break;
            case 'm':
                printf("m\n");
                option.Mode = 'm';
                transformflag++;
                break;
            case 't':
                printf("t\n");
                option.Mode = 't';
                // check for input error. 
                if (atoi(optarg) < 1 || atoi(optarg) > 8){
                    printf("Error: Invalid scale factor: %d; must be 1-8\n", atoi(optarg));
                    exit(0);
                }
                option.parameter = atoi(optarg);
                transformflag++;
                break;
            case 'n':
                printf("n\n");
                option.Mode = 'n';
                option.parameter = atoi(optarg);
                transformflag++;
                break;
            case 'o':
                printf("o\n");
                outflag = 1;
                // strcpy(&option.outfile_name, &optarg);
                option.outfile_name = optarg;
                break;
            case ':':
                printf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                exit(0);
            case '?':
                printf("Usage: ppmcvt [-bgirsmtno] [FILE]\n");
                exit(0);
        }
    }
    if (optind > argc){
        printf("Error: No input file specified\n");
        exit(0);
    }
    // output file is not specified
    if (outflag == 0){
        printf("Error: No output file specified.   %s\n", option.outfile_name);
        exit(0);
    }
    // multiple transformations specified
    if (transformflag != 1 && transformflag != 0){
        printf("Error: Multiple transformations specified\n");
        exit(0);
    }
    // Finding out what transform to perform
    if(option.Mode == 'b'){
        toBitmap(option);
    }
    else if(option.Mode == 'g'){
        toGrayScale(option);
    }
    else if(option.Mode == 'i'){
        isolate(option);
    }
    else if(option.Mode == 'r'){
        removeCol(option);
    }
    else if(option.Mode == 's'){
        sepia(option);
    }
    else if(option.Mode == 'm'){
        mirror(option);
    }
    else if(option.Mode == 't'){
        thumbnail(option);
    }
    else if(option.Mode == 'n'){
        nup(option);
    }
    return 0;
}
