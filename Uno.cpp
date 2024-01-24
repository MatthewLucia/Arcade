//
// Source file containing all function definitions
//

#include <fstream>
#include "Uno.h"
#include "graphics.h"

#include <utility>

void print(string str) {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(100, 40);
    for (const char &letter : str) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
    }
}

void print(string str, int x, int y) {
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(x, y);
    for (const char &letter : str) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
    }
}

void displayFile(string filename, int sideLength, int x, int y) {
    ifstream inFile("../Images/" + filename);
    inFile >> noskipws;
    char letter;
    bool draw;
    int originalX = x;
    while (inFile >> letter) {
        draw = true;
        switch(letter) {
            case 'r': glColor3f(1, 0, 0); break;
            case 'g': glColor3f(0, 1, 0); break;
            case 'b': glColor3f(0, 0, 1); break;
            case 'y': glColor3f(1, 1, 0); break;
            case 'w': glColor3f(1, 1, 1); break;
            case 'm': glColor3f(0.5, 0, 0); break;
            case ' ': glColor3f(0, 0, 0); break;
            default: // newline
                draw = false;
                x = originalX;
                y += sideLength;
        }
        if (draw) {
            glBegin(GL_QUADS);
            glVertex2i(x, y);
            glVertex2i(x + sideLength, y);
            glVertex2i(x+sideLength, y+sideLength);
            glVertex2i(x, y+sideLength);
            glEnd();
            x += sideLength;
        }
    }
    inFile.close();

    glFlush();
}

// Overloaded operator for comparing two player objects
bool operator==(Player p1, Player &p2) {
    if (p1.getName() == p2.getName()) {
        return true;
    }
    return false;
}

// Overloaded operator for comparing two card objects
bool operator==(const Card &card1, const Card &card2) {
    if (card1.color == card2.color || card1.type == card2.type) {
        return true;
    }
    return false;
}

// Overloaded operator for comparing two card objects
bool operator!=(const Card &card1, const Card &card2) {
    if (card1.color == card2.color || card1.type == card2.type) {
        return false;
    }
    return true;
}

Card::Card(Color c, Type t) {
    color = c;
    type = t;
}

string Card::toString() const {
    Color c = color;
    Type t = type;
    string output;
    if (c == Color::Red) {
        output = "Red ";
    }
    if (c == Color::Yellow) {
        output = "Yellow ";
    }
    if (c == Color::Blue) {
        output = "Blue ";
    }
    if (c == Color::Green) {
        output = "Green ";
    }
    if (c == Color::None) {
        output = "";
    }
    switch(t) {
        case Type::Zero : return output + "Zero";
        case Type::One : return output + "One";
        case Type::Two : return output + "Two";
        case Type::Three : return output + "Three";
        case Type::Four : return output + "Four";
        case Type::Five : return output + "Five";
        case Type::Six : return output + "Six";
        case Type::Seven : return output + "Seven";
        case Type::Eight : return output + "Eight";
        case Type::Nine : return output + "Nine";
        case Type::Skip : return output + "Skip";
        case Type::Reverse : return output + "Reverse";
        case Type::DrawTwo : return output + "Draw Two";
        case Type::Wild : return "Wild";
        case Type::DrawFour : return "Draw Four";
        default : return "Error";
    }
    return "Error";
}

string Card::colorToString() const {
    Color c = color;
    switch(c) {
        case Color::Green : return "Green";
        case Color::Red : return "Red";
        case Color::Yellow : return "Yellow";
        case Color::Blue : return "Blue";
        case Color::None : return "";
    }
}

string Card::typeToString() const {
    Type t = type;
    switch(t) {
        case Type::DrawFour : return "DrawFour";
        case Type::Wild : return "Wild";
        case Type::DrawTwo : return "DrawTwo";
        case Type::Skip : return "Skip";
        case Type::Reverse : return "Reverse";
        case Type::None : return "";
        case Type::Zero : return "Zero";
        case Type::One : return "One";
        case Type::Two : return "Two";
        case Type::Three : return "Three";
        case Type::Four : return "Four";
        case Type::Five : return "Five";
        case Type::Six : return "Six";
        case Type::Seven : return "Seven";
        case Type::Eight : return "Eight";
        case Type::Nine : return "Nine";
        default : return "";
    }
}

Uno::Uno(Deck &deck) {
    // Add user to playing order
    playingOrder.emplace_back(Player("User", deck));

    // Add CPUs to playing order
    playingOrder.emplace_back(Player("CPU1", deck));
    playingOrder.emplace_back(Player("CPU2", deck));
    playingOrder.emplace_back(Player("CPU3", deck));

    // Shuffle for random playing order
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(playingOrder.begin(), playingOrder.end(), default_random_engine(seed));
    turn = 0;
}

void Uno::reverse() {
    std::reverse(playingOrder.begin(), playingOrder.end());
    turn--;
    if (turn == -1) {
        turn =3;
    }
}

vector<Player> Uno::getPlayingOrder() {
    return playingOrder;
}

Player* Uno::getNext() {
    if (turn == 3) {
        return &playingOrder[0];
    }
    return &playingOrder[turn + 1];
}

void Uno::nextTurn() {
    turn++;
    if (turn == 4) {
        turn = 0;
    }
}

Player* Uno::getCurrentPlayer() {
    return &playingOrder[turn];
}

Deck::Deck() {
    for (int i = 0; i < 4; i++) {
        deck.emplace_back(static_cast<Color>(i), static_cast<Type>(0));
        for (int j = 1; j < 9; j++) {
            deck.emplace_back(static_cast<Color>(i), static_cast<Type>(j));
            deck.emplace_back(static_cast<Color>(i), static_cast<Type>(j));
        }
        for (int k = 10; k < 13; k++) {
            deck.emplace_back(static_cast<Color>(i), static_cast<Type>(k));
        }
        deck.emplace_back(static_cast<Color>(Color::None), static_cast<Type>(Type::Wild));
        deck.emplace_back(static_cast<Color>(Color::None), static_cast<Type>(Type::DrawFour));
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(deck.begin(), deck.end(), default_random_engine(seed));
}

void Deck::shuffleDeck() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(deck.begin(), deck.end(), default_random_engine(seed));
}

Card Deck::draw() {
    Card top = deck.back();
    deck.pop_back();
    return top;
}

int Deck::cardsLeft() {
    return deck.size();
}

void Deck::setNewDeck(vector<Card> d) {
    deck = std::move(d);
}

Discard::Discard(Deck &deck) {
    Card card = deck.draw();
    while (card.color == Color::None || card.type == Type::Reverse || card.type == Type::Skip || card.type == Type::DrawTwo) {
        card = deck.draw();
    }
pile.push_back(card);
}

Card Discard::getTopCard() {
    return pile.back();
}

void Discard::discardCard(const Card card) {
    pile.push_back(card);
}

vector<Card> Discard::getDiscardPile() {
    return pile;
}

Player::Player(string n, Deck &deck) {
    name = std::move(n);
    for (int i = 0; i < 7; i++) {
        hand.push_back(deck.draw());
    }
}

Card Player::play(Deck &deck, Discard &pile, Player* next, int choice) {

    string nums;
    string cards;
    bool playable = false;

    // Determine if there is any possible moves
    for (auto & i : hand) {
        if (i == pile.getTopCard() || i.color == Color::None) {
            playable = true;
        }
    }

    if (playable) {
        if (hand[choice].type == Type::DrawFour) {
                for (int i = 0; i < 4; i++) {
                    next->addCard(deck);
                }
            }

        // Add two cards to next player hand when draw two is played
        if (hand[choice].type == Type::DrawTwo) {
            next->addCard(deck);
            next->addCard(deck);
        }

        pile.discardCard(hand[choice]);
        hand.erase(hand.begin() + choice);
        return hand[choice];
    }
    else {
        // If no moves are available, user must draw a card
        addCard(deck);
        return play(deck, pile, next, choice);
    }
}

string Player::getName() {
    return name;
}

vector<Card> Player::getHand() {
    return hand;
}

void Player::addCard(Deck &deck) {
    hand.push_back(deck.draw());
}

Card Player::compMove(Deck &deck, Discard &pile, Player* next) {

    vector<Color> colors {Color::Red, Color::Blue, Color::Green, Color::Yellow};
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(colors.begin(), colors.end(), default_random_engine(seed));

    // CPU moves based on their hand, prioritizing playing draw twos, skips, and reverses,
    // then playing by number, then by color, and saving wild cards and draw four cards for last
    for (int i = 0; i < hand.size(); i++) {
        if (hand[i].type == pile.getTopCard().type && hand[i].color != Color::None) {

            // Give two cards to the next player when draw two is played
            if (hand[i].type == Type::DrawTwo) {
                Card output = hand[i];
                pile.discardCard(hand[i]);
                next->addCard(deck);
                next->addCard(deck);
                hand.erase(hand.begin() + i);
                return output;
            }

                // Play card based on number
            else {
                Card output = hand[i];
                pile.discardCard(hand[i]);
                hand.erase(hand.begin() + i);
                return output;
            }
        }

        if (hand[i].color == pile.getTopCard().color && hand[i].color != Color::None) {

            if (hand[i].type == Type::DrawTwo) {
                Card output = hand[i];
                pile.discardCard(hand[i]);
                next->addCard(deck);
                next->addCard(deck);
                hand.erase(hand.begin() + i);
                return output;
            }

            Card output = hand[i];
            pile.discardCard(hand[i]);
            hand.erase(hand.begin() + i);
            return output;
        }
    }
    // Select random color when wild card is played
    for (int k = 0; k < hand.size(); k++) {
        Card c1 = Card(colors[0], Type::Wild);
        if (hand[k].type == Type::Wild) {
            Card output = hand[k];
            pile.discardCard(c1);
            hand.erase(hand.begin() + k);
            return output;
        }

            // Give four cards to the next player and select random color when wild card is played
        else if (hand[k].type == Type::DrawFour) {
            Card output = hand[k];
            Card c2 = Card(colors[0], Type::DrawFour);
            for (int j = 0; j < 4; j++) {
                next->addCard(deck);
            }
            pile.discardCard(c2);
            hand.erase(hand.begin() + k);
            return output;
        }
    }

    // If no moves are available, draw card and restart turn recursively
    addCard(deck);
    return compMove(deck, pile, next);
}

