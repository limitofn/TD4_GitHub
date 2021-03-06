﻿////////////////////////////////////////////////////////////////////////////////
/// TD4 Automne 2017, INF1005C.
/// \file    TD4.cpp
/// \authors Ryan WAN-AJOUHU 1785993, Marc-Andre Primeau Breton 1856799
/// \date  dimanche 29 octobre
////////////////////////////////////////////////////////////////////////////////

#pragma region "Inclusions" //{

#include <ciso646>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

#pragma endregion //}



#pragma region "Déclaration" //{

#define JUSTE_POUR_QUE_CA_COMPILE return {}; //Toutes les lignes qui indiquent JUSTE_POUR_QUE_CA_COMPILE sont à éliminer, à mesure que vous écrivez vos fonctions; elles sont là uniquement pour que le programme compile avant d'avoir écrit les fonctions qui doivent retourner une valeur.

#pragma region "Constantes" //{

static const unsigned TAILLE_MAX_IMAGE = 20;         ///< La taille maximale d'une image, en pixels, dans chaque dimension.
static const unsigned TAILLE_MAX_GROUPE_IMAGES = 30; ///< Le nombre maximum d'images dans un groupe.

static const int MIN_CONCENTRATION_COULEUR_PRIMARE = 0;   ///< Valeur minimale que peut prendre une concentration de couleur dans un pixel d'image.
static const int MAX_CONCENTRATION_COULEUR_PRIMARE = 255; ///< Valeur maximale que peut prendre une concentration de couleur dans un pixel d'image.

static const unsigned TAILLE_ENTETE = 50;        ///< Taille, en nombre de caractères, des lignes de séparation dans les affichages d'entêtes; voir exemple dans l'énoncé.
static const char CARACTERE_ENTETE_GROUPE = '*'; ///< Le caractère à afficher pour les entêtes de groupe.
static const char CARACTERE_ENTETE_IMAGE  = '='; ///< Le caractère à afficher pour les entêtes d'image.

static const string NOM_FICHIER = "Images.txt"; ///< Le nom du fichier à charger.

static const int AUCUN = -1; ///< Constante à utiliser lorsqu'un indice n'existe pas, par exemple lorsqu'une recherche ne trouve pas.

// Constantes pour indiquer sur quelle dimension s'applique certaines fonctions (i.e. doublerTaille).
// Devrait être un enum mais est seulement matière au chapitre 8 du cours.
static const int DIMENSION_LARGEUR = 1; ///< Choisit d'appliquer sur la largeur.
static const int DIMENSION_HAUTEUR = 2; ///< Choisit d'appliquer sur la hauteur.

#pragma endregion //}


#pragma region "Structures" //{

/// La couleur composée d'un pixel.
/// Les concentrations de chaque couleur primaire sont entre MIN_CONCENTRATION_COULEUR_PRIMARE et MAX_CONCENTRATION_COULEUR_PRIMARE dans une image.
/// Ce type est aussi utilisé pour la différence entre deux compositions de couleur, et à ce moment les valeurs peuvent être aussi bien positives que négatives.
struct Pixel {
	int tauxRouge;           ///< La concentration de la couleur rouge dans le pixel.
	int tauxVert;            ///< La concentration de la couleur verte dans le pixel.
	int tauxBleu;            ///< La concentration de la couleur bleu  dans le pixel.
};


/// Pour la taille des images, en nombre de pixels.
struct Taille2D {
	unsigned largeur;
	unsigned hauteur;
};


/// Les informations et données d'une image.
/// Utilise actuellement l'allocation statique puisque l'allocation dynamique est vue seulement au chapitre 7 du cours.
struct Image {
	string nomImage;
	Taille2D taille;         ///< Le nombre de pixels de l'image en largeur et hauteur; chaque dimension doit toujours être entre 0 et TAILLE_MAX_IMAGE.
	Pixel pixels[TAILLE_MAX_IMAGE][TAILLE_MAX_IMAGE];   ///< L'ensemble des pixels de l'image dans l'ordre pixels[positionEnHauteur][positionEnLargeur]. Attention que le pixel de coordonnées (x,y) est donc à pixels[y][x].
};


/// Ensemble d'images appartenant au meme type.
/// Si une même image doit être dans plus d'un groupe, les données doivent actuellement être copiées car les pointeurs sont vus au chapitre 7 du cours.
struct GroupeImages {
	string type;                              ///< Le type des images appartenant à ce groupe.
	unsigned nImages;                         ///< Le nombre d'images dans ce groupe; doit être entre 0 et TAILLE_MAX_GROUPE_IMAGES.
	Image images[TAILLE_MAX_GROUPE_IMAGES];   ///< L'ensemble des images du groupe.
};

#pragma endregion //}


#pragma region "Prototypes de fonctions" //{

//  Fonctions pour les pixels

Pixel creerPixel(int tauxRouge, int tauxVert, int tauxBleu);

void ajouterConcentrationCouleurPrimaire(int& concentrationAModifier, int increment);

void ajouterCouleurPixel(Pixel& pixelAModifier, const Pixel& increment);

char retournerCouleurPixel(const Pixel& pixel);


// Fonctions pour les images

Image creerImage(const string& nomImage, unsigned tailleEnLargeur, unsigned tailleEnHauteur);

void doublerTaille(Image& image, int doitDoublerQuelleDimension, const Pixel& couleur);

void affecterPixel(Image& image, unsigned positionEnLargeur, unsigned positionEnHauteur, const Pixel& pixel);

void afficherImage(const Image& image);


// Fonctions pour les groupes d'images

void ajouterImage(GroupeImages& groupeImages, const Image& image);

void modifierType(GroupeImages& groupeImages, const string& type);

int chercherImageParNom(const GroupeImages& groupeImages, const string& nomImage);

void afficherGroupeImages(const GroupeImages& groupeImages);

GroupeImages lireFichier(const string& nomFichier);


#pragma endregion //}

#pragma endregion //}


int main()
{
	//Recuperer (lire) le group d'images à partir du fichier fourni.
	GroupeImages ensembleImages;
	ensembleImages = lireFichier("Images.txt");
	//Modifier le type de ce groupe d'images par "Images de tests".
	modifierType(ensembleImages, "Images de tests");
	//Chercher, dans ce groupe, les images ayant les nom "Image_Verte", "Image_Rouge", et "Blabla".
	int noImageVerte = chercherImageParNom(ensembleImages, "Image_Verte");
	int noImageRouge = chercherImageParNom(ensembleImages, "Image_Rouge");
	//Doubler la taille de Image_Verte en hauteur, en ajoutant des pixels bleus.
	Pixel pixelBleu = creerPixel(0, 0, 255);
	doublerTaille(ensembleImages.images[noImageVerte], DIMENSION_HAUTEUR, pixelBleu);
	//Doublee la taille des Image_Rouge en largeur, en ajoutant des pixels rouges.
	Pixel pixelRouge = creerPixel(255, 0, 0);
	doublerTaille(ensembleImages.images[noImageRouge], DIMENSION_LARGEUR, pixelRouge);
	//Modifier la couleur du pixel (1,1) de l'Image_Rouge en augmentant la concentration du bleu de 50 unités et en diminuant la concentration du rouge de 255 unités.
	int incrementRouge = -255;
	int incrementBleu = 50;
	ajouterConcentrationCouleurPrimaire(ensembleImages.images[noImageRouge].pixels[1][1].tauxBleu, incrementBleu);
	ajouterConcentrationCouleurPrimaire(ensembleImages.images[noImageRouge].pixels[1][1].tauxRouge, incrementRouge);
	//Modifier la couleur du pixel (2,1) de l'Image_Verte en augementant la concetration du bleu de 100 unités.
	incrementBleu = 100;
	ajouterConcentrationCouleurPrimaire(ensembleImages.images[noImageVerte].pixels[1][2].tauxBleu, incrementBleu);
	//Afficher le groupe d'images.
	afficherGroupeImages(ensembleImages);
}


#pragma region "Définitions" //{

#pragma region "Fonctions" //{

#pragma region "Fonctions pour les pixels" //{

Pixel creerPixel(int tauxRouge, int tauxVert, int tauxBleu)
{
	//Créer un pixel à partir des informations en paramètres et le retourner.
	Pixel nouveauPixel;
	nouveauPixel.tauxRouge = tauxRouge;
	nouveauPixel.tauxVert = tauxVert;
	nouveauPixel.tauxBleu = tauxBleu;
	return nouveauPixel;

}


void ajouterConcentrationCouleurPrimaire(int& concentrationAModifier, int increment) // concentration correspond au taux. ex : tauxRouge la fct retourne tauxRouge + increment
{
	concentrationAModifier += increment ;
	if(concentrationAModifier <= MIN_CONCENTRATION_COULEUR_PRIMARE)
		concentrationAModifier = MIN_CONCENTRATION_COULEUR_PRIMARE;
	if(concentrationAModifier >= MAX_CONCENTRATION_COULEUR_PRIMARE)
		concentrationAModifier = MAX_CONCENTRATION_COULEUR_PRIMARE;
	//Augmenter ou diminuer ( suivant le signe de increment ) la concentration de la couleur primaire passée en paramètre. La concentration finale doit être entre MIN_CONCENTRATION_COULEUR_PRIMARE et MAX_CONCENTRATION_COULEUR_PRIMARE .

}


void ajouterCouleurPixel(Pixel& pixelAModifier, const Pixel& increment)
{
	//Ajoute, composante par composante, l'increment au pixelAModifier, en s'assurant que chaque concentration résultante est dans les bornes.
	ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxRouge, increment.tauxRouge);
	ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxVert, increment.tauxVert);
	ajouterConcentrationCouleurPrimaire(pixelAModifier.tauxBleu, increment.tauxBleu);

}


char retournerCouleurPixel(const Pixel& pixel)
{
	//Retourner  'R' (respectivement 'V' et 'B') pour un pixel contenant uniquement du rouge (respectivement uniquement du vert et uniquement du bleu);  'Q' pour tout autre couleur de pixel.
	if (pixel.tauxBleu == 0 && pixel.tauxVert == 0 && pixel.tauxRouge != 0)
		return 'R';
	else if (pixel.tauxBleu == 0 && pixel.tauxVert != 0 && pixel.tauxRouge == 0)
		return 'V';
	else if (pixel.tauxBleu != 0 && pixel.tauxVert == 0 && pixel.tauxRouge == 0)
		return 'B';
	else
		return 'Q';

}

#pragma endregion //}


#pragma region "Fonctions pour les images" //{

Image creerImage(const string& nomImage, unsigned tailleEnLargeur, unsigned tailleEnHauteur)
{
	//Creer une image à partir des informations en paramètres et la retourner.
	Image nouvelleImage;
	nouvelleImage.nomImage = nomImage;
	nouvelleImage.taille.hauteur = tailleEnHauteur;
	nouvelleImage.taille.largeur = tailleEnLargeur;
	return nouvelleImage;

}


void doublerTaille(Image& image, int doitDoublerQuelleDimension, const Pixel& couleur)
{

	if (doitDoublerQuelleDimension == DIMENSION_LARGEUR) {
		image.taille.largeur = image.taille.largeur * 2;
        if (image.taille.largeur >= TAILLE_MAX_IMAGE) {
                image.taille.largeur = TAILLE_MAX_IMAGE;
        }
        for (unsigned int i = image.taille.largeur / 2; i<image.taille.largeur; i++)
        {
			for (unsigned int j = 0; j < image.taille.hauteur; j++){
				image.pixels[j][i] = couleur;
			}
        }
	}

	if (doitDoublerQuelleDimension == DIMENSION_HAUTEUR) {
		image.taille.hauteur += image.taille.hauteur;
		if (image.taille.hauteur >= TAILLE_MAX_IMAGE) {
			image.taille.hauteur = TAILLE_MAX_IMAGE;
		}
		for (unsigned int i = 0; i < image.taille.largeur; i++){
			for (unsigned int j = image.taille.hauteur / 2; j < image.taille.hauteur; j++){
				image.pixels[j][i] = couleur;
			}
		}
	}
}

	//Doubler la taille de l'image suivant doitDoublerQuelleDimension en paramètre (voir les définitions de constantes pour les valeurs possibles de ce paramètre) en respectant la taille maximale de l'image et en ajoutant des pixels de la couleur spécifiée en paramètre.



void affecterPixel(Image& image, unsigned positionEnLargeur, unsigned positionEnHauteur, const Pixel& pixel)
{
	//Remplacer le pixel de l'image à la position indiquée par positionEnLargeur et positionEnHauteur par la valeur du pixel passé en paramètre.
	image.pixels[positionEnHauteur][positionEnLargeur] = pixel;
}

void afficherImage(const Image& image)
{
	//Afficher l'image au complet, avec entête pour son nom, chaque pixel étant représenté par un caractère; le caractère à utiliser est indiqué dans la fonction retournerCouleurPixel.  Vous trouverez un exemple d'affichage dans l'enoncé.
	cout << string(TAILLE_ENTETE, CARACTERE_ENTETE_IMAGE) << endl;
	cout << image.nomImage  << endl;
	cout << string(TAILLE_ENTETE, CARACTERE_ENTETE_IMAGE) << endl;
	for (unsigned int i = 0; i < image.taille.hauteur; i++) {
		for (unsigned int j = 0; j < image.taille.largeur; j++) {
			cout << retournerCouleurPixel(image.pixels[i][j]);
		}
		cout << endl;
	}
	cout << string(TAILLE_ENTETE, CARACTERE_ENTETE_IMAGE) << endl;
}

#pragma endregion //}


#pragma region "Fonctions pour les groupes d'images" //{

void ajouterImage(GroupeImages& groupeImages, const Image& image)
{
	//si il y a encore de l'espace dans groupeImages, ajouter l'image en paramètre.
	if (groupeImages.nImages < TAILLE_MAX_GROUPE_IMAGES)
		groupeImages.images[groupeImages.nImages++] = image;
}


void modifierType(GroupeImages& groupeImages, const string& type)
{
	//Modifier le type du groupeImages par le type en paramètre.
	groupeImages.type = type;
}


int chercherImageParNom(const GroupeImages& groupeImages, const string& nomImage)
{
	//Retourner l'indice de l'image du groupeImages dont le nom est celui donné par le paramètre nomImage.  Retourner AUCUN si ce nom n'est pas trouvé.
	for (unsigned int i = 0; i < groupeImages.nImages; i++) {
		if (groupeImages.images[i].nomImage == nomImage)
			return i;
	}
	return AUCUN;
}


void afficherGroupeImages(const GroupeImages& groupeImages)
{
	//Afficher le groupe d'images, soit l'entête pour le type d'images et toutes les images du groupe. (Vous trouverez dans l'énoncé un exemple)
	cout << string(TAILLE_ENTETE, CARACTERE_ENTETE_GROUPE) << endl;
	cout << "Type du groupe d\'images : " << groupeImages.type << endl;
	cout << string(TAILLE_ENTETE, CARACTERE_ENTETE_GROUPE) << endl;
	for (unsigned int i = 0; i < groupeImages.nImages; i++) {
		afficherImage(groupeImages.images[i]);
	}
}


GroupeImages lireFichier(const string& nomFichier)
{
	//Récuperer (lire), à partir du fichier dont le nom est donné en paramètre, l'ensemble des images stockés, puis retourner un groupe d'images contenant ces images. Le type de ce groupe d'images n'est pas spécifié.
	ifstream fichier(nomFichier);
	GroupeImages nouveauGroupeImages;
	nouveauGroupeImages.nImages = 0;
	while (!ws(fichier).eof()) {
		fichier >> nouveauGroupeImages.images[nouveauGroupeImages.nImages].nomImage
			>> nouveauGroupeImages.images[nouveauGroupeImages.nImages].taille.hauteur
			>> nouveauGroupeImages.images[nouveauGroupeImages.nImages].taille.largeur;
		for (unsigned int i = 0; i < nouveauGroupeImages.images[nouveauGroupeImages.nImages].taille.hauteur; i++) {
			for (unsigned int j = 0; j < nouveauGroupeImages.images[nouveauGroupeImages.nImages].taille.largeur; j++) {
				fichier >> nouveauGroupeImages.images[nouveauGroupeImages.nImages].pixels[i][j].tauxRouge;
				fichier >> nouveauGroupeImages.images[nouveauGroupeImages.nImages].pixels[i][j].tauxVert;
				fichier >> nouveauGroupeImages.images[nouveauGroupeImages.nImages].pixels[i][j].tauxBleu;

			}
		}
		nouveauGroupeImages.nImages++;
	}
	return nouveauGroupeImages;
}


#pragma endregion //}

#pragma endregion //}

#pragma endregion //}
