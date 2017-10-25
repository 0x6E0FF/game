BUILD_DIR=build
TARGET=$(BUILD_DIR)/game.exe

C_FLAGS=-g
CPP_FLAGS=-g -std=c++11
LD_FLAGS=

SRCS=$(wildcard src/*.cpp)
OBJS=$(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS)) lib/glad/glad.o

GLM=glm-0.9.8.5
GLFW=glfw-3.2.1.bin.WIN32

GLM_INCS=-Ilib/$(GLM)
GLM_HEADERS=$(wildcard lib/$(GLM)/glm/*/*.hpp) $(wildcard lib/$(GLM)/glm/*.hpp)
PRECOMPILED_HEADERS=$(patsubst %.hpp,%.hpp.gch,$(GLM_HEADERS))

# PRECOMPILED_HEADERS=lib/$(GLM)/glm
PRECOMPILED_HEADERS=

INCS=-Isrc/\
	 -Ilib/$(GLFW)/include\
	 -Ilib/glad\
	 $(GLM_INCS)
	 
LIBS=-Llib/$(GLFW)/lib-mingw\
	 -lglfw3 \
	 -lgdi32 \
	 -lopengl32

all: $(PRECOMPILED_HEADERS) $(TARGET)
	 
$(BUILD_DIR)/%.o : %.cpp
	@mkdir -p $(dir $@)
	g++ $(CPP_FLAGS) $(INCS) -c $< -o $@
	
%.hpp.gch : %.hpp
	@mkdir -p $(dir $@)
	g++ $(CPP_FLAGS) $(GLM_INCS) -c $< -o $@
	
lib/glad/glad.o : lib/glad/glad.c
	gcc $(C_FLAGS) $(INCS) -c $< -o $@

$(TARGET) : $(OBJS)
	g++ $(LD_FLAGS) $^ $(LIBS) -o $@
	
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(PRECOMPILED_HEADERS)
