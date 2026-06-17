NAME = philo

INCLUDES = ./includes

MY_SOURCES = $(addprefix srcs/, \
			main.c \
			check_input.c \
			utils.c \
			philo_routine.c \
			monitor_routine.c \
			simulation.c \
			init.c \
			destroy.c)

MY_OBJECTS = $(MY_SOURCES:.c=.o)

CFLAGS += -Wall -Wextra -Werror -g $(addprefix -I, $(INCLUDES))

all: $(NAME)

$(NAME): $(MY_OBJECTS)
	cc $(CFLAGS) $(MY_SOURCES) -o $(NAME)

clean:
	rm -f $(MY_OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all
