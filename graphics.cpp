#include "Uno.cpp"
#include "graphics.h"
#include <ctime>
#include "rect.cpp"
#include <iostream>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;

void drawPlayerHand();
void drawDiscardPile();

// Colors
const color red(1, 0, 0);
const color white(1, 1, 1);
const color green(0, 1, 0);

// Rectangle to represent the target
Rect target;
// Vector to hold all the bits
vector<Rect> bits;

// Field to store high score value
int highScore = 0;

// Variables to represent the movement of the user
int moveX = 0, moveY = 0;

// Function to initialize the target
void initTarget() {
    dimensions dims;
    dims.height = 10;
    dims.width = 10;
    target = Rect(red, 10*((rand() % 49)+1), 10*((rand() % 49)+1), dims);
    for (int i = 0; i < bits.size(); i++) {
        while (target.isOverlapping(bits[i])) {
            target = Rect(red, 10*((rand() % 49)+1), 10*((rand() % 49)+1), dims);
        }
    }
}

// Function to initialize the bits
void initBits() {
    bits.clear();
    dimensions dims;
    dims.height = 10;
    dims.width = 10;
    Rect rect = Rect(green, 250, 250, dims);
    bits.push_back(rect);
}

// Create enum for each screen
enum Screen {
    START,
    BBB_START,
    BBB_PLAY,
    BBB_END,
    BBB_ABOUT,
    UNO_START,
    UNO_END,
    UNO_PLAY,
    UNO_USER_CARD,
    UNO_COMP_CARD,
    UNO_PLAY_RESULT,
    UNO_USER_COLOR,
    UNO_PRESS_UNO,
    UNO_UNO
};

// Initialize current screen and messages
Screen CURRENT_SCREEN = START;
// Create a deck object
Deck deck;
// Create a discard pile object
Discard pile = Discard(deck);
// Create a game object
Uno game = Uno(deck);
Color color;
string winner, message;
int colorChoice = -1, cardChoice = -1;

void init() {
    srand(time(0));
    initTarget();
    initBits();
    width = 1600;
    height = 800;
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

// Adds a new bit behind the user
void addBit() {
    dimensions dims;
    dims.height = 10;
    dims.width = 10;
    Rect rect = Rect(white, bits[0].getCenterX() - moveX*2, bits[0].getCenterY() - moveY*2, dims);
    bits.emplace_back(rect);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height);

    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE

    /*
     * Draw here
     */

    switch(CURRENT_SCREEN) {
        case START: {
            // Draws Arcade start screen
            print("Welcome to the Arcade!");
            print("Select a game to play!", 100, 70);
            displayFile("../Images/Uno.txt", 8, 50, 90);
            displayFile("../Images/BitbyBit.txt", 8, 250, 90);
            print("UNO!", 105, 280);
            print("Bit by Bit!", 275, 280);
            break;
        }
        case BBB_START: {
            // Draws Bit by Bit start screen
            glRasterPos2f(210, 230);
            glColor3f(1, 1, 1);
            for (const char letter : "Bit by Bit!") {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
            }
            glRasterPos2f(10, 250);
            glColor3f(1, 1, 1);
            for (const char letter : "Press 'p' to play, 'esc' to exit, or 'a' to learn more") {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
            }
            glRasterPos2f(95, 270);
            glColor3f(1, 1, 1);
            for (const char letter : "To return to the arcade, press 'q'") {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
            }
            break;
        }
            // Display while playing
        case BBB_PLAY: {
            // Draws Bit by Bit play screen
            // Draw border
            dimensions dims;
            dims.height = 500;
            dims.width = 1;
            Rect borderY(white, 500, 250, dims);
            dims.height = 1;
            dims.width = 500;
            Rect borderX(white, 250, 500, dims);
            borderX.draw();
            borderY.draw();

            // Draw target
            target.draw();

            // If user is overlapping with the target, add a new bit
            // behind the user, and create a new target
            if (bits[0].isOverlapping(target)) {
                initTarget();
                addBit();
            }

            // Draw bits
            for (Rect &segment: bits) {
                segment.draw();
            }
            break;
        }
            // End screen display
        case BBB_END: {
            // Draws Bit by Bit end screen
            glRasterPos2f(210, 230);
            glColor3f(1, 1, 1);
            for (const char letter : "Game Over") {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
            }
            glRasterPos2f(130, 250);
            glColor3f(1, 1, 1);
            for (const char letter : "Your Score: " + to_string(bits.size()-1) + " High Score: " + to_string(highScore)) {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
            }
            glRasterPos2f(80, 270);
            glColor3f(1, 1, 1);
            for (const char letter : "Press 'p' to play again, or 'esc' to exit") {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
            }
            break;
        }
        case BBB_ABOUT: {
            // Draws about screen
            glRasterPos2f(25, 200);
            glColor3f(1, 1, 1);
            for (const char letter : "Bit by Bit is an arcade game where you try to survive") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            glRasterPos2f(25, 230);
            glColor3f(1, 1, 1);
            for (const char letter : "as long as possible! Use the arrow keys to move around,") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            glRasterPos2f(25, 260);
            glColor3f(1, 1, 1);
            for (const char letter : "and hit the red targets to increase your score! When ") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            glRasterPos2f(25, 290);
            glColor3f(1, 1, 1);
            for (const char letter : "you hit a target, you will leave behind a bit. Make sure") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            glRasterPos2f(25, 320);
            glColor3f(1, 1, 1);
            for (const char letter : "not to touch it, or it'll be game over!") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            glRasterPos2f(80, 400);
            glColor3f(1, 1, 1);
            for (const char letter : "Press 'p' to return to the start screen") {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, letter);
            }
            break;
        }
        case UNO_START: {
            // Draws UNO! Start screen
            glRasterPos2f(210, 230);
            glColor3f(1, 1, 1);
            for (const char letter : "UNO!") {
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter);
            }
            // Prompts user to start game or return to arcade
            print("Press 's' to start!", 180, 250);
            print("To return to the arcade, press 'q'", 110, 270);
            break;
        }
        case UNO_PLAY: {
            // Draws the UNO! play screen
            // Display user hand and discard pile
            drawPlayerHand();
            drawDiscardPile();
            // Display current turn
            if (game.getCurrentPlayer()->getName() == "User") {
                print("Your turn! Press 's' to play!");
                break;
            } else {
                print(game.getCurrentPlayer()->getName() + "'s Turn! Press 's' to continue.");
                break;
            }
        }
        case UNO_PLAY_RESULT: {
            // Draws the result screen from the current turn
            drawPlayerHand();
            drawDiscardPile();
            print(message);
            print("Press 's' to continue", 100, 70);
            break;
        }
        case UNO_END: {
            // Draws the end message
            // Displays winner of the game and thank you message
            print(winner + " wins!");
            print("Thank you for playing!", 100, 60);
            break;
        }
        case UNO_USER_CARD: {
            // Draws user turn
            // Prompts the user to select a card
            print("Select a card to play!");
            drawPlayerHand();
            drawDiscardPile();
            break;
        }
        case UNO_PRESS_UNO: {
            // Draws UNO! message for CPUs, or prompts user
            if (game.getCurrentPlayer()->getName() == "User") {
                print("You have one more card left! press 's' to say Uno!");
                break;
            }
            else {
                print(game.getCurrentPlayer()->getName() + " says: \"Uno\"!");
                print("Press 's' to continue", 100, 70);
                break;
            }
        }
        case UNO_UNO: {
            // Displays UNO! shout
            print(game.getCurrentPlayer()->getName() + " shouts out: UNO!");
            print("Press 's' to continue", 100, 70);
            break;
        }
        case UNO_USER_COLOR: {
            // Draws color selection screen
            drawPlayerHand();
            drawDiscardPile();
            print("Click a color to play!");
            displayFile("../Images/Red.txt", 8, 20, 100);
            displayFile("../Images/Blue.txt", 8, 120, 100);
            displayFile("../Images/Green.txt", 8, 220, 100);
            displayFile("../Images/Yellow.txt", 8, 320, 100);
            break;
        }
    }
    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // Game begin
    if (key == 'q' && CURRENT_SCREEN == BBB_START) {
        CURRENT_SCREEN = START;
    }
    // Go to Bit by Bit play screen from start screen
    if (key == 'p' && CURRENT_SCREEN == BBB_START) {
        CURRENT_SCREEN = BBB_PLAY;
    }
    // Go to Bit by Bit about screen from start screen
    if (key == 'a' && CURRENT_SCREEN == BBB_START) {
        CURRENT_SCREEN = BBB_ABOUT;
    }
    // Go to Bit by Bit start screen from about screen
    if (key == 'p' && CURRENT_SCREEN == BBB_ABOUT) {
        CURRENT_SCREEN = BBB_START;
    }
    // Game restart if desired
    if (key == 'p' && CURRENT_SCREEN == BBB_END) {
        moveX = 0;
        moveY = 0;
        initBits();
        initTarget();
        CURRENT_SCREEN = BBB_PLAY;
    }
    // Go to UNO! display message screen from Uno press uno screen
    if (CURRENT_SCREEN == UNO_PRESS_UNO && key == 's') {
        CURRENT_SCREEN = UNO_UNO;
        return;
    }
    // Go to UNO! play screen from UNO! start screen
    if (CURRENT_SCREEN == UNO_START && key == 's') {
        CURRENT_SCREEN = UNO_PLAY;
        return;
    }
    // Go to arcade start screen from UNO! start screen
    if (CURRENT_SCREEN == UNO_START && key == 'q') {
        CURRENT_SCREEN = START;
    }
    // Go to UNO! User card selection screen from play screen
    if (CURRENT_SCREEN == UNO_PLAY && key == 's') {
        if (game.getCurrentPlayer()->getName() == "User") {
            CURRENT_SCREEN = UNO_USER_CARD;
        }
        else {
            CURRENT_SCREEN = UNO_COMP_CARD;
        }
    }
    // Go to UNO! play screen from result screen
    if (CURRENT_SCREEN == UNO_PLAY_RESULT && key == 's')  {
        CURRENT_SCREEN = UNO_PLAY;
    }
    // Go to UNO! play screen from Uno message display screen
    if (CURRENT_SCREEN == UNO_UNO && key == 's') {
        CURRENT_SCREEN = UNO_PLAY;
    }
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
}

void kbdUp(unsigned char key, int x, int y) {

    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            // Down
            moveX = 0;
            moveY = 10;
            break;
        case GLUT_KEY_LEFT:
            // Left
            moveY = 0;
            moveX = -10;
            break;
        case GLUT_KEY_RIGHT:
            // Right
            moveY = 0;
            moveX = 10;
            break;
        case GLUT_KEY_UP:
            // Up
            moveX = 0;
            moveY = -10;
            break;
    }
    glutPostRedisplay();
}

void cursor(int x, int y) {

    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    // Switches current screen when clicking on game icon
    if (CURRENT_SCREEN == START && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (x > 50 && x < 178 && y > 90 && y < 269) {
            CURRENT_SCREEN = UNO_START;
        }
        else if (x > 250 && x < 378 && y > 90 && y < 269) {
            CURRENT_SCREEN = BBB_START;
        }
    }
    // Selects card to play when clicking on card icon
    if (CURRENT_SCREEN == UNO_USER_CARD && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        for (int i = 0; i < game.getCurrentPlayer()->getHand().size(); i++) {
            if (x > 20 + (100*i) && x < 108 + (100*i) && y > 300 && y < 404) {
                cardChoice = i;
                CURRENT_SCREEN = UNO_PLAY_RESULT;
            }
        }
    }
    // Selects color to play whe clicking on card icon
    if (CURRENT_SCREEN == UNO_USER_COLOR && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        for (int i = 0; i < 4; i++) {
            if (x > 20 + (100*i) && x < 108 + (100*i) && y > 100 && y < 208) {
                colorChoice = i;
                CURRENT_SCREEN = UNO_PLAY_RESULT;
            }
        }
    }
    glutPostRedisplay();
}

void timer(int dummy) {
    if (CURRENT_SCREEN == BBB_PLAY) {
        // If user is offscreen, game is over.
        if (bits[0].getLeftX() < 0 || bits[0].getRightX() > 500 || bits[0].getTopY() < 0 || bits[0].getBottomY() > 500) {
            CURRENT_SCREEN = BBB_END;
        }
        // If user is overlapping with any bits, game is over.
        for (int i = 1; i < bits.size(); i++) {
            if (bits[0].isOverlapping(bits[i])) {
                CURRENT_SCREEN = BBB_END;
            }
        }
    }
    // Updates current high score
    if (bits.size()-1 > highScore) {
        highScore = bits.size()-1;
    }
    // Update user location
    bits[0].setCenter(bits[0].getCenterX() + moveX, bits[0].getCenterY() + moveY);

    if (CURRENT_SCREEN == UNO_COMP_CARD) {
        // When card is selected, make CPU move
        Card move = game.getCurrentPlayer()->compMove(deck, pile, game.getNext());
        // Change message based on card selected
        if (move.type == Type::Wild) {
            message = game.getCurrentPlayer()->getName() + " played a Wild Card!";
            CURRENT_SCREEN = UNO_USER_COLOR;
            return;
        }
        if (move.type == Type::DrawFour) {
            message = game.getCurrentPlayer()->getName() + "played a Draw Four! " + game.getNext()->getName() + " drew four cards!";
            CURRENT_SCREEN = UNO_USER_COLOR;
            return;
        }
        else if (move.type == Type::Skip) {
            message = game.getCurrentPlayer()->getName() + " skipped " + game.getNext()->getName() + "!";
            game.nextTurn();
        }
        else if (move.type == Type::Reverse) {
            message = game.getCurrentPlayer()->getName() + " reversed the playing order!";
            game.reverse();
        }
        else {
            message = game.getCurrentPlayer()->getName() + " played a " + move.toString();
        }
        // Move to the next turn
        game.nextTurn();
        // Switch to results screen
        CURRENT_SCREEN = UNO_PLAY_RESULT;
    }
    if (cardChoice != -1) {
        // If card is selected, make player move
        Card move = game.getCurrentPlayer()->play(deck, pile, game.getNext(), cardChoice);
        cardChoice = -1;
        // Change message based on card that is played
        if (move.type == Type::Wild) {
            message = "You played a Wild Card!";
            CURRENT_SCREEN = UNO_USER_COLOR;
            return;
        }
        else if (move.type == Type::DrawFour) {
            message = "You played a Draw Four! " + game.getNext()->getName() + " drew four cards!";
            CURRENT_SCREEN = UNO_USER_COLOR;
            return;
        }
        else if (move.type == Type::Skip) {
            message = "You skipped " + game.getNext()->getName() + "!";
            game.nextTurn();
        }
        else if (move.type == Type::Reverse) {
            message = "You reversed the playing order!";
            game.reverse();
        }
        else {
            message = "You played a " + pile.getTopCard().toString();
        }
        // Move to the next turn
        game.nextTurn();
        // Move to results screen
        CURRENT_SCREEN = UNO_PLAY_RESULT;
    }
    // Update the top of the discard pile when a wild card color is chosen
    if (colorChoice != -1) {
        vector<Color> colors = {Color::Red, Color::Blue, Color::Green, Color::Yellow};
        pile.discardCard(Card(colors[colorChoice], Type::None));
        colorChoice = -1;
    }
    // If the current player has one card, move to UNO! shout screen
    if (game.getCurrentPlayer()->getHand().size() == 1) {
        CURRENT_SCREEN = UNO_PRESS_UNO;
        return;
    }
    // If the current player has an empty hand, they have won the game and the game is over
    if (game.getCurrentPlayer()->getHand().empty()) {
        if (game.getCurrentPlayer()->getName() == "User") {
            message = "Congratulations! You won! Thank you for playing!";
            winner = "User";
            CURRENT_SCREEN = UNO_END;
            return;
        } else {
            message = game.getCurrentPlayer()->getName() + " Wins! Thank you for playing!";
            winner = game.getCurrentPlayer()->getName();
            CURRENT_SCREEN = UNO_END;
            return;
        }
    }
    if (CURRENT_SCREEN == UNO_PLAY) {
        // Shuffles deck when low
        if (deck.cardsLeft() < 5) {
            deck.setNewDeck(pile.getDiscardPile());
            deck.shuffleDeck();
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, timer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    init();

    glutInit(&argc, argv);          // Initialize GLUT

    glutInitDisplayMode(GLUT_RGBA);

    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(0, 0); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Arcade" /* title */ );

    // Register callback handler for window re-paint event
    glutDisplayFunc(display);

    // Our own OpenGL initialization
    initGL();

    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    glutKeyboardUpFunc(kbdUp);

    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);

    // handles mouse movement
    glutPassiveMotionFunc(cursor);

    // handles mouse click
    glutMouseFunc(mouse);

    // handles timer
    glutTimerFunc(0, timer, 0);

    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}

// Function to draw card icons in the user's hand
void drawPlayerHand() {
    for (Player player: game.getPlayingOrder()) {
        if (player.getName() == "User") {
            for (int i = 0; i < player.getHand().size(); i++) {
                string filename =
                        player.getHand()[i].colorToString() + player.getHand()[i].typeToString() + ".txt";
                displayFile(filename, 8, 20 + (100 * i), 300);
            }
        }
    }
}

// Function to draw the discard pile
void drawDiscardPile() {
    string topCard = pile.getTopCard().colorToString() + pile.getTopCard().typeToString() + ".txt";
    displayFile(topCard, 12, 350, 50);
}