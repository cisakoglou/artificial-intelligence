#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "structs.h"
#include "addFunct.h"

#define TRUE		1
#define	FALSE		0

void k_nearest(int k,char* testF,char* resultsF){
    // o idios provlimatismos me prin gia otan isapexoun
    int i,j;
    sample* sampleT = malloc(sizeof(sample));
    //double dis[NSAMPLES_test]; // na sortaristei se auksousa
    distance disFrom[NSAMPLES_train];
    int votes[NLABELS];
    int maxVotes,maxVotesLabelId;
    FILE* fileTest = fopen(testF,"r");
    FILE* fileresults = fopen(resultsF,"a");
    fprintf(fileresults,">>> K-nearest\n");
    fprintf(fileresults,"** k = %d **\n",k);
    for (i=0;i<NSAMPLES_test;i++){
        sampleT->featureVector = malloc(dimension*sizeof(double));
        for(j=0;j<dimension;j++) fscanf(fileTest,"%lf",&sampleT->featureVector[j]);
        for (j=0;j<NSAMPLES_train;j++){
            disFrom[j].dis = calculate_euclidean_distance(sampleT->featureVector,trainingSet->sampleList[j]->featureVector);
            disFrom[j].fromLabel = trainingSet->sampleList[j]->itsLabel->id;
        }
        // sort ton dis
        qsort(disFrom,NSAMPLES_train,sizeof(distance),compare_function);
        // na apodothoun psifoi apo thn k-geitonia sta labels
        // de tha mprousa na doulepsw se enan pinaka k-thesewn gia na min kanw anazitisi tou max
        // se pinaka me toses theseis osa kai ta labels ??
        for (j=0;j<NLABELS;j++){
            votes[j]=0;
        }
        for (j=0;j<k;j++){
            votes[disFrom[j].fromLabel-1]++; // auksise tis psifous tou label apo to opoio exei mikroteri apostasi
        }
        // na tupwthei o nikiths label pou exei thn pleiopsifia apo psifous :p
        maxVotes = votes[0];
        maxVotesLabelId = 1;
        for (j=1;j<NLABELS;j++){
            if (votes[j] > maxVotes) {
                //printf("mpika");
                maxVotes = votes[j];
                maxVotesLabelId = j+1;
            }
        }
        fprintf(fileresults,"%d \t:\t %d\n",i+1,maxVotesLabelId);
    }
    printf("Results recorded!\n");
    //cleaning
    if (sampleT) free(sampleT);
    fclose(fileTest);
    fclose(fileresults);
}
