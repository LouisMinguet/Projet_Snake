#include <stdlib.h>
#include <graph.h>
#include "jeu.h"
#include "option.h"
 
int main() {

    /* Initialisation de la fenêtre */
    InitialiserGraphique();
    CreerFenetre(0,0,1250,600);

    /* Initialisation des variables */
    int play_button = ChargerSprite("img/play_button.png");
    int option_button = ChargerSprite("img/option_button.png");
	int exit_button = ChargerSprite("img/exit_button.png");
    int snake_icon = ChargerSprite("img/snake2.png");
    int snake_icon_reverse = ChargerSprite("img/snake2_r.png");
    int c1, c2;

    int Param[14];
                Param[0]=60;
                Param[1]=40;
                Param[2]=5;
                Param[3]=5;
                Param[4]=10;
                Param[5]=10;
                Param[6] = 1;                      
                Param[7] = CouleurParComposante(255,128,128);         
                Param[8] = CouleurParComposante(255,255,0);
                Param[9] = CouleurParComposante(255, 0,255);
                Param[10] = CouleurParComposante(255,255,255);         
                Param[11] = CouleurParComposante(0,0,0);;
                Param[12] = CouleurParComposante(255,0,255);        
                Param[13] = CouleurParComposante(255,0,0); 

    /* Fond écran */
    c1=CouleurParComposante(20,120,20);
    EffacerEcran(c1);

/* ------------- Affichage des boutons et images ------------- */

    while (True) {

        /* Titre en haut de page */
        c2=CouleurParNom("black");
        ChoisirCouleurDessin(c2);
        EcrireTexte(600,60,"Snaky !",2);

        /* Bouton jouer */
        EcrireTexte(470,290,"Jouer",2);
        AfficherSprite(play_button, 405, 255);

        /* Bouton options */
        EcrireTexte(740,290,"Options",2);
        AfficherSprite(option_button, 675, 255);

        /* Bouton quitter */
        EcrireTexte(602,540,"Quitter",2);
        AfficherSprite(exit_button, 537, 505);

        /* Image du serpent */
        AfficherSprite(snake_icon_reverse, -20, 5);
        AfficherSprite(snake_icon, 675, 5);

        /* Contours des boutons */
        DessinerRectangle(400, 250, 190, 60);
        DessinerRectangle(670, 250, 190, 60);
        DessinerRectangle(532, 500, 190, 60);
    
/* ----------------- Changement de programme ----------------- */

        while(!SourisCliquee()) {}

        if ( _X > 400 && _X < 590 && _Y > 250 && _Y < 310 ) {
            /* Lancer le programme jeu */
            jeu(Param);
            EffacerEcran(c1);

        } else if ( _X > 670 && _X < 860 && _Y > 250 && _Y < 310 ) {
            /* Lancer les options */
            option(Param);

            EffacerEcran(c1);


            //FermerGraphique();

            //return EXIT_SUCCESS;
        } else if ( _X > 532 && _X < 722 && _Y > 500 && _Y < 560) {
            printf ("FIN\n");
            /* Quitter programme */
            FermerGraphique();

            return EXIT_SUCCESS;
        }

/* ----------------------------------------------------------- */

    }

    
}