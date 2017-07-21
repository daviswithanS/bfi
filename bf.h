#ifndef GUARD_bf_h
#define GUARD_bf_h

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

#endif
