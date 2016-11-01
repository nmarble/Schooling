/* 
 * File:   blackJack.c
 * Author: Nicholas Marble
 *
 * Created on February 4, 2015, 12:22 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Structure for a card
typedef struct card {
    char * face;
    char * suit;
    int value;
    struct card * next;
}Card;

typedef struct gameGroup {
    struct card * playerHand;
    struct card * dealerHand;
    struct card * deck;
}GameGroup;

//Function to deal with invalid functions
int garbage() {
    char junk;
    int counter = 0;
    //Loops through all of input
    while ((junk = getchar()) != EOF && junk != '\n') {
        counter++;
    }
    //Message for wrong input
    if(counter) {
        printf("Wrong input\n");
    }
    return counter;
}

//Function to create all the groups in memory
GameGroup * init(GameGroup * groups){
    Card * deck = (Card *)malloc(1*sizeof(Card));
    Card * playerHand = (Card *)malloc(1*sizeof(Card));
    Card * dealerHand = (Card *)malloc(1*sizeof(Card));    
    deck = NULL; 
    playerHand = NULL;
    dealerHand = NULL;
    groups->deck = deck;
    groups->playerHand = playerHand;
    groups->dealerHand = dealerHand;
    
    return groups;
}

//Function to add card to a group of cards
Card * createCard(Card * root, char * face, char * suit, int value) {
    Card * newCard = (Card *)malloc(1*sizeof(Card));
    newCard->face = face;
    newCard->suit = suit;
    newCard->value = value;
    newCard->next = NULL;
    
    if (root == NULL) {
        root = newCard;
        return root;
    }else {
        newCard->next = root;
        root=newCard;
    }
    
}

//Function that makes a 52 card deck
GameGroup * makeDeck(GameGroup * group) {
    char *faces[] = {"2","3","4","5","6","7","8","9","10","J","Q","K","A"};
    int values[] = {2,3,4,5,6,7,8,9,10,10,10,10,11};
    char *suits[] = {"Hearts", "Diamonds", "Spades", "Clubs"};
    
    for (int index = 0; index < 52; index++) {
        group->deck = createCard(group->deck, faces[index % 13], suits[index % 4], values[index % 13]);
        
    }
    return group;
}

//Function to get the total value of a group
int getValue(Card * root) {
    int totalValue = 0;
    //Returns 0 if the group is empty
    if (root == NULL) {
        return totalValue;
    }
    Card * current = (Card *)malloc(1*sizeof(Card));
    //Loops through the entire group
    for(current = root; current; current = current->next) {
        totalValue = totalValue + current->value;
    } 
    return totalValue;
    
}

//Function to count the amount of cards in a group
int count(Card * root) {
    Card * current = (Card *)malloc(1*sizeof(Card));
    int count = 0;
    //Loops through the entire group
    for(current = root; current; current = current->next) {
        count++; //adds to the count
    }
    return count;
}

//Function that removes a card from a group
Card * removeCard(Card * root, char * face, char * suit) {
    Card * current = (Card *)malloc(1*sizeof(Card));
    Card * prev = (Card *)malloc(1*sizeof(Card));
    //Loops through the entire group
    for(current = root; current; current = current->next) {
        //Matches the current card with the one searching for
        if (strcmp(current->face, face) == 0 && strcmp(current->suit, suit) == 0) {
            //Deals with the card according to its location
            if (current==root) {
                root = current->next;
                return root;
            }else {
                prev->next=current->next;
                return root;
            }
        } else {
            prev=current;
        }
    }
    
    return root;
}

//Function that removes the top card in a group 
Card * removeTop(Card * root) {
    root = root->next;
    return root;
}

//Function that checks for aces and replaces the value with 1
Card * checkForAce(Card * hand){
    Card * current = (Card *)malloc(1*sizeof(Card));
    int found = 0;
    //Loops through the entire group
    for(current = hand; current; current = current->next) {
        //Checks to see if there is an ace with a value of 11
        if ((strcmp(current->face, "A") == 0) && current->value == 11 && found == 0) {
            current->value = 1;
            found = 1;
        }
    }
    return hand;
}

//Function to move a card from the deck to a hand
GameGroup * drawCard(GameGroup * groups, char * dest) {
    //What happens when the destination is the players hand
    if (strcmp(dest, "player") == 0){
        groups->playerHand = createCard(groups->playerHand, groups->deck->face, groups->deck->suit, groups->deck->value);
        groups->deck = removeTop(groups->deck);
        //Checks if the value of the hand is over 21
        if (getValue(groups->playerHand) > 21) {
            groups->playerHand = checkForAce(groups->playerHand);
        }
    }
    else if (strcmp(dest, "dealer") == 0){
        groups->dealerHand = createCard(groups->dealerHand, groups->deck->face, groups->deck->suit, groups->deck->value);
        groups->deck = removeTop(groups->deck);
        //Checks if the value of the hand is over 21
        if (getValue(groups->dealerHand) > 21) {
            groups->dealerHand = checkForAce(groups->dealerHand);
        }
    } else {
        printf("Invalid destination \n");
    }return groups;
}

//Function to shuffle a group one at a time
GameGroup * shuffle(GameGroup * groups, int numOfCards) {
    Card * current = (Card *)malloc(1*sizeof(Card));
    //Loops the amount of times given
    while (numOfCards > 0) {
        int randCard =  rand() % (count(groups->deck)); 
        //Goes to the end of the group
        for(current = groups->deck; randCard > 0; current = current->next, randCard--);
        groups->deck = removeCard(groups->deck, current->face, current->suit);
        groups->deck = createCard(groups->deck, current->face, current->suit, current->value);
        numOfCards--;
    }
    return groups;
}

//Function to return all cards in hand to the deck
GameGroup * returnAll(GameGroup * groups) {
    Card * current = (Card *)malloc(1*sizeof(Card));
    //Loops through the entire player hand
    for(current = groups->playerHand; current; current = current->next) {
        //Changes the values of the aces back to 11
        if (current->value == 1) {
            current->value = 11;
        }
        groups->deck = createCard(groups->deck, current->face, current->suit, current->value);
        groups->playerHand = removeCard(groups->playerHand, current->face, current->suit);
    }
    //Loops through the entire dealer hand
    for(current = groups->dealerHand; current; current = current->next) {
        //Changes the values of the aces back to 11
        if (current->value == 1) {
            current->value = 11;
        }
        groups->deck = createCard(groups->deck, current->face, current->suit, current->value);
        groups->dealerHand = removeCard(groups->dealerHand, current->face, current->suit);
    }
    return groups;
}

//Function for a new bet
int newBet (int * credits) {
    int amount = 0;
     
    //Loops until valid input is given
    while (1) {
        
        //Get the amount of credits the user wishes to bet
 
        printf("You have %d credits\n", *credits);
        printf("How much would you like to bet?\n");
        scanf("%d", &amount);
            //Deals with invalid inputs
            if(garbage()) {
                continue;
            }
        //Deals with amounts entered
        
        if (amount > *credits) {
            printf("You don't have that much\n");
            continue;
        }else if (amount <= 0) {
            printf("You cant do that\n");         
            continue;
        }
        
    break;
    }
    *credits -= amount; //Remove the amount bet
    return amount;
}

//Function the prints out a group of cards
void printGroup(Card * group) {
    Card * current = (Card *)malloc(1*sizeof(Card));
    //Loops through the entire group
    for(current = group; current; current = current->next) {
        printf("|%s of %s|", current->face, current->suit);      
    }
    printf("\n");
}

//Function that prints only a certain amount of cards 
void printAmount(Card * group, int number) {
    Card * current = (Card *)malloc(1*sizeof(Card));
    //Loops through the entire group
    for(current = group; current; current = current->next) {
        //Prints the amount of cards given
        if (number > 0) {
            printf("|%s of %s|", current->face, current->suit);      
        }else {
            printf("|---------|");
        }
        number--;
    }
    printf("\n");
}

//Function to show ever card and total value
void displayTable (GameGroup * groups, int status) {
    printf("---------------------------------------\n");
    printf("Dealers cards: ");
    //What happens if it is the dealers turn
    if (status == 1) {
        printAmount(groups->dealerHand, 1);
        
    }else {
        printGroup(groups->dealerHand);
        printf("Dealer Total is %d\n", getValue(groups->dealerHand));
    }
    printf("Your cards: ");
    printGroup(groups->playerHand);
    printf("Your total is %d\n", getValue(groups->playerHand));
    printf("---------------------------------------\n");
}

//The main function for the game
int main(int argc, char** argv) {
    setbuf(stdout, NULL);   
    srand(time(NULL));    //Randomize with time10
    GameGroup * groups = (GameGroup *)malloc(1*sizeof(GameGroup));
    int credits = 100;
    groups = init(groups);        //Creates all the groups
    groups = makeDeck(groups);    //Creates a deck of 52 cards

    char choice;          //Holds user input for hitting
    char next;            //Used to allow breaking in between hands

    int lose;
    //Main loop for entire game
    while (1) {
        int bet = newBet(&credits);  //Get a new bet from user
        lose = 0;                    //Sets the default to win status
        groups = shuffle(groups, 1000);
        //Deals initial cards
        groups = drawCard(groups, "player");
        groups = drawCard(groups, "player");
        groups = drawCard(groups, "dealer");
        groups = drawCard(groups, "dealer");
        
        //Loop for players turn
        while(1) {
            //Shows all dealt card
            displayTable(groups, 1);
            printf("q to quit\n");
            printf("Hit (y/n):  ");
            scanf("%c", &choice);   //Get input from user
            //Deal with incorrect input
            if(garbage()) {
                continue;
            }
            //What is done if user wants to hit
            if(choice == 'y') {
                //Draws another card
                groups = drawCard(groups, "player");
                //Deal with what happens when value is over 21
                if (getValue(groups->playerHand) > 21) {
                    printf("You busted!\n");
                    lose = 1; //Set to lose status
                    break;
                }
            }else if(choice == 'n') {
                break; //Gets out of Users turn loop
            }else if(choice == 'q') {
                exit(0); //Quits if user wants to
            }
        }
        
        //Dealers Turn
        while (1 && lose == 0) {
            //Shows all of cards
            displayTable(groups, 0);
            //Dealer will hit if value is below 17
            if (getValue(groups->dealerHand) < 17) {
                printf("Dealer hits\n");
                //Draws a card for dealer
                groups = drawCard(groups, "dealer");
                //Deals with dealer going over 21
                if (getValue(groups->dealerHand) > 21) {
                    printf("Dealer busted!\n");
                    break;
                }
            }else {
                printf("Dealer stays\n");
                //Checks who has a higher value, tie goes to dealer
                if (getValue(groups->playerHand) > getValue(groups->dealerHand)) {
                    break; //Breaks from dealers turn
                }else {
                    lose = 1; //Set to lose status
                    break; //Breaks from dealers turn
                }
            }
        }
        
        //Show all cards
        displayTable(groups, 0);
        //Winning/losing clause
        if (lose == 0) {
            printf("YOU WIN!\n");
            credits += (bet*2);
        }else {
            printf("YOU LOSE!\n");
        }
        
        //Checks if player runs out of money
        if (credits <= 0) {
            printf("No more credits!\n");   
            printf("Enter to continue\n");
            scanf("%c", &next);            
            exit(0); //Quits game
        }
        
        //Stop to be able to view results of hand
        printf("Enter to continue\n");
        scanf("%c", &next);            
        
        //Clears all hands
        groups = returnAll(groups);
    } 
    return (0);
}