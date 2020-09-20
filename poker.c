#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Suit {
	Clubs=0, Diamonds, Hearts, Spades, NoSuits
};
typedef enum Suit Suit;

enum Pips {
	Ace=0, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, NumberOfPips
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
	for(int suitCtr = 0; suitCtr < NoSuits; ++suitCtr)
	{
		for(int pipsCtr = 0; pipsCtr < NumberOfPips; ++pipsCtr)
		{
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
	int pips[NumberOfPips];
	int suits[NoSuits];
};
typedef struct Hand Hand;
#define NoOfCardsPerHand 7

void initHand(Hand* hand)
{
	int idx = 0;
	for( ; idx<NumberOfPips; ++idx)
	{
		hand->pips[idx] = 0;
	}
	for(idx=0; idx<NoSuits; ++idx)
	{
		hand->suits[idx] = 0;
	}
}

void resetHand(Hand* hand)
{
	initHand(hand);
}

int isFlush(Hand* hand)
{
	for(int idx=0; idx<NoSuits; ++idx)
	{
		if(hand->suits[idx] >= 5)
		{
			return 1;
		}
	}
	return 0;
}

void addCardToHand(Hand* hand, Card* card)
{
	++hand->pips[card->pips];
	++hand->suits[card->suit];
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

