//Pour compiler gcc-o EXEC parking.c - Wall -libncurses

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int L, C;

void ncurses_initialiser();
void ncurses_couleurs();
void ncurses_souris();
int click_souris();
void menu_principal();
char key_pressed();
void affichage_parking(int A, int B, char plan_num[A][B]);
int lancement_jeu();





void ncurses_initialiser(){
  initscr(); //démarre le mode ncurses
  cbreak(); // pour les saisies clavier
  noecho(); //désactive l'affichage des caractères saisis
  keypad(stdscr, TRUE); //active les touches spécifiques
  refresh(); //met à jour l'affichage
  curs_set(FALSE); //masque le curseur
}

void ncurses_couleurs(){
  if(has_colors() == FALSE) {
    endwin();
    fprintf(stderr, "le terminal ne supporte pas les couleurs.\n");
    exit(EXIT_FAILURE);
  }

  start_color();
    init_pair(1, COLOR_MAGENTA, 0);
    init_pair(2, COLOR_YELLOW, 0);
    init_pair(3, COLOR_GREEN, 0);
    init_pair(4, COLOR_CYAN, 0);
    init_pair(5, COLOR_RED, 0);
}

void ncurses_souris(){
  if(!mousemask(ALL_MOUSE_EVENTS, NULL)){
    endwin();
    fprintf(stderr, "Erreur lors de l'initialistion de la souris.\n");
    exit(EXIT_FAILURE);
  }
  if(has_mouse() != TRUE){
    endwin();
    fprintf(stderr, "Aucune souris n'est détectée.\n");
    exit(EXIT_FAILURE);
  }
}

int click_souris(){
  MEVENT event;
  int ch;
  while((ch=getch()) != KEY_F(1)){
    switch(ch){
      case KEY_MOUSE:
      if(getmouse(&event) == OK){
        C = event.x;
        L = event.y;
        if(event.bstate & BUTTON1_CLICKED){
          return 0;
        }
      }
    }
  }
  return 0;
}

void menu_principal(){
	
	attron(COLOR_PAIR(4));

         FILE* fichier = NULL;
   	fichier = fopen("logo2.txt", "r");
   	char l;

   	while((l = fgetc(fichier)) != EOF){
  			switch(l) {
  				case '0': printw(" "); break;

  				default: printw("%c",l); break;
  				}
         }
         attroff(COLOR_PAIR(4));
	attron(COLOR_PAIR(2));//ce qui suit sera afficher de la couleur désignée par 2
	mvprintw(12,31,"Mode Fluide");
	mvprintw(17,31,"Mode Charge");
	attroff(COLOR_PAIR(2));//stop couleur

	attron(COLOR_PAIR(1));//ce qui suit sera affciher de la couleur désignée par 1

	int i;

	/******************* Bouton Mode fluide ******************************/
	mvaddch(10,27,ACS_ULCORNER); mvaddch(14,27,ACS_LLCORNER);//coins
	mvaddch(10,45,ACS_URCORNER); mvaddch(14,45,ACS_LRCORNER);
	mvaddch(11,27,ACS_VLINE); mvaddch(11,45,ACS_VLINE);//lignes verticales
	mvaddch(12,27,ACS_VLINE); mvaddch(12,45,ACS_VLINE);
	mvaddch(13,27,ACS_VLINE); mvaddch(13,45,ACS_VLINE);
	for(i=28;i<=44;i++){//lignes horizontales
		mvaddch(10,i,ACS_HLINE);
		mvaddch(14,i,ACS_HLINE);
	}


	/****************** Bouton Mode chargé ***************************/

	mvaddch(15,22,ACS_ULCORNER); mvaddch(19,22,ACS_LLCORNER);//coins
	mvaddch(15,50,ACS_URCORNER); mvaddch(19,50,ACS_LRCORNER);
	mvaddch(16,22,ACS_VLINE); mvaddch(16,50,ACS_VLINE);//lignes verticales
	mvaddch(17,22,ACS_VLINE); mvaddch(17,50,ACS_VLINE);
	mvaddch(18,22,ACS_VLINE); mvaddch(18,50,ACS_VLINE);
	for(i=23;i<=49;i++){//lignes horizontales
		mvaddch(15,i,ACS_HLINE);
		mvaddch(19,i,ACS_HLINE);
	}

	attroff(COLOR_PAIR(1));//stop couleur

	/****************************Bouton Quitter**********************************/
	attron(COLOR_PAIR(5));//ce qui suit sera afficheé de la couleur désignée par 5

	mvprintw(31,31,"quitter");
	mvaddch(29,27,ACS_ULCORNER); mvaddch(33,27,ACS_LLCORNER);//coins
	mvaddch(29,41,ACS_URCORNER); mvaddch(33,41,ACS_LRCORNER);
	mvaddch(30,27,ACS_VLINE); mvaddch(30,41,ACS_VLINE);//lignes verticales
	mvaddch(31,27,ACS_VLINE); mvaddch(31,41,ACS_VLINE);
	mvaddch(32,27,ACS_VLINE); mvaddch(32,41,ACS_VLINE);
	for(i=28;i<=40;i++){//lignes horizontales
		mvaddch(29,i,ACS_HLINE);
		mvaddch(33,i,ACS_HLINE);
	}

	attroff(COLOR_PAIR (5));//stop couleur

	/*********************************************************************/
}

/*Pour récupérer les données saisies par l'utilisateur sans bloquer le jeu*/
char key_pressed() {
	struct termios oldterm, newterm;
	int oldfd;
	char c, result = 0;
	tcgetattr (STDIN_FILENO, &oldterm);
	newterm = oldterm;
	newterm.c_lflag &= ~(ICANON | ECHO);
	tcsetattr (STDIN_FILENO, TCSANOW, &newterm);
	oldfd = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl (STDIN_FILENO, F_SETFL, oldfd | O_NONBLOCK);
	c = getchar();
	tcsetattr (STDIN_FILENO, TCSANOW, &oldterm);
	fcntl (STDIN_FILENO, F_SETFL, oldfd);
	if (c != EOF) {
		ungetc(c, stdin);
		result = getchar();
	}
	return result;
}

void affichage_parking(int A, int B, char plan_num[A][B]){

    FILE* fichier = NULL;
   	fichier = fopen("plan_num.txt", "r");
   	char l;

   	while((l = fgetc(fichier)) != EOF){
  			switch(l) {
  				case '0': printw(" "); break;

  				case '1': addch(ACS_VLINE); break;

  				case '2': addch(ACS_RTEE); break;

  				case '3': addch(ACS_URCORNER); break;

  				case '4': addch(ACS_LLCORNER); break;

  				case '5': addch(ACS_BTEE); break;

  				case '6': addch(ACS_TTEE); break;

  				case '7': addch(ACS_LTEE); break;

  				case '8': addch(ACS_HLINE); break;

  				case '9': addch(ACS_PLUS); break;

  				case 'A': addch(ACS_LRCORNER); break;

  				case 'B': addch(ACS_ULCORNER); break;

  				default: printw("%c",l); break;
  				}
        }

  /*bouton retour menu*/
	attron(COLOR_PAIR(2)); //active la couleur jaune
	mvprintw(32,123,"Retour au menu principal");
	mvaddch(30,121,ACS_ULCORNER); mvaddch(34,121,ACS_LLCORNER); //coin supérieur et inférieur gauche
	mvaddch(30,148,ACS_URCORNER); mvaddch(34,148,ACS_LRCORNER); //coin supérieur et inférieur droit
	mvaddch(31,121,ACS_VLINE); mvaddch(31,148,ACS_VLINE); //lignes verticales
  mvaddch(32,121,ACS_VLINE); mvaddch(32,148,ACS_VLINE);
  mvaddch(33,121,ACS_VLINE); mvaddch(33,148,ACS_VLINE);

	for(int i=122;i<148;i++){ //lignes horizontales
		mvaddch(30,i,ACS_HLINE);
		mvaddch(34,i,ACS_HLINE);
	}

	attroff(COLOR_PAIR(2)); // désactive la couleur jaune

	while(click_souris()==0){//click pour bouton retour
		if(C>=121 && C<=148 && L>=30 && L<=34){
			clear();
			lancement_jeu(); // retour au menu principal
		}
	}
  fclose(fichier);
}


int lancement_jeu(){

  char plan_num[200][200];

	menu_principal();

	while (click_souris() == 0){

		if(C>=27 && C<=45 && L>=10 && L<=14){//click dans le bouton "Mode image"
			clear();
			//mvprintw(1,26,"Choisisez une image");
			affichage_parking(200, 200, plan_num);

		}

		if(C>=22 && C<=50 && L>=15 && L<=19){//click dans le bouton "Mode aléatoire"
			clear ();
			affichage_parking(200, 200, plan_num);
		}

		/*********************************************************************/

		/****************************quitter le jeu***************************/

		if(C>=27 && C<=41 && L>=29 && L<=33){
			endwin();//suspendre la session ncurses et restaurer le terminal
			exit(-1);//arreter le programme
		}

		/*********************************************************************/
	}

	return 0;
}


int main(int argc, char *argv[]){

  ncurses_initialiser();
  ncurses_souris();
  ncurses_couleurs();
  lancement_jeu();
  endwin();

  return 0;
}
