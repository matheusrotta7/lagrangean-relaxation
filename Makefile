EXEC    = dcmstp-solver
CXX      = g++

CXXFLAGS  = -O2 -std=c++11 -Wall -Wextra -Wpedantic

SRC     = $(wildcard ./*$.cpp)
OBJ     = $(SRC:.cpp=.o)

all: $(EXEC)

${EXEC}: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	@rm -rf *.o
