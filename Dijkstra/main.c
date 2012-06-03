//
//  main.c
//  Dijkstra
//
//  Created by Eugene Dymov on 02.06.12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <stdbool.h>

FILE *outFile;

int numOfNodes;
int firstNode;
int iterationNumber;
int **inputData;
int **lArray;
int *finalLArray;
int *visited;
int *pVector;

typedef struct {
    unsigned int n;
    int *arr;
} arrInfo;

void parseFile() {
    FILE *inputFilePtr;
    inputFilePtr = fopen("input.txt", "r");
    char buf[255];
    
    fgets(buf, 255, inputFilePtr);
    numOfNodes = atoi(buf);
    
    fgets(buf, 255, inputFilePtr);
    firstNode = atoi(buf);
    firstNode--; // because counting from zero
    
    inputData = (int **)malloc(sizeof(int *) * numOfNodes);
    lArray = (int **)malloc(sizeof(int *) * numOfNodes);
    visited = (int *)malloc(sizeof(int) * numOfNodes);
    finalLArray = (int *)malloc(sizeof(int) * numOfNodes);
    pVector = (int *)malloc(sizeof(int) * numOfNodes);
    
    for (int i = 0; i < numOfNodes; i++) {
        inputData[i] = (int *)malloc(numOfNodes * sizeof(int));
        lArray[i] = (int *)malloc(numOfNodes * sizeof(int));
        
        visited[i] = 0;
        finalLArray[i] = INT32_MAX;
        pVector[i] = 0;
    }
    
    int parsedLineIndex = 0;
    
    while (fgets(buf, 255, inputFilePtr) != NULL) {
        int firstNumber = atoi(strtok(buf, "\t"));
        inputData[parsedLineIndex][0] = firstNumber;
        
        int parsedNumberAtLine = 1;
        
        char *nextNumberChar;
        while ( (nextNumberChar = strtok(NULL, "\t")) != NULL ) {
            int nextNumber = atoi(nextNumberChar);
            inputData[parsedLineIndex][parsedNumberAtLine] = nextNumber;
            parsedNumberAtLine++;
        }
        
        parsedLineIndex++;
    }
    printf("\tOK\n");
}

char *correctWeight(int weight) {
    char *printableNumber = (char *)malloc(sizeof(char) * 255);
    sprintf(printableNumber, "%d", weight);
    printableNumber = (weight == INT32_MAX) ? "∞" : printableNumber;
    return printableNumber;
}

void debugFinalLArray() {
    printf("%d) ", iterationNumber + 1);
    fprintf(outFile, "%d) ", iterationNumber + 1);
    for (int i = 0; i < numOfNodes; i++) {
        char *correctWeightString = correctWeight(finalLArray[i]);
        printf("\t%s", correctWeightString);
        fprintf(outFile, "%s\t", correctWeightString);
    }
    fprintf(outFile, "\n");
    printf("\n");
}

void debugVisitedArray() {
    printf("\t>> Debugging visitedArray\n\t");
    for (int i = 0; i < numOfNodes; i++) {
        printf("\t%d", visited[i]);
    }
    printf("\n");    
}

void debugPVector() {
    printf("\t>> Debugging pVector\n\t");
    for (int i = 0; i < numOfNodes; i++) {
        printf("\t%d", pVector[i]);
    }
    printf("\n");        
}

arrInfo crossingForNode(int node, bool returnIndex, bool filterVisited) {
    arrInfo crossings;
    crossings.arr = malloc(sizeof(int) * numOfNodes);
    unsigned int crossingsIndex = 0;
    for (int i = 0; i < numOfNodes; i++) {
        int currentWeight = inputData[node][i];
        bool isNotVisited = filterVisited ? ! visited[i] : true;
        if ( currentWeight > 0 && isNotVisited) {
            int returningValue = returnIndex ? i : currentWeight;
            crossings.arr[crossingsIndex++] = returningValue;
        }
    }
    crossings.n = crossingsIndex;
    return crossings;
}

void calculateLs() {
    while ( true ) {
        debugFinalLArray();
        int currentNode = -1;
        int currentWeight = INT32_MAX;
        
        for (int i = 0; i < numOfNodes; i++) {
            if ( (! visited[i]) && (currentWeight > finalLArray[i]) ) {
                currentNode = i;
                currentWeight = finalLArray[i];
            }
        }
        
        if ( currentNode < 0 ) {
            break;
        }
        visited[currentNode] = 1;
        arrInfo crossings = crossingForNode(currentNode, true, true);
        arrInfo crossingsWeights = crossingForNode(currentNode, false, true);
        
        iterationNumber ++;
        
        if ( ! crossings.n ) {
            continue;
        }
        
        // update final l
        
        for (int i = 0; i < crossings.n; i++) {
            int newWeight = currentWeight + crossingsWeights.arr[i];
            int nodeIndex = crossings.arr[i];
            if ( finalLArray[nodeIndex] > newWeight ) {
                finalLArray[nodeIndex] = newWeight;
                pVector[nodeIndex] = currentNode;
            }
        }
    }
    printf("\tOK\n");
}

arrInfo fullWayForNode(int node) {
    arrInfo fullWay;
    fullWay.arr = malloc(sizeof(int) * numOfNodes);
    int nodeIndex = 0;
    int lastNode = node;
    
    while ( true ) {
        if ( lastNode == firstNode || finalLArray[lastNode] >= INT32_MAX ) {
            break;
        }
        
        int prevNode = pVector[lastNode];
        fullWay.arr[nodeIndex++] = prevNode;
        lastNode = prevNode;
        
        if ( prevNode == firstNode ) {
            break;
        }
    }
    
    fullWay.n = nodeIndex;
    
    return fullWay;
}

void printPaths() {
    for (int i = 0; i < numOfNodes; i++) {
        arrInfo fullWay = fullWayForNode(i);
        fprintf(outFile, "%d) ", i+1);
        printf("%d) ", i+1);
        for (int i = fullWay.n - 1; i >=0; i--) {
            fprintf(outFile, "%d -> ", fullWay.arr[i] + 1);
            printf("%d -> ", fullWay.arr[i] + 1);
        }
        char *correctWeightString = correctWeight(finalLArray[i]);
        fprintf(outFile, "%d [%s]", i + 1, correctWeightString);
        printf("%d [%s]", i + 1, correctWeightString);
        fprintf(outFile, "\n");
        printf("\n");
    }
}

void checkArray() {
    int *horizontalSum = (int *)malloc(sizeof(int) * numOfNodes);
    int *verticalSum = (int *)malloc(sizeof(int) * numOfNodes);;
    
    for (int i = 0; i < numOfNodes; i++) {
        int hSum = 0;
        int vSum = 0;
        for (int j = 0; j < numOfNodes; j++) {
            hSum += inputData[i][j];
            vSum += inputData[j][i];
        }
        horizontalSum[i] = hSum;
        verticalSum[i] = vSum;
    }
    
    bool arrayIsOk = true;
    int suspiciousIndex = -1;
    
    for (int i = 0; i < numOfNodes; i++) {
        if ( horizontalSum[i] != verticalSum[i] ) {
            suspiciousIndex = i;
            arrayIsOk = false;
        }
        printf("\t%d:%d\n", horizontalSum[i], verticalSum[i]);
    }
    if (arrayIsOk) {
        printf("\tOK\n");
    } else {
        fprintf(outFile, "Your array is incorrect check line/column: %d", suspiciousIndex + 1);
        printf("\tFAILED\n");
        exit(1);
    }
}

int main(int argc, const char * argv[])
{
    outFile = fopen("output.txt", "w");
    fprintf(outFile, "L:\n");
    
    printf("=== Parsing file...\n");
    parseFile();
    
    printf("\tLoaded number of nodes: %d\n", numOfNodes);
    printf("\tFirst node is: %d\n", firstNode);
    printf("\tLoaded matrix:\n");
    for (int i = 0; i < numOfNodes; i++) {
        for (int j = 0; j < numOfNodes; j++) {
            printf("\t%d", inputData[i][j]);
            lArray[i][j] = INT32_MAX;
        }
        printf("\n");
    }
    printf("\n");
    
    printf("=== Checking array...\n");
    checkArray();
    
    finalLArray[firstNode] = 0;
    
    printf("=== Calculating L's...\n");
    calculateLs();
    
    debugPVector();
    
    printf("=== Print paths...\n");
    fprintf(outFile, "\nPaths:\n");
    printPaths();    
    
	printf("\nCheck out output.txt file. Press anykey to exit...\n");
	
	getchar();
	
    return 0;
}

