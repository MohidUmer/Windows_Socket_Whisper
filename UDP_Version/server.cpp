//========================================//
//     WindowsSocketWhisper - UDP Server   //
//========================================//
//              MOHID UMER                //
//               23i-2130                 //
//                BCY-4B                  //
//========================================//
//
//  Project: WindowsSocketWhisper
//  Protocol: UDP (Connectionless)
//  Platform: Windows (Winsock2)
//  Purpose: Fast broadcast multi-client chat system
//  
//========================================//

//========================================//
//              HEADER PART               //
//========================================//
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <conio.h>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

//========================================//
//          GLOBAL DECLARATIONS           //
//========================================//
const int MAX_CLIENTS = 10;
SOCKET serverSocket;
sockaddr_in clientAddrs[MAX_CLIENTS];
string clientNames[MAX_CLIENTS];
string clientStatuses[MAX_CLIENTS];
int clientCount = 0;
bool serverRunning = true;
CRITICAL_SECTION cs;

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

// Trim Function
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \r\n\t");
    if (first == string::npos) 
        return "";
    size_t last = str.find_last_not_of(" \r\n\t");
    return str.substr(first, (last - first + 1));
}

// Address Comparison Function
bool isSameAddress(sockaddr_in& a, sockaddr_in& b) {
    return (a.sin_addr.S_un.S_addr == b.sin_addr.S_un.S_addr) && (a.sin_port == b.sin_port);
}

// Find client index by address
int findClientIndex(sockaddr_in& addr) {
    for (int i = 0; i < clientCount; ++i) {
        if (isSameAddress(addr, clientAddrs[i])) {
            return i;
        }
    }
    return -1;
}

// Broadcast Function
void broadcastMessage(const string& msg, sockaddr_in* except = nullptr) {
    EnterCriticalSection(&cs);
    for (int i = 0; i < clientCount; ++i) {
        if (except && isSameAddress(clientAddrs[i], *except)) 
            continue;
        sendto(serverSocket, msg.c_str(), msg.length(), 0, 
               (sockaddr*)&clientAddrs[i], sizeof(clientAddrs[i]));
    }
    LeaveCriticalSection(&cs);
}

// Send message to specific client
void sendToClient(const string& msg, sockaddr_in& client) {
    sendto(serverSocket, msg.c_str(), msg.length(), 0, 
           (sockaddr*)&client, sizeof(client));
}

// Display connected clients
void displayConnectedClients() {
    if (clientCount == 0) {
        logActivity("No clients connected", BRIGHT_YELLOW);
        return;
    }
    
    logActivity("Connected Clients:", BRIGHT_CYAN);
    for (int i = 0; i < clientCount; ++i) {
        cout << "   " << (i + 1) << ". " << clientNames[i] << " (" << clientStatuses[i] << ") - " 
             << inet_ntoa(clientAddrs[i].sin_addr) << ":" << ntohs(clientAddrs[i].sin_port) << endl;
    }
}

// File transfer function - SIMPLE VERSION (Non-blocking)
void handleFileTransfer(const string& sender, const string& recipient, const string& filePath, sockaddr_in senderAddr) {
    // Find recipient
    int recipientIndex = -1;
    EnterCriticalSection(&cs);
    for (int i = 0; i < clientCount; ++i) {
        if (clientNames[i] == recipient) {
            recipientIndex = i;
            break;
        }
    }
    LeaveCriticalSection(&cs);

    if (recipientIndex == -1) {
        string errorMsg = "[ERROR] User '" + recipient + "' not found or offline.\n";
        sendToClient(errorMsg, senderAddr);
        return;
    }

    // Try to read file and send first 500 bytes as preview
    ifstream file(filePath);
    if (file.is_open()) {
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        
        // Send file notification to recipient
        string fileInfo = "[FILE] " + sender + " sent you a file: " + filePath + 
                         " (" + to_string(content.length()) + " bytes)\n";
        sendToClient(fileInfo, clientAddrs[recipientIndex]);
        
        // Send file preview (first 500 chars)
        string preview = content.substr(0, min(500, (int)content.length()));
        string fileMsg = "[FILE_CONTENT] From " + sender + " - " + filePath + ":\n" + 
                       preview + (content.length() > 500 ? "\n...[TRUNCATED - First 500 bytes shown]" : "") + "\n";
        sendToClient(fileMsg, clientAddrs[recipientIndex]);
        
        // Notify sender
        string confirmMsg = "[SYSTEM] File " + filePath + " (" + to_string(content.length()) + 
                           " bytes) sent successfully to " + recipient + "\n";
        sendToClient(confirmMsg, senderAddr);
        
        logActivity("File sent: " + sender + " -> " + recipient + " : " + filePath + 
                   " (" + to_string(content.length()) + " bytes)", BRIGHT_MAGENTA);
    } else {
        string errorMsg = "[ERROR] File not found: " + filePath + "\n";
        sendToClient(errorMsg, senderAddr);
    }
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
    
    // Clear screen and display header
    system("cls");
    setColor(BRIGHT_CYAN);
    cout << "************************************************************" << endl;
    cout << "*                 MOHID UMER - 23i-2130                   *" << endl;
    cout << "*                  UDP CHAT SERVER                        *" << endl;
    cout << "************************************************************" << endl;
    resetColor();

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        logActivity("WSAStartup failed!", BRIGHT_RED);
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        logActivity("Socket creation failed!", BRIGHT_RED);
        WSACleanup();
        return 1;
    }

    // Set socket to non-blocking mode for proper shutdown
    u_long mode = 1;
    ioctlsocket(serverSocket, FIONBIO, &mode);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8081);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        logActivity("Bind failed!", BRIGHT_RED);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    setColor(BRIGHT_GREEN);
    cout << "************************************************************" << endl;
    cout << "*             UDP SERVER RUNNING ON PORT 8081             *" << endl;
    cout << "*             Press Ctrl+C to stop the server             *" << endl;
    cout << "************************************************************" << endl;
    resetColor();
    cout << endl;

    logActivity("Server started successfully", BRIGHT_GREEN);

    char buffer[4096];  // Increased buffer size for file transfers

    while (serverRunning) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0, 
                                    (sockaddr*)&clientAddr, &addrLen);

        if (bytesReceived == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK) {
                // No data available, check if we should shutdown
                if (!serverRunning) {
                    break;
                }
                Sleep(100); // Small delay to prevent CPU spinning
                continue;
            } else {
                logActivity("Receive error: " + to_string(error), BRIGHT_RED);
                Sleep(100);
                continue;
            }
        }

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            string msg = trim(buffer);

            int userIndex = findClientIndex(clientAddr);
            bool isNewClient = (userIndex == -1);

            if (isNewClient && clientCount < MAX_CLIENTS) {
                // New client joining - first message is username
                EnterCriticalSection(&cs);
                clientAddrs[clientCount] = clientAddr;
                clientNames[clientCount] = msg;
                clientStatuses[clientCount] = "Available";
                userIndex = clientCount;
                clientCount++;
                LeaveCriticalSection(&cs);

                logActivity("New client joined: " + msg + " from " + 
                           string(inet_ntoa(clientAddr.sin_addr)) + ":" + to_string(ntohs(clientAddr.sin_port)), 
                           BRIGHT_GREEN);
                displayConnectedClients();

                // Send welcome message to new client
                string welcomeMsg = 
                    "\n[SYSTEM] Welcome to UDP Chat Server " + msg + "!\n"
                    "[SYSTEM] Type /help for available commands.\n";
                sendToClient(welcomeMsg, clientAddr);

                // Notify all clients about new user
                string notification = 
                    "\n[NOTIFICATION] " + msg + " joined the chat! (Status: Available)\n";
                broadcastMessage(notification, &clientAddr);
                continue;
            }

            if (userIndex == -1) {
                // Client not registered but server full
                string errorMsg = "[ERROR] Server is full. Cannot accept new connections.\n";
                sendToClient(errorMsg, clientAddr);
                continue;
            }

            string username = clientNames[userIndex];

            // Handle exit command
            if (msg.substr(0, 6) == "/exit ") {
                string exitingUser = msg.substr(6);
                
                logActivity("Client leaving: " + exitingUser, BRIGHT_YELLOW);
                
                // Broadcast exit notification
                string exitNotification = "\n[NOTIFICATION] " + exitingUser + " has left the chat.\n";
                broadcastMessage(exitNotification, &clientAddr);

                // Remove client from active list
                EnterCriticalSection(&cs);
                for (int i = userIndex; i < clientCount - 1; ++i) {
                    clientAddrs[i] = clientAddrs[i + 1];
                    clientNames[i] = clientNames[i + 1];
                    clientStatuses[i] = clientStatuses[i + 1];
                }
                clientCount--;
                LeaveCriticalSection(&cs);
                
                displayConnectedClients();
                continue;
            }

            // Status update command
            else if (msg.substr(0, 8) == "/status ") {
                string newStatus = msg.substr(8);
                EnterCriticalSection(&cs);
                clientStatuses[userIndex] = newStatus;
                LeaveCriticalSection(&cs);
                
                string statusUpdate = "\n[STATUS] " + username + " changed status to: " + newStatus + "\n";
                broadcastMessage(statusUpdate);
                logActivity("Status update: " + username + " -> " + newStatus, BRIGHT_CYAN);
            }
            // Help command
            else if (msg == "/help") {
                string helpMessage = 
                    "\n============================================================\n"
                    "                      COMMANDS HELP                         \n"
                    "============================================================\n"
                    "  /status <new status>  - Update your status\n"
                    "  /list                 - List all online users\n"
                    "  /msg <user> <text>    - Send private message\n"
                    "  /file <user> <path>   - Send file to user (UDP)\n"
                    "  /help                 - Show this help message\n"
                    "  /exit                 - Disconnect from server\n"
                    "============================================================\n";
                sendToClient(helpMessage, clientAddr);
            }
            // List users command
            else if (msg == "/list") {
                string listMessage = 
                    "\n============================================================\n"
                    "                 ONLINE USERS AND STATUSES                  \n"
                    "============================================================\n";
                
                EnterCriticalSection(&cs);
                for (int i = 0; i < clientCount; ++i) {
                    listMessage += "  " + clientNames[i] + " (" + clientStatuses[i] + ")\n";
                }
                LeaveCriticalSection(&cs);
                
                if (clientCount == 0) {
                    listMessage += "  No users online\n";
                }
                
                listMessage += "============================================================\n";
                sendToClient(listMessage, clientAddr);
            }
            // File transfer command - SIMPLE DIRECT CALL (no threads)
            else if (msg.substr(0, 6) == "/file ") {
                istringstream iss(msg);
                string dummy, recipient, filePath;
                iss >> dummy >> recipient >> filePath;

                // Handle file transfer directly (it's fast enough for small files)
                handleFileTransfer(username, recipient, filePath, clientAddr);
            }
            // Private message command
            else if (msg.substr(0, 5) == "/msg ") {
                istringstream iss(msg);
                string dummy, recipient, message;
                iss >> dummy >> recipient;
                getline(iss, message);
                
                // Find recipient
                int recipientIndex = -1;
                EnterCriticalSection(&cs);
                for (int i = 0; i < clientCount; ++i) {
                    if (clientNames[i] == recipient) {
                        recipientIndex = i;
                        break;
                    }
                }
                LeaveCriticalSection(&cs);

                if (recipientIndex == -1) {
                    string errorMsg = "[ERROR] User '" + recipient + "' not found.\n";
                    sendToClient(errorMsg, clientAddr);
                } else {
                    string privateMsg = "[PRIVATE] " + username + ": " + message + "\n";
                    sendToClient(privateMsg, clientAddrs[recipientIndex]);
                    logActivity("Private: " + username + " -> " + recipient, BRIGHT_MAGENTA);
                }
            }
            // Regular broadcast message
            else {
                string broadcastMsg = username + " (" + clientStatuses[userIndex] + "): " + msg + "\n";
                broadcastMessage(broadcastMsg, &clientAddr);
                logActivity("Message from " + username + ": " + msg, BRIGHT_BLUE);
            }
        }
        
        // Check for shutdown condition
        if (!serverRunning) {
            break;
        }
        
        // Display server status periodically
        static time_t lastStatus = 0;
        time_t currentTime = time(NULL);
        if (currentTime - lastStatus >= 30) {
            logActivity("Server Status: " + to_string(clientCount) + " clients connected", BRIGHT_CYAN);
            lastStatus = currentTime;
        }
    }

    // Clean shutdown
    logActivity("Shutting down server...", BRIGHT_RED);
    
    // Notify all clients about server shutdown
    string shutdownMsg = "\n[SYSTEM] Server is shutting down. Goodbye!\n";
    broadcastMessage(shutdownMsg);

    // Small delay to ensure messages are sent
    Sleep(500);

    closesocket(serverSocket);
    WSACleanup();
    DeleteCriticalSection(&cs);
    
    logActivity("Server terminated successfully", BRIGHT_GREEN);
    
    // Small delay to show the final message
    Sleep(1000);
    return 0;
}
