FILES = main.c parse.c driver.c
OBJECTS = $(subst .c,.o,$(FILES))

NAME = guard_start
BUILD_DIR = ../build/$(NAME)

all:
	gcc -c $(FILES)
	gcc -o $(NAME) $(OBJECTS)
	mv $(NAME) $(BUILD_DIR)
clean:
	rm -rf $(OBJECTS) $(BUILD_DIR)
