# FPGA-Accelerated CNN on Zynq-7020

## Overview
This project implements a CNN Conv2D accelerator using Vitis HLS and deploys it as RTL for FPGA acceleration.

## Tools Used
• Vitis HLS 2023.2  
• Vivado  
• Zynq-7020 FPGA  

## Files
• conv2d_accel.cpp – HLS source  
• bench.cpp – Testbench  
• conv2d_accel.v – RTL hardware  
• reports/ – synthesis reports  

## Results
• Clock Frequency: 114 MHz  
• Successful synthesis and cosimulation  
• Hardware RTL generated
