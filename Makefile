NAME = minishell

CC = cc 
CFLAGS = -Wall -Wextra -Werror -g
COMPILE_FLAGS = -c

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a
BUILD_DIR = build

SRC_SUBDIRS = . builtins env executor parser signals

SRC = $(foreach dir, $(SRC_SUBDIRS), $(wildcard $(dir)/*.c))
OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -I$(LIBFT_DIR) -Iinclude $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(BUILD_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

val: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp --suppressions=executables.supp ./minishell

.PHONY: all clean fclean re val