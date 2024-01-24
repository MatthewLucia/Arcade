//
// Created by Matt Lucia on 4/13/23.
//

#ifndef INC_8_BIT_UNO_H
#define INC_8_BIT_UNO_H

#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include "Card.h"
#include "Deck.h"
#include "Discard.h"
#include "Player.h"

using namespace std;

// Global function declarations
int getInt();
char getChar();

// Class representing a game of Uno
class Uno {
private:

    // Fields containing a vector representing the playing order and an int to track the turn
    vector<Player> playingOrder;
    int turn;
public:

    // Constructor prompts user for number of CPU opponents and initializes random playing order
    explicit Uno(Deck &deck);

    // Reverses direction of playing order
    void reverse();

    // Returns the next player int the playing order
    Player* getNext();

    // Moves to the next turn
    void nextTurn();

    // Returns current player
    Player* getCurrentPlayer();

    // Returns the playing order vector
    vector<Player> getPlayingOrder();
};

#endif //INC_8_BIT_UNO_H
