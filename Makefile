CXX=g++
CXXFLAGS=-Wall -Wextra  -march=native -mavx
DEPS=
OBJ=main.o fps.o mandelbrot.o 
ODIR=obj
IFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

%.o: %.c $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

mandelbrot: $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(IFLAGS)  

.PHONY: clean

clean:
	rm -f ./*.o

