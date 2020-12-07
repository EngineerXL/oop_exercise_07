#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP

#include <list>
#include <stack>

#include "factory.hpp"

template<class SCALAR_TYPE>
class TDocument {
private:
	struct IAction;

	using figure_pointer = std::shared_ptr<IFigure>;
	using action_pointer = std::shared_ptr<IAction>;
	using const_iterator = std::list< figure_pointer >::const_iterator;

	std::list< figure_pointer > FiguresList;
	std::stack< action_pointer > ActionStack;

	struct IAction {
		virtual void PerformAction(TDocument & fact) = 0;
		virtual ~IAction() {}
	};

	class TDeleteAction : public IAction {
	private:
		size_t DeletePos;
	public:
		TDeleteAction(const size_t & pos) : DeletePos(pos) {}
		void PerformAction(TDocument & fact) override {
			fact.Delete(DeletePos);
		}
	};

	class TAddAction : public IAction {
	private:
		size_t AddPos;
		figure_pointer AddFigure;
	public:
		TAddAction(const size_t & pos, const figure_pointer & fig) : AddPos(pos), AddFigure(fig) {}
		void PerformAction(TDocument & fact) override {
			fact.AddFigure(AddPos, AddFigure);
		}
	};
public:
	void CreateNew() {
		while (!ActionStack.empty()) {
			ActionStack.pop();
		}
		FiguresList.clear();
	}

	void LoadFromFile(FILE* in) {
		CreateNew();
		size_t n;
		fread(&n, sizeof(size_t), 1, in);
		for (size_t i = 0; i < n; ++i) {
			unsigned int type;
			fread(&type, sizeof(unsigned int), 1, in);
			if (type == SQUARE_TYPE_ID) {
				FiguresList.push_back(TFactory< SCALAR_TYPE, TSquare<SCALAR_TYPE> >::Read(in));
			} else if (type == RECTANGLE_TYPE_ID) {
				FiguresList.push_back(TFactory< SCALAR_TYPE, TRectangle<SCALAR_TYPE> >::Read(in));
			} else if (type == TRAPEZE_TYPE_ID) {
				FiguresList.push_back(TFactory< SCALAR_TYPE, TTrapeze<SCALAR_TYPE> >::Read(in));
			}
		}
	}

	void SaveToFile(FILE* out) {
		size_t n = FiguresList.size();
		fwrite(&n, sizeof(size_t), 1, out);
		for (const_iterator it = FiguresList.begin(); it != FiguresList.end(); ++it) {
			(*it)->Write(out);
		}
	}

	void Add(const size_t & pos, const unsigned int & figureID) {
		if (figureID == SQUARE_TYPE_ID) {
			AddFigure(pos, TFactory<SCALAR_TYPE, TSquare<SCALAR_TYPE> >::CreateFigure());
		} else if (figureID == RECTANGLE_TYPE_ID) {
			AddFigure(pos, TFactory<SCALAR_TYPE, TRectangle<SCALAR_TYPE> >::CreateFigure());
		} else if (figureID == TRAPEZE_TYPE_ID) {
			AddFigure(pos, TFactory<SCALAR_TYPE, TTrapeze<SCALAR_TYPE> >::CreateFigure());
		}
	}

	void AddFigure(const size_t & pos, const figure_pointer & fig) {
		if (pos > FiguresList.size()) {
			FiguresList.push_back(fig);
			TDeleteAction* delAct = new TDeleteAction(FiguresList.size());
			ActionStack.push(action_pointer(delAct));
		} else {
			size_t cur = 0;
			const_iterator it = FiguresList.begin();
			while (cur < pos) {
				++cur;
				++it;
			}
			FiguresList.insert(it, fig);
			TDeleteAction* delAct = new TDeleteAction(pos + 1);
			ActionStack.push(action_pointer(delAct));
		}
	}

	void Delete(const size_t & pos) {
		if (FiguresList.empty()) {
			std::cout << "Nothing to delete!" << std::endl;
			return;
		}
		if (pos > FiguresList.size()) {
			TAddAction* addAct = new TAddAction(FiguresList.size() - 1, FiguresList.back());
			ActionStack.push(action_pointer(addAct));
			FiguresList.pop_back();
		} else {
			size_t cur = 1;
			const_iterator it = FiguresList.begin();
			while (cur < pos) {
				++cur;
				++it;
			}
			TAddAction* addAct = new TAddAction(cur - 1, *it);
			ActionStack.push(action_pointer(addAct));
			FiguresList.erase(it);
		}
	}

	void Undo() {
		if (ActionStack.empty()) {
			std::cout << "Nothing to undo!" << std::endl;
		} else {
			ActionStack.top()->PerformAction(*this);
			ActionStack.pop();
			ActionStack.pop();
		}
	}

	template<class U>
	friend std::ostream & operator << (std::ostream & of, const TDocument<U> & fact) {
		TDocument::const_iterator it = fact.FiguresList.begin();
		for (size_t i = 0; i < fact.FiguresList.size(); ++i) {
			of << "[" << i + 1 << "] ";
			(*it)->Print(of);
			of << std::endl;
			++it;
		}
		return of;
	}
};

#endif /* DOCUMENT_HPP */