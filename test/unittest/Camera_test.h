#ifndef CAMERA_TEST_H

#define CAMERA_TEST_H

#include <gtest/gtest.h>

#include <fanFilm.h>
#include <fanVector.h>
#include <fanMatrix.h>
#include <fanPixel.h>
#include <fanCamera.h>
#include <fanTriangleMesh.h>
#include <texture/film/FreeImageFilm.h>
#include <texture/film/SDLFilm.h>
#include <lens/OrthogonalLens.h>
#include <lens/PerspectiveLens.h>

#include <fanBufferObject.h>

#include <boost/shared_ptr.hpp>

#include <unistd.h>

template <class T>
fan::fanCamera* CreateCamera() { return NULL; };

template <class T>
void DeleteCamera( fan::fanCamera* camera ) {
    delete camera;
}

template<typename T>
class Camera_test
    : public testing::Test
{
public:
    Camera_test()
        : createCamera( CreateCamera<T> )
        , deleteCamera( DeleteCamera<T> ) {};
    virtual ~Camera_test() {};

    fan::fanCamera* (*createCamera)();
    void (*deleteCamera)( fan::fanCamera* camera );
};

TYPED_TEST_CASE_P(Camera_test);

#if GTEST_HAS_TYPED_TEST

TYPED_TEST_P(Camera_test, SimpleScene) {
    // a matrix of vertices
    fan::fanScene scene;
    boost::shared_ptr<fan::fanBufferObject<fan::fanVector3<float> > >
        vertices( new fan::fanBufferObject<fan::fanVector3<float> >( 125 ) );
    boost::shared_ptr<fan::fanBufferObject<fan::fanTriangle> >
        faces( new fan::fanBufferObject<fan::fanTriangle>( 0 ) );

    fan::fanMatrix<float, 4, 4> pos { 1, 0, 0, 0,
                                      0, 1, 0, 0,
                                      0, 0, 1, 0,
                                      0, 0, 0, 1 };
    fan::fanTriangleMesh mesh( pos );
    mesh.mVertices.push_back( vertices );
    mesh.mFaces.push_back( faces );
    int verticesNum = 0;
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            for ( size_t k = 0; k <= 200; k += 50 ) {
                vertices->mBuffer[verticesNum] = fan::fanVector3<float>( i, j, k );
                ++verticesNum;
            }
        }
    }
    scene.mTriangleMeshes.push_back( mesh );

    fan::fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    FreeImageFilm film( size, "SimpleScene.png" );
    SDLFilm sdl( size );

    OrthogonalLens OrthoLens( fan::fanVector3<float>(-1, -1, -1),
                              fan::fanVector3<float>(0, 0, 0),
                              fan::fanVector3<float>(0, 1, 0),
                              fan::fanVector3<float>(800, 600, 1000) );

    PerspectiveLens PersLens( fan::fanVector3<float>(-1, -1, -1),
                              fan::fanVector3<float>(0, 0, 0),
                              fan::fanVector3<float>(0, 1, 0),
                              fan::fanVector3<float>(800, 600, 1000),
                              5000 );


    fan::fanCamera* camera = this->createCamera();
    if ( !camera ) {
        FAIL() << "film not instantiate";
        return;
    }

    /*
    camera->takePicture( scene, film, OrthoLens );
    film.develope();
    camera->takePicture( scene, sdl, OrthoLens );
    sdl.develope();
    */

    camera->takePicture( scene, film, PersLens );
    film.develope();
    camera->takePicture( scene, sdl, PersLens );
    sdl.develope();

    this->deleteCamera( camera );
}

REGISTER_TYPED_TEST_CASE_P(
    Camera_test,
    SimpleScene);

#endif

#endif /* end of include guard: CAMERA_TEST_H */
