	 	 	 	
//
// RK_Sabre.cpp
//
// Compilation : 
// >> g++ `pkg-config opencv --cflags` Filtre_Anthony.cpp -o Filtre_Anthony `pkg-config opencv --libs`
//
// Exécution :
// >> ./RK_Projet
//
// pour trouver le /dev/video? de la WebCam faire :
// sudo rm /dev/video*
// brancher la WebCam
// >> debian@SabreLite:~/Desktop$ ls /dev/video*
// ==>/dev/video4
//
// adapter la ligne :
//     capture = cvCreateCameraCapture( 4 );
//

/*
 * Code written by Vinz (GeckoGeek.fr) 14/03/2010
 * http://www.geckogeek.fr/lire-le-flux-dune-webcam-camera-video-avec-opencv.html
 * And modified by Rostom Kachouri; 16/02/2016
 * IN5E23
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
// OpenCV header

#include "highgui.h"
#include "cv.h"
#include <iostream>

// Fonction tri bulle
void BubbleSort(unsigned char arr[9]){
    for(int i = 0; i < 8; i++){ 
   	 for(int j = 0; j < 8 - i; j++){  // on parcours les élements de la liste 
   		 if(arr[j]>arr[j+1]){ // si l'élement j+1 est plus petit que l'element j, on les inverse 
   			 unsigned char temp = arr[j];
   			 arr[j] = arr[j+1];
   			 arr[j+1] = temp;
   		 }
   	 }
    }
}

void selectionSort(unsigned char arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < size; j++) { // on parcours les elements du tableau non tries
            if (arr[j] < arr[minIndex]) { 
                minIndex = j;				// on enregistre la position de la valeurs la plus petite 
            }
        }
        if (minIndex != i) { 				// on met la valeur minimale à la bonne position dans le tableau
            unsigned char temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}


void MedianFilter_zero_padding_bubble_sort(uchar *inputData, uchar *outputData, int step, int width, int height) {

	for (int x = 0 ; x < width; x++){ 		// traitement ligne 1 et height 
		outputData[0*step+x] =0; 			// Tous les pixels de la ligne 0 sont mis à 0 
		outputData[(height-1)*step+x] =0;	// Tous les pixels de la ligne height-1 sont mis à 0 
	}
	
	for (int y = 0 ; y < height; y++){ 		// traitement colonne 1 et width
		outputData[y*step+0] =0;			// Tous les pixels de la colonne 0 sont mis à 0
		outputData[y*step+(width-1)] =0;	// Tous les pixels de la colonne width-1 sont mis à 0 
	}

	for (int y = 1; y < height-1; ++y) { //On parcours les lignes de l'image 
        	for (int x = 1; x < width-1; ++x) { // on parcours les colonnes de l'image 

			uchar values[9]; // tableau contenant les 9 valeurs des pixels 
			int index = 0;
		    // Parcours des 9 valeurs du carré d'application du filtre
			for (int i=-1; i <= 1 ; i++){ // parcours des lignes 
				for (int j =-1; j<=1 ; j++){ // parcours des colonnes 
					values[index] = inputData[(y+i)*step+(x+j)]; // permet de spécifier le pixel à récupérer 
					index=index+1; // on incrémente pour remplir la liste 
				}
			}
				

		    // Tri du tableau plus obtention de la valeur médiane
		    BubbleSort(values); // tri de la lsite 
		    outputData[y*step+x] = values[4]; // on change la valeur du pixel par la médiane 
		}
		
		
	}
}


void MedianFilter_zero_padding_std_sort(uchar *inputData, uchar *outputData, int step, int width, int height) {

	for (int x = 0 ; x < width; x++){ 		// traitement ligne 1 et height 
		outputData[0*step+x] =0; 			// Tous les pixels de la ligne 0 sont mis à 0 
		outputData[(height-1)*step+x] =0;	// Tous les pixels de la ligne height-1 sont mis à 0 
	}
	
	for (int y = 0 ; y < height; y++){ 		// traitement colonne 1 et width
		outputData[y*step+0] =0;			// Tous les pixels de la colonne 0 sont mis à 0
		outputData[y*step+(width-1)] =0;	// Tous les pixels de la colonne width-1 sont mis à 0 
	}

	for (int y = 1; y < height-1; ++y) { //traitemen du reste de l'image en median 
        	for (int x = 1; x < width-1; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;

		    // Parcours du carrée du filtre
			for (int i=-1; i <= 1 ; i++){
				for (int j =-1; j<=1 ; j++){
					values[index] = inputData[(y+i)*step+(x+j)];
					//values[index] = inputData[3*step+2];
					index=index+1;
				}
			}
		    // Tri du tableau plus obtention de la valeur médiane
		    std::sort(values, values + 9); // tri du tableau avec la fontion sort
		    outputData[y*step+x] = values[4]; // récupération de la valeur médiane pour le pixel x, y 
		}
	}
}

void MedianFilter_zero_padding_tri_classique(uchar *inputData, uchar *outputData, int step, int width, int height) {

	for (int x = 0 ; x < width; x++){ 		// traitement ligne 1 et height 
		outputData[0*step+x] =0; 			// Tous les pixels de la ligne 0 sont mis à 0 
		outputData[(height-1)*step+x] =0;	// Tous les pixels de la ligne height-1 sont mis à 0 
	}
	
	for (int y = 0 ; y < height; y++){ 		// traitement colonne 1 et width
		outputData[y*step+0] =0;			// Tous les pixels de la colonne 0 sont mis à 0
		outputData[y*step+(width-1)] =0;	// Tous les pixels de la colonne width-1 sont mis à 0 
	}

	for (int y = 1; y < height-1; ++y) { //traitemen du reste de l'image utilisant la médiane 
        	for (int x = 1; x < width-1; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;

		    // Parcours du carrée du filtre
			for (int i=-1; i <= 1 ; i++){
				for (int j =-1; j<=1 ; j++){
					values[index] = inputData[(y+i)*step+(x+j)];
					index=index+1;
				}
			}
		    // Tri du tableau plus obtention de la valeur médiane
		    int size = sizeof(values) / sizeof(values[0]);
		    selectionSort(values,size);
		    outputData[y*step+x] = values[4];
		}
		
		
	}
}


void MedianFilter_sans_bord_bubble_sort(uchar *inputData, uchar *outputData, int step, int width, int height) {

	for (int x = 0 ; x < width; x++){ 				// traitement ligne 1 et height -1 en pixel entrant = pixel sortant 
		outputData[0*step+x] = inputData[0*step+x];						// ligne 0 
		outputData[(height-1)*step+x] = inputData[(height-1)*step+x];	// ligne height - 1
	}
	
	for (int y = 0 ; y < height; y++){ 				// traitement colonne 1 et width-1 en pixel entrant = pixel sortant 
		outputData[y*step+0] = inputData[y*step+0];						// colonne 0 
		outputData[y*step+(width-1)] = inputData[y*step+(width-1)];		//colonne width - 1
	}

	for (int y = 1; y < height-1; ++y) { //traitemen du reste de l'image en median 
        	for (int x = 1; x < width-1; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;

		    // Parcours du carrée du filtre
			for (int i=-1; i <= 1 ; i++){
				for (int j =-1; j<=1 ; j++){
					values[index] = inputData[(y+i)*step+(x+j)];
					index=index+1;
				}
			}
				

		    // Tri du tableau plus obtention de la valeur médiane
		    BubbleSort(values); 
		    outputData[y*step+x] = values[4];
		}
		
		
	}
}

void MedianFilter_sans_bord_std_sort(uchar *inputData, uchar *outputData, int step, int width, int height) {

	for (int x = 0 ; x < width; x++){ 				// traitement ligne 1 et height -1 en pixel entrant = pixel sortant 
		outputData[0*step+x] = inputData[0*step+x];						// ligne 0 
		outputData[(height-1)*step+x] = inputData[(height-1)*step+x];	// ligne height - 1
	}
	
	for (int y = 0 ; y < height; y++){ 				// traitement colonne 1 et width-1 en pixel entrant = pixel sortant 
		outputData[y*step+0] = inputData[y*step+0];						// colonne 0 
		outputData[y*step+(width-1)] = inputData[y*step+(width-1)];		//colonne width - 1
	}

	for (int y = 1; y < height-1; ++y) { //traitemen du reste de l'image en median 
        	for (int x = 1; x < width-1; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;

		    // Parcours du carrée du filtre
			for (int i=-1; i <= 1 ; i++){
				for (int j =-1; j<=1 ; j++){
					values[index] = inputData[(y+i)*step+(x+j)];
					//values[index] = inputData[3*step+2];
					index=index+1;
				}
			}
				

		    // Tri du tableau plus obtention de la valeur médiane
		    std::sort(values, values + 9);
		    outputData[y*step+x] = values[4];
		}
		
		
	}
}

void MedianFilter_sans_bord_tri_classique(uchar *inputData, uchar *outputData, int step, int width, int height) {

	for (int x = 0 ; x < width; x++){ 				// traitement ligne 1 et height -1 en pixel entrant = pixel sortant 
		outputData[0*step+x] = inputData[0*step+x];						// ligne 0 
		outputData[(height-1)*step+x] = inputData[(height-1)*step+x];	// ligne height - 1
	}
	
	for (int y = 0 ; y < height; y++){ 				// traitement colonne 1 et width-1 en pixel entrant = pixel sortant 
		outputData[y*step+0] = inputData[y*step+0];						// colonne 0 
		outputData[y*step+(width-1)] = inputData[y*step+(width-1)];		//colonne width - 1
	}

	for (int y = 1; y < height-1; ++y) { //traitemen du reste de l'image en median 
        	for (int x = 1; x < width-1; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;

		    // Parcours du carrée du filtre
			for (int i=-1; i <= 1 ; i++){
				for (int j =-1; j<=1 ; j++){
					values[index] = inputData[(y+i)*step+(x+j)];
					//values[index] = inputData[3*step+2];
					index=index+1;
				}
			}
				

		    // Tri du tableau plus obtention de la valeur médiane
		    int size = sizeof(values) / sizeof(values[0]);
		    selectionSort(values,size);
		    outputData[y*step+x] = values[4];
		}
		
		
	}
}


void MedianFilter_AvecBord_SYM_std_sort(uchar *inputData, uchar *outputData, int step, int width, int height) {
	for (int y = 0; y < height; ++y) {
        	for (int x = 0; x < width; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;
			//uchar image1[9];

		    // Parcours du carrée du filtre
			if (x==0 and y==0){ // pixel dans le coin supérieur gauche 
				for (int i=0; i <= 1 ; i++){ // parcour des elements y et y+1
						for (int j =0; j<=1 ; j++){ // parcour des elements x et x+1
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
				// création des pixels par symétrie
				values[4] = inputData[(y+1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y+1)*step+(x+1)];
				values[7] = inputData[(y+1)*step+(x)];
				values[8] = inputData[(y+1)*step+(x+1)];
				
			}
			else if (x==0 and y == (height-1)){ // pixel dans le coin inférieur gauche
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				// création des pixels par symétrie
				values[4] = inputData[(y-1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y-1)*step+(x+1)];
				values[7] = inputData[(y-1)*step+(x)];
				values[8] = inputData[(y-1)*step+(x+1)];
				
			}
			else if (x == (width-1) and y == 0){ // pixel dans le coin supérieur droit
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				// création des pixels par symétrie
				values[4] = inputData[(y+1)*step+(x-1)];
				values[5] = inputData[(y+1)*step+(x)];
				values[6] = inputData[(y+1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x-1)];
				values[8] = inputData[(y+1)*step+(x-1)];
			}
			else if (x == (width-1) and y == (height-1)){ // pixel dans le coin inférieur droit 
				for (int i =-1; i<=0 ; i++){
					for (int j =-1; j<=0 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				values[4] = inputData[(y-1)*step+(x-1)];
				values[5] = inputData[(y-1)*step+(x)];
				values[6] = inputData[(y-1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x+1)];
				values[8] = inputData[(y-1)*step+(x-1)];
			}
			else if (x==0 or y==0){
				if (x==0){ // pixels sur la première colonne
					for (int i=-1; i <= 1 ; i++){
						for (int j =0; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x+1)];
					values[7] = inputData[(y)*step+(x+1)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (x==(width-1)){ // pixel sur la dernière colonne
					for (int i=-1; i <= 1 ; i++){
						for (int j =-1; j<=0 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
						}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y)*step+(x-1)];
					values[8] = inputData[(y+1)*step+(x-1)];
				}
				if (y==0){ // pixel sur la première ligne 
					for (int i=0; i <= 1 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y+1)*step+(x-1)];
					values[7] = inputData[(y+1)*step+(x)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (y==(height-1)){ // pixel sur la dernière ligne 
					for (int i=-1; i <= 0 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y-1)*step+(x)];
					values[8] = inputData[(y-1)*step+(x+1)];
				}
			}
			else{ // gestion de tous les autres pixels 
				for (int i=-1; i <= 1 ; i++){
					for (int j =-1; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						//values[index] = inputData[3*step+2];
						index=index+1;
					}
				}
			}	

		    // Tri du tableau plus obtention de la valeur médiane
		    std::sort(values, values + 9); // tri du tableau de 9 élements 
		    outputData[y*step+x] = values[4];
		}
		
		
	}
}

void MedianFilter_AvecBord_SYM_bubble_sort(uchar *inputData, uchar *outputData, int step, int width, int height) {
for (int y = 0; y < height; ++y) {
        	for (int x = 0; x < width; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;
			//uchar image1[9];

		    // Parcours du carrée du filtre
			if (x==0 and y==0){ // pixel dans le coin supérieur gauche 
				for (int i=0; i <= 1 ; i++){ // parcour des elements y et y+1
						for (int j =0; j<=1 ; j++){ // parcour des elements x et x+1
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
				// création des pixels par symétrie
				values[4] = inputData[(y+1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y+1)*step+(x+1)];
				values[7] = inputData[(y+1)*step+(x)];
				values[8] = inputData[(y+1)*step+(x+1)];
				
			}
			else if (x==0 and y == (height-1)){ // pixel dans le coin inférieur gauche
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				// création des pixels par symétrie
				values[4] = inputData[(y-1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y-1)*step+(x+1)];
				values[7] = inputData[(y-1)*step+(x)];
				values[8] = inputData[(y-1)*step+(x+1)];
				
			}
			else if (x == (width-1) and y == 0){ // pixel dans le coin supérieur droit
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				// création des pixels par symétrie
				values[4] = inputData[(y+1)*step+(x-1)];
				values[5] = inputData[(y+1)*step+(x)];
				values[6] = inputData[(y+1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x-1)];
				values[8] = inputData[(y+1)*step+(x-1)];
			}
			else if (x == (width-1) and y == (height-1)){ // pixel dans le coin inférieur droit 
				for (int i =-1; i<=0 ; i++){
					for (int j =-1; j<=0 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				values[4] = inputData[(y-1)*step+(x-1)];
				values[5] = inputData[(y-1)*step+(x)];
				values[6] = inputData[(y-1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x+1)];
				values[8] = inputData[(y-1)*step+(x-1)];
			}
			else if (x==0 or y==0){
				if (x==0){ // pixels sur la première colonne
					for (int i=-1; i <= 1 ; i++){
						for (int j =0; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x+1)];
					values[7] = inputData[(y)*step+(x+1)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (x==(width-1)){ // pixel sur la dernière colonne
					for (int i=-1; i <= 1 ; i++){
						for (int j =-1; j<=0 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
						}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y)*step+(x-1)];
					values[8] = inputData[(y+1)*step+(x-1)];
				}
				if (y==0){ // pixel sur la première ligne 
					for (int i=0; i <= 1 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y+1)*step+(x-1)];
					values[7] = inputData[(y+1)*step+(x)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (y==(height-1)){ // pixel sur la dernière ligne 
					for (int i=-1; i <= 0 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y-1)*step+(x)];
					values[8] = inputData[(y-1)*step+(x+1)];
				}
			}
			else{ // gestion de tous les autres pixels 
				for (int i=-1; i <= 1 ; i++){
					for (int j =-1; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						//values[index] = inputData[3*step+2];
						index=index+1;
					}
				}
			}	

		    // Tri du tableau plus obtention de la valeur médiane
		    BubbleSort(values);
		    outputData[y*step+x] = values[4];
		}
		
		
	}
}

void MedianFilter_AvecBord_SYM_tri_classique(uchar *inputData, uchar *outputData, int step, int width, int height) {
	for (int y = 0; y < height; ++y) {
        	for (int x = 0; x < width; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			int index = 0;
			//uchar image1[9];

		    // Parcours du carrée du filtre
			if (x==0 and y==0){ // pixel dans le coin supérieur gauche 
				for (int i=0; i <= 1 ; i++){ // parcour des elements y et y+1
						for (int j =0; j<=1 ; j++){ // parcour des elements x et x+1
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
				// création des pixels par symétrie
				values[4] = inputData[(y+1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y+1)*step+(x+1)];
				values[7] = inputData[(y+1)*step+(x)];
				values[8] = inputData[(y+1)*step+(x+1)];
				
			}
			else if (x==0 and y == (height-1)){ // pixel dans le coin inférieur gauche
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				// création des pixels par symétrie
				values[4] = inputData[(y-1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y-1)*step+(x+1)];
				values[7] = inputData[(y-1)*step+(x)];
				values[8] = inputData[(y-1)*step+(x+1)];
				
			}
			else if (x == (width-1) and y == 0){ // pixel dans le coin supérieur droit
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				// création des pixels par symétrie
				values[4] = inputData[(y+1)*step+(x-1)];
				values[5] = inputData[(y+1)*step+(x)];
				values[6] = inputData[(y+1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x-1)];
				values[8] = inputData[(y+1)*step+(x-1)];
			}
			else if (x == (width-1) and y == (height-1)){ // pixel dans le coin inférieur droit 
				for (int i =-1; i<=0 ; i++){
					for (int j =-1; j<=0 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				values[4] = inputData[(y-1)*step+(x-1)];
				values[5] = inputData[(y-1)*step+(x)];
				values[6] = inputData[(y-1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x+1)];
				values[8] = inputData[(y-1)*step+(x-1)];
			}
			else if (x==0 or y==0){
				if (x==0){ // pixels sur la première colonne
					for (int i=-1; i <= 1 ; i++){
						for (int j =0; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x+1)];
					values[7] = inputData[(y)*step+(x+1)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (x==(width-1)){ // pixel sur la dernière colonne
					for (int i=-1; i <= 1 ; i++){
						for (int j =-1; j<=0 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
						}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y)*step+(x-1)];
					values[8] = inputData[(y+1)*step+(x-1)];
				}
				if (y==0){ // pixel sur la première ligne 
					for (int i=0; i <= 1 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y+1)*step+(x-1)];
					values[7] = inputData[(y+1)*step+(x)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (y==(height-1)){ // pixel sur la dernière ligne 
					for (int i=-1; i <= 0 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y-1)*step+(x)];
					values[8] = inputData[(y-1)*step+(x+1)];
				}
			}
			else{ // gestion de tous les autres pixels 
				for (int i=-1; i <= 1 ; i++){
					for (int j =-1; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						//values[index] = inputData[3*step+2];
						index=index+1;
					}
				}
			}	

		    // Tri du tableau plus obtention de la valeur médiane
		    int size = sizeof(values) / sizeof(values[0]);
		    selectionSort(values,size);
		    outputData[y*step+x] = values[4];
		}
		
		
	}
}

void MedianFilter_AvecBord_SYM_tri_classique_7tris(uchar *inputData, uchar *outputData, int step, int width, int height) {
	for (int y = 0; y < height; ++y) {
        	for (int x = 0; x < width; ++x) {
		    // Tableau avec les valeurs des pixels
			uchar values[9];
			// tableau contenant les 3 lignes, les 3 colonnes et la diagonale 
			uchar value1[3];
			uchar value2[3];
			uchar value3[3];
			uchar value4[3];
			uchar value5[3];
			uchar value6[3];
			uchar value7[3];

			int index = 0;
			//uchar image1[9];

		    // Parcours du carrée du filtre ( code identique au précédent)
			if (x==0 and y==0){
				for (int i=0; i <= 1 ; i++){
						for (int j =0; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
				values[4] = inputData[(y+1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y+1)*step+(x+1)];
				values[7] = inputData[(y+1)*step+(x)];
				values[8] = inputData[(y+1)*step+(x+1)];
				
			}
			else if (x==0 and y == (height-1)){
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				values[4] = inputData[(y-1)*step+(x+1)];
				values[5] = inputData[(y)*step+(x+1)];
				values[6] = inputData[(y-1)*step+(x+1)];
				values[7] = inputData[(y-1)*step+(x)];
				values[8] = inputData[(y-1)*step+(x+1)];
				
			}
			else if (x == (width-1) and y == 0){
				for (int i=-1; i <= 0 ; i++){
					for (int j =0; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				values[4] = inputData[(y+1)*step+(x-1)];
				values[5] = inputData[(y+1)*step+(x)];
				values[6] = inputData[(y+1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x-1)];
				values[8] = inputData[(y+1)*step+(x-1)];
			}
			else if (x == (width-1) and y == (height-1)){
				for (int i =-1; i<=0 ; i++){
					for (int j =-1; j<=0 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						index=index+1;
					}
				}
				values[4] = inputData[(y-1)*step+(x-1)];
				values[5] = inputData[(y-1)*step+(x)];
				values[6] = inputData[(y-1)*step+(x-1)];
				values[7] = inputData[(y)*step+(x+1)];
				values[8] = inputData[(y-1)*step+(x-1)];
			}
			else if (x==0 or y==0){
				if (x==0){
					for (int i=-1; i <= 1 ; i++){
						for (int j =0; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x+1)];
					values[7] = inputData[(y)*step+(x+1)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (x==(width-1)){
					for (int i=-1; i <= 1 ; i++){
						for (int j =-1; j<=0 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
						}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y)*step+(x-1)];
					values[8] = inputData[(y+1)*step+(x-1)];
				}
				if (y==0){
					for (int i=0; i <= 1 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y+1)*step+(x-1)];
					values[7] = inputData[(y+1)*step+(x)];
					values[8] = inputData[(y+1)*step+(x+1)];
				}
				if (y==(height-1)){
					for (int i=-1; i <= 0 ; i++){
						for (int j =-1; j<=1 ; j++){
							values[index] = inputData[(y+i)*step+(x+j)];
							index=index+1;
						}
					}
					values[6] = inputData[(y-1)*step+(x-1)];
					values[7] = inputData[(y-1)*step+(x)];
					values[8] = inputData[(y-1)*step+(x+1)];
				}
			}
			else{
				for (int i=-1; i <= 1 ; i++){
					for (int j =-1; j<=1 ; j++){
						values[index] = inputData[(y+i)*step+(x+j)];
						//values[index] = inputData[3*step+2];
						index=index+1;
					}
				}
			}	

		    // On applique l'optimisation des 7 tris

		    for (int z=0; z<3; z++){
		    	value1[z]=values[z]; // ligne 1 de la matrice 
		    	value2[z]=values[z+3]; // ligne 2 de la matrice 
		    	value3[z]=values[z+6]; // ligne 3 de la matrice
		    }
		    
		    int size = sizeof(value1) / sizeof(value1[0]);
		    
			// tri des 3 lignes 
		    selectionSort(value1,size);
		    selectionSort(value2,size);
		    selectionSort(value3,size);
		    
		    //std::sort(value1, value1 + 3);
		    //std::sort(value2, value2 + 3);
		    //std::sort(value3, value3 + 3);
		    
			// création des 3 colonnes de la matrice 
		    value4[0]=value1[0];
		    value4[1]=value2[0];
		    value4[2]=value3[0];
		    
		    value5[0]=value1[1];
		    value5[1]=value2[1];
		    value5[2]=value3[1];

		    value6[0]=value1[2];
		    value6[1]=value2[2];
		    value6[2]=value3[2];
		    
			// tri des 3 colonnes 
		    selectionSort(value4,size);
		    selectionSort(value5,size);
		    selectionSort(value6,size);
		    
		    // creation du tableau contenant la diagonale
		    value7[0]=value4[0];
		    value7[1]=value5[1];
		    value7[2]=value6[2];
		
			// tri de la diagonale 
		   selectionSort(value7,size);
		    // remplacement de la valeur du pixel par la médiane 
		    outputData[y*step+x] = value7[1];
		}
		
		
	}
}

void applySobelFilter(int width_in, int height_in, int step_in, unsigned char* imagedata_in, unsigned char* imagedata_out, int kernel[3][3]) {
    // parcourir tous les pixels de l'image sauf les bords
    for (int i = 1; i < height_in - 1; i++) { 		// on parcours les lignes sauf la 1 et la height-1
        for (int j = 1; j < width_in - 1; j++) {	// on parcours les conolles sauf la 1 et la width-1

            int sumx = 0;		// initialisation de la somme des produits de convolution horizontal
            int sumy = 0;		// initialisation de la somme des produits de convoliution vertical
            
			// appliquer la convolution
            for (int m = -1; m <= 1; m++) {			// on parcours les lignes des pixels voisins 
                for (int n = -1; n <= 1; n++) {		// on parcours les colonnes des pixels voisins
		    		int x = i + m;
		    		int y = j + n;
		    		int index = x*step_in + y; // on récupère la position du pixel
			    	int pixelvalue = (unsigned char)imagedata_in[index];

		            sumx += pixelvalue * kernel[m + 1][n + 1];  //On applique la convolution horizontal
		            sumy += pixelvalue * kernel[n + 1][m + 1];  //On applique la convolution vertical    
                }
            }
        // mise a jour de la valeur du pixel dans l'image de sortie
	    imagedata_out [width_in*i + j] = sqrt(sumx * sumx + sumy * sumy); // on combine le filtre horizontal et vertical
        }
    }
}



int main()
{
	
	long clk_tck = CLOCKS_PER_SEC;
	clock_t t1, t2;
	double val = 0;
	double moy =0;

    // Touche clavier pour quitter
    char ESC_keyboard;
	// intialisation de la variable ESC_keyboard
	ESC_keyboard = cvWaitKey(5);

    // Images
     IplImage *Image_IN;
     IplImage *Image_OUT;
     IplImage *Image_OUT_median;
     IplImage *Image_OUT_sobel;
	 
    // Capture vidéo
    CvCapture *capture;
	
	// variables images
	int height,width,step,channels;		// Taille de l'image
	uchar *Data_in;			// pointeur des données Image_IN
	uchar *Data_out;			// pointeur des données Image_OUT
	uchar *Data_out_median;		// pointeur des données Image_OUT_median
	uchar *Data_out_sobel;		// pointeur des données Image_OUT_sobel
	int i,j,k;				// indices
 
    // Ouvrir le flux vidéo
    //capture = cvCreateFileCapture("/path/to/your/video/test.avi"); // chemin pour un fichier
    //capture = cvCreateFileCapture("/dev/v4l/by-id/usb-046d_HD_Pro_Webcam_C920_*");
    //capture = cvCreateFileCapture("/dev/v4l/by-path/platform-ci_hdrc.1-usb-0\:1.1\:1.0-video-index0");
    //capture = cvCreateFileCapture("/dev/v4l/by-path/platform-ci_hdrc.1-usb-0\:1.1\:1.0-video-index0");
    capture = cvCreateCameraCapture( CV_CAP_ANY );
    //capture = cvCreateCameraCapture( 4 );
    
    // Vérifier si l'ouverture du flux est ok
    if (!capture) {
 
       printf("Ouverture du flux vidéo impossible !\n");
       return 1;
 
    }
 
    // Définition des fenêtres
    cvNamedWindow("Image_OUT_Window", CV_WINDOW_AUTOSIZE); 	// Image_OUT
    cvNamedWindow("Image_IN_Window", CV_WINDOW_AUTOSIZE);   // Image_IN
    cvNamedWindow("Image_OUT_median_Window", CV_WINDOW_AUTOSIZE); // Image_OUT_median
    cvNamedWindow("Image_OUT_sibel_Window", CV_WINDOW_AUTOSIZE); // Image_OUT_median
	
    // Positionnement des fenêtres
    cvMoveWindow("Image_IN_Window", 0,0);
    cvMoveWindow("Image_OUT_Window", 0,450);
    cvMoveWindow("Image_OUT_median_Window", 650,0);
    cvMoveWindow("Image_OUT_sobel_Window", 650,450);
 
	// Première acquisition
   Image_IN = cvQueryFrame(capture); 
	
   // Création de l'image de sortie
   Image_OUT = cvCreateImage(cvSize(Image_IN->width,Image_IN->height),  IPL_DEPTH_8U, 1); 	
   int step_gray = Image_OUT->widthStep/sizeof(uchar);
   Image_OUT_median = cvCreateImage(cvSize(Image_IN->width,Image_IN->height),  IPL_DEPTH_8U, 1); 
   Image_OUT_sobel = cvCreateImage(cvSize(Image_IN->width,Image_IN->height),  IPL_DEPTH_8U, 1); 
   
    // Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(ESC_keyboard != 'q' && ESC_keyboard != 'Q') {
   
     	
    	for(int t=0; t<100; t++){ // boucle pour faire les 100 itération pour calculer le temps d'exécution 

		// On récupère une Image_IN
		Image_IN = cvQueryFrame(capture);
		// Dimension
		height    = Image_IN->height;
		width     = Image_IN->width;
		// distance entre les deux premiers pixels de lignes successives
		step      = Image_IN->widthStep;
		channels  = Image_IN->nChannels;
		// initialisation des pointeurs de donnée
		Data_in      = (uchar *)Image_IN->imageData; 
		Data_out = (uchar *) Image_OUT->imageData;
		Data_out_median = (uchar *) Image_OUT_median->imageData;
		Data_out_sobel = (uchar *) Image_OUT_sobel->imageData;


		// ajout des lignes de bruit 
		for(i=0;i<height;i++)
			{Data_in[i*step+200] = 1;}
		for(j=0;j<width;j++)
			{Data_in[200*step+j] = 1;}


		//conversion RGB en niveau de gris
		for(i=0;i<height;i++) 
			{ for(j=0;j<width;j++)
 				{ 
 Data_out[i*step_gray+j]=0.114*Data_in[i*step+j*channels+0]+ 0.587*Data_in[i*step+j*channels+1] + 0.299*Data_in[i*step+j*channels+2];
Data_out_median[i*step_gray+j]=0.114*Data_in[i*step+j*channels+0]+ 0.587*Data_in[i*step+j*channels+1] + 0.299*Data_in[i*step+j*channels+2];
}}

        // Noyaux Sobel pour les contours horizontaux et verticaux
        int sobelKernel_Rob3[3][3] = { {1, 1, 1}, {0, 0, 0}, {-1, -1, -1} };
        int sobelKernel_Rob4[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} }; // noyau classique vue en cours 
	 	int sobelKernel_Kirsch[3][3] = { {5, 5, 5}, {-3, 0, -3}, {3, 3, 3} };

	t1 = clock();
	
	//MedianFilter_zero_padding_std_sort(Data_out, Data_out_median, step_gray, width, height);



	MedianFilter_AvecBord_SYM_tri_classique_7tris(Data_out, Data_out_median, step_gray, width, height);

	//MedianFilter_zero_padding_tri_classique(Data_out, Data_out_median, step_gray, width, height);
	//MedianFilter_zero_padding_bubble_sort(Data_out, Data_out_median, step_gray, width, height);
	//MedianFilter_zero_padding_std_sort(Data_out, Data_out_median, step_gray, width, height);
	//MedianFilter_sans_bord_bubble_sort(Data_out, Data_out_median, step_gray, width, height);
	//MedianFilter_sans_bord_std_sort(Data_out, Data_out_median, step_gray, width, height);
	//MedianFilter_AvecBord_SYM_std_sort(Data_out, Data_out_median, step_gray, width, height);
	//MedianFilter_AvecBord_SYM_bubble_sort(Data_out, Data_out_median, step_gray, width, height);
	
	applySobelFilter(width, height,step_gray ,Data_out_median ,Data_out_sobel ,sobelKernel_Rob3);
	


	
	t2 = clock();
	
	val = (double)(t2-t1)/(double)clk_tck;
	moy = moy + val;
	std::cout<<"val";
	std::cout<< t ;
	std::cout<<": ";
	std::cout<<val;
	std::cout<<"\n";
	

	

		// On affiche l'Image_IN dans une fenêtre
		cvShowImage( "Image_IN_Window", Image_IN);
		// On affiche l'Image_OUT dans une deuxième fenêtre
		cvShowImage( "Image_OUT_Window", Image_OUT);
		// On affiche l'Image_OUT dans une troisième fenêtre
		cvShowImage( "Image_OUT_median_Window", Image_OUT_median);
		// On affiche l'Image_OUT dans une troisième fenêtre
		cvShowImage( "Image_OUT_sobel_Window", Image_OUT_sobel);
 
		// On attend 5ms
		ESC_keyboard = cvWaitKey(5);
		
	}
 

 	
 	moy = moy*10; // diviser par 100 car 100 mesures mais multiplier par 1000 pour avoir un affichage en ms 
 	std::cout<<"moyenne des temps d'éxécution : \n";
 	std::cout<<moy;
 	std::cout<<"\n";
 	
 	break;
 	
    }
 
    // Fermeture de l'acquisition Vidéo
	cvReleaseCapture(&capture);
   
	// Fermeture des fenêtres d'affichage
	cvDestroyWindow("Image_IN_Window");
    cvDestroyWindow("Image_OUT_Window");
    cvDestroyWindow("Image_OUT_median_Window");
 
    return 0;
 
}






