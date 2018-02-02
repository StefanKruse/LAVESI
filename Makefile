HEADER=./inc/libraries.h ./inc/declarations.h ./inc/structures.h ./inc/functions.h
CFLAGS= -c -O0 -g3  -ggdb3 -fno-omit-frame-pointer -std=c++0x -include ./inc/libraries.h -include ./inc/structures.h -include ./inc/declarations.h -include ./inc/functions.h
CC=g++ -g #$(CXX)
OBJS = $(./%.o)

parallel: CC += -fopenmp
parallel: all

main.o: src/main.cpp $(HEADER)
	$(CC) -c src/main.cpp $(CFLAGS)

ageing.o: src/ageing.cpp $(HEADER)
	$(CC) -c src/ageing.cpp $(CFLAGS)
	
dataoutput.o: src/dataoutput.cpp $(HEADER)
	$(CC) -c src/dataoutput.cpp  $(CFLAGS)
	
distribution.o: src/distribution.cpp $(HEADER)
	$(CC) -c src/distribution.cpp  $(CFLAGS)
	
establishment.o: src/establishment.cpp $(HEADER)
	$(CC) -c src/establishment.cpp $(CFLAGS)
	
growth.o: src/growth.cpp $(HEADER)
	$(CC) -c src/growth.cpp $(CFLAGS)
	
mortality.o: src/mortality.cpp $(HEADER)
	$(CC) -c src/mortality.cpp $(CFLAGS)
	
parameterintro.o: src/parameterintro.cpp $(HEADER)
	$(CC) -c src/parameterintro.cpp $(CFLAGS)
	
plotupdate.o: src/plotupdate.cpp $(HEADER)
	$(CC) -c src/plotupdate.cpp $(CFLAGS)
	
reset.o: src/reset.cpp $(HEADER)
	$(CC) -c src/reset.cpp $(CFLAGS)
	
seeddispersal.o: src/seeddispersal.cpp $(HEADER)
	$(CC) -c src/seeddispersal.cpp $(CFLAGS)

seedproduction.o: src/seedproduction.cpp $(HEADER)
	$(CC) -c src/seedproduction.cpp $(CFLAGS)
	
treedistribution.o: src/treedistribution.cpp $(HEADER)
	$(CC) -c src/treedistribution.cpp $(CFLAGS)
	
weatherinput.o: src/weatherinput.cpp $(HEADER)
	$(CC) -c src/weatherinput.cpp $(CFLAGS)
	
.PHONY : all
	
all: ageing.o dataoutput.o establishment.o distribution.o growth.o main.o mortality.o parameterintro.o plotupdate.o reset.o seeddispersal.o seedproduction.o treedistribution.o weatherinput.o executables
	
executables: $(OBJS)
	$(CC) ./ageing.o ./dataoutput.o ./establishment.o ./distribution.o ./growth.o ./main.o ./mortality.o ./parameterintro.o ./plotupdate.o ./reset.o ./seeddispersal.o ./seedproduction.o ./treedistribution.o ./weatherinput.o  -lm -z muldefs -o LAVESI_WIND
	rm -rf *.o
	
.PHONY : clean
clean: 
	rm -rf data*.csv nohup.out output/data*.csv pollDist* output/windgen*.txt t_*.txt *o LAVESI_WIND

cleanoutput:
	rm -rf data*.csv output/data*.csv t_*.txt

