// main.c - Main kernel file for Hammed OS

// VGA text mode buffer address
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// Color definitions
enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
};

static int cursor_x = 0;
static int cursor_y = 0;

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void clear_screen() {
    uint16_t* vga_buffer = (uint16_t*) VGA_MEMORY;
    uint8_t color = vga_entry_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', color);
    }
    
    cursor_x = 0;
    cursor_y = 0;
}

void putchar(char c) {
    uint16_t* vga_buffer = (uint16_t*) VGA_MEMORY;
    uint8_t color = vga_entry_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = VGA_HEIGHT - 1;
        }
        return;
    }
    
    vga_buffer[cursor_y * VGA_WIDTH + cursor_x] = vga_entry(c, color);
    
    cursor_x++;
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
        if (cursor_y >= VGA_HEIGHT) {
            cursor_y = VGA_HEIGHT - 1;
        }
    }
}

void print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}

void kernel_main() {
    clear_screen();
    
    print("=== Hammed OS Kernel v1.0 ===\n");
    print("Kernel started successfully!\n\n");
    
    print("System Information:\n");
    print("- Architecture: x86-64\n");
    print("- Bootloader: Custom BIOS\n");
    print("- Status: Running\n\n");
    
    print("Welcome to Hammed OS!\n");
    
    // Infinite loop
    while (1) {
        __asm__ volatile ("hlt");
    }
}