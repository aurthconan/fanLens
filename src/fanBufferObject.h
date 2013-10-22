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

    size_t mSize;
    T* mBuffer;

private:
    fanBufferObject();
    fanBufferObject( const fanBufferObject& );
    void operator=( const fanBufferObject& );
};

} /* namespace fan */

#endif /* end of include guard: FANBUFFEROBJECT_H */
