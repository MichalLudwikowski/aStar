// aStar.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

const int rozX = 20, rozY = 20;

void pokaz(int tab[rozX][rozY])
{
	for (int i = 0; i<rozX; i++)
	{
		for (int j = 0; j<rozY; j++)
			cout << tab[i][j];
		cout << endl;
	}
}

void wczytajMape(int grid[rozX][rozY])
{
	ifstream mapa;
	int symbol;
	mapa.open("grid.txt");

	for (int i = 0; i<rozX; i++)
		for (int j = 0; j<rozY; j++)
		{
			mapa >> symbol;
			grid[i][j] = symbol;
		}
	mapa.close();
}

void obrocMape(int grid[rozX][rozY])
{
	int temp[rozX][rozY];

	for (int i = 0; i<rozX; i++)
		for (int j = 0; j<rozY; j++)
		{
			temp[i][j] = grid[i][j];
		}

	for (int i = 0; i<rozX; i++)
		for (int j = 0; j<rozY; j++)
		{
			grid[i][j] = temp[rozX - 1 - i][j];
		}
}

class Pole
{
public:
	int wspX;
	int wspY;
	int g;
	double h;
	double wartoscF;
	Pole *rodzic;

};

int znajdzPoleZMinWartF(vector<Pole> &lista)
{
	double minimum = lista[0].wartoscF;
	for (unsigned int i = 0; i<lista.size(); i++)
	{
		if (minimum>lista[i].wartoscF)
			minimum = lista[i].wartoscF;
	}

	unsigned int pozycja = 0;
	for (pozycja; pozycja<lista.size(); pozycja++)
		if (lista[pozycja].wartoscF == minimum)
			return pozycja;
}

void zaznaczNaMapie(Pole *poz, int grid[rozX][rozY])
{
	do
	{
		grid[poz->wspX][poz->wspY] = 3;
		poz = poz->rodzic;
	} while (poz->rodzic != NULL);
	if (poz->rodzic == NULL)
		grid[poz->wspX][poz->wspY] = 3;
}

int kosztOdStartu(Pole *poz)
{
	int wart = 0;
	while (poz->rodzic != NULL)
	{
		wart++;
		poz = poz->rodzic;
	}
	return wart;
}

double h(Pole *poz, Pole *stop)
{
	return
		sqrt(pow(poz->wspX - stop->wspX, 2) + pow(poz->wspY - stop->wspY, 2));
}

bool jestPozaLista(Pole *poz, vector<Pole> &lista)
{
	for (unsigned int i = 0; i<lista.size(); i++)
		if (poz->wspX == lista[i].wspX && poz->wspY == lista[i].wspY)
			return false;
	return true;
}

void dodajSasiadow(Pole *poz, vector<Pole> &lista, vector<Pole> &listaZam, int grid[rozX][rozY])
{
	if (grid[poz->wspX + 1][poz->wspY] != 5 && poz->wspX<rozX - 1)
	{
		Pole *sasiad = new Pole;
		sasiad->wspX = poz->wspX + 1;
		sasiad->wspY = poz->wspY;
		sasiad->rodzic = poz;
		if (jestPozaLista(sasiad, listaZam))
		{
			lista.push_back(*sasiad);
		}
	}
	if (grid[poz->wspX - 1][poz->wspY] != 5 && poz->wspX>0)
	{
		Pole *sasiad = new Pole;
		sasiad->wspX = poz->wspX - 1;
		sasiad->wspY = poz->wspY;
		sasiad->rodzic = poz;
		if (jestPozaLista(sasiad, listaZam))
		{
			lista.push_back(*sasiad);
		}
	}
	if (grid[poz->wspX][poz->wspY - 1] != 5 && poz->wspY>0)
	{
		Pole *sasiad = new Pole;
		sasiad->wspX = poz->wspX;
		sasiad->wspY = poz->wspY - 1;
		sasiad->rodzic = poz;
		if (jestPozaLista(sasiad, listaZam))
		{
			lista.push_back(*sasiad);
		}
	}
	if (grid[poz->wspX][poz->wspY + 1] != 5 && poz->wspY<rozY - 1)
	{
		Pole *sasiad = new Pole;
		sasiad->wspX = poz->wspX;
		sasiad->wspY = poz->wspY + 1;
		sasiad->rodzic = poz;
		if (jestPozaLista(sasiad, listaZam))
		{
			lista.push_back(*sasiad);
		}
	}
}

void aStar(int grid[rozX][rozY], Pole *start, Pole *stop)
{
	vector<Pole> listaOtwarta;
	vector<Pole> listaZamknieta;
	listaOtwarta.push_back(*start);

	while (listaOtwarta.size() != 0)
	{
		vector<Pole> sasiedzi;
		Pole *temp = new Pole;
		int pozyc = znajdzPoleZMinWartF(listaOtwarta);
		*temp = listaOtwarta[pozyc];
		listaZamknieta.push_back(*temp);
		listaOtwarta.erase(listaOtwarta.begin() + pozyc);
		if (listaZamknieta.back().wspX == (stop->wspX) && listaZamknieta.back().wspY == stop->wspY)
		{
			Pole *ostatni = new Pole;
			*ostatni = listaZamknieta.back();
			zaznaczNaMapie(ostatni, grid);
			delete ostatni;
			return;
		}
		Pole *ostatni = new Pole;
		*ostatni = listaZamknieta.back();
		dodajSasiadow(ostatni, sasiedzi, listaZamknieta, grid);
		for (int i = 0; i<sasiedzi.size(); i++)
		{
			Pole *sasiad = new Pole;
			*sasiad = sasiedzi[i];
			if (jestPozaLista(sasiad, listaOtwarta))
			{
				sasiad->rodzic = ostatni;
				sasiad->g = kosztOdStartu(sasiad);
				sasiad->h = h(sasiad, stop);
				sasiad->wartoscF = sasiad->g + sasiad->h;
				listaOtwarta.push_back(*sasiad);
			}
			else
			{
				if (kosztOdStartu(sasiad)<sasiad->g);
				{
					sasiad->rodzic = ostatni;
					sasiad->g = kosztOdStartu(sasiad);
					sasiad->h = h(sasiad, stop);
					sasiad->wartoscF = sasiad->g + sasiad->h;
				}
			}
		}
	}
	cout << "nie znaleziono drogi" << endl;
}

int main()
{
	Pole *startowe = new Pole;
	startowe->wspX = 0;
	startowe->wspY = 0;
	startowe->rodzic = NULL;
	startowe->wartoscF = 0;

	Pole *koncowe = new Pole;
	koncowe->wspX = 19;
	koncowe->wspY = 19;
	koncowe->rodzic = NULL;

	int siata[rozX][rozY] = {};
	wczytajMape(siata);
	pokaz(siata);
	cout << endl;
	obrocMape(siata);
	aStar(siata, startowe, koncowe);
	obrocMape(siata);
	pokaz(siata);
	return 0;
}

