#include <stdio.h>
#include <stdlib.h>

// Function to dynamically resize the matrix if needed
void resizeMatrix(int ***matrix, int *rows, int *cols, int newRow, int newCol) {
    int newRows = *rows;
    int newCols = *cols;

    if (newRow >= *rows) newRows = newRow + 1;
    if (newCol >= *cols) newCols = newCol + 1;

    if (newRows > *rows) {
        *matrix = (int **)realloc(*matrix, newRows * sizeof(int *));
        for (int i = *rows; i < newRows; i++) {
            (*matrix)[i] = (int *)calloc(*cols, sizeof(int));
        }
        *rows = newRows;
    }

    if (newCols > *cols) {
        for (int i = 0; i < *rows; i++) {
            (*matrix)[i] = (int *)realloc((*matrix)[i], newCols * sizeof(int));
            for (int j = *cols; j < newCols; j++) {
                (*matrix)[i][j] = 0;
            }
        }
        *cols = newCols;
    }
}

// Function to print the matrix
void printMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == 1) {
                printf("[ X ]");
            } else if (matrix[i][j] == 2) {
                printf("[ O ]");
            } else {
                printf("[   ]");
            }
        }
        printf("\n");
    }
}

// Function to safely get an integer input
int getValidInteger(const char *prompt) {
    char input[100];
    int value;
    char extra;

    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Check if input is a valid integer and doesn't contain any extra characters
            if (sscanf(input, "%d %c", &value, &extra) == 1) {
                return value;
            }
        }
        printf("Invalid input! Please enter a valid integer.\n");
    }
}

// Function to handle player moves
void makeMove(int ***matrix, int *rows, int *cols, int player) {
    int row, col;
    while (1) {
        printf("\nPlayer %d, enter a position (row and column):\n", player);
        row = getValidInteger("Row: ");
        col = getValidInteger("Column: ");

        // Convert 1-based input to 0-based indexing
        row -= 1;
        col -= 1;

        if (row < 0 || col < 0) {
            printf("Invalid input! Row and column must be at least 1.\n");
        } else {
            resizeMatrix(matrix, rows, cols, row, col);
            if ((*matrix)[row][col] == 0) {
                (*matrix)[row][col] = player;
                break;
            } else {
                printf("This cell is already filled! Please choose another cell.\n");
            }
        }
    }
}

// Function to check for a winner
int checkWinner(int **matrix, int rows, int cols, int player) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j + 2 < cols &&
                matrix[i][j] == player &&
                matrix[i][j + 1] == player &&
                matrix[i][j + 2] == player)
                return 1;

            if (i + 2 < rows &&
                matrix[i][j] == player &&
                matrix[i + 1][j] == player &&
                matrix[i + 2][j] == player)
                return 1;

            if (i + 2 < rows && j + 2 < cols &&
                matrix[i][j] == player &&
                matrix[i + 1][j + 1] == player &&
                matrix[i + 2][j + 2] == player)
                return 1;

            if (i + 2 < rows && j - 2 >= 0 &&
                matrix[i][j] == player &&
                matrix[i + 1][j - 1] == player &&
                matrix[i + 2][j - 2] == player)
                return 1;
        }
    }
    return 0;
}

int main() {
    int rows = 3, cols = 3;
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)calloc(cols, sizeof(int));
    }

    int player1, player2, currentPlayer, moveCount = 0;

    // Player 1 chooses X (1) or O (2)
    while (1) {
        player1 = getValidInteger("Player 1, do you want to be X (1) or O (2)? ");
        if (player1 == 1 || player1 == 2) {
            break;
        }
        printf("Invalid input! Player 1, please choose 1 for X or 2 for O.\n");
    }

    player2 = (player1 == 1) ? 2 : 1;
    printf("Player 1 is %s, Player 2 is %s.\n\n", (player1 == 1) ? "X" : "O", (player2 == 1) ? "X" : "O");

    // Start the game
    currentPlayer = player1; // Player 1 starts first
    while (1) {
        makeMove(&matrix, &rows, &cols, currentPlayer);
        printMatrix(matrix, rows, cols);

        if (checkWinner(matrix, rows, cols, currentPlayer)) {
            printf("\nPlayer %d wins! Congratulations!\n", currentPlayer);
            break;
        }

        // Switch to the next player
        currentPlayer = (currentPlayer == player1) ? player2 : player1;
        moveCount++;
    }

    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
