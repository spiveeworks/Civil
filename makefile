all: console

CXX=g++
CXXFLAGS=-Iinclude -std=c++11

ODIR=object

_CIVOBJS = Space Event EventObjects
CIVOBJS = object/Space.o object/Event.o object/EventObjects.o
#CIVOBJS = $(_CIVOBJS:%,$(ODIR)/%.o)
CIVIL = $(patsubst %,include/civil/%.h,$(_CIVOBJS) Entity EntitySignals)

#$(CIVOBJS): $(patsubst $(ODIR)/%.o,src/%.cpp,$@)
$(ODIR)/%.o: src/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR)/Space.o: include/civil/Space.h
$(ODIR)/Event.o $(ODIR)/Space.o: include/civil/Event.h

$(ODIR)/EventObjects.o $(ODIR)/Space.o: include/civil/EventObjects.h

_CONSOBJS =
CONSOBJS = $(_CONSOBJS:%,console/%.o)
CONSOLE = 

obj: $(CIVOBJS)
console: $(CIVOBJS) $(CONSOBJS) console/main.o
	$(CXX) -o $@ $(CIVOBJS) console/main.o $(CXXFLAGS)
console/main.o: console/main.cpp $(CIVIL) console/EntityDerivs.h

.PHONY: clean all obj

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
