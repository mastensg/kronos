RFBSL_PATH = /dev/ttyACM0

kronos.elf: main.c lcd.c lcd.h
	msp430-gcc -mmcu=cc430x6137 -Os -Wall -o kronos.elf main.c lcd.c

kronos.txt: kronos.elf
	python tools/memory.py -i kronos.elf -o kronos.txt

program: kronos.elf
	mspdebug rf2500 "prog kronos.elf"

rfbsl: kronos.txt
	python tools/ChronosTool.py -d $(RFBSL_PATH) rfbsl kronos.txt

clean:
	rm -f kronos.elf kronos.txt

all: kronos.txt
