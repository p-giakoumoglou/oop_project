#include <iostream>
#include <cstdlib>
#include <ctime>
#include "table.h"
#include "deck.h"
#include "player.h"
#include "game.h"
#include "Node.h"
#include "tablehand.h"
#include "deckplayed.h"

using namespace std;

DeckPlayed deckplayed;



float findLeafValue(Node* pointLeaf, TableHand *handTable){
    float importance[8] = {0.5, 0.5, 1, 2, 2, 4, 7, 10};
    float LeafValue = 0;

        for(int i=0;i<pointLeaf->getCombinationSize();i++){
            float temp = importance[(int) pointLeaf->getCombination(i)->getType()];
            LeafValue+=( 1 - handTable->findCombinations(pointLeaf->getCombination(i))/100)*(temp)*pointLeaf->getCombination(i)->getValue();
        }
    return LeafValue;
}


Node* PowerRanger(Hand* hand, TableHand handTable, CombinationTree* tree){

    Node** leaves = tree->getLeaves();

    for(int i=0; i < tree->getNumberOfleaves(); i++){
       float temp = findLeafValue(leaves[i], &handTable);
       leaves[i]->setValue(temp);
       //cout << "LEAF "<<i<<" VALUE IS "<<leaves[i]->getValue()<<endl;
     }
     float BestValue = leaves[0]->getValue();
     int counter=0;
     for(int i=0; i < tree->getNumberOfleaves()-1; i++){
        if(BestValue < leaves[i+1]->getValue()){
                BestValue = leaves[i+1]->getValue();
                counter = i+1;
        }
     }
    return leaves[counter];
}

int main(){
    // Initialize game
    srand(time(NULL));
    //srand(5);
    Table* table = new Table();
    ///create a test hand
    /*
    SimpleCard card1(5, JADE), card2(12, SWORD), card3(13, JADE), card4(5, JADE), card5(7, JADE), card6(8, SWORD), card7(2, JADE), card8(2, SWORD), card9(2, PAGODA), card10(8, SWORD), card11(9, SWORD), card12(10, SWORD), card13(11, SWORD), card14(12, SWORD);
    SpecialCard cardSpecial(PHOENIX);
    Hand handTest;
    Hand handTest2;
    handTest.addCard(&card1);
    handTest.addCard(&card2);
    handTest.addCard(&card3);
    handTest.addCard(&cardSpecial);
    handTest.setPhoenixValue(6);
    //handTest.addCard(&card4);
    //handTest.addCard(&card5);
    //handTest.addCard(&card6);
    //handTest.addCard(&card7);
    //handTest.addCard(&card8);
    //handTest.addCard(&card9);
    handTest2.addCard(&card10);
    handTest2.addCard(&card11);
    handTest2.addCard(&card12);
    handTest2.addCard(&card13);
    handTest2.addCard(&card14);
    handTest.createAllCombinations();
    Hand* pointHand = &handTest;

    cout<<handTest.toString()<<endl;
    cout<<endl;
    CombinationTree tree;
    tree.CreateAragorn(pointHand);
    tree.printLeaves();
    Node** leaves = tree.getLeaves();
    cout<<"Total Nodes "<<tree.getCounter()<<endl;
    cout<<"Total leaves "<<tree.getNumberOfleaves()<<endl;
    cout<<"No of Combinations "<<leaves[0]->getCombinationSize()<<endl;
    cout<<"hand: "<<tree.leaves[0]->getHand()->toString();
    cout<<"A random comb: "<<endl;
    cout<<tree.leaves[0]->getCombination(0)->toString()<<endl;
    tree.clearTree();
    cout<<"The first tree has been destroyed"<<endl;
    pointHand = &handTest2;
    tree.CreateAragorn(pointHand);
    tree.printLeaves();
    //delete pointHand;
    //delete tree;
    //exit(0);

    ///
    ///test deck played
    /*DeckPlayed newDeck;
    newDeck.HandCard(&handTest);
    newDeck.printTable();
    */
    ///
    ///test find combination
    /*bool tableSimple[4][13] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {1, 1, 1, 1 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,1, 1},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0}};

    bool tableSpecial[4] = {0, 0, 0, 0};
    TableHand handy(tableSimple, tableSpecial);
    cout<<"Cards that have not be played:"<<endl;
    cout<<handy.toString()<<endl;
    cout<<"Our combination: "<<leaves[0]->getCombination(0)->toString()<<endl;
    cout<<"The bool table: "<<endl;
    handy.printtable();
    cout<<"CAN BE DEFEATED BY: " <<handy.findCombinations(leaves[0]->getCombination(0));
    cout<<endl;
    exit(0);
    */
    ///
    ///test power ranger
    /*
        deckplayed.HandCard(&handTest);
        CombinationTree treeky(&handTest);
        TableHand handy(deckplayed.cardplayed, deckplayed.specialcardplayed);
        //cout<<findLeafValue(leaves[0], &handy)<<endl;
        //cout<<findLeafValue(leaves[1], &handy)<<endl;
        Node* bestLeaf = PowerRanger(&handTest, handy, &treeky);
        cout<<"AND THE BEST LEAF IS... "<<endl;
        for(int i = 0; i < bestLeaf->getCombinationSize(); i++){
            cout<<bestLeaf->getCombination(i)->toString()<<endl;
        }
        exit(0);
    */
    ///

    Deck* deck = new Deck();
    Player** players = new Player*[4];
    players[0] = new SmartPlayer(0);
    players[1] = new ComputerPlayer(1);
    players[2] = new SmartPlayer(2);
    players[3] = new ComputerPlayer(3);
    Game* game = new Game(table, deck, players);



    // Deal and start round
    int playerTurn = game->startRound();
    Player* currentPlayer = players[playerTurn];
    int idOfLastPlayerThatHasPlayed = -1;
    bool dogHasBeenPlayed = false;
    currentPlayer->setStatus(WONTHETRICK);

    game->printState();

    cout << "NEW TRICK" << endl;
    do{
        currentPlayer = players[playerTurn];

        // Check if last three players passed
        if (game->otherPlayersPassed(idOfLastPlayerThatHasPlayed) &&
                players[idOfLastPlayerThatHasPlayed]->hasStatus(PLAYED)){
            cout << players[idOfLastPlayerThatHasPlayed]->getName() << " won the trick" << endl;

            Player* playerToGetTheCardsOfTheTable = players[idOfLastPlayerThatHasPlayed];
            if (table->getTopCombination()->getCard(0)->hasSuit(DRAGON)){
                // If the player has won the trick with a dragon then the trick must be given to an opponent
                Player* opponent1 = players[(playerTurn + 1) % 4];
                Player* opponent2 = players[(playerTurn + 3) % 4];
                // The cards are given to the opponent with the most cards in hand
                playerToGetTheCardsOfTheTable = (opponent1->getNumberOfCards() > opponent2->getNumberOfCards()) ?
                                                opponent1 : opponent2;
            }
            for (int j = 0; j < table->getNumberOfCombinations(); j++){
                Combination *temp = table->getCombination(j);
                playerToGetTheCardsOfTheTable->addCombinationToBucket(temp);
            }
            table->clear();

            if (players[idOfLastPlayerThatHasPlayed]->hasNoCardsLeft())
                players[idOfLastPlayerThatHasPlayed]->setStatus(HASNTPLAYED); // The player that won the trick has no cards left
            else{
                players[idOfLastPlayerThatHasPlayed]->setStatus(WONTHETRICK); // The player that won the trick has cards left
                for (int i = 0; i < 4; i++){
                    if (i != idOfLastPlayerThatHasPlayed)
                        players[i]->setStatus(HASNTPLAYED);
                }
            }

            cout << endl << "NEW TRICK" << endl;
        }
        cout << "Turn of " << currentPlayer->toString() << endl;

        if (currentPlayer->hasNoCardsLeft()){
            cout << currentPlayer->getName() << " has no cards left" << endl;
            currentPlayer->setStatus(PASSED);
        }
        else{
            // In each turn the player can see the combinations of the table, the statuses
            // of all players and the number of cards of all players
            PlayerStatus* playerStatuses = new PlayerStatus[4];
            int* playerCards = new int[4];
            for (int i = 0; i < 4; i++){
                playerStatuses[i] = players[i]->getStatus();
                playerCards[i] = players[i]->getNumberOfCards();
            }
            // Play a combination or pass
            // Get the last combination of the table (or NULL if the table has no combinations)
            Combination *lastComb = (table->getNumberOfCombinations() == 0) ? NULL : table->getTopCombination();
            Combination *currentComb = NULL;
            if (currentPlayer->hasPhoenix()) // set the value of the phoenix if the player has it
                currentPlayer->setTheValueOfThePhoenix(table, playerStatuses, playerCards);
            currentPlayer->createHandCombinations(); // create all possible combinations for the player
            // Check if the player has to play and if the player can play
            if (table->getNumberOfCombinations() == 0 || currentPlayer->findNumberOfPlayableCombinations(lastComb) > 0){
                currentComb = currentPlayer->playCombinationOrPass(table, playerStatuses, playerCards);
            }
            else{
                currentPlayer->watch(table, playerStatuses, playerCards);
            }
            delete[] playerStatuses;

            if (currentComb != NULL){ // Player has played
                dogHasBeenPlayed = currentComb->getCard(0)->hasSuit(DOG); // check if dog has been played
                // check if phoenix has been played as a single
                if (currentComb->hasType(SINGLE) && currentComb->getCard(0)->hasSuit(PHOENIX)){
                    // set phoenix to the value of the last single card
                    if (dogHasBeenPlayed || table->getNumberOfCombinations() == 0)
                        ((Single*)currentComb)->setValue(1);
                    else
                        ((Single*)currentComb)->setValue(table->getTopCombination()->getCard(0)->getValue());
                }

                currentPlayer->removeCombinationFromHand(currentComb);
                table->addCombinationOnTable(currentComb);
                cout << currentPlayer->getName() << " played: " << currentComb->toString() << endl;
                currentPlayer->setStatus(PLAYED);
                idOfLastPlayerThatHasPlayed = playerTurn;
                for (int i = 0; i < 4; i++){
                    if (i != idOfLastPlayerThatHasPlayed)
                        players[i]->setStatus(HASNTPLAYED);
                }
            }
            else{ // Player has passed
                cout << currentPlayer->getName() << " passed" << endl;
                currentPlayer->setStatus(PASSED);
            }

            // Check if player has no cards left
            if (currentPlayer->hasNoCardsLeft()){
                cout << currentPlayer->getName() << " has no cards left" << endl;
                game->playerCardsHaveEnded(playerTurn);
                // If the partner of the player has no cards left then get the trick and game over
                if (game->partnerHasNoCardsLeft(playerTurn)){
                    // Get the rest of the cards
                    for (int j = 0; j < table->getNumberOfCombinations(); j++){
                        Combination *temp = table->getCombination(j);
                        currentPlayer->addCombinationToBucket(temp);
                    }
                    table->clear();

                    cout << endl << "Game Over!" << endl;
                    break;
                }
            }
        }

        if (dogHasBeenPlayed){
            currentPlayer->setStatus(HASNTPLAYED);
            playerTurn = (playerTurn + 2) % 4; // if the dog has been played then the partner plays
            idOfLastPlayerThatHasPlayed = playerTurn;
            players[playerTurn]->setStatus(PLAYED);
            dogHasBeenPlayed = false;
        }
        else
            playerTurn = (playerTurn + 1) % 4; // else the next player plays
    }
    while(true);


    game->endRound();

    // Print ranking
    int* ranking = game->getRanking();
    cout << endl << "Ranking: " << endl;
    for (int i = 0; i < 4; i++){
        cout << players[ranking[i]]->getName() << endl;
    }

    // Print score
    cout << endl << "Score: " << endl;
    cout << players[0]->getName() << " and " << players[2]->getName() << " - " << game->getTeamPoints(0) << endl;
    cout << players[1]->getName() << " and " << players[3]->getName() << " - " << game->getTeamPoints(1) << endl;

    // Delete all objects
    delete table;
    delete deck;
    for (int i = 0; i < 4; i++)
        delete players[i];
    delete[] players;
    delete game;

    return 0;
}
