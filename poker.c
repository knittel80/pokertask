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

// Return value will be > 0 if it is a royal flush
// and 0 if it is not.
int isRoyalFlush(Hand* hand)
{
	int checkValue = hand->availableCards[Ten]; 
	for(int idx=Jack; idx<NumberOfPips; ++idx)
	{
		checkValue &= hand->availableCards[idx];
	}
	return checkValue;
}

int isStraightFlush(Hand* hand)
{
	// Implementation not yet finished
	/*
	int checkValue = 0;hand->availableCards[Ten]; 
	for(int idx=Jack; idx<NumberOfPips; ++idx)
	{
		checkValue &= hand->availableCards[idx];
	}*/
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

