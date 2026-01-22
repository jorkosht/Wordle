/**
*
* Solution to course project # <09>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author <Георги Ивайлов Щерев>
* @idnumber <0MI0600664>
* @compiler <Apple Clang compiler>
*
* <Главен файл на играта wordle>
*
*/

#include <iostream>
#include <fstream>
#include <cstdlib>

const int SIZE = 5;
const int TRIES = 6;
const int LETTERS = 26;
const int USERS = 1024;
const int MAX_LEN = 256;
const int MAX_WORDS = 10000;

const char GREEN[] = "\x1b[32m";
const char YELLOW[] = "\x1b[33m";
const char RESET[] = "\x1b[0m";

const char adminName[] = "admin";
const char adminPassword[] = "admin123";

void myStrcpy(const char* source, char* dest)
{
    if (!source)
    {
        return;
    }
    
    while (*source)
    {
        *dest = *source;
        ++source;
        ++dest;
    }
    *dest = *source;
}

bool myStrcmp(const char* arr1, const char* arr2)
{
    if (!arr1 || !arr2)
    {
        return false;
    }
    
    while (*arr1)
    {
        if (*arr1 != *arr2)
        {
            return false;
        }
        ++arr1;
        ++arr2;
    }
    return *arr1 == *arr2;
}

int myStrlen(const char* str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

bool isLowercase(char c)
{
    return c >= 'a' && c <= 'z';
}

bool isValidWord(const char* word)
{
    int len = myStrlen(word);
    if (len != SIZE)
    {
        return false;
    }
    
    for (int i = 0; i < len; i++)
    {
        if (!isLowercase(word[i]))
        {
            return false;
        }
    }
    
    return true;
}

bool isAdmin(const char* username, const char* adminName)
{
    return myStrcmp(username, adminName);
}

bool openFiles(std::ifstream& wordFile, std::fstream& usersFile, std::fstream& leadFile)
{
    wordFile.open("/Applications/Zadachi/Project/Wordle/Wordle/words.txt");
    usersFile.open("/Applications/Zadachi/Project/Wordle/Wordle/users.txt", std::ios::in | std::ios::out | std::ios::app);
    leadFile.open("/Applications/Zadachi/Project/Wordle/Wordle/leaderboard.txt", std::ios::in | std::ios::out | std::ios::app);
    
    if (!wordFile.is_open() || !usersFile.is_open() || !leadFile.is_open())
    {
        std::cout << "Error: Cannot open files" << std::endl;
        return false;
    }
    
    return true;
}

void closeFiles(std::ifstream& wordFile, std::fstream& usersFile, std::fstream& leadFile)
{
    if (wordFile.is_open())
    {
        wordFile.close();
    }
    if (usersFile.is_open())
    {
        usersFile.close();
    }
    if (leadFile.is_open())
    {
        leadFile.close();
    }
}

bool isValidUsername(const char* username)
{
    int len = myStrlen(username);
    return len > 0 && len < MAX_LEN;
}

bool isValidPassword(const char* password)
{
    int len = myStrlen(password);
    return len > 0 && len < MAX_LEN;
}

bool userExists(std::fstream& usersFile, const char* username)
{
    usersFile.clear();
    usersFile.seekg(0);
    
    char existing[MAX_LEN];
    while (usersFile >> existing)
    {
        if (myStrcmp(username, existing))
        {
            usersFile.clear();
            usersFile.seekg(0);
            return true;
        }
        usersFile >> existing;
    }
    
    usersFile.clear();
    usersFile.seekg(0);
    return false;
}

bool verifyPassword(std::fstream& usersFile, const char* username, const char* password)
{
    usersFile.clear();
    usersFile.seekg(0);
    
    char existing[MAX_LEN];
    while (usersFile >> existing)
    {
        if (myStrcmp(username, existing))
        {
            usersFile >> existing;
            usersFile.clear();
            usersFile.seekg(0);
            return myStrcmp(password, existing);
        }
        usersFile >> existing;
    }
    
    usersFile.clear();
    usersFile.seekg(0);
    return false;
}

bool signUp(std::fstream& usersFile, char* username, char* password)
{
    if (!usersFile.is_open())
    {
        std::cout << "Cannot reach file" << std::endl;
        return false;
    }
    
    char tempUser[MAX_LEN];
    char tempPass[MAX_LEN];
    
    std::cout << "Select username: ";
    std::cin >> tempUser;
    
    if (!isValidUsername(tempUser))
    {
        std::cout << "Invalid username!" << std::endl;
        return signUp(usersFile, username, password);
    }
    
    if (myStrcmp(tempUser, adminName))
    {
        std::cout << "This username is reserved!" << std::endl;
        return signUp(usersFile, username, password);
    }
    
    if (userExists(usersFile, tempUser))
    {
        std::cout << "Username is already taken!" << std::endl;
        return signUp(usersFile, username, password);
    }
    
    std::cout << "Select password: ";
    std::cin >> tempPass;
    
    if (!isValidPassword(tempPass))
    {
        std::cout << "Invalid password!" << std::endl;
        return signUp(usersFile, username, password);
    }
    
    myStrcpy(tempUser, username);
    myStrcpy(tempPass, password);
    
    usersFile.clear();
    usersFile.seekp(0);
    usersFile << username << " " << password << std::endl;
    
    std::cout << "User " << username << " registered successfully!" << std::endl;
    
    return true;
}

bool logIn(std::fstream& usersFile, char* username, char* password)
{
    if (!usersFile.is_open())
    {
        std::cout << "Cannot reach file" << std::endl;
        return false;
    }
    
    char tempUser[MAX_LEN];
    char tempPass[MAX_LEN];
    
    std::cout << "Username: ";
    std::cin >> tempUser;
    std::cout << std::endl << "Password: ";
    std::cin >> tempPass;
    std::cout << std::endl;
    
    myStrcpy(tempUser, username);
    myStrcpy(tempPass, password);
    
    if (!userExists(usersFile, username) && !isAdmin(username, adminName))
    {
        std::cout << "User not found!" << std::endl;
        return logIn(usersFile, username, password);
    }
    
    if (!verifyPassword(usersFile, username, password) && !isAdmin(username, adminName))
    {
        std::cout << "Password is incorrect!" << std::endl;
        return logIn(usersFile, username, password);
    }
    
    if (isAdmin(username, adminName) && !myStrcmp(password, adminPassword))
    {
        std::cout << "Password is incorrect!" << std::endl;
        return logIn(usersFile, username, password);
    }
    
    if (isAdmin(username, adminName))
    {
        std::cout << "Welcome, Administrator!" << std::endl;
    }
    
    return true;
}

void printMainMenu()
{
    std::cout << "====== MAIN MENU ======" << std::endl;
    std::cout << "1. Log in" << std::endl;
    std::cout << "2. Sign up" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "=======================" << std::endl;
}

void printPlayerMenu()
{
    std::cout << std::endl;
    std::cout << "===== PLAYER MENU =====" << std::endl;
    std::cout << "1. Play game" << std::endl;
    std::cout << "2. View leaderboard" << std::endl;
    std::cout << "3. Log out" << std::endl;
    std::cout << "=======================" << std::endl;
}

void printAdminMenu()
{
    std::cout << std::endl;
    std::cout << "===== ADMIN MENU =====" << std::endl;
    std::cout << "1. Add word" << std::endl;
    std::cout << "2. Remove word" << std::endl;
    std::cout << "3. View leaderboard" << std::endl;
    std::cout << "4. Log out" << std::endl;
    std::cout << "======================" << std::endl;
}

bool wordExistsInFile(std::ifstream& wordFile, const char* word)
{
    wordFile.clear();
    wordFile.seekg(0);
    
    char existing[MAX_LEN];
    while (wordFile >> existing)
    {
        if (myStrcmp(word, existing))
        {
            wordFile.clear();
            wordFile.seekg(0);
            return true;
        }
    }
    
    wordFile.clear();
    wordFile.seekg(0);
    return false;
}

void addWord(std::ifstream& wordFile)
{
    char newWord[MAX_LEN];
    
    std::cout << "Enter word to add (5 lowercase letters): ";
    std::cin >> newWord;
    
    if (!isValidWord(newWord))
    {
        std::cout << "Invalid word! Must be 5 lowercase letters." << std::endl;
        return;
    }
    
    if (wordExistsInFile(wordFile, newWord))
    {
        std::cout << "Word already exists!" << std::endl;
        return;
    }
    
    wordFile.close();
    std::ofstream wordFileOut("/Applications/Zadachi/Project/Wordle/Wordle/words.txt", std::ios::app);
    
    if (wordFileOut.is_open())
    {
        wordFileOut << newWord << std::endl;
        wordFileOut.close();
        std::cout << "Word added successfully!" << std::endl;
    }
    else
    {
        std::cout << "Error: Cannot write to file" << std::endl;
    }
    
    wordFile.open("/Applications/Zadachi/Project/Wordle/Wordle/words.txt");
}

void removeWord(std::ifstream& wordFile)
{
    char wordToRemove[MAX_LEN];
    
    std::cout << "Enter word to remove: ";
    std::cin >> wordToRemove;
    
    if (!wordExistsInFile(wordFile, wordToRemove))
    {
        std::cout << "Word not found!" << std::endl;
        return;
    }
    
    char allWords[MAX_WORDS][MAX_LEN];
    int wordCount = 0;
    
    wordFile.clear();
    wordFile.seekg(0);
    
    char currentWord[MAX_LEN];
    while (wordFile >> currentWord && wordCount < MAX_WORDS)
    {
        if (!myStrcmp(currentWord, wordToRemove))
        {
            myStrcpy(currentWord, allWords[wordCount]);
            wordCount++;
        }
    }
    
    wordFile.close();
    std::ofstream wordFileOut("/Applications/Zadachi/Project/Wordle/Wordle/words.txt", std::ios::trunc);
    
    if (wordFileOut.is_open())
    {
        for (int i = 0; i < wordCount; i++)
        {
            wordFileOut << allWords[i] << std::endl;
        }
        wordFileOut.close();
        std::cout << "Word removed successfully!" << std::endl;
    }
    else
    {
        std::cout << "Error: Cannot write to file" << std::endl;
    }
    
    wordFile.open("/Applications/Zadachi/Project/Wordle/Wordle/words.txt");
}

unsigned countWords(std::ifstream& wordFile)
{
    unsigned wordCount = 0;
    char word[MAX_LEN];
    
    wordFile.clear();
    wordFile.seekg(0);
    
    while (wordFile >> word)
    {
        ++wordCount;
    }
    
    wordFile.clear();
    wordFile.seekg(0);
    
    return wordCount;
}

void assignWord(std::ifstream& wordFile, char* chosenWord)
{
    if (!wordFile.is_open())
    {
        std::cout << "Cannot reach file" << std::endl;
        return;
    }
    
    unsigned wordCount = countWords(wordFile);
    
    if (wordCount == 0)
    {
        std::cout << "No words available" << std::endl;
        return;
    }
    
    std::srand(std::time(0));
    int randomIndex = std::rand() % wordCount;
    
    char word[MAX_LEN];
    for (int i = 0; i <= randomIndex; ++i)
    {
        wordFile >> word;
    }
    
    wordFile.clear();
    wordFile.seekg(0);
    
    myStrcpy(word, chosenWord);
}

void assignLetters(char* letters)
{
    for (int i = 0; i < LETTERS; ++i)
    {
        *letters = 'a' + i;
        ++letters;
    }
}

bool isWordInFile(std::ifstream& wordFile, const char* word)
{
    wordFile.clear();
    wordFile.seekg(0);
    
    char validWord[MAX_LEN];
    while (wordFile >> validWord)
    {
        if (myStrcmp(word, validWord))
        {
            wordFile.clear();
            wordFile.seekg(0);
            return true;
        }
    }
    
    wordFile.clear();
    wordFile.seekg(0);
    return false;
}

void enterWord(std::ifstream& wordFile, char* word)
{
    char temp[MAX_LEN];
    std::cout << "Guess: ";
    std::cin >> temp;
    
    if (!isValidWord(temp))
    {
        std::cout << "Invalid word! Must be 5 lowercase letters." << std::endl;
        enterWord(wordFile, word);
        return;
    }
    
    myStrcpy(temp, word);
    
    if (!wordFile.is_open())
    {
        std::cout << "Cannot reach file" << std::endl;
        return;
    }
    
    if (!isWordInFile(wordFile, word))
    {
        std::cout << "Invalid guess! Word not in dictionary." << std::endl;
        enterWord(wordFile, word);
    }
}

void numberOfLetters(const char* word, int* numOfLetters)
{
    for (int i = 0; i < LETTERS; ++i)
    {
        *(numOfLetters + i) = 0;
    }
    
    for (int i = 0; i < SIZE; ++i)
    {
        int currLetter = *(word + i) - 'a';
        ++(*(numOfLetters + currLetter));
    }
}

void colourWord(const char* word, const char* chosenWord, int* numOfLetters, char* availableLetters)
{
    bool isGreen[SIZE] = {false};
    
    for (int i = 0; i < SIZE; ++i)
    {
        if (*(word + i) == *(chosenWord + i))
        {
            *(isGreen + i) = true;
            --(*(numOfLetters + (*(word + i) - 'a')));
        }
    }
    
    for (int i = 0; i < SIZE; ++i)
    {
        if (*(isGreen + i))
        {
            std::cout << GREEN << *(word + i) << RESET;
        }
        else
        {
            bool foundYellow = false;
            for (int j = 0; j < SIZE; ++j)
            {
                if (*(word + i) == *(chosenWord + j) && *(numOfLetters + (*(word + i) - 'a')) > 0)
                {
                    foundYellow = true;
                    --(*(numOfLetters + (*(word + i) - 'a')));
                    break;
                }
            }
            
            if (foundYellow)
            {
                std::cout << YELLOW << *(word + i) << RESET;
            }
            else
            {
                std::cout << *(word + i);
                *(availableLetters + (*(word + i) - 'a')) = 0;
            }
        }
    }
}

void displayAvailableLetters(const char* availableLetters)
{
    std::cout << "Available letters: ";
    bool first = true;
    
    for (int i = 0; i < LETTERS; ++i)
    {
        if (*(availableLetters + i))
        {
            if (!first)
            {
                std::cout << ", ";
            }
            std::cout << *(availableLetters + i);
            first = false;
        }
    }
    std::cout << std::endl;
}

void checkWord(std::ifstream& wordFile, char* word, const char* chosenWord, bool& hasWon, int* numOfLetters, char* availableLetters, unsigned& currentTurn)
{
    enterWord(wordFile, word);
    numberOfLetters(chosenWord, numOfLetters);
    colourWord(word, chosenWord, numOfLetters, availableLetters);
    std::cout << std::endl;
    
    if (myStrcmp(word, chosenWord))
    {
        hasWon = true;
        return;
    }
    
    if (currentTurn >= TRIES)
    {
        return;
    }
    
    int remainingGuesses = TRIES - currentTurn;
    if (remainingGuesses == 1)
    {
        std::cout << "You have 1 guess left" << std::endl;
    }
    else
    {
        std::cout << "You have " << remainingGuesses << " guesses left" << std::endl;
    }
    
    displayAvailableLetters(availableLetters);
    
    ++currentTurn;
    checkWord(wordFile, word, chosenWord, hasWon, numOfLetters, availableLetters, currentTurn);
}

void sortLeaderboard(char playerNames[][MAX_LEN], int* playerWins, int* playerGames, int playerCount)
{
    for (int i = 0; i < playerCount - 1; ++i)
    {
        for (int j = 0; j < playerCount - i - 1; ++j)
        {
            if (playerWins[j] < playerWins[j + 1])
            {
                int tempW = playerWins[j];
                playerWins[j] = playerWins[j + 1];
                playerWins[j + 1] = tempW;
                
                int tempG = playerGames[j];
                playerGames[j] = playerGames[j + 1];
                playerGames[j + 1] = tempG;
                
                char tempName[MAX_LEN];
                myStrcpy(playerNames[j], tempName);
                myStrcpy(playerNames[j + 1], playerNames[j]);
                myStrcpy(tempName, playerNames[j + 1]);
            }
        }
    }
}

void writeLeaderboardToFile(std::fstream& leadFile, char playerNames[][MAX_LEN], int* playerWins, int* playerGames, int playerCount)
{
    leadFile.close();
    leadFile.open("/Applications/Zadachi/Project/Wordle/Wordle/leaderboard.txt", std::ios::out | std::ios::trunc);
    
    for (int i = 0; i < playerCount; ++i)
    {
        leadFile << playerNames[i] << " " << playerWins[i] << " " << playerGames[i] << std::endl;
    }
    
    leadFile.close();
    leadFile.open("/Applications/Zadachi/Project/Wordle/Wordle/leaderboard.txt", std::ios::in | std::ios::out);
}

void updateLeaderboard(std::fstream& leadFile, const char* username, bool won)
{
    if (!leadFile.is_open())
    {
        std::cout << "Cannot reach leaderboard file" << std::endl;
        return;
    }
    
    char playerNames[USERS][MAX_LEN];
    int playerWins[USERS];
    int playerGames[USERS];
    int playerCount = 0;
    bool found = false;
    
    leadFile.clear();
    leadFile.seekg(0);
    
    char tempUser[MAX_LEN];
    int tempWins;
    int tempPlayed;
    
    while (leadFile >> tempUser >> tempWins >> tempPlayed && playerCount < USERS)
    {
        myStrcpy(tempUser, playerNames[playerCount]);
        playerWins[playerCount] = tempWins;
        playerGames[playerCount] = tempPlayed;
        
        if (myStrcmp(username, tempUser))
        {
            found = true;
            playerGames[playerCount]++;
            if (won)
            {
                playerWins[playerCount]++;
            }
        }
        
        playerCount++;
    }
    
    if (!found)
    {
        myStrcpy(username, playerNames[playerCount]);
        playerGames[playerCount] = 1;
        playerWins[playerCount] = won ? 1 : 0;
        playerCount++;
    }
    
    sortLeaderboard(playerNames, playerWins, playerGames, playerCount);
    writeLeaderboardToFile(leadFile, playerNames, playerWins, playerGames, playerCount);
}

void displayLeaderboard(std::fstream& leadFile)
{
    if (!leadFile.is_open())
    {
        std::cerr << "Error: Cannot access leaderboard file" << std::endl;
        return;
    }
    
    leadFile.clear();
    leadFile.seekg(0);
    
    char username[MAX_LEN];
    int wins;
    int gamesPlayed;
    int rank = 1;
    
    std::cout << std::endl;
    std::cout << "================ LEADERBOARD ================" << std::endl;
    std::cout << "Rank | Username       | Wins | Games | Win%" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
    
    while (leadFile >> username >> wins >> gamesPlayed && rank <= 10)
    {
        double winrate = (gamesPlayed > 0) ? (static_cast<double>(wins) / gamesPlayed) * 100.0 : 0.0;
        
        std::cout << rank << "      ";
        std::cout << username;
        
        int usernameLen = myStrlen(username);
        for (int i = usernameLen; i < 15; i++)
        {
            std::cout << " ";
        }
        
        std::cout << "  " << wins << "      ";
        std::cout << gamesPlayed << "       ";
        std::cout << static_cast<int>(winrate) << "%" << std::endl;
        
        rank++;
    }
    
    if (rank == 1)
    {
        std::cout << "      No players on leaderboard yet" << std::endl;
    }
    
    std::cout << "=============================================" << std::endl;
    std::cout << std::endl;
    
    leadFile.clear();
    leadFile.seekg(0);
}

void playGame(std::ifstream& wordFile, std::fstream& leadFile, const char* username)
{
    char chosenWord[MAX_LEN];
    char word[MAX_LEN];
    int numOfLetters[LETTERS] = {0};
    bool hasWon = false;
    char availableLetters[LETTERS];
    unsigned currentTurn = 1;
    
    std::cout << std::endl << "Game starting..." << std::endl;
    
    assignWord(wordFile, chosenWord);
    assignLetters(availableLetters);
    
    checkWord(wordFile, word, chosenWord, hasWon, numOfLetters, availableLetters, currentTurn);
    
    if (hasWon)
    {
        std::cout << "You win!" << std::endl;
        updateLeaderboard(leadFile, username, true);
    }
    else
    {
        std::cout << "You lose!" << std::endl << "The word was: " << chosenWord << std::endl;
        updateLeaderboard(leadFile, username, false);
    }
    
    displayLeaderboard(leadFile);
}

bool handleMainMenu(std::fstream& usersFile, char* username, char* password)
{
    char choose;
    
    std::cin >> choose;
    
    if (choose < '1' || choose > '3')
    {
        std::cout << "Invalid input" << std::endl;
        return handleMainMenu(usersFile, username, password);
    }
    
    int choice = choose - '0';

    
    if (choice < 1 || choice > 3)
    {
        std::cout << "Invalid input" << std::endl;
        return handleMainMenu(usersFile, username, password);
    }
    
    switch (choice)
    {
        case 1:
            return logIn(usersFile, username, password);
        case 2:
            return signUp(usersFile, username, password);
        case 3:
            return false;
        default:
            return handleMainMenu(usersFile, username, password);
    }
}

void handleAdminMenu(std::ifstream& wordFile, std::fstream& leadFile)
{
    bool loggedIn = true;
    
    while (loggedIn)
    {
        printAdminMenu();
        char choose;
        
        std::cin >> choose;
        
        if (choose < '1' || choose > '4')
        {
            std::cout << "Invalid choice!" << std::endl;
            continue;
        }
        
        int choice = choose - '0';
        
        switch (choice)
        {
            case 1:
                addWord(wordFile);
                break;
            case 2:
                removeWord(wordFile);
                break;
            case 3:
                displayLeaderboard(leadFile);
                break;
            case 4:
                std::cout << "Logging out..." << std::endl;
                loggedIn = false;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }
    }
}

void handlePlayerMenu(std::ifstream& wordFile, std::fstream& leadFile, const char* username)
{
    bool loggedIn = true;
    
    while (loggedIn)
    {
        printPlayerMenu();
        
        char choose;
        
        std::cin >> choose;
        
        if (choose < '1' || choose > '3')
        {
            std::cout << "Invalid choice!" << std::endl;
            continue;
        }
        
        int choice = choose - '0';
        
        switch (choice)
        {
            case 1:
                playGame(wordFile, leadFile, username);
                break;
            case 2:
                displayLeaderboard(leadFile);
                break;
            case 3:
                std::cout << "Logging out..." << std::endl;
                loggedIn = false;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
                break;
        }
    }
}

int main()
{
    std::ifstream wordFile;
    std::fstream usersFile;
    std::fstream leadFile;
    
    if (!openFiles(wordFile, usersFile, leadFile))
    {
        return 1;
    }
    
    char username[MAX_LEN];
    char password[MAX_LEN];
    bool running = true;
    
    while (running)
    {
        printMainMenu();
        
        if (handleMainMenu(usersFile, username, password))
        {
            if (isAdmin(username, adminName))
            {
                handleAdminMenu(wordFile, leadFile);
            }
            else
            {
                handlePlayerMenu(wordFile, leadFile, username);
            }
        }
        else
        {
            running = false;
        }
    }
    
    closeFiles(wordFile, usersFile, leadFile);
        
    return 0;
}
