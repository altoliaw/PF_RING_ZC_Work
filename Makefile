# Project root path from (to project)
Prdir:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
Headers=${Prdir}/Headers
Sources=${Prdir}/Sources

# Setting of the general compiled grammer
## The Compiler, library, level of the compiler optimization, detected information, wall and message
Cmp=gcc
#Stdlib=-std=c99
Stdlib=-std=gnu99
Cmpopt=-O0
Detinfo=-g0
Wall=-Wall
Fsg=-fmessage-length=0 -pthread
Cflags=-I/usr/include
LDflags=-L/usr/lib64

## Project execution's name
PjN:= $(word $(words $(subst /, ,${Prdir})), $(subst /, ,${Prdir}))

# Make's Processes
.Suffixs: .c .h

.Phony: all
all: ${Prdir}/${PjN}
	@make run

.Phony: compile
compile: ${Prdir}/${PjN}


.Phony: clean
clean:
	@clear
	@rm -rf ${Sources}/*.o
	@rm -rf ${Sources}/*/*.o
	@rm -rf ${Prdir}/${PjN}
	@rm -rf ${Prdir}/*.o

.Phony: cmakeClean
cmakeClean:
	@clear
	@rm -rf ${Prdir}/build/*
	@rm -rf ${Prdir}/build/*/*

.Phony: run
run:
	@${Prdir}/${PjN}

##================================================================
# Create a module
${Prdir}/${PjN}: 	${Prdir}/Main.o \
					${Sources}/LibPFRingPacketLost.o \
					${Sources}/LibFileSystem.o
					
	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Fsg} ${Cflags} ${LDflags} -o ${Prdir}/${PjN} \
		${Prdir}/Main.o \
		${Sources}/LibPFRingPacketLost.o \
		${Sources}/LibFileSystem.o \
		-lc -lrt

# Main
${Prdir}/Main.o:	${Headers}/LibPFRingPacketLost.h ${Prdir}/Main.c
	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Prdir}/Main.c -c ${Fsg} ${Cflags} ${LDflags} -o ${Prdir}/Main.o

# LibPFRingPacketLost
${Sources}/LibPFRingPacketLost.o:	${Headers}/LibPFRingPacketLost.h \
								${Headers}/DebugFileInclusion.h \
								${Headers}/LibFileSystem.h \
								${Sources}/LibPFRingPacketLost.c

	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Sources}/LibPFRingPacketLost.c -c ${Fsg} ${Cflags} ${LDflags} -o ${Sources}/LibPFRingPacketLost.o

# LibFileSystem
${Sources}/LibFileSystem.o:	${Headers}/LibFileSystem.h \
								${Headers}/DebugFileInclusion.h \
								${Sources}/LibFileSystem.c

	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Sources}/LibFileSystem.c -c ${Fsg} -o ${Sources}/LibFileSystem.o
