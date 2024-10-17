BASE_DIR := $(shell pwd)

APPLICATION_DIRS := _axpy _blackscholes _canneal _jacobi-2d _lavaMD _matmul _swaptions _streamcluster _somier _particlefilter _pathfinder 

all: axpy blackscholes canneal jacobi-2d lavaMD matmul swaptions streamcluster somier particlefilter pathfinder

axpy:
	cd _axpy; 			\
	make start;			\
	make vector; 		\
	make serial;

blackscholes:
	cd _blackscholes; 	\
	make start;			\
	make vector; 		\
	make serial;

canneal:
	cd _canneal; 		\
	make start;			\
	make vector; 		\
	make serial;

jacobi-2d:
	cd _jacobi-2d;		\
	make start;			\
	make vector; 		\
	make serial;

lavaMD:
	cd _lavaMD;		    \
	make start;			\
	make vector; 		\
	make serial;

matmul:
	cd _matmul;		    \
	make start;			\
	make vector; 		\
	make serial;

particlefilter:
	cd _particlefilter;	\
	make start;			\
	make vector; 		\
	make serial;

pathfinder:
	cd _pathfinder;		\
	make start;			\
	make vector; 		\
	make serial;

swaptions:
	cd _swaptions; 		\
	make start;			\
	make vector; 		\
	make serial;

somier:
	cd _somier; 		\
	make start;			\
	make vector; 		\
	make serial;

streamcluster:
	cd _streamcluster; 	\
	make start;			\
	make vector; 		\
	make serial;

clean:
	for dir in $(APPLICATION_DIRS) ; do cd $$dir ; make clean ; cd .. ; done
