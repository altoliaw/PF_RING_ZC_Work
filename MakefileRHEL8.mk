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
${Prdir}/${PjN}: 	${Prdir}/Test.o
					
	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Fsg} ${Cflags} ${LDflags} -o ${Prdir}/${PjN} \
		${Prdir}/Test.o

# Main
${Prdir}/Test.o:	${Prdir}/Test.c
	${Cmp} ${Stdlib} ${Cmpopt} ${Detinfo} ${Wall} ${Prdir}/Test.c -c ${Fsg} ${Cflags} ${LDflags} -o ${Prdir}/Test.o