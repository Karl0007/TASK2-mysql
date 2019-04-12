target = main
source = $(wildcard *.cpp)
OBJS = $(source:%.cpp=%.o)

$(target): $(OBJS)
	g++ -o $(target) $(OBJS)
%.o:%.cpp
	g++ -c -o $@ $<
clean:
	rm main *.o
