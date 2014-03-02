#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "structs.h"
#include "addFunct.h"

#define TRUE		1
#define	FALSE		0

void nearest(char* testF,char* resultsF){
    // se periptwsh pou isapexei apo parapanw apo ena simeio , ta opoia anikoun se diaforetikes klaseis ??
    int i,j;
    sample* sampleT = malloc(sizeof(sample));
    double dis[NSAMPLES_test];
    double minDis;
    int minDisLabelId;
    FILE* fileTest = fopen(testF,"r");
    FILE* fileresults = fopen(resultsF,"a");
    fprintf(fileresults,">>> Nearest\n");
    for (i=0;i<NSAMPLES_test;i++){
        sampleT->featureVector = malloc(dimension*sizeof(double));
        for(j=0;j<dimension;j++) fscanf(fileTest,"%lf",&sampleT->featureVector[j]);
        for (j=0;j<NSAMPLES_train;j++){
            dis[j] = calculate_euclidean_distance(sampleT->featureVector,trainingSet->sampleList[j]->featureVector);
        }
        minDis = dis[0];
        minDisLabelId = trainingSet->sampleList[0]->itsLabel->id;
        for (j=1;j<NSAMPLES_train;j++){
            if (dis[j] < minDis){
                minDis = dis[j]; minDisLabelId = trainingSet->sampleList[j]->itsLabel->id;
            }
        }

        fprintf(fileresults,"%d \t:\t %d\n",i+1,minDisLabelId);
    }
    printf("Results recorded!\n");
    // cleaning
    if (sampleT) free(sampleT);
    fclose(fileTest);
    fclose(fileresults);
}
