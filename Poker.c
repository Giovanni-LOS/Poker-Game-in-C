#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FACES 13
#define SUITS 4
#define CARDS 52
#define HANDS 10

struct player_chips
{
    unsigned int coinValue1;
    unsigned int coinValue5;
    unsigned int coinValue10;
};
typedef struct player_chips player_chips;

void shuffle(unsigned int deckShuffle[][FACES]);
void initializePlayerChips(player_chips *player_chips);
void drawCardsToPlayer(int *histogram, int *player);
void handValue(int *playerCards, int *playerHandValue, unsigned int deckValuation[][FACES]);
void showPlayerCards(int playerCards[], unsigned int deckCards[][FACES], const char *cardSuit[], const char *cardFace[]);
void showPlayerHandValue(int playerHandValue, const char *handValues[], unsigned int potValue);
void printDeckAndHandForDebug(unsigned int deckDebug[][FACES], int playerCardsDebug[]);
int sumPlayerChips(player_chips playerChipSum, int *sumOfPlayerChips);
void potInitialBet(player_chips *playerChipInitialBet, unsigned int *potPtr);
void betRoundPlayer(int *playerHoundIn, unsigned int *totalBet, int *personalRoundBet, player_chips *playerChipsRound, unsigned int *potRound);
void printCurrentRoundStatus(unsigned int potStatus, int personalRoundStatus, player_chips *playerChipsStatus);

int main()
{
    int player1Cards[5] = {0}, player2Cards[5] = {0}, player3Cards[5] = {0}, player4Cards[5] = {0}, player5Cards[5] = {0};
    int player1ChipsSum = 0, player2ChipsSum = 0, player3ChipsSum = 0, player4ChipsSum = 0, player5ChipsSum = 0;
    int personalBet = 0;
    int player1HoundIn = 0, player2HoundIn = 0, player3HoundIn = 0, player4HoundIn = 0, player5HoundIn = 0;
    unsigned int deck[SUITS][FACES] = {0};
    int histogramOfDrawCards[CARDS] = {0};
    const char *suits[SUITS] = {"Hearts", "Diamond", "Clubs", "Spades"};
    const char *faces[FACES] = {"Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};
    const char *hands[HANDS] = {"High Card", "Pair", "Two Pair's", "Three of a Kind", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush", "Royal Flush"};
    player_chips player1Chips, player2Chips, player3Chips, player4Chips, player5Chips;
    int player1HandValue = 0, player2HandValue = 0, player3HandValue = 0, player4HandValue = 0, player5HandValue = 0;
    unsigned int pot = 0;
    unsigned int betTotal = 0;

    initializePlayerChips(&player1Chips);
    initializePlayerChips(&player2Chips);
    initializePlayerChips(&player3Chips);
    initializePlayerChips(&player4Chips);
    initializePlayerChips(&player5Chips);

    srand(time(NULL));

    puts("POKER THE GAME by GigiLucas");
    puts("Press enter to continue...");
    getchar();

    while (sumPlayerChips(player1Chips, &player1ChipsSum) && sumPlayerChips(player2Chips, &player2ChipsSum) && sumPlayerChips(player3Chips, &player3ChipsSum) && sumPlayerChips(player4Chips, &player4ChipsSum) && sumPlayerChips(player5Chips, &player5ChipsSum))
    {

        shuffle(deck);

        potInitialBet(&player1Chips, &pot);
        potInitialBet(&player2Chips, &pot);
        potInitialBet(&player3Chips, &pot);
        potInitialBet(&player4Chips, &pot);
        potInitialBet(&player5Chips, &pot);

        drawCardsToPlayer(histogramOfDrawCards, player1Cards);
        drawCardsToPlayer(histogramOfDrawCards, player2Cards);
        drawCardsToPlayer(histogramOfDrawCards, player3Cards);
        drawCardsToPlayer(histogramOfDrawCards, player4Cards);
        drawCardsToPlayer(histogramOfDrawCards, player5Cards);

        handValue(player1Cards, &player1HandValue, deck);
        handValue(player2Cards, &player2HandValue, deck);
        handValue(player3Cards, &player3HandValue, deck);
        handValue(player4Cards, &player4HandValue, deck);
        handValue(player5Cards, &player5HandValue, deck);

        showPlayerCards(player1Cards, deck, suits, faces);

        showPlayerHandValue(player1HandValue, hands, pot);

        printCurrentRoundStatus(pot, personalBet, &player1Chips);

        betRoundPlayer(&player1HoundIn, &betTotal, &personalBet, &player1Chips, &pot);

        printCurrentRoundStatus(pot, personalBet, &player1Chips);

        getchar();
    }

    // printDeckAndHandForDebug(deck, player1Cards);

    return 0;
}

void shuffle(unsigned int deckShuffle[][FACES])
{
    for (size_t i = 0; i < CARDS; i++)
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

void initializePlayerChips(player_chips *player_chips)
{
    // Setup of the Variables.
    // First round entry price
    player_chips->coinValue1 = 10;
    player_chips->coinValue5 = 4;
    player_chips->coinValue10 = 2;
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
                if (playerCardsCounter == 5)
                {
                    break;
                }
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
                continue;
            }
            if (*playerHandValue == 3)
            {
                *playerHandValue = 6;
                continue;
            }
            *playerHandValue = 1;
        }
        if (histogramFaces[i] == 3)
        {
            if (*playerHandValue == 1)
            {
                *playerHandValue = 6;
                continue;
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

void showPlayerCards(int playerCards[], unsigned int deckCards[][FACES], const char *cardSuit[], const char *cardFace[])
{
    puts("###");
    puts("Your hand is:");
    for (size_t card = 0; card < 5; card++)
    {
        for (size_t i = 0; i < SUITS; i++)
        {
            for (size_t j = 0; j < FACES; j++)
            {
                if (deckCards[i][j] == playerCards[card])
                {
                    printf("%lu -> %s of %s <-\n", card + 1, cardFace[j], cardSuit[i]);
                }
            }
        }
    }
    puts("###");
}

void showPlayerHandValue(int playerHandValue, const char *handValues[], unsigned int potValue)
{
    puts("###");
    printf("Your hand have a %s.\n", handValues[playerHandValue]);
    printf("The pot value is $%d.\n", potValue);
    puts("###");
}

void printDeckAndHandForDebug(unsigned int deckDebug[][FACES], int playerCardsDebug[])
{
    for (size_t i = 0; i < SUITS; i++)
    {
        puts("");
        for (size_t j = 0; j < FACES; j++)
        {
            printf("%d ", deckDebug[i][j]);
        }
    }
    puts("");
    for (size_t i = 0; i < 5; i++)
    {
        printf("%d ", playerCardsDebug[i]);
    }
}

int sumPlayerChips(player_chips playerChipSum, int *sumOfPlayerChips)
{
    *sumOfPlayerChips = playerChipSum.coinValue1 + (playerChipSum.coinValue5 * 5) + (playerChipSum.coinValue10 * 10);
    if (*sumOfPlayerChips == 250)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void potInitialBet(player_chips *playerChipInitialBet, unsigned int *potPtr)
{
    if (playerChipInitialBet->coinValue5 == 0)
    {
        if (playerChipInitialBet->coinValue10 > 0)
        {
            playerChipInitialBet->coinValue10--;
            playerChipInitialBet->coinValue5 += 2;
        }
    }
    if (playerChipInitialBet->coinValue1 == 0)
    {
        if (playerChipInitialBet->coinValue5 > 0)
        {
            playerChipInitialBet->coinValue5--;
            playerChipInitialBet->coinValue1 += 5;
        }
    }
    if (playerChipInitialBet->coinValue1 > 0)
    {
        playerChipInitialBet->coinValue1--;
        *potPtr += 1;
    }
    if (playerChipInitialBet->coinValue5 == 0 && playerChipInitialBet->coinValue1 == 0 && playerChipInitialBet->coinValue10 == 0)
    {
        puts("Some player do not have Chips");
        getchar();
    }
}

void betRoundPlayer(int *playerHoundIn, unsigned int *totalBet, int *personalRoundBet, player_chips *playerChipsRound, unsigned int *potRound)
{
    if (*playerHoundIn == 0)
    {
        int playerRoute = 0;
        puts("Press 1 for call/check, 2 for raise/bet, and 3 for fold: (then press enter)");
        scanf("%d", &playerRoute);
        while (playerRoute > 3 || playerRoute < 0)
        {
            puts("Incorrect number, select the right number for action:");
            scanf("%d", &playerRoute);
        }
        if (playerRoute == 1)
        {
            if (*personalRoundBet == *totalBet)
            {
                return;
            }
            int sumChips = playerChipsRound->coinValue1 + (playerChipsRound->coinValue5 * 5) + (playerChipsRound->coinValue10 * 10);
            if ((*totalBet - *personalRoundBet) >= sumChips)
            {
                playerChipsRound->coinValue1 = 0;
                playerChipsRound->coinValue5 = 0;
                playerChipsRound->coinValue10 = 0;
                *potRound += sumChips;
                return;
            }
            int difT = *totalBet - *personalRoundBet;
            int dif1 = 0;
            int dif5 = 0;
            int dif10 = 0;
            if (difT >= 10)
            {
                dif1 = difT % 10;
                dif10 = (difT - dif1) / 10;
            }
            else
            {
                dif1 = difT;
            }
            if (dif1 > 5)
            {
                dif1 -= 5;
                dif5 += 1;
            }
            if (playerChipsRound->coinValue1 >= dif1)
            {
                playerChipsRound->coinValue1 -= dif1;
                *potRound += dif1;
            }
            else
            {
                playerChipsRound->coinValue1 = 0;
            }
            if (playerChipsRound->coinValue5 >= dif5)
            {
                playerChipsRound->coinValue5 -= dif5;
                *potRound += dif5 * 5;
            }
            if (playerChipsRound->coinValue10 >= dif10)
            {
                playerChipsRound->coinValue10 -= dif10;
                *potRound += dif10 * 10;
            }
            return;
        }
        if (playerRoute == 2)
        {
            int sumChips = playerChipsRound->coinValue1 + (playerChipsRound->coinValue5 * 5) + (playerChipsRound->coinValue10 * 10);
            if ((*totalBet - *personalRoundBet) >= sumChips)
            {
                puts("You do not have enough chips to bet/raise. This is an all in.");
                playerChipsRound->coinValue1 = 0;
                playerChipsRound->coinValue5 = 0;
                playerChipsRound->coinValue10 = 0;
                *potRound += sumChips;
                *personalRoundBet += sumChips;
                return;
            }
            puts("You are bet/raise how much?");
            int bet;
            scanf("%d", &bet);
            while (bet + (*totalBet - *personalRoundBet) > sumChips)
            {
                puts("You can't bet/raise that amount, try again:");
                scanf("%d", &bet);
            }
            int difT = (*totalBet - *personalRoundBet) + bet;
            int dif1 = 0;
            int dif5 = 0;
            int dif10 = 0;
            if (difT >= 10)
            {
                dif1 = difT % 10;
                dif10 = (difT - dif1) / 10;
            }
            else
            {
                dif1 = difT;
            }
            if (dif1 > 5)
            {
                dif1 -= 5;
                dif5 += 1;
            }
            if (playerChipsRound->coinValue1 >= dif1)
            {
                playerChipsRound->coinValue1 -= dif1;
                *potRound += dif1;
            }
            else
            {
                playerChipsRound->coinValue1 = 0;
            }
            if (playerChipsRound->coinValue5 >= dif5)
            {
                playerChipsRound->coinValue5 -= dif5;
                *potRound += dif5 * 5;
            }
            if (playerChipsRound->coinValue10 < dif10)
            {
                while (playerChipsRound->coinValue10 < dif10)
                {
                    dif10--;
                    dif5 += 2;
                }
                while (playerChipsRound->coinValue5 < dif5)
                {
                    dif5--;
                    dif1 += 5;
                }
                while (playerChipsRound->coinValue1 < dif1)
                {
                    dif1--;
                }
                if (playerChipsRound->coinValue1 >= dif1)
                {
                    playerChipsRound->coinValue1 -= dif1;
                    *potRound += dif1;
                }
                if (playerChipsRound->coinValue5 >= dif5)
                {
                    playerChipsRound->coinValue5 -= dif5;
                    *potRound += dif5 * 5;
                }
            }
            if (playerChipsRound->coinValue10 >= dif10)
            {
                playerChipsRound->coinValue10 -= dif10;
                *potRound += dif10 * 10;
            }
            *personalRoundBet += (*totalBet - *personalRoundBet) + bet;
            *totalBet += bet;
            return;
        }
        if (playerRoute == 3)
        {
            puts("You fold.");
            *playerHoundIn = 1;
            return;
        }
    }
}

void printCurrentRoundStatus(unsigned int potStatus, int personalRoundStatus, player_chips *playerChipsStatus)
{
    int sumChips = playerChipsStatus->coinValue1 + (playerChipsStatus->coinValue5 * 5) + (playerChipsStatus->coinValue10 * 10);
    printf("The pot is $%d.\nThe current money put in is $%d.\nYou have left in balance $%d\n", potStatus, personalRoundStatus + 1, sumChips);
}