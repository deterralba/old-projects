#ifndef CC3_FUNC
#define CC3_FUNC

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <cc3.h>
#include <cc3_ilp.h>

#define RELEASE //si activé, version finale
//#define PRINTF_MAIN
//#define PRINTF_INITBLANC
//#define PRINTF_IGMANAL
//#define PRINTF_MOTCOMM
//#define BENCH

//FINAL
#define PRINTF_ABS_FINAL

//Release
#ifdef RELEASE
	#undef PRINTF_MAIN
	#undef PRINTF_INITBLANC
	#undef PRINTF_IGMANAL
	#undef PRINTF_MOTCOMM
	#undef BENCH
	#undef PRINTF_SPECOMM_FINAL
#endif

#define SERVO_DROITE 2
#define SERVO_GAUCHE 0
#define POS_ARRET 162
#define Y_STEP 14 //permet 10 val par image
#define NB_VAL 10 //utilisé pour la tableau tab_x[]
#define VITESSE 20 //Vitesse Motcomm

void initAll(void);
int imgAnalysis(uint16_t* pX, uint16_t* pMax_x);
int motComm(uint8_t x, uint8_t max_x, char speComm);

#endif
