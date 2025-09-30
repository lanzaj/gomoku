# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlanza <jlanza@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/16 12:05:27 by jlanza            #+#    #+#              #
#    Updated: 2025/09/30 11:16:15 by jlanza           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Gomoku
TEST_BIN  = run_tests

SOURCES_DIR = ./src/
BUILD_DIR = ./build/
TESTS_DIR = ./tests
LIBS_DIR   = ./libs/googletest

SOURCES =	Board.cpp \
			Gomoku.cpp \
			Player.cpp \
			Server.cpp \
			Cell.cpp

OBJECTS		= $(addprefix ${BUILD_DIR}, ${SOURCES:.cpp=.o})
DEPS := $(OBJECTS:.o=.d)

CC = c++
CFLAGS =	-Wall -Wextra -Werror -MMD -MP -g3 -std=c++17 \
			-Iinclude -Ilibs \
			-I$(LIBS_DIR)/include \
			-I$(LIBS_DIR) \
 			-pthread \
			-O2 \
			-march=native \
			-flto
		
CLIB   = -pthread

GTEST_SRC = $(LIBS_DIR)/src/gtest-all.cc
GTEST_OBJS = ${BUILD_DIR}gtest-all.o

TESTS_FILES = \
    main_tests.cpp \
    alignment2.cpp \
    heatmap.cpp \
    board_init.cpp

TESTS_SRC = $(addprefix $(TESTS_DIR)/, $(TESTS_FILES))
TESTS_OBJ = $(addprefix $(BUILD_DIR), $(TESTS_FILES:.cpp=.o))

RM = rm -rf

# **************************************************************************** #
#                               BUILD RULES                                    #
# **************************************************************************** #

all: $(NAME)

$(BUILD_DIR)%.o: $(SOURCES_DIR)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "\e[32mCompiling \e[0m" $<

$(NAME):	$(OBJECTS) 
			@echo
			@$(CC) $(CFLAGS) $(OBJECTS) ./src/Main.cpp $(CLIB) -o $(NAME)
			@echo "\nCreating ./"$(NAME)
			@echo "\n		      ALL DONE THANKS!\e[36m"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⣤⣄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⡿⠇⠀⠀⠸⢿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⣤⣾⣿⣿⣿⣦⡀⢀⣴⣿⣿⣿⣷⣤⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⠁⠈⠙⣷⣾⠋⠁⠈⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣷⣶⣶⣿⣿⣶⣶⣾⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⠀⣦⣙⢿⣿⣿⣿⣿⡿⣋⣴⠀⠀⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⢀⣀⣽⣿⣿⣮⡉⢩⣷⣿⣿⣯⣀⡀⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⣰⣾⣿⣿⣿⣿⣿⣿⣿⣾⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀"
			@echo "		⠀⠀⣀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣀⠀⠀"
			@echo "		⢀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀"
			@echo "		⢺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿"
			@echo "		⠀⠙⠿⠿⠿⠿⠿⢿⣿⣿⣶⣦⣴⣿⣷⣦⣴⣾⣿⣿⣿⠿⠿⠟⠿⠿⠛⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠘⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠇⠀⠀⠀⠀⠀⠀⠀"
			@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠛⠛⠛⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\e[0m"

memcheck: all
	(valgrind --track-fds=yes --trace-children=yes --leak-check=full --show-leak-kinds=all ./$(NAME) ) &
#	-fsanitize=address

./build/gtest-all.o: $(LIBS_DIR)/src/gtest-all.cc
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "\e[34mCompiling test library \e[0m" $<

$(BUILD_DIR)%.o: $(TESTS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "\e[34mCompiling test \e[0m" $<


$(TEST_BIN): $(TESTS_OBJ) $(GTEST_OBJS) $(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) $(TESTS_OBJ) $(GTEST_OBJS) $(CLIB) -o $(TEST_BIN)
	@echo "\e[34mBuilding tests done!\e[0m"
	
tests: $(TEST_BIN)
	./$(TEST_BIN)
	
# 	./run_tests --gtest_filter=

run: all
	./$(NAME) &
	cd ../frontend && python3 Gomoku.py

# **************************************************************************** #
#                               CLEAN RULES                                    #
# **************************************************************************** #

clean:
	$(RM) $(BUILD_DIR) Gomoku.d

fclean: clean
	$(RM)  $(NAME)
	$(RM) $(TEST_BIN)
	@clear

re: fclean $(NAME)
	./$(NAME) &
	
avatar:
	@echo "\n			Water. Earth. Fire. Air.\n"
	@echo "	\e[32mLong ago, the four nations lived together in harmony."
	@echo "	Then everything changed when the Fire Nation attacked"
	@echo "	Only the Avatar, master of all four elements, could stop"
	@echo "	them. But when the world needed him most, he vanished.\n"
	@echo "	A hundred years passed and my brother and I discovered"
	@echo "	the new Avatar, an airbender named Aang, and although"
	@echo "	his airbending skills are great, he still has a lot to"
	@echo "	learn before he's ready to save anyone.\n"
	@echo "	But I believe Aang can save the world.\e[0m\n"
	@echo "		⠀⠀⠀⠀⠀⠀⠀⠀⢀⠔⣪⣿⡿⠋⠉⠀⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "		⡇⠀⠀⠀⠀⢀⣼⣿⠟⠉⠉⠙⢰⣇⠀⠀⠀⢻⡆⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "		⣷⡀⢀⢤⣤⣾⣿⣿⣤⣤⠄⠀⠘⣿⣆⠀⠀⠀⢧⠀⠈⢷⡄⠀⠀⠀⠀⠀⠀⠀"
	@echo "		⠙⠿⠇⠀⢻⣿⣿⡿⠋⠀⠀⠀⢸⣿⠏⠂⠀⠀⠀⠀⠀⠀⠃⠐⠀⠀⠀⠀⠀⠀"
	@echo "		⠀⠰⠀⠀⠀⠙⠉⠀⠀⠀⠀⠀⠀⠀⢠⠀⠀⠀⠀⠀⠀⠀⢀⠀⠁⠀⠀⠀⠀⠀"
	@echo "		⠀⠀⠣⢤⣀⠀⠀⠀⣀⣠⣴⡦⡶⠚⠥⠆⠀⠀⠀⠀⠀⢰⡄⠀⡄⠀⠀⠀⠀⠀"
	@echo "		⠀⠀⠀⢸⢹⣭⣭⣭⣵⣦⠜⠁⠈⣦⡀⠀⠀⠀⠀⡆⠀⠘⠀⠀⣿⡄⠀⠀⠀⠀"
	@echo "		⠀⠀⠀⠀⠙⠿⢿⡿⠿⣯⣤⣴⣾⣿⣧⠀⠀⠀⢠⠁⠀⢸⠀⠀⡟⣿⣦⣀⠀⠀"
	@echo "		⠀⠀⠀⠀⠀⠀⠀⡉⠀⠀⢙⢉⣝⣉⠙⠀⠀⠀⣆⣀⣀⣠⣀⣼⡷⠤⠽⠛⠛⠃"
	@echo "		⠀⠀⠀⠀⠀⠀⠘⠛⠛⠛⠊⠈⠉⠉⠿⢷⣿⠾⠿⠛⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀\n"

.PHONY: all clean fclean memcheck re run

-include $(DEPS)