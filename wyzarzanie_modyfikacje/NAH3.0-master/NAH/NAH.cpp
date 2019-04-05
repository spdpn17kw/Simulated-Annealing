#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include "NAH.h"
#include<cstdlib>


using namespace std;
class Param {
public:
	vector<int> _order;
	int _cmax;
};
class Task {
public:
	int time; //ile trwa zadanie
	int ex; // ktore to zadanie
	int wart_sciezki;
	int wart_sciezki_wch;
	Task() {
		time = 0;
		ex = 0;
		wart_sciezki = 0;
		wart_sciezki_wch = 0;
	}
	Task(int t, int e, int k,int p) {
		time = t;
		ex = e;
		wart_sciezki = k;
		wart_sciezki = p;
	}

	Task(int t, int e) {
		time = t;
		ex = e;
	}
};
void delete_task(vector<vector<Task>> &macierz, int nr) {
	int size = macierz.size() - 1; //3
	macierz[nr].clear();
	for (int i = nr; i < size; i++)
		macierz[i].swap(macierz[i + 1]);
	macierz.resize(size);
}
//void print_matrix_time(vector<vector<Task>> macierz) {
//	for (int j = 0; j < macierz.size(); ++j) {
//		for (int i = 0; i < macierz[j].size(); ++i)
//			cout << macierz[j][i].time << " ";
//		cout << "\n";
//	}
//}
//void size_of_matrix(vector<vector<Task>> macierz) {
//	//cout << "wymiary macierzy: [" << macierz.size() << "][" << macierz[0].size() << "]" << endl;
//}

vector<vector<Task>> read_data(int &number_of_ex, int &n_m) {
	ifstream data("data.txt");
	data >> number_of_ex;
	data >> n_m;
	vector<vector<Task>> macierz(number_of_ex, vector<Task>(n_m));
	for (int j = 0; j < number_of_ex; j++) {
		for (int i = 0; i < n_m; i++) {
				data >> macierz[j][i].time;
				macierz[j][i].ex = j + 1;
		}
	}
	data.close();
	return macierz;
}
//int cmax(vector<int> order, vector<vector<Task>> macierz, vector<vector<int>> Cm, int number_of_ex, int n_m) {
//	int n_ex = number_of_ex + 1;
//	for (int i = 0; i <= n_m; i++) Cm[0][i] = 0;
//	for (int i = 0; i < n_ex; i++) Cm[i][0] = 0;
//	for (int i = 1; i <= n_m; i++) {
//		for (int j = 1; j <= number_of_ex; j++) {
//			Cm[j][i] = max(Cm[j][i - 1], Cm[j - 1][i]) + macierz[order[j - 1] - 1][i - 1].time;
//		}
//
//	}
//	return Cm[n_ex - 1][n_m];
//}
vector<int> sort(vector<int> sum_time) {
	vector<int> order(sum_time.size());
	vector<int>::iterator it;
	int b = 0;
	for (int i = 0; i < sum_time.size(); i++) {
		it = max_element(sum_time.begin(), sum_time.end());
		order[b] = distance(sum_time.begin(), it)+1;  //kolejnosc zadan od jedynki
		*it = 0;
		b++;
	}
	return order;
}
int cmax(vector<vector<Task>> macierz, vector<int> order) {
	int n_ex = order.size()+1;
	int n_m = macierz[0].size()+1;
	vector<vector<int>> Cm(n_ex, vector<int>(n_m));
	for (int i = 0; i < n_m; i++) Cm[0][i] = 0;
	for (int i = 0; i < n_ex; i++) Cm[i][0] = 0;
	for (int i = 1; i < n_m; i++) {
		for (int j = 1; j < n_ex; j++) {
			Cm[j][i] = max(Cm[j][i - 1], Cm[j - 1][i]) + macierz[order[j - 1] - 1][i - 1].time;
		}
	}
	return Cm[n_ex - 1][n_m-1];
}

vector<int> min_element(vector<Param> cmaxy,int size) {
	int min = 999999999;
	vector<int> order(size+1);
	for (int i = 0; i < size+1; i++) {
		if (cmaxy[i]._cmax < min) {
			min = cmaxy[i]._cmax;
			for (int j = 0; j < size+1; j++)
				order[j] = cmaxy[i]._order[j];
		}		
	}
	//cout << "min cmax: " << min<<endl;
	return order;
}
void add_element(vector<int> &order, int element, int position) {
	vector<int> pom_order = order;
	order[position] = element;
	for (int i = position+1; i < order.size(); i++) {
		order[i] = pom_order[i - 1];
	}
}

vector<int> sort_cmax(vector<vector<Task>> macierz, vector<int> order) {
	vector<int> order_pom = { order[0] };
	vector<Param> cmaxy(order.size());
	for (int i = 0; i < order.size(); i++) { 
		vector<int> order_pompom = order_pom;
		for (int j = 0; j <= i; j++) {	//petla po pozycji
			order_pom = order_pompom;
			order_pom.resize(i + 1);
			add_element(order_pom, order[i], j);
			cmaxy[j]._cmax = cmax(macierz, order_pom);	
			cmaxy[j]._order = order_pom;
			//for (int id = 0; id < order_pom.size(); id++)
				//cout << "order_pom = " << order_pom[id] << endl;
			//cout << "cmaxy[j] = " << cmaxy[j]._cmax << endl;
		}
		order_pom = min_element(cmaxy,i);
	}
	return order_pom;
}

//modyfikacja uwzgledniajaca zapamietanie najlepszego  rozwiazania 
void wyzarzanie(vector<vector<Task>> macierz, vector<int> & order) {
	double T = 1000000000;
	double Tk = 0.0001;
	int zad = 0;
	int zad1 = 0;
	int c = 0;
	int c_prim = 0;
	int c_better = 0;
	double P = 0;
	double u = 0.75;
	double zmienna;
	srand(time(NULL));
	vector<int> betterSolution = order;
	

	while (T > Tk) {
		zad = rand() % order.size();
		zad1 = rand() % order.size();
		//cout << zad1;

		while (zad == zad1)
		{
			zad = rand() % order.size();
			zad1 = rand() % order.size();
		}

		c = cmax(macierz, order);
		swap(order[zad], order[zad1]);
		c_prim = cmax(macierz, order);
		c_better = cmax(macierz, betterSolution); 

		//zapamietanie lepszego rozwiązania
		if (c_prim < c_better) betterSolution = order; 

		if (c < c_prim) {
			P = 1;
		}
		else {
			P = exp((c - c_prim) / T);
		}

		zmienna = static_cast<double>(rand() % 10000) / 10000;
		//cout << zmienna<<" ";
		if (P >= zmienna) {
			T = u * T;
		}
		else {
			swap(order[zad], order[zad1]);
			T = u * T;
		}
		//cout << T << " ";
	}
}

//podstawowe wyzarzanie na 3.0 
void wyzarzanie(vector<vector<Task>> macierz, vector<int> & order) {
	double T=1000000000;
	double Tk = 0.0001;
	int zad = 0;
	int zad1 = 0;
	int c = 0;
	int c_prim = 0;
	double P = 0;
	double u = 0.75;
	double zmienna;
	srand(time(NULL));

	while (T > Tk) {
		zad = rand() % order.size();
		zad1 = rand() % order.size();
		//cout << zad1;

		while ( zad==zad1)
		{
			zad = rand() % order.size();
			zad1 = rand() % order.size();
		}

		c = cmax(macierz, order);
		swap(order[zad], order[zad1]);
		c_prim = cmax(macierz, order);

		if (c < c_prim) {
			P = 1;
		}
		else {
			P = exp((c - c_prim) / T);
		}
		
		zmienna = static_cast<double>(rand() % 10000) / 10000;
		//cout << zmienna<<" ";
		if (P >= zmienna) {
			T = u * T;
		}
		else {
			swap(order[zad], order[zad1]);
			T = u * T;
		}
		//cout << T << " ";
	}
}

//modyikacja ktora rozwaza jedynie wieksze lub mniejsze C_max od C_max_prim
void wyzarzanie_cmax_rozne(vector<vector<Task>> macierz, vector<int> & order) {
	double T = 1000000000;
	double Tk = 0.0001;
	int zad = 0;
	int zad1 = 0;
	int c = 0;
	int c_prim = 0;
	double P = 0;
	double u = 0.75;
	double zmienna;
	srand(time(NULL));

	while (T > Tk) {
		zad = rand() % order.size();
		zad1 = rand() % order.size();
		//cout << zad1;

		while (zad == zad1)
		{
			zad = rand() % order.size();
			zad1 = rand() % order.size();
		}

		c = cmax(macierz, order);
		swap(order[zad], order[zad1]);
		c_prim = cmax(macierz, order);

		if (c < c_prim) {
			P = 1;
		}// zmiana c != C_prim
		else if (c > c_prim) {
			P = exp((c - c_prim) / T);
		}

		zmienna = static_cast<double>(rand() % 10000) / 10000;
		//cout << zmienna<<" ";
		if (P >= zmienna) {
			T = u * T;
		}
		else {
			swap(order[zad], order[zad1]);
			T = u * T;
		}
		//cout << T << " ";
	}
}

// modyfikacja dla pominięcia prawdopodobienstwa rownego 1 
void wyzarzanie_prawdopodobienstwo(vector<vector<Task>> macierz, vector<int> & order) {
	double T = 1000000000;
	double Tk = 0.0001;
	int zad = 0;
	int zad1 = 0;
	int c = 0;
	int c_prim = 0;
	double P = 0;
	double u = 0.75;
	double zmienna;
	srand(time(NULL));

	while (T > Tk) {
		zad = rand() % order.size();
		zad1 = rand() % order.size();
		//cout << zad1;

		while (zad == zad1)
		{
			zad = rand() % order.size();
			zad1 = rand() % order.size();
		}

		c = cmax(macierz, order);
		swap(order[zad], order[zad1]);
		c_prim = cmax(macierz, order);

		//pominiecie warunku gdy prawdopodobienstwo rowne jest 1
		if (c > c_prim) P = exp((c - c_prim) / T);
		
		zmienna = static_cast<double>(rand() % 10000) / 10000;
		// cout << zmienna<<" ";
		if (P >= zmienna) {
			T = u * T;
		}
		else {
			swap(order[zad], order[zad1]);
			T = u * T;
		}
		//cout << T << " ";
	}
}

//modyfikacja do badania wspólczynnika u 
void wyzarzanie(vector<vector<Task>> macierz, vector<int> & order,double u) {
	double T = 1000000000;
	double Tk = 0.0001;
	int zad = 0;
	int zad1 = 0;
	int c = 0;
	int c_prim = 0;
	double P = 0;
	//double u = 0.75;
	double zmienna;
	srand(time(NULL));

	while (T > Tk) {
		zad = rand() % order.size();
		zad1 = rand() % order.size();
		//cout << zad1;

		while (zad == zad1)
		{
			zad = rand() % order.size();
			zad1 = rand() % order.size();
		}

		c = cmax(macierz, order);
		swap(order[zad], order[zad1]);
		c_prim = cmax(macierz, order);

		if (c < c_prim) {
			P = 1;
		}
		else {
			P = exp((c - c_prim) / T);
		}

		zmienna = static_cast<double>(rand() % 10000) / 10000;
		//cout << zmienna<<" ";
		if (P >= zmienna) {
			T = u * T;
		}
		else {
			swap(order[zad], order[zad1]);
			T = u * T;
		}
		//cout << T << " ";
	}
}

//modyfikacja do badania między swap a insert 
void wyzarzanie_insert(vector<vector<Task>> macierz, vector<int> & order) {
	double T = 1000000000;
	int zad = 0;
	int zad1 = 0;
	int c = 0;
	int c_prim = 0;
	double P = 0;
	double u = 0.75;
	double zmienna;
	srand(time(NULL));

	while (T > 0.0001) {
		zad = rand() % order.size();
		zad1 = rand() % order.size();
		//cout << zad1;

		while (zad == zad1)
		{
			zad = rand() % order.size();
			zad1 = rand() % order.size();
		}

		c = cmax(macierz, order);
		//swap(order[zad], order[zad1]);
		order.insert(order.begin()+zad,order[zad1]);
		//cout << order[zad1];
		if (zad1>zad)
		order.erase(order.begin()+zad1+1);

		if (zad1<zad)
		order.erase(order.begin() + zad1);


		c_prim = cmax(macierz, order);

		if (c < c_prim) {
			P = 1;
		}
		else {
			P = exp((c - c_prim) / T);
		}

		zmienna = static_cast<double>(rand() % 10000) / 10000;
		//cout << zmienna<<" ";
		if (P >= zmienna) {
			T = u * T;
		}
		else {
			//swap(order[zad], order[zad1]);
		    order.insert(order.begin() + zad1, order[zad]);
			if (zad1 > zad)
				order.erase(order.begin() + zad+1 );

			if (zad1 < zad)
				order.erase(order.begin() + zad);
			T = u * T;
		}
		//cout << T << " ";
	}
}

//modyfikacja do badania zależnosci miedzy doborem temp startowej  i koncowej i mozliwosc zmiany wspolczynnika u
void wyzarzanie(vector<vector<Task>> macierz, vector<int> & order, double u, double Tk, int T) {
	//double T = 1000000000;
	int zad = 0;
	int zad1 = 0;
	int c = 0;
	int c_prim = 0;
	double P = 0;
	//double u = 0.75;
	double zmienna;
	srand(time(NULL));

	while (T > Tk) {
		zad = rand() % order.size();
		zad1 = rand() % order.size();
		//cout << zad1;

		while (zad == zad1)
		{
			zad = rand() % order.size();
			zad1 = rand() % order.size();
		}

		c = cmax(macierz, order);
		swap(order[zad], order[zad1]);
		c_prim = cmax(macierz, order);

		if (c < c_prim) {
			P = 1;
		}
		else {
			P = exp((c - c_prim) / T);
		}

		zmienna = static_cast<double>(rand() % 10000) / 10000;
		//cout << zmienna<<" ";
		if (P >= zmienna) {
			T = u * T;
		}
		else {
			swap(order[zad], order[zad1]);
			T = u * T;
		}
		//cout << T << " ";
	}
}

int main()
{

	
	int number_of_ex, n_m;
	vector<vector<Task>> macierz = read_data(number_of_ex, n_m);
	vector<int> sum_time(number_of_ex);  //suma czasow wykonania danego zadania przez wszystkie maszyny
	for (int j = 0; j < number_of_ex; j++) {
		for (int i = 0; i < n_m; i++) {
			sum_time[j] += macierz[j][i].time;
		}
		//cout << "time: " << sum_time[j] << endl;
	}

	vector<int> order;
	for (int i = 0; i < number_of_ex; i++) {
		order.push_back(i + 1);
	}
	
	//vector<int> order;
	vector<int> order1 = sort(sum_time);
 //for (int i = 0; i < order.size(); i++)
	//cout << "czas od max do min = " << order[i] << endl;
	//vector<int> order1 = { 3,1,2 };
   // graf_cmax(macierz, order1);
	//graf_sort(maciez, order);
	//dodanie_zadania(macierz, order1);
	vector<int> order_naj = sort_cmax(macierz, order);
   // for(int i=0;i<order_naj.size();i++)
	//cout << "order_naj = " << order_naj[i] << endl;
	//print_matrix_time(macierz);

	//wyzarzanie(macierz,order);

	chrono::time_point< std::chrono::system_clock> start;
		chrono::time_point< std::chrono::system_clock> end;
		chrono::duration<double> elapsed_seconds;
	char znak=0; 
	char znak1=0;
	
	
	while (znak != 'q' ) {
		cout << "u - wspolczynnik chlodzenia" << endl;
		cout << " i - insert wersja" << endl;
		cout << "q - zakoncz" << endl;
		cout << "t - ustaw temperature poczatkowa i koncowa"<<endl;
		cout << "n - zacznij od ustawienia zadanian w wyniku sortowania algorytmem NEH" << endl;
		cout << "p - wyzarzanie gdzie pomijamy prawdopodobienstwo rowne 1 " << endl; 
		cout << "c - c_maxy rozne od c_max_prim" << endl;


		cin >> znak;
			switch (znak)
		{
		case 'u':
			cout << "1- u=85" << endl;
			cout << "2- u=90"<<endl;
			cout << "3- u= 95"<<endl;
			cout << "4- u=99" << endl;
			cin >> znak1;
			switch (znak1) {
				
				case '1':
				start = std::chrono::system_clock::now();
				wyzarzanie(macierz, order,0.85);
				end = std::chrono::system_clock::now();
				 elapsed_seconds = end - start;
				cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
				cout<<"cmax: "<< cmax(macierz, order)<<endl;
				break;

			case '2':
				 start = std::chrono::system_clock::now();
				wyzarzanie(macierz, order, 0.9);
				 end = std::chrono::system_clock::now();
				elapsed_seconds = end - start;
				cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
				cout << "cmax: " << cmax(macierz, order) << endl;
				break;
			case '3':
				 start = std::chrono::system_clock::now();
				wyzarzanie(macierz, order, 0.95);
				 end = std::chrono::system_clock::now();
			 elapsed_seconds = end - start;
				cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
				cout << "cmax: " << cmax(macierz, order) << endl;
				break;
			case '4':
				 start = std::chrono::system_clock::now();
				wyzarzanie(macierz, order, 0.99);
				end = std::chrono::system_clock::now();
				 elapsed_seconds = end - start;
				cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
				cout << "cmax: " << cmax(macierz, order) << endl;
				break;
			}
			break;

		case 'i':
			 start = std::chrono::system_clock::now();
			 wyzarzanie_insert(macierz, order);
			 end = std::chrono::system_clock::now();
			 elapsed_seconds = end - start;
			cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
			cout << "cmax: " << cmax(macierz, order) << endl;
			break;

		case 't':
			double T;
			double Tk;
			cout << "podaj temperature starową"<<endl;
			cin >> T;
			cout << "podaj temp koncowa" << endl; 
			cin >> Tk; 
			start = std::chrono::system_clock::now();
			wyzarzanie(macierz, order, 0.8,T,Tk);
			end = std::chrono::system_clock::now();
			elapsed_seconds = end - start;
			cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
			cout << "cmax: " << cmax(macierz, order) << endl;
			break;

		case 'n':
			//vector<int> order = sort(sum_time);
			//vector<int> order_naj = sort_cmax(macierz, order);
			order = order_naj;
			start = std::chrono::system_clock::now();
			
			wyzarzanie(macierz, order, 0.8);
			end = std::chrono::system_clock::now();
			chrono::duration<double> elapsed_seconds = end - start;
			cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
			cout << "cmax: " << cmax(macierz, order) << endl;

			break;

		case 'p':
			start = std::chrono::system_clock::now();
			wyzarzanie_prawdopodobienstwo(macierz, order);
			end = std::chrono::system_clock::now();
			elapsed_seconds = end - start;
			cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
			cout << "cmax: " << cmax(macierz, order) << endl;
			break;
		case 'c':
			start = std::chrono::system_clock::now();
			wyzarzanie_cmax_rozne(macierz, order);
			end = std::chrono::system_clock::now();
			elapsed_seconds = end - start;
			cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
			cout << "cmax: " << cmax(macierz, order) << endl;
			break;

		default:
			break;
		}
			for (int i = 0; i < number_of_ex; i++) cout << "kolejnosc: " << order[i] << " " << endl;
	}
	cout << "Przewidywno cmax rowny 6632 wg NEH " << endl; 
		 
    //cout << cmax(macierz, order);
	//for (int i = 0; i < number_of_ex; i++) cout << "kolejnosc: " << order[i] << " " << endl;
	//print_matrix_time(new_macierz);


	system("pause");
	return 0;
}
