#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FACES 13
#define SUITS 4
#define CARDS 52
#define HANDS 10

struct player_chips
{
    int coinValue1;
    int coinValue5;
    int coinValue10;
};

void shuffle(unsigned int deckShuffle[][FACES]);
void startOfTheGame(unsigned int *potStart, struct player_chips p1, struct player_chips p2, struct player_chips p3, struct player_chips p4, struct player_chips p5);
void drawCardsToPlayer(int *histogram, int *player);
void handValue(int *playerCards, int *playerHandValue, unsigned int deckValuation[][FACES]);

int main()
{
    unsigned int deck[SUITS][FACES] = {0};
    srand(time(NULL));
    shuffle(deck);
    struct player_chips player1Chips, player2Chips, player3Chips, player4Chips, player5Chips;
    unsigned int pot = 0;
    startOfTheGame(&pot, player1Chips, player2Chips, player3Chips, player4Chips, player5Chips);
    const char *suits[SUITS] = {"Hearts", "Diamond", "Clubs", "Spades"};
    const char *faces[FACES] = {"Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};
    const char *hands[HANDS] = {"High Card", "Pair", "Two Pair's", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush", "Royal Flush"};
    int histogramOfDrawCards[CARDS] = {0};
    int player1Cards[5] = {0}, player2Cards[5] = {0}, player3Cards[5] = {0}, player4Cards[5] = {0}, player5Cards[5] = {0};
    drawCardsToPlayer(histogramOfDrawCards, player1Cards);
    drawCardsToPlayer(histogramOfDrawCards, player2Cards);
    drawCardsToPlayer(histogramOfDrawCards, player3Cards);
    drawCardsToPlayer(histogramOfDrawCards, player4Cards);
    drawCardsToPlayer(histogramOfDrawCards, player5Cards);
    int player1HandValue = 0, player2HandValue = 0, player3HandValue = 0, player4HandValue = 0, player5HandValue = 0;
    handValue(player1Cards, &player1HandValue, deck);
    handValue(player2Cards, &player2HandValue, deck);
    handValue(player3Cards, &player3HandValue, deck);
    handValue(player4Cards, &player4HandValue, deck);
    handValue(player5Cards, &player5HandValue, deck);
    return 0;
}

void shuffle(unsigned int deckShuffle[][FACES])
{
    for (size_t i = 1; i <= CARDS; i++)
    {
        size_t row;
        size_t column;
        do
        {
            row = rand() % SUITS;
            column = rand() % FACES;
        } while (deckShuffle[row][column] != 0);
        deckShuffle[row][column] = i;
    }
}

void startOfTheGame(unsigned int *potStart, struct player_chips p1, struct player_chips p2, struct player_chips p3, struct player_chips p4, struct player_chips p5)
{
    // Setup of the Variables.
    p1.coinValue1 = 10;
    p2.coinValue1 = 10;
    p3.coinValue1 = 10;
    p4.coinValue1 = 10;
    p5.coinValue1 = 10;
    p1.coinValue5 = 4;
    p2.coinValue5 = 4;
    p3.coinValue5 = 4;
    p4.coinValue5 = 4;
    p5.coinValue5 = 4;
    p1.coinValue10 = 2;
    p2.coinValue10 = 2;
    p3.coinValue10 = 2;
    p4.coinValue10 = 2;
    p5.coinValue10 = 2;
    // First round entry price.
    p1.coinValue1--;
    p2.coinValue1--;
    p3.coinValue1--;
    p4.coinValue1--;
    p5.coinValue1--;
    *potStart += 5;
}

void drawCardsToPlayer(int *histogram, int *playerCardsToDraw)
{
    size_t card = rand() % CARDS;
    int cardsDrawed = 0;
    while (cardsDrawed <= 4)
    {
        if (histogram[card] == 0)
        {
            playerCardsToDraw[cardsDrawed] = card;
            histogram[card]++;
            cardsDrawed++;
        }
        card = rand() % CARDS;
    }
}

void handValue(int *playerCards, int *playerHandValue, unsigned int deckValuation[][FACES])
{
    // Straight histograms to find the hand rankings.
    int histogramFaces[FACES] = {0};
    int histogramSuits[SUITS] = {0};
    int playerCardsCounter = 0;
    while (playerCardsCounter < 5)
    {
        for (size_t i = 0; i < SUITS; i++)
        {
            for (size_t j = 0; j < FACES; j++)
            {
                if (deckValuation[i][j] == playerCards[playerCardsCounter])
                {
                    histogramFaces[j]++;
                    histogramSuits[i]++;
                    playerCardsCounter++;
                }
            }
        }
    }
    // Find a pair, 2 pair's, three of a kind, four of a kind and full houses.
    // Setting the value to a variable who will become the index of the Hand.
    for (size_t i = 0; i < FACES; i++)
    {
        if (histogramFaces[i] == 2)
        {
            if (*playerHandValue == 1)
            {
                *playerHandValue = 2;
            }
            if (*playerHandValue == 3)
            {
                *playerHandValue = 6;
            }
            *playerHandValue = 1;
        }
        if (histogramFaces[i] == 3)
        {
            if (*playerHandValue == 1)
            {
                *playerHandValue = 6;
            }
            *playerHandValue = 3;
        }
        if (histogramFaces[i] == 4)
        {
            *playerHandValue = 7;
        }
    }
    // Find a straight, flush, straight flush and royal flush.
    // Setting the value to a variable who will become the index of the Hand.
    if (*playerHandValue == 0)
    {
        int sequence = 0;
        for (size_t i = 0; i < FACES - 1; i++)
        {
            if (histogramFaces[i] == 1 && histogramFaces[i + 1] == 1)
            {
                sequence++;
            }
            if (sequence == 4)
            {
                *playerHandValue = 4;
            }
        }
        for (size_t i = 0; i < SUITS; i++)
        {
            if (histogramSuits[i] == 5)
            {
                if (*playerHandValue == 4)
                {
                    *playerHandValue = 8;
                    if (histogramFaces[12] == 1)
                    {
                        *playerHandValue = 9;
                    }
                }
                else
                {
                    *playerHandValue = 5;
                }
            }
        }
    }
}