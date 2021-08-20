RB=linuxRB
CC?=g++
all:${RB}

deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o)

$(RB): $(obj)
    $(cc) -o $(RB) $(obj) -lm
%.o: %.c $(deps)
    $(cc) -c $< -o $@
clean:
	rm -rf $(obj) $(RB)