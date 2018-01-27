#!/bin/sh

# 
# Vivado(TM)
# runme.sh: a Vivado-generated Runs Script for UNIX
# Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
# 

if [ -z "$PATH" ]; then
  PATH=/home/opt/Xilinx/SDK/2017.2/bin:/home/opt/Xilinx/Vivado/2017.2/ids_lite/ISE/bin/lin64:/home/opt/Xilinx/Vivado/2017.2/bin
else
  PATH=/home/opt/Xilinx/SDK/2017.2/bin:/home/opt/Xilinx/Vivado/2017.2/ids_lite/ISE/bin/lin64:/home/opt/Xilinx/Vivado/2017.2/bin:$PATH
fi
export PATH

if [ -z "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH=/home/opt/Xilinx/Vivado/2017.2/ids_lite/ISE/lib/lin64
else
  LD_LIBRARY_PATH=/home/opt/Xilinx/Vivado/2017.2/ids_lite/ISE/lib/lin64:$LD_LIBRARY_PATH
fi
export LD_LIBRARY_PATH

HD_PWD='/home/davidmc/prep_new_ddr_cache_v2/prep_new_ddr_cache_v2.runs/system_rst_mig_7series_0_100M_0_synth_1'
cd "$HD_PWD"

HD_LOG=runme.log
/bin/touch $HD_LOG

ISEStep="./ISEWrap.sh"
EAStep()
{
     $ISEStep $HD_LOG "$@" >> $HD_LOG 2>&1
     if [ $? -ne 0 ]
     then
         exit
     fi
}

EAStep vivado -log system_rst_mig_7series_0_100M_0.vds -m64 -product Vivado -mode batch -messageDb vivado.pb -notrace -source system_rst_mig_7series_0_100M_0.tcl
