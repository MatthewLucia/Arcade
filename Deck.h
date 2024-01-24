//
// Created by Matt Lucia on 4/13/23.
//

#ifndef INC_8_BIT_DECK_H
#define INC_8_BIT_DECK_H

#include <vector>
#include "Card.h"

// Class representing an Uno deck
class Deck {
private:

    // Single field, vector representing deck
    vector<Card> deck;
public:

    // Constructor initializes deck with all necessary card objects
    Deck();

    // Shuffles deck
    void shuffleDeck();

    // Draws a card from the deck
    Card draw();

    // Returns the amount of cards left in the deck
    int cardsLeft();

    // Sets a new deck
    void setNewDeck(vector<Card> d);
};

#endif //INC_8_BIT_DECK_H
