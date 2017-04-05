#Credits to Luke Morrison.
CC = gcc
CXX = icpc
CFLAGS = -Wall -std=c99 -openmp -O3 -g -I -pthread.
CXXFLAGS = -Wall -openmp -O3 -g
#overkill on the flags, but that doesn't hurt anything
LDFLAGS = -lrt -lpthread 
CTARGET = a
CSOURCE = c1.c
CINCLUDES = 
#Target for the tex file
TARGET= week8

default: pdf
all: compile pdf
clean: cclean clean-pdf


#~~~~~~~~~~~~~~~~~~~~ C ~~~~~~~~~~~~~~~~~~~~#
#depends on all of you source and header files
compile: ${CSOURCE} ${CINCLUDES}
#this assumes you actually are linking all of the source files together
	${CC} ${CFLAGS} ${CSOURCE} -o ${CTARGET} 

debug: ${CSOURCE} ${CINCLUDES}
	${CC} ${CFLAGS} ${CSOURCE} -o ${CTARGET} ${LDFLAGS} -DDEBUG

cclean:
	rm -f ${CTARGET}




#~~~~~~~~~~~~~~~~~~~ Tex ~~~~~~~~~~~~~~~~~~~#
dvi: ${TARGET}.tex 
	latex ${TARGET}.tex
	latex $(TARGET).tex
bib: dvi
	bibtex $(TARGET)
latex:	bib
	latex $(TARGET).tex
	latex $(TARGET).tex
ps: latex
	dvips -R -Poutline -t letter ${TARGET}.dvi -o ${TARGET}.ps
pdf: ps
	ps2pdf ${TARGET}.ps

clean-dvi:
	rm -f ${TARGET}.aux
	rm -f ${TARGET}.log
	rm -f ${TARGET}.dvi
	rm -f ${TARGET}.out

clean-ps: clean-dvi
	rm -f ${TARGET}.ps

clean-bib:
	rm -f $(TARGET).bbl $(TARGET).blg
clean-pdf: clean-ps clean-bib
	rm -f ${TARGET}.pdf
