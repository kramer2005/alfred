#include <alfred.h>
#include <stdio.h>
#include "pokemon.h"

void index(Request *req, Response *res)
{
    res->send_file("public/index.html");
}

int main()
{
    // Create a server
    Alfred server = create_server();

    // Index Route
    server.get("/", index);

    // Pokemon Middleware
    server.use("/pokemon", pokemon_routes);

    // Public folder
    server.public("/", "public");

    // Start the server
    server.listen(8080);
}