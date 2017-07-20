#ifndef GUARD_bf_h
#define GUARD_bf_h

#include <cstddef>
#include <string>
#include <vector>

// The Interpreter stores the instructions and indicates where the process is 
// in its program.
class Interpreter {
public:
	Interpreter(const std::string&);
	
	char current() const { return *iter; }
	void next() { ++iter; }
	void jumpf();
	void jumpb();
private:
	std::vector<char> commands;
	std::vector<char>::const_iterator iter;
	void load_instructions(const std::string&);
	bool is_valid(const char c);
};

// The Pointer indicates the "address" of the current cell in memory. It can be
// thought of as an infinite string of cells starting at 0, where each cell can
// store a number up to 255. 
class Pointer {
public:
	Pointer();
	Pointer(std::vector<unsigned char>::size_type); 

	unsigned char current() const { return cells[index]; }
	void right();
	void left();
	void increment();
	void decrement();
	void write();
	void read();
private:
	std::vector<unsigned char> cells;
	std::vector<unsigned char>::size_type index;
	std::vector<unsigned char>::size_type size;
};

#endif
