# Setup server

### 1. Create socket


A socket is created using the socket() system call, which returns a file descriptor used to refer to the socket in subsequent system calls:

fd = socket(domain, type, protocol);

domaine:    The domain argument specifies a communication domain; this
            selects the protocol family which will be used for communication.
    
    - AF_UNIX : Communication performed within the kernel between applications on same host.
    Address format: pathname.

    - AF_INET : Communication performed via IPv4 between applications on hosts connected via an IPv4 network.
    Address format: 32-bit IPv4 address + 16-bit port number

    - AF_INET : Communication performed via IPv6 between applications on hosts connected via an IPv6 network.
    Address format: 128-bit IPv6 address + 16-bit port number

type: The type of socket created, either SOCK_STREAM, SOCK_DGRAM, or SOCK_RAW.

    - SOCK_STREAM: The stream socket (SOCK_STREAM) interface defines a reliable connection-oriented service. Data is sent without errors or duplication and is received in the same order as it is sent.

    - SOCK_DGRAM: The datagram socket (SOCK_DGRAM) interface defines a connectionless service for datagrams, or messages. Datagrams are sent as independent packets. The reliability is not guaranteed, data can be lost or duplicated, and datagrams can arrive out of order. However, datagram sockets have improved performance capability over stream sockets and are easier to use.

protocol: Specifies a particular protocol to be used with the socket. Specifying a protocol of 0 causes socket() to use an unspecified default protocol appropriate for the requested socket type. (In case of SOCK_STREAM the protocol will be TCP - IPPROTO_TCP)

### 2. define socket address structure

A socket address is the combination of an IP address and a port number
--> 192.168.1.10:74

We declare a sockaddr_in structure to hold the address for the socket

- sin_family = AF_INET;
AF_INET set the address family to IPv4.

- sin_addr.s_addr = INADDR_ANY;
INADDR_ANY used when we don't want to bind a socket to any specific IP. It allows our server to receive packets that have been targeted by any of the interfaces.

- sin_port = htons(8080);
Converts the port number from host byte order to network byte order.

### 3. binding socket

