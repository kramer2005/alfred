#include <alfred/route.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

const char *delim = "/";

char **split_path(char *str)
{
    char **result = malloc(sizeof(char *) * strlen(str) + 1);
    char *pos = str;
    result[0] = str;
    int found = 1;

    while ((pos = strchr(pos, '/')) != NULL)
    {
        result[found++] = pos + 1;
        pos = pos + 1;
    }

    result[found++] = str + strlen(str);
    result[found] = NULL;

    for (int i = 0; result[i + 1] != NULL; i++)
    {
        char *actual_token = malloc(sizeof(char) * (result[i + 1] - result[i] + 2));
        strncpy(actual_token, result[i], result[i + 1] - result[i]);
        actual_token[result[i + 1] - result[i] + 1] = '\0';
        result[i] = actual_token;
    }
    return result;
}

void *config_route(Route *root, char **splitted, void (*callback)(Request *, Response *))
{
    char *actual_token = splitted[0];
    DEBUG_PRINT("%s ->", actual_token);

    if (root == NULL)
    {
        DEBUG_PRINT("ROOT NULL\n");
        root = malloc(sizeof(Route));
        root->path = actual_token;
        root->callback = NULL;
        root->next = NULL;
        root->children = NULL;
        if (splitted[1] == NULL)
        {
            root->callback = callback;
        }
        else
        {
            root = push_route(root, splitted, callback);
        }
        return root;
    }
    if (strcmp(actual_token, root->path) == 0)
    {
        DEBUG_PRINT("EQUAL\n");

        if (splitted[1] == NULL)
        {
            root->callback = callback;
        }
        else
        {
            root->children = push_route(root, splitted + 1, callback);
        }
        return root;
    }
    else
    {
        DEBUG_PRINT("DIFFERENT\n");

        root->next = config_route(root->next, splitted, callback);
        return root;
    }
}

void *push_route(Route *root, char **splitted, void (*callback)(Request *, Response *))
{
    char *actual_token = splitted[0];
    DEBUG_PRINT("%s -> ", actual_token);
    if (splitted[0] == NULL)
    {
        DEBUG_PRINT("[NULL]\n");
        return root;
    }
    if (root == NULL)
    {
        DEBUG_PRINT("[config route]\n");
        return config_route(root, splitted, callback);
    }
    if (strcmp(actual_token, root->path) == 0)
    {
        DEBUG_PRINT("[push route]\n");
        root->children = push_route(root->children, splitted + 1, callback);
        return root;
    }
    else
    {
        DEBUG_PRINT("[next]\n");
        root->next = push_route(root->next, splitted, callback);
        return root;
    }
}

void print_tree(Route *root)
{
    if (root == NULL)
    {
        return;
    }
    printf("%s", root->path);
    print_tree(root->children);
    print_tree(root->next);
}

int main()
{
    char *path = "/pokemon/:id";
    char **splitted = split_path(path);
    Route *root = push_route(NULL, &splitted[0], NULL);

    // path = "/pokemon/:id";
    // splitted = split_path(path);
    // root = push_route(NULL, &splitted[0], NULL);

    path = "/pokemon/search/:id";
    splitted = split_path(path);
    root = push_route(NULL, &splitted[0], NULL);

    print_tree(root);
    return 0;
}