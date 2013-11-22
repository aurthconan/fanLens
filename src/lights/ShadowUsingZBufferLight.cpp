#include "ShadowUsingZBufferLight.h"

#include <camera/RasterisationScanner.h>
#include <filler/DepthFiller.h>

#include <texture/film/FreeImageFilm.h>
#include <texture/LinearInterpolatorTexture2D.h>

#include <fanFiller.h>

#include <limits>
#include <cmath>

using namespace boost;
using namespace fan;

#define _BUFFER_SIZE_ 400

ShadowUsingZBufferLight::ShadowUsingZBufferLight( shared_ptr<PointLight> point,
                                                  const fanVector<float, 3>& dimens,
                                                  const fanVector<float, 3>& pointAt )
    : mpPointLight( point )
    , mDimens{_BUFFER_SIZE_, _BUFFER_SIZE_}
    , m3Dimens( dimens )
    , mPointAt( pointAt )
{
    zNegative.reset( new MemoryTexture<int, float, 2>( mDimens ) );
    zNegative->reset(std::numeric_limits<float>::max());
}

class DepthFillerWithBias
    : public fanFiller
{
public:
    class Data {
    public:
        Data& operator+=( const Data& o ) {
            this->pos += o.pos;
            this->normal += o.normal;
            return *this;
        }

        Data& operator-=( const Data& o ) {
            this->pos -= o.pos;
            this->normal -= o.normal;
            return *this;
        }

        Data& operator*=( const int& ratio ) {
            this->pos *= ratio;
            this->normal *= ratio;
            return *this;
        }

        Data& operator/=( const int& ratio ) {
            this->pos /= ratio;
            this->normal /= ratio;
            return *this;
        }

        fan::fanVector3<float> pos;
        fan::fanVector3<float> normal;
    };

    DepthFillerWithBias( float factor, fanVector3<float> zNormal )
        : mFactor( factor*factor )
        , mZNormal( zNormal )
    {
    }

    inline void getCompaionData( size_t i,
                                 fan::fanTriangle& triangle,
                                 fan::fanTriangleMesh& mesh,
                                 TriangleMeshObject& object,
                                 fan::fanVector<float,4>& coord,
                                 Data& data )
    {   (void) i; (void) coord; (void) data;
        data.normal = mesh.mNormals->mBuffer[triangle.pointsIndex[i]];
        data.pos = transform( object.mObjectToWorld, *triangle.points[i] );
    };

    inline void plot( fan::fanVector<float, 2> pos,
                      Data& data,
                      float depth,
                      fan::fanTexture<int, float, 2>& texture )
    { (void) data; (void) pos; (void) depth; (void) texture;
        float bias = 0;
        if ( depth < texture.getValue( pos ) ) {
            float a = normalize(mZNormal)*data.normal;
            bias = std::sqrt(mFactor*(1 - a*a));
            texture.setValue( pos, depth + bias/2 );
        }
    }

    float mFactor;
    fanVector3<float> mZNormal;
};

void ShadowUsingZBufferLight::bakeShadowMap( fanScene& scene )
{
    zNLens.reset( new PerspectiveLens(mpPointLight->mPos, mPointAt,
                            mpPointLight->mPos + fanVector3<float>(1,0,0), m3Dimens, _BUFFER_SIZE_/2));

    zNegative->reset(std::numeric_limits<float>::max());
    DepthFillerWithBias filler( 0.01, mPointAt - mpPointLight->mPos );
    RasterisationScanner<DepthFillerWithBias, float> depthFiller( filler );

    depthFiller.takePicture( scene, *zNegative, *zNLens );

    /*
    FreeImageFilm freeImage( mDimens, "zNShadowMap.png" );
    fanVector<int, 2> pos;
    for ( int i = 0; i < mDimens[1]; ++i ) {
        pos[1] = i;
        for ( int j = 0; j < mDimens[0]; ++j ) {
            pos[0] = j;
            float depth = (1.0f + zNegative->getValue( pos ))/2.0f;
            fanPixel pixel( 255, 255*depth, 255*depth, 255*depth );
            freeImage.setValue( pos, pixel );
        }
    }
    freeImage.develope();
    // */
}

fanPixel ShadowUsingZBufferLight::getLight( fanVector3<float> world,
                                            fanVector3<float> normal,
                                            fanVector3<float> viewer )
{
    fan::fanVector<float, 2> pos;
    fan::fanVector<float, 4> homoPos;
    //LinearInterpolatorTexture2D<float> texture( *zNegative );
    project( world, *zNLens, mDimens, pos, homoPos );
    if ( !(homoPos[0] < 0 || homoPos[0] > 1
           || homoPos[1] < 0 || homoPos[1] > 1) ) {
        if ( homoPos[2]  <= zNegative->getValue( pos ) ) {
            return mpPointLight->getLight( world, normal, viewer );
        } else {
            return fanPixel(0,0,0,0);
        }
    }

    return fanPixel(0,0,0,0);
}

