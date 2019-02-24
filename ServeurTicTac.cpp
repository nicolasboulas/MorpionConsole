// ServeurTicTac.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#pragma comment(lib, "ws2_32.lib")

int StartWs()
{
	WSAData wsa;
	return WSAStartup(MAKEWORD(2, 0), &wsa);
}

#define MAX 2

int fullClientsArray(SOCKET* sarr)
{

	int compteur = 0;
	for (int i = 0; i != MAX; ++i)
	{
		if (sarr[i] != INVALID_SOCKET)
			++compteur;
	}

	return compteur;
}

int main()
{
	SOCKET sConnection;
	SOCKET clients[MAX];
	FD_SET fdSet;
	SOCKADDR_IN sinType;

	int codeRetour = StartWs();
	if (codeRetour != 0)
		return 1;

	sConnection = socket(AF_INET, SOCK_STREAM, 0);
	if (sConnection == INVALID_SOCKET)
		return 1;

	for (int i = 0; i != MAX; ++i)
		clients[i] = INVALID_SOCKET;

	memset(&sinType, 0, sizeof(sinType));
	sinType.sin_addr.s_addr = INADDR_ANY;
	sinType.sin_family = AF_INET;
	sinType.sin_port = htons(12345);


	codeRetour = bind(sConnection, (SOCKADDR*)&sinType, sizeof(sinType));
	if (codeRetour == SOCKET_ERROR)
		return 1;

	codeRetour = listen(sConnection, MAX);
	if (codeRetour == SOCKET_ERROR)
		return 1;

	while (fullClientsArray(clients) != MAX)
	{
		FD_ZERO(&fdSet);
		FD_SET(sConnection, &fdSet);

		codeRetour = select(0, &fdSet, nullptr, nullptr, nullptr);
		if (FD_ISSET(sConnection, &fdSet))
			for (int i = 0; i != MAX; ++i)
				if (clients[i] == INVALID_SOCKET)
					clients[i] = accept(sConnection, nullptr, nullptr);
	}

	std::cout << " Les deux connexions ont été acceptées " << std::endl;
	char GamePlayerTyp[2] = { 'X', 'O' };

	for (int i = 0; i != MAX; ++i)
	{
		codeRetour = send(clients[i], &GamePlayerTyp[i], 1, 0);
		if (codeRetour == SOCKET_ERROR || codeRetour == '=' )
			return 1;
	}

	char client0username[30];
	char client1username[30];

	for (int i = 0; i != MAX; ++i)
	{
		if (i == 0)
		{
			codeRetour = recv(clients[0], client0username, 30, 0);
			if (codeRetour == SOCKET_ERROR || codeRetour == 0)
				return 1;

			client0username[codeRetour] = '\0';
		}
		else if (i == 1)
		{
			codeRetour = recv(clients[1], client1username, 30, 0);
			if (codeRetour == SOCKET_ERROR || codeRetour == 0)
				return 1;

			client1username[codeRetour] = '\0';
		}

	}

	for (int i = 0; i != MAX; ++i)
	{
		if (i == 0)
		{
			codeRetour = send(clients[0], client1username, strlen(client1username), 0);
			if (codeRetour == SOCKET_ERROR || codeRetour == 0)
				return 1;
		}
		else if (i == 1)
		{
			codeRetour = send(clients[1], client0username, strlen(client0username), 0);
			if (codeRetour == SOCKET_ERROR || codeRetour == 0)
				return 1;
		}
	}


	int compteur = 0;
	char Plateau[30];

	while (true)
	{
		FD_ZERO(&fdSet);
		FD_SET(clients[0], &fdSet);
		FD_SET(clients[1], &fdSet);
		codeRetour = select(0, &fdSet, nullptr, nullptr, nullptr);
		if (codeRetour == SOCKET_ERROR || codeRetour == 0)
			return 1;

		for (int i = 0; i != MAX; ++i)
		{
			if (FD_ISSET(clients[i], &fdSet))
			{
				if (i == 0)
				{
					codeRetour = recv(clients[0], Plateau, 30, 0);
					if (codeRetour == SOCKET_ERROR || codeRetour == 0)
						return 1;

					Plateau[codeRetour] = '\0';
					codeRetour = send(clients[1], Plateau, strlen(Plateau), 0);
					if (codeRetour == SOCKET_ERROR || codeRetour == 0)
						return 1;

				}
				else
				{

					codeRetour = recv(clients[1], Plateau, 30, 0);
					if (codeRetour == SOCKET_ERROR || codeRetour == 0)
						return 1;

					Plateau[codeRetour] = '\0';

					codeRetour = send(clients[0], Plateau, strlen(Plateau), 0);
					if (codeRetour == SOCKET_ERROR || codeRetour == 0)
						return 1;
				}

			}
		}



	}

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
