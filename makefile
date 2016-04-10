all: console

CXX=g++
CXXFLAGS=-Iinclude -std=c++11

#       #
# Civil #
#       #

ODIR=object
HDIR=include/Civil

_CIVOBJS = Space Event EventObjects
CIVOBJS = object/Space.o object/Event.o object/EventObjects.o
#CIVOBJS = $(_CIVOBJS:%,$(ODIR)/%.o)
CIVIL = $(patsubst %,$(HDIR)/%.h,$(_CIVOBJS) Entity EntitySignals)

#$(CIVOBJS): $(patsubst $(ODIR)/%.o,src/%.cpp,$@)
$(ODIR)/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

#Watch out that Space doesn't end up with superfluous prerequisites
$(ODIR)/Space.o: $(HDIR)/Space.h $(HDIR)/EntitySignals.h
$(ODIR)/Event.o: $(HDIR)/Event.h $(HDIR)/Event.forward
$(ODIR)/EventObjects.o $(ODIR)/Space.o: $(HDIR)/EventObjects.h $(HDIR)/Event.h $(HDIR)/Event.forward $(HDIR)/EntitySignals.h

$(ODIR)/Space.o $(ODIR)/EventObjects.o: $(HDIR)/Entity.h $(HDIR)/EntitySignals.h $(HDIR)/Event.forward

#         #
# Console #
#         #

CON_ODIR=$(ODIR)/Console
CON_HDIR=include/Console

_CONSOBJS = EntityDerivs IO
CONSOBJS = $(patsubst %,$(CON_ODIR)/%.o,$(_CONSOBJS))
CONSOLE = $(patsubst %,$(CON_HDIR)/%.h,$(_CONSOBJS))

obj: $(CIVOBJS)
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


.PHONY: clean all obj

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
	rm -f $(CON_ODIR)/*.o *~ core $(INCDIR)/*~
