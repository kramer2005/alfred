#include <stdio.h>
#include <stdlib.h>
#include <linux/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <alfred/server.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif
#define BUFFER_SIZE 1024

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 16
#endif

Server *global_server;

void server_get(char *path, void (*callback)(void *, void *))
{
    Route *route = malloc(sizeof(Route));
    route->path = path;
    route->callback = callback;
    route->next = global_server->routes;
    route->type = GET;
    global_server->routes = route;
    DEBUG_PRINT("server_get(%s, %p)\n", path, callback);
}

void server_post(char *path, void (*callback)(void *, void *))
{
    Route *route = malloc(sizeof(Route));
    route->path = path;
    route->callback = callback;
    route->next = global_server->routes;
    route->type = POST;
    global_server->routes = route;
    DEBUG_PRINT("server_get(%s, %p)\n", path, callback);
}

void create_detached_thread(void *(*start_routine)(void *), void *arg)
{
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &attr, start_routine, arg);
}

void route_404(Response *res)
{
    res->send_status(404, "Not Found");
}

Request *create_request(char *buffer)
{
    Request *req = malloc(sizeof(Request));
    req->get_param = NULL;
    return req;
}

void send_status(int socket)
{
    char buffer[BUFFER_SIZE];
    sprintf(buffer, "HTTP/1.1 %d %s\r\n", global_server->status, global_server->status_message);
    send(socket, buffer, strlen(buffer), 0);
}

Response *create_response(int socket)
{
    Response *res = malloc(sizeof(Response));
    res->send = NULL;
    res->send_file = NULL;
    res->send_status = send_status;
    return res;
}

void find_route(char *path, short type, char *buffer)
{
    Route *route = global_server->routes;
    Request *req = create_request(buffer);
    while (route != NULL)
    {
        if (route->type == type && strcmp(route->path, path) == 0)
        {
            route->callback(buffer, NULL);
            return;
        }
        route = route->next;
    }
    route_404(buffer);
}

void *request(void *ns)
{
    long socket = *(long *)ns;
    char buffer[BUFFER_SIZE];
    int n;

    n = read(socket, buffer, BUFFER_SIZE);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    DEBUG_PRINT("%s\n", buffer);

    char *method = strtok(buffer, " ");
    char *path = strtok(NULL, " ");
    DEBUG_PRINT("path: %s\n", path);
    DEBUG_PRINT("method: %s\n", method);

    if (strcmp(method, "GET") == 0)
    {
        find_route(path, GET, buffer);
    }
    else if (strcmp(method, "POST") == 0)
    {
        find_route(path, POST, buffer);
    }
    else
    {
        DEBUG_PRINT("method not implemented\n");
    }

    close(socket);
    return NULL;
}

void server_listen(short port)
{
    struct sockaddr_in server;
    struct sockaddr_in client;
    unsigned int name_len;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(global_server->socket, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("bind");
        exit(1);
    }

    if (listen(global_server->socket, MAX_CONNECTIONS) < 0)
    {
        perror("listen");
        exit(1);
    }

    DEBUG_PRINT("listen(%d)\n", port);

    name_len = sizeof(client);
    while (1)
    {
        int *socket = malloc(sizeof(int));
        *socket = accept(global_server->socket, (struct sockaddr *)&client, &name_len);
        create_detached_thread(request, socket);
    }

    DEBUG_PRINT("Exited");

    close(global_server->socket);
}

Server create_server()
{
    Server *server = (Server *)malloc(sizeof(struct Server));
    server->socket = socket(AF_INET, SOCK_STREAM, 0);
    server->get = server_get;
    server->post = server_post;
    server->listen = server_listen;
    server->routes = NULL;
    global_server = server;
    return *server;
}