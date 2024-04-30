CC = g++
CFLAGS = -Wall -Wextra 

TARGET = kry
 
  all: $(TARGET)
 
  $(TARGET): $(TARGET).cpp
				$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp
 
  clean:
				$(RM) $(TARGET)