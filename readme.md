

useful commands:
- view output hex
$ hexdump -C a.out

- assemble
$ vasm6502_oldstyle -Fbin -dotdir -wdc02 ./rs232.s

- write to eprom
$ minipro -p 28C256 -w a.out
or?
$ minipro -p AT28C256 -w a.out