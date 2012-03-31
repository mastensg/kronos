#define SEG_A (1 << 4)
#define SEG_B (1 << 5)
#define SEG_C (1 << 6)
#define SEG_D (1 << 7)
#define SEG_E (1 << 2)
#define SEG_F (1 << 0)
#define SEG_G (1 << 1)

extern unsigned char * const lcd;
extern const unsigned char lcd_font[];

void lcd_clear(void);
void lcd_putchar(int i, char c);
void lcd_putint(int line, int i);
