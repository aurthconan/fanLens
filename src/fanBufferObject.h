#ifndef FANBUFFEROBJECT_H

#define FANBUFFEROBJECT_H

namespace fan {

template<typename T>
class fanBufferObject
{
public:
    fanBufferObject( size_t size )
        : mSize( size )
        , mBuffer( new T[mSize] )
    {
    }
    ~fanBufferObject() {
        if ( mBuffer ) {
            delete[] mBuffer;
        }
    }

    const size_t mSize;
    T* const mBuffer;

private:
    fanBufferObject();
    fanBufferObject( const fanBufferObject& );
    void operator=( const fanBufferObject& );
};

} /* namespace fan */

#endif /* end of include guard: FANBUFFEROBJECT_H */
