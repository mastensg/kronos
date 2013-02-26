#include <inttypes.h>
#include <legacymsp430.h>
#include <cc430x613x.h>

#include "lcd.h"
#include "sun.h"

#define RFBSL ((void (*)())0x1000)

#define BUTTONS_IN              P2IN
#define BUTTONS_OUT             P2OUT
#define BUTTONS_DIR             P2DIR
#define BUTTONS_REN             P2REN
#define BUTTONS_IE              P2IE
#define BUTTONS_IES             P2IES
#define BUTTONS_IFG             P2IFG
#define BUTTONS_IRQ_VECT2       PORT2_VECTOR

#define BUTTON_STAR             (1 << 2)
#define BUTTON_NUM              (1 << 1)
#define BUTTON_UP               (1 << 4)
#define BUTTON_DOWN             (1 << 0)
#define BUTTON_BACKLIGHT        (1 << 3)
#define ALL_BUTTONS             (BUTTON_STAR | BUTTON_NUM | BUTTON_UP | BUTTON_DOWN | BUTTON_BACKLIGHT)

static uint8_t buttons = 0;

void
low_power_mode(void) {
	_BIS_SR(LPM3_bits + GIE);
	__no_operation();
}

int
main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    // LCD_FREQ = ACLK/16/8 = 256Hz
    // Frame frequency = 256Hz/4 = 64Hz, LCD mux 4, LCD on
    LCDBCTL0 = LCDDIV0 | LCDDIV1 | LCDDIV2 | LCDDIV3 | LCDPRE0 | LCDPRE1 | LCD4MUX | LCDON;

    // LCB_BLK_FREQ = ACLK/8/4096 = 1Hz
    LCDBBLKCTL = LCDBLKPRE0 | LCDBLKPRE1 | LCDBLKDIV0 | LCDBLKDIV1 | LCDBLKDIV2 | LCDBLKMOD0;

    P5SEL |= BIT5 | BIT6 | BIT7;
    P5DIR |= BIT5 | BIT6 | BIT7;

    LCDBPCTL0 = 0xFFFF;
    LCDBPCTL1 = 0x00FF;

    BUTTONS_OUT &= ~ALL_BUTTONS;
    BUTTONS_REN |= ALL_BUTTONS;
    BUTTONS_IES &= ~ALL_BUTTONS;
    BUTTONS_IFG &= ~ALL_BUTTONS;
    BUTTONS_IE |= ALL_BUTTONS;

    RTCCTL0 |= RTCTEVIE;
    RTCCTL1 &= ~(RTCHOLD >> 8);
    RTCCTL1 |= (RTCMODE >> 8);

    RTCYEAR = CT_YEAR;
    RTCMON = CT_MONTH - 100;
    RTCDAY = CT_DAY - 100;
    RTCHOUR = CT_HOUR - 100;
    RTCMIN = CT_MINUTE - 100;
    RTCSEC = CT_SECOND - 100;

    lcd_clear();

    __enable_interrupt();

    for(;;) {
        buttons = BUTTONS_IN;

        if(buttons & BUTTON_STAR) {
            uint8_t sun_h, sun_m;
            uint16_t sun_i;

            lcd[4] |= (1 << 0);

            sun_i = RTCMON * 32 + RTCDAY;

            sun_h = sun_up_down[sun_i].uh;
            sun_m = sun_up_down[sun_i].um;

            lcd_putdigit(0, sun_h / 10);
            lcd_putdigit(1, sun_h % 10);
            lcd_putdigit(2, sun_m / 10);
            lcd_putdigit(3, sun_m % 10);

            sun_h = sun_up_down[sun_i].dh;
            sun_m = sun_up_down[sun_i].dm;

            lcd_putdigit(5, sun_h / 10);
            lcd_putdigit(6, sun_h % 10);
            lcd_putdigit(7, sun_m / 10);
            lcd_putdigit(8, sun_m % 10);

        } else {
            lcd[0] &= ~(1 << 6);
            lcd[0] |= (1 << 5);
            lcd[4] &= ~(1 << 0);

            lcd_putdigit(0, RTCHOUR / 10);
            lcd_putdigit(1, RTCHOUR % 10);

            lcd_putdigit(2, RTCMIN / 10);
            lcd_putdigit(3, RTCMIN % 10);

            lcd_putdigit(5, RTCDAY / 10);
            lcd_putdigit(6, RTCDAY % 10);

            lcd_putdigit(7, RTCMON / 10);
            lcd_putdigit(8, RTCMON % 10);
        }

        if((buttons & (BUTTON_STAR | BUTTON_NUM)) == 0)
            low_power_mode();

        if(buttons & BUTTON_UP)
            ++RTCMIN;

        if(buttons & BUTTON_DOWN)
            --RTCMIN;
    }

    return 0;
}

interrupt(PORT2_VECTOR) PORT2_ISR(void) {
    buttons = BUTTONS_IN;

    if((buttons & BUTTON_STAR) && (buttons & BUTTON_NUM) && (buttons & BUTTON_BACKLIGHT))
        RFBSL();

    BUTTONS_IFG &= ~ALL_BUTTONS;

	__bic_SR_register_on_exit(LPM4_bits); 
}

interrupt(RTC_VECTOR) RTC_ISR(void) {
    RTCCTL0 &= ~RTCTEVIFG;

	__bic_SR_register_on_exit(LPM4_bits); 
}
