// a brainfuck interpreter
#include <fstream>
#include <iostream>
#include <vector>
#include "bfi.h"

using std::ifstream;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

Instructions::Instructions(char* filename)
{
	ifstream file(filename);
	if (!file) {
		cerr << "invalid or missing filename" << endl;
	}

	// load instructions and clear all non-brainfuck commands
	char command;
	while (file.get(command)) {
		if (is_valid(command)) {
			instructions.push_back(command);
		}
	}
	iter = instructions.begin();
}

bool Instructions::is_valid(char c)
{
	return c == '>' || c == '<' || c == '+' || c == '-'
		|| c == '.' || c == ',' || c == '[' || c == ']';
}

void Instructions::jumpf()
{
	vector<char>::size_type brackets = 0;

	while (iter != instructions.end()) {
		switch (*iter) {
		case '[': ++brackets; break;
		case ']': --brackets; break;
		}

		if (brackets == 0) {
			break;
		}
		++iter;
	}

	if (brackets > 0) {
		cerr << ("program had an unclosed '['") << endl;
	}
}

void Instructions::jumpb()
{
	vector<char>::size_type brackets = 0;

	while (iter != instructions.begin()) {
		switch (*iter) {
		case ']': ++brackets; break;
		case '[': --brackets; break;
		}

		if (brackets == 0) {
			break;
		}
		--iter;
	}

	if (brackets > 0) {
		cerr << "program had an unclosed ']'" << endl;
	}
}

CellSpace::CellSpace(vector<unsigned char>::size_type n): cells(n, 0), index(0) {}

void CellSpace::move_right()
{
	if (index == cells.max_size()-1) {
		cerr << "program tried to access too large of a cell" << endl;
	}
	++index;
	// add more cells if necessary
	if (index >= cells.size()) {
		cells.push_back(0);
	}
}

void CellSpace::move_left()
{
	if (index == 0) {
		cerr << "program tried to access cell -1" << endl;
	}
	--index;
}

void CellSpace::write()
{
	cout << cells[index];
}

void CellSpace::read()
{
	char c;
	if (!cin.get(c)) {
		c = 0;
	}
	cells[index] = c;
}

int main(int argc, char** argv)
{
	if (argc == 1) {
		cerr << "no file given" << endl;
	}

	Instructions in(argv[1]);
	CellSpace cs(3000);

	while (!in.eof()) {
		switch (in.current()) {
		case '>': cs.move_right(); break;
		case '<': cs.move_left(); break;
		case '+': cs.increment(); break;
		case '-': cs.decrement(); break;
		case '.': cs.write(); break;
		case ',': cs.read(); break;
		case '[': if (cs.value() == 0) { in.jumpf(); }; break;
		case ']': if (cs.value() != 0) { in.jumpb(); }; break;
		}
		in.next();
	}
	return 0;
}	
