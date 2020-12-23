HEADERS=$(wildcard inc/*.h)
CFLAGS=-O3 -flto -fno-fat-lto-objects -std=c++11 -I inc -Wall -pedantic -Wextra -Wno-reorder -fno-math-errno -fno-rtti
CC=g++
OBJS=$(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))

.PHONY: all clean cleanoutput debug debugparallel format parallel

all: LAVESI_WIND

clean:
	@rm -rf data*.csv nohup.out output/data*.csv t_*.txt pollDist* output/windgen*.txt output/*.png output/*.mp4 output/*.gif t_*.txt build LAVESI_WIND

cleanoutput:
	@rm -rf data*.csv output/data*.csv t_*.txt

debug: CFLAGS += -O0 -g3 -ggdb3 -DDEBUG
debug: all

format:
	@clang-format -i --style=file src/* inc/*

parallel: CFLAGS += -fopenmp
parallel: all

paralleldebug: CFLAGS += -fopenmp -O0 -g3 -ggdb3 -DDEBUG
paralleldebug: all

profile: CFLAGS += -fopenmp -g
profile: all

# Files

build/%.o: src/%.cpp $(HEADERS)
	@mkdir -p build
	$(CC) -c $< -o $@ $(CFLAGS) -c

LAVESI_WIND: $(OBJS)
	$(CC) $^ -lm -o $@ $(CFLAGS)
