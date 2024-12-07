#define _GNU_SOURCE

#include "gooey_backend.h"

#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"
#include <sys/mman.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int WIDTH = 800;
const int HEIGHT = 600;

typedef struct
{
    struct wl_surface *surface;
    struct wl_shm *shm;
    struct wl_buffer *buffer;
    struct xdg_wm_base *wm_base;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_display *display;
    struct wl_compositor *compositor;
} GooeyBackendContext;

static GooeyBackendContext ctx = {0};

int create_shm_file(size_t size)
{
    int fd = memfd_create("wayland-shm", FD_CLOEXEC | MFD_ALLOW_SEALING);
    if (fd < 0)
    {
        perror("Failed to create SHM file");
        exit(1);
    }
    if (ftruncate(fd, size) < 0)
    {
        perror("Failed to set SHM file size");
        close(fd);
        exit(1);
    }
    return fd;
}

struct wl_buffer *create_buffer(int width, int height, struct wl_shm *shm)
{
    int stride = width * 4;
    int size = stride * height;

    int fd = create_shm_file(size);

    void *data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED)
    {
        perror("Failed to mmap SHM");
        close(fd);
        exit(1);
    }

    uint32_t *pixels = (uint32_t *)data;
    for (int i = 0; i < width * height; i++)
    {
        pixels[i] = 0xFFFFFF;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0, width, height, stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    return buffer;
}

static void handle_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}
static const struct xdg_wm_base_listener wm_base_listener = {
    .ping = handle_wm_base_ping,
};

void registry_handler(void *data, struct wl_registry *registry, uint32_t id,
                      const char *interface, uint32_t version)
{
    if (strcmp(interface, "wl_compositor") == 0)
    {
        ctx.compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    }
    else if (strcmp(interface, "wl_shm") == 0)
    {
        ctx.shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    }
    else if (strcmp(interface, "xdg_wm_base") == 0)
    {
        ctx.wm_base = wl_registry_bind(registry, id, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(ctx.wm_base, &wm_base_listener, NULL);
    }
}

void registry_remove_handler(void *data, struct wl_registry *registry, uint32_t id)
{
}

const struct wl_registry_listener registry_listener = {
    .global = registry_handler,
    .global_remove = registry_remove_handler,
};

int wayland_init()
{
    ctx.display = wl_display_connect(NULL);
    if (!ctx.display)
    {
        fprintf(stderr, "Failed to connect to Wayland display\n");
        return -1;
    }

    struct wl_registry *registry = wl_display_get_registry(ctx.display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    
    if (wl_display_roundtrip(ctx.display) < 0)
    {
        fprintf(stderr, "Failed to initialize Wayland interfaces\n");
        return -1;
    }

    return 0; 
}

GooeyWindow wayland_create_window()
{
    ctx.surface = wl_compositor_create_surface(ctx.compositor);
    if (!ctx.surface)
    {
        fprintf(stderr, "Failed to create Wayland surface\n");
        exit(1);
    }

    ctx.xdg_surface = xdg_wm_base_get_xdg_surface(ctx.wm_base, ctx.surface);
    if (!ctx.xdg_surface)
    {
        fprintf(stderr, "Failed to create XDG surface\n");
        exit(1);
    }
    ctx.xdg_toplevel = xdg_surface_get_toplevel(ctx.xdg_surface);
    if (!ctx.xdg_toplevel)
    {
        fprintf(stderr, "Failed to create XDG toplevel\n");
        exit(1);
    }
    xdg_toplevel_set_title(ctx.xdg_toplevel, "Hello Wayland");
    xdg_toplevel_set_app_id(ctx.xdg_toplevel, "wayland-test-app");
    wl_surface_commit(ctx.surface);
    ctx.buffer = create_buffer(WIDTH, HEIGHT, ctx.shm);
    if (!ctx.buffer)
    {
        fprintf(stderr, "Failed to create buffer\n");
        exit(1);
    }
    wl_surface_attach(ctx.surface, ctx.buffer, 0, 0);
    wl_surface_commit(ctx.surface);

    GooeyWindow window = (GooeyWindow){.height = HEIGHT, .width = WIDTH};
    return window;
}

void wayland_run()
{
    while (1)
    {
        int ret = wl_display_dispatch(ctx.display);
        if (ret == -1)
        {
            fprintf(stderr, "Wayland connection closed\n");
            break;
        }
    }
}
void wayland_cleanup()
{
    wl_buffer_destroy(ctx.buffer);
    wl_surface_destroy(ctx.surface);
    wl_display_disconnect(ctx.display);
}

GooeyBackend wayland_backend = {
    .Init = wayland_init,
    .CreateWindow = wayland_create_window,
    .Cleanup = wayland_cleanup,
    .Render = wayland_run};
