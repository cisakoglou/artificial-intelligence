#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "structs.h"
#include "addFunct.h"

#define TRUE		1
#define	FALSE		0

void OVA(char* testF,char* resultsF){
    sample_test_OVA* sampleT;
    sampleT = malloc(sizeof(sample_test_OVA));
    FILE* fileTest = fopen(testF,"r");
    FILE* fileresults = fopen(resultsF,"a");
    fprintf(fileresults,">>> One-versus-all\n");
    sample_list* negativeLabel = malloc(sizeof(sample_list));
    sample_list* positiveLabel = malloc(sizeof(sample_list));
    int i,j;
    double *avPos[NLABELS],*avNeg[NLABELS]; // gia na apothikeuontai oi ar.mesoi twn klasewn - sth thetikh klasi antistoixei to i
                                            // apofeugw epanaupologismo
    double disFromPos,disFromNeg;
    for(i=0;i<NLABELS;i++){
        positiveLabel->numberOfSamples = labels->labelList[i]->nItsSamples;
        positiveLabel->sampleList = malloc(positiveLabel->numberOfSamples*sizeof(sample*));
        negativeLabel->numberOfSamples = NSAMPLES_train - labels->labelList[i]->nItsSamples;
        negativeLabel->sampleList = malloc(negativeLabel->numberOfSamples*sizeof(sample*));
        merge_samples_without(negativeLabel,i);// ws parametros h thetikh klasi, ta samples twn upoloipwn tha apoteloun tin eniaia
                                        // arnhtikh klasi
        merge_samples_from(positiveLabel,i);
        avPos[i] = malloc (dimension*sizeof(double));
        avNeg[i] = malloc (dimension*sizeof(double));
        calculate_mean_vectors(avPos[i],avNeg[i],positiveLabel,negativeLabel);//(mean vector of positive label,
                                    // mean vector of negative label, id of positive label, samples of negative label)
    }

    for (i = 0;i<NSAMPLES_test;i++){
        sampleT->featureVector = malloc(dimension*sizeof(double));
        sampleT->labels_in = 0;
        for(j=0;j<dimension;j++){
            fscanf(fileTest,"%lf",&sampleT->featureVector[j]);
        }
        sampleT->label_id = 0;
        for(j=0;j<NLABELS;j++){  // j+1 to id ths trexousas thetikhs klaseis, oles oi upoloipes apoteloun mia eniaia thn arnhtikh klash
            //euclidean distances
            disFromPos = calculate_euclidean_distance(avPos[j],sampleT->featureVector);
            disFromNeg = calculate_euclidean_distance(avNeg[j],sampleT->featureVector);
            if (disFromPos < disFromNeg){
                sampleT->label_id = j+1;
                sampleT->labels_in++;
                if (sampleT->labels_in > 1) {
                        break;
                }
            }
        }
        if ((sampleT->labels_in > 1)||(sampleT->labels_in==0)) fprintf(fileresults,"%d \t:\t Indeterminate Region\n",i+1);
        else fprintf(fileresults,"%d \t:\t %d\n",i+1,sampleT->label_id);
    }
    printf("Results recorded!\n");
    // cleaning
    for(i=0;i<NLABELS;i++){
        if (avPos[i]) free(avPos[i]);
        if (avNeg[i]) free(avNeg[i]);
    }
    if (sampleT->featureVector) free(sampleT->featureVector);
    if (sampleT) free(sampleT);
    if (positiveLabel) free(positiveLabel);
    if (negativeLabel) free(negativeLabel);
    fclose(fileTest);
    fclose(fileresults);
}
