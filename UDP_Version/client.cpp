//========================================//
//    WindowsSocketWhisper - UDP Client    //
//========================================//
//              MOHID UMER                //
//               23i-2130                 //
//                BCY-4B                  //
//========================================//
//
//  Project: WindowsSocketWhisper
//  Protocol: UDP (Connectionless)
//  Platform: Windows (Winsock2)
//  Purpose: Fast interactive multi-user chat client
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
sockaddr_in serverAddr;
string username;
bool connected = true;

//========================================//
//            COLOR CONSTANTS             //
//========================================//
enum Colors {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5, 
    YELLOW = 6, WHITE = 7, GRAY = 8, BRIGHT_BLUE = 9, 
    BRIGHT_GREEN = 10, BRIGHT_CYAN = 11, BRIGHT_RED = 12, 
    BRIGHT_MAGENTA = 13, BRIGHT_YELLOW = 14, BRIGHT_WHITE = 15
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
    cout << "*         WindowsSocketWhisper - UDP Client                *" << endl;
    cout << "*                  MOHID UMER - 23i-2130                   *" << endl;
    cout << "************************************************************" << endl;
    resetColor();
}

// Function to send a message to server
void sendLineUDP(SOCKET sock, const string& msg, sockaddr_in& addr) {
    sendto(sock, msg.c_str(), msg.length(), 0, (sockaddr*)&addr, sizeof(addr));
}

// Save command to user's local file
void saveCommandToFile(const string& username, const string& command) {
    ofstream outfile(username + "_udp.txt", ios::app);
    if (outfile.is_open()) {
        outfile << command << endl;
        outfile.close();
    }
}

// Show last N commands from local history
void showHistory(const string& username, int num) {
    ifstream infile(username + "_udp.txt");
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

// Display help menu
void displayHelp() {
    setColor(BRIGHT_YELLOW);
    cout << "============================================================" << endl;
    cout << "                      UDP COMMANDS HELP                     " << endl;
    cout << "============================================================" << endl;
    resetColor();
    
    setColor(BRIGHT_GREEN);
    cout << "  /status <status>      ";
    setColor(WHITE);
    cout << "Update your status" << endl;
    
    setColor(BRIGHT_GREEN);
    cout << "  /list                 ";
    setColor(WHITE);
    cout << "List all online users" << endl;
    
    setColor(BRIGHT_GREEN);
    cout << "  /msg <user> <text>    ";
    setColor(WHITE);
    cout << "Send private message" << endl;
    
    setColor(BRIGHT_GREEN);
    cout << "  /file <user> <path>   ";
    setColor(WHITE);
    cout << "Send file to user (UDP)" << endl;
    
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

// Display typing indicator
void showTypingIndicator() {
    setColor(GRAY);
    cout << "Type your message or command (/help for commands)..." << endl;
    resetColor();
}

// Receiver Thread for server messages
DWORD WINAPI receiverThread(LPVOID lpParam) {
    char buffer[4096];
    sockaddr_in fromAddr;
    int fromLen = sizeof(fromAddr);
    
    while (connected) {
        int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0, 
                                (sockaddr*)&fromAddr, &fromLen);
        
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            string message(buffer);
            
            // Color coding based on message type
            if (message.find("[PRIVATE]") != string::npos) {
                setColor(BRIGHT_MAGENTA);
                cout << "\n" << message;
            } else if (message.find("[NOTIFICATION]") != string::npos) {
                setColor(BRIGHT_CYAN);
                cout << "\n" << message;
            } else if (message.find("[STATUS]") != string::npos) {
                setColor(BRIGHT_YELLOW);
                cout << "\n" << message;
            } else if (message.find("[SYSTEM]") != string::npos) {
                setColor(BRIGHT_BLUE);
                cout << "\n" << message;
            } else if (message.find("[ERROR]") != string::npos) {
                setColor(BRIGHT_RED);
                cout << "\n" << message;
            } else if (message.find("[FILE]") != string::npos) {
                setColor(BRIGHT_GREEN);
                cout << "\n" << message;
            } else if (message.find("[FILE_CONTENT]") != string::npos) {
                setColor(BRIGHT_CYAN);
                cout << "\n" << message;
            } else {
                setColor(WHITE);
                cout << "\n" << message;
            }
            
            resetColor();
            cout << "> ";
            cout.flush();
        }
        else if (bytesReceived == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                // Only show real errors, not "would block" errors
                setColor(BRIGHT_RED);
                cout << "\n[ERROR] Network error: " << error << endl;
                resetColor();
                cout << "> ";
                cout.flush();
            }
        }
        
        Sleep(50); // Small delay to prevent CPU spinning
    }
    return 0;
}

//========================================//
//               MAIN PART                //
//========================================//
int main() {
    WSADATA wsaData;

    clearScreen();
    displayHeader();

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        setColor(BRIGHT_RED);
        cout << "ERROR: WSAStartup failed!" << endl;
        resetColor();
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        setColor(BRIGHT_RED);
        cout << "ERROR: Socket creation failed!" << endl;
        resetColor();
        WSACleanup();
        return 1;
    }

    // Set socket to non-blocking mode
    u_long mode = 1;
    ioctlsocket(clientSocket, FIONBIO, &mode);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    setColor(BRIGHT_YELLOW);
    cout << "Connecting to UDP server at 127.0.0.1:8081..." << endl;
    resetColor();

    // Ask for username
    setColor(BRIGHT_CYAN);
    cout << "Enter your username: ";
    resetColor();
    getline(cin, username);

    // Send the username to the server on first connection
    sendLineUDP(clientSocket, username, serverAddr);
    
    setColor(BRIGHT_GREEN);
    cout << "************************************************************" << endl;
    cout << "*              CONNECTED TO UDP SERVER                    *" << endl;
    cout << "************************************************************" << endl;
    resetColor();
    cout << endl;

    // Create local history file if not exist
    ofstream outfile(username + "_udp.txt", ios::app);
    outfile.close();

    // Start receiver thread
    DWORD threadId;
    CreateThread(NULL, 0, receiverThread, NULL, 0, &threadId);

    // Small delay to receive welcome message
    Sleep(500);

    // Display initial help
    displayHelp();
    showTypingIndicator();

    // Command loop
    while (connected) {
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
            int num = 5;
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
            continue;
        }

        // Handle /help locally
        if (command == "/help") {
            displayHelp();
            showTypingIndicator();
            continue;
        }

        // Handle /exit command
        if (command == "/exit") {
            setColor(BRIGHT_YELLOW);
            cout << "Disconnecting from server..." << endl;
            resetColor();
            sendLineUDP(clientSocket, "/exit " + username, serverAddr);
            
            // Wait a bit for the message to be sent
            Sleep(300);
            
            connected = false;
            break;
        }

        // Send all other commands to server
        sendLineUDP(clientSocket, command, serverAddr);
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
    
    setColor(BRIGHT_GREEN);
    cout << "Successfully disconnected from UDP chat server." << endl;
    resetColor();
    
    return 0;
}