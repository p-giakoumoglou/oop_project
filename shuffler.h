#include<iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

/**
 * Shuffles an array using an algorithm that initially iterates an index i from the last to second element.
 * And then a second index j is used to select an element randomly between the first element and the i-th element.
 * And finally The elements in positions i and j are swapped.
 *
 * @param myArray the array to be shuffled.
 * @param myArraySize the index of the array to be shuffled.
 */
template <class X> void shuffle(X** myArray, int myArraySize){
    // TODO: Implement here the shuffle algorithm
    int j;

    X* temp;
    srand(time(NULL));

    for(int i = myArraySize-1 ; i >0 ; i--){


    j= rand() % (i+1);

    temp=myArray[i];

    myArray[i]=myArray[j];

    myArray[j]=temp;
    }

}

/**
 * "Cuts" an array at a random point similarly to a deck of cards (i.e. getting the last part of
 * the cards and moving it before the first part of the cards).
 *
 * @param myArray the array to be "cut".
 * @param myArraySize the index of the array to be "cut".
 */
template <class X> void cut(X** myArray, int myArraySize){
    // TODO: Implement here the cut algorithm
     int cutPoint = rand() % (myArraySize-1);
    X** tempArray = new X*[myArraySize];
    for(int i = 0; i < cutPoint; i++) tempArray[myArraySize - cutPoint + i] = myArray[i];
    for(int i = cutPoint; i < myArraySize; i++) tempArray[i - cutPoint] = myArray[i];
    for(int i = 0; i < myArraySize ; i++) myArray[i] = tempArray[i];
    delete[] tempArray;
}
