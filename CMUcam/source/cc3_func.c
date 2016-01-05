#include "cc3_func.h"

void initAll(void)
{
//SYSTEM INIT
    cc3_filesystem_init ();
//GPIO INIT
    cc3_gpio_set_mode(0, CC3_GPIO_MODE_SERVO);
    cc3_gpio_set_mode(2, CC3_GPIO_MODE_SERVO);
    cc3_gpio_set_servo_position(0, POS_ARRET);
    cc3_gpio_set_servo_position(2, POS_ARRET);
//LED INIT
    cc3_led_set_state(0, true);
    cc3_led_set_state(1, true);
//UART INIT
    cc3_uart_init(0, CC3_UART_RATE_115200, CC3_UART_MODE_8N1, CC3_UART_BINMODE_TEXT);
    setvbuf (stdout, NULL, _IONBF, 0);
    setvbuf (stdin, NULL, _IONBF, 0);
//CAMERA INIT
    cc3_camera_init();
    cc3_camera_set_auto_exposure(true);
    cc3_camera_set_auto_white_balance(true);
    cc3_camera_set_colorspace(CC3_COLORSPACE_HSV);
    cc3_camera_set_resolution(CC3_CAMERA_RESOLUTION_LOW);
	cc3_pixbuf_frame_set_coi(CC3_CHANNEL_VAL);
}

int motComm(uint8_t x, uint8_t max_x, char speCom)
{
//Vairables
    const int8_t MIDDLE = max_x/2;
    int8_t desax = x - MIDDLE, vrel;

	#ifdef PRINTF_MOTCOMM
		printf ("\nDebut motCom \n\tdesax:%d\n", desax);
	#endif

//Direction : 0 < abs(desax) < MIDDLE (88)    0 < vitessse < POS_MED (162)
    vrel = abs(desax) * (VITESSE) / MIDDLE;

	#ifdef PRINTF_MOTCOMM
		printf("vrel:%d\n", vrel);
	#endif

	if(speCom == 'O')
	{
		if(vrel <= VITESSE/10) // x gauche => VITESSE droit < pos arrêt
		{
			cc3_gpio_set_servo_position(SERVO_DROITE, POS_ARRET - VITESSE);
			cc3_gpio_set_servo_position(SERVO_GAUCHE, POS_ARRET + VITESSE);
		}
		else if(desax < 0) // x gauche => VITESSE droit < pos arrêt
		{
			cc3_gpio_set_servo_position(SERVO_DROITE, POS_ARRET - VITESSE - vrel);
			cc3_gpio_set_servo_position(SERVO_GAUCHE, POS_ARRET + VITESSE - vrel);
		}
		else //x droite => VITESSE droit < pos arrêt
		{
			cc3_gpio_set_servo_position(SERVO_DROITE, POS_ARRET - VITESSE + vrel * 11/10);
			cc3_gpio_set_servo_position(SERVO_GAUCHE, POS_ARRET + VITESSE + vrel * 11/10);
		}
	}
	else if(speCom == 'R')//Mode Recherche de ligne
	{
		cc3_gpio_set_servo_position(SERVO_DROITE, POS_ARRET - 10);
		cc3_gpio_set_servo_position(SERVO_GAUCHE, POS_ARRET - 10);
	}
	else
	{
		cc3_gpio_set_servo_position(SERVO_DROITE, POS_ARRET);
		cc3_gpio_set_servo_position(SERVO_GAUCHE, POS_ARRET);
	}
    return 0;
}

int imgAnalysis(uint16_t* pX, uint16_t* pMax_x)
{
//Variables
    cc3_image_t img;
    uint8_t val, min_val, i=0, moy=0;
    uint16_t x=0, my_x=0, sum=0;
	uint8_t tab_x[NB_VAL] = {0};

//Récuperation
    img.channels = 3;
    img.width = cc3_g_pixbuf_frame.width;
    img.height = 1;
    img.pix = cc3_malloc_rows(1);
    //cc3_pixbuf_frame_set_coi(CC3_CHANNEL_VAL);
    cc3_pixbuf_load();
	cc3_g_pixbuf_frame.y_step = Y_STEP;

//Traitement
	//on capte la position du pixel le plus foncé sur 10 lignes, on les met dans tab_x
	#ifdef PRINTF
		printf("ImgAnalysis begin now\n");
		printf("Récuperation des valeurs\n");
		printf("\tPremiere boucle\n");
	#endif

    while(cc3_pixbuf_read_rows(img.pix, 1)) //Read a row into the image picture memory from the camera
    {
		min_val=255;
        for(x=0; x<img.width; x++)
        {
			val = ((uint8_t *) img.pix)[x];
            if(val < min_val && val!=16 && x!=174)//evite le pixel bloqué
            {
				min_val = val;
                my_x = x;
			}
		}

		if(min_val<50)
			tab_x[i] = my_x;
		else
			i--;

		#ifdef PRINTF_IGMANAL
			printf("\tmin_val:%d  my_x:%d  tab_x:%d  i:%d\n", min_val, my_x, tab_x[i], i);
		#endif
		i++; //à la derniere incrementation, i correspond au nombre de ligne du tableau
    }
	printf("\ti:%d\n", i);
	if(i < 2)
		return 1;

	//on fait la somme de tab_x puis la moyenne
	for(uint8_t j=0; j<i; j++)
	{
		sum += tab_x[j];
	}
	moy = sum / i;

	//liberation de la memoire
	free(img.pix);

	//modification des valeurs par pointeurs
    *pX = moy;
    *pMax_x = img.width;

	#ifdef PRINTF_IGMANAL
		printf ("\n--- Fin de l'interpretation ---");
	#endif
    return 0;
}
