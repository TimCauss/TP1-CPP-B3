// TP1-CPP-B3.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

const int BOARD_SIZE = 6;
const int NUM_SHIPS = 4;
const int NUM_PLAYERS = 2;
const char shipChar = 'S';
const char seaChar = '0';

struct Ship {
    int size;
    int hits;
    Ship(int s) : size(s), hits(0) {}
};

// Structure de données pour représenter la grille
typedef std::vector<std::vector<char>> Board;

// Fonction pour initialiser la grille
void initializeBoard(Board& board) {
    board = Board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, seaChar));
}

// Fonction pour afficher la grille
void printBoard(const Board& board) {
    int boardSize = board.size();

    // num des colonnes:
    std::cout << "   ";
    for (int col = 0; col < boardSize; ++col) {
        std::cout << std::setw(2) << col + 1 << " ";
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

// Fonction qui vérifie si le navire peut être placé
bool canPlaceShip(const Board& board, int x, int y, int size, int direction) {
    //Zone à vérif
    int startX = std::max(0, x - 1);
    int startY = std::max(0, y - 1);
    int endX = (direction == 0) ? x + 1 : x + size;
    int endY = (direction == 1) ? y + 1 : y + size;
    endX = std::min(endX + 1, BOARD_SIZE);
    endY = std::min(endY + 1, BOARD_SIZE);


    for (int i = startX; i < endX; ++i) {
        for (int j = startY; j < endY; ++j) {
            if (board[i][j] != seaChar) {
                return false;
            }
        }
    }

    if ((direction == 0 && y + size > BOARD_SIZE) || (direction == 1 && x + size > BOARD_SIZE)) {
        return false;
    }

    return true;
}

// Fonction pour placer les navires sur la grille
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
            if (cell == shipChar) return false;
        }
    }
    return true;
}

bool shootingShip(Board& enemyBoard, int shootX, int shootY) {

    //on verif les coordonnées
    if (shootX < 0 || shootX >= BOARD_SIZE || shootY < 0 || shootY >= BOARD_SIZE) {
        std::cout << "Invalid coordinates. Try again.\n";
        return false;
    }

    // Vérifier si le tir est sur un ship
    if (enemyBoard[shootX][shootY] == shipChar) {
        enemyBoard[shootX][shootY] = 'X'; 
        return true;
    }
    else if (enemyBoard[shootX][shootY] == seaChar) {
        enemyBoard[shootX][shootY] = '+';  // Marquer le tir manqué
        return false;
    }
    else {
        std::cout << "You have already shot here. Choose different coordinates.\n";
        return false;
    }
}

int main() {
    // Démarrage
    std::string playerNames[2];
    Board playerBoards[2];
    std::vector<Ship> playerShips[2] = { {{2}, {3}, {3}, {4}}, {{2}, {3}, {3}, {4}} }; // rendre aléatoire et en fonction de la constante apres

    //On récup le nom des joueurs
    for (int i = 0; i < 2; ++i) {
        std::cout << "Enter the name for Player " << i + 1 << ": ";
        std::getline(std::cin, playerNames[i]);
        initializeBoard(playerBoards[i]);
        placeShips(playerBoards[i], playerShips[i]);
    }

    int currentPlayer = 0;
    bool gameOn = true;


    while (gameOn) {
        int shootX = 0;
        int shootY = 0;
        //On affiche la grille du joueur d'en face:
        std::cout << playerNames[currentPlayer] << "'s turn. Here's the enemy's board: \n\n";
        int enemyPlayer = (currentPlayer + 1) % 2;
        printBoard(playerBoards[enemyPlayer]);

        //On demande les coordonées de tir
        do {
            std::cout << playerNames[currentPlayer] << ", enter your shot coordinates (x,y):\n";
            std::cout << "Choose x coordinate (1 to " << BOARD_SIZE << "): ";
            std::cin >> shootX;
            std::cout << "Choose y coordinate (1 to " << BOARD_SIZE << "): ";
            std::cin >> shootY;

            shootX--;
            shootY--;
        } while (!isValidInput(shootX, shootY));

        //Tir !
        bool hit = shootingShip(playerBoards[enemyPlayer], shootX, shootY);
        if (hit) {
            std::cout << "Hit!\n\n";
            if (areShipsSunk(playerBoards[enemyPlayer], playerShips[enemyPlayer])) {
                std::cout << "All ships have been sunk! " << playerNames[currentPlayer] << " wins!\n";
                gameOn = false;
            }
        }
        else {
            std::cout << "Miss!\n\n";
        }

        currentPlayer = enemyPlayer;
    }
    
    return 0;

}