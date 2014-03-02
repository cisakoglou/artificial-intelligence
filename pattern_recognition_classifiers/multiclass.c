#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "structs.h"
#include "addFunct.h"

#define TRUE		1
#define	FALSE		0

void multiclass(char* testF,char* resultsF){
    int i,s,w;
    double* av[NLABELS];
    double dis,minDis;
    int minDisIdLabel;
    sample_list* temp = malloc(sizeof(sample_list));
    FILE* fileTest = fopen(testF,"r");
    FILE* fileresults = fopen(resultsF,"a");
    fprintf(fileresults,">>> Multiclass\n");
    for (i=0;i<NLABELS;i++){
        temp->numberOfSamples = labels->labelList[i]->nItsSamples;
        temp->sampleList = malloc(temp->numberOfSamples*sizeof(sample*));
        merge_samples_from(temp,i);
        av[i] = malloc(dimension*sizeof(double));
        calculate_mean_vector(av[i],temp);
    }
    sample* sampleT =malloc(sizeof(sample));
    for (w = 0;w<NSAMPLES_test;w++){
        sampleT->featureVector = malloc(dimension*sizeof(double));
        for(s=0;s<dimension;s++) fscanf(fileTest,"%lf",&sampleT->featureVector[s]);
        for (i=0;i<NLABELS;i++){
            dis = calculate_euclidean_distance(av[i],sampleT->featureVector);
            if (i==0) {
                    minDis = dis;
                    minDisIdLabel = 1;
            }
            if (dis < minDis) {
                    minDis = dis;
                    minDisIdLabel = i+1;
            }
        }
        fprintf(fileresults,"%d \t:\t %d\n",w+1,minDisIdLabel);
    }
    printf("Results recorded!\n");
    // cleaning
    for(i=0;i<NLABELS;i++){
        if (av[i]) free(av[i]);
    }
    if (sampleT->featureVector) free(sampleT->featureVector);
    if (sampleT) free(sampleT);
    if (temp) free(temp);
    fclose(fileTest);
    fclose(fileresults);
}
