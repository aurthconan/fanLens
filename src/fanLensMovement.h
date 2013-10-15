#ifndef FANLENSMOVEMENT_H

#define FANLENSMOVEMENT_H

#include <fanVector.h>

namespace fan {

class fanLens;

void Yaw( float radian, fan::fanLens& lens );
void Pitch( float radian, fan::fanLens& lens );
void Roll( float radian, fan::fanLens& lens );

void YawAroundFocus( float radian, fan::fanLens& lens );
void PitchAroundFocus( float radian, fan::fanLens& lens );
void RollAroundFocus( float radian, fan::fanLens& lens );

void Translate( const fan::fanVector<float, 3>& pos, fan::fanLens& lens );

} /* namespace fan */

#endif /* end of include guard: FANLENSMOVEMENT_H */
