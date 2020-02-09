#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <time.h>
#include "jeu.h"
#include "option.h"
#include "launcher.h"

/* Coordonnées du serpent */
typedef struct{
    	int x;
    	int y;
} Serpent;

typedef struct{
    	int x;
    	int y;
} objet_map;

/* Couleurs */	
int c_fond_page, c_fond_tableau, c_serpent, c_bordure;			/* Initialisation des couleurs */
/* Initialisation des variables */
int largeur_tableau = 53;										// Largeur de la zone de test
int longueur_tableau = 39;										// Longueur de la zone de test
int serpentMax;													// Longueur maximum du serpent
int ptDeb, ptFin;												// Nos "pointeurs" pour lire les coordonnées du serpent
int old_x, old_y;												// Anciennes valeurs de la tête en x et y
int incX = 0, incY = 0;											// Nouvelles valeurs de la tête en x et y
int sens_serpent=0;												// Défini le sens dans lequel se dirige le serpent
int tailleMax = 300;											// Taille maximale du serpent
int ptTab;														// "Pointeur" pour se déplacer dans le tableau du serpent
int vitesse = 20000000;											// Valeur de la vitesse du serpent
int vitesse_snake = 1;
int taillePixel = 10;											// Taille d'un pixel
Serpent serpent[300];											// Tableau du serpent (Taille 300)
/* Couleurs du snake */
int color[2];													// Une case du tableau par couleur
int ptCouleur = 0;												// On initialise la variable à 0
int c_serpent_1;												// Variable pour la première couleur du serpent
int c_serpent_2;												// Variable pour la seconde couleur du serpent
int taille_serpent = 19;										/* Taille du serpent affiché sur le côté */
int nbr_objet_max = 20;											/* Nombre d'objets (Pommes et Obstacles) */

objet_map pomme[20];
objet_map obstacle[20];

void init_snake() {
    c_serpent_1 = CouleurParComposante(000,000,000);		/* Couleur 1 du serpent */
    c_serpent_2 = CouleurParComposante(000,000,000);			/* Couleur 2 du serpent */


	color[0] = c_serpent_1;									/* On affecte aux valeurs les couleurs choisies */
    color[1] = c_serpent_2;

    c_fond_page=CouleurParComposante(20,120,20);					// Couleur du fond de la page
    c_fond_tableau=CouleurParNom("white");							// Couleur du fond de la zone de test
    c_serpent=CouleurParNom("yellow");								// Couleur du serpent
    c_bordure=CouleurParNom("gray");								// Couleur bordure

	/* Définir les coordonnées du serpent de base */    
    

	/* Définir le "pointeur" de début et de fin*/
   	ptDeb = 19;
    ptFin = 0;

}

void snake() {

		/* Afficher le serpent */
		ptTab = (ptFin + 19 - taille_serpent) % tailleMax;
		ptCouleur = 0;
		while(ptTab != ptDeb) {
			ChoisirCouleurDessin(color[ptCouleur]);																		// Couleur du serpent en fonction du tableau de couleur
			RemplirRectangle(serpent[ptTab].x*taillePixel+10+580, serpent[ptTab].y*taillePixel+10+90, 10, 10);			// On dessine le pixel
			ptTab = (ptTab + 1) % tailleMax;																			// On incrémente + 1, et si on est en bout de tableau, on fais %tailleMax pour revenir au début	
			ptCouleur = (ptCouleur+1)%2;					
		}
		
		/* Sauvegarde old_x old_y de la tête */
		old_x = serpent[ptDeb].x;
	   	old_y = serpent[ptDeb].y;


		/* Touches */
			/* > Modifier le sens */

	   	/* Sens de direction 
			 ---------------
			|	    3		|
			|	    ^		|
			|	2 <-+-> 0	|
			|	    v 		|
			|		1		|	
			 ---------------
		*/
		
	   	int touche = 0;
	   	int j=0;
	   	/* Tant qu'on appui pas sur une touche, rien ne se passe */
	   	if(ToucheEnAttente()) {
	   		touche = Touche();
	   	}

	   	/* Action des touches */
	   	if(touche == XK_Left) {			// Aller à gauche
	   		sens_serpent = 2;
	   	} 

	   	if(touche == XK_Up) {			// Aller en haut
	   		sens_serpent = 3;
	   	}

	   	if(touche == XK_Right) {		// Aller à droite
	   		sens_serpent = 0;
	   	}

	   	if(touche == XK_Down) {			// Aller en bas
	   		sens_serpent = 1;
	   	}

		/* Vérifier bords du tableau en fonction de la tête sauvegardée et son sens */
			/* > Changer le sens ou non */
		if (old_y == 0 && old_x == 0 && sens_serpent == 2) {														// Si le serpent touche le bord de gauche avec le sens 2 en haut
			sens_serpent = 0;

		} else if (old_y != longueur_tableau && old_x == largeur_tableau && sens_serpent == 0) {					// Si le serpent touche le bord de droite avec le sens 0 en haut
			sens_serpent = 1;
 
 		} else if (old_x == largeur_tableau && old_y == 0 && sens_serpent == 3) {									// Si le serpent touche le bord du haut avec le sens 3 à droite
			sens_serpent = 1;

		} else if (old_x != 0 && old_y == longueur_tableau && sens_serpent == 1) {									// Si le serpent touche le bord du bas avec le sens 1 à droite
			sens_serpent = 2;

		} else if (old_y == longueur_tableau && old_x == largeur_tableau && sens_serpent == 0) {					// Si le serpent touche le bord droit avec le sens 0 en bas
			sens_serpent = 2;

		} else if (old_x == 0 && sens_serpent == 2) {																// Si le serpent touche le bord de gauche avec le sens 2 en bas
			sens_serpent = 3;

		} else if (old_y == longueur_tableau && sens_serpent == 1) {												// Si le serpent touche le bord du bas avec le sens 1 à gauche
			sens_serpent = 3;

		} else if (old_x != largeur_tableau && old_y == 0 && sens_serpent == 3) {									// Si le serpent touche le bord du haut avec le sens 3 à gauche
			sens_serpent = 0;	
		}

		/* En fonction du sens : new_x et new_y grâce à old_x et old_y */
		incX = 0;		// Paramètres d'incrémentation
		incY = 0;

		if (sens_serpent == 0) { 			// Si le serpent va à droite : Coordonnées de X + 1
			incX = 1;
		} else if (sens_serpent == 1) {		// Si le serpent va en bas : Coordonnées de Y + 1
			incY = 1;
		} else if (sens_serpent == 2) {		// Si le serpent va à gauche : Coordonnées de X - 1
			incX = - 1;
		} else if (sens_serpent == 3) {		// Si le serpent va en haut : Coordonnées de Y - 1
			incY = - 1;
		}

		/* Incrémente ptDeb et ptFin (%tailleMax) */
	   	ptDeb = (ptDeb + 1)%tailleMax;					// On passe à la valeur suivante du tableau (On fais un % dans le cas où on arrive en fin de tableau)
	   	ptFin = (ptFin + 1)%tailleMax;

		serpent[ptDeb].x = old_x + incX;				// On modifie les nouvelles coordonnées de la tête
	   	serpent[ptDeb].y = old_y + incY;
		
		/* Délai avant de recommencer */    
		for(j = 0; j <= vitesse; j++)					// On fais une boucle pour modifier le temps entre chaque boucle "while"
			;				
}

void dessiner_terrain_menu_option(couleur c1_vide,couleur c2_vide,couleur c3_vide,couleur hauteur,couleur longueur){
	int i = 0, j = 0;
	ChoisirEcran(2);
	for ( i = 0 ; i < longueur ; i++ )							/* longueur */
		{
			for ( j = 0 ; j < hauteur ; j++ )					/* hauteur */
			{
				if ( i%2 == 0 && j%2 == 0)						/* longueur paire && hauteur paire */
				{
					ChoisirCouleurDessin(c1_vide);
					RemplirRectangle(10*i,10*j,10,10);
				}
				else if ( i%2 == 0 && j%2 == 1)					/* longueur paire && hauteur impaire */
				{
					ChoisirCouleurDessin(c2_vide);
					RemplirRectangle(10*i,10*j,10,10);
				}
				else if ( i%2 == 1 && j%2 == 0)					/* longueur impaire && hauteur paire */
				{
					ChoisirCouleurDessin(c2_vide);
					RemplirRectangle(10*i,10*j,10,10);
				}
				else											/* longueur impaire && hauteur impaire */
				{
					ChoisirCouleurDessin(c1_vide);
					RemplirRectangle(10*i,10*j,10,10);
				}
				ChoisirCouleurDessin(c3_vide);					/* Quadrillage */
				DessinerRectangle(10*i,10*j,10,10);
		   	}
		}
	ChoisirEcran(0);
}

int option(int *Param) {



	/* Initialisation de la fenêtre */
    InitialiserGraphique();
    CreerFenetre(0,0,1250,600);

    time_t t;

    srand((unsigned) time(&t));									/* Réinitialiser compteur aléatoire */

    init_snake();	

    /* Initialisation des variables */
    couleur c_fenetre, c_texte, c_couleur, c_bord,c_bordure;	/* Variables des couleurs */
    int nbr_pomme = 5;											/* Valeur par défaut */
    int nbr_obstacle = 5;										/* Valeur par défaut */
    couleur c_fond_base; 										/* Couleur de fond par défaut */
    couleur c_pomme_base;										/* Couleur des pommes par défaut */
    c_pomme_base=CouleurParComposante(220,20,60);
    couleur c_fond_new = CouleurParComposante(000,255,000);		/* Couleur de fond choisie */
    c_bordure=CouleurParComposante(105,105,105);				/* Couleur des bordures */
    c_bord=CouleurParComposante(000,000,000);					/* Couleur du bord des rectangles du menu */
    c_texte=CouleurParComposante(000,000,000);					/* Couleur du texte */
    
    int c_pomme_new;											/* Couleur des pommes choisie */

    int largeur_select = 1, longueur_select = 0;									
	int nbr_pomme_select = 1, nbr_obstacle_select = 0, compteur_pomme_obstacle = 0;
	int c_tab1_select = 1, c_tab2_select = 0, c_tab3_select = 0;
	int c_serpent_1_select = 0, c_serpent_2_select = 0;
	int c_pomme_select = 0, c1_obstacle_select = 0;
	

	// REMPLIR
	nbr_pomme = Param[2];
	// REMPLIR


    couleur c1_vide = CouleurParComposante(000,105,000);		/*	Case vide */
    couleur c2_vide = CouleurParComposante(000,100,000);		/* 	Si on veut mettre 2 couleurs de fond */
    couleur c3_vide = CouleurParComposante(000,100,000);		/* 	Si on veut mettre un cadrillage (pour l'enlever on le met de la même couleur que c1 ou c2) */

    couleur c1_serpent = CouleurParComposante(000,000,000);		/* Case serpent */
    couleur c2_serpent = CouleurParComposante(100,100,000);

    couleur c1_obstacle = CouleurParComposante(050,050,050);	/* Case obstacle */

    couleur c1_pomme = CouleurParComposante(255,000,000);		/* Case serpent */

    couleur selectionner = CouleurParComposante(255,255,000);
    int taille_pixel_5_select = 0 , taille_pixel_10_select = 1, taille_pixel_20_select = 0;
    int taille_pixel =10;
    double largeur_longueur_echelle_taille_pixel = 1;
    int largeur_valeur_2 = 20, longueur_valeur_2=30;
    int new_color = -1;

    int longueur = 54, hauteur = 40, i=0, j=0, k=0, couleur_terrain_changer = 1;
    int** tableau_terrain=malloc(sizeof(int*)*longueur);	/* Longeur soit en x */
    for ( i = 0 ; i < longueur ; i++ )
    {
    	tableau_terrain[i]=malloc(sizeof(int)*hauteur);	/* Hauteur soit en y */
    }
    
    int largeur_valeur = 40, longueur_valeur = 60;
    char largeur_longueur_valeur_afficher[50];

    /* Fond écran */
    c_fenetre=CouleurParComposante(20,120,20);
    EffacerEcran(c_fenetre);

    for(k = 0; k < nbr_objet_max; k++) {
		pomme[k].x = (rand()%largeur_tableau)+1;
		pomme[k].y = (rand()%longueur_tableau)+1;
		obstacle[k].x = (rand()%largeur_tableau)+1;
		obstacle[k].y = (rand()%longueur_tableau)+1;
	}

    while (True) {

    	snake();
    	
    	/* Titre + couleur texte en haut de page */
	    ChoisirCouleurDessin(c_texte);
	    EcrireTexte(600,60,"Options",2);

	    /* Cliquer dans la zone de taille correspondante : */

	    /* --------------- Taille de la grille --------------- */

	    /* Taille pixel*/
	    ChoisirCouleurDessin(c_texte);
	    EcrireTexte(20,50,"Taille pixel :",1);

	    EcrireTexte(150,50,"5",1);
	    ChoisirCouleurDessin(c_fenetre); // on initialise
	    RemplirRectangle(140,45, 5, 5); //5
	    RemplirRectangle(235,40, 10, 10); //10
	    RemplirRectangle(325,35, 20, 20); //20
	    ChoisirCouleurDessin(c_texte);
	    if (taille_pixel_5_select)
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(140,45, 5, 5);
	    	ChoisirCouleurDessin(c_texte);
	    }
	    DessinerRectangle(140,45, 5, 5);
	    //DessinerRectangle(140,30, 30, 25); //5
	    EcrireTexte(250,50,"10",1);
	    if (taille_pixel_10_select)
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(235,40, 10, 10);
	    	ChoisirCouleurDessin(c_texte);
	    }
	    DessinerRectangle(235,40, 10, 10);
	    //DessinerRectangle(235,30, 40, 25); //10
	    EcrireTexte(350,50,"20",1);
	    if (taille_pixel_20_select)
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(325,35, 20, 20);
	    	ChoisirCouleurDessin(c_texte);
	    }
	    DessinerRectangle(325,35, 20, 20);
	    //DessinerRectangle(325,30, 50, 30); //20

	    /* Longueur et largeur*/
	    if (largeur_select)
	    {
	    	ChoisirCouleurDessin(selectionner); //largeur
	    	RemplirRectangle(9, 80, 90, 25);
	    	ChoisirCouleurDessin(c_fenetre); // longueur
	    	RemplirRectangle(109, 80, 90, 25);
	    }
	    else
	    {
	    	ChoisirCouleurDessin(c_fenetre); // largeur
	    	RemplirRectangle(9, 80, 90, 25);
	    	ChoisirCouleurDessin(selectionner); //longueur
	    	RemplirRectangle(109, 80, 90, 25);
	    }
	    
	    ChoisirCouleurDessin(c_texte);
	    EcrireTexte(20,100,"Largeur",1);
	    EcrireTexte(120,100,"Longueur",1);
	    DessinerRectangle(9, 80, 90, 25);
	    DessinerRectangle(109, 80, 90, 25);

	    ChoisirCouleurDessin(selectionner);
	    RemplirRectangle(9, 105, 460, 35);
		ChoisirCouleurDessin(c_texte);
	    EcrireTexte(50,130,"Petite",1);
	    EcrireTexte(150,130,"Moyenne",1);
	    EcrireTexte(250,130,"Grande",1);
	    EcrireTexte(350,130,"Gigantesque !",1);
	   	DessinerRectangle(9, 105, 460, 35);

	   	

	   	/* -------- J'affiche la taille en chiffre ---------- */	   
	   	sprintf(largeur_longueur_valeur_afficher,"Largeur: %d || Longueur: %d",largeur_valeur,longueur_valeur);
	   	
	   	ChoisirCouleurDessin(c_fenetre);
	   	RemplirRectangle(220,80,250,23); 									/* Pour eviter la superposition je cache avec un rectangle */
	   	ChoisirCouleurDessin(c_texte);
	   	EcrireTexte(220,100,largeur_longueur_valeur_afficher,1);

	    /* ---------------- Pommes et obstacles ---------------- */
	    if (nbr_obstacle_select)
	    {
	    	ChoisirCouleurDessin(selectionner); //obstacle
	    	RemplirRectangle(109, 150, 90, 25);
	    	ChoisirCouleurDessin(c_fenetre);	//pomme
	    	RemplirRectangle(9, 150, 90, 25);
	    }
	    else
	    {
	    	ChoisirCouleurDessin(c_fenetre); // obstacle
	    	RemplirRectangle(109, 150, 90, 25);
	    	ChoisirCouleurDessin(selectionner); //pomme
	    	RemplirRectangle(9, 150, 90, 25);
	    }
	    ChoisirCouleurDessin(c_texte);
	    EcrireTexte(20,170,"Pommes",1);
	    EcrireTexte(115,170,"Obstacles",1);
		DessinerRectangle(9, 150, 90, 25);
	    DessinerRectangle(109, 150, 90, 25);

	    ChoisirCouleurDessin(selectionner);
	    RemplirRectangle(9, 175, 460, 35);
	    ChoisirCouleurDessin(c_texte);
	    EcrireTexte(70,200,"5",1);
	    EcrireTexte(170,200,"10",1);
	    EcrireTexte(270,200,"15",1);
	    EcrireTexte(370,200,"20",1);
	    EcrireTexte(440,200,"0",1);
	   	DessinerRectangle(9, 175, 460, 35);

	    /* ----------- Taille initiale du serpent ----------- */
	    EcrireTexte(10,240,"Taille du serpent :",1);
	    EcrireTexte(50,270,"Petit",1);									/* 2 */
	    EcrireTexte(150,270,"Moyen",1);									/* 10 */
	    EcrireTexte(250,270,"Grand",1);									/* 15 */
	    EcrireTexte(350,270,"ENORME !",1);								/* 20 */

	    /* ---------- Vitesse initiale du serpent ---------- */
	    EcrireTexte(10,310,"Vitesse du serpent :",1);
	    EcrireTexte(50,340,"Lent",1);
	    EcrireTexte(150,340,"Normal",1);
	    EcrireTexte(250,340,"Rapide",1);
	    EcrireTexte(350,340,"SONIC !!!!",1);

	    /* -------------- Couleurs -------------- */
	   /* --------------- On initialise les onglets ---------------*/
		ChoisirCouleurDessin(c_fenetre);
		RemplirRectangle(9, 360, 80, 25); 	// tableau c1
		RemplirRectangle(89, 360, 30, 25); 	// tableau c2
		RemplirRectangle(119, 360, 20, 25);	// tableau c3
		RemplirRectangle(160, 360, 80, 25); // Serpent c1
		RemplirRectangle(240, 360, 30, 25);	// Serpent c2
		RemplirRectangle(290, 360, 80, 25);	// Pomme
		RemplirRectangle(390, 360, 90, 25); // Obstacle
		/* --------------- On met en valeur celui qui est selectionné ---------------*/
	    if (c_tab1_select)							// tableau
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(9, 360, 80, 25);
	    }
	    if (c_tab2_select)
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(89, 360, 30, 25);
	    }
	    if (c_tab3_select)
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(119, 360, 20, 25);
	    	
	    }
	    if (c_serpent_1_select)						// Serpent
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(160, 360, 80, 25);
	    }
	    if (c_serpent_2_select)						// Serpent
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(240, 360, 30, 25);
	    }
	    if (c_pomme_select)							// pommes
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(290, 360, 80, 25);
	    }
	    if (c1_obstacle_select)							// obstacles
	    {
	    	ChoisirCouleurDessin(selectionner);
	    	RemplirRectangle(390, 360, 90, 25);
	    }
	
	    ChoisirCouleurDessin(c_texte);
	    EcrireTexte(12,380, "Tableau 1",1);
	    EcrireTexte(100,380, "2",1);
	    EcrireTexte(125,380, "3",1);
	    EcrireTexte(165,380, "Serpent 1",1);
	    EcrireTexte(255,380, "2",1);
	    EcrireTexte(295,380, "Pommes",1);
	    EcrireTexte(395,380, "Obstacles",1);

	    /* Grand rectangle (le contour et l'interrieur) */
		ChoisirCouleurDessin(selectionner);
		RemplirRectangle(9, 385, 480, 200);
		ChoisirCouleurDessin(c_texte);
	    DessinerRectangle(9, 385, 480, 200);
	    /* Terrain */
	    DessinerRectangle(9, 360, 80, 25);
	    DessinerRectangle(89, 360, 30, 25);
	    DessinerRectangle(119, 360, 20, 25);
	 	/* Serpent */
	 	DessinerRectangle(160, 360, 80, 25);
	    DessinerRectangle(240, 360, 30, 25);
	    /* Pommes */
	    DessinerRectangle(290, 360, 80, 25);
	    /* Obstacles */
	    DessinerRectangle(390, 360, 90, 25);

	    /* Violet */
	    c_couleur=CouleurParComposante(106,90,205);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(20, 410, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(20, 410, 50, 50);

	    /* Bleu */
	    c_couleur=CouleurParComposante(0,191,255);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(120, 410, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(120, 410, 50, 50);

	    /* Vert */
	    c_couleur=CouleurParComposante(0,255,0);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(220, 410, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(220, 410, 50, 50);

	    /* Jaune */
	    c_couleur=CouleurParComposante(255,255,000);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(320, 410, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(320, 410, 50, 50);

	    /* Blanc */
	    c_couleur=CouleurParComposante(255,255,255);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(420, 410, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(420, 410, 50, 50);

	    /* Violet foncé*/
	    c_couleur=CouleurParComposante(72,61,139);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(20, 520, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(20, 520, 50, 50);

	    /* Bleu foncé*/
	    c_couleur=CouleurParComposante(65,105,225);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(120, 520, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(120, 520, 50, 50);

	    /* Vert foncé*/
	    c_couleur=CouleurParComposante(85,107,47);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(220, 520, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(220, 520, 50, 50);

	    /* Rouge*/
	    c_couleur=CouleurParComposante(255,000,000);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(320, 520, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(320, 520, 50, 50);

	    /*blanc*/
	    c_couleur=CouleurParComposante(000,000,000);
	    ChoisirCouleurDessin(c_couleur);
	    RemplirRectangle(420, 520, 50, 50);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(420, 520, 50, 50);    

	    //_____________________________________________________________________________________________________________________________________
	    //_____________________________________________________________________________________________________________________________________
	    



	    EcrireTexte(900,550,"Retourner au menu principal",1);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(890, 525, 240, 35);

	    /* -------------- Remise à zéro -------------- */

	    EcrireTexte(900,60,"Valeurs par defaut",1);
	    ChoisirCouleurDessin(c_bord);
	    DessinerRectangle(890, 40, 170, 27);

	    /* -------------- Zone d'affichage des modifications -------------- */

	    /* Affichage de la zone de test */
	    
	    ChoisirCouleurDessin(c_bordure);
	    RemplirRectangle(580, 91, 561, 421);

		if (couleur_terrain_changer) {
	    	dessiner_terrain_menu_option(c1_vide,c2_vide,c3_vide,hauteur,longueur);
		    couleur_terrain_changer = 0;
	    }

	    CopierZone(2,0,0,0,longueur*10,hauteur*10,590,100);				/* Pour afficher le terrain */

	    /* Pommes aléatoires */

	    
	    ChoisirCouleurDessin(c_pomme_base);
	    int taille_pixel_dessin = 10;

	    for(k = 0; k < nbr_pomme; k++) {
	    	RemplirArc(10*pomme[k].x+580, 10*pomme[k].y+90, taille_pixel_dessin, taille_pixel_dessin, 0, 360);
	    }

	    /* Obstacles aléatoires */

	    ChoisirCouleurDessin(c_bordure);
	    for(k = 0; k < nbr_obstacle; k++) {
	    	RemplirRectangle(10*obstacle[k].x+580, 10*obstacle[k].y+90, taille_pixel_dessin, taille_pixel_dessin);
	    }

	    /* Afficher le serpent sur le côté */

	    if(taille_serpent >= 2) {
	    	ChoisirCouleurDessin(c_fenetre);
	    	RemplirRectangle(1175, 140, 20, 2000);

		    ChoisirCouleurDessin(c_serpent_1);
		    RemplirRectangle(1175, 100, 20, 20);
		    ChoisirCouleurDessin(c_serpent_2);
		    RemplirRectangle(1175, 120, 20, 20);

		    if(taille_serpent >= 10) {
		    	ChoisirCouleurDessin(c_fenetre);
	    		RemplirRectangle(1175, 300, 20, 2000);

		    	ChoisirCouleurDessin(c_serpent_1);
		    	RemplirRectangle(1175, 140, 20, 20);
			    RemplirRectangle(1175, 180, 20, 20);
			    RemplirRectangle(1175, 220, 20, 20);
			    RemplirRectangle(1175, 260, 20, 20);
			    ChoisirCouleurDessin(c_serpent_2);
		    	RemplirRectangle(1175, 160, 20, 20);
			    RemplirRectangle(1175, 200, 20, 20);
			    RemplirRectangle(1175, 240, 20, 20);
			    RemplirRectangle(1175, 280, 20, 20);

		    	if(taille_serpent >= 15) {
		    		ChoisirCouleurDessin(c_fenetre);
	    			RemplirRectangle(1175, 460, 20, 2000);

		    		ChoisirCouleurDessin(c_serpent_1);
		    		RemplirRectangle(1175, 300, 20, 20);
		    		RemplirRectangle(1175, 340, 20, 20);
		    		RemplirRectangle(1175, 380, 20, 20);
		    		ChoisirCouleurDessin(c_serpent_2);
			   		RemplirRectangle(1175, 320, 20, 20);
		    		RemplirRectangle(1175, 360, 20, 20);

		    		if(taille_serpent >= 20) {
		    			ChoisirCouleurDessin(c_serpent_1);
		    			RemplirRectangle(1175, 460, 20, 20);
		    			RemplirRectangle(1175, 420, 20, 20);
		    			ChoisirCouleurDessin(c_serpent_2);
		    			RemplirRectangle(1175, 400, 20, 20);
			    		RemplirRectangle(1175, 440, 20, 20);
			    		RemplirRectangle(1175, 480, 20, 20);
		    		}
		    	}
		    }
		}

	    /*
		int ptAfficher_Serpent = 0;
		ptCouleur = 0;
		for(int y = 7; y < taille_serpent+7; y++) {
			ChoisirCouleurDessin(color[ptCouleur]);
			RemplirRectangle(1175, y*20, 20,20);
			ptCouleur = (ptCouleur+1) % 2;
		}
		*/

	    /* -------------- Interactions lors du clic -------------- */

	    if (SourisCliquee()) {
	    	/* On choisi la taille du pixel */
	    	//DessinerRectangle(140,30, 30, 25); //5
		    //DessinerRectangle(235,30, 40, 25); //10
		    //DessinerRectangle(325,30, 50, 30); //20
		    //________________________________________________________________________________________________________________________________
		    //________________________________________________________________________________________________________________________________

		    if(_X > 140 && _Y > 30 && _X < 170 && _Y < 55) { // Taille pixel 5
	    		taille_pixel_5_select = 1;
	    		taille_pixel_10_select = 0;
	    		taille_pixel_20_select = 0;

	    		taille_pixel = 5;
	    	}
	    	if(_X > 235 && _Y > 30 && _X < 275 && _Y < 55) { // Taille pixel 10
	    		taille_pixel_5_select = 0;
	    		taille_pixel_10_select = 1;
	    		taille_pixel_20_select = 0;

	    		taille_pixel = 10;
	    	}

	    	if(_X > 325 && _Y > 30 && _X < 375 && _Y < 60) { // Taille pixel 10
	    		taille_pixel_5_select = 0;
	    		taille_pixel_10_select = 0;
	    		taille_pixel_20_select = 1;

	    		taille_pixel = 20;
	    	}

	    	/* On choisi entre largeur et longueur */

	   		if(_X > 9 && _Y > 80 && _X < 99 && _Y < 105) {
	    		largeur_select = 1;
	    		longueur_select = 0;
	    	}
	    	if(_X > 109 && _Y > 80 && _X < 199 && _Y < 105) {
	    		largeur_select = 0;
	    		longueur_select = 1;
	    	}

	    	/* On attribut les valeurs de la largeur et de la longueur */
	    	if (taille_pixel == 5)
	    		largeur_longueur_echelle_taille_pixel = 4;
	    	if (taille_pixel == 10)
	    		largeur_longueur_echelle_taille_pixel = 2;
	    	if (taille_pixel == 20)
	    		largeur_longueur_echelle_taille_pixel = 1;

	    	if (largeur_select)
	    	{
	    		if (_X > 45 && _Y > 117 && _X < 90 && _Y < 132)
	    			largeur_valeur_2 = 10;
	    		if (_X > 145 && _Y > 117 && _X < 200 && _Y < 132)
	    			largeur_valeur_2 = 15;
	    		if (_X > 245 && _Y > 117 && _X < 295 && _Y < 132)
	    			largeur_valeur_2 = 20;
	    		if (_X > 345 && _Y > 117 && _X < 435 && _Y < 132)
	    			largeur_valeur_2 = 25;
	    	}
	    	if (longueur_select)
	    	{
	    		if (_X > 45 && _Y > 117 && _X < 90 && _Y < 132)
	    			longueur_valeur_2 = 20;
	    		if (_X > 145 && _Y > 117 && _X < 200 && _Y < 132)
	    			longueur_valeur_2 = 30;
	    		if (_X > 245 && _Y > 117 && _X < 295 && _Y < 132)
	    			longueur_valeur_2 = 40;
	    		if (_X > 345 && _Y > 117 && _X < 435 && _Y < 132)
	    			longueur_valeur_2 = 50;
	    	}

	    	//largeur_valeur_2 = 40, longueur_valeur_2=60;
	    	largeur_valeur = largeur_valeur_2 *largeur_longueur_echelle_taille_pixel;
	    	longueur_valeur = longueur_valeur_2 * largeur_longueur_echelle_taille_pixel;
		    //________________________________________________________________________________________________________________________________
		    //________________________________________________________________________________________________________________________________


	    	/* Vitesse */
			if (_X > 45 && _Y > 325 && _X < 85 && _Y < 345) {
				vitesse = 100000000;
				vitesse_snake = 1;
				snake(vitesse);
			}

	    	if (_X > 145 && _Y > 325 && _X < 190 && _Y < 345) {
				vitesse = 20000000;
				vitesse_snake = 4;
				snake(vitesse);
			}

	    	if (_X > 245 && _Y > 325 && _X < 295 && _Y < 345) {
				vitesse = 10000000;
				vitesse_snake = 6;
				snake(vitesse);
			}

	    	if (_X > 345 && _Y > 325 && _X < 435 && _Y < 345) {
				vitesse = 5000000;
				vitesse_snake = 9;
				snake(vitesse);
			}

			/* Affichage test 
	    	DessinerRectangle(45,325,40,20);
	    	DessinerRectangle(145,325,45,20);
	    	DessinerRectangle(245,325,50,20);
	    	DessinerRectangle(345,325,90,20);
			*/

	    	/* Si on sélectionne le nombre de pommes */
	    	if(_X > 9 && _Y > 150 && _X < 99 && _Y < 175) {
	    		nbr_obstacle_select = 0;
	    		nbr_pomme_select = 1;
	    	}
	    	/* Si on sélectionne le nombre d'obstacles */
	    	if(_X > 109 && _Y > 150 && _X < 199 && _Y < 175) {
	    		nbr_obstacle_select = 1;
	    		nbr_pomme_select = 0;
	    	}

	    	//RemplirRectangle(9, 360, 89, 385);

	    	/*On initialise*/
	    	
	    	/*On choisie celle où on a cliquée*/
	    	if(_X > 9 && _Y > 360 && _X < 89 && _Y < 385) { /* Si on sélectionne la couleur du tableau 1 */	
	    		c_tab1_select = 1;
		    	c_tab2_select = 0;
		    	c_tab3_select = 0;
		    	c_serpent_1_select = 0;
		    	c_serpent_2_select = 0;
		    	c_pomme_select = 0;
		    	c1_obstacle_select = 0;
	    	}
	    	if(_X > 89 && _Y > 360 && _X < 119 && _Y < 385) { /* Si on sélectionne la couleur du tableau 2 */
	    		c_tab1_select = 0;
	    		c_tab2_select = 1;
		    	c_tab3_select = 0;
		    	c_serpent_1_select = 0;
		    	c_serpent_2_select = 0;
		    	c_pomme_select = 0;
		    	c1_obstacle_select = 0;
	    	}
	    	if(_X > 119 && _Y > 360 && _X < 139 && _Y < 385) { /* Si on sélectionne la couleur du tableau 3 */
	    		c_tab1_select = 0;
	    		c_tab2_select = 0;
	    		c_tab3_select = 1;
	    		c_serpent_1_select = 0;
		    	c_serpent_2_select = 0;
		    	c_pomme_select = 0;
		    	c1_obstacle_select = 0;
	    	}
	    	if(_X > 160 && _Y > 360 && _X < 240 && _Y < 385) { /* Si on sélectionne la couleur du serpent 1 */
	    		c_tab1_select = 0;
	    		c_tab2_select = 0;
	    		c_tab3_select = 0;
	    		c_serpent_1_select = 1;
	    		c_serpent_2_select = 0;
		    	c_pomme_select = 0;
		    	c1_obstacle_select = 0;
	    	}
	    	if(_X > 240 && _Y > 360 && _X < 270 && _Y < 385) { /* Si on sélectionne la couleur du serpent 2 */
	    		c_tab1_select = 0;
	    		c_tab2_select = 0;
	    		c_tab3_select = 0;
	    		c_serpent_1_select = 0;
	    		c_serpent_2_select = 1;
	    		c_pomme_select = 0;
		    	c1_obstacle_select = 0;
	    	}
	    	if(_X > 290 && _Y > 360 && _X < 370 && _Y < 385) { /* Si on sélectionne la couleur de la pomme */
	    		c_tab1_select = 0;
	    		c_tab2_select = 0;
	    		c_tab3_select = 0;
	    		c_serpent_1_select = 0;
	    		c_serpent_2_select = 0;
	    		c_pomme_select = 1;
	    		c1_obstacle_select = 0;
	    	}
	    	if(_X > 390 && _Y > 360 && _X < 480 && _Y < 385) { /* Si on sélectionne la couleur de l'obstacle */
	    		c_tab1_select = 0;
	    		c_tab2_select = 0;
	    		c_tab3_select = 0;
	    		c_serpent_1_select = 0;
	    		c_serpent_2_select = 0;
	    		c_pomme_select = 0;
	    		c1_obstacle_select = 1;
	    	}

	    	/* Valeur par défaut */
	    	if ( _X > 890 && _X < 1060 && _Y > 40 && _Y < 67) {
	    		longueur_valeur= 60;				// max mon ecran 122 pour taille_pixel 10
				largeur_valeur = 40;				// max mon ecran 55 pour taille_pixel 10
				nbr_pomme = 5;
				nbr_obstacle = 0;
				taille_serpent = 10;
				taille_pixel = 10;
				vitesse = 20000000;									// Vitesse du serpent (entre 0 et 10) Remarque: Une valeur négative le fera juste ralentir
				c1_vide = CouleurParComposante(000,105,000);		//	Case vide
			    c2_vide = CouleurParComposante(000,100,000);		// 	Si on veut mettre 2 couleurs de fond
			    c3_vide = CouleurParComposante(000,100,000);		// 	Si on veut mettre un cadrillage (pour l'enlever on le met de la même couleur que c1 ou c2)
			    c1_serpent = CouleurParComposante(000,000,000);		//	Case serpent
			    c2_serpent = CouleurParComposante(000,000,000);
			    c1_obstacle = CouleurParComposante(050,050,050);	//	Case obstacle
			    c1_pomme = CouleurParComposante(255,000,000);		//	Case serpent


			    color[1] = c1_serpent; c_serpent_1 = c1_serpent;
			    color[0] = c2_serpent; c_serpent_2 = c2_serpent;
			    c_pomme_base = c1_pomme;
			    c_bordure = c1_obstacle;
			    taille_pixel_5_select = 0;
			    taille_pixel_10_select = 1;
			    taille_pixel_20_select = 0;
			    longueur_valeur_2 = 30;
			    largeur_valeur_2 = 20;
			    couleur_terrain_changer = 1;
	    		}

	    	if (nbr_pomme_select)
	    	{
	    		if ( _X > 60 && _X < 85 && _Y > 180 && _Y < 205) {
	  				nbr_pomme = 5;
		    	}

		    	if ( _X > 165 && _X < 190 && _Y > 180 && _Y < 205) {
	  				nbr_pomme = 10;
		    	}
		    	
		    	if ( _X > 265 && _X < 290 && _Y > 180 && _Y < 205) {
	  				nbr_pomme = 15;
		    	}

		    	if ( _X > 365 && _X < 390 && _Y > 180 && _Y < 205) {
	  				nbr_pomme = 20;
		    	}

	    	}

	    	/* Taille du serpent */
	    	if(_X > 49 && _Y > 250 && _X < 94 && _Y < 275) {
	    		taille_serpent = 2;
	    	}

			if(_X > 149 && _Y > 250 && _X < 204 && _Y < 275) {
	    		taille_serpent = 10;
	    	}

	    	if(_X > 249 && _Y > 250 && _X < 343 && _Y < 275) {
	    		taille_serpent = 15;
	    	}

	    	if(_X > 349 && _Y > 250 && _X < 443 && _Y < 275) {
	    		taille_serpent = 20;
	    	}

	    	/* Couleurs */

	    	new_color = -1;  /* On réinitialise à chaque tour pour ne pas avoir de problèmes de couleurs */

	    	if (_X > 20 && _X < 70 && _Y > 410 && _Y < 460)	{new_color=CouleurParComposante(72,61,139);} // 1ere ligne

	    	if (_X > 120 && _X < 170 && _Y > 410 && _Y < 460) {new_color=CouleurParComposante(0,191,255);}

	    	if (_X > 220 && _X < 270 && _Y > 410 && _Y < 460) {new_color=CouleurParComposante(000,255,000);}

	    	if (_X > 320 && _X < 370 && _Y > 410 && _Y < 460) {new_color=CouleurParComposante(255,255,000);}

	    	if (_X > 420 && _X < 470 && _Y > 410 && _Y < 460) {new_color=CouleurParComposante(255,255,255);}

	    	if (_X > 20 && _X < 70 && _Y > 520 && _Y < 570) {new_color=CouleurParComposante(75, 0, 130);}	//2e ligne
	    	
	    	if (_X > 120 && _X < 170 && _Y > 520 && _Y < 570) {new_color=CouleurParComposante(65,105,225);}

	    	if (_X > 220 && _X < 270 && _Y > 520 && _Y < 570) {new_color=CouleurParComposante(85,107,47);}

	    	if (_X > 320 && _X < 370 && _Y > 520 && _Y < 570) {new_color=CouleurParComposante(255,000,000);}

	    	if (_X > 420 && _X < 470 && _Y > 520 && _Y < 570) {new_color=CouleurParComposante(000,000,000);}
			
	    	if(c_tab1_select == 1 && new_color != -1) {  c1_vide = new_color; couleur_terrain_changer = 1; }
	    	if(c_tab2_select == 1 && new_color != -1) {  c2_vide = new_color; couleur_terrain_changer = 1; }
	    	if(c_tab3_select == 1 && new_color != -1) {  c3_vide = new_color; couleur_terrain_changer = 1; }
	    	if(c_serpent_1_select == 1 && new_color != -1) { color[1] = new_color; c_serpent_1 = new_color; c1_serpent = new_color; }
	    	if(c_serpent_2_select == 1 && new_color != -1) { color[0] = new_color; c_serpent_2 = new_color; c2_serpent = new_color;}
	    	if(c_pomme_select == 1 && new_color != -1) { c_pomme_base = new_color; c1_pomme = new_color; }
	    	if(c1_obstacle_select == 1 && new_color != -1) { c_bordure = new_color; c1_obstacle = new_color; }
	    	


	    }


			if ( _X > 890 && _X < 1130 && _Y > 525 && _Y < 560 ) {
		        /* Retour au menu principal */
	    		/* Donc on remet les valeurs par défaut car on ne sauvegarde pas */
	    		/* On retourne à la première page */
		        //main();
		        

				Param[0]=longueur_valeur;
				Param[1]=largeur_valeur;
				Param[2]=nbr_pomme;
				Param[3]=nbr_obstacle;
				Param[4]=taille_serpent;
				Param[5]=taille_pixel;
	

				Param[6] = vitesse_snake;						 
				
			    Param[7] = c1_vide;			
			    Param[8] = c2_vide;			
			    Param[9] = c3_vide;		 	

			    Param[10] = c1_serpent;			
			    Param[11] = c2_serpent;

			    Param[12] = c1_obstacle;		

			    Param[13] = c1_pomme;	
			
              


			    return 0;

	    }


	}
	return 0;
}

/* Pour les couleurs : http://www.css-faciles.com/couleurs-css.php */	