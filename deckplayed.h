#ifndef DECKPLAYED_H
#define DECKPLAYED_H

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include "Player.h"
#include "hand.h"
#include "table.h"
#include "card.h"

using namespace std;

class DeckPlayed{
public:

      bool cardplayed[4][13];
      bool specialcardplayed[4];
      int countCard;

                        //we make a dynamic boolean array where everything is 0, cause no card has been played yet
                        // constructs a deck where we save the played cards with dynamic allocation, we make a dynamic array KnownCards with every card
    DeckPlayed(){
        initTable();
    }

     ~DeckPlayed(){
     // destructs the DeckPlayed

     }

     void initTable(){
        for(int i = 0; i < 4; i++){
            specialcardplayed[i] = true;
            for(int j = 0; j < 13; j++){
                cardplayed[i][j] = true;
            }
        }
     }

       void HandCard(Hand* hand){
               int value;
              CardSuit suit;
                for(int i=0;i<hand->getCardCount() ; i++){
                value=hand->getCard(i)->getValue();
                suit=hand->getCard(i)->getSuit();

              switch (suit){
                   case JADE:
                   case SWORD :
                   case PAGODA:
                   case STAR:
                   cardplayed[(int)suit][value-2]=false;
                   break;
                  case PHOENIX:
                      specialcardplayed[2]=false;
                        break;
                  case DRAGON:
                      specialcardplayed[1]=false;
                        break;
                  case MAHJONG:
                      specialcardplayed[0]=false;
                        break;
                  case DOG:
                      specialcardplayed[3]=false;
                        break;
                      }
            }
       }



void  cardCounter(Table* table){
            int value;
            CardSuit suit;
            for(int i=0; i<table->getNumberOfCombinations() ; i++){
              for(int j=0; j<table->getCombination(i)->getNumberOfCards() ; j++){
                    value=table->getCombination(i)->getCard(j)->getValue();
                    suit=table->getCombination(i)->getCard(j)->getSuit();
                        switch (suit) {
                            case JADE:
                            case SWORD:
                            case PAGODA:
                            case STAR:
                                cardplayed[(int) suit][value -2] = false;
                                break;
                          case PHOENIX:
                                specialcardplayed[2] = false;
                                break;
                          case DRAGON:
                                specialcardplayed[1] = false;
                                break;
                          case MAHJONG:
                                specialcardplayed[0] = false;
                                break;
                          case DOG:
                                specialcardplayed[0] = false;
                                break;

                }
          }
        }
}

void printTable(){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            cout<<cardplayed[i][j]<<" ";
        }
        cout<<endl;
    }
}
};



#endif // DECKPLAYED_H
