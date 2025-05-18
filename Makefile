CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g # -Wall enables all warnings, -g adds debug info

# Source files
SRCS = main.c player.c world.c combat.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = arkhons_awakening

# Default target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean 