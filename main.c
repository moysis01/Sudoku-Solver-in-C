#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int value;
    int possibilities[9];
} cell;

void initialize_possibilities(cell sudoku[][9]);
void update_possibilities(cell sudoku[][9], int row, int col, int val);
bool solve_sudoku(cell sudoku[][9]);
bool is_valid(cell sudoku[][9], int row, int col, int val);
bool is_valid_sudoku(cell sudoku[][9]);


int main() {
    cell sudoku[9][9];

    // Read input Sudoku from file
    FILE* fp = fopen("C:\\Users\\c0003255\\CLionProjects\\untitled1\\cmake-build-debug\\sudoku_easy.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open file\n");
        return 1;
    }
    printf("Input Sudoku:\n");
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            fscanf(fp, "%d", &sudoku[row][col].value);
            printf("%d ", sudoku[row][col].value);
            initialize_possibilities(sudoku);
        }
        printf("\n");
    }
    fclose(fp);

    // Solve Sudoku
    if (solve_sudoku(sudoku)) {
        // Print solved Sudoku
        printf("\nSolved Sudoku:\n");
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                printf("%d ", sudoku[row][col].value);
            }
            printf("\n");
        }
    } else {
        printf("\nNo solution found.\n");
    }
    if (is_valid_sudoku(sudoku)) {
        printf("Sudoku is valid!\n");
    } else {
        printf("Sudoku is invalid!\n");
    }


    return 0;
}

// Initialize the possibilities for a cell
void initialize_possibilities(cell sudoku[][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (sudoku[row][col].value == 0) {
                for (int i = 0; i < 9; i++) {
                    sudoku[row][col].possibilities[i] = i + 1;
                }
            }
        }
    }
}

void update_possibilities(cell sudoku[][9], int row, int col, int val) {
    // Update row
    for (int i = 0; i < 9; i++) {
        if (i != col && sudoku[row][i].value == 0) {
            sudoku[row][i].possibilities[val - 1] = 0;
        }
    }

    // Update column
    for (int i = 0; i < 9; i++) {
        if (i != row && sudoku[i][col].value == 0) {
            sudoku[i][col].possibilities[val - 1] = 0;
        }
    }

    // Update box
    int box_row = (row / 3) * 3;
    int box_col = (col / 3) * 3;
    for (int i = box_row; i < box_row + 3; i++) {
        for (int j = box_col; j < box_col + 3; j++) {
            if (i != row && j != col && sudoku[i][j].value == 0) {
                sudoku[i][j].possibilities[val - 1] = 0;
            }
        }
    }
}

bool is_valid(cell sudoku[][9], int row, int col, int val) {
    // Check row
    for (int i = 0; i < 9; i++) {
        if (i != col && sudoku[row][i].value == val) {
            return false;
        }
    }

    // Check column
    for (int i = 0; i < 9; i++) {
        if (i != row && sudoku[i][col].value == val) {
            return false;
        }
    }

    // Check box
    int box_row = (row / 3) * 3;
    int box_col = (col / 3) * 3;
    for (int i = box_row; i < box_row + 3; i++) {
        for (int j = box_col; j < box_col + 3; j++) {
            if (i != row && j != col && sudoku[i][j].value == val) {
                return false;
            }
        }
    }

    // The value is valid
    return true;
}

bool is_valid_sudoku(cell sudoku[][9]) {
    // Check rows
    for (int i = 0; i < 9; i++) {
        bool used[9] = { false };
        for (int j = 0; j < 9; j++) {
            int val = sudoku[i][j].value;
            if (val < 1 || val > 9 || used[val - 1]) {
                return false;
            }
            used[val - 1] = true;
        }
    }

    // Check columns
    for (int j = 0; j < 9; j++) {
        bool used[9] = { false };
        for (int i = 0; i < 9; i++) {
            int val = sudoku[i][j].value;
            if (val < 1 || val > 9 || used[val - 1]) {
                return false;
            }
            used[val - 1] = true;
        }
    }

    // Check sub-boxes
    for (int box_row = 0; box_row < 9; box_row += 3) {
        for (int box_col = 0; box_col < 9; box_col += 3) {
            bool used[9] = { false };
            for (int i = box_row; i < box_row + 3; i++) {
                for (int j = box_col; j < box_col + 3; j++) {
                    int val = sudoku[i][j].value;
                    if (val < 1 || val > 9 || used[val - 1]) {
                        return false;
                    }
                    used[val - 1] = true;
                }
            }
        }
    }

    // The Sudoku is valid
    return true;
}




bool solve_sudoku(cell sudoku[][9]) {
    // Find an empty cell
    int row = -1;
    int col = -1;
    bool found = false;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j].value == 0) {
                row = i;
                col = j;
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    // If there are no empty cells, the Sudoku is solved
    if (!found) {
        return true;
    }

    // Try each possibility for the empty cell
    for (int i = 0; i < 9; i++) {
        if (sudoku[row][col].possibilities[i] != 0) {
            int val = sudoku[row][col].possibilities[i];
            if (is_valid(sudoku, row, col, val)) {
                sudoku[row][col].value = val;
                update_possibilities(sudoku, row, col, val);
                if (solve_sudoku(sudoku)) {
                    return true;
                }
                sudoku[row][col].value = 0;
                initialize_possibilities(sudoku);
            }
        }
    }

    return false;

}


// Nai re , emena estile mou o lupin ena kwdika pou ekamne me C  gia to sudoku , je ekama tin etsi me to gpt . Paw na piw tsiaro