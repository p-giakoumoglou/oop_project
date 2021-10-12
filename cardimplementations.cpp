#include "card.h"
#include <iostream>

// TODO: Implement here the methods of SimpleCard and SpecialCard
SimpleCard::SimpleCard(int value, CardSuit suit):Card(value, suit){
//this->value=value;
//this->suit=suit;
}

int SimpleCard::getPoints(){
switch(value){
case 5:
    return 5;
case 10:
    return 10;
case 13:
    return 10;
default:
    return 0;
}
}

bool SimpleCard::canBeInCombination(){
    return true;
}

bool SimpleCard::canBeInBomb(){
    return true;
}




SpecialCard::SpecialCard(CardSuit suit){
this->suit=suit;
//cout << this->suit << endl;
switch(suit){
    case MAHJONG:
                    value=1;
                    break;
    case DRAGON:
                    value=15;
                    break;
    case PHOENIX:
                    value=-1;
                    break;
    case DOG:
                    value=-1;
                    break;
    default:
                    value=-1;

}
}

int SpecialCard::getPoints(){
    if(suit==DRAGON){
        return 25;
    }
    else if(suit==PHOENIX){
        return -25;
    }
    else return 0;

}

bool SpecialCard::canBeInCombination(){
    switch(suit){
        case MAHJONG:
                    return true;

        case PHOENIX:
                    return true;

        case DOG:
                    return false;

        case DRAGON:
                    return false;

        default:
                    return true;

    }
}

bool SpecialCard::canBeInBomb(){
    switch(suit){
        case MAHJONG:
                    return false;

        case PHOENIX:
                    return false;

        case DOG:
                    return false;

        case DRAGON:
                    return false;

        default:
                    return true;

    }


}


