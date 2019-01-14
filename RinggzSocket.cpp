#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "RinggzSocket.h"

// http://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient

// int const DEFAULT_PORT = 55432;
int const DEFAULT_PORT = 55432;
int const MAX_DATA_SIZE = 4096;
std::string const DEFAULT_IP = "localhost";

namespace {

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

} // anonymous namespace

RinggzSocket::~RinggzSocket() {
    if (sockfd) {
        shutdown(*sockfd, SHUT_RDWR);
        close(*sockfd);
    }
}

bool RinggzSocket::send_string(std::string const str) {
    if (send(*sockfd, str.c_str(), str.length(), 0) == -1) {
        return false;
    }

    return true; 
}

bool RinggzSocket::send_json(json const json_data) {
    std::string str_data = json_data.dump() + "\n";
    return send_string(str_data);
}

bool RinggzSocket::is_connected() {
    return !!sockfd;
}

bool RinggzSocket::try_connect(std::string const ip, int const port) {
    int sockfd, numbytes;  
    char buf[MAX_DATA_SIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(ip.c_str(), std::to_string(port).c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        // fprintf(stderr, "client: failed to connect\n");
        return false;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    // printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    // Make non blocking?
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    
    this->sockfd = std::unique_ptr<int>(new int(sockfd));

    return true;
}

std::optional<json> RinggzSocket::recv_json() {
    std::array<uint8_t, MAX_DATA_SIZE> buf;

    // Receive all the bytes
    int receivedBytes = 0;
    if ((receivedBytes = recv(*sockfd, &buf[0], MAX_DATA_SIZE - 1, 0)) == -1) {
        // Return none on error
        return {};
    }

    // Put them all in the vector
    for (int i = 0; i < receivedBytes; ++i) {
        leftoverBytes.push_back(buf[i]);
    }

    // Check if we can find a newline
    auto it = std::find(leftoverBytes.begin(), leftoverBytes.end(), '\n');

    // If it's found
    if (it != leftoverBytes.end()) {
        // Construct json up until the newline
        std::string strData(leftoverBytes.begin(), it);
        json data = json::parse(strData);

        // Erase the used bytes + the newline!
        leftoverBytes.erase(leftoverBytes.begin(), it + 1);
        // Return the json

        // std::cout << "Received message: \n" << data.dump(4) << "\n\n";

        return data;
    }

    // No newline found, we'll keep the newlines around for later.
    return {};
}

