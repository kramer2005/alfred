#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <alfred.h>
#include "./pokemon.h"

Pokemon **pokemons;

char *get_pokemon_json(Pokemon *pokemon)
{
    char *json = malloc(sizeof(char) * 4096);
    sprintf(json, "{\n");
    sprintf(json + strlen(json), "  \"id\": %d,\n", pokemon->id);
    sprintf(json + strlen(json), "  \"name\": %s,\n", pokemon->name);
    sprintf(json + strlen(json), "  \"types\": [");
    for (int i = 0; i < pokemon->type_count; i++)
    {
        sprintf(json + strlen(json), "    %s,\n", pokemon->types[i]);
    }
    sprintf(json + strlen(json), "  ],\n");
    sprintf(json + strlen(json), "  \"hp\": %d,\n", pokemon->hp);
    sprintf(json + strlen(json), "  \"attack\": %d,\n", pokemon->attack);
    sprintf(json + strlen(json), "  \"defense\": %d,\n", pokemon->defense);
    sprintf(json + strlen(json), "  \"spAttack\": %d,\n", pokemon->sp_attack);
    sprintf(json + strlen(json), "  \"spDefense\": %d,\n", pokemon->sp_defense);
    sprintf(json + strlen(json), "  \"speed\": %d\n", pokemon->speed);
    sprintf(json + strlen(json), "}");
    return json;
}

Pokemon **load_pokemons()
{
    FILE *fp = fopen("resources/pokedex.json", "r");
    Pokemon **pokemons = malloc(sizeof(void *) * 809);
    Pokemon *actualPokemon = pokemons[0];
    int id;
    char buffer[1024];
    fscanf(fp, "%[^\n]\n", buffer);
    while (!(buffer[0] == ']' && buffer[1] == '\0'))
    {

        if (strstr(buffer, "id") != NULL)
        {
            sscanf(buffer, "\"id\": %d", &id);
            pokemons[id - 1] = actualPokemon = malloc(sizeof(Pokemon));
            actualPokemon->id = id;
        }

        if (strstr(buffer, "english") != NULL)
        {
            sscanf(buffer, "\"english\": \"%[^\"]", actualPokemon->name);
        }

        if (strstr(buffer, "HP") != NULL)
        {
            sscanf(buffer, "\"HP\": %d", &actualPokemon->hp);
        }

        if (strstr(buffer, "Attack") != NULL)
        {
            sscanf(buffer, "\"Attack\": %d", &actualPokemon->attack);
        }

        if (strstr(buffer, "Defense") != NULL)
        {
            sscanf(buffer, "\"Defense\": %d", &actualPokemon->defense);
        }

        if (strstr(buffer, "Sp. Attack") != NULL)
        {
            sscanf(buffer, "\"Sp. Attack\": %d", &actualPokemon->sp_attack);
        }

        if (strstr(buffer, "Sp. Defense") != NULL)
        {
            sscanf(buffer, "\"Sp. Defense\": %d", &actualPokemon->sp_defense);
        }

        if (strstr(buffer, "Speed") != NULL)
        {
            sscanf(buffer, "\"Speed\": %d", &actualPokemon->speed);
        }

        if (strstr(buffer, "type") != NULL)
        {
            fscanf(fp, "%[^\n]\n", buffer);
            while (buffer[0] != ']')
            {
                sscanf(buffer, "\"%[^\"]", actualPokemon->types[actualPokemon->type_count]);
                actualPokemon->type_count++;
                fscanf(fp, "%[^\n]\n", buffer);
            }
        }

        fscanf(fp, "%[^\n]\n", buffer);
    }

    return pokemons;
}

void get_pokemon_by_id(Request *req, Response *res) {
    int id = atoi(req->get_param("id"));
    Pokemon *pokemon = pokemons[id - 1];
    char *json = get_pokemon_json(pokemon);
    res->send(json);
    free(json);
}

void search_pokemon(Request *req, Response *res) {
    char *query = req->get_param("query");
    char *json = malloc(sizeof(char) * 4096);
    sprintf(json, "[\n");
    for (int i = 0; i < 809; i++)
    {
        Pokemon *pokemon = pokemons[i];
        if (strstr(pokemon->name, query) != NULL)
        {
            sprintf(json + strlen(json), "  %s,\n", get_pokemon_json(pokemon));
        }
    }
    sprintf(json + strlen(json), "]");
    res->send(json);
    free(json);
}

void pokemon_routes(Alfred server)
{
    pokemons = load_pokemons();
    server.get("/:id", get_pokemon_by_id);
    server.get("/search/:query", search_pokemon);
}