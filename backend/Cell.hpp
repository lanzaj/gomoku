#ifndef CELL_HPP
# define CELL_HPP

# include <iostream>

enum class Cell { Empty, Black, White };

std::ostream& operator<<(std::ostream& os, Cell cell);

#endif