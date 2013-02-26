CFLAGS = -Os -Wall -Wextra -Werror

RFBSL_PATH = /dev/ttyACM0
TIME = `date +"-DCT_YEAR=%Y -DCT_MONTH=1%m -DCT_DAY=1%d -DCT_HOUR=1%H -DCT_MINUTE=1%M -DCT_SECOND=1%S"`

sun.c:
	python tools/2012.py > sun.c

kronos.elf: main.c lcd.c lcd.h sun.c sun.h
	msp430-gcc -mmcu=cc430f6137 $(CFLAGS) $(TIME) -o kronos.elf main.c lcd.c sun.c

kronos.txt: kronos.elf
	python tools/memory.py -i kronos.elf -o kronos.txt

program: kronos.elf
	mspdebug rf2500 "prog kronos.elf"

rfbsl: kronos.txt
	python tools/ChronosTool.py -d $(RFBSL_PATH) rfbsl kronos.txt

clean:
	rm -f kronos.elf kronos.txt

all: kronos.txt
