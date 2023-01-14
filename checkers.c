#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define BOARDSIZE 8 // apparently in C, you can't just have const objects inside a subscript operator as the size of an array....

unsigned int blackCount = 12, whiteCount = 12; // keep track of the amount of pieces in the board, assume that the board size is 8

const char whiteSymbol = 'w', blackSymbol = 'b';

struct Pos{ int x, y; };

struct Checker{
    struct Pos pos;

    bool team; // (team) ? white : black;
    bool isPiece;
    bool isQueen;
};

bool isWithinBoard(const int *const rowIndex_input, const int *const columnIndex_input){
    return !(*rowIndex_input < 1 || *rowIndex_input > BOARDSIZE || *columnIndex_input < 1 || *columnIndex_input > BOARDSIZE);
}

bool isValidMove(struct Checker (*const checkerPtr)[BOARDSIZE][BOARDSIZE], const struct Pos *const initialPos, const struct Pos *const targetPos){
    struct Pos diffPos;

    diffPos.x = initialPos->x - targetPos->x;
    diffPos.y = initialPos->y - targetPos->y;

    const bool teamSelect = (*checkerPtr)[initialPos->y - 1][initialPos->x - 1].team;

    if(teamSelect && ((diffPos.x == 1 || diffPos.x == -1) && diffPos.y == 1)) return 1;
    else if(!teamSelect && ((diffPos.x == 1 || diffPos.x == -1) && diffPos.y == -1)) return 1;

    if(teamSelect && (diffPos.x == 2 || diffPos.x == -2) && diffPos.y == 2){
        if(diffPos.x == -2)
            if((*checkerPtr)[initialPos->y - 2][initialPos->x].isPiece && !((*checkerPtr)[initialPos->y - 2][initialPos->x].team))
                return 1;
        if(diffPos.x == 2)
            if((*checkerPtr)[initialPos->y - 2][initialPos->x - 2].isPiece && !((*checkerPtr)[initialPos->y - 2][initialPos->x - 2].team))
                return 1;
    }
    else if(!teamSelect && (diffPos.x == 2 || diffPos.x == -2) && diffPos.y == -2){
        if(diffPos.x == -2)
            if((*checkerPtr)[initialPos->y][initialPos->x].isPiece && (*checkerPtr)[initialPos->y][initialPos->x].team)
                return 1;
        if(diffPos.x == 2)
            if((*checkerPtr)[initialPos->y][initialPos->x - 2].isPiece && (*checkerPtr)[initialPos->y][initialPos->x - 2].team)
                return 1;
    }

    if((*checkerPtr)[initialPos->y - 1][initialPos->x - 1].isQueen)
        if(abs(diffPos.x) == abs(diffPos.y)){
            unsigned int cnt = 0, buf = -1;
            for(struct Pos balls = *initialPos; balls.x != targetPos->x && balls.y != targetPos->y; ){
                if((*checkerPtr)[balls.y - 1][balls.x - 1].isPiece){
                    if((*checkerPtr)[balls.y - 1][balls.x - 1].team && !teamSelect) ++cnt;
                    else if(!((*checkerPtr)[balls.y - 1][balls.x - 1].team) && teamSelect) ++cnt;

                    ++buf;
                }

                if(balls.x < targetPos->x && balls.y < targetPos->y){ ++balls.x; ++balls.y; }
                else if(balls.x > targetPos->x && balls.y < targetPos->y){ --balls.x; ++balls.y; }
                else if(balls.x < targetPos->x && balls.y > targetPos->y){ ++balls.x; --balls.y; }
                else if(balls.x > targetPos->x && balls.y > targetPos->y){ --balls.x; --balls.y; }
            }
            if(buf > 1) return 0;
            if(cnt < 2) return 1;
        }

    return 0;
}

struct Pos sourceSelect(const struct Checker board[BOARDSIZE][BOARDSIZE], const struct Pos *const buf, const bool teamSelect){
    struct Pos posInput = *buf;
    do{
        scanf("%d%d", &posInput.y, &posInput.x);
        if(!isWithinBoard(&posInput.y, &posInput.x)) printf("Both positions must be lower than 1 and 8:\n");
        else if(!(board[posInput.y - 1][posInput.x - 1].isPiece)) printf("Specified source position is not a valid piece:\n");
        else if(board[posInput.y - 1][posInput.x - 1].team != teamSelect) printf("Select a piece that is your color:\n");
    }
    while(!isWithinBoard(&posInput.y, &posInput.x) || !board[posInput.y - 1][posInput.x - 1].isPiece
          || board[posInput.y - 1][posInput.x - 1].team != teamSelect);
    return posInput;
}

void targetSelect(struct Checker (*const checkerPtr)[BOARDSIZE][BOARDSIZE], const struct Pos *const posInput, const bool teamSelect){
    struct Pos initialPos = *posInput;
    struct Pos targetPos;
    do{
        scanf("%d%d", &targetPos.y, &targetPos.x);
        if(!isWithinBoard(&targetPos.y, &targetPos.x)) printf("Both positions must be lower than 1 and 8!\n");
        else if((*checkerPtr)[targetPos.y - 1][targetPos.x - 1].isPiece){
            printf("Specified target position is already occupied!\n");
            printf("It's %s turn - choose your source piece [syntax is \"{source row} {source column}\"]:\n", (teamSelect) ? "white's" : "black's");
            initialPos = sourceSelect(*checkerPtr, &initialPos, teamSelect);
            printf("It's %s turn - choose your target location [syntax is \"{source row} {source column}\"]:\n", (teamSelect) ? "white's" : "black's");
        }
        else if(!isValidMove(checkerPtr, &initialPos, &targetPos)) printf("Not a valid move, try again!\n");
    }
    while(!isWithinBoard(&targetPos.y, &targetPos.x) || ((*checkerPtr)[targetPos.y - 1][targetPos.x - 1].isPiece)
          || !isValidMove(checkerPtr, &initialPos, &targetPos));

    struct Pos diffPos;
    diffPos.x = initialPos.x - targetPos.x;
    diffPos.y = initialPos.y - targetPos.y;

    if(teamSelect && (diffPos.x == 2 || diffPos.x == -2) && diffPos.y == 2){
        if(diffPos.x == -2)
            if((*checkerPtr)[initialPos.y - 2][initialPos.x].isPiece && !((*checkerPtr)[initialPos.y - 2][initialPos.x].team))
                (*checkerPtr)[initialPos.y - 2][initialPos.x].isPiece = 0;
        if(diffPos.x == 2)
            if((*checkerPtr)[initialPos.y - 2][initialPos.x - 2].isPiece && !((*checkerPtr)[initialPos.y - 2][initialPos.x - 2].team))
                (*checkerPtr)[initialPos.y - 2][initialPos.x - 2].isPiece = 0;
        --blackCount;
    }
    else if(!teamSelect && (diffPos.x == 2 || diffPos.x == -2) && diffPos.y == -2){
        if(diffPos.x == -2)
            if((*checkerPtr)[initialPos.y][initialPos.x].isPiece && (*checkerPtr)[initialPos.y][initialPos.x].team)
                (*checkerPtr)[initialPos.y][initialPos.x].isPiece = 0;
        if(diffPos.x == 2)
            if((*checkerPtr)[initialPos.y][initialPos.x - 2].isPiece && (*checkerPtr)[initialPos.y][initialPos.x - 2].team)
                (*checkerPtr)[initialPos.y][initialPos.x - 2].isPiece = 0;
        --whiteCount;
        //printf("\n\n\n white count is: %u, diffPos.x is: %d, diffPos.y is: %d", whiteCount, diffPos.x, diffPos.y);
    }

    if((*checkerPtr)[initialPos.y - 1][initialPos.x - 1].isQueen)
        if(abs(diffPos.x) == abs(diffPos.y)){
            unsigned int cnt = 0;
            struct Pos validPiece;
            for(struct Pos balls = initialPos; balls.x != targetPos.x && balls.y != targetPos.y; ){
                if((*checkerPtr)[balls.y - 1][balls.x - 1].isPiece){
                    if((*checkerPtr)[balls.y - 1][balls.x - 1].team && !teamSelect){ ++cnt; validPiece.x = balls.x; validPiece.y = balls.y; }
                    else if(!((*checkerPtr)[balls.y - 1][balls.x - 1].team) && teamSelect){ ++cnt; validPiece.x = balls.x; validPiece.y = balls.y; }
                }

                if(balls.x < targetPos.x && balls.y < targetPos.y){ ++balls.x; ++balls.y; }
                else if(balls.x > targetPos.x && balls.y < targetPos.y){ --balls.x; ++balls.y; }
                else if(balls.x < targetPos.x && balls.y > targetPos.y){ ++balls.x; --balls.y; }
                else if(balls.x > targetPos.x && balls.y > targetPos.y){ --balls.x; --balls.y; }
            }
            if(cnt == 1){
                ((teamSelect) ? --blackCount : --whiteCount);
                (*checkerPtr)[validPiece.y - 1][validPiece.x - 1].isPiece = 0;
            }
        }

    if(targetPos.y == 1 && (*checkerPtr)[initialPos.y - 1][initialPos.x - 1].team)
        (*checkerPtr)[initialPos.y - 1][initialPos.x - 1].isQueen = 1;
    else if(targetPos.y == 8 && !((*checkerPtr)[initialPos.y - 1][initialPos.x - 1].team))
        (*checkerPtr)[initialPos.y - 1][initialPos.x - 1].isQueen = 1;

    (*checkerPtr)[initialPos.y - 1][initialPos.x - 1].isPiece = 0;
    (*checkerPtr)[targetPos.y - 1][targetPos.x - 1].isPiece = 1;
    (*checkerPtr)[targetPos.y - 1][targetPos.x - 1].isQueen = (*checkerPtr)[initialPos.y - 1][initialPos.x - 1].isQueen;
    (*checkerPtr)[targetPos.y - 1][targetPos.x - 1].team = (*checkerPtr)[initialPos.y - 1][initialPos.x - 1].team;
}

void renderBoard(const struct Checker (*const checkerPtr)[BOARDSIZE][BOARDSIZE]){
    if(checkerPtr == NULL) return;

    printf("\n--- BOARD PREVIEW ---\n");
    for(size_t rowIndex = 0; rowIndex != BOARDSIZE; ++rowIndex){
        printf("      ");
        for(size_t columnIndex = 0; columnIndex != BOARDSIZE; ++columnIndex){
            bool buf = (*checkerPtr)[rowIndex][columnIndex].team;
            if((*checkerPtr)[rowIndex][columnIndex].isPiece){
                if((*checkerPtr)[rowIndex][columnIndex].isQueen) printf("%c", buf ? toupper(whiteSymbol) : toupper(blackSymbol));
                else printf("%c", buf ? whiteSymbol : blackSymbol);
            }
            else printf("-");
        }
        printf("\n");
    }
    printf("--- BOARD PREVIEW ---\n\n");
}

int main(){
    setbuf(stdout, 0);
    printf("======================================================================\n"
            "Welcome to my spaghetti checkers game I made in a few days!\n"
            "There's no AI so please make sure you have friends before playing...\n"
            "======================================================================\n\n"
            "           1 - Start\n"
            "           2 - Exit\n\n"
            "Please enter either 1 or 2:");
    for(char buf[2] = ""; scanf(" %s", buf) != EOF; printf("Please enter either 1 or 2:")){
        int choice = atoi(buf);
        if(choice == 1) break;
        if(choice == 2) return 1;
    }

    // the board is a two-dimensional array and its size will be 8 x 8. each tile (or element) will be of object type Checker
    struct Checker board[BOARDSIZE][BOARDSIZE];

    // iterate through each tile of the board. if the tile follows certain conditions, then make that tile a checker piece
    for(size_t rowIndex = 0; rowIndex != BOARDSIZE; ++rowIndex)
        for(size_t columnIndex = 0; columnIndex != BOARDSIZE; ++columnIndex){
            board[rowIndex][columnIndex].isPiece = 0;
            if((rowIndex < (BOARDSIZE / 2 - 1)) || (rowIndex > (BOARDSIZE / 2)))
                if(rowIndex != columnIndex){
                    board[rowIndex][columnIndex].team = rowIndex > (BOARDSIZE / 2 - 1);
                    board[rowIndex][columnIndex].isPiece = (rowIndex % 2) != (columnIndex % 2); // xor
                    board[rowIndex][columnIndex].isQueen = 0; // assume every piece is not a queen for now
                    board[rowIndex][columnIndex].pos.x = columnIndex; board[rowIndex][columnIndex].pos.y = rowIndex;
                }
        }

    renderBoard(&board);

    while(blackCount != 0 || whiteCount != 0){
        printf("It's black's turn - choose your source piece [syntax is \"{source row} {source column}\"]:\n");
        struct Pos posInput = sourceSelect(board, &posInput, 0);
        printf("It's black's turn - choose your target location [syntax is \"{source row} {source column}\"]:\n");
        targetSelect(&board, &posInput, 0);

        renderBoard(&board);

        printf("It's white's turn - choose your source piece [syntax is \"{source row} {source column}\"]:\n");
        posInput = sourceSelect(board, &posInput, 1);
        printf("It's white's turn - choose your target location [syntax is \"{source row} {source column}\"]:\n");
        targetSelect(&board, &posInput, 1);

        renderBoard(&board);
    }

    printf("\n+++++++++%s wins!\n+++++++++\n", (blackCount == 0) ? "White" : "Black");

    return 0;
}
