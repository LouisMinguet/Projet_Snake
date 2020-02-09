#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "jeu.h"
#include "option.h"
#include "launcher.h"
/*#include <windows.h>*/

/*#define CYCLE_BOUCLE_JEU_SUIVANTE 100000L*/
#define CYCLE_BOUCLE_JEU_SUIVANTE 130000L
#define CYCLE_SECONDE 1000000L


enum valeur_case	/* Valeur que prendre une case du tableau terrain*/
{
	vide,
	pommeT,
	obstacle
};


struct pomme 		/* Coordonnées d'une pomme*/
{
	int x_pomme;
	int y_pomme;
};

struct obstacle 		/* Coordonnées d'un obstacle*/
{
	int x_obstacle;
	int y_obstacle;
};

struct serpent 		/* Coordonnées du serpent (déterminé par celle de la tête)*/
{
	int x_tete_serpent;
	int y_tete_serpent;
};

/*-------------------------------------------------------------------------------------------------------------------------------------*/

void vide_dessin(int x, int y, couleur c1, couleur c2, couleur c3, int taille_pixel){
	
	if ( x%2 == 0 && y%2 == 0)				/* longueur paire && hauteur paire*/
	{
		ChoisirCouleurDessin(c1);
		RemplirRectangle(taille_pixel*x,taille_pixel*y,taille_pixel,taille_pixel);
	}
	else if ( x%2 == 0 && y%2 == 1)			/* longueur paire && hauteur impaire*/
	{
		ChoisirCouleurDessin(c2);
		RemplirRectangle(taille_pixel*x,taille_pixel*y,taille_pixel,taille_pixel);
	}
	else if ( x%2 == 1 && y%2 == 0)			/* longueur impaire && hauteur paire*/
	{
		ChoisirCouleurDessin(c2);
		RemplirRectangle(taille_pixel*x,taille_pixel*y,taille_pixel,taille_pixel);
	}
	else									/* longueur impaire && hauteur impaire*/
	{
		ChoisirCouleurDessin(c1);
		RemplirRectangle(taille_pixel*x,taille_pixel*y,taille_pixel,taille_pixel);
	}
	ChoisirCouleurDessin(c3);				/* Quadrillage */
	DessinerRectangle(taille_pixel*x,taille_pixel*y,taille_pixel,taille_pixel);
}

void serpent_dessin(int x, int y,int i, couleur c1, couleur c2, int taille_pixel){

	if ( i%2 == 0)							/* Couleur du serpent (commençant par la tête) */
	{
		ChoisirCouleurDessin(c1);
		RemplirRectangle(x,y,taille_pixel,taille_pixel);
	}
	else
	{
		ChoisirCouleurDessin(c2);			/* Couleur du serpent (le reste, 1 sur 2)*/
		RemplirRectangle(x,y,taille_pixel,taille_pixel);
	}
}

void obstable_dessin(int x, int y, couleur c1, int taille_pixel){
	ChoisirCouleurDessin(c1);				/*	Couleur obstacle*/
	RemplirRectangle(taille_pixel*x,taille_pixel*y,taille_pixel,taille_pixel);
}

void pomme_dessin(int x, int y, couleur c1,int taille_pixel){
	ChoisirCouleurDessin(c1);				/* 	Couleur pomme*/
	RemplirArc(taille_pixel*x,taille_pixel*y,taille_pixel,taille_pixel,0,360);
}

void pauseF(int *pause,int * sortie,int longueur,int hauteur, int taille_pixel){
	int T = 0;
	int tmp=0;
	ChoisirEcran(4);
	while (T != XK_space)			/* Tant qu'on n'appuie pas sur espace on est en pause*/
	{
		ChoisirCouleurDessin(CouleurParNom("white"));
		
		/* On fait clignoter */
		if (tmp>1500)
			;
		else{
			EcrireTexte(longueur*taille_pixel/2-60,hauteur*taille_pixel/2-50,"       Pause",2);
			EcrireTexte(longueur*taille_pixel/2-60,hauteur*taille_pixel/2-30,"Press SPACE to start",2);
		}
		if (tmp == 1900)	/* On recommence le clignotement*/
			tmp = 0;
		/* On affiche à l'écran */
		CopierZone(4,0,0,0,longueur*taille_pixel,hauteur*taille_pixel,620-(((longueur-2)*taille_pixel)/2),0);/* Affiche la pause*/
		CopierZone(2,4,0,0,longueur*taille_pixel,hauteur*taille_pixel,0,0);/*Pour afficher le terrain en fond durant la pause*/
		CopierZone(1,4,0,0,longueur*taille_pixel,hauteur*taille_pixel,0,0);/*Pour afficher le serpent en fond durant la pause*/
		/*CopierZone(2,1,0,0,longueur*taille_pixel,hauteur*taille_pixel,0,0);*/
		if (ToucheEnAttente())
		{
			T=Touche();		/* On ne met pas clavier() pour éviter de pouvoir changer la direction du serpent pendant la pause*/
		}
		if ( T == XK_Escape)	/* Si on appuie sur ECHAPPE on quitte le jeu*/
		{
			*sortie = 0;
			break;
		}
		tmp++;
	}

	*pause = 0;	/* Désactive la pause*/


}

void clavier(int T,int* sortie,int* direction_x,int* direction_y, int* pause){
	
	/*	ECHAPPE*/
	if (T==XK_Escape)
	{
		*sortie = 0;
	}
	/*	ESPACE*/
	if (T==XK_space)
	{
		*pause = 1;	/* On active la pause (elle se désactive elle-même dans la fonction pauseF à la fin)*/
	}
	
	/*	GAUCHE*/
	if (T==XK_Left && *direction_x != 1)	/* direction_x != 1 	ça empêche de faire demi-tour*/
	{
		*direction_x=-1;
		*direction_y=0;
	}
	
	/* 	DROITE*/
	if (T==XK_Right && *direction_x != -1) 	/* direction_x != -1 	ça empêche de faire demi-tour*/
	{
		*direction_x=1;
		*direction_y=0;
	}
	
	/*	HAUT*/
	if (T==XK_Up && *direction_y != 1) 		/* direction_y != 1 	ça empêche de faire demi-tour*/
	{
		*direction_x=0;
		*direction_y=-1;
	}
	
	/* 	BAS*/
	if (T==XK_Down && *direction_y != -1) 	/* direction_y != -1 	ça empêche de faire demi-tour*/
	{
		*direction_x=0;
		*direction_y=1;
	}
}


void initialiser_serpent(struct serpent* tableau_serpent ,int taille_serpent, int hauteur, int longueur,
	int* direction_x,int* direction_y, int taille_pixel){
	int i = 0;

	/*	On détermine la position initiale de la tête du serpent	*/
    tableau_serpent[0].x_tete_serpent=(longueur/2*taille_pixel);
    tableau_serpent[0].y_tete_serpent=(hauteur/2*taille_pixel);

    /*	On détermine la position du reste du serpent	*/
	for ( i = 1 ; i < taille_serpent ; i++ )
    {
    	tableau_serpent[i].x_tete_serpent=tableau_serpent[i-1].x_tete_serpent+taille_pixel;
		tableau_serpent[i].y_tete_serpent=tableau_serpent[i-1].y_tete_serpent;
    }

    /*On redéfinie le sens du serpent initial*/
    *direction_x = -1;
    *direction_y = 0;
}

void initialiser_terrain(int hauteur,int longueur,int** tableau_terrain){	/*On crée le terrain*/
	int i,j;
	for ( i = 0 ; i < longueur ; i++ )
    {
    	for ( j = 0 ; j < hauteur; j++ )
    	{
    		if ( j == 0 )		 
    		{
    			*(*(tableau_terrain+i)+j)=obstacle; 	/* 	On met un mur en haut*/
    		}
    		else if ( i == 0 )
    		{
    			*(*(tableau_terrain+i)+j)=obstacle;		/* 	On met un mur à gauche*/
    		}
    		else if ( j == hauteur-1 )
    		{
    			*(*(tableau_terrain+i)+j)=obstacle;		/* 	On met un mur à droite*/
    		}
    		else if ( i == longueur-1 )
    		{
    			*(*(tableau_terrain+i)+j)=obstacle;		/*	On met un mur en bas*/
    		}
    		else
    		*(*(tableau_terrain+i)+j)=vide;				/* 	Le reste est vide*/
    	}
    }
}

 

void creation_pomme(int longueur, int hauteur, int taille_serpent, int** tableau_terrain, struct serpent* tableau_serpent, int taille_pixel){

	struct pomme pomme;	/*On place les pommes*/
	int test = 2;
	int serpent_test_x = tableau_serpent[0].x_tete_serpent;
	serpent_test_x = serpent_test_x / taille_pixel;
	int serpent_test_y = tableau_serpent[0].y_tete_serpent;
	serpent_test_y = serpent_test_y / taille_pixel;

	while (test > 0)	/* On teste pour savoir s'il remplisse bien une case vide et pas autre chose*/
		{
			test = 2;
			pomme.x_pomme=rand()%longueur;
			pomme.y_pomme=rand()%hauteur;
			if (*(*(tableau_terrain+pomme.x_pomme)+pomme.y_pomme) != obstacle && 
				*(*(tableau_terrain+pomme.x_pomme)+pomme.y_pomme) != pommeT)
			{
				test = test - 1;
			}
			/*Pour ne pas gêner le serpent au départ*/
			if ( !(pomme.x_pomme>serpent_test_x-20 && pomme.x_pomme<serpent_test_x+taille_serpent && pomme.y_pomme==serpent_test_y) )
			{
				test = test - 1;
			}
		}
	*(*(tableau_terrain+pomme.x_pomme)+pomme.y_pomme) = pommeT; 	/* On attribue à la case du terrain la valeur PommeT*/
}

void creation_obstacle(int longueur, int hauteur, int taille_serpent, int** tableau_terrain, struct serpent* tableau_serpent, int taille_pixel){

	/* On place les obstacles*/

	struct obstacle obstacles;
	int test = 2;
	int serpent_test_x = tableau_serpent[0].x_tete_serpent;
	serpent_test_x = serpent_test_x / taille_pixel;
	int serpent_test_y = tableau_serpent[0].y_tete_serpent;
	serpent_test_y = serpent_test_y / taille_pixel;


	while (test > 0) /* On teste pour savoir s'il y a déjà quelque chose à leur place où s'ils sont juste devant le serpent (spawn kill)*/
		{
			test = 2;
			obstacles.x_obstacle=rand()%longueur;
			obstacles.y_obstacle=rand()%hauteur;
			if (*(*(tableau_terrain+obstacles.x_obstacle)+obstacles.y_obstacle) != obstacle)
			{
				test = test - 1;
			}
			/*Pour ne pas gêner le serpent au départ*/
			if (!(obstacles.x_obstacle>serpent_test_x-20 && obstacles.x_obstacle<serpent_test_x+taille_serpent && obstacles.y_obstacle==serpent_test_y))
			{
				test = test - 1;
			}
		}
	*(*(tableau_terrain+obstacles.x_obstacle)+obstacles.y_obstacle) = obstacle;	/* On attribue à la case du terrain la valeur Obstacle*/
}

void dessiner_terrain(int longueur, int hauteur, int** tableau_terrain,
 couleur c1_vide, couleur c2_vide, couleur c3_vide, couleur c1_obstacle, couleur c1_pomme,int taille_pixel){
	int i,j;
	for ( i = 0 ; i < longueur ; i++ )		/* longueur*/
	{
	   	for ( j = 0 ; j < hauteur ; j++ )	/* hauteur*/
	   	{
	   		if (*(*(tableau_terrain+i)+j) == vide)						/* Si la case est vide*/
	   		{
	   			vide_dessin(i,j,c1_vide,c2_vide,c3_vide,taille_pixel);
	   		}
	   		else if (*(*(tableau_terrain+i)+j) == obstacle)				/* S'il y a un obstacle*/
	   		{
	   			obstable_dessin(i,j,c1_obstacle,taille_pixel);
	   		}
	   		else if (*(*(tableau_terrain+i)+j) == pommeT)				/* S'il y a une pomme*/
	   		{
	   			vide_dessin(i,j,c1_vide,c2_vide,c3_vide,taille_pixel);
	   			pomme_dessin(i,j,c1_pomme,taille_pixel);
	   		}
	   	}
	}
}


/*--------------------------------------------------------------------------------------------------------------------------------*/

int jeu(int *tableau_valeur){
	/*int longueur_initiale,
	int hauteur_initiale,
	int nombre_de_pomme_initiale,
	int nombre_de_obstacle_initiale,
	int taille_serpent_initiale,
	int taille_pixel,
	int vitesse,
	int c1_vide,
    int c2_vide,
    int c3_vide,
    int c1_serpent,
    int c2_serpent,
    int c1_obstacle,
    int c1_pomme)*/
	

	srand(time(NULL));

	/*	initialisation	*/
	int longueur_initiale = tableau_valeur[0];				 
	int hauteur_initiale = tableau_valeur[1];				 
	int nombre_de_pomme_initiale = tableau_valeur[2];
	int nombre_de_obstacle_initiale = tableau_valeur[3];
	int taille_serpent_initiale = tableau_valeur[4];
	int taille_pixel = tableau_valeur[5];
	int vitesse_initial = 0;
	int vitesse = tableau_valeur[6];						 
	
		/*Déclarations des Couleur 	*/
    couleur c1_vide = tableau_valeur[7];			
    couleur c2_vide = tableau_valeur[8];			
    couleur c3_vide = tableau_valeur[9];		 	

    couleur c1_serpent = tableau_valeur[10];			
    couleur c2_serpent = tableau_valeur[11];

    couleur c1_obstacle = tableau_valeur[12];		

    couleur c1_pomme = tableau_valeur[13];			





	/*int longueur_initiale = 60;				max mon ecran 122 pour taille_pixel 10
	int hauteur_initiale = 40;				max mon ecran 55 pour taille_pixel 10
	int nombre_de_pomme_initiale = 5;
	int nombre_de_obstacle_initiale = 0;
	int taille_serpent_initiale = 10;
	int taille_pixel = 10;
	int vitesse_initial = 0;
	int vitesse = 1;						 Vitesse du serpent (entre 0 et 10) Remarque: Une valeur négative le fera juste ralentir*/
	


	/*	Déclarations des Couleur 	*/
    /*couleur c1_vide = CouleurParComposante(000,105,000);		Case vide
    couleur c2_vide = CouleurParComposante(000,100,000);		Si on veut mettre 2 couleurs de fond
    couleur c3_vide = CouleurParComposante(000,100,000);		Si on veut mettre un cadrillage (pour l'enlever on le met de la même couleur que c1 ou c2)

    couleur c1_serpent = CouleurParComposante(000,000,000);		Case serpent
    couleur c2_serpent = CouleurParComposante(000,000,000);

    couleur c1_obstacle = CouleurParComposante(050,050,050);	Case obstacle

    couleur c1_pomme = CouleurParComposante(255,000,000);		Case serpent*/
	
    /* Déclaration des variables non importées */

	/*	déclaration des variables	*/
	int vitesse_max = 10;
	int longueur = longueur_initiale + 2 ;	  			/* Dans le tableau du terrain il faut inclure les obstacles du bord*/
    int hauteur = hauteur_initiale +2 ; 	  			/* Dans le tableau du terrain il faut inclure les obstacles du bord*/
    int nombre_pomme = nombre_de_pomme_initiale;		/* On veut modifier le nombre de pomme mais se rappeler du nombre d'origine (utile ?)*/
    int nombre_obstacle = nombre_de_obstacle_initiale;	/* On veut modifier le nombre d'obstacle mais se rappeler du nombre d'origine (utile ?)*/
    int compteur_pomme=0;				/* Compte notre nombre de pomme manger -> permet de déterminer si on les a toutes mangées*/
    int recharger_terrain = 1;			/* Détermine principalement s'il faut recharger le terrain après avoir mangé toutes les pommes*/
    int taille_serpent = taille_serpent_initiale; /* On veut modifier la taille du serpent mais se rappeler de celle d'origine*/
    int nouvelle_taille_serpent=0;
    int i,j;				/*  Diverses utilités, surtout pour parcourir les boucles for
    							Et pour comparer les coordonnées de la tête du serpent avec celle du terrain*/
    int direction_x=-1,direction_y=0;	/*Dirige la tête en x et en y*/

    int Score_int = 0;		/* Calcul du score*/
	char Score_char [30];	/* On écrit le score en chaîne de caractère*/
	int Stage_int = 1;		/* Numéro du stage*/
	char Stage_char [30];
	int chronometre_se_int = 00; /*seconde*/
    int chronometre_min_int = 00; /*minute*/
    char chronometre_char [30];

    char ecriture_menu[150];

    int sortie = 1;			/* Rester dans la boucle du jeu*/
    int T=0;				/* Retient la touche du clavier sur laquelle on a appuiyé dans cette variable*/
    int pause=1;			/* Permet de rentrer dans la boucle pause, on y rentre dès le début*/
    /*int file_attente_touche = 0;*/

    unsigned long boucle_de_jeu_suivante = Microsecondes()+CYCLE_BOUCLE_JEU_SUIVANTE; /* Pour lancer une nouvelle boucle de jeu*/

    unsigned long chronometre_seconde = Microsecondes()+CYCLE_SECONDE;
    

    /*-------------------------------------------------------------------------------	*/

    /*	On crée la fenêtre de jeu	*/
    /* InitialiserGraphique();*/
    /*int Fenetre_l=500-((longueur_initiale*taille_pixel)/2);*/
    CreerFenetre(0,0,1250,600);
    

    /*	On choisi ici l'écran de fond pour d'abord créer le terrain de jeu	*/
    ChoisirEcran(2);

    /*	On crée le tableau concernant le terrain	*/
    int** tableau_terrain=malloc(sizeof(enum valeur_case*)*longueur);	/*longeur soit en x*/
    for ( i = 0 ; i < longueur ; i++ )
    {
    	tableau_terrain[i]=malloc(sizeof(enum valeur_case)*hauteur);	/*hauteur soit en y*/
    }

	/*	On crée le tableau concernant le serpent	*/    
    struct serpent* tableau_serpent = malloc(sizeof(struct serpent)*taille_serpent);

    /*	On initialise les deux tableaux	*/
    	/*	On ré-écrit le tableau serpent 	*/
	tableau_serpent = (struct serpent*)realloc(tableau_serpent,sizeof(struct serpent)*taille_serpent_initiale);
		/*	On ré-initialise la taille	*/
    taille_serpent = taille_serpent_initiale;
    initialiser_serpent(tableau_serpent,taille_serpent,hauteur,longueur,&direction_x,&direction_y,taille_pixel);
    initialiser_terrain(hauteur,longueur,tableau_terrain);

    /*	On place un fond noir */
    ChoisirEcran(3);
	ChoisirCouleurDessin(CouleurParNom("black"));
	RemplirRectangle(0,0,1250,600);
	CopierZone(3,0,0,0,1250,600,0,0);

	/* On commence le jeu */
    while(sortie)
	{
		

		/* Pour recharger le terrain */
		if ( recharger_terrain )
		{
			ChoisirEcran(2);	/* écran de fond*/


			initialiser_terrain(hauteur,longueur,tableau_terrain);
			/*	On place les obstacles	*/
			for ( j = 0 ; j < nombre_obstacle ; j++)
			{
				creation_obstacle(longueur,hauteur,taille_serpent,tableau_terrain,tableau_serpent,taille_pixel);
			}

			/*	On place les pommes	*/
			for ( j = 0 ; j < nombre_pomme ; j++)
			{
				creation_pomme(longueur,hauteur,taille_serpent,tableau_terrain,tableau_serpent,taille_pixel);
			}

			/*	On dessine le terrain 	*/
			dessiner_terrain(longueur,hauteur,tableau_terrain,c1_vide,c2_vide,c3_vide,c1_obstacle,c1_pomme,taille_pixel);
			recharger_terrain = 0; /* Le terrain a été rechargé*/

			CopierZone(2,1,0,0,longueur*taille_pixel,hauteur*taille_pixel,0,0); /*Pour afficher le terrain en fond durant la pause*/
		}
		
		ChoisirEcran(1);	/* écran en premier plan*/

		/*-----------------------------------------------------------------------------------------------------*/
		/*Début de la boucle qui gère le temps ()*/
		if (Microsecondes()>boucle_de_jeu_suivante)
			{
				boucle_de_jeu_suivante = Microsecondes()+CYCLE_BOUCLE_JEU_SUIVANTE - vitesse * 10000;
				
				/*	On déplace le serpent	*/
					/*chaque case avance de 1*/
				for ( i = taille_serpent-1 ; i > 0 ;i=i-1)
				{
					tableau_serpent[i]=tableau_serpent[i-1];
				}
					/*On replace la tête*/
				tableau_serpent[0].x_tete_serpent = tableau_serpent[0].x_tete_serpent + (taille_pixel*direction_x);
				tableau_serpent[0].y_tete_serpent = tableau_serpent[0].y_tete_serpent + (taille_pixel*direction_y);

				/*	On dessine le serpent 	*/
				for ( i = 0 ; i < taille_serpent ; i++)
				{
					/* On le dessine fragment par fragment*/
					serpent_dessin(tableau_serpent[i].x_tete_serpent,tableau_serpent[i].y_tete_serpent,i,c1_serpent,c2_serpent,taille_pixel);
				}

				if (pause)
				{
					pauseF(&pause,&sortie,longueur,hauteur,taille_pixel);
					
				}

				/*------------------------------------------------*/

				/*	Si le serpent se mord la queue	*/
				for ( i = 1 ; i < taille_serpent ; i++)
				{
					if ( tableau_serpent[0].x_tete_serpent == tableau_serpent[i].x_tete_serpent
						&& tableau_serpent[0].y_tete_serpent == tableau_serpent[i].y_tete_serpent)
					{
						sortie = 0;
					}
				}

				/*	On convertie les coordonnées de la tête du serpent de la fenêtre dans le tableau du terrain */
				i=tableau_serpent[0].x_tete_serpent;
				j=tableau_serpent[0].y_tete_serpent;
				i=i/taille_pixel;
				j=j/taille_pixel;
				
				/*	Le Serpent touche un obstacle	*/
				if ( *(*(tableau_terrain+i)+j) == obstacle )
				{
					sortie = 0;
				}
				
				/*	Le Serpent mange une pomme	*/
				if ( *(*(tableau_terrain+i)+j) == pommeT )
				{
					ChoisirEcran(2); /*	on redisse l'écran de fond  */
					vide_dessin(i,j,c1_vide,c2_vide,c3_vide,taille_pixel);		/* On remplce le dessin de la pomme par une case vide*/
					ChoisirEcran(1);								
					*(*(tableau_terrain+i)+j) = vide;	/* On remplace la valeur "pommeT" de la case du terrain par "vide"*/
					nouvelle_taille_serpent = nouvelle_taille_serpent + 2; /* Le serpent doit grandir de 2 cases en plus*/
					compteur_pomme++;				/* Le nombre de pomme mangé augmente de 1*/
					Score_int = Score_int + 5;		/* Le score augment de 5*/
				}

				/*	Le serpent grandi de 1 case	*/
				if (nouvelle_taille_serpent > 0 )
				{
					taille_serpent = taille_serpent + 1;
					nouvelle_taille_serpent = nouvelle_taille_serpent - 1 ;		/* 	Le nombre de fois qu'il lui reste à grandir
																					Ex: On mande 2 pommes, donc il faut qu'il grandisse 4 fois */
					tableau_serpent = (struct serpent*)realloc(tableau_serpent,sizeof(struct serpent)*taille_serpent);
				}

				/*	Si on a mangé toutes les pommes	*/
				if ( compteur_pomme == nombre_pomme )
				{
					recharger_terrain = 1;	/* On redessine le terrai */
					compteur_pomme = 0;	
					nombre_pomme = nombre_pomme + 1;		/* On augement le nombre de pomme sur le terrain*/
					nombre_obstacle = nombre_obstacle + 1;	/* On augement le nombre d'obstacle sur le terrain*/
					nouvelle_taille_serpent = 0; 			/* Il arrête de grandir*/
					/*	On ré-écrit le tableau serpent 	*/
					tableau_serpent = (struct serpent*)realloc(tableau_serpent,sizeof(struct serpent)*taille_serpent_initiale);
					/*	On ré-initialise la taille	*/
				    taille_serpent = taille_serpent_initiale;
					initialiser_serpent(tableau_serpent,taille_serpent,hauteur,longueur,&direction_x,&direction_y,taille_pixel);
					pause = 1;								/* On met le jeu en pause*/

					Stage_int = Stage_int + 1;

					if ( vitesse < vitesse_max )
						vitesse = vitesse + 1;					/* Le serpent accélère*/
					/*if ( vitesse > vitesse_max)
						vitesse = vitesse_max ;	*/
				}

				/*	Interaction clavier	*/
				if (ToucheEnAttente())
				{
					T=Touche();
					/*int jj = 0;*/
					clavier(T,&sortie,&direction_x,&direction_y,&pause);
					/*if (ToucheEnAttente() > 1)
						Touche();
						if (jj > 1){
							break;
						}*/
				}



				/* On recrée le fond en arrière plan */
				ChoisirEcran(3);
				ChoisirCouleurDessin(CouleurParNom("black"));	/* On place un carré noir*/
				RemplirRectangle(0,0,1250,600);					

				if ( Score_int < 100)	/* On transforme le score d'un entier à une chaine de caractère*/
				{
					if ( Score_int < 10)	/* On transforme le score d'un entier à une chaine de caractère*/
						sprintf(Score_char,"Score : 00%d",Score_int);				
					else
						sprintf(Score_char,"Score : 0%d",Score_int);					
				}
				else
					sprintf(Score_char,"Score : %d",Score_int);	

				if ( Stage_int < 10)	/* On transforme le stage d'un entier à une chaine de caractère*/
					sprintf(Stage_char,"Niveau : 0%d",Stage_int);
				else
					sprintf(Stage_char,"Niveau : %d",Stage_int);

				/* On transforme le temps d'un entier à une chaine de caractère */
				if (chronometre_se_int == 0)
					sprintf(chronometre_char,"Temps : %d : 00",chronometre_min_int);
				else if (chronometre_se_int < 10)	
					sprintf(chronometre_char,"Temps : %d : 0%d",chronometre_min_int,chronometre_se_int);
				else	
					sprintf(chronometre_char,"Temps : %d : %d",chronometre_min_int,chronometre_se_int);
				
				sprintf(ecriture_menu,"|| %s   -   %s   -   %s   || Pause : ESPACE   -   Exit : ECHAPPE ||",Score_char,Stage_char,chronometre_char);

				ChoisirCouleurDessin(CouleurParNom("white"));
				/*	On écrit le Score */
				EcrireTexte(615 - 275,hauteur*taille_pixel+20,ecriture_menu,2);
				/*hauteur du terrain multiplier par la place que prend un pixel + la place nessaire à l'écriture*/

				/* On supperpose les différents écrans et on envoie tout sur l'écran principal */
				CopierZone(1,3,0,0,longueur*taille_pixel,hauteur*taille_pixel,620-((longueur_initiale*taille_pixel)/2),0);
				CopierZone(2,1,0,0,longueur*taille_pixel,hauteur*taille_pixel,0,0);
				CopierZone(3,0,0,0,1250,600,0,0);	

			}
		/*-----------------------------------------------------------------------------------------------------*/
		/*Fin de la boucle qui gère le temps*/

			if (Microsecondes() > chronometre_seconde)
			{
				chronometre_seconde = Microsecondes()+CYCLE_SECONDE;
				chronometre_se_int = chronometre_se_int + 01 ;
				if (chronometre_se_int == 60)
				{
					chronometre_se_int = 0;
					chronometre_min_int = chronometre_min_int + 1;
				}
			}

    }

    free(tableau_serpent);
    free(tableau_terrain);
    main(tableau_valeur);
    return EXIT_SUCCESS;
}
