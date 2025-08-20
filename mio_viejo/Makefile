# === Compilador y flags ===
CC      = gcc 
CFLAGS  = 

# === Rutas y nombres ===
NAME        = minishell
LIBFT_DIR   = libft
LIBFT_A     = $(LIBFT_DIR)/libft.a
BUILD_DIR   = build

# === Fuentes y objetos ===
SRC     = $(wildcard *.c)
OBJ     = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))

# === Reglas ===
all: $(NAME)

$(NAME): libft $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

libft:
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(BUILD_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft
