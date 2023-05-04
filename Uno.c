#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define CLUB 'c'
#define SPADES 's'
#define HEARTS 'h'
#define DIAMONDS 'd'

#define GRAPHIC_CLUB "♣"
#define GRAPHIC_SPADES "♠"
#define GRAPHIC_HEARTS "♥"
#define GRAPHIC_DIAMONDS "♦"

#define QUEENOFHEARTS 11

typedef struct card_s // struct for dynamic list of cards
{
    char suit;
    int face;
    struct card_s* next;
    
} Card;

typedef struct Hand_Holder_s // struct to keep track of who is holding a hand and what is in that hand 
{
    char* name;
    Card* hand;
    struct Hand_Holder_s* next;
    
} HandHolder;

int countCardsInHand(Card* headCard) // Input: first card of hand , Output: count of number of cards in hand (works for computer or player hand depending on call), Goal: tally number of cards in hand 
{
    int count = 0;
    
    for (Card* temp = headCard; temp != NULL; temp = temp -> next)
    {
        count++;
    }
    
    return count;
}

int randomInt(int b) // Input: integer, Output: returns random integer , Goal: to create a random integer, this function is called by the next function to pick random card 
{
    return (b == 0) ? 0 : ( rand() % b );
}

Card* randomPick(Card* headCard) // Input: first card of hand , Output: a random card from the hand , Goal: make headCard pointer point to a random card, this function is used later to shuffle card once
{
    int chosenPos = randomInt(countCardsInHand(headCard));
    
    for (int i = 0; i < chosenPos; i++) // moves head pointer to different cards for a random amount of times
    {
        headCard = headCard -> next;
    }
    
    return headCard;
}

Card* pickAtPos(int cardPos, HandHolder* players) // Input: chosen card position and either computer or player's hand, Output: chosen card at chosen position, Goal: allows player to pick card at chosen position from other player's hand 
{
    Card *prev, *cur;
    
    if (cardPos == 0) 
    {
        cur = players -> hand;
        players -> hand = cur -> next;
        cur -> next = NULL;
    } 
    
    else
    {
        prev = players -> hand;
        cur = players -> hand -> next;
        
        for (int i = 1; i < cardPos; i++)
        {
            prev = cur;
            cur = cur -> next;
        }
        
        prev -> next = cur -> next;
        cur -> next = NULL;
    }
    
    return cur;
}

void addCardtoHand(HandHolder* player, Card* c) // Input: a player and a chosen card , Output: none , Goal: puts chosen card in other player's hand (player's linked list)
{
    c -> next = player -> hand; 
    player -> hand = c;         // points hand to chosen card
}

void printOneCard(Card* temp) //Input: one card based on the input of which card from previous functions, output: card (without the card art) Goal: To display the cards in the hand of the computer or player
{
    const char* printedsuit = ((temp -> suit == HEARTS) ? GRAPHIC_HEARTS : ((temp -> suit == CLUB) ? GRAPHIC_CLUB : ((temp -> suit == SPADES) ? GRAPHIC_SPADES : ((temp -> suit == DIAMONDS) ? GRAPHIC_DIAMONDS : "E"))));
    
    if (temp -> face == 11)
    {
        printf("Q%s", printedsuit);  // changes an 11 to a Queen 
    }
    
    else if (temp -> face == 1)
    {
        printf("A%s", printedsuit); // changes a 1 to an Ace
    } 
    
    else
    {
        printf("%d%s", temp -> face, printedsuit); // prints card regularly if not an 11 or 1
    }
}

void CardArt(Card* temp) //Input: card number and suit, output: card number and suit with card art (looks a little weird for 10's and when the suits missprint as ?? in gdb compiler), Goal: to print the number and suit of a card with card art when called in to other functions
{
    Card* ctemp;
    
    while (temp != NULL)
    {
        printf("\n");
        
        ctemp = temp;
        
        for (int i = 0; i < 5 && ctemp != NULL; i++) // creates top of card art
        {
            printf("---------   ");
            ctemp = ctemp -> next;
        } printf("\n");

        ctemp = temp;
        
        for (int i = 0; i < 5 && ctemp != NULL; i++) //creates side of card art
        {
            printf("|"); 
            printOneCard(ctemp);
            printf("     |   ");
            ctemp = ctemp -> next;
        } printf("\n");

        ctemp = temp;
        
        for (int i = 0; i < 5 && ctemp != NULL; i++) //creates side of card art
        {
            printf("|       |   ");
            ctemp = ctemp -> next;
        } printf("\n");

        ctemp = temp;
        for (int i = 0; i < 5 && ctemp != NULL; i++) //creates side of card art
        {
            printf("|     ");
            printOneCard(ctemp);
            printf("|   ");
            ctemp = ctemp -> next;
        } printf("\n");
        
        ctemp = temp;
        for (int i = 0; i < 5 && ctemp != NULL; i++) // creates bottom of card art
        {
            printf("---------   ");
            ctemp = ctemp -> next;
        } printf("\n");

        temp = ctemp;
    }
    printf("\n");
}

void shuffleCardOnce(Card* headCard) //Input: 1 singular card from a linked list (total cards, player cards or computer cards), Output: None:  Goal: swap 2 cards within the linked list its called from
{
    char suit; int face; Card* chosenCard = NULL;
    
    for (Card* temp = headCard; temp != NULL; temp = temp -> next)  //choses a random card and switches face and suit to "shuffle"
    {
        chosenCard = randomPick(headCard);
        
        face = temp -> face;
        suit = temp -> suit;
        temp -> face = chosenCard -> face;
        temp -> suit = chosenCard -> suit;
        chosenCard -> face = face;
        chosenCard -> suit = suit;
    }
}

Card* newCardWith(char suit, int face) // Input: a player and a chosen card , Output: none , Goal: puts chosen card in other player's hand (player's linked list)
{
    Card* temp = (Card*) malloc(sizeof(Card)); // creates a new card a certain suit and face 
    temp -> face = face;
    temp -> suit = suit;
    temp -> next = NULL;
    
    return temp;
}

Card* findAnother(Card* target) // Input: target pair of cards, Output: none, Goal: finds another pair within the hand (used later in remove pairs function)
{
    Card* others = target -> next;
    Card* prevs = target;
    
    while (others != NULL) 
    {
        if (others -> face == target -> face) // if target matches 
        {
            prevs -> next = others -> next;
            
            return others;
        }
        
        prevs = others;
        others = others -> next;
    }
    
    return NULL;
}

void printAndFreeDeletedPairs(Card* pairOne, Card* pairTwo) // Input: two pairs of cards, Output: None, Goal: prints and frees memory deleted pairs from hands (used later in remove pairs function)
{
    printf("Remove pair: ");
    
    CardArt( pairOne );
    free( pairTwo ); // frees memory of pair
    free( pairOne );
}

void removeAllPairs(HandHolder* players) //Input: player that is removing pairs, Output: none, Goal: actually does the removing of the pairs from inputted players hand (works for computer and human), is used later in displaypairthrower function
{
    Card *duplicateCard, *del;
    
    if (players -> hand == NULL)
    { 
        return;
    }
    
    else if ((del = findAnother(players -> hand)) != NULL)
    {
        duplicateCard = players -> hand;
        players -> hand = players -> hand -> next;
        duplicateCard->next = del;
        del -> next = NULL;
        
        printAndFreeDeletedPairs(duplicateCard, del);
    }

    if (players -> hand == NULL)
    { 
        return;
    }
    
    else {
        Card* prev = players -> hand;
        Card* temp = players -> hand -> next;
        
        while (temp != NULL)
        {
            if ((del = findAnother(temp)) != NULL) 
            {
                duplicateCard = temp;
                prev -> next = temp -> next;
                temp = temp -> next;
                duplicateCard -> next = del;
                del -> next = NULL;
                
                printAndFreeDeletedPairs(duplicateCard, del);
            } 
            
            else 
            {
                prev = temp;
                temp = temp -> next;
            }
        }
    }
}


Card* createCards() // Input: none, Output: first card of deck, Goal: creates deck of card for use in game, used later in dealCards function
{
    Card *head = NULL, *temp = NULL;
    
    for (int i = 1; i < 11; i++) 
    {
        if (head == NULL)
        { 
            temp = newCardWith(CLUB, i);
            head = temp;
        }
        
        else
        {
            temp -> next = newCardWith(CLUB, i); // creates Clubs 
            temp = temp -> next;
        }

        temp -> next = newCardWith(SPADES, i); // creates Spades
        temp = temp -> next;

        temp -> next = newCardWith(HEARTS, i); // creates Hearts
        temp = temp -> next;

        temp -> next = newCardWith(DIAMONDS, i); // creates Diamonds
        temp = temp -> next;

    }
    
    temp -> next = newCardWith(HEARTS, QUEENOFHEARTS);
    
    return head;
}

HandHolder* createPlayers() // Input: none, Output: human player of game, Goal: allows human player to enter their name and start the game 
{
    HandHolder* players = (HandHolder*) malloc(sizeof(HandHolder));
    
    players->name = (char*) malloc( sizeof(char) * 250 );
    printf("Enter your name: ");
    scanf("%s", players -> name);
    printf("\n***---*** ***---*** ***---***\n");
    printf("%s, let's play Losing Queen\n", players -> name);
    printf("***---*** ***---*** ***---***\n\n");
    players -> hand = NULL;

    players -> next = (HandHolder*) malloc(sizeof(HandHolder));
    players -> next -> name = (char*) malloc(sizeof(char) * 250);
    bzero(players -> next -> name, sizeof(char) * 250);
    strcpy(players -> next -> name, "Computer" );
    players -> next -> hand = NULL;
    
    players -> next -> next = players;
    return players;
}

void ShuffleCard(Card* headCard) //Input: first card of either deck or hand, Output: shuffled deck or hand, Goal: this function refers to the shuffleCardOnce function and shuffles one card (replaces it with another) 1000 times to ensure full shuffling 
{ 
    for (int i = 0; i < 1000; i++) //repeats 1000 times as stated in instructions
    { 
        shuffleCardOnce(headCard);
    } 
}

void dealCards(HandHolder* holder) //Input: deck being played , Output: none (dealt cards), Goal: this function deals the cards after shuffling them
{
    Card *allCards = createCards(), *OneCard;
    
    ShuffleCard(allCards); //shuffles cards before dealing
    
    while(allCards != NULL) // deals cards into hands
    {
        OneCard = allCards;
        allCards = allCards -> next;

        OneCard -> next = holder -> hand;
        holder -> hand = OneCard;
        holder = holder -> next;
    }
}

void displaypairthrower(HandHolder* players)  // Input: player (human or computer) , Output: tells who throws out pairs, Goal: this function uses the removeAllPairs function and displays who is throwing away pairs while simultaneously getting rid of them from the linked list
{
    printf("%s's hand: throw away pairs\n", players->name ); //displays who is throwing pairs
    removeAllPairs(players);
    removeAllPairs(players);
}

void displayPlayersHand(HandHolder* players) // Input: player (human or computer), Output: inputted player's hand with card art, Goal: this function displays the hand of chosen player with card art
{
    printf( "\n" );
    CardArt( players -> hand ); // displays hand with card art
    printf("\n\n");
}

int readUsrCardChoice(int range) // Input: how many cards in computers hand, Output: subtraction of 1 from count of number of cards in hand, Goal: this function allows the player to choose a card from computer's hand 
{
    int i;
    
    while(1)
    {
        printf("Which one do you want to choose (1-%d)?: ", range);
        scanf("%d", &i); //allows user to choose what card to take 
        
        if (i < 1 || i > range)
        {
            printf("Computer: I have ONLY %d cards\n", range); // does not allow player to choose card outside of range
            continue;
        }
        
        else 
        {
            break;
        }
    }
    
    return i - 1;
}

void computerTurn(HandHolder* computerPlayer, HandHolder* humanPlayer) //Input: computer and human players, Output: displays human player hand, Goal: allows computer to play the game and take its turn in picking cards from human
{
    int myChoose = rand() % countCardsInHand(humanPlayer -> hand);
    
    printf("Computer: I choose card #%d from you\n", myChoose + 1);
    addCardtoHand(computerPlayer, pickAtPos(myChoose, humanPlayer)); // allows computer to add to its hand 
    displaypairthrower(computerPlayer); // displays how many pairs computer has thrown 
    shuffleCardOnce(computerPlayer -> hand); // shuffles computer hand 
    
    printf("Computer: I now have %d cards\n", countCardsInHand(computerPlayer -> hand));
    printf( "\n%s, this is your hand:", humanPlayer->name );
    
    displayPlayersHand(humanPlayer); // display human hand
}

void humanTurn(HandHolder* humanPlayer, HandHolder* computerPlayer) //Input: computer and human players, Output: displays added cards, thrown pairs, and then displays shuffled player hand, Goal: displays to user needed info to play game and shuffles user hand
{
    int cc = countCardsInHand(computerPlayer -> hand);
    
    printf("%s: I now have %d cards\n", computerPlayer -> name, cc); // lets user know how many cards that computer has
    
    for (int i = 0; i < cc; i++)
    {
        printf("%d ", i + 1 );
    }
    
    printf("\n");

    cc = readUsrCardChoice(cc);

    addCardtoHand(humanPlayer, pickAtPos(cc, computerPlayer)); // allows human to add to his/her hand 
    displaypairthrower(humanPlayer); // displays how many pairs human has thrown 
    shuffleCardOnce(humanPlayer -> hand); // shuffles human hand

    if (humanPlayer -> hand != NULL)
    {
        printf("\n%s, this is your (Shuffled) hand:", humanPlayer -> name);
        displayPlayersHand(humanPlayer);
    }
}

int continueGame(HandHolder* player) //Input: player, Output: none, Goal: this funciton continues the game and switches who's turn it is (continueGame is used later in playGame)
{
    HandHolder* temp = player;
    int test = 0, c = 0;
    
    do 
    {
        if (countCardsInHand(player -> hand) > 0) // continues game until a player has no cards in hand 
        { 
            test++; 
        }
        
        c++;
        
        temp = temp -> next;
        
    } 
    while(temp != player);
    
    return (test == c);
}

void playGame()//Input: none, Output: displays current status of game including nunber of cards in each players hands, whos turn it is, and who lost the game, and shows human his/her cards using card art, Goal: this function decides who goes first based on number of cards in their hand as well as runs the game and decides who loses and frees memory
{
    HandHolder* players = createPlayers();

    HandHolder* humanPlayer = players;
    HandHolder* computerPlayer = players -> next;

    dealCards(players);
    displaypairthrower(computerPlayer);

    printf("\n");
    printf("%s: I now have %d cards\n", computerPlayer -> name, countCardsInHand(computerPlayer -> hand));
    printf("\n");

    printf("%s's hand has %d cards", humanPlayer -> name, countCardsInHand(humanPlayer -> hand));
    
    displayPlayersHand(humanPlayer);
    displaypairthrower(humanPlayer);
    
    printf("\n%s, this is your hand:", humanPlayer -> name);
    displayPlayersHand(humanPlayer);

    if(countCardsInHand(humanPlayer -> hand) > countCardsInHand(computerPlayer -> hand))
    {
        players = humanPlayer;
        printf("%s, you have more cards, you start\n", humanPlayer -> name);
    } 
    
    else 
    {
        players = computerPlayer;
        printf("%s, you have fewer cards, I start\n", humanPlayer -> name);
    }

    while(continueGame(players)) 
    {
        if (players -> hand == NULL || players -> next -> hand == NULL)
        { 
            break;
        }
        
        if (players == computerPlayer )
        {
            printf("\n-----%s playing---------\n", computerPlayer -> name );
            computerTurn(computerPlayer, computerPlayer -> next );
        } 
        
        else {
            humanTurn(humanPlayer, humanPlayer -> next);
        }

        players = players -> next;
    }

    if(countCardsInHand(humanPlayer -> hand ) > countCardsInHand(computerPlayer -> hand)) 
    {
        printf("\nSorry, you are a Losing Queen\n");
        free( humanPlayer -> name );
        free( humanPlayer -> hand );
        free( computerPlayer -> name );
    } 
    
    else 
    {
        printf("\nCongratulations, you won. I am a Losing Queen\n");
        free( computerPlayer -> name );
        free( computerPlayer -> hand );
        free( humanPlayer -> name );
    }

    free(computerPlayer);
    free(humanPlayer);
}

int wantToPlayAgain() // Input: none, Output: new game, Goal: this function runs at the end of the game after a loser is decided and prompts user to play again if they enter a y or Y
{
    char check;
    
    while(1) 
    {
        printf("\nDo you want to continue? (q or Q to quit): ");
        scanf(" %c", &check);
        
        if (check == 'q' || check == 'Q')  // if user picks other than q or Q the game will continue (works for numbers and letters)
        {
            return 0;
        }
        
        else
        {
            printf("\n");
            return 1;
        }
    }
    
    return 0;
}

int main()
{
    srand((unsigned int) time(0));
    do playGame();
    while(wantToPlayAgain());
    printf("\nGood Bye\n");
    
    return 0;
}


