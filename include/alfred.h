#ifndef __ALFRED_SERVER_H__
#define __ALFRED_SERVER_H__

typedef struct Alfred
{
    // Public mathods
    void (*get)(char *path, void (*callback)(void *, void *));     // method to register a GET route
    void (*head)(char *path, void (*callback)(void *, void *));    // method to register a HEAD route
    void (*post)(char *path, void (*callback)(void *, void *));    // method to register a POST route
    void (*put)(char *path, void (*callback)(void *, void *));     // method to register a PUT route
    void (*delete)(char *path, void (*callback)(void *, void *));  // method to register a DELETE route
    void (*connect)(char *path, void (*callback)(void *, void *)); // method to register a CONNECT route
    void (*options)(char *path, void (*callback)(void *, void *)); // method to register a OPTIONS route
    void (*trace)(char *path, void (*callback)(void *, void *));   // method to register a TRACE route
    void (*patch)(char *path, void (*callback)(void *, void *));   // method to register a PATCH route
    void (*public)(char *path, char *folder);                      // method to register a public route
    void (*use)(char *path, void (*callback)(void *, void *));     // method to register a middleware
    void (*listen)(short port);                                    // method to start the server
} Alfred;

typedef struct Request
{
    char *(*get_param)(char *name); // method to get a parameter from the request
} Request;

typedef struct Response
{
    void (*send)(char *data);                    // method to send data to the client
    void (*send_file)(char *path);               // method to send a file to the client
    void (*send_status)(int status, char *data); // method to send a status code to the client
} Response;

Alfred create_server();

#endif // __ALFRED_SERVER_H__