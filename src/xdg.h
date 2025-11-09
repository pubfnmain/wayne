#ifndef XDG_H
#define XDG_H

#include <wayland-server-core.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_xdg_shell.h>

struct toplevel {
	struct wl_list link;
	struct server *server;
	struct wlr_xdg_toplevel *xdg_toplevel;
	struct wlr_scene_tree *scene_tree;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener commit;
	struct wl_listener destroy;
	struct wl_listener request_move;
	struct wl_listener request_resize;
	struct wl_listener request_maximize;
	struct wl_listener request_fullscreen;
};

struct popup {
	struct wlr_xdg_popup *xdg_popup;
	struct wl_listener commit;
	struct wl_listener destroy;
};

struct toplevel *desktop_toplevel_at(
  struct server *server, double lx, double ly,
	struct wlr_surface **surface, double *sx, double *sy);

void focus_toplevel(struct toplevel *toplevel);

void handle_new_xdg_toplevel(struct wl_listener *listener, void *data);

void handle_new_xdg_popup(struct wl_listener *listener, void *data);

#endif
