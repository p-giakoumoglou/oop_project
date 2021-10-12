#ifndef TABLEHAND_H
#define TABLEHAND_H


#include <string>
#include "combination.h"

using namespace std;

/**
 * Class denoting a TableHand of a player.
 */
class TableHand
{
private:
	Card** cards;
	int currentNumberOfCards;
    Combination** allCombinations;
    int allCombinationsSize;
    bool hasPhoenix;
    bool hasDragon;
    int remainingCards;
    bool simpleTableCopy[4][13];
    int cardPerSuitCounter[13];
    int tempTotalCards;
    float bombprob;
    int bombCounter;

public:
    /**
     * Initializes the TableHand that holds the cards.
     */
	TableHand(bool table[4][13], bool tableSpecial[4]);

    /**
     * Deletes the TableHand by deleting its combinations and cards. Warning: this method
     * does not also delete each individual card! Cards are deleted by the deck.
     * However, this method deletes all combinations before deleting the combinations array.
     */
    ~TableHand();

    /**
     * Clears the TableHand for a new round by clearing all combinations and setting the
     * value of the phoenix.
     */
    void clearTableHand();

    /**
     * Returns a combination of the TableHand given its index.
     *
     * @param index the index of the combination to be returned.
     * @return a combination of the TableHand given its index.
     */
    Combination* getCombination(int index);

    void scanForPhoenix(bool tableSpecial[4]);

    void scanForDragon(bool tableSpecial[4]);

    void scanTable(bool tableSimple[4][13], bool tableSpecial[4]);

    void remakeTable(bool tableSimple[4][13]);

    void printtable();

    float calcPropabillity(Combination* comb, int noOfCards, int totalCards);

    float findCombinations(Combination* comb);



    /**
     * Returns the number of combinations of the TableHand.
     *
     * @return the number of combinations of the TableHand.
     */
    int getNumberOfCombinations();

    /**
     * Adds a card to the TableHand.
     *
     * @param card the card to be added to the TableHand.
     */
	void addCard(Card* card);

    /**
     * Removes a card from the TableHand.
     *
     * @param card the card to be removed to the TableHand.
     */
	void removeCard(Card* card);

    /**
     * Removes a combination from the TableHand. This method also removes
     * all cards present in the combination.
     *
     * @param combination the combination to be removed to the TableHand.
     */
	void removeCombination(Combination* combination);

    /**
     * Returns the number of cards that are currently in the TableHand.
     *
     * @return the number of cards of the TableHand.
     */
	int getCardCount();

    /**
     * Returns a card of the TableHand given its index.
     *
     * @param index the index of the card to be returned.
     * @return a card of the TableHand given its index.
     */
	Card *getCard(int index);

    /**
     * Sets the value of the phoenix (if the TableHand has it) to a random
     * value between 2 and 14.
     *
     * @param value the value to set the phoenix to.
     */
    void setPhoenixValue(int value);

    /**
     * Returns a string representation of the TableHand, including all cards.
     *
     * @return the TableHand of the player as string.
     */
	string toString();

    /**
     * Returns the number of cards of the TableHand as a string.
     *
     * @return the number of cards of the TableHand as string.
     */
    string numberOfCardsToString();

    /**
     * Creates all the possible combinations of the TableHand given its cards. Combinations must
     * be put in the table allCombinations and the variable allCombinationsSize must be updated.
     */
    void createAllCombinations();

private:
    void addCombinationsToTableHand(Combination** combinations, int numCombinations);

    Combination** findLargerStraight(Combination** inputStraights, int inputSize, int& outputSize);

    Combination** findSingles(Card** cards, int numberOfCards, int& numSingles);
    Combination** findPairs(Card** cards, int numberOfCards, int& numPairs);
    Combination** findThreeOfAKinds(Card** cards, int numberOfCards, int& numThrees);
    Combination** findStairs(Card** cards, int numberOfCards, int& numStairs);
    Combination** findFullHouses(Card** cards, int numberOfCards, int& numFullHouses);
    Combination** findStraights(Card** cards, int numberOfCards, int& numStraights, bool keepAlsoFlushes);
    Combination** findFourOfAKinds(Card** cards, int numberOfCards, int& numFours);
    Combination** findStraightFlushes(Card** cards, int numberOfCards, int& numStraightFlushes);

    Combination** resizeArray(Combination** oldArray, int newSize);
};

#endif // TABLE_H
