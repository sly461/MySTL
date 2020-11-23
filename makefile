CXX ?= g++

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g
else
    CXXFLAGS += -O2

endif

test: main.cpp
	$(CXX) -o test $^ $(CXXFLAGS)
clean:
	rm  -r test
