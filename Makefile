CC = gcc
CFLAGS = -Wall -Wextra

TARGET = mini-Shell

all:
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	rm -f $(TARGET)
