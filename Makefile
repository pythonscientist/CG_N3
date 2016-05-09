CPP_FILES := $(wildcard ./*.cpp)
OBJ_FILES := $(addprefix ./,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -lGL -lGLU -lglut
CC_FLAGS := -Wall -std=c++11 -ggdb

main: $(OBJ_FILES)
	g++ -o $@ $^ $(LD_FLAGS)
./%.o: ./%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<
clean:
	rm *.o main
