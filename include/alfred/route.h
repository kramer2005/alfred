/**
 * @file route.h
 * @author your name (you@domain.com)
 * @brief Implements a route based on a directory tree.
 * @version 0.1
 * @date 2022-02-12
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "request.h"
#include "response.h"

#ifndef __ALFRED_ROUTER_ROUTE_H__
#define __ALFRED_ROUTER_ROUTE_H__

typedef struct Route
{
    char *path;
    void (*callback)(Request *, Response *);
    struct Route *next;
    struct Route *children;
} Route;

void *push_route(Route *parent, char **splitted, void (*callback)(Request *, Response *));
Route *find_route(Route *root, char *path);
Route *find_child(Route *parent, char *path);

#endif // __ALFRED_ROUTER_ROUTE_H__