#include "./request.h"
#include "./response.h"

#ifndef __ALFRED_SERVER_CONTEXT_H__
#define __ALFRED_SERVER_CONTEXT_H__

typedef struct Context
{
    int id;
    Request req;
    Response res;
    struct Context *next;
} Context;

extern Context *context_list;

Context *push_context(int id);
Context *pop_context(int id);
Context *find_context(int id);

#endif // __ALFRED_SERVER_CONTEXT_H__