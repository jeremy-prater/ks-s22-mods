#ifndef _S22_SOUND_
#define _S22_SOUND_

#ifdef __cplusplus
extern "C"
{
#endif

    void sound_init();
    void sound_play(const char *filename);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif