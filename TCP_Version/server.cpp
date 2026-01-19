//========================================//
//     WindowsSocketWhisper - TCP Server   //
//========================================//
//              MOHID UMER                //
//               23i-2130                 //
//                BCY-4B                  //
//========================================//
//
//  Project: WindowsSocketWhisper
//  Protocol: TCP (Connection-Oriented)
//  Platform: Windows (Winsock2)
//  Purpose: Reliable multi-client chat system
//  
//========================================//

//========================================//
//              HEADER PART               //
//========================================//
#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iomanip>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

//========================================//
//          GLOBAL DECLARATIONS           //
//========================================//
const int MAX_CLIENTS = 10;
SOCKET clientSockets[MAX_CLIENTS];
string clientNames[MAX_CLIENTS];
string clientStatuses[MAX_CLIENTS];
int clientCount = 0;
bool serverRunning = true;
CRITICAL_SECTION cs;  // Critical section for thread safety

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

// Function to display server header
void displayServerHeader() {
    setColor(BRIGHT_CYAN);
    cout << "************************************************************" << endl;
    cout << "*           WindowsSocketWhisper - TCP Server              *" << endl;
    cout << "*                  MOHID UMER - 23i-2130                   *" << endl;
    cout << "************************************************************" << endl;
    resetColor();
}

// Function to display current time
string getCurrentTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    stringstream ss;
    ss << "[" << put_time(localTime, "%H:%M:%S") << "]";
    return ss.str();
}

// Function to log server activity
void logActivity(const string& activity, int color = WHITE) {
    EnterCriticalSection(&cs);
    setColor(GRAY);
    cout << getCurrentTime() << " ";
    setColor(color);
    cout << activity << endl;
    resetColor();
    LeaveCriticalSection(&cs);
}

// Sending Function
void sendLine(SOCKET sock, const string& msg) {
    send(sock, msg.c_str(), msg.length(), 0);
}

// Receiving Function
string recvLine(SOCKET sock) {
    string data;
    char ch;
    while (recv(sock, &ch, 1, 0) > 0) {
        if (ch == '\n') break;
        data += ch;
    }
    return data;
}

// Trim Function
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \r\n\t");
    if (first == string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \r\n\t");
    return str.substr(first, (last - first + 1));
}

// Authentication Function
bool authenticateUser(SOCKET clientSocket, string& username) {
    // Ask for username
    sendLine(clientSocket, "Enter the username: ");
    username = recvLine(clientSocket);
    
    // Ask for password
    sendLine(clientSocket, "Enter the password: ");
    string password = recvLine(clientSocket);
    
    // Check both in the user data file
    ifstream file("userdata.txt");
    string fileUser, filePass;
    while (file >> fileUser >> filePass) {
        // If user found (verified)
        if (fileUser == username && filePass == password) {
            sendLine(clientSocket, "Connection Successful\nUser Authentication successful!\n");
            return true;
        }
    }
    // If user not found (Not verified)
    sendLine(clientSocket, "Connection Failed\nInvalid username or password!\n");
    return false;
}

// Display connected clients
void displayConnectedClients() {
    if (clientCount == 0) {
        logActivity("No clients connected", BRIGHT_YELLOW);
        return;
    }
    
    logActivity("Connected Clients:", BRIGHT_CYAN);
    for (int i = 0; i < clientCount; ++i) {
        cout << "   " << (i + 1) << ". " << clientNames[i] << " (" << clientSockets[i] << ")" << endl;
    }
}

// Client Handling Function
void handleClient(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;
    string username;

    // First of all authenticate the user
    if (!authenticateUser(clientSocket, username)) {
        // if failed close the socket
        logActivity("Authentication failed for socket: " + to_string(clientSocket), BRIGHT_RED);
        closesocket(clientSocket);
        return;
    }

    // Add client to the list
    EnterCriticalSection(&cs);
    if (clientCount < MAX_CLIENTS) {
        clientSockets[clientCount] = clientSocket;
        clientNames[clientCount] = username;
        clientCount++;
    }
    LeaveCriticalSection(&cs);

    logActivity("Client connected: " + username + " (" + to_string(clientSocket) + ")", BRIGHT_GREEN);
    displayConnectedClients();
    
    sendLine(clientSocket, "\nWelcome! Feel Free To Use Commands \nType /help to View commands\n");

    char buffer[4096];
    while (serverRunning) {
        int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            // Client disconnected or error
            logActivity("Client disconnected: " + username + " (" + to_string(clientSocket) + ")", BRIGHT_YELLOW);
            break;
        }
        buffer[bytes] = '\0';

        string cmd(buffer);
        cmd = trim(cmd);

        // Log the received command
        logActivity("From " + username + ": " + cmd, CYAN);

        // Handling the commands
        if (cmd.substr(0, 5) == "/msg ") {
            istringstream iss(cmd);
            string dummy, recipient, message;
            iss >> dummy >> recipient;
            getline(iss, message);
            
            message = username + " (private):" + message;

            bool found = false;
            EnterCriticalSection(&cs);
            for (int i = 0; i < clientCount; ++i) {
                if (clientNames[i] == recipient) {
                    sendLine(clientSockets[i], message + "\n");
                    found = true;
                    logActivity("Private message from " + username + " to " + recipient, BRIGHT_MAGENTA);
                    break;
                }
            }
            LeaveCriticalSection(&cs);
            
            if (!found) {
                sendLine(clientSocket, "User '" + recipient + "' not found or offline.\n");
            }
            
            // Update the history
            ofstream log("history.txt", ios::app);
            log << getCurrentTime() << " " << message << endl;
            
        } else if (cmd.substr(0, 6) == "/file ") {
            istringstream iss(cmd);
            string dummy, recipient, path;
            iss >> dummy >> recipient >> path;

            ifstream file(path, ios::binary);
            if (!file.is_open()) {
                sendLine(clientSocket, "File not found: " + path + "\n");
                continue;
            }
            
            stringstream ss;
            ss << file.rdbuf();
            string content = ss.str();

            bool found = false;
            EnterCriticalSection(&cs);
            for (int i = 0; i < clientCount; ++i) {
                if (clientNames[i] == recipient) {
                    sendLine(clientSockets[i], username + " sent a file: " + path + "\n");
                    sendLine(clientSockets[i], content + "\n");
                    found = true;
                    logActivity("File sent from " + username + " to " + recipient + ": " + path, BRIGHT_CYAN);
                    break;
                }
            }
            LeaveCriticalSection(&cs);
            
            if (!found) {
                sendLine(clientSocket, "User '" + recipient + "' not found or offline.\n");
            }
            
        } else if (cmd.substr(0, 9) == "/history ") {
            int n = 5;
            try {
                n = stoi(cmd.substr(9));
            } catch (...) {
                n = 5;
            }
            
            ifstream log("history.txt");
            string line;
            int count = 0;
            string lastLines[100];

            while (getline(log, line)) {
                lastLines[count % 100] = line;
                count++;
            }

            sendLine(clientSocket, "\nMessage History (Last " + to_string(min(n, count)) + " messages):\n");
            
            int start = max(0, count - n);
            for (int i = start; i < count; i++) {
                sendLine(clientSocket, lastLines[i % 100] + "\n");
            }
            sendLine(clientSocket, "[END]\n");
            
        } else if (cmd == "/help") {
            sendLine(clientSocket,
                "\nAvailable Commands:\n\n"
                "/msg <user> <text>    - Send private message\n"
                "/file <user> <path>   - Send file to user\n"
                "/history <num>        - Show message history\n"
                "/help                 - Show this help\n"
                "/exit                 - Disconnect\n\n"
                "[END]\n");
                
        } else if (cmd == "/exit") {
            break;
        } else {
            sendLine(clientSocket, "Invalid command. Type /help for available commands.\n");
        }
    }

    // Remove client from the list when disconnected
    EnterCriticalSection(&cs);
    for (int i = 0; i < clientCount; ++i) {
        if (clientSockets[i] == clientSocket) {
            closesocket(clientSocket);
            clientSockets[i] = clientSockets[clientCount - 1];
            clientNames[i] = clientNames[clientCount - 1];
            clientCount--;
            break;
        }
    }
    LeaveCriticalSection(&cs);

    logActivity("Client disconnected: " + username + " (" + to_string(clientSocket) + ")", BRIGHT_YELLOW);
    displayConnectedClients();
}

// Server control handler
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        logActivity("Server shutdown initiated...", BRIGHT_RED);
        serverRunning = false;
        return TRUE;
    }
    return FALSE;
}

//========================================//
//               MAIN PART                //
//========================================//
int main() {
    // Initialize critical section
    InitializeCriticalSection(&cs);
    
    // Set console control handler
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
    
    clearScreen();
    displayServerHeader();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logActivity("WSAStartup failed!", BRIGHT_RED);
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        logActivity("Socket creation failed!", BRIGHT_RED);
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        logActivity("Bind failed!", BRIGHT_RED);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        logActivity("Listen failed!", BRIGHT_RED);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    setColor(BRIGHT_GREEN);
    cout << "************************************************************" << endl;
    cout << "*             SERVER RUNNING ON PORT 8080                 *" << endl;
    cout << "*             Press Ctrl+C to stop the server             *" << endl;
    cout << "************************************************************" << endl;
    resetColor();
    cout << endl;

    logActivity("Server started successfully", BRIGHT_GREEN);

    // Main server loop
    while (serverRunning) {
        // Use select to make accept() non-blocking
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(serverSocket, &readSet);
        
        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int result = select(0, &readSet, NULL, NULL, &timeout);
        
        if (result > 0 && FD_ISSET(serverSocket, &readSet)) {
            sockaddr_in clientAddr;
            int size = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &size);

            if (clientSocket == INVALID_SOCKET) {
                if (!serverRunning) break;
                continue;
            }

            if (clientCount >= MAX_CLIENTS) {
                logActivity("Maximum clients reached, rejecting connection", BRIGHT_YELLOW);
                sendLine(clientSocket, "Server is full. Please try again later.\n");
                closesocket(clientSocket);
                continue;
            }

            logActivity("New connection received: " + to_string(clientSocket), BRIGHT_BLUE);
            
            DWORD threadId;
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handleClient, (LPVOID)clientSocket, 0, &threadId);
        }
        
        // Display server status periodically
        static time_t lastStatus = 0;
        time_t currentTime = time(NULL);
        if (currentTime - lastStatus >= 30) { // Every 30 seconds
            logActivity("Server Status: " + to_string(clientCount) + " clients connected", BRIGHT_CYAN);
            lastStatus = currentTime;
        }
    }

    // Clean shutdown
    logActivity("Shutting down server...", BRIGHT_RED);
    
    // Disconnect all clients
    EnterCriticalSection(&cs);
    for (int i = 0; i < clientCount; ++i) {
        sendLine(clientSockets[i], "Server is shutting down. Disconnecting...\n");
        closesocket(clientSockets[i]);
    }
    LeaveCriticalSection(&cs);

    closesocket(serverSocket);
    WSACleanup();
    DeleteCriticalSection(&cs);
    
    logActivity("Server terminated successfully", BRIGHT_GREEN);
    return 0;
}