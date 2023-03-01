HEADERS=$(wildcard inc/*.h)
FLAGS=-O3 -flto -fno-fat-lto-objects -std=c++11 -I inc -Wall -pedantic -Wextra -Wno-reorder
OBJS=$(patsubst src/%.cpp,build/%.o,$(wildcard src/*.cpp))

.PHONY: all clean cleanoutput debug debugparallel format parallel

all: LAVESI_WIND

clean:
	@rm -rf data*.csv nohup.out output/data*.csv t_*.txt pollDist* output/windgen*.txt output/*.png output/*.mp4 output/firegap/*_firegap.csv output/firegap/firegap_*.txt output/*.gif t_*.txt build LAVESI_WIND

cleanoutput:
	@rm -rf data*.csv output/data*.csv t_*.txt

debug: FLAGS += -O0 -g3 -ggdb3 -DDEBUG
debug: all

format:
	@clang-format -i --style=file src/* inc/*

parallel: FLAGS += -fopenmp
parallel: all

paralleldebug: FLAGS += -fopenmp -O0 -g3 -ggdb3 -DDEBUG
paralleldebug: all

profile: FLAGS += -fopenmp -g
profile: all

# Files

build/%.o: src/%.cpp $(HEADERS)
	@mkdir -p build
	$(CXX) -c $< -o $@ $(FLAGS) $(CXXFLAGS) -c

LAVESI_WIND: $(OBJS)
	$(CXX) $^ -lm -o $@ $(FLAGS) $(CXXFLAGS)
