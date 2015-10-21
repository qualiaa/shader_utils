LIBNAME=shader_utils

OBJDIR := obj
SRCDIR := src

LIB = lib$(LIBNAME).a
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
CXXFLAGS += -std=c++14 -Iinclude -Isrc

$(warning $(OBJS))

default: $(LIB)

$(LIB): $(OBJS)
	$(AR) rs $@ $^

obj/%.o: src/%.cpp | $(OBJDIR)
	$(CC) $(CXXFLAGS) -c $< -o $@

obj/load_shaders.o:  $(addprefix include/,GLException.hpp load_shaders.hpp)
obj/ShaderObject.o:  $(addprefix include/,GLException.hpp ShaderObject.hpp) src/read_file_to_string.hpp
obj/ShaderProgram.o: $(addprefix include/,GLException.hpp ShaderObject.hpp)
obj/read_file_to_string.o: src/read_file_to_string.hpp

.PHONY: clean default
clean:
	-$(RM) $(LIB) $(OBJS) 2>/dev/null; exit 0

$(OBJDIR):
	mkdir $(OBJDIR)
