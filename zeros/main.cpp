#include <iostream>
#include <math.h>
#include <omp.h>
#include <windows.h>

using namespace std;

double f(double x)
{
double wynik=x*x-3;

return wynik;
}

int main()
{
	long double a,b,x,dokladnosc,skokPodzialu;
	int iloscWatkow,j;
	//cout << "Podaj Poczatek" << endl;
	//cin >> start;
	//cout << "Podaj Koniec" << endl;
	//cin >> koniec;
	//cout << "Podaj dokladnosc" << endl;
	//cin >> dokladnosc;
	
	a=1;
	b=11;
	x=0;
	dokladnosc=0.0001;
	omp_set_num_threads(4); //ustawianie ilosci watkow
	iloscWatkow = omp_get_max_threads(); //pobieranie maksymalnej ilosc watkow
	//float Start = GetTickCount(); // Pocz¹tek czasu dzia³ania programu
	long double * start = (long double*) malloc(iloscWatkow * sizeof(long double)); //stworzenie tablicy poczatków przedzia³u dla wszystkich w¹tków
	long double * koniec = (long double*) malloc(iloscWatkow * sizeof(long double)); //stworzenie tablicy konców przedzia³u dla wszystkich w¹tków

	skokPodzialu = (b - a) / iloscWatkow;
	for(int i=0; i<iloscWatkow; i++ )
	{
		start[i] = (i) * skokPodzialu + a;
		koniec[i] = (i+1) * skokPodzialu + a;
	}

	if (f(a)*f(b)>0)
		{
			cout << "Brak pierwiastka w podanym przedziale!"<<endl;
			exit(1);
		}
	double start_time = omp_get_wtime();

	while (abs(b-a) > dokladnosc)
	{
		#pragma omp parallel private (j)
		{
			j = omp_get_thread_num();
			if (f(start[j])*f(koniec[j])<0)
			{
				a=start[j];
				b=koniec[j];
			}
		}

		x = (a+b)/2;
		if (abs(f(x)) < dokladnosc)
		{
			break;
		}

		if (iloscWatkow == 1)
		{
			skokPodzialu = (b - a) / 2;
			if (f(a)*f(x)<0)
				b=x;
			else
				a=x;
		}
		else
		{
			skokPodzialu = (b - a) / iloscWatkow;
		}

		for(int i=0; i<iloscWatkow; i++ )
		{
			start[i] = (i) * skokPodzialu + a;
			koniec[i] = (i+1) * skokPodzialu + a;
		}
	}
	double end_time = omp_get_wtime();
	cout << "Miejscem zerowym jest: " << x;
	cout << "\n\n";
	cout << "\nCzas dzialania programu " << (end_time-start_time) << "ms";
	cout << "\n\n";
	system("pause");
	return 0;
}