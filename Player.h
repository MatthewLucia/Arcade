//
// Created by Matt Lucia on 4/13/23.
//

#ifndef INC_8_BIT_PLAYER_H
#define INC_8_BIT_PLAYER_H

#include "Card.h"
#include "Deck.h"
#include "Discard.h"

// Class representing a player in a game of Uno
class Player {
private:

    // Fields including a vector of cards representing a player's hand, and a string representing the player's name
    vector<Card> hand;
    string name;
public:

    // Constructor initializing a player object along with their hand
    Player(string n, Deck &deck);

    // Method performing one user turn
    Card play(Deck &deck, Discard &pile, Player* next, int choice);

    // Returns name of player
    string getName();

    // Returns player hand
    vector<Card> getHand();

    // Adds a card to a player's hand from the deck
    void addCard(Deck &deck);

    // Method performing one turn for the CPU
    Card compMove(Deck &deck, Discard &pile, Player* next);
};

#endif //INC_8_BIT_PLAYER_H
