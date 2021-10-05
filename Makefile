all: compare

compare: compare.cpp
	g++ -mavx2 compare.cpp -o compare