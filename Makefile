CC = g++
CFLAGS = -Wall -Wextra -fsanitize=address -fsanitize=leak

TARGET = main
 
  all: $(TARGET)
 
  $(TARGET): $(TARGET).cpp
				$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp
 
  clean:
				$(RM) $(TARGET)