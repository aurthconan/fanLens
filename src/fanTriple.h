#ifndef FANTRIPLE_H

#define FANTRIPLE_H

namespace fan {

template<typename T>
class fanTriple
{
public:
    union {
        struct {
            T x, y, z;
        };
        T axis[3];
    };

    fanTriple( T _x = 0, T _y = 0, T _z = 0 )
        : x(_x), y(_y), z(_z)
    {}

    virtual ~fanTriple() {}
};

}

#endif /* end of include guard: FANTRIPLE_H */

