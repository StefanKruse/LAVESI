#!/bin/bash

## mountain treelines

for i in {51001..51009..1} #Bash v4.0+
do
	cd /albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/output_files
	mkdir $i
	
	cd /albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects

	## get dimensions of simulation area
	y=$(grep $i alpine_treelines_LAVESI_simulation_area_extent.txt | cut -d' ' -f3)
	x=$(grep $i alpine_treelines_LAVESI_simulation_area_extent.txt | cut -d' ' -f4)
	## get region of interest (1-Siberia, 2-Canada, 3-Alaska)
	roi=$(grep $i alpine_treelines_LAVESI_simulation_area_extent.txt | cut -d' ' -f6)
	## get wind cell name
	wind_cell=$(grep $i alpine_treelines_LAVESI_simulation_area_extent.txt | cut -d' ' -f7)

	## check if wind tar file is untared: check if "cell_?" exist and if not untar it!
	cd /albedo/work/projects/p_lavesi/LAVESI_input/LAVESI_input_climate_data/wind_last25kyr_fromYearlyData
	if [ -e $wind_cell ]
	then
	    echo "wind ok"
	else
	    tar -xvf $wind_cell.tar.gz
	fi

	cd /albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/LAVESI/inc

	## declarations.h -> define extent of simulation area
	sed -i "s/treerows=.*/treerows=$y;/" declarations.h
	sed -i "s/treecols=.*/treecols=$x;/" declarations.h

	cd /albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/LAVESI
	
	## replace a string with another string
	sed -i "s/omp_num_threads=.*/omp_num_threads=128;/" parameters.txt
	sed -i "s/firemode=.*/firemode=112;/" parameters.txt
	sed -i "s/weatherchoice=.*/weatherchoice=$i;/" parameters.txt
	sed -i "s/roi=.*/roi=$roi;/" parameters.txt
	sed -i "s/elevationoffset=.*/elevationoffset=-200;/" parameters.txt
	sed -i "s/seedintro_maxy=.*/seedintro_maxy=0;/" parameters.txt
	sed -i "s/seedintro_maxx=.*/seedintro_maxx=0;/" parameters.txt

	## remove all file according to command in Makefile
	make clean
	## use multiple cpus
	make parallel -j
	## run LAVESI
	./LAVESI_WIND
	
	## archive file: tar -cvf tarFile originalFile /without compression/
	## move file: mv myfile "where"
	chmod 777 -R output
	tar -cvf output.tar.gz output
	chmod 777 -R output.tar.gz
	destOutput="/albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/output_files/$i"
	mv output.tar.gz "$destOutput"
	## copy LAVESI directory and its contents: cp -r "directory" "where"
	## copy key files(parameters.txt)
	origPar="/albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/LAVESI/parameters.txt"
	destPar="/albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/output_files/$i"
	cp "$origPar" "$destPar"
	## copy key files2(specieslist.csv)
	origSpec="/albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/LAVESI/specieslist.csv"
	destSpec="/albedo/work/projects/p_lavesi/projects/Mountains/Mountains_transects/output_files/$i"
	cp "$origSpec" "$destSpec"

done
