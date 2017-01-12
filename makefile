all: console

CXX=g++
CXXFLAGS=-Iinclude -std=c++11

#       #
# Civil #
#       #

ODIR=object
HDIR=include/Civil

## _CIVOBJS lists names without extensions of any files that
##      A. appear with a .cpp file extension in src
##  AND B. appear with a .h file extension in HDIR
_CIVOBJS = Space Event EventObjects tax

## _CIVIL lists names without extensions of some files that satisfy B without A
_CIVIL = Entity EntitySignals

## CIVOBJS is a generated list of obj files that make up the internal library of civil
CIVOBJS = $(_CIVOBJS:%,$(ODIR)/%.o)

## CIVIL acts as a list of all of the important headers to be used as prerequisites for any full implementation (e.g. main.o)
CIVIL = $(patsubst %,$(HDIR)/%.h,$(_CIVOBJS) $(_CIVIL))

## any .o object required will be compiled using a similarly named .cpp source file
$(ODIR)/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)


## Header oriented recipes:
##  For each header, X.h say, write a line of the form
##   <obj that directly includes X.h> [etc.]: <X.h> [header included in X.h [headers included in this header [etc.]]] [etc.]
##  note that some headers only ever appear in other headers and will not require a line at all


#Watch out that Space doesn't end up with superfluous prerequisites 
$(ODIR)/Space.o: $(HDIR)/Space.h $(HDIR)/EntitySignals.h
$(ODIR)/Event.o: $(HDIR)/Event.h $(HDIR)/Event.forward
$(ODIR)/EventObjects.o $(ODIR)/Space.o: $(HDIR)/EventObjects.h $(HDIR)/Event.h $(HDIR)/Event.forward $(HDIR)/EntitySignals.h

$(ODIR)/Space.o $(ODIR)/EventObjects.o: $(HDIR)/Entity.h $(HDIR)/EntitySignals.h $(HDIR)/Event.forward

$(ODIR)/tax.o: $(HDIR)/tax.h

#         #
# Console #
#         #

## most of this is an analogue to Civil
CON_ODIR=$(ODIR)/Console
CON_HDIR=include/Console

_CONSOBJS = EntityDerivs IO
CONSOBJS = $(_CONSOBJS:%,$(CON_ODIR)/%.o)
CONSOLE = $(patsubst %,$(CON_HDIR)/%.h,$(_CONSOBJS))

## compile civil, console, and main.o, then link them to make console :D
console: $(CIVOBJS) $(CONSOBJS) $(CON_ODIR)/main.o
	$(CXX) -o $@ $(CIVOBJS) $(CONSOBJS) $(CON_ODIR)/main.o $(CXXFLAGS)

$(CON_ODIR)/%.o: src/console/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

#Does main always need all of the console headers?
$(CON_ODIR)/main.o: $(CIVIL) $(CONSOLE)
$(CON_ODIR)/EntityDerivs.o: $(CON_HDIR)/EntityDerivs.h $(HDIR)/Entity.h $(HDIR)/EntitySignals.h $(HDIR)/Event.forward
$(CON_ODIR)/IO.o $(CON_ODIR)/EntityDerivs.o: $(CON_HDIR)/IO.h $(HDIR)/EntitySignals.h $(HDIR)/Event.forward

$(CON_ODIR)/IO.o: $(HDIR)/Entity.h
$(CON_ODIR)/IO.o: $(HDIR)/Space.h
$(CON_ODIR)/IO.o: $(HDIR)/EventObjects.h

#         #
# Testing #
#         #

test: $(ODIR)/test.o $(ODIR)/tax.o
	$(CXX) -o $@ $(ODIR)/test.o $(ODIR)/tax.o $(CXXFLAGS)
$(ODIR)/test.o: src/test.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR)/test.o: $(HDIR)/tax.h

#            #
# make stuff #
#            #

.PHONY: clean all

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(CON_ODIR)/*.o *~ core $(INCDIR)/*~
