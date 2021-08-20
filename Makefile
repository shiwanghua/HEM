RB=linuxRB
CC?=mpic++
all:${RB}

deps = $(shell find ./ -name "*.h")
src = $(shell find ./ -name "*.cpp")
obj = $(src:%.cpp=%.o)

$(RB): $(obj)
    $(cc) -o $(RB) $(obj) -lstdc++ -lm
%.o: %.c $(deps)
    $(cc) -c $< -o $@
clean:
