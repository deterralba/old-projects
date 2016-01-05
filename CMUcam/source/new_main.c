#include "cc3_func.h"

int main(void)
{
//VARIABLES
    uint16_t x = 0, max_x = 0;
	char speComm = 'O';

//INIT
	initAll();

//START PROG
	#ifdef PRINTF_MAIN
		printf("BEGIN");
	#endif

	#ifdef PRINTF_MAIN
		printf("\nAppuyer sur le boutton");
	#endif
	//initialisation du blanc
	while(!cc3_button_get_state());//attente d'une pression du boutton

    while(1)
    {
		#ifdef BENCH
			uint16_t start_time, end_time;
			start_time = cc3_timer_get_current_ms();
		#endif

        //IMG
        if(imgAnalysis(&x, &max_x) == 0)
			speComm = 'O';
		else
			speComm = 'R';

		#ifdef PRINTF_ABS_FINAL
			printf("\nmoyenneFinal: %d", x);
		#endif

        //Motor
        motComm(x, max_x, speComm);

		#ifdef BENCH
			end_time = cc3_timer_get_current_ms();
			printf ("\ntime:%d", end_time - start_time);
		#endif
    }

    //END
	#ifdef PRINTF_MAIN
		printf ("\nEND");
	#endif
    return 0;
}
