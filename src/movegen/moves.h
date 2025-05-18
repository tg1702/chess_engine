#include <vector>
#include "move.h"
#include "../types.h"

#pragma once
class Moves {
public:
    Moves() = default;

    explicit Moves(BoardState& bs) {
        generator.setState(bs);
        generator.generateMoves(&mv);

        moves_.insert(moves_.end(), &mv.moves[0], &mv.moves[mv.count]);
    }

    using iterator = std::vector<Move>::iterator;
    using const_iterator = std::vector<Move>::const_iterator;

    iterator begin() { return moves_.begin(); }
    iterator end() { return moves_.end(); }
    const_iterator begin() const { return moves_.begin(); }
    const_iterator end() const { return moves_.end(); }

    void erase(iterator it, iterator end) { moves_.erase(it, end);}

    Move& operator[](int i) { return moves_[i]; }
    const Move& operator[](int i) const { return moves_[i]; }
    size_t size() const { return moves_.size(); }

private:
    MoveList mv;
    std::vector<Move> moves_;
    MoveGen generator;
};

/* class Moves{
    public:
        explicit Moves(BoardState& bs) { 
            last = new MoveList();
            last->count = 0; 
            generator.setState(bs); 
            generator.generateMoves(last);
        }

        Moves(){
            last = new MoveList();
        }

        ~Moves()
        {
            delete last;
        }

        Moves(const Moves& other) {
            generator = other.generator;
            last = new MoveList(*other.last);
        }

        Moves& operator=(const Moves& other){
            if (this != &other){
                generator = other.generator;
                if (last) delete last;
                last = new MoveList(*other.last);
        }        

            return *this;
        }

        class iterator{
            public:
                iterator(Move* m) : move(m) {}
                iterator& operator++() {(++move); return *this;}
                bool operator!=(const iterator& other) { return other.move != move;}
                const Move& operator*() const {return *move;}

            private:
                Move* move;
        };


        iterator begin() const { return iterator(last->moves);}
        iterator end() const { return iterator(last->moves + last->count);}

         Move operator[](int i){
            return (last->moves[i]);
        }
        size_t size() const {return last->count;}

	
    private:
        MoveList* last;
        //MoveList* first = new MoveList();
        MoveGen generator;
}; */