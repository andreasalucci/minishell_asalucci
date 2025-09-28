# === Compilador y flags ===
CC      = gcc 
CFLAGS  = -Wall -Wextra -Werror -g

# === Rutas y nombres ===
NAME        = minishell
LIBFT_DIR   = libft
LIBFT_A     = $(LIBFT_DIR)/libft.a
BUILD_DIR   = build

# === Subcarpetas fuente ===
SRC_SUBDIRS = . builtins env executor parser signals

# === Fuentes y objetos ===
SRC = $(foreach dir, $(SRC_SUBDIRS), $(wildcard $(dir)/*.c))
OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))

# === Reglas ===
all: $(NAME)

$(NAME): libft $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_A) -lreadline -o $(NAME)

# Regla para compilar archivos .c a .o respetando la estructura de subdirectorios
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -Iinclude -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

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