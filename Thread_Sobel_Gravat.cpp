#include <iostream> // Inclure la bibliothèque standard pour la gestion des entrées/sorties
#include <ctime> // Inclure pour manipuler des fonctions de temps
#include <thread> // Inclure pour gérer les threads
#include <vector> // Inclure pour utiliser le conteneur vector
#include <algorithm> // Inclure pour les fonctions d'algorithmes standard
#include <cmath> // Inclure pour les fonctions mathématiques
#include "cv.h" // Inclure les entêtes de OpenCV pour la gestion des images
#include "highgui.h" // Inclure pour l'interface utilisateur de OpenCV

// Définition d'un alias pour unsigned char pour simplifier la lecture du code
using uchar = unsigned char;

struct Image {
    uchar* data; // Pointeur vers les données de l'image
    int step; // Nombre d'octets par ligne dans l'image
    int cols; // Largeur de l'image en pixels
    int rows; // Hauteur de l'image en pixels
};

void SobelFilterHorizontal(uchar *inputData, uchar *outputData, int step, int width, int height, int startY, int endY) {
    // Applique un filtre de Sobel horizontal sur une partie de l'image
    for (int y = startY; y < endY; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            // Noyau de Sobel pour le gradient horizontal
            int gx = inputData[(y - 1) * step + (x + 1)] - inputData[(y - 1) * step + (x - 1)]
                   + 2 * inputData[y * step + (x + 1)] - 2 * inputData[y * step + (x - 1)]
                   + inputData[(y + 1) * step + (x + 1)] - inputData[(y + 1) * step + (x - 1)];

            // Limite le gradient à 255
            outputData[y * step + x] = std::min(255, std::max(0, gx));
        }
    }
}

void SobelFilterVertical(uchar *inputData, uchar *outputData, int step, int width, int height, int startY, int endY) {
    // Applique un filtre de Sobel vertical sur une partie de l'image
    for (int y = startY; y < endY; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            // Noyau de Sobel pour le gradient vertical
            int gy = inputData[(y - 1) * step + (x - 1)] + 2 * inputData[(y - 1) * step + x] + inputData[(y - 1) * step + (x + 1)]
                   - inputData[(y + 1) * step + (x - 1)] - 2 * inputData[(y + 1) * step + x] - inputData[(y + 1) * step + (x + 1)];

            // Limite le gradient à 255
            outputData[y * step + x] = std::min(255, std::max(0, gy));
        }
    }
}

int main() {
    // Initialisation de la capture vidéo avec n'importe quelle caméra disponible
    CvCapture *capture = cvCaptureFromCAM(CV_CAP_ANY);
    if (!capture) {
        std::cerr << "Erreur lors de l'ouverture du flux vidéo !" << std::endl;
        return 1; // Retourne 1 en cas d'erreur
    }

    // Définition des dimensions de l'image
    const int imageWidth = 640;
    const int imageHeight = 480;

    // Création de la fenêtre d'affichage pour l'image de sortie
    cvNamedWindow("Output Image", CV_WINDOW_AUTOSIZE);

    // Variables pour mesurer le temps d'exécution
    clock_t t1, t2;
    double val = 0;
    double moy = 0;
    long clk_tck = CLOCKS_PER_SEC;

    // Boucle principale pour le traitement d'images
    while (true) {
        // Capture d'une nouvelle image depuis la caméra
        IplImage* frame = cvQueryFrame(capture);
        if (!frame) {
            std::cerr << "Erreur lors de la capture de l'image" << std::endl;
            break; // Sortie de la boucle en cas d'erreur
        }

// Conversion de l'image en niveaux de gris
        IplImage* grayFrame = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
        cvCvtColor(frame, grayFrame, CV_BGR2GRAY);


    // Création des structures pour représenter l'image d'entrée et de sortie
    // Ces structures contiennent les données de l'image, les dimensions et le pas (nombre d'octets par ligne).
    Image inputImage = {reinterpret_cast<uchar*>(grayFrame->imageData), grayFrame->widthStep, grayFrame->width, grayFrame->height};
    Image outputImage = {new uchar[imageWidth * imageHeight], imageWidth, imageWidth, imageHeight};

    // Définition du nombre de threads pour paralléliser le traitement
    // Cela permet d'accélérer le traitement de l'image en utilisant plusieurs cœurs de processeur.
    const int numThreads = 4;
    std::thread threads[numThreads];

    // Calcul de la hauteur de bloc à traiter par chaque thread pour diviser l'image
    int blockHeight = imageHeight / numThreads;

    // Création des threads pour appliquer le filtre de Sobel horizontal
    // Chaque thread traite une portion de l'image pour calculer le gradient horizontal.
    for (int i = 0; i < numThreads; ++i) {
        int startY = i * blockHeight;
        int endY = (i == numThreads - 1) ? imageHeight : (i + 1) * blockHeight;
        threads[i] = std::thread(SobelFilterHorizontal, inputImage.data, outputImage.data, inputImage.step, inputImage.cols, inputImage.rows, startY, endY);
    }

    // Attente de la fin de l'exécution de tous les threads
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    // Répétition du processus avec de nouveaux threads pour le filtre de Sobel vertical
    // Cette étape calcule le gradient vertical de l'image.
    for (int i = 0; i < numThreads; ++i) {
        int startY = i * blockHeight;
        int endY = (i == numThreads - 1) ? imageHeight : (i + 1) * blockHeight;
        threads[i] = std::thread(SobelFilterVertical, inputImage.data, outputImage.data, inputImage.step, inputImage.cols, inputImage.rows, startY, endY);
    }

    // Attente de la fin de l'exécution des threads pour le gradient vertical
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    // Calcul et affichage du temps d'exécution du traitement
    t2 = clock();
    val = static_cast<double>(t2 - t1) / static_cast<double>(clk_tck);
    moy += val;
    std::cout << "Temps d'exécution : " << val << " secondes." << std::endl;

    // Affichage de l'image traitée
    // Conversion de la structure de données de sortie en un format compatible avec HighGUI pour l'affichage.
    IplImage* outputIplImage = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, 1);
    cvSetData(outputIplImage, outputImage.data, imageWidth);
    cvShowImage("Output Image", outputIplImage);

    // Libération de la mémoire allouée pour l'image d'entrée et l'image de sortie
    cvReleaseImage(&grayFrame); // Libère la mémoire allouée pour l'image en niveaux de gris
    delete[] outputImage.data; // Libère la mémoire allouée pour les données de l'image de sortie

    // Attente de la pression de la touche 'q' pour quitter le programme
    char key = cvWaitKey(1);
    if (key == 'q') {
        break; // Sortie de la boucle principale si 'q' est pressé
    }

    // Préparation pour la mesure du temps de la prochaine itération
    t1 = clock();
}

// Libération des ressources et fermeture de la fenêtre de capture vidéo
cvReleaseCapture(&capture); // Libère la capture vidéo
cvDestroyAllWindows(); // Ferme toutes les fenêtres ouvertes par HighGUI

return 0; // Fin du programme

