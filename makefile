FILENAME = final.c

main: ${FILENAME}
	msp430-gcc -mmcu=msp430g2553 ${FILENAME} -g -o final.elf -lconio -lserial -lshell

flash: final.elf
	mspdebug rf2500 'prog final.elf'

clean:
	rm -rf *.o *.elf *.out

erase:
	mspdebug rf2500 erase

debug: final.elf
	(mspdebug rf2500 "gdb" 1>/dev/null &); msp430-gdb final.elf -ex "target remote :2000"

screen:
	screen /dev/ttyACM0 9600

