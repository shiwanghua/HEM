RB=linuxRB
CC=g++

deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o)

all: ${RB}
$(RB): $(obj)
	$(CC) -std=c++11 -O3 -o $(RB) $(obj) #-lm
	rm -rf $(obj)
%.o: %.c $(deps)
	$(CC) -c $< -o $@
clean:
	rm -rf $(obj) $(RB)
