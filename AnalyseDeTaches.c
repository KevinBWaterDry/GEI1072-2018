#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#include <conio.h>
#include <ctype.h>

#define MAX_CHAINE 80

char couleurVide = 'O';
int nbCouleur = 3;
char Couleur[] = { 'R','B','V'};

//printf("%c",8);

typedef struct noeud
{
	int taille;
	char couleurTache;
	int posX;
	int posY;
	struct noeud *suivant;
} Noeud;

void Affiche(Noeud *n);
Noeud *CreerNoeud(int taille, Noeud *suivant, char couleurTache, int posX, int posY);
int Taille(char **m, int y, int x, char couleur, int dimension);
void AffichePosition(Noeud *n, char couleur);
void AfficheSommeCouleur(Noeud* liste, char* Couleur, int nbCouleur);
void AfficheTotalCouleur(Noeud* liste, char* Couleur, int nbCouleur);
void AfficheToutesCouleur(Noeud* liste, char* Couleur, int nbCouleur);
void AfficheInverse(Noeud* liste);


int main()
{
	FILE *fdFichierEntree;
	char *nomFichier = ".\\Final.txt";
	char s[MAX_CHAINE], *ptr;
	int dimension = 0;
	Noeud *liste = NULL;

	if ((fdFichierEntree = fopen(nomFichier, "r")) == NULL)
	{
		printf("Erreur a la lecture du fichier %s\n", nomFichier);
		_getch();
		exit(0);
	}

	fgets(s, MAX_CHAINE, fdFichierEntree);
	ptr = s;
	while (*ptr > ' ')
	{
		++dimension;
		++ptr;
	}

	char **tableau;

	tableau = (char **)malloc(sizeof(char *) * dimension);
	for (int i = 0; i < dimension; ++i)
	{
		tableau[i] = (char *)malloc(sizeof(char) * dimension);
		fgets(s, MAX_CHAINE, fdFichierEntree);
		for (int j = 0; j < dimension; ++j)
			tableau[i][j] = s[j];
	}

	for (int y = 0; y < dimension; ++y)
	{
		for (int x = 0; x < dimension; ++x)
		{
			for (int i = 0; i < nbCouleur; i++)
			{
				if (tableau[y][x] == Couleur[i])
				{
					liste = CreerNoeud(Taille(tableau, y, x, Couleur[i], dimension), liste, Couleur[i], x, y);
				}
			}
		}
	}

	fclose(fdFichierEntree);
	Affiche(liste);
	printf("\n");
	AfficheInverse(liste);
	printf("\n");
	//AffichePosition(liste, 'R');
	AfficheSommeCouleur(liste, Couleur, nbCouleur);
	printf("\n");
	AfficheTotalCouleur(liste, Couleur, nbCouleur);
	printf("\n");
	AfficheToutesCouleur(liste, Couleur, nbCouleur);

	_getch();
}

void Affiche(Noeud *n)
{
	if (n == NULL)	return;

	Affiche(n->suivant);
	printf("tache %c  de grosseur %d\n", n->couleurTache, n->taille);

}
void AfficheInverse(Noeud* liste)
{
	if (liste == NULL)	return;
	printf("tache %c  de grosseur %d\n", liste->couleurTache, liste->taille);
	AfficheInverse(liste->suivant);
}
void AffichePosition(Noeud *n, char couleur)
{//c'est la position du dernier pixel de cette couleur trouvé
	if (n == NULL)	return;

	AffichePosition(n->suivant, couleur);
	if (n->couleurTache == couleur)
	{
		printf("en X: %d en Y:%d\n", n->posX, n->posY);
	}

}

Noeud *CreerNoeud(int taille, Noeud *suivant, char couleurTache, int posX, int posY)
{
	Noeud *n = (Noeud *)malloc(sizeof(Noeud));
	n->taille = taille;
	n->suivant = suivant;
	n->couleurTache = couleurTache;
	n->posX = posX;
	n->posY = posY;
	return n;
}

int Taille(char **m, int y, int x, char couleur, int dimension)
{
	if (y < 0 || x < 0 || y >= dimension || x >= dimension)	return 0;

	if (m[y][x] != couleur)	return 0;

	m[y][x] = couleurVide;

	return 1 + Taille(m, y - 1, x, couleur, dimension) +
		Taille(m, y + 1, x, couleur, dimension) +
		Taille(m, y, x - 1, couleur, dimension) +
		Taille(m, y, x + 1, couleur, dimension)
		/*	+ Taille(m, y + 1, x + 1, couleur, dimension)				//pour chercher avec les diagonales
		+ Taille(m, y + 1, x - 1, couleur, dimension)
		+ Taille(m, y - 1, x + 1, couleur, dimension)
		+ Taille(m, y - 1, x - 1, couleur, dimension) */
		;
}
void AfficheSommeCouleur(Noeud* liste, char* Couleur,int nbCouleur)
{
	Noeud* sauvegarde = liste;
	for (int i = 0; i < nbCouleur;++i)
	{
		int somme=0; 
		int nbTaches=0;
		liste = sauvegarde;
		while (liste != NULL)
		{
			if (liste->couleurTache == Couleur[i])
			{
				somme += liste->taille;
				nbTaches += 1;
			}
			liste = liste->suivant;
		}
		printf("il y a %d taches %c ont une grandeur de %d\n", nbTaches, Couleur[i], somme );
	}
}
void AfficheToutesCouleur(Noeud* liste, char* Couleur, int nbCouleur)
{
	Noeud* sauvegarde = liste;
	for (int i = 0; i < nbCouleur; ++i)
	{
		int somme = 0;
		int nbTaches = 0;
		liste = sauvegarde;
		while (liste != NULL)
		{
			if (liste->couleurTache == Couleur[i])
			{
				somme += liste->taille;
				nbTaches += 1;
			}
			liste = liste->suivant;
		}
		printf("il y a %d taches %c ont une grandeur de", nbTaches, Couleur[i]);
		liste = sauvegarde;
		while (liste != NULL)
		{
			if (liste->couleurTache == Couleur[i])
			{
				printf(" %d,", liste->taille);
				
			}
			liste = liste->suivant;
		}
		printf("%c \n",8);
	}
}
void AfficheTotalCouleur(Noeud* liste, char* Couleur, int nbCouleur)
{
	int somme = 0;
	while (liste != NULL)
	{
		somme += liste->taille;
		liste = liste->suivant;
	}
	printf("la grandeur totale des tache est de %d\n", somme);
}
