#include "http_client.hpp"
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

bool HttpClient::parse_url(const std::string& url, std::string& host, std::string& path, std::string& port) {
    std::string temp = url;
    
    // Remove scheme if present
    if (temp.find("http://") == 0) {
        temp = temp.substr(7);
        port = "80";
    } else if (temp.find("https://") == 0) {
        // We do not support HTTPS since we don't use external libraries (OpenSSL)
        std::cerr << "HTTPS is not supported in this educational browser without external libraries." << std::endl;
        return false;
    } else {
        port = "80"; // Default
    }

    // Find path
    size_t path_pos = temp.find('/');
    if (path_pos != std::string::npos) {
        host = temp.substr(0, path_pos);
        path = temp.substr(path_pos);
    } else {
        host = temp;
        path = "/";
    }

    // Check for explicit port in host
    size_t port_pos = host.find(':');
    if (port_pos != std::string::npos) {
        port = host.substr(port_pos + 1);
        host = host.substr(0, port_pos);
    }

    return true;
}

HttpResponse HttpClient::get(const std::string& url) {
    HttpResponse response = {0, "", "", false};
    std::string host, path, port;

    if (!parse_url(url, host, path, port)) {
        return response;
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return response;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        std::cerr << "socket error" << std::endl;
        freeaddrinfo(res);
        return response;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "connect error" << std::endl;
        close(sockfd);
        freeaddrinfo(res);
        return response;
    }

    freeaddrinfo(res);

    // Build HTTP request
    std::stringstream request_ss;
    request_ss << "GET " << path << " HTTP/1.1\r\n";
    request_ss << "Host: " << host << "\r\n";
    request_ss << "Connection: close\r\n";
    request_ss << "User-Agent: Ovenpasta/1.0\r\n";
    request_ss << "Accept: text/html\r\n";
    request_ss << "\r\n";

    std::string request = request_ss.str();
    
    // Send request
    if (send(sockfd, request.c_str(), request.length(), 0) == -1) {
        std::cerr << "send error" << std::endl;
        close(sockfd);
        return response;
    }

    // Receive response
    std::string raw_response;
    char buffer[4096];
    ssize_t bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        raw_response += buffer;
    }

    close(sockfd);

    if (bytes_received == -1) {
        std::cerr << "recv error" << std::endl;
        return response;
    }

    // Parse response
    size_t header_end = raw_response.find("\r\n\r\n");
    if (header_end != std::string::npos) {
        response.headers = raw_response.substr(0, header_end);
        response.body = raw_response.substr(header_end + 4);
    } else {
        response.headers = raw_response;
    }

    // Extract status code
    if (response.headers.find("HTTP/") == 0) {
        size_t space1 = response.headers.find(' ');
        if (space1 != std::string::npos) {
            size_t space2 = response.headers.find(' ', space1 + 1);
            if (space2 != std::string::npos) {
                try {
                    response.status_code = std::stoi(response.headers.substr(space1 + 1, space2 - space1 - 1));
                    response.success = true;
                } catch (...) {
                    // ignore
                }
            }
        }
    }

    return response;
}
