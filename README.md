# tcp-udp-data-transmission
C programs for exploring TCP and UDP data transmission through client-server communication, focusing on protocol stack traversal and three-way handshake
Title
Assignment 3: TCP and UDP Data Transmission Exploration

Description
This repository contains programs and instructions for understanding TCP and UDP data transmission by implementing client-server communication. The focus is on the practical implementation of these protocols and observing how data is exchanged between devices.

Features
TCP Client and Server:

Demonstrates the establishment of a connection using the three-way handshake process.
Includes data transmission and acknowledgment procedures.
UDP Server:

Implements connectionless communication via UDP.
Facilitates understanding of data exchange without guaranteed delivery.
Protocol Exploration:

Examines the differences between TCP (reliable) and UDP (unreliable) protocols.
Highlights key packet behaviors in a simple, illustrative manner.

TCP Client-Server:

The server waits for a connection request.
The client initiates the connection using the three-way handshake.
Data is exchanged with acknowledgment to ensure reliable delivery.
UDP Server:

The server listens for incoming packets without establishing a connection.
The client sends messages to the server without guarantees of delivery.
Learning Objectives
Understand the differences between TCP and UDP protocols.
Learn how connection-oriented (TCP) and connectionless (UDP) communication works.
Analyze the roles of sequence numbers and acknowledgments in TCP.
Future Work
Extend the programs to support file transfers using TCP and UDP.
Implement a basic retransmission mechanism for UDP to emulate reliability.
