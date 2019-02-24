// ClientTicTac.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <Windows.h>


void  AfficherPlateau(const char* Plateau)
{
	system("cls");
	for (int i = 0; i != 9; ++i)
	{
		if (i % 3 == 0 && i != 0)
		{
			std::cout << std::endl;
		}
		std::cout << Plateau[i] << " ";
	}
}



bool PlateauOccupe(const char* Plateau, int value)
{
	if (value < 0 || value > 8)
		return true;
	if (Plateau[value] == 'X' || Plateau[value] == 'O')
		return true;
	else
		return false;
}

void ChoixPlateau(char* Plateau, char joueur)
{
	int selection;

	do {
		system("cls");
		AfficherPlateau(Plateau);
		std::cout << std::endl << "Choisissez la case a cocher:";
		std::cin >> selection;
		--selection;
		std::cin.sync(); std::cin.clear();

	} while (PlateauOccupe(Plateau, selection));

	Plateau[selection] = joueur;
}


char CheckGagnant(const char* Plateau)
{
	char Type[2] = { 'X', 'O' };
	for (int i = 0; i != 2; ++i)
	{
		if (Plateau[0] == Type[i] && Plateau[1] == Type[i] && Plateau[2] == Type[i] ||
			Plateau[3] == Type[i] && Plateau[4] == Type[i] && Plateau[5] == Type[i] ||
			Plateau[6] == Type[i] && Plateau[7] == Type[i] && Plateau[8] == Type[i] ||

			Plateau[0] == Type[i] && Plateau[3] == Type[i] && Plateau[6] == Type[i] ||
			Plateau[1] == Type[i] && Plateau[4] == Type[i] && Plateau[7] == Type[i] ||
			Plateau[2] == Type[i] && Plateau[5] == Type[i] && Plateau[8] == Type[i] ||

			Plateau[0] == Type[i] && Plateau[4] == Type[i] && Plateau[8] == Type[i] ||
			Plateau[6] == Type[i] && Plateau[4] == Type[i] && Plateau[2] == Type[i]
			)
			return Type[i];
	}

	return ' ';
}


int StartWs()
{
	WSAData wsa;
	return WSAStartup(MAKEWORD(2, 0), &wsa);
}

int main()
{
	char nomAdversaire[30];
	char monNom[30];
	std::cout << "Entrez votre nom:";
	std::cin >> monNom;

	SOCKET sConnection;
	SOCKADDR_IN sinType;
	int codeRetour = StartWs();
	if (codeRetour != 0)
		return 1;

	sConnection = socket(AF_INET, SOCK_STREAM, 0);
	if (codeRetour == INVALID_SOCKET)
		return 1;

	memset(&sinType, 0, sizeof(sinType));
	sinType.sin_addr.s_addr = inet_addr("127.0.0.1");
	sinType.sin_family = AF_INET;
	sinType.sin_port = htons(12345);

	codeRetour = connect(sConnection, (SOCKADDR*)&sinType, sizeof(sinType));
	if (codeRetour == SOCKET_ERROR)
		return 1;

	int  JoueurCommence;
	char JoueurType;

	std::cout << "Attente de joueurs pour debuter la partie" << std::endl;

	codeRetour = recv(sConnection, &JoueurType, 1, 0);
	if (codeRetour == SOCKET_ERROR || codeRetour == 0)
		return 1;

	std::cout << "Type de symbole pour joueur recu " << JoueurType << " Obtenir" << std::endl;

	if (JoueurType == 'X')
		JoueurCommence = 0;
	else
		JoueurCommence = 1;

	codeRetour = send(sConnection, monNom, strlen(monNom), 0);
	if (codeRetour == SOCKET_ERROR || codeRetour == 0)
		return 1;

	codeRetour = recv(sConnection, nomAdversaire, 30, 0);
	if (codeRetour == SOCKET_ERROR || codeRetour == 0)
		return 1;

	nomAdversaire[codeRetour] = '\0';
	std::cout << "Nom de l'adversaire: " << nomAdversaire << std::endl;

	char Plateau[30] = { '1','2','3','4','5','6','7','8', '9' };
	int  compteur = 0;
	char Gagnant;

	std::cout << "La partie commence" << std::endl;
	AfficherPlateau(Plateau);
	while (compteur < 9)
	{
		if (JoueurCommence == 0 || JoueurCommence % 2 == 0)
		{
			AfficherPlateau(Plateau);
			ChoixPlateau(Plateau, JoueurType);
			codeRetour = send(sConnection, Plateau, strlen(Plateau), 0);
			if (codeRetour == SOCKET_ERROR || codeRetour == 0)
				return 1;

			AfficherPlateau(Plateau);

	
			Gagnant = CheckGagnant(Plateau);
			if (Gagnant == JoueurType)
			{
				std::cout << std::endl << "Vous avez gagne " << std::endl;
				std::cin.sync(); std::cin.clear();
				std::cin.get();
				return 1;
			}
			else if (Gagnant != JoueurType && Gagnant != ' ')
			{
				std::cout << std::endl << nomAdversaire << " a gagné" << std::endl;
				std::cin.sync(); std::cin.clear();
				std::cin.get();
				return 1;
			}

		}
		else if (JoueurCommence == 1 || JoueurCommence % 2 != 0)
		{
			std::cout << std::endl << "C'est le tour de " << nomAdversaire << std::endl;
			codeRetour = recv(sConnection, Plateau, 30, 0);
			if (codeRetour == SOCKET_ERROR || codeRetour == 0)
				return 1;

			Plateau[codeRetour] = '\0';
			AfficherPlateau(Plateau);
			Gagnant = CheckGagnant(Plateau);

			if (Gagnant == JoueurType)
			{
				std::cout << std::endl << "Vous avez gagne ! " << std::endl;
				std::cin.sync(); std::cin.clear();
				std::cin.get();
				return 1;
			}
			else if (Gagnant != JoueurType && Gagnant != ' ')
			{
				std::cout << std::endl << nomAdversaire << " a remporte la partie" << std::endl;
				std::cin.sync(); std::cin.clear();
				std::cin.get();
				return 1;
			}

		}

		++JoueurCommence;
		++compteur;
	}

	std::cout << "C'est indecis..." << std::endl;
	std::cin.sync(); std::cin.clear();
	std::cin.get();

	closesocket(sConnection);
	WSACleanup();
	return 0;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
