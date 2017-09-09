#include <SDL2/SDL.h>
#include "audio.h"

static unsigned char *audio_buffer;
static int audio_len;


static unsigned int wav_len;
static unsigned char *wav_buffer;
static SDL_AudioSpec wav_spec;



void callback(void *userdata, unsigned char *stream, int len){
	if(audio_len==0){
		audio_buffer=wav_buffer; 
		audio_len=wav_len;
		return;
	}
	
	len=len>audio_len ? audio_len : len ;
	SDL_memcpy(stream, audio_buffer, len); 				
	SDL_MixAudio(stream, audio_buffer, len, 128);
	
	audio_buffer += len;
	audio_len -= len;

	if(userdata) return;
}


void initAudio(){
	SDL_Init(SDL_INIT_AUDIO);

	if(SDL_LoadWAV("res/music.wav", &wav_spec, &wav_buffer, &wav_len)==NULL){
		printf("ERROR: Could not found music.wav resource\n");
		exit(3);
	}

	wav_spec.callback=callback;
	wav_spec.userdata=NULL;

	audio_buffer=wav_buffer; 
	audio_len=wav_len;

	if(SDL_OpenAudio(&wav_spec, NULL)<0){
	  	printf("ERROR: Could not open audio device: %s\n", SDL_GetError());
	  	exit(4);
	}
}

void setMusic(char on){
	if(on){			
		SDL_PauseAudio(0);
	}else{
		audio_buffer=wav_buffer; 
		audio_len=wav_len;

		SDL_PauseAudio(1);		
	}
}

void pauseMusic(char pause){
	SDL_PauseAudio(pause);
}

void cleanAudio(){
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
}


