all: hellsgate

hellsgate: hellsgate.cpp
	g++ hellsgate.cpp -o hellsgate -lncurses
