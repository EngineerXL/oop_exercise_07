#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <iostream>
#include <tuple>

class IFigure {
public:
	virtual void Print(std::ostream & of) = 0;
	virtual void Write(FILE* out) = 0;
	virtual ~IFigure() {}
};

template<class T1, class T2>
std::ostream & operator << (std::ostream & out, const std::pair<T1, T2> & p) {
	out << "(" << p.first << ", " << p.second << ")";
	return out;
}

#endif /* FIGURE_HPP */