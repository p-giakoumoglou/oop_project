#ifndef NODE_H
#define NODE_H
#include "hand.h"
#include "combination.h"
#include "player.h"
#include <iostream>
#include <cstdlib>

using namespace std;

class Node
{
    Hand *hand; //the hand with the removed cards till the level of the node
    Combination **comb;// an array of pointers to the combination we have thus far
    Node *parent;
    Node **children;
    int combinationSize;
    int totalChildren;
    bool root;
    bool leaf;
    float value;


    public:

        Node(Hand *hand){//constructor root
            this->hand = new Hand();///NEW HERE
            //(*this->hand) = (*hand);
            for(int i = 0; i < hand->getCardCount(); i++){
                this->hand->addCard(hand->getCard(i));
            }
            this->hand->createAllCombinations();
            root = 1;
            leaf = 0;
            parent = 0;
            combinationSize = 0;
            comb = 0;
            children = new Node*[20];///NEW HERE, the maximum number of children is 11 but i am afraid to change this
            totalChildren = 0;
        }


        Node(Node *father, Combination *newComb){//constructor for other stuff
            parent = father;

            ///for hand:
            this->hand = new Hand();///NEW HERE
            for(int i = 0; i < parent->getHand()->getCardCount(); i++){
                this->hand->addCard(parent->getHand()->getCard(i));
            }
            for(int i = 0; i < newComb->getNumberOfCards(); i++){
                this->hand->removeCard(newComb->getCard(i));
            }

            this->hand->createAllCombinations();

            ///for comb
            this->comb = new Combination*[parent->getCombinationSize() + 1];///NEW HERE

            if(parent->hasParent()==0){
                this->comb[0] = newComb; //if parent is root
            }else{
            for(int i = 0; i < father->getCombinationSize(); i++){
                this->comb[i]=father->getCombination(i);//take all the combinations of the father
            }
            }
            this->comb[father->getCombinationSize()] = newComb;
            combinationSize = 1 + father->getCombinationSize();


            children = new Node*[20]; ///NEW HERE
            totalChildren = 0;
            root = 0;


        }


        ~Node(){
            delete[] children;//sad
            delete[] comb;
            //delete hand;
        };

        void addChild(Node* offspring){
                offspring->parent = this;
                children[totalChildren] = offspring;
                totalChildren++;
        }

        bool hasParent(){
            if(parent != 0){
                root = 0;
                return 1;
            }
            root = 1;
            return 0;
        }

        bool hasChildren(){
            if(totalChildren == 0){
                leaf = 1;
                return 0;
            }
            leaf = 0;
            return 1;
        }

        bool isBrother(Node brother){
            if(brother.parent == this->parent) return 1;
            return 0;
        }


        Combination* getCombination(int index){
            return comb[index];
        }

        Node* getParent(){
            return parent;
        }

        Hand* getHand(){
            return hand;
        }

        void setLeaf(bool i){
            leaf = i;
        }

        void addCombination(Combination* single){
            Combination **tempComp;
            combinationSize += 1;
            tempComp = new Combination*[combinationSize];
            for(int i = 0; i < combinationSize - 1; i++)
                tempComp[i] = this->comb[i];
            delete[] this->comb;
            tempComp[combinationSize-1] = single;
            this->comb = tempComp;
        }

        int getCombinationSize(){
            return combinationSize;
        }

        void setValue(float i){
        value = i;
        }


        float getValue(){
            return value;
        }

        int getTotalChildren(){
            return totalChildren;
        }

};



class CombinationTree{

    int leafCounter;
    int counter;
    Node** allNode;
    //Node** leaves;


public:
    Node** leavesTable;
    Node** leaves;

    CombinationTree(){
        leafCounter = 0;
        counter = 0;
        allNode = new Node*[100000];
        leaves = new Node*[100000];
        leavesTable = new Node*[100000];
    }

    CombinationTree(Hand *hand){
        leafCounter = 0;
        counter = 0;
        allNode = new Node*[100000];
        leaves = new Node*[100000];
        leavesTable = new Node*[100000];

        Node* Aragorn = new Node(hand);//o mpampas olwn
        this->createTree(Aragorn);
        delete Aragorn;//the King has fallen
    }

    /*void CreateAragorn(Hand *hand){
        Node* Aragorn = new Node(hand);//o mpampas olwn
        this->createTree(Aragorn);
        delete Aragorn;//the King has fallen
    }*/

    ~CombinationTree(){
        //for(int i = 0; i < counter; i++)
        //    delete allNode[i];
        delete[] allNode;
        delete[] leaves;
    }

 void createTree(Node *root){//basically sub-root but I am bored to change the name

        bool firstPAIR = 0, firstTHREEOFAKIND = 0, firstSTAIRS = 0, firstFULLHOUSE = 0,  firstFOUROFAKIND = 0;
        //bool firstSTRAIGHT = 0, firstSTRAIGHTFLUSH = 0;
        bool firstQUEEN = 0, firstKING = 0, firstACE = 0;
        for(int i = 0; i < root->getHand()->getNumberOfCombinations(); i++){
            if(root->getHand()->getCombination(i)->hasType(SINGLE) == 1 && root->getHand()->getCombination(i)->getValue() != 12 && root->getHand()->getCombination(i)->getValue() != 13 && root->getHand()->getCombination(i)->getValue() != 14){
                continue;
            }
            if(root->getHand()->getCombination(i)->hasType(SINGLE) == 1 && firstQUEEN == 0 && root->getHand()->getCombination(i)->getValue() == 12){///QUEEN
                firstQUEEN = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i));
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }

            if(root->getHand()->getCombination(i)->hasType(SINGLE) == 1 && firstKING == 0 && root->getHand()->getCombination(i)->getValue() == 13){///KING
                firstKING= 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i));
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
            if(root->getHand()->getCombination(i)->hasType(SINGLE) == 1 && firstACE == 0 && root->getHand()->getCombination(i)->getValue() == 14){///ACE
                firstACE = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i));
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }

            if(root->getHand()->getCombination(i)->hasType(PAIR) == 1 && firstPAIR == 0){///PAIR
                firstPAIR = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i));
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
            if(root->getHand()->getCombination(i)->hasType(THREEOFAKIND) == 1 && firstTHREEOFAKIND == 0){///THREEOFAKIND
                firstTHREEOFAKIND = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i) );
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
            if(root->getHand()->getCombination(i)->hasType(STAIRS) == 1 && firstSTAIRS == 0){///STAIRS
                firstSTAIRS = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i) );
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
            if(root->getHand()->getCombination(i)->hasType(FULLHOUSE) == 1 && firstFULLHOUSE == 0){///FULLHOUSE
                firstFULLHOUSE = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i) );
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
            if(root->getHand()->getCombination(i)->hasType(STRAIGHT) == 1){///STRAIGHT
                //firstSTRAIGHT = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i) );
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
            if(root->getHand()->getCombination(i)->hasType(FOUROFAKIND) == 1 && firstFOUROFAKIND == 0){///FOUROFAKIND
                firstFOUROFAKIND = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i) );
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
            if(root->getHand()->getCombination(i)->hasType(STRAIGHTFLUSH) == 1){///STRAIGHTFLUSH
                //firstSTRAIGHTFLUSH = 1;
                allNode[counter] = new Node(root, root->getHand()->getCombination(i) );
                root->addChild(allNode[counter]);
                createTree(allNode[counter++]);
            }
        }

        int counterOfsingles = 0;
        for(int i = 0; i < root->getHand()->getNumberOfCombinations(); i++){
            if(root->getHand()->getCombination(i)->hasType(SINGLE) == 1){
                counterOfsingles ++;
            }
        }
        if(counterOfsingles==root->getHand()->getNumberOfCombinations()){
            /*for(int i = 0; i < root->getHand()->getNumberOfCombinations(); i++ ){
                //root->addCombination(root->getHand()->getCombination(i));
            }*/
        if(counterOfsingles > 0 && firstACE == 0 && firstQUEEN == 0 && firstKING == 0){
            allNode[counter] = new Node(root, root->getHand()->getCombination(0) );
            root->addChild(allNode[counter]);
            createTree(allNode[counter++]);
            }
        if(counterOfsingles==0){
            root->setLeaf(1);
            leaves[leafCounter++] = root;
            //cout<<"YOLO "<<leaves[0]->getCombination(3)->toString()<<endl;
            //cout<<"COMB SIZE "<<this->leaves[0]->getCombinationSize()<<endl;
            counter++;
            }
        }
        if(counter > 100000)
            cout<<"MEMORY EXCEDEED ALERT"<<endl;


    }


    /*void clearTree(){
        delete[] allNode;
        allNode = new Node*[100000];
    }*/

    int getCounter(){
        return counter;
    }

    Node** getLeaves(){
        return leaves;
    }

    int getNumberOfleaves(){
        return leafCounter;
    }

    void printLeaves(){
        cout<<"Leaf combinations: "<<endl;
        for(int i = 0; i < leafCounter; i++){
            cout<<"Leaf "<<i<<":"<<endl;
            for(int j = 0; j < leaves[i]->getCombinationSize(); j++){
                cout<<this->leaves[i]->getCombination(j)->toString()<<endl;
            }
            cout<<endl;
        }
    }



};




#endif // NODE_H
