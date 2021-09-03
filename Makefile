
CPP := g++
CPPFLAGS := -Wall -g -std=c++11 -lz

OBJ := fastqsplit.o 

%.o: %.cpp $(DEPS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

fastqsplit: $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o fastqsplit 

