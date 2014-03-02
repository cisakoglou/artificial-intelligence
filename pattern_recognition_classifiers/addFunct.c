#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "structs.h"

#define TRUE		1
#define	FALSE		0

// voithitikes sunartiseis

/*
 * epistrefei ena-TRUE an to label exei hdh emfanistei
 */
int isOldLabel(int l){
    if (labels->numberOfLabels == 0) return 0;
    if (l > labels->numberOfLabels) return 0;
    if (labels->labelList[l-1]) return 1;
    return 0;
}

/*
 * pernaei ola ta diathesima samples plhn autwn pou anikoun sto label me thesi sto labelist indexPositiveLabel
 * sto samplelist negative label
 */
int merge_samples_without(sample_list* negativeLabel,int indexPositiveLabel){
    int s = 0;
    int i,j;
    for(i=0;i<labels->numberOfLabels;i++){
        if (i!=indexPositiveLabel){
            for(j=0;j<labels->labelList[i]->nItsSamples;j++){
                negativeLabel->sampleList[s] = labels->labelList[i]->itsSamples[j];s++;
            }
        }
    }
    if (s == negativeLabel->numberOfSamples) return TRUE;
    else return FALSE;
}

/*
 * pernaei apo ta diathesima samples mono auta pou anikoun sto label me thesi sto labelist indexPositiveLabel
 * sto samplelist positive label
 */
int merge_samples_from(sample_list* positiveLabel,int indexPositiveLabel){
    int i,s = 0;
    for(i=0;i < labels->labelList[indexPositiveLabel]->nItsSamples;i++){
        positiveLabel->sampleList[i] = labels->labelList[indexPositiveLabel]->itsSamples[i];s++;
    }
    if (s == positiveLabel->numberOfSamples) return TRUE;
    else return FALSE;
}

void calculate_mean_vector(double* a,sample_list* pos){
    int i,w;
    double* temp = malloc(dimension*sizeof(double));
    if (pos->numberOfSamples == 1){
        for(w=0;w<dimension;w++){
            temp[w] = pos->sampleList[0]->featureVector[w];
        }
        return;
    }

    for(i=0;i<dimension;i++){
        temp[i]=0;
    }
    for(i=0;i < pos->numberOfSamples;i++){
            for(w=0;w < dimension;w++){
                temp[w] = temp[w] + (pos->sampleList[i]->featureVector[w]);
            }
    }

    for(i=0;i<dimension;i++){
        temp[i]=(double) (temp[i] / pos->numberOfSamples);
    }
    for(i=0;i<dimension;i++){
        a[i] = temp[i];
    }
}

void calculate_mean_vectors(double*a,double* b,sample_list* pos,sample_list* neg){
    calculate_mean_vector(a,pos);
    calculate_mean_vector(b,neg);
}

double calculate_euclidean_distance(double* a,double* b){
    double s = 0;
    int i;
    for(i=0;i<dimension;i++){
        s = s + pow((a[i]-b[i]),2);
    }
    return sqrt(s);
}

int compare_function(const void *a,const void *b) {
    distance *x = (distance *) a;
    distance *y = (distance *) b;
    if (x->dis < y->dis) return -1;
    else if (x->dis > y->dis) return 1;
    return 0;
}

int converges(double* a,double* b){
    int s=0,i;
    for (i=0;i<dimension;i++){
        if (abs(a[i]-b[i])<0.0000001) s++;
    }
    if (s==dimension) return TRUE;
    else return FALSE;
}
