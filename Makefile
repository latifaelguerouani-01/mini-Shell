CC = gcc
CFLAGS = -Wall -Wextra

TARGET = minishell

all:
	$(CC) $(CFLAGS) main.c -o $(TARGET)

clean:
	rm -f $(TARGET)
