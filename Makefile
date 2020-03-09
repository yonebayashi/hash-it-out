CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++17 -O0 -g
LDFLAGS=$(CXXFLAGS)
OBJ=$(SRC:.cc=.o)

all:  test_cache_lib 

test_cache_lib: test_cache_lib.o fifo_evictor.o cache_lib.o
	$(CXX) $(LDFLAGS) -o $@ $^

#fifo_evictor: fifo_evictor
#	$(CXX) $(LDFLAGS) -o $@ $^

#cache_lib: cache_lib
#	$(CXX) $(LDFLAGS) -o $@ $^


%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf *.o test_cache_lib 



