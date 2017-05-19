CC=gcc
LD=gcc
CXX=g++

TARGET=calc

OBJS=calc.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(OBJS) -o $(TARGET) -ldl

%.o: %.c
	$(CC) -c $^

%.o: %.cpp
	$(CXX) -c $^

clean:
	rm -rf $(TARGET) $(OBJS)
