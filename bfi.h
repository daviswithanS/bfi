#ifndef GUARD_bfi_h
#define GUARD_bfi_h

#include <vector>

class Instructions {
public:
	Instructions(char*);

	char current() const { return *iter; }
	bool eof() const { return iter == instructions.end(); }

	void next() { ++iter; }
	void jumpf();
	void jumpb();
private:
	std::vector<char> instructions;
	std::vector<char>::const_iterator iter;

	bool is_valid(char);
};

class CellSpace {
public:
	CellSpace(std::vector<unsigned char>::size_type);

	unsigned char value() const { return cells[index]; }

	void move_right();
	void move_left();
	void increment() { ++cells[index]; }
	void decrement() { --cells[index]; }
	void write();
	void read();
private:
	// represents the "address field" - a vector of cells from 0 to
	// infinity, where each cell stores a number up to 255
	std::vector<unsigned char> cells;
	std::vector<unsigned char>::size_type index;
};

#endif
