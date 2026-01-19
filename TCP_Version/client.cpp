//========================================//
//    WindowsSocketWhisper - TCP Client    //
//========================================//
//              MOHID UMER                //
//               23i-2130                 //
//                BCY-4B                  //
//========================================//
//
//  Project: WindowsSocketWhisper
//  Protocol: TCP (Connection-Oriented)
//  Platform: Windows (Winsock2)
//  Purpose: Interactive multi-user chat client
//  
//========================================//

//========================================//
//              HEADER PART               //
//========================================//
#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <ctime>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

//========================================//
//          GLOBAL DECLARATIONS           //
//========================================//
SOCKET clientSocket;
string username;

//========================================//
//            COLOR CONSTANTS             //
//========================================//
enum Colors {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,
    BRIGHT_BLUE = 9,
    BRIGHT_GREEN = 10,
    BRIGHT_CYAN = 11,
    BRIGHT_RED = 12,
    BRIGHT_MAGENTA = 13,
    BRIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};

//========================================//
//            FUNCTION PART               //
//========================================//

// Function to set console text color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function to reset console color
void resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRIGHT_WHITE);
}

// Function to clear screen
void clearScreen() {
    system("cls");
}

// Function to display header
void displayHeader() {
    setColor(BRIGHT_CYAN);
    cout << "************************************************************" << endl;
    cout << "*         WindowsSocketWhisper - TCP Client                *" << endl;
    cout << "*                  MOHID UMER - 23i-2130                   *" << endl;
    cout << "************************************************************" << endl;
    resetColor();
}

// Function to display connection status
void displayConnectionStatus(bool connected) {
    if (connected) {
        setColor(BRIGHT_GREEN);
        cout << "************************************************************" << endl;
        cout << "*      Connected to WindowsSocketWhisper - TCP Server      *" << endl;
        cout << "************************************************************" << endl;
    } else {
        setColor(BRIGHT_RED);
        cout << "************************************************************" << endl;
        cout << "*            WindowsSocketWhisper - Connection Failed     *" << endl;
        cout << "************************************************************" << endl;
    }
    resetColor();
    cout << endl;
}

// Function to send a message to server
void sendLine(SOCKET sock, const string& msg) {
    send(sock, msg.c_str(), msg.length(), 0);
}

// Save command to user's local file
void saveCommandToFile(const string& username, const string& command) {
    ofstream outfile(username + ".txt", ios::app);
    if (outfile.is_open()) {
        outfile << command << endl;
        outfile.close();
    }
}

// Show last N commands from local history
void showHistory(const string& username, int num) {
    ifstream infile(username + ".txt");
    if (!infile.is_open()) {
        setColor(BRIGHT_YELLOW);
        cout << "No history available." << endl;
        resetColor();
        return;
    }

    string lines[1000];
    int count = 0;
    while (getline(infile, lines[count]) && count < 1000) {
        count++;
    }
    infile.close();

    int start = count - num;
    if (start < 0) 
        start = 0;

    setColor(BRIGHT_CYAN);
    cout << "============================================================" << endl;
    cout << "                     COMMAND HISTORY                        " << endl;
    cout << "                    (Last " << num << " commands)                   " << endl;
    cout << "============================================================" << endl;
    resetColor();
    
    for (int i = start; i < count; i++) {
        setColor(CYAN);
        cout << "  " << (i - start + 1) << ". ";
        setColor(WHITE);
        cout << lines[i] << endl;
    }
    
    setColor(BRIGHT_CYAN);
    cout << "============================================================" << endl;
    cout << "                     END OF HISTORY                         " << endl;
    cout << "============================================================" << endl;
    resetColor();
}

// Display typing indicator
void showTypingIndicator() {
    setColor(GRAY);
    cout << "Type your message (or /help for commands)..." << endl;
    resetColor();
}

// Display help menu
void displayHelp() {
    setColor(BRIGHT_YELLOW);
    cout << "============================================================" << endl;
    cout << "                      COMMANDS HELP                         " << endl;
    cout << "============================================================" << endl;
    resetColor();
    
    setColor(BRIGHT_GREEN);
    cout << "  /msg <user> <text>    ";
    setColor(WHITE);
    cout << "Send private message" << endl;
    
    setColor(BRIGHT_GREEN);
    cout << "  /file <user> <path>   ";
    setColor(WHITE);
    cout << "Send file to user" << endl;
    
    setColor(BRIGHT_GREEN);
    cout << "  /history [num]        ";
    setColor(WHITE);
    cout << "Show command history (default: 5)" << endl;
    
    setColor(BRIGHT_GREEN);
    cout << "  /help                 ";
    setColor(WHITE);
    cout << "Show this help message" << endl;
    
    setColor(BRIGHT_RED);
    cout << "  /exit                 ";
    setColor(WHITE);
    cout << "Disconnect from server" << endl;
    
    cout << endl;
}

// Receiver Thread for server messages
DWORD WINAPI receiverThread(LPVOID lpParam) {
    char buffer[4096];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            string message(buffer);
            
            // Color coding based on message type
            if (message.find("(private)") != string::npos) {
                setColor(BRIGHT_MAGENTA);
                cout << "\n[PRIVATE] " << message;
            } else if (message.find("sent a file") != string::npos) {
                setColor(BRIGHT_CYAN);
                cout << "\n[FILE] " << message;
            } else if (message.find("User not found") != string::npos || 
                       message.find("File not found") != string::npos ||
                       message.find("Invalid") != string::npos) {
                setColor(BRIGHT_RED);
                cout << "\n[ERROR] " << message;
            } else if (message.find("Welcome") != string::npos || 
                       message.find("successful") != string::npos) {
                setColor(BRIGHT_GREEN);
                cout << "\n[SYSTEM] " << message;
            } else {
                setColor(BRIGHT_BLUE);
                cout << "\n[MESSAGE] " << message;
            }
            
            resetColor();
            cout << "\n> ";
            cout.flush();
        }
        else if (bytesReceived == 0 || bytesReceived == SOCKET_ERROR) {
            setColor(BRIGHT_RED);
            cout << "\n[ERROR] Connection lost from server." << endl;
            resetColor();
            closesocket(clientSocket);
            WSACleanup();
            ExitProcess(0);
        }
    }
    return 0;
}

//========================================//
//               MAIN PART                //
//========================================//
int main() {
    WSADATA wsaData;
    sockaddr_in serverAddr;

    clearScreen();
    displayHeader();

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        setColor(BRIGHT_RED);
        cout << "ERROR: WSAStartup failed!" << endl;
        resetColor();
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        setColor(BRIGHT_RED);
        cout << "ERROR: Socket creation failed!" << endl;
        resetColor();
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    setColor(BRIGHT_YELLOW);
    cout << "Connecting to server at 127.0.0.1:8080..." << endl;
    resetColor();

    // Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        displayConnectionStatus(false);
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    displayConnectionStatus(true);

    // Authentication
    char buffer[2048];
    int bytesReceived;

    // Receive username prompt
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesReceived] = '\0';
    setColor(BRIGHT_CYAN);
    cout << "[AUTH] " << buffer;
    resetColor();
    getline(cin, username);
    sendLine(clientSocket, username + "\n");

    // Receive password prompt
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesReceived] = '\0';
    setColor(BRIGHT_CYAN);
    cout << "[AUTH] " << buffer;
    resetColor();
    string password;
    getline(cin, password);
    sendLine(clientSocket, password + "\n");

    // Receive auth result
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesReceived] = '\0';
    
    if (string(buffer).find("successful") != string::npos) {
        setColor(BRIGHT_GREEN);
        cout << "[SUCCESS] " << buffer;
        resetColor();
    } else {
        setColor(BRIGHT_RED);
        cout << "[FAILED] " << buffer;
        resetColor();
        cout << "Authentication failed. Exiting..." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 0;
    }

    // Create local history file if not exist
    ofstream outfile(username + ".txt", ios::app);
    outfile.close();

    // Receive welcome message
    bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[bytesReceived] = '\0';
    setColor(BRIGHT_BLUE);
    cout << "[SERVER] " << buffer;
    resetColor();

    // Start receiver thread
    DWORD threadId;
    CreateThread(NULL, 0, receiverThread, NULL, 0, &threadId);

    // Display initial help
    displayHelp();
    showTypingIndicator();

    // Command loop
    while (true) {
        string command;
        setColor(BRIGHT_GREEN);
        cout << "> ";
        resetColor();
        getline(cin, command);

        if (command.empty()) 
            continue;

        saveCommandToFile(username, command);

        // Handle /history locally
        if (command.find("/history") == 0) {
            int num = 5; // Default
            size_t spacePos = command.find(' ');
            if (spacePos != string::npos) {
                try {
                    num = stoi(command.substr(spacePos + 1));
                } catch (...) {
                    num = 5;
                }
            }
            showHistory(username, num);
            showTypingIndicator();
            continue; // Don't send /history to server
        }

        // Handle /help locally
        if (command == "/help") {
            displayHelp();
            showTypingIndicator();
            continue;
        }

        // Send to server
        sendLine(clientSocket, command + "\n");

        // Exit command
        if (command == "/exit") {
            setColor(BRIGHT_YELLOW);
            cout << "Disconnecting from server..." << endl;
            resetColor();
            break;
        }
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    
    setColor(BRIGHT_GREEN);
    cout << "Client terminated successfully." << endl;
    resetColor();
    
    return 0;
}