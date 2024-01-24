//
// Created by Matt Lucia on 4/13/23.
//

#ifndef INC_8_BIT_DISCARD_H
#define INC_8_BIT_DISCARD_H

#include "Deck.h"
#include "Card.h"
#include <vector>

// Class representing the discard pile in Uno
class Discard {
private:

    // Single field, vector representing discard pile
    vector<Card> pile;
public:

    // Constructor initializes discard pile with starting basic card
    explicit Discard(Deck &deck);

    // Returns the top card in the discard pile
    Card getTopCard();

    // Discards a card, placing it on top of the discard pile
    void discardCard(Card card);

    // Returns the discard pile vector
    vector<Card> getDiscardPile();
};

#endif //INC_8_BIT_DISCARD_H
