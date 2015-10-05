#ifndef PROCESS_SERVER
#define PROCESS_SERVER

#ifdef MSVC

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <iostream>
#include <string>

namespace Tools
{

using std::string;

template <class Connector>
class ProcessServer
{
public:
    ProcessServer(const string& file);
    ~ProcessServer();
    void Start();
    virtual string GetResponce(const string& request) const = 0;
private:
    Connector connector;

    string file;
    int socketIndex;
    sockaddr_un address;
};

template <class Connector>
ProcessServer<Connector>::ProcessServer(const string& file)
    : file(file)
{
    Tools::RemoveFile(file);
    // Creating socket
    socketIndex = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socketIndex < 0)
    {
        throw ProcessServerCreationException();
    }
    // Bind socket to file
    unlink(file.c_str());
    memset(&address, 0, sizeof(struct sockaddr_un)); // start with a clean addres structure
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, file.c_str());
    if (bind(socketIndex, (struct sockaddr *)&address, sizeof(struct sockaddr_un)) < 0)
    {
        throw ProcessServerCreationException();
    }
    // Listen
    if (listen(socketIndex, 5) != 0)
    {
        throw ProcessServerCreationException();
    }
}

template <class Connector>
void ProcessServer<Connector>::Start()
{
    int connectionIndex;
    socklen_t addressLength = sizeof(address);
    while(true)
    {
        connectionIndex = accept(socketIndex,
           (struct sockaddr *) &address,
           &addressLength);
        if (connectionIndex > -1) {
            string responce;
            string request;
            while (true)
            {
                try {
                    request = connector.Get(connectionIndex);
                    if (request == END_SESSION_STRING)
                    {
                        close(connectionIndex);
                        break;
                    }
                } catch (...) {
                    close(connectionIndex);
                    break;
                }
                responce = GetResponce(request);
                try {
                    connector.Send(responce, connectionIndex);
                } catch (...) {
                    close(connectionIndex);
                    break;
                }
            }
        }
    }
}

template <class Connector>
ProcessServer<Connector>::~ProcessServer()
{
    close(socketIndex);
    unlink(file.c_str());
    Tools::RemoveFile(file);
}

}

#endif //MSVC
#endif // PROCESS_SERVER
