CC=gcc
LD=gcc
CXX=g++

TARGET=libexample-plugin-1.so

OBJS=calc.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) --shared $(OBJS) -o $(TARGET) -lm

%.o: %.c
	$(CC) -fPIC -c $^

%.o: %.cpp
	$(CXX) -fPIC -c $^

clean:
	rm -rf $(TARGET) $(OBJS)
