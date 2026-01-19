# WindowsSocketWhisper - TCP & UDP Chat System

**A Professional Windows-Based Socket Programming Chat System with Dual Protocol Support**

> Real-time messaging at your fingertips. Experience the difference between reliable connections and blazing-fast delivery.

---

## 👨‍💻 Developer
**MOHID UMER** | *Computer Networks (CNET) Assignment*

---

## 📁 Repository Structure

### TCP Protocol Implementation (Connection-Oriented)
* `TCP_Version/` — Reliable, ordered chat system using TCP protocol:
  * `TCP_Version/server.cpp` — WindowsSocketWhisper TCP Server with authentication, multi-threading, and secure file transfer
  * `TCP_Version/client.cpp` — WindowsSocketWhisper TCP Client with colorful UI, command history, and connection management
  * `TCP_Version/userdata.txt` — User credentials database (username/password pairs)

### UDP Protocol Implementation (Connectionless)
* `UDP_Version/` — Fast, broadcast-enabled chat system using UDP protocol:
  * `UDP_Version/server.cpp` — WindowsSocketWhisper UDP Server with client management and broadcast messaging
  * `UDP_Version/client.cpp` — WindowsSocketWhisper UDP Client with status updates and real-time notifications
  * `UDP_Version/userdata.txt` — User credentials database

### Supporting Files
* `.gitignore` — Excludes binaries, logs, and system-generated files
* `README.md` — Comprehensive WindowsSocketWhisper documentation

---

## ✨ What is WindowsSocketWhisper?

**WindowsSocketWhisper** is a production-grade chatting system designed to demonstrate advanced socket programming concepts on Windows platforms. It showcases the practical differences between TCP and UDP protocols through a feature-rich messaging application.

### Key Highlights:
- 🔐 **Secure Authentication** - Username/password based login system
- 💬 **Dual Protocol Support** - Compare TCP reliability vs UDP speed in real-time
- 🎨 **Rich Console UI** - Color-coded messages and organized display
- 📁 **File Transfer Capability** - Send and receive files seamlessly
- 👥 **Multi-Client Support** - Handle multiple concurrent users
- ⏱️ **Command History** - Track user commands locally
- 🔔 **Real-time Notifications** - Join/leave alerts and status updates
- 🌐 **Broadcast Messaging** - Send messages to all users (UDP)
- 📊 **Network Educational Value** - Perfect for learning socket programming

---

## 🚀 Features Comparison

### 🔴 WindowsSocketWhisper - TCP Version Features
* **Reliable Delivery** - Guaranteed message delivery with TCP three-way handshake
* **User Authentication** - Secure login with encrypted credential verification
* **Private Messaging** - Direct user-to-user communication: `/msg <user> <message>`
* **Complete File Transfer** - Send entire file contents: `/file <user> <filepath>`
* **Command History** - Access recent commands: `/history [num]` (stored locally)
* **Thread-Safe Operations** - Critical sections ensure data consistency
* **Ordered Delivery** - Messages arrive in exact transmission order
* **Error Checking** - Built-in packet validation and error recovery

### 🔵 WindowsSocketWhisper - UDP Version Features
* **Fast Connectionless Mode** - No handshake overhead, minimal latency
* **Broadcast Messaging** - Send messages to all connected users simultaneously
* **User Status System** - Update and display status: `/status <new_status>`
* **Connected Users List** - View all online users: `/list`
* **File Preview** - Send first 500 bytes of files: `/file <user> <filepath>`
* **Real-time Notifications** - User join/leave alerts for all clients
* **Low Overhead** - Minimal protocol overhead compared to TCP
* **Connectionless Architecture** - No connection establishment required

---

## 📋 System Requirements

* **Operating System**: Windows (XP or newer recommended)
* **Compiler**: GCC/G++ with Winsock2 support (MinGW recommended)
* **C++ Standard**: C++11 or later
* **Libraries**: Windows Sockets 2 (ws2_32.lib)
* **Network**: Local area network or loopback testing capability
* **Minimum RAM**: 256MB
* **Disk Space**: ~2MB for source and binaries

### Optional Tools
* **Wireshark** - For network packet analysis and educational verification
* **Visual Studio Code** - Recommended IDE for development

---

## 🛠️ Installation & Compilation Guide

### Prerequisites
Ensure you have MinGW or GCC installed with Winsock2 support:
```bash
gcc --version
g++ --version
```

### Compile WindowsSocketWhisper - TCP Version
```bash
cd TCP_Version
g++ -std=c++11 -o server.exe server.cpp -lws2_32 -Wall
g++ -std=c++11 -o client.exe client.cpp -lws2_32 -Wall
```

### Compile WindowsSocketWhisper - UDP Version
```bash
cd UDP_Version
g++ -std=c++11 -o server.exe server.cpp -lws2_32 -Wall
g++ -std=c++11 -o client.exe client.cpp -lws2_32 -Wall
```

**Compilation Flags:**
- `-std=c++11` - Enable C++11 standard features
- `-o` - Specify output executable name
- `-lws2_32` - Link Windows Sockets 2 library (REQUIRED)
- `-Wall` - Show all compiler warnings

---

## 🚀 Quick Start Guide

### Step 1: Start the WindowsSocketWhisper Server

**For TCP Version:**
```bash
cd TCP_Version
./server.exe
```

**For UDP Version:**
```bash
cd UDP_Version
./server.exe
```

You should see:
```
************************************************************
*              WindowsSocketWhisper TCP/UDP SERVER        *
************************************************************
[HH:MM:SS] Server listening on port 8080/8081...
```

### Step 2: Launch WindowsSocketWhisper Clients

**Open multiple terminals and run:**
```bash
cd TCP_Version  (or UDP_Version)
./client.exe
```

**Connection Flow:**
1. Client starts and connects to server
2. Prompts for username
3. Prompts for password (checked against userdata.txt)
4. Upon success: `Connected to WindowsSocketWhisper Server`
5. Ready for messaging

### Step 3: Available Commands

### TCP Commands
```
/msg <user> <text>     Send private message to specific user
/file <user> <path>    Transfer complete file to user  
/history [num]         Show last N commands (default: 5)
/help                  Display all available commands
/exit                  Gracefully disconnect from server
```

### UDP Commands
```
/status <status>       Update your online status
/list                  Show all connected users and their status  
/msg <user> <text>     Send private message
/file <user> <path>    Send file preview (first 500 bytes)
/help                  Display command help
/exit                  Disconnect from server
```

---
### 🔴 TCP Protocol Commands
```
/msg <username> <message>    Send private message to a specific user
/file <username> <filepath>  Transfer complete file to recipient
/history [num]               Display last N commands (default: 10)
/users                       List all connected users
/help                        Display complete command help
/clear                       Clear console screen
/exit or /quit               Disconnect gracefully from server
```

**Example Usage:**
```
/msg alice "Hello from WindowsSocketWhisper!"
/file bob C:\Documents\data.txt
/history 5
/help
```

### 🔵 UDP Protocol Commands
```
/status <status_text>        Update your online status
/list                        Display all connected users and their status
/msg <username> <message>    Send private message to user
/file <username> <filepath>  Send file preview (first 500 bytes)
/history [num]               Display last N commands (default: 10)
/help                        Display command help menu
/clear                       Clear console screen
/exit or /quit               Disconnect from server
```

**Example Usage:**
```
/status "Available for chat"
/list
/msg charlie "Hi there!"
/file diana C:\Pictures\screenshot.jpg
/history 8
```

---

## 🔐 User Authentication

### Setup Credentials

1. **TCP Version** - Edit `TCP_Version/userdata.txt`:
```
alice password123
bob secure_pass
charlie mypassword
diana test123
```

2. **UDP Version** - Edit `UDP_Version/userdata.txt`:
```
alice password123
bob secure_pass
charlie mypassword
diana test123
```

**Format Rules:**
- One credential per line
- Format: `username password`
- Separated by single space
- No special characters or extra spaces
- Case-sensitive

### First-Time Login

When connecting with WindowsSocketWhisper:
```
[SERVER] Enter the username: alice
[SERVER] Enter the password: password123
[SERVER] ✓ Authentication successful!
[SERVER] Welcome to WindowsSocketWhisper, alice!
```

---

## 🔍 Technical Architecture

### 🔴 WindowsSocketWhisper - TCP Protocol Details
* **Protocol**: TCP (Transmission Control Protocol) - Connection-Oriented
* **Port**: 8080 (Configurable)
* **Socket Type**: `SOCK_STREAM` 
* **Maximum Clients**: 10 concurrent connections (configurable)
* **Connection Method**: Three-way handshake (SYN, SYN-ACK, ACK)
* **Delivery Guarantee**: 100% - All packets guaranteed to arrive
* **Order Preservation**: Yes - Messages arrive in transmission order
* **Error Detection**: Built-in checksum and acknowledgments
* **Best For**: Critical messages, file transfers, data integrity required

**TCP Packet Flow:**
```
Client                                Server
  |-- SYN ----------------------------->|
  |<-- SYN-ACK -------------------------|
  |-- ACK ---------------------------->| (Connection established)
  |-- Data (Message) ---------------->|
  |<-- ACK ----------------------------|
  |-- FIN ---------------------------->| (Close connection)
```

### 🔵 WindowsSocketWhisper - UDP Protocol Details
* **Protocol**: UDP (User Datagram Protocol) - Connectionless
* **Port**: 8081 (Configurable)
* **Socket Type**: `SOCK_DGRAM`
* **Maximum Clients**: 10 concurrent connections (configurable)
* **Connection Method**: No connection established - Direct transmission
* **Delivery Guarantee**: No guarantee - Packets may be lost
* **Order Preservation**: No - Packets may arrive out of order
* **Error Detection**: Optional checksum only
* **Best For**: Real-time applications, broadcast messages, speed-critical tasks

**UDP Packet Flow:**
```
Client                                Server
  |-- Datagram (Message) ------------>|
  |-- Datagram (Message) ------------>|
  (No handshake, no acknowledgments)
```

### 📊 Protocol Comparison Table

| Feature | TCP | UDP |
|---------|-----|-----|
| Connection Type | Connection-Oriented | Connectionless |
| Reliability | 100% Guaranteed | Best Effort |
| Speed | Slower (verification overhead) | Faster (minimal overhead) |
| Ordering | In-order delivery | No guarantee |
| Overhead | High (headers, ACKs) | Low (minimal headers) |
| Use Case | File transfer, chat | Streaming, broadcasts |
| Port (WindowsSocketWhisper) | 8080 | 8081 |

---

## 🐛 Troubleshooting Guide

### Compilation Errors

**Error: "undefined reference to `WSAStartup'"**
- **Cause**: Missing Winsock2 library linking
- **Solution**: Ensure `-lws2_32` flag is in compilation command
```bash
g++ -std=c++11 -o server.exe server.cpp -lws2_32
```

**Error: "No such file or directory" for winsock2.h**
- **Cause**: MinGW or compiler not properly installed
- **Solution**: Reinstall MinGW with Winsock support

**Error: "Address already in use"**
- **Cause**: Port 8080/8081 already in use
- **Solution**: Wait 30 seconds or change port number in code

---

### Runtime Errors

**"Connection refused" on client startup**
- **Cause**: Server is not running
- **Solution**: Start the WindowsSocketWhisper server first in another terminal

**"Invalid hostname/IP address"**
- **Cause**: Incorrect server address
- **Solution**: Use `127.0.0.1` for local testing or correct IP address

**"Authentication failed"**
- **Cause**: Wrong credentials or missing userdata.txt
- **Solution**: 
  - Verify credentials in `userdata.txt`
  - Ensure file exists in same directory as server executable
  - Check format: `username password` (space-separated)

**"File transfer failed"**
- **Cause**: File path doesn't exist or permissions denied
- **Solution**:
  - Use absolute file paths
  - Verify file exists: `dir C:\path\to\file.txt`
  - Check file read permissions

**Messages not appearing**
- **Cause**: User offline or network issue
- **Solution**:
  - Verify recipient is connected: Use `/list` command
  - Check firewall allows ports 8080/8081
  - Restart client and server

---

## 🌐 Network Testing & Verification

### Local Machine Testing (Loopback)
```bash
# Terminal 1 - Start TCP Server
cd TCP_Version
./server.exe

# Terminal 2 - Connect Client 1
cd TCP_Version
./client.exe
# Enter: localhost or 127.0.0.1

# Terminal 3 - Connect Client 2
cd TCP_Version
./client.exe
# Enter: localhost or 127.0.0.1

# Test messaging between clients
```

### Network Analysis with Wireshark
1. Launch Wireshark
2. Start capturing on loopback or network interface
3. Run WindowsSocketWhisper server and clients
4. Filter results:
   - TCP traffic: `tcp.port == 8080`
   - UDP traffic: `udp.port == 8081`
5. Observe:
   - **TCP**: Three-way handshake, ACK packets, ordered delivery
   - **UDP**: Direct datagrams, no connection establishment

### Performance Testing
```bash
# Measure message throughput
# Send 1000 messages and note time difference
/msg user "message 1"
/msg user "message 2"
... (repeat for 1000 messages)

# TCP: Should be consistent but slower
# UDP: Should be faster but may lose some packets
```

---

## 📚 Code Architecture Overview

### WindowsSocketWhisper TCP Server (`TCP_Version/server.cpp`)
**Key Components:**
- **WSAStartup()**: Initialize Winsock2 library
- **socket()**: Create TCP socket with SOCK_STREAM
- **bind()**: Attach socket to port 8080
- **listen()**: Enable connection acceptance
- **accept()**: Accept incoming client connections
- **Thread Pool**: Handle each client in separate thread
- **Credential Validation**: Check username/password against userdata.txt
- **Message Routing**: Direct messages to specific clients
- **Critical Sections**: Ensure thread-safe access to shared data

**Thread Safety:**
- CRITICAL_SECTION for protecting clientNames, clientStatuses arrays
- EnterCriticalSection/LeaveCriticalSection around shared resource access

### WindowsSocketWhisper TCP Client (`TCP_Version/client.cpp`)
**Key Components:**
- **WSAStartup()**: Initialize Winsock2
- **socket()**: Create TCP socket
- **connect()**: Establish connection to server
- **Authentication**: Send username/password
- **Message Loop**: Read user input and send to server
- **Receive Thread**: Listen for incoming messages asynchronously
- **Command Parser**: Process special commands like /msg, /file, /history
- **Local Storage**: Save command history to username.txt

### WindowsSocketWhisper UDP Server (`UDP_Version/server.cpp`)
**Key Components:**
- **WSAStartup()**: Initialize Winsock2 library
- **socket()**: Create UDP socket with SOCK_DGRAM
- **bind()**: Attach socket to port 8081
- **recvfrom()**: Receive datagrams from any client
- **sendto()**: Send messages to specific client address
- **Broadcast()**: Send message to all connected clients
- **Client Registry**: Maintain list of active client addresses
- **Stateless Design**: No persistent connection tracking

### WindowsSocketWhisper UDP Client (`UDP_Version/client.cpp`)
**Key Components:**
- **WSAStartup()**: Initialize Winsock2
- **socket()**: Create UDP socket
- **sendto()**: Send datagrams to server address
- **recvfrom()**: Receive datagrams from server
- **Non-blocking Sockets**: Allows simultaneous sending/receiving
- **Local File Tracking**: Store username_udp.txt for history

---

## 🎓 Educational Value & Learning Outcomes

**WindowsSocketWhisper** demonstrates essential networking concepts:

### Socket Programming Fundamentals
- **Winsock2 API**: Windows-specific socket implementation
- **Socket Creation**: Creating TCP (SOCK_STREAM) and UDP (SOCK_DGRAM) sockets
- **Connection Lifecycle**: bind() → listen() → accept() for servers
- **Client Connections**: socket() → connect() for clients
- **Data Transmission**: send()/recv() for TCP, sendto()/recvfrom() for UDP

### Protocol Comparison
- **TCP Handshake**: Observe three-way handshake in Wireshark
- **Reliability Mechanisms**: Sequence numbers, acknowledgments, retransmission
- **UDP Speed**: Connectionless overhead savings
- **Trade-offs**: Reliability vs Performance

### Multi-threaded Programming
- **Thread Creation**: Spawn new thread per client
- **Synchronization**: Critical sections for shared data protection
- **Thread Safety**: Preventing data races and corruption

### Network Communication Patterns
- **Client-Server Architecture**: Centralized message hub
- **Message Routing**: Direct peer-to-peer messaging through server
- **Broadcasting**: Sending to multiple recipients
- **Unicasting**: Point-to-point communication

### Real-World Applications
- **Chat Systems**: Core messaging protocol
- **Game Servers**: Real-time multiplayer communication
- **IoT Devices**: Lightweight UDP for sensor networks
- **Financial Systems**: TCP for guaranteed transaction delivery

---

## 📊 Project Statistics

- **Total Lines of Code**: ~1,800+ (across all 4 files)
- **TCP Server**: ~470 lines
- **TCP Client**: ~410 lines
- **UDP Server**: ~474 lines
- **UDP Client**: ~374 lines
- **Supported Languages**: C++11 or later
- **Target Platform**: Windows (XP or newer)
- **Network Ports Used**: 8080 (TCP), 8081 (UDP)
- **Maximum Concurrent Users**: 10 per server (configurable)
- **Thread Safety**: Fully protected with CRITICAL_SECTION

---

## ✅ Testing Checklist

Before publishing WindowsSocketWhisper, verify:

- [ ] Both TCP and UDP versions compile without errors
- [ ] Server starts successfully and listens on correct ports
- [ ] Clients can authenticate with valid credentials
- [ ] Clients are rejected with invalid credentials
- [ ] Private messages work between clients
- [ ] File transfer functionality is operational
- [ ] Command history is saved and retrievable
- [ ] Multiple clients can connect simultaneously
- [ ] User list displays correctly (/list command)
- [ ] Status updates work (UDP only)
- [ ] Server gracefully handles client disconnections
- [ ] Broadcast messages reach all users (UDP)
- [ ] No memory leaks during extended testing
- [ ] Wireshark analysis shows correct protocol behavior

---

## 🚀 Future Enhancement Ideas

1. **Encryption**: Add SSL/TLS for secure communication
2. **Database Backend**: Replace userdata.txt with SQLite/MySQL
3. **Group Messaging**: Create chat rooms/channels
4. **File Sharing**: Multi-file transfers with progress bar
5. **Voice Calling**: Integrate audio streaming
6. **Mobile Clients**: Port to Android/iOS
7. **Web Interface**: Create web-based client
8. **Message Persistence**: Store chat history
9. **User Profiles**: Avatar, bio, status updates
10. **Admin Dashboard**: Server management interface

---

## 📜 License & Academic Integrity

**WindowsSocketWhisper** is provided as an educational resource for learning socket programming and network communication concepts.

### Usage Terms:
✅ **Allowed:**
- Educational study and learning
- Academic assignments (with proper attribution)
- Network analysis and protocol learning
- Personal experimentation and modification
- Running locally for testing

❌ **Not Allowed:**
- Commercial use without modification
- Academic plagiarism or cheating
- Unauthorized distribution
- Removal of attribution credits
- Malicious use or network interference

### Attribution:
When using or referencing WindowsSocketWhisper, please credit:
```
WindowsSocketWhisper - Windows Socket Chat System
Developer: Mohid Umer
Contact: mohidumer112@gmail.com
```

---

## 👨‍💻 About the Developer

**Name**: Mohid Umer  
**Email**: mohidumer112@gmail.com  

---

## 🤝 Contributing & Support

For bug reports, feature suggestions, or questions about WindowsSocketWhisper:

1. **Report Issues**: Document the exact error and reproduction steps
2. **Suggest Features**: Describe enhancement ideas with use cases
3. **Ask Questions**: Reach out via email with specific technical questions

**Contact**: mohidumer112@gmail.com

---

## 📝 Changelog

### Version 1.0 (Current)
- ✨ Initial release of WindowsSocketWhisper
- 🔴 Full TCP implementation with authentication
- 🔵 Complete UDP implementation with broadcasting
- 💾 Local command history storage
- 📁 File transfer capability
- 🎨 Color-coded console interface
- ✅ Multi-client support
- 📚 Comprehensive documentation

---

## 📞 Support & Contact

For issues, questions, or feedback about **WindowsSocketWhisper**:

**Email**: mohidumer112@gmail.com  
**Status**: Active Development  
**Last Updated**: January 19, 2026

---

**Thank you for using WindowsSocketWhisper!** 🎉
