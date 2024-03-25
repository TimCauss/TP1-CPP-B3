// TP1-CPP-B3.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Suivez les commentaires pour obtenir une explication des fonctions
// Je n'ai pas eu le temps de tout commenter x)


#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>


// Var constantes
const int BOARD_SIZE = 6; // Taille de la grille
const int NUM_SHIPS = 4; // Nombre de bateau
const int NUM_PLAYERS = 2; // Nombre de joueur (non utilis�s dans ce programme pour le moment)
const char shipChar = 'S'; // Le contenu de la case de la grille qui contient une partie de bateau
const char seaChar = '0'; // Le contenu de la grille quand aucun bateau n'est dessus ou qu'aucun tir n'a �t� effectu�s sur cette coord.

// structure du ship
struct Ship {
    int size;
    int hits;
    Ship(int s) : size(s), hits(0) {}
};

// Structure de donn�es pour repr�senter la grille
typedef std::vector<std::vector<char>> Board;

// Fonction pour initialiser la grille
void initializeBoard(Board& board) {
    // on initialise la board r�cup en argument, de la taille des constantes BOARD_SIZE * BOARD_SIZE et on le remplit avec la constante seaChar
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

// Fonction qui v�rifie si le navire peut �tre plac�
bool canPlaceShip(const Board& board, int x, int y, int size, int direction) {
    
    //Zone � v�rif, j'ai set un -1 pour le 'design' car je n'aime pas quand les bateaux sont col�es
    //On d�fini la zone gr�ce � la position x, y et la taille des ships
    int startX = std::max(0, x - 1);
    int startY = std::max(0, y - 1);
    int endX = (direction == 0) ? x + 1 : x + size;
    int endY = (direction == 1) ? y + 1 : y + size;
    endX = std::min(endX + 1, BOARD_SIZE);
    endY = std::min(endY + 1, BOARD_SIZE);

    // Boucle de v�rif pour savoir si les cases de la zone d�finies sont occup�es
    for (int i = startX; i < endX; ++i) {   // lignes (i)
        for (int j = startY; j < endY; ++j) { // colonnes (j)
            if (board[i][j] != seaChar) {   // Si occup�e
                return false;               // On return false
            }
        }
    }

    // Si non occup� mais que :
    if ((direction == 0 && y + size > BOARD_SIZE) || (direction == 1 && x + size > BOARD_SIZE)) { // Ca d�passe du plateau
        return false; // On retourne false �galement
    }

    // Si aucun false n'a �t� retourn�
    return true; // C'est ok on place le bateau
}

// Fonction pour placer les ships (A REFAIRE -> peu cr�er une boucle infinie si aucune zone n'est consid�r�e safe!)
void placeShips(Board& board, std::vector<Ship>& ships) { // Prend une Board et un vector en args
    std::srand(static_cast<unsigned int>(std::time(0)));

    for (Ship& ship : ships) {      // On boucle sur chaque navire du vector Ship
        bool placed = false;        // Variable pour v�rifier si le bateau est plac�
        while (!placed) {           // On Boucle tant que le ship n'est pas plac�
            int direction = std::rand() % 2; // On choisi al�atoirement la direction: 0 pour horizontal, 1 pour vertical
            int x = std::rand() % BOARD_SIZE; // On choisi al�atoirement ca coord x puis y
            int y = std::rand() % BOARD_SIZE; 

            if (canPlaceShip(board, x, y, ship.size, direction)) { // Ici on v�rifie la position avec la fonction canPlaceShip
                for (int i = 0; i < ship.size; ++i) { //boucle qui parcourt la longueur du ship
                    if (direction == 0) board[x][y + i] = shipChar; // Place le ship horizontalement si dir = 0
                    else board[x + i][y] = shipChar; // sinon verticalement
                }
                placed = true;
            }
        }
    }

}

// Fonction pour v�rifier si une entr�e utilisateur est valide
bool isValidInput(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE; // retourne false si les coord son hors plateau
}

// Fonction pour v�rifier si tous les navires d'un joueur ont �t� coul�s
// A refaire avec un calcul des hit par rapport au case total des bateaux pour optimiser le jeu
bool areShipsSunk(const Board& board, const std::vector<Ship>& ships) {
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == shipChar) return false;
        }
    }
    return true;
}

bool shootingShip(Board& enemyBoard, int shootX, int shootY) {

    //on verif les coordonn�es
    if (shootX < 0 || shootX >= BOARD_SIZE || shootY < 0 || shootY >= BOARD_SIZE) {
        std::cout << "Invalid coordinates. Try again.\n";
        return false;
    }

    // V�rifier si le tir est sur un ship
    if (enemyBoard[shootX][shootY] == shipChar) {
        enemyBoard[shootX][shootY] = 'X'; // Marque le tir touch� (constante ?)
        return true;
    }
    else if (enemyBoard[shootX][shootY] == seaChar) {
        enemyBoard[shootX][shootY] = '+';  // Marque le tir manqu�
        return false;
    }
    else {
        std::cout << "You have already shot here. Choose different coordinates.\n";
        return false;
    }
}

int main() {
    // D�marrage
    std::string playerNames[2]; // On pr�pare la variable pour stocker les noms des joueurs
    Board playerBoards[2]; // On instancie les board dans un tableau
    std::vector<Ship> playerShips[2] = { {{2}, {3}, {3}, {4}}, {{2}, {3}, {3}, {4}} }; // A FINIR ! cr�er une fonction de cr�ation des ships

    //On r�cup le nom des joueurs et on initialise les boards et les ship des joueurs
    for (int i = 0; i < 2; ++i) {
        std::cout << "Enter the name for Player " << i + 1 << ": ";
        std::getline(std::cin, playerNames[i]);         // R�cup le nom du joueur i
        initializeBoard(playerBoards[i]);               // Initialise le board du joueur i
        placeShips(playerBoards[i], playerShips[i]);    // Place les bateaux du joueur i
    }

    int currentPlayer = 0;  // On d�clare et set le joueur actuel sur 0
    bool gameOn = true;     // On d�clare et set la partie sur True

    // boucle de gameplay
    while (gameOn) { // Tant que gameOn = true
        int enemyPlayer = (currentPlayer + 1) % 2;  // On set l'enemyPlayer (joueur actuel +1) %2 pour rester sur l'interval 0-1
        
        int shootX = 0;                             // On initialise et set les coordonn�es X de tir sur 0
        int shootY = 0;                             // On initialise et set les coordonn�es Y de tir sur 0

        //On affiche le nom du joueur actuel et la grille du joueur d'en face:
        std::cout << playerNames[currentPlayer] << "'s turn. Here's" << playerNames[enemyPlayer] <<"'s board: \n\n";
        printBoard(playerBoards[enemyPlayer]);      // On print la grille du joueur enemy

        //On demande les coordon�es de tir dans un do while qu'on va uiliser comme un try:
        do {
            std::cout << playerNames[currentPlayer] << ", enter your shot coordinates (x,y):\n";
            std::cout << "Choose x coordinate (1 to " << BOARD_SIZE << "): ";
            std::cin >> shootX;
            std::cout << "Choose y coordinate (1 to " << BOARD_SIZE << "): ";
            std::cin >> shootY;

            shootX--; // pour �viter les erreurs (index 0)
            shootY--;
        } while (!isValidInput(shootX, shootY));

        //Tir !
        bool hit = shootingShip(playerBoards[enemyPlayer], shootX, shootY); // fonction pour v�rifier les tirs
        if (hit) {          // Si on touche on l'affiche et on v�rifie s'il reste des bateaux dans la board de l'ennemi avec la fonction areShipsSunk
            std::cout << "Hit!\n\n";
            if (areShipsSunk(playerBoards[enemyPlayer], playerShips[enemyPlayer])) { // Si la fonction areShipSunk retourne true, on annonce le gagnant et on stop le jeu
                std::cout << "All ships have been sunk! " << playerNames[currentPlayer] << " wins!\n"; 
                gameOn = false;
            }
        }
        else {
            std::cout << "Miss!\n\n";
        }

        currentPlayer = enemyPlayer; // On change de joueur
    }
    
    return 0;

}