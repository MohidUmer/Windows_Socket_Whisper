# WindowsSocketWhisper – TCP & UDP Chat System

---

## 1. Project Overview

### 1.1 Why
WindowsSocketWhisper demonstrates **Windows-specific socket programming**, showcasing both **TCP (reliable)** and **UDP (fast)** protocols.  
Ideal for **learning network programming, protocol differences, and real-time messaging systems**.

### 1.2 What
A production-grade, **terminal-based chat system** in **C++** with dual protocol support:

- TCP: Reliable, connection-oriented communication  
- UDP: Fast, connectionless broadcast messaging  
- Multi-client handling, authentication, file transfer, and command history  

### 1.3 How (High-Level)
- TCP: Three-way handshake, ordered delivery, guaranteed messages  
- UDP: Connectionless, low-latency messaging, broadcast support  
- Winsock2 API for Windows socket programming  
- Multi-threaded server and concurrent client handling  
- Local storage for credentials and command history  

---

## 2. Features

- 🔐 **Secure Authentication** – Username/password validation  
- 💬 **Dual Protocol Support** – Compare TCP reliability vs UDP speed  
- 🎨 **Rich Console UI** – Color-coded messages and organized display  
- 📁 **File Transfer Capability** – Send and receive files  
- 👥 **Multi-Client Support** – Handle concurrent users  
- ⏱️ **Command History** – Track and retrieve previous commands  
- 🔔 **Real-time Notifications** – Join/leave alerts and status updates  
- 🌐 **Broadcast Messaging** – UDP-only feature  
- 📊 **Network Educational Value** – Learn protocol behaviors  

---

## 3. Tech Stack

- **Language**: C++11+  
- **Platform**: Windows XP or newer  
- **Networking**: Winsock2 (ws2_32.lib)  
- **Compiler**: GCC/MinGW recommended  
- **Concurrency**: Threads for TCP server handling  
- **Storage**: Local `userdata.txt` for credentials  

---

## 4. Project Structure

```

WindowsSocketWhisper/
│── TCP_Version/
│   ├── server.cpp
│   ├── client.cpp
│   └── userdata.txt
│── UDP_Version/
│   ├── server.cpp
│   ├── client.cpp
│   └── userdata.txt
│── README.md
│── .gitignore

````

---

## 5. Setup & Usage

### 5.1 Prerequisites
- MinGW or GCC with Winsock2 support  
- Windows XP or newer  
- Local network or loopback testing  

### 5.2 Compilation

**TCP Version**
```bash
cd TCP_Version
g++ -std=c++11 -o server.exe server.cpp -lws2_32 -Wall
g++ -std=c++11 -o client.exe client.cpp -lws2_32 -Wall
````

**UDP Version**

```bash
cd UDP_Version
g++ -std=c++11 -o server.exe server.cpp -lws2_32 -Wall
g++ -std=c++11 -o client.exe client.cpp -lws2_32 -Wall
```

---

## 6. Major Components

### 6.1 TCP Server

* Connection-oriented, reliable message delivery
* Handles authentication, multiple clients, file transfer
* Thread-safe using CRITICAL_SECTION
* Ports: 8080 (configurable)

### 6.2 TCP Client

* Connects to TCP server
* Handles authentication, messaging, file transfer
* Maintains command history locally

### 6.3 UDP Server

* Connectionless, fast broadcast messaging
* Tracks connected client addresses
* Ports: 8081 (configurable)

### 6.4 UDP Client

* Sends/receives datagrams asynchronously
* Updates user status and command history

---

## 7. Security & Authentication

* Username/password stored in `userdata.txt`
* Credentials format: `username password` (space-separated, case-sensitive)
* TCP: Authentication before messaging
* UDP: Optional status updates with authentication check

---

## 8. Logging & Monitoring

* Command history saved per user locally
* Server console displays connection status, notifications
* UDP: Broadcast join/leave alerts
* TCP: Reliable message logging (in-memory or optional file output)

---

## 9. Workflow / Execution

### 9.1 Server Start

* TCP/UDP server binds to configured port
* Listens (TCP) or receives datagrams (UDP)

### 9.2 Client Connect

* TCP: Three-way handshake, authentication
* UDP: Direct datagram send/receive, authentication optional

### 9.3 Messaging Loop

* TCP: Ordered, acknowledged messages
* UDP: Low-latency, potential packet loss

### 9.4 Session Termination

* Client command (`/exit`) or server shutdown
* TCP: Graceful connection close
* UDP: Stateless termination

---

## 10. Learning Outcomes

* Windows socket programming with TCP and UDP
* Understanding protocol trade-offs: reliability vs speed
* Multi-threaded server design
* Real-time messaging and broadcast systems
* File transfer over TCP/UDP

---

## 11. Limitations

* Terminal-only interface
* Max 10 concurrent clients (configurable)
* No encryption (plaintext credentials/messages)
* UDP messages not guaranteed delivery

---

## 12. Future Improvements

* Add SSL/TLS encryption
* Replace `userdata.txt` with SQLite/MySQL
* Group chat / channels
* Multi-file transfer with progress
* Audio streaming / voice chat
* Web or mobile client interface
* Persistent message history
* Admin management dashboard

---

## 13. Ethical & Legal Notice ⚠️

* Educational use only
* Controlled testing environment
* Unauthorized network use or malicious activity prohibited

---

## 14. Author & Contact

**Name**: Mohid Umer  
**Email**: [mohidumer112@gmail.com](mailto:mohidumer112@gmail.com)

---

## 15. License

* Provided for **educational purposes**
* See repository license file for details

---

> WindowsSocketWhisper is a learning project for Windows network programming and TCP/UDP protocol experimentation. Feedback and collaboration are welcome.

```
