#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FACES 13
#define SUITS 4
#define CARDS 52

void shuffle(unsigned int deckShuffle[][FACES]);
void drawCardsToPlayer(int *histogram, int *player);

int main()
{
    unsigned int deck[SUITS][FACES] = {0};
    srand(time(NULL));
    shuffle(deck);
    const char *suits[SUITS] = {"Hearts", "Diamond", "Clubs", "Spades"};
    const char *faces[FACES] = {"Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
    int histogramOfDrawCards[CARDS] = {0};
    int player1[5] = {0}, player2[5] = {0}, player3[5] = {0}, player4[5] = {0}, player5[5] = {0};
    drawCardsToPlayer(histogramOfDrawCards, player1);
    drawCardsToPlayer(histogramOfDrawCards, player2);
    drawCardsToPlayer(histogramOfDrawCards, player3);
    drawCardsToPlayer(histogramOfDrawCards, player4);
    drawCardsToPlayer(histogramOfDrawCards, player5);

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

void drawCardsToPlayer(int *histogram, int *player)
{
    size_t card = rand() % CARDS;
    int cardsDrawed = 0;
    while (cardsDrawed <= 4)
    {
        if (histogram[card] == 0)
        {
            player[cardsDrawed] = card;
            histogram[card]++;
            cardsDrawed++;
        }
        card = rand() % CARDS;
    }
}