BASE_DIR := $(shell pwd)

APPLICATION_DIRS := _axpy _blackscholes _swaptions _streamcluster _canneal _particlefilter _pathfinder _jacobi-2d

all: axpy blackscholes swaptions streamcluster canneal particlefilter pathfinder jacobi-2d

axpy:
	cd _axpy; 			\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

blackscholes:
	cd _blackscholes; 	\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

swaptions:
	cd _swaptions; 		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

streamcluster:
	cd _streamcluster; 	\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

canneal:
	cd _canneal; 		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

particlefilter:
	cd _particlefilter;	\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

pathfinder:
	cd _pathfinder;		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

jacobi-2d:
	cd _jacobi-2d;		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

matmul:
	cd _matmul; 		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;

clean:
	for dir in $(APPLICATION_DIRS) ; do cd $$dir ; make clean ; cd .. ; done
