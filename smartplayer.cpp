#include <cstdlib>
#include "player.h"
#include "shuffler.h"
#include "combination.h"
#include "hand.h"
#include "evaluation.h"
#include "deckplayed.h"
#include "tablehand.h"

DeckPlayed stalkDeck;
CombinationTree* tree;
TableHand* handy;
Node* bestLeaf;

float Aggresiveness;//the lower the value, the more aggresive
int Emergency;
int TotalEmergency;
int playBomb;
int comImportanceMultiplier;
CombinationType UltimateType;

int firstTime = 0;

int getpointsoftable(Table* table ){

int PointsOnTable=0;
//Combination* combi;
//Card* c;
int Num=0;
for(int i=0; i< table->getNumberOfCombinations() ; i++){
Num=table->getCombination(i)->getNumberOfCards();
for(int j=0 ; j<Num ; j++){
PointsOnTable+=table->getCombination(i)->getCard(j)->getPoints();
 }

}
return PointsOnTable;
}

void caclAggresiveness(int* numberOfCardsPerPlayer, Table* table, int index){
    Aggresiveness = 0.4;
    Emergency = 0;
    TotalEmergency = 0;
    comImportanceMultiplier = 1;
    if((numberOfCardsPerPlayer[index]>numberOfCardsPerPlayer[(index+1)%4]+4) || (numberOfCardsPerPlayer[index]>numberOfCardsPerPlayer[(index+3)%4]+4)){
        Aggresiveness -= 0.1;
    }
    if(numberOfCardsPerPlayer[index]){
        Aggresiveness -= 0.1;
    }
    if((numberOfCardsPerPlayer[(index+1)%4] == 1) || (numberOfCardsPerPlayer[(index+3)%4] ==  1)){
            Emergency = 1;
    }

    if(table->getTopCombination()!= NULL){
        UltimateType = table->getTopCombination()->getType();
        int tablePoints = getpointsoftable(table);
        if(tablePoints>=5){
            Aggresiveness-=0.1;
        }
        if(tablePoints>=25){
            TotalEmergency = 1;
        }
        comImportanceMultiplier = 2*(tablePoints/10)*1.5;
    }

}

void resetGlobalValues(){
    Aggresiveness = 0.4;
    Emergency = 0;
    TotalEmergency = 0;
    comImportanceMultiplier = 1;
}

float findLeafValue3(Node* pointLeaf, TableHand *handTable){
    float importance[8] = {0.5, 0.5, 1, 2, 2, 4, 7, 10};
    if(comImportanceMultiplier > 1){
        importance[UltimateType]*=comImportanceMultiplier;
    }
    float LeafValue = 0;
    for(int i=0;i<pointLeaf->getCombinationSize();i++){
            float temp = importance[(int) pointLeaf->getCombination(i)->getType()];
            LeafValue+=( 1 - handTable->findCombinations(pointLeaf->getCombination(i))/100)*(temp)*(pointLeaf->getCombination(i)->getValue());
    }
    return LeafValue;
}

Node* PowerRanger3(Hand* hand, TableHand *handTable, CombinationTree* tree){
    for(int i=0; i < tree->getNumberOfleaves(); i++){
       float temp = findLeafValue3(tree->leaves[0], handTable);
       tree->leaves[i]->setValue(temp);
     }
     float BestValue = tree->leaves[0]->getValue();
     int counter=0;
     for(int i=0; i < tree->getNumberOfleaves()-2; i++){
        if(BestValue < tree->leaves[i+1]->getValue()){
                BestValue = tree->leaves[i+1]->getValue();
                counter = i+1;
        }
     }
    return tree->leaves[counter];

}

float findCombValue(Combination* comb, TableHand *handTable){
    float importance[8] = {0.5, 0.5, 1, 2, 2, 4, 7, 10};
    float combValue = 0;
    float temp = importance[(int) comb->getType()];
    combValue = ( 1 - handTable->findCombinations(comb)/100)*(temp)*comb->getValue();
    return combValue;
}


SmartPlayer::SmartPlayer(int index) : Player(index){
    team = "Team 004"; // TODO: Change this to your team number e.g. "Team 150"
}

SmartPlayer::~SmartPlayer(){

};

int SmartPlayer::setPhoenixValue(Table* table, PlayerStatus* playerStatuses, int* numberOfCardsPerPlayer, Combination* lastCombinationOnTable){
    // TODO: Implement this function (some useful objects that you may use are the cards that can be retrieved from the hand
    // or the playable combinations that are given as a parameter)
    /// This function MUST return an integer value from 1 to 14
    watch(table, playerStatuses, numberOfCardsPerPlayer);
    Hand** caseHand = new Hand*[14];
    for(int i = 0; i < 14; i++){
        caseHand[i] = new Hand();
    }
    for(int i = 0; i < 14; i++){
            for(int j = 0; j < hand->getCardCount(); j++){
                caseHand[i]->addCard(hand->getCard(j));
            }
            caseHand[i]->setPhoenixValue(i+1);
            caseHand[i]->createAllCombinations();
    }
    int bestPhoenixValue = 0;
    for(int i = 0; i < 14; i ++){
        tree = new CombinationTree(caseHand[i]);
        handy = new TableHand(stalkDeck.cardplayed, stalkDeck.specialcardplayed);
        bestLeaf = PowerRanger3(caseHand[i], handy, tree);
        float bestValue = -9999;
        if(bestValue < bestLeaf->getValue()){
            bestValue = bestLeaf->getValue();
            bestPhoenixValue = i + 1;
        }
        delete tree;
        delete handy;
    }
    resetGlobalValues();
    return bestPhoenixValue;
}

int SmartPlayer::decideAndPlay(Combination** playableCombinations, int numPlayableCombinations, Table* table, PlayerStatus* playerStatuses, int* numberOfCardsPerPlayer, Combination* lastCombinationOnTable){
    // TODO: Implement this function (some useful objects that you may use are the cards that can be retrieved from the hand
    // or the combinations that are currently on the table and can be retrieved from the table or the number of cards or the statuses of other players)
    /// After that, it MUST return an integer index to the combinations available in this dynamic array (a Combination* object).
    /// This function can also return -1 if the player wishes to pass, however -1 CANNOT BE RETURNED if the last given
    /// combination (the 'Combination* lastCombinationOnTable' that is given as a parameter) is NULL
    if(lastCombinationOnTable != NULL){
    watch(table, playerStatuses, numberOfCardsPerPlayer);
    if(TotalEmergency == 1 || Emergency == 1){
        int temp = -9999;
        int tempIndex = 0;
        for(int i = 0; i < numPlayableCombinations; i++){
            if(playableCombinations[i]->getValue() > temp){
                temp = playableCombinations[i]->getValue();
                tempIndex = i;
            }
        }
        resetGlobalValues();
        return tempIndex;
    }
    tree = new CombinationTree(hand);
    handy = new TableHand(stalkDeck.cardplayed, stalkDeck.specialcardplayed);
    bestLeaf = PowerRanger3(hand, handy, tree);

    int lowestValue = 9999;
    int temp1 = 0;
    for(int i = 0; i < bestLeaf->getCombinationSize(); i++){
        if((bestLeaf->getCombination(i)->canBePlayed(lastCombinationOnTable)) && (bestLeaf->getCombination(i)->getValue() < lowestValue )){
            lowestValue = bestLeaf->getCombination(i)->getValue();
            temp1 = i;
        }
    }
    int temp2 = -1;
    for(int i = 0; i < numPlayableCombinations; i++){
        if(playableCombinations[i]->getType() == bestLeaf->getCombination(temp1)->getType() && playableCombinations[i]->getValue() == bestLeaf->getCombination(temp1)->getValue()){
                temp2 = i;
        }
    }
    if(playerStatuses[(index + 3) % 4] == PASSED && playerStatuses[(index + 2) % 4] == PLAYED && temp2 != -1){
        if((lastCombinationOnTable->getType() == SINGLE || lastCombinationOnTable->getType() == PAIR || lastCombinationOnTable->getType() == THREEOFAKIND) && (playableCombinations[temp2]->getValue() < (lastCombinationOnTable->getValue() + 3)) && (lastCombinationOnTable->getValue() < 10)){
            delete handy;
            delete tree;
            resetGlobalValues();
            return temp2;
        }else{
            delete handy;
            delete tree;
            resetGlobalValues();
            return -1;
        }
    }
    float value = bestLeaf->getValue();
    float newValue = value - findCombValue(bestLeaf->getCombination(temp1), handy);
    delete handy;
    delete tree;
    if(newValue > value*Aggresiveness){
            resetGlobalValues();
            return temp2;
    }else{
        resetGlobalValues();
        return -1;
    }
    }else if(lastCombinationOnTable == NULL){
            if(Emergency == 1){
                int temp = -9999;
                int tempIndex = 0;
                for(int i = 0; i < numPlayableCombinations; i++){
                    if(playableCombinations[i]->getValue() > temp){
                        temp = playableCombinations[i]->getValue();
                        tempIndex = i;
                    }
                }
                resetGlobalValues();
                return tempIndex;
            }
        for(int i = 0; i < numPlayableCombinations; i++){
        if(playableCombinations[i]->getCard(0)->getSuit()==DOG && playableCombinations[i]->getCard(0)->getSuit()!= MAHJONG){//play dog if you have dog
            resetGlobalValues();
            return i;
        }
        watch(table, playerStatuses, numberOfCardsPerPlayer);
        tree = new CombinationTree(hand);
        handy = new TableHand(stalkDeck.cardplayed, stalkDeck.specialcardplayed);
        bestLeaf = PowerRanger3(hand, handy, tree);

        int lowestValue = 9999;
        int temp1 = 0;
        for(int i = 0; i < bestLeaf->getCombinationSize(); i++){
            if(bestLeaf->getCombination(i)->getValue() < lowestValue){
                lowestValue = bestLeaf->getCombination(i)->getValue();
                temp1 = i;
            }
        }
        int temp2 = 0;
        for(int i = 0; i < numPlayableCombinations; i++){
            if(playableCombinations[i]->equals(bestLeaf->getCombination(temp1))){
                    temp2 = i;
            }
        }
        delete handy;
        delete tree;
        resetGlobalValues();
        return temp2;

    }
    }
    return rand() % numPlayableCombinations;
}

void SmartPlayer::watch(Table* table, PlayerStatus* playerStatuses, int* numberOfCardsPerPlayer){
    // TODO: This function is optional. It is called whenever the player cannot play in order to watch the game.
    /// Use this to watch what cards/combinations are played, or just simply leave it empty.
    //DeckPlayed mpampis;
    //this->CardKnown = mpampis.CARDCOUNTER();///push or something

    /*if(firstTime == 0){
        firstTime = 1;
        InitializeBoolTable();
    }
    if(table->getTopCombination()!=NULL){
        for(int i = 0; i < table->getNumberOfCombinations(); i++){
        fillBoolTable(table->getCombination(i));
        }
    }

    cout<<"BOOL TABLE "<<endl;
    printBoolTable();
    */
    caclAggresiveness(numberOfCardsPerPlayer, table, this->index);
    if(firstTime == 0){
        firstTime = 1;
        stalkDeck.HandCard(hand);
    }
    stalkDeck.cardCounter(table);

}


/*void SmartPlayer::calcAggresiveness(int* numberOfCardsPerPlayer){
    aggresiveness = 0.3;
    emergency = 0;
    if(bestLeaf->getValue()>100){
        aggresiveness -=0.1;
    }
    if((numberOfCardsPerPlayer[index]>numberOfCardsPerPlayer[(index+1)%4]+4) || (numberOfCardsPerPlayer[index]>numberOfCardsPerPlayer[(index+3)%4]+4)){
        aggresiveness -= 0.1;
    }
    if(numberOfCardsPerPlayer[index]){
        aggresiveness -= 0.1;
    }
    if((numberOfCardsPerPlayer[(index+1)%4]+4 == 1) || (numberOfCardsPerPlayer[index]>numberOfCardsPerPlayer[(index+3)%4]+4 == 1)){
            emergency = 1
    }

    }

void SmartPlayer::InitializeBoolTable(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            tableCardPlayed[i][j] = 1;
        }
    }
    for(int i = 0; i < 4; i++){
        tableSpecialPlayed[i] = 1;
    }
    for(int i = 0; i < this->hand->getCardCount(); i++)
        fillBoolTable(this->hand->getCard(i));
}


void SmartPlayer::fillBoolTable(Card* card){
    if((card->getSuit() != PHOENIX) && (card->getSuit() != DOG) && (card->getSuit() != DRAGON) && (card->getSuit() != MAHJONG)){
        tableCardPlayed[card->getSuit()][card->getValue()-2] = 0;
    }
    else{
        tableSpecialPlayed[card->getSuit()%4] = 0;
    }
}

void SmartPlayer::fillBoolTable(Combination* comb){
    for(int i = 0; i < comb->getNumberOfCards(); i++){
        fillBoolTable(comb->getCard(i));
    }


}

void SmartPlayer::printBoolTable(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            cout<<tableCardPlayed[i][j]<<" ";
        }
        cout<<endl;
    }
}
*/

