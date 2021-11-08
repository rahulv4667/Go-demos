// Source: https://github.com/toprakkeskin/Cpp-Socket-Simple-TCP-Echo-Server-Client/blob/master/server/tcp-echo-server-main.cpp

#include <iostream>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <type_traits>
#include <unistd.h>
#include <thread>
#include <vector>
#include <csignal>
// using namespace std;

std::vector<std::thread> threads;
int sock_listener;

void handle_conn(int sock_client, sockaddr_in client_addr, socklen_t client_addr_size, char* buf) {
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
    if(getnameinfo(
        (sockaddr*)&client_addr, client_addr_size,
        host, NI_MAXHOST, svc, NI_MAXSERV, 0
    ) != 0) {
        std::cout << "[INFO] Client: (" << inet_ntop(AF_INET, &client_addr.sin_addr, buf, INET_ADDRSTRLEN)
                    << ":" << ntohs(client_addr.sin_port) << ")\n";
    } else {
        std::cout << "[INFO] Client: (host: " << host << ", service: " << svc << ")\n";
    }

    char msg_buf[4096];
    int bytes;

    while(true) {
        bytes = recv(sock_client, &msg_buf, 4096, 0);

        if(bytes == 0) {
            std::cout << "[INFO] Client is disconnected.\n";
            break;
        } else if(bytes < 0) {
            std::cerr << "[ERROR] Something went wrong while receiving data!\n";
            break;
        } else {
            std::cout << "client> " << std::string(msg_buf, 0, bytes) << "\n";
            if(send(sock_client, &msg_buf, bytes, 0) < 0) {
                std::cerr << "[ERROR] Message cannot be send, exiting...\n";
                break;
            }
        }
    }

    close(sock_client);
    std::cout << "[INFO] Client socket is closed.\n";
}

void signal_handler(int signal_num) {
    std::cout<<"The interrupt signal is (" << signal_num<<").\n";
    for(auto& t: threads) {
        t.~thread();
    }

    close(sock_listener);
    std::cout << "[INFO] Main listener socket is closed.\n";

}

int main(int argc, char **argv) {
    signal(SIGABRT, signal_handler);

    if(argc != 2) {
        std::cerr << "[ERROR] Port is not provided via command line parameters!\n";
        return -1;
    }

    // int sock_listener = socket(AF_INET, SOCK_STREAM, 0);
    sock_listener = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_listener < 0) {
        std::cerr << "[ERROR] Socket cannot be created!\n";
        return -2;
    }

    std::cout<<"[INFO] Socket has been created!\n";
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::atoi(argv[1]));
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

    char buf[INET_ADDRSTRLEN];

    if(bind(sock_listener, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "[ERROR] Created socket cannot be binded to ( " 
                    << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
                    << ":" << ntohs(server_addr.sin_port) << ")\n";
        return -3;
    }

    std::cout << "[INFO] Sock is binded to ("
                << inet_ntop(AF_INET, &server_addr.sin_addr, buf, INET_ADDRSTRLEN)
                << ":" << ntohs(server_addr.sin_port) << ")\n";

    if(listen(sock_listener, SOMAXCONN) < 0) {
        std::cerr << "[ERROR] Socket cannot be switched to listen mode!\n";
        return -4;
    }
    std::cout << "[INFO] Socket is listening now!\n";

    sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int sock_client;

    while(true) {
        sock_client = accept(sock_listener, (sockaddr*)&client_addr, &client_addr_size);
        if(sock_client < 0) {
            std::cerr << "[ERROR] Connections cannot be accepted for a reason.\n";
            continue;        
        }

        std::cout << "[INFO] A connection is accepted now.\n";
        threads.push_back(std::thread(handle_conn, sock_client, client_addr, client_addr_size, buf));
    }
    // if((sock_client = accept(sock_listener, (sockaddr*)&client_addr, &client_addr_size)) < 0) {
    //     std::cerr << "[ERROR] Connections cannot be accepted for a reason.\n";
    //     return -5;
    // }

    // std::cout << "[INFO] A connection is accepted now.\n";

    // close(sock_listener);
    // std::cout << "[INFO] Main listener socket is closed.\n";

    

    return 0;
}