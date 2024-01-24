//
// Created by Matt Lucia on 4/13/23.
//

#ifndef INC_8_BIT_CARD_H
#define INC_8_BIT_CARD_H

#include <string>
using namespace std;

// Enum representing color of card
enum class Color {
    Red,
    Green,
    Blue,
    Yellow,
    None
};

// Enum representing type of card
enum class Type {
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Skip,
    Reverse,
    DrawTwo,
    Wild,
    DrawFour,
    None
};

// Struct representing a card
struct Card {
    // Fields
    Color color;
    Type type;


    // Constructor
    Card(Color c, Type t);

    // Returns a  string representation of card
    string toString() const;

    // Returns a string representation of the color of card
    string colorToString() const;

    // Returns a string representation of the type of card
    string typeToString() const;
};


#endif //INC_8_BIT_CARD_H
