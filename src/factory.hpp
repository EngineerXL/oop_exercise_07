#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <memory>

#include "rectangle.hpp"
#include "square.hpp"
#include "trapeze.hpp"

template<class T, class FIGURE>
class TFactory;

template<class T>
class TFactory< T, TSquare<T> > {
public:
	static std::shared_ptr<IFigure> CreateFigure() {
		std::pair<T, T> curCords;
		T curSide;
		std::cout << "Input square as follows: x y a" << std::endl;
		std::cout << "x, y is a left bottom corner cords" << std::endl;
		std::cout << "a is square side" << std::endl;
		std::cin >> curCords.first >> curCords.second >> curSide;
		TSquare<T>* sq = new TSquare<T>(curCords, curSide);
		return std::shared_ptr<IFigure>(sq);
	}

	static std::shared_ptr<IFigure> Read(FILE* in) {
		std::pair<T, T> curCords;
		T curSide;
		fread(&curCords.first, sizeof(T), 1, in);
		fread(&curCords.second, sizeof(T), 1, in);
		fread(&curSide, sizeof(T), 1, in);
		TSquare<T>* sq = new TSquare<T>(curCords, curSide);
		return std::shared_ptr<IFigure>(sq);
	}
};

template<class T>
class TFactory< T, TRectangle<T> > {
public:
	static std::shared_ptr<IFigure> CreateFigure() {
		std::pair<T, T> curCords;
		T curHeight, curWidth;
		std::cout << "Input rectangle as follows: x y a b" << std::endl;
		std::cout << "x, y is a left bottom corner cords" << std::endl;
		std::cout << "a and b are width and heigth" << std::endl;
		std::cin >> curCords.first >> curCords.second >> curHeight >> curWidth;
		TRectangle<T>* rect = new TRectangle<T>(curCords, curHeight, curWidth);
		return std::shared_ptr<IFigure>(rect);
	}

	static std::shared_ptr<IFigure> Read(FILE* in) {
		std::pair<T, T> curCords;
		T curHeight, curWidth;
		fread(&curCords.first, sizeof(T), 1, in);
		fread(&curCords.second, sizeof(T), 1, in);
		fread(&curHeight, sizeof(T), 1, in);
		fread(&curWidth, sizeof(T), 1, in);
		TRectangle<T>* rect = new TRectangle<T>(curCords, curHeight, curWidth);
		return std::shared_ptr<IFigure>(rect);
	}
};

template<class T>
class TFactory< T, TTrapeze<T> > {
public:
	static std::shared_ptr<IFigure> CreateFigure() {
		std::pair<T, T> curCords;
		T curGreaterBase, curSmallerBase, curHeight;
		std::cout << "Input trapeze as follows: x y a b c" << std::endl;
		std::cout << "x, y is a left bottom corner cords" << std::endl;
		std::cout << "a, b and c are larger, smaller base and height" << std::endl;
		std::cin >> curCords.first >> curCords.second >> curGreaterBase >> curSmallerBase >> curHeight;
		TTrapeze<T>* trap = new TTrapeze<T>(curCords, curGreaterBase, curSmallerBase, curHeight);
		return std::shared_ptr<IFigure>(trap);
	}

	static std::shared_ptr<IFigure> Read(FILE* in) {
		std::pair<T, T> curCords;
		T curGreaterBase, curSmallerBase, curHeight;
		fread(&curCords.first, sizeof(T), 1, in);
		fread(&curCords.second, sizeof(T), 1, in);
		fread(&curGreaterBase, sizeof(T), 1, in);
		fread(&curSmallerBase, sizeof(T), 1, in);
		fread(&curHeight, sizeof(T), 1, in);
		TTrapeze<T>* trap = new TTrapeze<T>(curCords, curGreaterBase, curSmallerBase, curHeight);
		return std::shared_ptr<IFigure>(trap);
	}
};

#endif /* FACTORY_HPP */