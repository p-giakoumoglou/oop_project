#include "tablehand.h"

#include <iostream>
#include <sstream>
#include "hand.h"
#include "card.h"
#include "combination.h"

TableHand::TableHand(bool tableSimple[4][13], bool tableSpecial[4]){
    currentNumberOfCards = 0;
    allCombinationsSize = 0;
    allCombinations = new Combination*[allCombinationsSize];
    cards = new Card*[56];
    for(int i = 0; i < 13; i++)
        cardPerSuitCounter[i] = 0;
    hasPhoenix = 0;
    remainingCards = 0;
    scanTable(tableSimple, tableSpecial);
    scanForPhoenix(tableSpecial);
    scanForDragon(tableSpecial);

}
/*
TableHand::TableHand(){
    currentNumberOfCards = 0;
    allCombinationsSize = 0;
    allCombinations = new Combination*[allCombinationsSize];
    cards = new Card*[56];
    for(int i = 0; i < 13; i++)
        cardPerSuitCounter[i] = 0;
    hasPhoenix = 0;
    hasDragon = 0;
    remainingCards = 0;


}
*/
TableHand::~TableHand(){
    for (int i = 0; i < allCombinationsSize; i++)
        delete allCombinations[i];
    for(int i = 0; i < remainingCards; i++)
        delete cards[i];
    allCombinationsSize = 0;
    delete[] allCombinations;
    delete[] cards;
}

void TableHand::scanForPhoenix(bool tableSpecial[4]){

    if(tableSpecial[2] == 1){
        hasPhoenix = 1;
    }
    else hasPhoenix = 0;
}

void TableHand::scanForDragon(bool tableSpecial[4]){
    if(tableSpecial[1] == 1){
        hasDragon = 1;
    }
    else hasDragon = 0;
}

void TableHand::scanTable(bool tableSimple[4][13], bool tableSpecial[4]){
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 13; j++){
            if(tableSimple[i][j] == 1){
                Card* card = new SimpleCard(j+2, (CardSuit) i);
                this->addCard(card);
                simpleTableCopy[i][j] = 1;
                cardPerSuitCounter[j]++;
                remainingCards ++;
            }else simpleTableCopy[i][j] = 0;
        }
    }
    for(int i = 0; i < 4; i++){
        if(tableSpecial[i] == 1){
                int temp = i + 4;
                Card* card = new SpecialCard((CardSuit)temp);
                this->addCard(card);
                remainingCards++;
        }
    }
}

/*void TableHand::remakeTable(bool tableSimple[4][13], CombinationType type){
    if(type == PAIR){

    }else if(type == THREEOFAKIND){

    }else if(type == STAIRS){

    }else if(type == FULLHOUSE){

    }else if(type == STRAIGHT){

    }

    for(int j = 0; j < 13; j++){
        for(int i = 0; i < 4; i++){
            if(tableSimple[i][j] == 0){
                Card* card = new SimpleCard(j+2, (CardSuit) i);
                cardPerSuitCounter[j]++;
                this->addCard(card);
                tableSimple[i][j] = 1;
                break;
            }
        }
    }
}*/

void TableHand::printtable(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            cout<<simpleTableCopy[i][j]<<" ";
        }
        cout<<endl;
    }
}

float TableHand::calcPropabillity(Combination* comb, int noOfCards, int totalCards){
    CombinationType type= comb->getType();
    //int cardCounter = cardPerSuitCounter[comb->getCard(comb->getNumberOfCards())->getValue()-2];
    int cardCounter = noOfCards;
    int cardCounter2;
    if(type == SINGLE){
        return (float)cardCounter/totalCards;
    }else if(type == PAIR){
        Single single1(comb->getCard(0));
        Single single2(comb->getCard(1));
        cardCounter = cardPerSuitCounter[comb->getCard(0)->getValue()-2];
        return calcPropabillity(&single1, cardCounter, totalCards)*calcPropabillity(&single2, cardCounter-1, totalCards - 1);
    }else if(type == THREEOFAKIND){
        Single single1(comb->getCard(0));
        Single single2(comb->getCard(1));
        Single single3(comb->getCard(2));
        cardCounter = cardPerSuitCounter[comb->getCard(0)->getValue()-2];
        return calcPropabillity(&single1, cardCounter, totalCards)*calcPropabillity(&single2, cardCounter-1, totalCards-1)*calcPropabillity(&single3, cardCounter-2, totalCards-2);
    }else if(type == STAIRS){
        Pair pair1(comb->getCard(0), comb->getCard(1));
        Pair pair2(comb->getCard(2), comb->getCard(3));
        cardCounter = cardPerSuitCounter[comb->getCard(0)->getValue()-2];
        cardCounter2 = cardPerSuitCounter[comb->getCard(2)->getValue()-2];
        return calcPropabillity(&pair1, cardCounter, totalCards)*calcPropabillity(&pair2, cardCounter2, totalCards - 2);
    }else if(type == FULLHOUSE){
        Pair pair1(comb->getCard(0), comb->getCard(2));
        ThreeOfAKind three1(comb->getCard(0), comb->getCard(2), comb->getCard(3));
        cardCounter = cardPerSuitCounter[comb->getCard(0)->getValue()-2];
        cardCounter2 = cardPerSuitCounter[comb->getCard(3)->getValue()-2];
        return calcPropabillity(&pair1, cardCounter, totalCards)*calcPropabillity(&three1, cardCounter2, totalCards-2);
    }else if(type == STRAIGHT){
            float prob1 = 1;
            for(int i = 0; i < comb->getNumberOfCards(); i++){
                cardCounter = cardPerSuitCounter[comb->getCard(i)->getValue()-2];
                Single singleStraight(comb->getCard(i));
                prob1 *= calcPropabillity(&singleStraight, cardCounter, totalCards-i);
            }
            return prob1;
    }else if(type == FOUROFAKIND){
        return (1.0/remainingCards)*(1.0/(remainingCards-1))*(1.0/(remainingCards-2))*(1.0/(remainingCards-3))*(1.0/(remainingCards-4));
    }else return 0;

}


float TableHand::findCombinations(Combination* comb){///will return the propabillity
    float prob = 1;
    int numFours, counter = 0, temp = 0;
    Combination** fours = findFourOfAKinds(cards, currentNumberOfCards, numFours);
    addCombinationsToTableHand(fours, numFours);
    bombCounter = numFours;
    if(comb->getType() == FOUROFAKIND){
        for(int i = 0; i < numFours; i++){
            if((allCombinations[i]->getValue() > comb->getValue())){
                counter ++;
            }
        }
    clearTableHand();
    return counter;
    }else{
    clearTableHand();
    }

    counter = 0;

    if(hasPhoenix == 1){
            counter++;//if there is a phoenix potentially there would be at most only one more combination with higher value
                    //so we increase the combinations that can hit you by 1 because it would be too complicated to find if it is 1 or 0 and we would have very little gain
            temp = 1;
    }

    if(comb->getType() == SINGLE){
        int numSingles;
        if(hasDragon == 1)
            counter++;
        Combination** singles = findSingles(cards, currentNumberOfCards, numSingles);
        addCombinationsToTableHand(singles, numSingles);
        for(int i = 0; i < numSingles; i++){
            if(allCombinations[i]->getValue() > comb->getValue()){
                counter ++;
                prob += calcPropabillity(allCombinations[i], cardPerSuitCounter[(int)allCombinations[i]->getValue()-2+temp], remainingCards);
            }
        }
    clearTableHand();
    return counter + bombCounter;
    }

    if(comb->getType() == PAIR){
        int numPairs;
        Combination** pairs = findPairs(cards, currentNumberOfCards, numPairs);
        addCombinationsToTableHand(pairs, numPairs);
        for(int i = 0; i < numPairs; i++){
            if(allCombinations[i]->getValue() > comb->getValue()){
                prob += calcPropabillity(allCombinations[i], cardPerSuitCounter[(int)allCombinations[i]->getValue()-2+temp], remainingCards);
                counter ++;
            }
        }
    clearTableHand();
    return counter + bombCounter;
    }

    if(comb->getType() == THREEOFAKIND){
        int numThrees;
        Combination** threes = findThreeOfAKinds(cards, currentNumberOfCards, numThrees);
        addCombinationsToTableHand(threes, numThrees);
        for(int i = 0; i < numThrees; i++){
            if(allCombinations[i]->getValue() > comb->getValue()){
                counter ++;
            }
        }
    clearTableHand();
    return counter + bombCounter;
    }

    if(comb->getType() == STAIRS){
        int numStairs;
        Combination** stairs = findStairs(cards, currentNumberOfCards, numStairs);
        addCombinationsToTableHand(stairs, numStairs);
        for(int i = 0; i < numStairs; i++){
            if(allCombinations[i]->getValue() > comb->getValue()){
                counter ++;
            }
        }
    clearTableHand();
    return counter + bombCounter;
    }

    if(comb->getType() == STRAIGHT){
        int numStraights;
        Combination** straights = findStraights(cards, currentNumberOfCards, numStraights, false);
        addCombinationsToTableHand(straights, numStraights);
        for(int i = 0; i < numStraights; i++){
            if((allCombinations[i]->getValue() > comb->getValue()) && (allCombinations[i]->getNumberOfCards() == comb->getNumberOfCards())){
                counter ++;
            }
        }
    clearTableHand();
    return counter + bombCounter;
    }
    else return 0;




    /*int numStraightFlushes;
    Combination** straightFlushes = findStraightFlushes(cards, currentNumberOfCards, numStraightFlushes);
    addCombinationsToTableHand(straightFlushes, numStraightFlushes);*/
}

void TableHand::clearTableHand(){
    //for (int i = 0; i < allCombinationsSize; i++)
    //    delete allCombinations[i];
    allCombinationsSize = 0;
    allCombinations = new Combination*[allCombinationsSize];
}

int TableHand::getNumberOfCombinations(){
    return allCombinationsSize;
}

Combination* TableHand::getCombination(int index){
    return allCombinations[index];
}

void TableHand::addCard(Card *card)
{
    int index = 0;
    while(index < currentNumberOfCards && (cards[index]->getValue() < card->getValue() ||
          (cards[index]->getValue() == card->getValue() && cards[index]->getSuit() > card->getSuit()))){
        index++;
    }
    // Move all cards to the right until index
    for (int j = currentNumberOfCards - 1; j >= index; j--)
        cards[j + 1] = cards[j];
    cards[index] = card;
//    cards[currentNumberOfCards] = card;
    currentNumberOfCards++;
}

void TableHand::removeCard(Card* card){
    // Find the index of the card
    int index = -1;
    for (int i = 0; i < currentNumberOfCards; i++){
        if (cards[i]->equals(card)){
            index = i;
            break;
        }
    }
    // "Remove" the found card by moving all cards to the left until index
    for (; index < currentNumberOfCards - 1; index++)
        cards[index] = cards[index + 1];
    cards[currentNumberOfCards - 1] = NULL;
    currentNumberOfCards--;
}

void TableHand::removeCombination(Combination* combination){
    // First remove all cards of combination
    for (int i = 0; i < combination->getNumberOfCards(); i++)
        removeCard(combination->getCard(i));
    // Remove the combination
    Combination** newCombinations = new Combination*[allCombinationsSize - 1];
    int j = 0;
    for (int i = 0; i < allCombinationsSize; i++){
        if (!(allCombinations[i]->equals(combination)))
            newCombinations[j++] = allCombinations[i];
    }
    delete[] allCombinations;
    allCombinations = newCombinations;
    allCombinationsSize--;
}

int TableHand::getCardCount(){
    return currentNumberOfCards;
}

Card *TableHand::getCard(int index){
    return cards[index];
}

string TableHand::toString(){
    string str;
    for (int i = 0; i < currentNumberOfCards; i++)
        str += " " + getCard(i)->toString();
    return str;
}

string TableHand::numberOfCardsToString(){
    stringstream ss;
    ss << currentNumberOfCards;
    return ss.str();
}

void TableHand::addCombinationsToTableHand(Combination** combinations, int numCombinations){
    int newCombinationsSize = allCombinationsSize + numCombinations;
    Combination** newCombinations = new Combination*[newCombinationsSize];
    for (int i = 0; i < allCombinationsSize; i++)
        newCombinations[i] = allCombinations[i];
    for (int i = 0; i < numCombinations; i++)
        newCombinations[allCombinationsSize + i] = combinations[i];
    delete[] combinations;
    delete[] allCombinations;
    allCombinationsSize = newCombinationsSize;
    allCombinations = newCombinations;
}

void TableHand::setPhoenixValue(int value){
    int phoenixIndex = -1;
    for (int i = 0; i < currentNumberOfCards; i++){
        if (cards[i]->hasSuit(PHOENIX))
            phoenixIndex = i;
    }
    if (phoenixIndex >= 0){
        Card* phoenix = cards[phoenixIndex];
        phoenix->setValue(value);
        removeCard(phoenix);
        addCard(phoenix);
    }
}

void TableHand::createAllCombinations(){
    int numSingles;
    Combination** singles = findSingles(cards, currentNumberOfCards, numSingles);
    addCombinationsToTableHand(singles, numSingles);

    int numPairs;
    Combination** pairs = findPairs(cards, currentNumberOfCards, numPairs);
    addCombinationsToTableHand(pairs, numPairs);

    int numThrees;
    Combination** threes = findThreeOfAKinds(cards, currentNumberOfCards, numThrees);
    addCombinationsToTableHand(threes, numThrees);

    int numStairs;
    Combination** stairs = findStairs(cards, currentNumberOfCards, numStairs);
    addCombinationsToTableHand(stairs, numStairs);

    int numFullHouses;
    Combination** fullHouses = findFullHouses(cards, currentNumberOfCards, numFullHouses);
    addCombinationsToTableHand(fullHouses, numFullHouses);

    int numStraights;
    Combination** straights = findStraights(cards, currentNumberOfCards, numStraights, false);
    addCombinationsToTableHand(straights, numStraights);

    int numFours;
    Combination** fours = findFourOfAKinds(cards, currentNumberOfCards, numFours);
    addCombinationsToTableHand(fours, numFours);

    int numStraightFlushes;
    Combination** straightFlushes = findStraightFlushes(cards, currentNumberOfCards, numStraightFlushes);
    addCombinationsToTableHand(straightFlushes, numStraightFlushes);
}

Combination** TableHand::findLargerStraight(Combination** inputStraights, int inputSize, int& outputSize){
    outputSize = 0;
    for (int i = 0; i < inputSize - 1; i++)
    {
        int j = i + 1;
        while (j < inputSize && inputStraights[i]->getValue() + 1 == inputStraights[j]->getValue())
        {
            outputSize++;
            j++;
        }
    }
    Combination** outputStraights = new Combination*[outputSize];

    int finalCombinationsSize = 0;
    for (int i = 0; i < inputSize - 1; i++)
    {
        int j = i + 1;
        while (j < inputSize && inputStraights[i]->getValue() + 1 == inputStraights[j]->getValue())
        {
            // Get all cards without duplicates
            int numCards = 0;
            Card** combination = new Card*[inputStraights[i]->getNumberOfCards() + inputStraights[j]->getNumberOfCards()];
            for (int a = 0; a < inputStraights[i]->getNumberOfCards(); a++)
                combination[numCards++] = inputStraights[i]->getCard(a);
            for (int a = 0; a < inputStraights[j]->getNumberOfCards(); a++)
            {
                Card* toAdd = inputStraights[j]->getCard(a);
                bool alreadyExists = false; // First check if the card already exists
                for (int b = 0; b < numCards; b++)
                    alreadyExists = alreadyExists || (combination[b] == toAdd);
                if (!alreadyExists)
                    combination[numCards++] = inputStraights[j]->getCard(a);
            }
            // Delete extra space
            Card** newcombination = new Card*[numCards];
            for (int b = 0; b < numCards; b++)
                newcombination[b] = combination[b];
            delete[] combination;
            if (inputStraights[j]->hasType(STRAIGHT))
                outputStraights[finalCombinationsSize++] = new Straight(newcombination, numCards);
            else //if (inputStraights[j]->hasType(STAIRS))
                outputStraights[finalCombinationsSize++] = new Stairs(newcombination, numCards);
            j++;
        }
    }
    return outputStraights;
}

Combination** TableHand::findSingles(Card** cards, int numberOfCards, int& numSingles){
    numSingles = numberOfCards;
    Combination** singles = new Combination*[numSingles];
    for (int i = 0; i < numberOfCards; i++)
        singles[i] = new Single(cards[i]);
    return singles;
}

Combination** TableHand::findPairs(Card** cards, int numberOfCards, int& numPairs){
    numPairs = 0;
    Combination** pairs = new Combination*[1000];
    for (int i = 0; i < numberOfCards - 1; i++)
    {
        int j = i + 1;
        while (j < numberOfCards && cards[i]->getValue() == cards[j]->getValue())
        {
            if (cards[i]->canBeInCombination() && cards[j]->canBeInCombination())
                pairs[numPairs++] = new Pair(cards[i], cards[j]);
            j++;
        }
    }
    return resizeArray(pairs, numPairs);
}

Combination** TableHand::findThreeOfAKinds(Card** cards, int numberOfCards, int& numThrees){
    numThrees = 0;
    Combination** threes = new Combination*[1000];
    for (int i = 0; i < numberOfCards - 2; i++)
    {
        int j = i + 1;
        while (j < numberOfCards - 1 && cards[i]->getValue() == cards[j]->getValue())
        {
            int k = j + 1;
            while (k < numberOfCards && cards[i]->getValue() == cards[k]->getValue())
            {
                if (cards[i]->canBeInCombination() && cards[j]->canBeInCombination() && cards[k]->canBeInCombination())
                    threes[numThrees++] = new ThreeOfAKind(cards[i], cards[j], cards[k]);
                k++;
            }
            j++;
        }
    }
    return resizeArray(threes, numThrees);
}

Combination** TableHand::findStairs(Card** cards, int numberOfCards, int& numStairs){
    numStairs = 0;
    Combination** stairs = new Combination*[1000];

    // Get pairs and convert them to one-stairs
    int oneStairsSize;
    Combination** pairs = findPairs(cards, numberOfCards, oneStairsSize);
    Combination** oneStairs = new Combination*[oneStairsSize];
    for (int i = 0; i < oneStairsSize; i++){
        Card** sCards =  new Card*[2]; sCards[0] = pairs[i]->getCard(0); sCards[1] = pairs[i]->getCard(1);
        oneStairs[i] = new Stairs(sCards, 2);
        delete pairs[i];
    }
    delete[] pairs;

    int stairLength = 2;
    int inputSize = oneStairsSize;
    Combination** inputStairs = oneStairs;
    while (stairLength < (numberOfCards / 2) + 1)
    {
        // Find all stairs of size stairLength
        int outputSize;
        Combination** outputStairs = findLargerStraight(inputStairs, inputSize, outputSize);

        // Keep all stairs with stairLength larger than or equal to 2
        if (stairLength >= 2)
        {
            for (int i = 0; i < outputSize; i++)
                stairs[numStairs++] = outputStairs[i];
        }
        else{
            for (int i = 0; i < inputSize; i++)
                delete inputStairs[i];
        }

        delete[] inputStairs;
        inputSize = outputSize;
        inputStairs = outputStairs;
        stairLength++;
    }
    delete[] inputStairs;

    return resizeArray(stairs, numStairs);
}

Combination** TableHand::findFullHouses(Card** cards, int numberOfCards, int& numFullHouses){
    numFullHouses = 0;
    Combination** fullHouses = new Combination*[1000];

    // Get pairs
    int pairsSize;
    Combination** pairs = findPairs(cards, numberOfCards, pairsSize);

    // Get threes of a kind
    int threesSize = 0;
    Combination** threes = findThreeOfAKinds(cards, numberOfCards, threesSize);

    for (int i = 0; i < threesSize; i++)
    {
        for (int j = 0; j < pairsSize; j++)
        {
            // Check if the two combinations contain any same card
            if (!(threes[i]->containsCard(pairs[j]->getCard(0)) || threes[i]->containsCard(pairs[j]->getCard(1))))
                fullHouses[numFullHouses++] = new FullHouse((ThreeOfAKind*)threes[i], (Pair*)pairs[j]);
        }
    }
    for (int i = 0; i < threesSize; i++)
        delete threes[i];
    delete[] threes;
    for (int j = 0; j < pairsSize; j++)
        delete pairs[j];
    delete[] pairs;
    return resizeArray(fullHouses, numFullHouses);
}

Combination** TableHand::findStraights(Card** cards, int numberOfCards, int& numStraights, bool keepAlsoFlushes){
    numStraights = 0;
    Combination** straights = new Combination*[1000];

    // Get singles and convert them to one-straights
    int oneCardStraightsSize = 0;
    for (int i = 0; i < numberOfCards; i++)
    {
        if (cards[i]->canBeInCombination()) oneCardStraightsSize++;
    }
    int singlesSize;
    Combination** singles = findSingles(cards, numberOfCards, singlesSize);
    Combination** oneCardStraights = new Combination*[oneCardStraightsSize];
    oneCardStraightsSize = 0;
    for (int i = 0; i < singlesSize; i++)
    {
        if (singles[i]->getCard(0)->canBeInCombination()){
            Card** sCards =  new Card*[1]; sCards[0] = singles[i]->getCard(0);
            oneCardStraights[oneCardStraightsSize++] = new Straight(sCards, 1);
        }
        delete singles[i];
    }
    delete[] singles;

    int straightLength = 2;
    int inputSize = oneCardStraightsSize;
    Combination** inputStraights = oneCardStraights;
    while (straightLength < numberOfCards + 1)
    {
        // Find all straights of size straightLength
        int outputSize = 0;
        Combination** outputStraights = findLargerStraight(inputStraights, inputSize, outputSize);

        // Keep all straights with straightLength larger than or equal to 5
        if (straightLength >= 5)
        {
            for (int i = 0; i < outputSize; i++)
            {
                if (keepAlsoFlushes || (!((Straight*)outputStraights[i])->cardsHaveTheSameSuit()))
                    straights[numStraights++] = outputStraights[i];
            }
        }
        else{
            for(int i = 0; i < inputSize; i++)
                delete inputStraights[i];
        }

        delete[] inputStraights;
        inputSize = outputSize;
        inputStraights = outputStraights;
        straightLength++;
    }
    delete[] inputStraights;

    return resizeArray(straights, numStraights);
}

Combination** TableHand::findFourOfAKinds(Card** cards, int numberOfCards, int& numFours){
    numFours = 0;
    Combination** fours = new Combination*[1000];
    for (int i = 0; i < numberOfCards - 3; i++)
    {
        int j = i + 1;
        while (j < numberOfCards - 2 && cards[i]->getValue() == cards[j]->getValue())
        {
            int k = j + 1;
            while (k < numberOfCards - 1 && cards[i]->getValue() == cards[k]->getValue())
            {
                int l = k + 1;
                while (l < numberOfCards && cards[i]->getValue() == cards[l]->getValue())
                {
                    if (cards[i]->canBeInBomb() && cards[j]->canBeInBomb() && cards[k]->canBeInBomb() && cards[l]->canBeInBomb())
                        fours[numFours++] = new FourOfAKind(cards[i], cards[j], cards[k], cards[l]);
                    l++;
                }
                k++;
            }
            j++;
        }
    }
    return resizeArray(fours, numFours);
}

Combination** TableHand::findStraightFlushes(Card** cards, int numberOfCards, int& numStraightFlushes){
    numStraightFlushes = 0;
    Combination** straightFlushes = new Combination*[1000];

    int straightsSize = 0;
    Combination** straights = findStraights(cards, numberOfCards, straightsSize, true);

    for (int i = 0; i < straightsSize; i++)
    {
        if (((Straight*)straights[i])->cardsHaveTheSameSuit())
            straightFlushes[numStraightFlushes++] = new StraightFlush((Straight*)straights[i]);
    }
    return resizeArray(straightFlushes, numStraightFlushes);
}

Combination** TableHand::resizeArray(Combination** oldArray, int newSize){
    Combination** newArray = new Combination*[newSize];
    for (int i = 0; i < newSize; i++)
        newArray[i] = oldArray[i];
    delete[] oldArray;
    return newArray;
}

