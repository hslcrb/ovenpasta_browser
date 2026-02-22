#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <string>

struct HttpResponse {
    int status_code;
    std::string headers;
    std::string body;
    bool success;
};

class HttpClient {
public:
    static HttpResponse get(const std::string& url);

private:
    static bool parse_url(const std::string& url, std::string& host, std::string& path, std::string& port);
};

#endif // HTTP_CLIENT_HPP
