##
TMP=../../rtems/boot/isofiles
BASE=../../rtems/boot

INCLUDE1=../../HeleonOS/HelenOS/boot/arch/sparc64/loader
INCLUDE2=../../HeleonOS/HelenOS/boot/generic
INCLUDE3=../../HeleonOS/HelenOS/boot/genarch
HELENONSBOOT=../../HeleonOS/HelenOS/boot/arch/sparc64/loader

TOOLCHAINRTEMS=../../../rtems/rtems4.10/bin/sparc-rtems4.10
TOOLCHAINHELENOS=../compilers/sparc64-newlib-rtems4.10-4.4.1.gcc.full/sparc64/bin/sparc64-rtems4.10
TOOLCHAINLINUX=/home/eugen/work/simics/sparc64crosscompiler/sparc64/bin/sparc64-linux-gnu

CC32=$(TOOLCHAINRTEMS)-gcc
CC64=$(TOOLCHAINHELENOS)-gcc
CC64linux=$(TOOLCHAINLINUX)-gcc

LINKSCRIPT32=_link32.ld
LINKSCRIPT64=../../HeleonOS/HelenOS/boot/arch/sparc64/loader/_link.ld

RTEMSEXE = ../b-sun4vcvs/sparc64-rtems4.10/c/sun4v/testsuites/samples/hello/hello.exe

ELFFORMAT32=-m32
ELFFORMAT64=-m64 
#iso:
#	mkisofs -f -G boot/isofs.b -B ... -r -o image.iso .
#	cp ../b-sun4v/sparc-rtems4.10/c/leon2/testsuites/samples/hello/hello.exe image.boot
#	gzip ./image.boot
#	mv image.boot.gz $(TMP)/rtems
#	mkisofs -f -G $(TMP)/boot/isofs.b -B ... -r -o $(BASE)/image.iso $(TMP)
	
rtems32:
	
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../generic/printf.c -o printf.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../generic/string.c -o string.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/balloc.c -o balloc.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/main.c -o main.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/ofwarch.c -o ofwarch.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -D__ASM__ -c $(HELENONSBOOT)/asm.S -o asm.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -D__ASM__ -c $(HELENONSBOOT)/boot.S -o boot.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/_components.c -o _components.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/ofw.c -o ofw.o
	$(CC32) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT32) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/ofw_tree.c -o ofw_tree.o

	$(TOOLCHAINRTEMS)-ld -Map image.map -no-check-sections -N -T $(LINKSCRIPT32) main.o _components.o printf.o string.o balloc.o ofw.o ofw_tree.o ofwarch.o asm.o boot.o -o image.1.boot
	$(TOOLCHAINRTEMS)-objdump -D image.1.boot &> image.1.boot.disasm
	$(TOOLCHAINRTEMS)-readelf -a image.1.boot &> image.1.elf
	-rm image.1.boot.gz
	gzip image.1.boot
	cp image.1.boot.gz isofiles/rtems32/image.1.boot.gz -f
	mkisofs -f -G $(TMP)/boot/isofs.b -B ... -r -o $(BASE)/image.iso $(TMP)
	ls -l image.iso
	
rtems64:

	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../generic/printf.c -o printf.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../generic/string.c -o string.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/balloc.c -o balloc.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/main.c -o main.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/ofwarch.c -o ofwarch.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -D__ASM__ -c $(HELENONSBOOT)/asm.S -o asm.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -D__ASM__ -c $(HELENONSBOOT)/boot.S -o boot.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/_components.c -o _components.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/ofw.c -o ofw.o
	$(CC64) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/ofw_tree.c -o ofw_tree.o

	$(TOOLCHAINHELENOS)-ld -Map image.map -no-check-sections -N -T $(LINKSCRIPT64) main.o _components.o printf.o string.o balloc.o ofw.o ofw_tree.o ofwarch.o asm.o boot.o -o image.1.boot
	$(TOOLCHAINHELENOS)-objdump -D image.1.boot &> debug/image.1.elf64.disasm
	$(TOOLCHAINHELENOS)-readelf -a image.1.boot &> debug/image.1.elf64.elf
	-rm image.1.boot.gz
	cp image.1.boot debug/image.1.elf64.boot
	gzip image.1.boot 
	cp image.1.boot.gz isofiles/rtems64/image.1.boot.gz -f
	mkisofs -f -G $(TMP)/boot/isofs.b -B ... -r -o $(BASE)/image.iso $(TMP)
	ls -l image.iso

rtems64linux:

	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../generic/printf.c -o printf.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../generic/string.c -o string.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/balloc.c -o balloc.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/main.c -o main.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/ofwarch.c -o ofwarch.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -D__ASM__ -c $(HELENONSBOOT)/asm.S -o asm.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -D__ASM__ -c $(HELENONSBOOT)/boot.S -o boot.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/_components.c -o _components.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/ofw.c -o ofw.o
	$(CC64linux) -DCONFIG_DEBUG -DRELEASE=\"0.3.0\" -I$(INCLUDE1) -I$(INCLUDE2) -I$(INCLUDE3) -nostdinc -nostdlib -fno-builtin -Werror-implicit-function-declaration -Wmissing-prototypes -Werror -O3 -mcpu=niagara $(ELFFORMAT64) -mno-fpu -pipe "-DREVISION=\"4663\"" "-DTIMESTAMP=\"2009-10-08 11:45:21\"" -c $(HELENONSBOOT)/../../../genarch/ofw_tree.c -o ofw_tree.o

	$(TOOLCHAINLINUX)-ld -Map image.map -no-check-sections -N -T $(LINKSCRIPT64) main.o _components.o printf.o string.o balloc.o ofw.o ofw_tree.o ofwarch.o asm.o boot.o -o image.1.boot
	$(TOOLCHAINLINUX)-objdump -D image.1.boot &> debug/image.1.elf64linux.disasm
	$(TOOLCHAINLINUX)-readelf -a image.1.boot &> debug/image.1.elf64linux.elf
	-rm image.1.boot.gz
	cp image.1.boot debug/image.1.elf64linux.boot
	gzip image.1.boot
	cp image.1.boot.gz isofiles/rtems64linux/image.1.boot.gz -f
	mkisofs -f -G $(TMP)/boot/isofs.b -B ... -r -o $(BASE)/image.iso $(TMP)
	ls -l image.iso
	
rtemshelloworld:
	cp $(RTEMSEXE) image.1.boot
	$(TOOLCHAINHELENOS)-objdump -D image.1.boot &> debug/rtemshelloworld.disasm
	$(TOOLCHAINHELENOS)-readelf -a image.1.boot &> debug/rtemshelloworld.elf
	-rm image.1.boot.gz
	cp image.1.boot debug/rtemshelloworld.exe
	gzip image.1.boot 
	cp image.1.boot.gz isofiles/rtemshelloworld/image.1.boot.gz -f
	mkisofs -f -G $(TMP)/boot/isofs.b -B ... -r -o $(BASE)/image.iso $(TMP)
	ls -l image.iso
	
	
all: rtemshelloworld

clean:
	-rm *.o
	-rm $(BASE)/image.iso
	-rm isofiles/rtems/image.1.boot.gz
