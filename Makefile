# General makefile for AVR Projects
#
# Kevin Cuzner

# Compilation parameters
PROJECT=kos
HEXFORMAT=ihex

BINDIR=bin
SRCDIR=src
OBJDIR=obj

LIBS=

MCU=atmega48a
INCLUDES=-Iinclude
F_CPU=9600000

# Programmer parameters
PMCU=m48
PROGRAMMER=usbasp

# Programs
AS=avr-gcc
CC=avr-gcc
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
MKDIR=mkdir
RM=rm

# Compilation setup
SRC=$(wildcard $(SRCDIR)/*.c)
ASM=$(wildcard $(SRCDIR)/*.S)
OBJ:=$(addprefix $(OBJDIR)/,$(notdir $(SRC:.c=.o)))
OBJ+=$(addprefix $(OBJDIR)/,$(notdir $(ASM:.S=.o)))

GCFLAGS=$(INCLUDES) -DF_CPU=$(F_CPU) -g -mmcu=$(MCU) -Os -fpack-struct -fshort-enums\
		-funsigned-bitfields -funsigned-char -Wall -Wstrict-prototypes -Wa,-ahlms=$(addprefix $(OBJDIR)/,$(notdir $(<:.c=.lst)))
ASFLAGS=$(INCLUDES) -DF_CPU=$(F_CPU) -mmcu=$(MCU) \
		-gstabs
LDFLAGS=-mmcu=$(MCU) -lm $(LIBS)

all: $(BINDIR)/$(PROJECT).hex

install: $(BINDIR)/$(PROJECT).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(PMCU) -e -U flash:w:$(BINDIR)/$(PROJECT).hex

clean:
	$(RM) -rf $(BINDIR)
	$(RM) -rf $(OBJDIR)

$(BINDIR)/$(PROJECT).hex: $(BINDIR)/$(PROJECT).elf
	$(OBJCOPY) -j .text -j .data -O $(HEXFORMAT) $< $@

fuse: $(BINDIR)/fuses.bin
	$(AVRDUDE) -c $(PROGRAMMER) -p $(PMCU) -U lfuse:w:0x$(shell dd if=bin/fuses.bin bs=1 count=1 | od -An -t x1 | tr -d ' '):m
	$(AVRDUDE) -c $(PROGRAMMER) -p $(PMCU) -U hfuse:w:0x$(shell dd if=bin/fuses.bin bs=1 skip=1 count=1 | od -An -t x1 | tr -d ' '):m

$(BINDIR)/fuses.bin: $(BINDIR)/$(PROJECT).elf
	$(OBJCOPY) -j .fuse -O binary $< $(BINDIR)/fuses.bin

$(BINDIR)/$(PROJECT).elf: $(OBJ)
	@$(MKDIR) -p $(dir $@)
	$(CC) $(OBJ) $(LDFLAGS) -o $@
	$(SIZE) -C --mcu=$(MCU) $(BINDIR)/$(PROJECT).elf

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(MKDIR) -p $(dir $@)
	$(CC) $(GCFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	@$(MKDIR) -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@
