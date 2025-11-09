#ifndef LAYER_H
#define LAYER_H

struct server;

void layer_shell_init(struct server *server);
void layer_shell_destroy(struct server *server);

#endif
