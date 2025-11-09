#ifndef OUTPUT_H
#define OUTPUT_H

#include "server.h"

struct output {
	struct wl_list link;
	struct server *server;
	struct wlr_output *wlr_output;
	struct wl_listener frame;
	struct wl_listener request_state;
	struct wl_listener destroy;

	struct wlr_scene_tree *layer_shell_background;
	struct wlr_scene_tree *layer_shell_bottom;
	struct wlr_scene_tree *layer_shell_top;
	struct wlr_scene_tree *layer_shell_overlay;

};

void handle_new_output(struct wl_listener *listener, void *data);

void output_frame(struct wl_listener *listener, void *data);

#endif
