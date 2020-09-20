#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Suit {
	Clubs=1, Diamonds=1<<1, Hearts=1<<2, Spades=1<<3, Suit_End=1<<4
};
typedef enum Suit Suit;

// Sorry for the Two=0
enum Pips {
	Two=0, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace, NumberOfPips
};
typedef enum Pips Pips;


struct Card {
	Suit suit;
	Pips pips;	
};
typedef struct Card Card;

void swapCards( Card* card1, Card* card2)
{
	Card buf;
	buf.suit = card1->suit;
	buf.pips = card1->pips;

	card1->suit = card2->suit;
	card1->pips = card2->pips;

	card2->suit = buf.suit;
	card2->pips = buf.pips;
}

#define NoOfCardsInDeck 52
struct Deck {
	Card cards[NoOfCardsInDeck];
	int topOfDeck;
};
typedef struct Deck Deck;

void initDeck(Deck* deck)
{
	int cardCtr = 0;
	//For the suits a somewhat unconvential loop is need, since the values are not contigious.
	for(int suitCtr = Clubs; suitCtr < Suit_End; suitCtr<<=1)
	{
		printf("----------------------------------------------------------------\n");
		printf("The suitCtr counter is %d.\n", suitCtr);
		for(int pipsCtr = 0; pipsCtr < NumberOfPips; ++pipsCtr)
		{
			printf("Accessing cards index: %d\n", cardCtr);
			deck->cards[cardCtr].suit = (Suit)suitCtr;
			deck->cards[cardCtr].pips = (Pips)pipsCtr;
			++cardCtr;

		}
	}
	deck->topOfDeck = NoOfCardsInDeck - 1;
}

void shuffleDeck(Deck* deck)
{
	int indexOfCardToSwap = 0;
	for(int i=0; i<NoOfCardsInDeck; ++i)
	{
		indexOfCardToSwap = rand() % NoOfCardsInDeck;
		swapCards(&deck->cards[i],&deck->cards[indexOfCardToSwap]);
	}
}

struct Hand
{
	// This data structure encodes the information which particular cards are in a hand.
	// The array index corresponds to the pip value of a card.
	// The array value will be the sum of the corresponding suit values, which are
	// present in the hand.
	// Examples:
	// The hand does not have a TWO:
	// availableCards[Two] = 0;
	// The hand has one queen, which is Hearts:
	// availableCards[Queen] = Hearts;
	// The hand has two tens, Diamonds and Clubs:
	// availableCards[Ten] = Diamonds + Clubs;
	int availableCards[NumberOfPips];
};
typedef struct Hand Hand;
#define NoOfCardsPerHand 7

void initHand(Hand* hand)
{
	int idx = 0;
	for( ; idx<NumberOfPips; ++idx)
	{
		hand->availableCards[idx] = 0;
	}
}

void resetHand(Hand* hand)
{
	initHand(hand);
}

int nextPip(Pips initialPips)
{
	Pips nextPips = initialPips + 1;
	if(NumberOfPips==nextPips){
		nextPips=Two;
	}
	return nextPips;
}

int checkStraightFlushFrom(Hand* hand, Pips startingPips)
{
	int checkValue = hand->availableCards[startingPips];
	Pips currentPips = startingPips;
	for(int numberOfCardsChecked=1; numberOfCardsChecked<5; ++numberOfCardsChecked)
	{
		if(0==checkValue) {
			break;
		}
		currentPips = nextPip(currentPips);
		checkValue &= hand->availableCards[currentPips];
	}
	return checkValue;
}


// Return value will be > 0 if it is a royal flush
// and 0 if it is not.
int isRoyalFlush(Hand* hand)
{
	return checkStraightFlushFrom(hand,Ten);
}

int isStraightFlush(Hand* hand)
{
	for(Pips startingPip=Two; startingPip<NumberOfPips;++startingPip)
	{
		int result = checkStraightFlushFrom(hand,startingPip);
		if(result>0) {
			return result;
		}
	}
	return 0;
}

/* Implementation of checks for encoded integer values as used in hand->availableCards array.*/

// Clubs=1, Diamonds=1<<1, Hearts=1<<2, Spades=1<<3, Suit_End=1<<4


// Will return 1, if the encodedPipsValue contains the given suit.
// Otherwise it will return 0 
int hasSuit(Suit suit, int encodedPipsValue)
{
	if(encodedPipsValue & suit)
	{
		return 1;
	}
	return 0;
}

/* Will Return 1 if there are more than 4 cards of a single suit. Otherwise it will return 0. */
int isFlush(Hand* hand)
{
	int numberOfClubs = 0;
	int numberOfDiamonds = 0;
	int numberOfHearts = 0;
	int numberOfSpades = 0;
	for(Pips currentPip=Two; currentPip<NumberOfPips;++currentPip)
	{
		numberOfClubs += hasSuit(Clubs,hand->availableCards[currentPip]);
		numberOfDiamonds += hasSuit(Diamonds,hand->availableCards[currentPip]);
		numberOfHearts += hasSuit(Hearts,hand->availableCards[currentPip]);
		numberOfSpades += hasSuit(Spades,hand->availableCards[currentPip]);
	}
	if( numberOfClubs > 4 || numberOfDiamonds > 4 || numberOfHearts > 4 || numberOfSpades > 4)
	{
		return 1;
	}
	return 0;
}

void addCardToHand(Hand* hand, Card* card)
{
	hand->availableCards[card->pips] += card->suit;
}

void giveOutHand(Deck* deck, Hand* hand)
{
	resetHand(hand);
	if( deck->topOfDeck < NoOfCardsPerHand-1) {
		printf( "Could not give out hand. Not enought cards on deck." );
		return;
	}
	for(int i=0; i<NoOfCardsPerHand; ++i)
	{
		addCardToHand(hand, &deck->cards[deck->topOfDeck]);
		--deck->topOfDeck;
	}
}

enum HandCombinations {
	ROYAL_FLUSH = 0,
	STRAIGHT_FLUSH,
	FOUR_OF_A_KIND,
	FULL_HOUSE,
	FLUSH,
	STRAIGHT,
	THREE_OF_A_KIND,
	TWO_PAIR,
	PAIR,
	NO_PAIR,
	NO_HAND_COMBINATIONS
};
typedef enum HandCombinations HandCombinations;

struct PokerStats
{
	int occurenceOfHandCombination[NO_HAND_COMBINATIONS];
	int totalNumberOfHands;
};
typedef struct PokerStats PokerStats;

void initPokerStats(PokerStats* pokerStats)
{
	for(int i=0; i<NO_HAND_COMBINATIONS; ++i)
	{
		pokerStats->occurenceOfHandCombination[i] = 0;
	}
	pokerStats->totalNumberOfHands = 0;
}

void addHandToStats(PokerStats* pokerStats, Hand* hand)
{

}

int main(int argc, char**argv)
{
	srand (time (NULL));
	printf("Hello world!");
	Deck deck;
	initDeck(&deck);
	shuffleDeck(&deck);
	Hand hand;
	initHand(&hand);
	giveOutHand(&deck,&hand);
	return 0;
}

