CC := cc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -g -MMD -MP
LDFLAGS :=

TARGET := main
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

.PHONY: clean run

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

run: $(TARGET)
	./$(TARGET)
