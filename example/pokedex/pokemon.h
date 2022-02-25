#ifndef __POKEMON_H__
#define __POKEMON_H__

typedef struct Pokemon
{
    int id;
    char name[20];
    char types[2][20];
    int type_count;
    int hp;
    int attack;
    int defense;
    int sp_attack;
    int sp_defense;
    int speed;
} Pokemon;

Pokemon **load_pokemons();
char *get_pokemon_json(Pokemon *pokemon);
void pokemon_routes(Alfred server);

#endif // __POKEMON_H__