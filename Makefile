CXX = g++
OPT ?= -O2
CXXFLAGS = -std=c++17 $(OPT)
HEADERS = Studentas.h Pagalbiniai.h Failai.h Meniu.h

programa: main.o Failai.o Pagalbiniai.o
	$(CXX) $(CXXFLAGS) main.o Failai.o Pagalbiniai.o -o programa

%.o: %.cpp $(HEADERS) flags.tmp
	$(CXX) $(CXXFLAGS) -c $< -o $@

flags.tmp: FORCE
	@echo '$(CXXFLAGS)' | cmp -s - $@ || echo '$(CXXFLAGS)' > $@

FORCE:

clean:
	rm -f *.o programa flags.tmp
