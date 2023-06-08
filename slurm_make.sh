#!/bin/bash

#SBATCH --account=envi.envi
#SBATCH --job-name=openMP_LAVESI
#SBATCH --qos=12h
#SBATCH --time=03:00:00
#SBATCH --output=out_%x.%j
#SBATCH --mail-type=END,FAIL,TIME_LIMIT
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=64
#SBATCH --partition=smp
#SBATCH --hint=nomultithread
export OMP_PROC_BIND=true
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
export SRUN_CPUS_PER_TASK=$SLURM_CPUS_PER_TASK
date
srun make clean
srun make parallel
date
