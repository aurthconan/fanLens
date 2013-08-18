#ifndef FANTRIPLE_H

#define FANTRIPLE_H

template<typename T>
class fanTriple
{
public:
    union {
        struct {
            T x, y, z
        };
        T axis[3];
    };

    fanTriple();
    ~fanTriple();
};

fanTriple::fanTriple()
    : x(0), y(0), z(0)
{
}

#endif /* end of include guard: FANTRIPLE_H */
