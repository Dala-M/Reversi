/* 
 * File:   Game.c
 * Author: munajedd
 *
 * Created on November 10, 2015, 1:50 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * 
 */
//prints initial board

void printBoard(char board[26][26], int n) {
    printf("  ");
    for (char i = 'a'; i < 'a' + n; i++)
        printf("%c", i);

    printf("\n");

    for (int r = 0; r < n; r++) {
        printf("%c ", 'a' + r);

        //checks if it is midway through the board to print out the B/W
        for (int c = 0; c < n; c++) {
            if ((n / 2 - 1 == r)&&(n / 2 - 1 == c) || ((n / 2 == r)&&(n / 2 == c))) {
                board[r][c] = 'W';
            } else if ((n / 2 == r)&&(n / 2 - 1 == c) || ((n / 2 - 1 == r)&&((n / 2) == c))) {
                board[r][c] = 'B';
            } else {
                board[r][c] = 'U'; //else print out unoccupied
            }

            printf("%c", board[r][c]);
        }
        printf("\n");
    }

}
//prints board

void print(char board[][26], int n) {
    printf("  ");
    for (char i = 'a'; i < 'a' + n; i++)
        printf("%c", i);

    printf("\n");

    //loops through all values of the board and prints them out
    for (int r = 0; r < n; r++) {
        printf("%c ", 'a' + r);

        for (int c = 0; c < n; c++) {

            printf("%c", board[r][c]);
        }
        printf("\n");
    }

}
//checks if the rows and cols are within the bounds of the board

bool positionInBounds(int n, char row, char col) {
    if ((row - 'a') > n || ((col - 'a') > n)) {
        return (false);
    } else
        return (true);

}
//determines what possible moves can be made
void legality(char board[][26], int n, char colour, char opponentColour, int scoreBoard[][26]) {
    int count = 1;
    //loops through the rows and cols of the board and searches for an unoccupied space

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            scoreBoard[r][c] = 0;
            if (board[r][c] == 'U') { //if an unoccupied space is found, loops through all 8 directions around that space
                for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
                    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                        //checks of board block is equal to the opponent colour
                        count = 1;
                        if (board[r + count * deltaRow][c + count * deltaCol] == opponentColour) { //multiplies the delta directions by a factor to keep searching
                            while (board[r + deltaRow * count][c + deltaCol * count] == opponentColour) {
                                count++; //increase counter(multiplier)
                            }
                            //if the player colour is found, the row and col location is a valid location (start location)
                            if (board[r + count * deltaRow][c + count * deltaCol] == colour && positionInBounds(n, r + count * deltaRow, c + count * deltaCol)) {
                                //sets the score for that location
                                scoreBoard[r][c] += count - 1;
                            }

                        }
                    }
                }
            }


        }
    }

}

//determines if the human has any possible moves

bool legalMoves(int n, char board[][26], char colour, char opponentColour, int scoreBoard[][26], bool compValid) {
    legality(board, n, colour, opponentColour, scoreBoard);
    int largest = 0;
    //checks what the largest score is by looping through the array
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scoreBoard[i][j] > largest) {
                largest = scoreBoard[i][j];
            }
        }
    }
    //if the largest value is 0 then the human has no valid moves
    if (largest == 0) {
        if (compValid)
            printf("%c player has no valid move.\n", colour);
        return (false);
    }
    return (true); //returns true if human has valid moves

}

//function that performs the moves of either player
bool placeMove(int n, char board[][26], int row, int col, char playerColour, char opponentColour, bool compTurn) {

    int count;
    int shiftRow, shiftCol; //variables used to store the row and col values
    bool isValid = false;

    //if the user config is an occupied, loop through all 8 directions to find valid locations
    if (board[(row)][(col)] == 'U') {
        for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
            for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
                count = 1;
                if (board[(row) + count * deltaRow][(col) + count * deltaCol] == opponentColour) {
                    while (board[(row) + deltaRow * count][(col) + deltaCol * count] == opponentColour) {
                        count++;
                    }

                    //if the player colour is found, perform switch
                    if (board[(row) + count * deltaRow][(col) + count * deltaCol] == playerColour) {
                        shiftRow = row; //store row and col
                        shiftCol = col;

                        //changes the board config
                        do {
                            board[shiftRow][shiftCol] = playerColour;

                            shiftRow += deltaRow;
                            shiftCol += deltaCol;

                        } while (board[shiftRow][shiftCol] != playerColour);
                        isValid = true; //if the move was successful, the human has inputed a valid move
                    }
                }
            }
        }
    }
    return (isValid);

}

//determines possible moves for computer and makes that move
bool computerMove(char board[][26], int n, char cColour, char pColour, int scoreBoard[26][26], bool compTurn) {

    int moveR, moveC;
    bool validMoves;
    legality(board, n, cColour, pColour, scoreBoard); //determines possible  moves
    int largest = 0;
    //searches for move with the higher score
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scoreBoard[i][j] > largest) {
                largest = scoreBoard[i][j];
                moveR = i;
                moveC = j;
            }
        }
    }
    //if no moves are possible... 
    if (largest == 0) {
        validMoves = false;
        if (legalMoves(n, board, pColour, cColour, scoreBoard, validMoves))
            printf("%c player has no valid move.\n", cColour);

    } 
    //if there is a valid move
    else {
        validMoves = true;
        printf("Computer places %c at %c%c.\n", cColour, moveR + 'a', moveC + 'a'); 
        placeMove(n, board, moveR, moveC, cColour, pColour, compTurn); //make the move with the highest score
        print(board, n);
    }

    return (validMoves);
}

bool playerMove(int n, char board[][26], char row, char col, char pColour, char cColour, bool compTurn) {
    bool validCheck = false;
    printf("Enter move for colour %c (RowCol): ", pColour);
    scanf(" %c %c", &row, &col);

    //checks if the player move is valid and make that move
    validCheck = placeMove(n, board, row - 'a', col - 'a', pColour, cColour, compTurn);

    if (validCheck && positionInBounds(n, row, col)) {
        print(board, n);
    }
    else { //else print out invalid 
        printf("Invalid move.\n");
    }
    return (validCheck);
}

//determines the winner 
void winner(int n, char board[][26]) {
    int scoreB = 0, scoreW = 0;
    //loops through board and calculates the number of B & W
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++) {
            if (board[r][c] == 'B')
                scoreB++;
            else if (board[r][c] == 'W')
                scoreW++;
        }
//determines the winner
    if (scoreB > scoreW)
        printf("B player wins.");
    else if (scoreW > scoreB)
        printf("W player wins.");
    else if (scoreB == scoreW)
        printf("Draw.");
}

int main(int argc, char** argv) {
//declaring vairables
    bool compTurn = false;
    bool validCheck = true;
    int n;
    char board[26][26];
    int scoreBoard[26][26];
    char row, col;
    char compColour, playerColour;
    bool valid1 = true, valid2 = true, validMove = true;

    //prompt user to enter board dimensions
    printf("Enter the board dimension: ");
    scanf("%d", &n);

    //user enters which colour comp is playing
    printf("Computer plays (B/W) : ");
    scanf(" %c", &compColour);

    //if comp is black, then let comp's turn be first 
    if (compColour == 'B') {
        compTurn = true;
        playerColour = 'W';
        printBoard(board, n);
    } else {
        playerColour = 'B';
        printBoard(board, n);

    }

    //loop the game while all conditions of the game are met
    while ((valid1 || valid2) && validMove) {
        //if its the computer's turn, let the comp make a move
        if (compTurn)
            valid1 = computerMove(board, n, compColour, playerColour, scoreBoard, compTurn);
        //let the human make a move
        else {
            valid2 = legalMoves(n, board, playerColour, compColour, scoreBoard, valid1); //check if human has valid moves
            if (valid2)
                validMove = playerMove(n, board, row, col, playerColour, compColour, compTurn);//make move if it is valid
        }
        compTurn = !compTurn; //switch turns

        //end game if both players no longer have any valid moves or if human plays an invalid move
        if ((!valid1 && !valid2) || !validMove)
            break;
    }

    //declare winner
    if (validMove)
        winner(n, board);
    else
        printf("%c player wins.", compColour);

    return (EXIT_SUCCESS);
}
