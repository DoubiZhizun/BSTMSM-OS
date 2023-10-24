.PHONY : run clean build

main.exe : main.cpp
	g++ -g -std=c++14 -I${XILINX_XRT}/include -L${XILINX_XRT}/lib -o main.exe main.cpp -lxrt_coreutil -pthread -O3

xo/PippengerU250.xo : PippengerU250.v PippengerU250.tcl
	-vivado -mode batch -source PippengerU250.tcl -nojournal -nolog
	rm -rf .Xil

PippengerU250.xclbin : xo/PippengerU250.xo config.cfg
	-v++ --platform xilinx_u250_gen3x16_xdma_4_1_202210_1 --link xo/PippengerU250.xo -o PippengerU250.xclbin --log_dir ./log --optimize 3 --config config.cfg
	rm -rf log
	rm -f xcd.log
	rm -f xrc.log

clean : 
	-rm -rf main.exe
	-rm -rf xo
	-rm -rf PippengerU250.xclbin

build : main.exe PippengerU250.xclbin
