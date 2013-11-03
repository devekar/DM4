#!/bin/bash
# Job name:
#SBATCH --job-name=DM

# use --exclusive to get the whole nodes exclusively for this job
#SBATCH --exclusive

#SBATCH --time=04:00:00

#SBATCH -p batch

# Set up work environment
#. /opt/intel/mkl/bin/mklvars.sh intel64

# Set up your environment 
cd $HOME/DM/DM4
#
# Start
python DM4_dbscan.py $1 $2
