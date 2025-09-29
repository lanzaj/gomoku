# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlanza <jlanza@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/31 12:11:01 by jlanza            #+#    #+#              #
#    Updated: 2025/09/30 01:47:13 by jlanza           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	cd backend && make run

clean:
	cd backend && make clean

fclean:
	cd backend && make fclean

memcheck:
	(cd backend && make memcheck) && (cd frontend && python3 Gomoku.py &)

re:
	(cd backend && make re) && (cd frontend && python3 Gomoku.py &)

.PHONY: all clean fclean memcheck re run