#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "structs.h"
#include "addFunct.h"

#define TRUE		1
#define	FALSE		0

void kmeans(int k,char* testF,char* resultsF){
    // pointers
    int i,j,w;
    int s;
    testSet = malloc(sizeof(sample_list));
    testSet->numberOfSamples = NSAMPLES_test;
    testSet->sampleList = malloc(testSet->numberOfSamples*sizeof(sample*));
    // pernaw ta test samples
    FILE* fileTest = fopen(testF,"r");
    FILE* fileresults = fopen(resultsF,"a");
    fprintf(fileresults,">>> K-means\n");
    fprintf(fileresults,"** k = %d **\n",k);
    for (i=0;i<testSet->numberOfSamples;i++){
        testSet->sampleList[i] = malloc(sizeof(sample));
        testSet->sampleList[i]->featureVector = malloc(dimension*sizeof(double));
        for(j=0;j<dimension;j++) fscanf(fileTest,"%lf",&testSet->sampleList[i]->featureVector[j]);
    }
    // arxikopoiw tis listes pou tha periexoun ta labels - kai ta samples pou anikoun se auta
    labels = malloc(sizeof(label_list));
    labels->numberOfLabels = k;
    labels->labelList = malloc(labels->numberOfLabels*sizeof(label*));
    NLABELS = k;
    for (i=0;i<labels->numberOfLabels;i++){
        labels->labelList[i] = malloc(sizeof(label));
        labels->labelList[i]->id = i+1; // to id kata ena megalutero apo th thesi
        labels->labelList[i]->nItsSamples = 0;
        labels->labelList[i]->itsSamples = malloc(testSet->numberOfSamples*sizeof(sample*));
    }
    int re=0; // epanalipseis
    double* z[labels->numberOfLabels]; // ta kentra twn klasewn
    for (i=0;i<labels->numberOfLabels;i++){
        z[i] = malloc(dimension*sizeof(double));
        for(s=0;s<dimension;s++){
            z[i][s] = testSet->sampleList[i]->featureVector[s];
        }
    }
    double minDis;
    int minId;
    sample_list* tempList = malloc(sizeof(tempList));;
    int check;
    double* checkCond = malloc(dimension*sizeof(double));
    // arxizei i epanalhptikh diadikasia tou algorithmou
    while (re<200){
        for (i=0;i<labels->numberOfLabels;i++){
            labels->labelList[i]->nItsSamples = 0;
        }
        // kathe sample na katataxthei sto label pou vrisketai pio konta sto kentro tou
        for(i=0;i<testSet->numberOfSamples;i++){
            minDis = DBL_MAX;
            for(j=0;j<labels->numberOfLabels;j++){
                if (labels->labelList[j]->id == 0) continue;
                if (calculate_euclidean_distance(z[j],testSet->sampleList[i]->featureVector) < minDis){
                    minDis = calculate_euclidean_distance(z[j],testSet->sampleList[i]->featureVector);
                    minId = labels->labelList[j]->id; // to id tou label sto opoio tha mpei
                }
            }
            labels->labelList[minId-1]->
                itsSamples[labels->labelList[minId-1]->nItsSamples] = testSet->sampleList[i];

            labels->labelList[minId-1]->nItsSamples++;
            testSet->sampleList[i]->itsLabel = labels->labelList[minId-1];
        }
        // epanaprosdiorizw ta kentra mou
        check = 0;
        for(i=0;i<labels->numberOfLabels;i++){

            if (labels->labelList[i]->nItsSamples == 0){
                labels->labelList[i]->id = 0; // to akurwnei san label - elliminate a cluster if somewhere during k-means
                // it has no points assigned to its centroid - tha mporouse na epanarxikopoihsei ta kentra
                // me diaforetiko tropo
                continue;
            }

            tempList->numberOfSamples = labels->labelList[i]->nItsSamples;
            tempList->sampleList = malloc(tempList->numberOfSamples*sizeof(sample*));
            merge_samples_from(tempList,i);
            for (j=0;j<dimension;j++){
                checkCond[j] = z[i][j];
            }
            calculate_mean_vector(z[i],tempList);
            if (converges(checkCond,z[i]) == 1) check++;
        }

        if (check == labels->numberOfLabels) {
            break;
        }
        re++;
    }
    fprintf(fileresults,"Convergence of centers after %d iterations!\n",re);
    for(i=0;i<labels->numberOfLabels;i++){
        if (labels->labelList[i]->id == 0) continue;
        fprintf(fileresults,"--Label %d--\n",labels->labelList[i]->id);
        for(j=0;j<labels->labelList[i]->nItsSamples;j++){
            for (w=0;w<dimension;w++) fprintf(fileresults,"(%g)",labels->labelList[i]->itsSamples[j]->featureVector[w]);
            fprintf(fileresults,"\n");
        }
    }
    printf("Results recorded!\n");
    //cleaning the house
    fclose(fileTest);
    fclose(fileresults);
    for (i=0;i<testSet->numberOfSamples;i++){
         for(j=0;j<dimension;j++){
            if (testSet->sampleList[i]->featureVector) free(testSet->sampleList[i]->featureVector);
         }

    }

    for (i=0;i<labels->numberOfLabels;i++){
        if (labels->labelList[i]->itsSamples) free(labels->labelList[i]->itsSamples);
    }
    if (labels->labelList) free(labels->labelList);
    if (testSet->sampleList) free(testSet->sampleList);
    if (tempList->sampleList) free(tempList->sampleList);
            if (tempList) free(tempList);
}
