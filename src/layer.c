#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wayland-server-core.h>
#include <wlr/types/wlr_output.h>
#include <wlr/types/wlr_scene.h>
#include <wlr/types/wlr_layer_shell_v1.h>
#include <wlr/types/wlr_seat.h>
#include <wlr/util/box.h>
#include <wlr/util/log.h>

#include "layer.h"
#include "output.h"
#include "server.h"

struct layer_surface {
	struct server *server;
	struct wlr_layer_surface_v1 *wlr_layer_surface;
	struct wlr_scene_layer_surface_v1 *scene_surface;

	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener destroy;
	struct wl_listener commit;
	struct wl_listener new_popup;
};

static void handle_layer_surface_map(struct wl_listener *listener, void *data) {
	struct layer_surface *layer_surface =
	    wl_container_of(listener, layer_surface, map);
	struct wlr_layer_surface_v1 *wlr_layer_surface =
	    layer_surface->wlr_layer_surface;

	wlr_log(WLR_DEBUG, "layer mapped: namespace=%s keyboard_interactive=%d",
	        wlr_layer_surface->namespace,
	        wlr_layer_surface->current.keyboard_interactive);

	if (wlr_layer_surface->current.keyboard_interactive ==
	    ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_EXCLUSIVE ||
	    wlr_layer_surface->current.keyboard_interactive ==
	    ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_ON_DEMAND) {
		struct wlr_seat *seat = layer_surface->server->seat;
		if (seat) {
			wlr_seat_keyboard_notify_enter(seat,
			    wlr_layer_surface->surface, NULL, 0, NULL);
			wlr_log(WLR_DEBUG, "put keyboard to layer");
		}
	}
}

static void handle_layer_surface_unmap(struct wl_listener *listener, void *data) {
	struct layer_surface *layer_surface =
	    wl_container_of(listener, layer_surface, unmap);
	struct wlr_layer_surface_v1 *wlr_layer_surface =
	    layer_surface->wlr_layer_surface;

	wlr_log(WLR_DEBUG, "layer unmapped");

	struct wlr_seat *seat = layer_surface->server->seat;
	if (seat) {
		struct wlr_surface *focused =
		    seat->keyboard_state.focused_surface;
		if (focused == wlr_layer_surface->surface) {
			wlr_seat_keyboard_clear_focus(seat);
			wlr_log(WLR_DEBUG, "remove keyboard from layer");
		}
	}
}

static void handle_layer_surface_destroy(struct wl_listener *listener, void *data) {
	struct layer_surface *layer_surface =
	    wl_container_of(listener, layer_surface, destroy);

	wl_list_remove(&layer_surface->map.link);
	wl_list_remove(&layer_surface->unmap.link);
	wl_list_remove(&layer_surface->destroy.link);
	wl_list_remove(&layer_surface->commit.link);
	wl_list_remove(&layer_surface->new_popup.link);

	free(layer_surface);
}

static void handle_layer_surface_commit(struct wl_listener *listener, void *data) {
	struct layer_surface *layer_surface =
	    wl_container_of(listener, layer_surface, commit);
	struct wlr_layer_surface_v1 *wlr_layer_surface =
	    layer_surface->wlr_layer_surface;

	if (wlr_layer_surface->initial_commit) {
		if (wlr_layer_surface->output) {
			struct wlr_output *output = wlr_layer_surface->output;
			struct wlr_box full_area, usable_area;

			int width, height;
			wlr_output_effective_resolution(output, &width, &height);
			full_area.x = full_area.y = 0;
			full_area.width = width;
			full_area.height = height;
			usable_area = full_area;

			wlr_log(WLR_DEBUG, "init layer: output=%dx%d", width, height);

			wlr_scene_layer_surface_v1_configure(layer_surface->scene_surface,
			    &full_area, &usable_area);
		}
	}
}

static void handle_new_popup(struct wl_listener *listener, void *data) {
	struct layer_surface *layer_surface =
	    wl_container_of(listener, layer_surface, new_popup);
	wlr_log(WLR_DEBUG, "layer popup");
}

static void handle_new_layer_surface(struct wl_listener *listener, void *data) {
	struct server *server =
	    wl_container_of(listener, server, new_layer_surface);
	struct wlr_layer_surface_v1 *wlr_layer_surface = data;

	wlr_log(WLR_INFO, "new layer: namespace %s layer %d",
	        wlr_layer_surface->namespace,
	        wlr_layer_surface->pending.layer);

	struct layer_surface *layer_surface =
	    calloc(1, sizeof(struct layer_surface));
	if (!layer_surface) {
		wlr_layer_surface_v1_destroy(wlr_layer_surface);
		return;
	}

	layer_surface->wlr_layer_surface = wlr_layer_surface;
	layer_surface->server = server;

	if (!wlr_layer_surface->output) {
		struct output *output;
		wl_list_for_each(output, &server->outputs, link) {
			wlr_layer_surface->output = output->wlr_output;
			break;
		}

		if (!wlr_layer_surface->output) {
			wlr_log(WLR_ERROR, "no output");
			wlr_layer_surface_v1_destroy(wlr_layer_surface);
			free(layer_surface);
			return;
		}
	}

	struct output *output = wlr_layer_surface->output->data;
	struct wlr_scene_tree *parent = NULL;

	// put scene trees to array
	switch (wlr_layer_surface->pending.layer) {
	case ZWLR_LAYER_SHELL_V1_LAYER_BACKGROUND:
		parent = output->layer_shell_background;
		break;
	case ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM:
		parent = output->layer_shell_bottom;
		break;
	case ZWLR_LAYER_SHELL_V1_LAYER_TOP:
		parent = output->layer_shell_top;
		break;
	case ZWLR_LAYER_SHELL_V1_LAYER_OVERLAY:
		parent = output->layer_shell_overlay;
		break;
	}

	if (!parent) {
		wlr_log(WLR_ERROR, "invalid layer for layer surface");
		wlr_layer_surface_v1_destroy(wlr_layer_surface);
		free(layer_surface);
		return;
	}

	layer_surface->scene_surface = wlr_scene_layer_surface_v1_create(
	    parent, wlr_layer_surface);

	if (!layer_surface->scene_surface) {
		wlr_log(WLR_ERROR, "failed to create layer scene");
		wlr_layer_surface_v1_destroy(wlr_layer_surface);
		free(layer_surface);
		return;
	}

	layer_surface->map.notify = handle_layer_surface_map;
	wl_signal_add(&wlr_layer_surface->surface->events.map,
	              &layer_surface->map);

	layer_surface->unmap.notify = handle_layer_surface_unmap;
	wl_signal_add(&wlr_layer_surface->surface->events.unmap,
	              &layer_surface->unmap);

	layer_surface->destroy.notify = handle_layer_surface_destroy;
	wl_signal_add(&wlr_layer_surface->events.destroy,
	              &layer_surface->destroy);

	layer_surface->commit.notify = handle_layer_surface_commit;
	wl_signal_add(&wlr_layer_surface->surface->events.commit,
	              &layer_surface->commit);

	layer_surface->new_popup.notify = handle_new_popup;
	wl_signal_add(&wlr_layer_surface->events.new_popup,
	              &layer_surface->new_popup);

	wlr_layer_surface->data = layer_surface;
}

void layer_shell_init(struct server *server) {
	server->layer_shell = wlr_layer_shell_v1_create(server->wl_display, 4);
	if (!server->layer_shell) {
		wlr_log(WLR_ERROR, "failed to create layer shell");
		return;
	}

	server->new_layer_surface.notify = handle_new_layer_surface;
	wl_signal_add(&server->layer_shell->events.new_surface,
	              &server->new_layer_surface);

	wlr_log(WLR_INFO, "init layer shell");
}

void layer_shell_destroy(struct server *server) {
	if (server->layer_shell) {
		wl_list_remove(&server->new_layer_surface.link);

		server->layer_shell = NULL;
	}
}
