#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "structs.h"
#include "addFunct.h"

#define TRUE		1
#define	FALSE		0

void OVO(char* testF,char* resultsF){
    int i,j;
    int w;
    sample_list* labelA = malloc(sizeof(sample_list));
    sample_list* labelB = malloc(sizeof(sample_list));
    int nBinProb = NLABELS*(NLABELS-1)/2; //number of binary problems
    int pointerProb = 0; // se poio duadiko provlima vriskomai
    double *avA[nBinProb],*avB[nBinProb];
    double disFromA,disFromB;
    FILE* fileTest = fopen(testF,"r");
    FILE* fileresults = fopen(resultsF,"a");
    fprintf(fileresults,">>> One-versus-one\n");
    for (i = 0;i<NLABELS-1;i++){ // label a
            for (j = i+1;j<NLABELS;j++){ // comparing to label b
                labelA->numberOfSamples = labels->labelList[i]->nItsSamples;
                labelA->sampleList = malloc(labelA->numberOfSamples*sizeof(sample*));
                labelB->numberOfSamples = labels->labelList[j]->nItsSamples;
                labelB->sampleList = malloc(labelB->numberOfSamples*sizeof(sample*));
                merge_samples_from(labelA,i);
                merge_samples_from(labelB,j);
                avA[pointerProb] = malloc (dimension*sizeof(double));
                avB[pointerProb] = malloc (dimension*sizeof(double));
                calculate_mean_vectors(avA[pointerProb],avB[pointerProb],labelA,labelB);
                pointerProb++;
            }
    }
    sample_test_OVO* sampleT =malloc(sizeof(struct sample_test_OVO));
    sampleT->votes = malloc(NLABELS*sizeof(int));
    int s;
    for (w = 0;w<NSAMPLES_test;w++){
        pointerProb = 0;
        sampleT->featureVector = malloc(dimension*sizeof(double));
        for(s=0;s<NLABELS;s++) sampleT->votes[s]=0;
        for(s=0;s<dimension;s++) fscanf(fileTest,"%lf",&sampleT->featureVector[s]);
        for (i = 0;i<NLABELS-1;i++){ // label a
            for (j = i+1;j<NLABELS;j++){ // comparing to label b
                    disFromA = calculate_euclidean_distance(avA[pointerProb],sampleT->featureVector);
                    disFromB = calculate_euclidean_distance(avB[pointerProb],sampleT->featureVector);
                    if (disFromA < disFromB) sampleT->votes[i]++;
                    else sampleT->votes[j]++;
                    pointerProb++;
            }
        }
        int maxVotes = sampleT->votes[0];
        sampleT->maxVotesLabel = 1;
        for(s=1;s<NLABELS;s++){
            if (sampleT->votes[s] > maxVotes) {
                    maxVotes = sampleT->votes[s];
                    sampleT->maxVotesLabel = s+1;
            }
        }
        // elegxos an uparxei isopsifia -- vrisketai ki allo label pou mazepse toses psifous oses kai to maxVotesLabel
        for(s=0;s<NLABELS;s++){
            if ((sampleT->votes[s] == maxVotes)&&(sampleT->maxVotesLabel!=s+1)) sampleT->maxVotesLabel = 0;
        }
        if (sampleT->maxVotesLabel != 0) fprintf(fileresults,"%d \t:\t %d\n",w+1,sampleT->maxVotesLabel);
        else fprintf(fileresults,"%d \t:\t Indeterminate Region\n",w+1);
    }
    printf("Results recorded!\n");
    // cleaning
    for(i=0;i<nBinProb;i++){
        if (avA[i]) free(avA[i]);
        if (avB[i]) free(avB[i]);
    }
    if (sampleT->featureVector) free(sampleT->featureVector);
    if (sampleT) free(sampleT);
    if (labelA) free(labelA);
    if (labelB) free(labelB);
    fclose(fileTest);
    fclose(fileresults);
}
