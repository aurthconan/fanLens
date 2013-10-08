#ifndef FANLENSMOVEMENT_H

#define FANLENSMOVEMENT_H

namespace fan {

class fanLens;

void Yaw( float radian, fan::fanLens& lens );
void Pitch( float radian, fan::fanLens& lens );
void Roll( float radian, fan::fanLens& lens );

void YawAroundFocus( float radian, fan::fanLens& lens );
void PitchAroundFocus( float radian, fan::fanLens& lens );
void RollAroundFocus( float radian, fan::fanLens& lens );

} /* namespace fan */

#endif /* end of include guard: FANLENSMOVEMENT_H */
