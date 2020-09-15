BASE_DIR := $(shell pwd)

APPLICATION_DIRS := _blackscholes _swaptions _streamcluster _canneal _particlefilter _pathfinder _jacobi-2d _matmul _axpy

all: blackscholes swaptions streamcluster canneal particlefilter pathfinder jacobi-2d matmul axpy

blackscholes:
	cd _blackscholes; 	\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

swaptions:
	cd _swaptions; 		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

streamcluster:
	cd _streamcluster; 	\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

canneal:
	cd _canneal; 		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

particlefilter:
	cd _particlefilter;	\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

pathfinder:
	cd _pathfinder;		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

jacobi-2d:
	cd _jacobi-2d;		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

matmul:
	cd _matmul; 		\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

axpy:
	cd _axpy; 			\
	make start;			\
	make vector; 		\
	make serial; 		\
	make host;			\
	make copy;

clean:
	for dir in $(APPLICATION_DIRS) ; do cd $$dir ; make clean ; cd .. ; done
