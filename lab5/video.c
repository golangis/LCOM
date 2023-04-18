#include <lcom/lcf.h>

static char* video_mem;

static unsigned h_res;
static unsigned v_res;
static unsigned bits_per_pixel;
static unsigned bytes_per_pixel;
static unsigned rsvd_mask_size;

void* (vg_init)(uint16_t mode) {
    vbe_mode_info_t info;
    if (vbe_get_mode_info(mode, &info) != 0) return NULL;

    struct minix_mem_range mr;
    unsigned int vram_base = info.PhysBasePtr;
    unsigned int vram_size = info.XResolution * info.YResolution * ((info.BitsPerPixel + 7)/8);
    int r;

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if(OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED) panic("couldn't map video memory");

    h_res = info.XResolution;
    v_res = info.YResolution;
    rsvd_mask_size = info.RsvdMaskSize;
    bits_per_pixel = info.BitsPerPixel;
    bytes_per_pixel = (bits_per_pixel + 7)/8;

    reg86_t r_86;
    memset(&r_86, 0, sizeof(r_86));
    r_86.ax = 0x4F02;
    r_86.bx = (1<<14) | mode;
    r_86.intno = 0x10;
    if (sys_int86(&r_86) != OK) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }
    
    return 0;
}

uint8_t (vg_get_red)(uint32_t color) {
    color <<= 8;
    return color >> 24;
}

uint8_t (vg_get_green)(uint32_t color) {
    color <<= 16;
    return color >> 24;
}

uint8_t (vg_get_blue)(uint32_t color) {
    color <<= 24;
    return color >> 24;
}

uint8_t (vg_get_alpha)(uint32_t color) {
    return color >> 24;
}

uint16_t (vg_get_color_16)(uint32_t color) {
    uint8_t a,r,g,b;
    r = vg_get_red(color) >> 3;
    g = ((rsvd_mask_size) == 0) ? vg_get_green(color) >> 2 : vg_get_green(color) >> 3;
    b = vg_get_blue(color) >> 3;
    a = vg_get_alpha(color) >> 7;
    if((rsvd_mask_size) == 0) return (r << 11) | (g << 5) | (b);
    else return (a<<15) | (r << 10) | (g << 5) | (b);
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    uint8_t* pixel_mem = (uint8_t*)video_mem + (x * bytes_per_pixel) + (y * h_res * bytes_per_pixel);

    if (bytes_per_pixel == 2) color = vg_get_color_16(color);
    
    for (uint8_t i = 0; i < bytes_per_pixel; i++) {
        *(pixel_mem + i) = (uint8_t) color;
        color >>= 8;
    }

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (uint16_t i = 0; i < len; i++) {
        if (vg_draw_pixel(x+i, y, color) != 0) return 1;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t i = 0; i < height; i++) {
        if (vg_draw_hline(x, y+i, width, color) != 0) return 1;
    }
    return 0;
}
