// TP1-CPP-B3.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

const int BOARD_SIZE = 6;
const int NUM_SHIPS = 4;
const int NUM_PLAYERS = 2;
const char shipChar = 'S';

struct Ship {
    int size;
    int hits;
    Ship(int s) : size(s), hits(0) {}
};

// Structure de données pour représenter la grille
typedef std::vector<std::vector<char>> Board;

// Fonction pour initialiser la grille
void initializeBoard(Board& board) {
    board = Board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, '0'));
}

// Fonction pour afficher la grille
void printBoard(const Board& board) {
    int boardSize = board.size();

    // Num des colonnes:
    std::cout << " ";
    for (int col = 0; col < boardSize; ++col) {
        std::cout << std:setw(2) << col + 1 << " ";
    }
    std::cout << "\n";

    for (int row = 0; row < boardSize; ++row) {
        std::cout << std::setw(2) << row + 1 << " ";
        for (int col = 0; col < boardSize; ++col) {
            std::cout << std::setw(2) << board[row][col] << " ";
        }
        std::cout << "\n";
    }
}

void canPlaceShip(const Board& board, int x, int y, int size, int direction) {
    if (direction == 0) {
        if (y + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i) {
            if (board[x][y + i] == shipChar) return false;
        }
    }
    else {
        if (x + size > BOARD_SIZE) return false;
        for (int i = 0; i < size; ++i) {
            if (board[x + i][y] == shipChar) return false;
        }
    }
    return true;
}

// Fonction pour placer les navires aléatoirement sur la grille
void placeShips(Board& board, std::vector<Ship>& ships) {
    std::srand(static_cast<unsigned int>(std::time(0)));


    for (Ship& ship : ships) {
        bool placed = false;
        while (!placed) {
            int direction = std::rand() % 2; // 0: Horizontal, 1: Vertical
            int x = std::rand() % BOARD_SIZE;
            int y = std::rand() % BOARD_SIZE;

            if (canPlaceShip(board, x, y, ship.size, direction)) {
                for (int i = 0; i < ship.size; ++i) {
                    if (direction == 0) board[x][y + i] = shipChar;
                    else board[x + i][y] = shipChar;
                }
                placed = true;
            }
        }
    }

}

// Fonction pour vérifier si une entrée utilisateur est valide
bool isValidInput(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

// Fonction pour vérifier si tous les navires d'un joueur ont été coulés
bool areShipsSunk(const Board& board, const std::vector<Ship>& ships) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == 'S') return false;
        }
    }
    return true;
}

int main() {
    Board board;
    std::vector<Ship> ships = { {2}, {3}, {3}, {4} }; // Initialisation des navires avec des tailles différentes

    initializeBoard(board);
    placeShips(board, ships);
    printBoard(board);

    // La grille affichée devrait maintenant montrer les positions des navires marqués par 'S'
    return 0;

}