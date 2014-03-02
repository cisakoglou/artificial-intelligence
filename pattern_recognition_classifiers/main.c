#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "structs.h"
#include "addFunct.h"
#include "OVA.h"
#include "OVO.h"
#include "multiclass.h"
#include "nearest.h"
#include "k_nearest.h"
#include "kmeans.h"

int NSAMPLES_train;
int NSAMPLES_test;
int dimension;
int NLABELS;

sample_list* trainingSet;
sample_list* testSet;
label_list* labels;

void init_structures(char* trainF,char* labelF){
    trainingSet = malloc(sizeof(sample_list));
    trainingSet->numberOfSamples = NSAMPLES_train;
    trainingSet->sampleList = malloc(NSAMPLES_train*sizeof(sample*));
    labels = malloc(sizeof(label_list));
    labels->numberOfLabels = 0;
    int l,i,j;
    FILE* fileTrain = fopen(trainF,"r");
    FILE* fileLabels = fopen(labelF,"r");
    for(i=0;i<trainingSet->numberOfSamples;i++){
        // create the sample
        sample* temp = malloc(sizeof(sample));
        temp->featureVector = malloc(dimension*sizeof(double));
        // diavazw dianusmata xarakthristikwn
        for(j=0;j<dimension;j++){
            fscanf(fileTrain,"%lf",&temp->featureVector[j]);
        }
        // diavazw label
        fscanf(fileLabels,"%d",&l);
        // create the label
        label* tempL = malloc(sizeof(label));
        tempL->id = l;
        tempL->nItsSamples = 0;
        // tha prepei 1.na auksanetai o arithmos twn labels an to id tou einai neo
        // kai 2.na prostithetai sta labels
        if (isOldLabel(l) == 0){
            if (labels->numberOfLabels == 0 ){
                // malloc
                labels->labelList = malloc(l*sizeof(label*));
                j = 0;
                while (j < l){
                    labels->labelList[j] = NULL;j++;
                }
                labels->numberOfLabels = l;
                labels->labelList[l-1] = tempL;
            }
            else{
                // realloc
                if (l > labels->numberOfLabels) {
                    labels->labelList = realloc(labels->labelList,(labels->numberOfLabels*sizeof(label*))
                                                +(l-labels->numberOfLabels)*sizeof(label*)); // gia na kalupsei th diafora
                    for(j=labels->numberOfLabels;j < l;j++){
                        labels->labelList[j] = NULL;
                    }
                    labels->numberOfLabels = l;
                }
                labels->labelList[l-1] = tempL;
            }
        }

        // to kataxwrw ws label tou antistoixou deigmatos
        temp->itsLabel = tempL;
        // to pernaw sto set
        trainingSet->sampleList[i] = temp;
    }
    NLABELS = labels->numberOfLabels;
    // gia kathe label pou uparxei na to susxetisei me ola ta deigmata pou anikoun se auto
    for(i=0;i<trainingSet->numberOfSamples;i++){
        int id = trainingSet->sampleList[i]->itsLabel->id;
        if (labels->labelList[id-1]->nItsSamples == 0){
            labels->labelList[id-1]->itsSamples = malloc(sizeof(sample*));
            labels->labelList[id-1]->itsSamples[labels->labelList[id-1]->nItsSamples] = trainingSet->sampleList[i];
            labels->labelList[id-1]->nItsSamples++;
        }
        else{
            labels->labelList[id-1]->itsSamples = realloc(labels->labelList[id-1]->itsSamples,
                                                          (labels->labelList[id-1]->nItsSamples+1)*sizeof(sample*));
            labels->labelList[id-1]->itsSamples[labels->labelList[id-1]->nItsSamples] = trainingSet->sampleList[i];
            labels->labelList[id-1]->nItsSamples++;
        }
    }
    fclose(fileTrain);
    fclose(fileLabels);
}

int main()
{
    int choise;
    char name[10];
    char trainF[100];
    char labelF[100];
    char testF[100];
    char resultsF[100];
    int k;
    do{
    printf("Choose algorithm:\n");
    printf("1 -> One Against All\n");
    printf("2 -> One Against One\n");
    printf("3 -> Multiclass\n");
    printf("4 -> Nearest\n");
    printf("5 -> k-Nearest\n");
    printf("6 -> k-Means\n");
    printf("0 -> Esc\n");
    scanf("%d",&choise);
    if (choise == 0) break;
    printf("Name of data set:\n");
    scanf("%s",name);
    printf("Dimension:\n");
    scanf("%d",&dimension);
    printf("#Test Samples:\n");
    scanf("%d",&NSAMPLES_test);
    if (dimension == 2){
        strcpy(testF,".\\data\\");
        strcat(testF,name);
        strcat(testF,"\\");
        strcat(testF,name);
        strcat(testF,"_test_2Ddata.txt");
    }
    else{
        strcpy(testF,".\\data\\");
        strcat(testF,name);
        strcat(testF,"\\");
        strcat(testF,name);
        strcat(testF,"_test_data.txt");
    }
    if (dimension == 2){
        strcpy(resultsF,".\\data\\");
        strcat(resultsF,name);
        strcat(resultsF,"\\");
        strcat(resultsF,name);
        strcat(resultsF,"_test_2Dresults.txt");
    }
    else{
        strcpy(resultsF,".\\data\\");
        strcat(resultsF,name);
        strcat(resultsF,"\\");
        strcat(resultsF,name);
        strcat(resultsF,"_test_results.txt");
    }
    if (choise == 6){
        printf("k :\n");
        scanf("%d",&k);
        kmeans(k,testF,resultsF); // na ginei efarmogi gia k=2,3,4,5,10,20
    }
    else{
        printf("#Train Samples:\n");
        scanf("%d",&NSAMPLES_train);
        if (dimension == 2){
            strcpy(trainF,".\\data\\");
            strcat(trainF,name);
            strcat(trainF,"\\");
            strcat(trainF,name);
            strcat(trainF,"_train_2Ddata.txt");
        }
        else{
            strcpy(trainF,".\\data\\");
            strcat(trainF,name);
            strcat(trainF,"\\");
            strcat(trainF,name);
            strcat(trainF,"_train_data.txt");
        }
        strcpy(labelF,".\\data\\");
        strcat(labelF,name);
        strcat(labelF,"\\");
        strcat(labelF,name);
        strcat(labelF,"_train_labels.txt");
        init_structures(trainF,labelF); //".\\data\\bee\\bee_test_data.txt",
        switch(choise){
            case(1):
               OVA(testF,resultsF);
               break;
            case(2):
               OVO(testF,resultsF);
               break;
            case(3):
               multiclass(testF,resultsF);
               break;
            case(4) :
               nearest(testF,resultsF);
               break;
            case(5):
                printf("k :\n");
                scanf("%d",&k);
                k_nearest(k,testF,resultsF); // na ginei efarmogi gia k=2,3,4,5
                break;
            default:
                break;
        }
    }
    }while(choise!=0);
    if (trainingSet) free(trainingSet);
    if (testSet) free(testSet);
    if (labels) free(labels);
    system("pause");
    return 0;
}
