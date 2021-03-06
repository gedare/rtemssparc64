#
# Copyright (c) 2006 Martin Decky
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# - The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

include ../../../../version
include ../../../Makefile.config

## Toolchain configuration
#

ifndef CROSS_PREFIX
	CROSS_PREFIX = /usr/local
endif

BFD_NAME = elf64-sparc
BFD_ARCH = sparc
TARGET = sparc64-linux-gnu
TOOLCHAIN_DIR = $(CROSS_PREFIX)/sparc64/bin

ifeq ($(COMPILER),gcc_native)
	CC = gcc
	AS = as
	LD = ld
	OBJCOPY = objcopy
	OBJDUMP = objdump
endif

ifeq ($(COMPILER),gcc_cross)
	CC = $(TOOLCHAIN_DIR)/$(TARGET)-gcc
	AS = $(TOOLCHAIN_DIR)/$(TARGET)-as
	LD = $(TOOLCHAIN_DIR)/$(TARGET)-ld
	OBJCOPY = $(TOOLCHAIN_DIR)/$(TARGET)-objcopy
	OBJDUMP = $(TOOLCHAIN_DIR)/$(TARGET)-objdump
endif

CFLAGS = -DRELEASE=\"$(RELEASE)\" -I. -I../../../generic -I../../../genarch -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=ultrasparc -m64 -mno-fpu -pipe

ifdef REVISION
	CFLAGS += "-DREVISION=\"$(REVISION)\""
endif

ifdef TIMESTAMP
	CFLAGS += "-DTIMESTAMP=\"$(TIMESTAMP)\""
endif

SOURCES = \
	main.c \
	_components.c \
	../../../generic/printf.c \
	../../../generic/string.c \
	../../../genarch/balloc.c \
	../../../genarch/ofw.c \
	../../../genarch/ofw_tree.c \
	ofwarch.c \
	asm.S \
	boot.S

#
# All components that go to image.boot without the ramdisk.
#
COMPONENTS = \
	$(KERNELDIR)/kernel.bin \
	$(USPACEDIR)/srv/ns/ns \
	$(USPACEDIR)/app/init/init \
	$(USPACEDIR)/srv/loader/loader \
	$(USPACEDIR)/srv/devmap/devmap \
	$(USPACEDIR)/srv/rd/rd \
	$(USPACEDIR)/srv/vfs/vfs
ifeq ($(RDFMT),tmpfs)
	COMPONENTS += $(USPACEDIR)/srv/fs/tmpfs/tmpfs
endif
ifeq ($(RDFMT),fat)
	COMPONENTS += $(USPACEDIR)/srv/fs/fat/fat
endif

#
# Final list of all components that go to image.boot.
#
ALL_COMPONENTS = $(COMPONENTS)
ifeq ($(CONFIG_RD_EXTERNAL),n)
	ALL_COMPONENTS += ./initrd.img
endif

RD_SRVS = \
	$(USPACEDIR)/srv/fb/fb \
	$(USPACEDIR)/srv/kbd/kbd \
	$(USPACEDIR)/srv/console/console \
	$(USPACEDIR)/srv/fs/tmpfs/tmpfs \
	$(USPACEDIR)/srv/fs/fat/fat

RD_APPS = \
	$(USPACEDIR)/app/tetris/tetris \
	$(USPACEDIR)/app/tester/tester \
	$(USPACEDIR)/app/trace/trace \
	$(USPACEDIR)/app/bdsh/bdsh \
	$(USPACEDIR)/app/klog/klog

OBJECTS := $(addsuffix .o,$(basename $(SOURCES)))
ALL_COMPONENT_OBJECTS := $(addsuffix .o,$(basename $(notdir $(ALL_COMPONENTS))))

.PHONY: all clean depend

all: image.boot disasm

-include Makefile.depend

image.boot: depend _components.h _link.ld $(ALL_COMPONENT_OBJECTS) $(OBJECTS)
	$(LD) -Map image.map -no-check-sections -N -T _link.ld $(ALL_COMPONENT_OBJECTS) $(OBJECTS) -o $@

depend:
	-makedepend -f - -- $(DEFS) $(CFLAGS) -- $(SOURCES) > Makefile.depend 2> /dev/null

clean:
	-for file in $(RD_SRVS) ; do \
		rm -f $(USPACEDIR)/dist/srv/`basename $$file` ; \
	done
	-for file in $(RD_APPS) ; do \
		rm -f $(USPACEDIR)/dist/app/`basename $$file` ; \
	done
	-rm -f _components.h _components.c _link.ld $(ALL_COMPONENT_OBJECTS) $(OBJECTS) initrd.img image.boot image.map image.disasm Makefile.depend

_components.h _components.c _link.ld $(ALL_COMPONENT_OBJECTS): $(COMPONENTS) $(RD_SRVS) $(RD_APPS) _link.ld.in
	for file in $(RD_SRVS) ; do \
		cp $$file $(USPACEDIR)/dist/srv/ ; \
	done
	for file in $(RD_APPS) ; do \
		cp $$file $(USPACEDIR)/dist/app/ ; \
	done
ifeq ($(RDFMT),tmpfs)
	../../../../tools/mktmpfs.py $(USPACEDIR)/dist/ initrd.fs
endif
ifeq ($(RDFMT),fat)
	../../../../tools/mkfat.py $(USPACEDIR)/dist/ initrd.fs
endif
	../../../../tools/mkhord.py 16384 initrd.fs initrd.img
	rm initrd.fs
	../../../tools/pack.py $(OBJCOPY) $(BFD_NAME) $(BFD_ARCH) 1 "unsigned long" $(ALL_COMPONENTS)

%.o: %.S
	$(CC) $(DEFS) $(CFLAGS) -D__ASM__ -c $< -o $@

%.o: %.c
	$(CC) $(DEFS) $(CFLAGS) -c $< -o $@

disasm: image.boot
	$(OBJDUMP) -d image.boot > image.disasm
