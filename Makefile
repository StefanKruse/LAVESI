HEADERS=$(wildcard inc/*.h)
CFLAGS=-O3 -flto -fno-fat-lto-objects -std=c++0x -I inc
CC=g++
OBJS=$(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))

.PHONY: all clean cleanoutput debug debugparallel parallel

# Phony targets

all: LAVESI_WIND

clean:
	rm -rf data*.csv nohup.out output/data*.csv t_*.txt pollDist* output/windgen*.txt t_*.txt build LAVESI_WIND

cleanoutput:
	rm -rf data*.csv output/data*.csv t_*.txt

debug: CFLAGS += -O0 -g3 -ggdb3 -DDEBUG
debug: all

parallel: CFLAGS += -fopenmp
parallel: all

paralleldebug: CFLAGS += -fopenmp -O0 -g3 -ggdb3 -DDEBUG
paralleldebug: all

profile: CFLAGS += -fopenmp -g -DDEBUG
profile: all

# Files

build/%.o: src/%.cpp $(HEADERS)
	@mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS)

LAVESI_WIND: $(OBJS)
	$(CC) $^ -lm -z muldefs -o $@ $(CFLAGS)
