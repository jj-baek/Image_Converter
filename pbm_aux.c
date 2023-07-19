#include "pbm.h"
#include <stdio.h>
#include <stdlib.h>

PPMImage * new_ppmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PPMImage *ppm = (PPMImage*) malloc(sizeof(PPMImage));
    ppm->pixmap[0] = (unsigned int **) malloc( sizeof(int *) * h );
    ppm->pixmap[1] = (unsigned int **) malloc( sizeof(int *) * h );
    ppm->pixmap[2] = (unsigned int **) malloc( sizeof(int *) * h );

    for (int i=0; i<h; i++){
        ppm->pixmap[0][i] = (unsigned int *) malloc(sizeof (int) * w);
    }
    for (int i=0; i<h; i++){
        ppm->pixmap[1][i] = (unsigned int *) malloc(sizeof (int) * w);
    }
    for (int i=0; i<h; i++){
        ppm->pixmap[2][i] = (unsigned int *) malloc(sizeof (int) * w);
    }
    ppm->height = h;
    ppm->width = w;
    ppm->max = m;
    return ppm;
}

PBMImage * new_pbmimage( unsigned int w, unsigned int h )
{
    PBMImage *pbm = (PBMImage*) malloc(sizeof(PBMImage));
    pbm->pixmap = (unsigned int **) malloc( sizeof(int *) * h );
    for (int i=0; i<h; i=i+1){
        pbm->pixmap[i] = (unsigned int *) malloc(sizeof (int) * w);
    }
    pbm->height = h;
    pbm->width = w;
    return pbm;
}

PGMImage * new_pgmimage( unsigned int w, unsigned int h, unsigned int m )
{
    PGMImage *pgm = (PGMImage*) malloc(sizeof(PGMImage));
    pgm->pixmap = (unsigned int **) malloc( sizeof(int *) * h );
    for (int i=0; i<h; i=i+1){
        pgm->pixmap[i] = (unsigned int *) malloc(sizeof (int) * w);
    }
    pgm->height = h;
    pgm->width = w;
    pgm->max = m;
    return pgm;
}

// Free dynamically located memory
void del_ppmimage( PPMImage * p )
{
    for(int i = 0; i < p->height; i++){
        free(p->pixmap[0][i]);
    }
    for(int i = 0; i < p->height; i++){
        free(p->pixmap[1][i]);
    }
    for(int i = 0; i < p->height; i++){
        free(p->pixmap[2][i]);
    }
    free(p->pixmap[0]);
    free(p->pixmap[1]);
    free(p->pixmap[2]);
    free(p);
}

void del_pgmimage( PGMImage * p )
{
    for(int i = 0; i < p->height; i++){
        free(p->pixmap[i]);
    }
    free(p->pixmap);
    free(p);
}

void del_pbmimage( PBMImage * p )
{
    for(int i = 0; i < p->height; i++){
        free(p->pixmap[i]);
    }
    free(p->pixmap);
    free(p);
}

