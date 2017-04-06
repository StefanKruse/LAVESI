HEADER=./inc/libraries.h ./inc/declarations.h ./inc/structures.h ./inc/funktionen.h
CFLAGS= -c -O0 -g3 -ggdb3 -std=c++0x -Wno-parentheses -Wall -include ./inc/libraries.h -include ./inc/structures.h -include ./inc/declarations.h -include ./inc/funktionen.h
CC=$(CXX)
OBJS = $(./%.o)

ageing.o: src/ageing.cpp $(HEADER)
	$(CC) -c src/ageing.cpp $(CFLAGS)
	
data_output.o: src/data_output.cpp $(HEADER)
	$(CC) -c src/data_output.cpp  $(CFLAGS)
	
distribution.o: src/distribution.cpp $(HEADER)
	$(CC) -c src/distribution.cpp  $(CFLAGS)
	
establishment.o: src/establishment.cpp $(HEADER)
	$(CC) -c src/establishment.cpp $(CFLAGS)
	
fire.o: src/fire.cpp $(HEADER)
	$(CC) -c src/fire.cpp $(CFLAGS)
	
growth.o: src/growth.cpp $(HEADER)
	$(CC) -c src/growth.cpp $(CFLAGS)
	
influx.o: src/influx.cpp $(HEADER)
	$(CC) -c src/influx.cpp $(CFLAGS)
	
main.o: src/main.cpp $(HEADER)
	$(CC) -c src/main.cpp $(CFLAGS)
	
mortality.o: src/mortality.cpp $(HEADER)
	$(CC) -c src/mortality.cpp $(CFLAGS)
	
normdistributedrn.o: src/normdistributedrn.cpp $(HEADER)
	$(CC) -c src/normdistributedrn.cpp $(CFLAGS)
	
parameterintro.o: src/parameterintro.cpp $(HEADER)
	$(CC) -c src/parameterintro.cpp $(CFLAGS)
	
plot_update.o: src/plot_update.cpp $(HEADER)
	$(CC) -c src/plot_update.cpp $(CFLAGS)
	
reset.o: src/reset.cpp $(HEADER)
	$(CC) -c src/reset.cpp $(CFLAGS)
	
SA_parametervariation.o: src/SA_parametervariation.cpp $(HEADER)
	$(CC) -c src/SA_parametervariation.cpp $(CFLAGS)
	
seed_dispersal.o: src/seed_dispersal.cpp $(HEADER)
	$(CC)  -c src/seed_dispersal.cpp $(CFLAGS)

seed_production.o: src/seed_production.cpp $(HEADER)
	$(CC)  -c src/seed_production.cpp $(CFLAGS)
	
treedistribution.o: src/treedistribution.cpp $(HEADER)
	$(CC) -c src/treedistribution.cpp $(CFLAGS)
	
weather_input.o: src/weather_input.cpp $(HEADER)
	$(CC) -c src/weather_input.cpp $(CFLAGS)
	
#weathereingabe_matrix.o: src/weathereingabe_matrix.cpp $(HEADER)
	#$(CC) -c src/weathereingabe_matrix.cpp $(CFLAGS)
	
#weathereingabe_tempexp.o: src/weathereingabe_tempexp.cpp $(HEADER)
#	$(CC) -c src/weathereingabe_tempexp.cpp $(CFLAGS)
	
.PHONY : all
	
all: ageing.o data_output.o establishment.o distribution.o fire.o growth.o influx.o main.o mortality.o normdistributedrn.o parameterintro.o plot_update.o reset.o SA_parametervariation.o seed_dispersal.o seed_production.o treedistribution.o weather_input.o executables
	
executables: $(OBJS)
	$(CC) -o growth ./*.o -lm -z muldefs
	rm -rf *.o
	
.PHONY : clean
clean: 
	rm -rf data*.csv output/data*.csv *o growth

