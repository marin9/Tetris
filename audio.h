#ifndef AUDIO_H
#define AUDIO_H

void callback(void *userdata, unsigned char *stream, int len);
void initAudio();
void setMusic(char on);
void pauseMusic(char pause);
void cleanAudio();

#endif
