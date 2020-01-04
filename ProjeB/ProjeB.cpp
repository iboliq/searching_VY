#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>

#include "rapidxml.hpp"      //XML HTML PHP açmak için 3. parti kütüphane kullanıldı
#include "rapidxml_utils.hpp"

using namespace rapidxml;
using namespace std;

# define NO_OF_CHARS 256 //Kötü karakter dizisi için 
# define MIN(x,y) ((x) < (y) ? (x) : (y)) 


struct node {
	string data;
	node* next = NULL;
};
struct que {
	int cnt = 0;
	node* front = NULL;
	node* rear = NULL;
};

que* enqueue(que* q, string x) {
	if (q->rear == NULL)
	{
		q->rear = new node;
		q->front = q->rear;
	}
	else
	{
		q->rear->next = new node;
		q->rear = q->rear->next;
	}
	q->rear->data = x;
	q->rear->next = NULL;
	q->cnt++;
	return q;
}
que* dequeue(que* q) {
	if (q->front == NULL)
		cout << "eleman yok";
	else
	{
		node* yrd;
		yrd = q->front;
		q->front = yrd->next;
		q->cnt--;
		yrd->next = NULL;
	}
	return q;
}


void badCharHeuristic(string str, int size, int badchar[NO_OF_CHARS])
{
	int i;

	for (i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;

	for (i = 0; i < size; i++)
		badchar[(int)str[i]] = i;
}


int lev(string s1, string s2) {
	int i, j, l1, l2, t, track;
	int dist[50][50];
	//stores the lenght of strings s1 and s2
	l1 = s1.length();
	l2 = s2.length();
	for (i = 0; i <= l1; i++) {
		dist[0][i] = i;
	}
	for (j = 0; j <= l2; j++) {
		dist[j][0] = j;
	}
	for (j = 1; j <= l1; j++) {
		for (i = 1; i <= l2; i++) {
			if (s1[i - 1] == s2[j - 1]) {
				track = 0;
			}
			else {
				track = 1;
			}
			t = MIN((dist[i - 1][j] + 1), (dist[i][j - 1] + 1));
			dist[i][j] = MIN(t, (dist[i - 1][j - 1] + track));
		}
	}
	return dist[l2][l1];
}
void searchfilet(que* q, string pat)
{
	int patUzunluk = pat.size();

	int badchar[NO_OF_CHARS];
	badCharHeuristic(pat, patUzunluk, badchar);

	int elemanSay = 0, small = 5;
	string didYouMean;
	bool find = false, tames = false;
	ofstream fileind("indis.txt", ios::app);
	fileind << endl << pat << " kelimesi için arama sonuçları: " << endl;
	while (q->cnt != 0)
	{
		int kelUzunluk = q->front->data.length();
		int kelimeIndis = 0;
		string txt = q->front->data;
		if (txt == pat)
		{
			cout << "Tam eşleşen kelimenin sırası: " << elemanSay << endl;
			fileind << "Tam eşleşen kelimenin '" << txt << "' kelimesinin indisi: " << elemanSay << endl;
			find = true;
			tames = true;
		}
		else
		{
			while (kelimeIndis <= (kelUzunluk - patUzunluk))
			{
				int j = patUzunluk - 1;
				while (j >= 0 && pat[j] == txt[kelimeIndis + j])
					j--;

				if (j >= 0)
				{
					kelimeIndis += max(1, j - badchar[txt[kelimeIndis + j]]);
				}
				else
				{
					cout << "Aranan ifadeyi içeren kelimenin sırası: " << elemanSay << endl;
					fileind << "Aranan ifadeyi iceren '" << txt << "' kelimesinin indisi: " << elemanSay << endl;
					find = true;
					break;
				}
				if (tames == false)//eger tam eslesmiyosa
				{
					int x = lev(txt, pat);
					if (x < small)
					{
						small = x;
						didYouMean = txt;
					}
				}

			}
		}

		elemanSay++;
		dequeue(q);
	}
	if (tames == false && didYouMean != "")
	{
		cout << endl << "Bunu mu demek istediniz: " << didYouMean << endl << endl << endl;
	}
	if (find == false)
	{
		cout << endl << "Aranan kelime dosyada bulunmuyor." << endl << endl;
	}
}

string FindIndis(que* q, int x)
{
	int i = 0;
	node* yrd;
	yrd = q->front;
	while (i != x)
	{
		yrd = yrd->next;
		i++;
	} 
	return "Aranan indisteki kelime: " + yrd->data +"\n\n";
}

void emptyQue(que* q) {

	while (q->cnt != 0)
	{
		dequeue(q);
	}
	q = NULL;
	delete q;
}

int main()
{
	setlocale(LC_ALL, "Turkish");

	char choice = ' ';
	while (choice != 'e')
	{
		cout << "1)TXT\n2)XML\n3)HTML\n4)PHP\ne)Çıkış\nSecim yapiniz:";
		cin >> choice;
		switch (choice)
		{
		case '1':
		{
			que* q = new que;
			ifstream file("BigData.txt", ios::in);
			string pat, s;

			while (!file.eof())
			{
				file >> s;
				q = enqueue(q, s);
			}

			cout << "a)Kelimeye göre ara\nb)İndise göre ara\nYapılacak işlemi seçiniz: ";
			cin >> choice;
			switch (choice)
			{
			case 'a':
			{
				cout << "Aranacak kelimeyi giriniz: ";
				cin.ignore();
				getline(cin, pat);
				
				clock_t start;
				double duration;
				start = clock();

				searchfilet(q, pat);

				duration = (clock() - start) / (double)CLOCKS_PER_SEC;

				cout <<"Arama süresi: " << duration << " saniye\n";
				break;
			}
			case 'b':
			{
				int indis;
				cout << "İndis giriniz: ";
				cin >> indis;
				cout << FindIndis(q, indis);
				break;
			}
			default:
				break;
			}
			emptyQue(q);

			break;
		}
		case '2':
		{
			file<> xmlFile("xml.xml");
			xml_node<>* root_node;
			xml_document<> doc;
			doc.parse<0>(xmlFile.data());
			root_node = doc.first_node("body");
			que* q = new que;
			char c = ' ';
			string s, pat;
			int i = 0;
			while (c != '\0')
			{
				c = root_node->value()[i];
				if (c != ' ')
				{
					s += c;
				}
				else
				{
					q = enqueue(q, s);
					s = "";
				}
				i++;
			}
			cout << "a)Kelimeye göre ara\nb)İndise göre ara\nYapılacak işlemi seçiniz: ";
			cin >> choice;
			switch (choice)
			{
			case 'a':
			{
				cout << "Aranacak kelimeyi giriniz: ";
				cin.ignore();
				getline(cin, pat);
				clock_t start;
				double duration;
				start = clock();

				searchfilet(q, pat);

				duration = (clock() - start) / (double)CLOCKS_PER_SEC;

				cout << "Arama süresi: " << duration << " saniye\n";
				break;
			}
			case 'b':
			{
				int indis;
				cout << "İndis giriniz: ";
				cin >> indis;
				cout << FindIndis(q, indis);
				break;
			}
			default:
				break;
			}
			emptyQue(q);
			break;
		}
		case '3':
		{
			file<> xmlFile("index.html");
			xml_node<>* root_node;
			xml_document<> doc;
			doc.parse<0>(xmlFile.data());
			root_node = doc.first_node("body");
			que* q = new que;
			char c = ' ';
			string s, pat;
			int i = 0;
			while (c != '\0')
			{
				c = root_node->value()[i];
				if (c != ' ')
				{
					s += c;
				}
				else
				{
					q = enqueue(q, s);
					s = "";
				}
				i++;
			}
			cout << "a)Kelimeye göre ara\nb)İndise göre ara\nYapılacak işlemi seçiniz: ";
			cin >> choice;
			switch (choice)
			{
			case 'a':
			{
				cout << "Aranacak kelimeyi giriniz: ";
				cin.ignore();
				getline(cin, pat); 
				clock_t start;
				double duration;
				start = clock();

				searchfilet(q, pat);

				duration = (clock() - start) / (double)CLOCKS_PER_SEC;

				cout << "Arama süresi: " << duration << " saniye\n";
				break;
			}
			case 'b':
			{
				int indis;
				cout << "İndis giriniz: ";
				cin >> indis;
				cout << FindIndis(q, indis);
				break;
			}
			default:
				break;
			}
			emptyQue(q);
			break;
		}
		case '4':
		{
			file<> xmlFile("php.php");
			xml_node<>* root_node;
			xml_document<> doc;
			doc.parse<0>(xmlFile.data());
			root_node = doc.first_node("body");
			que* q = new que;
			char c = ' ';
			string s, pat;
			int i = 0;
			while (c != '\0')
			{
				c = root_node->value()[i];
				if (c != ' ')
				{
					s += c;
				}
				else
				{
					q = enqueue(q, s);
					s = "";
				}
				i++;
			}
			cout << "a)Kelimeye göre ara\nb)İndise göre ara\nYapılacak işlemi seçiniz: ";
			cin >> choice;
			switch (choice)
			{
			case 'a':
			{
				cout << "Aranacak kelimeyi giriniz: ";
				cin.ignore();
				getline(cin, pat);
				clock_t start;
				double duration;
				start = clock();

				searchfilet(q, pat);

				duration = (clock() - start) / (double)CLOCKS_PER_SEC;

				cout << "Arama süresi: " << duration << " saniye\n";
				break;
			}
			case 'b':
			{
				int indis;
				cout << "İndis giriniz: ";
				cin >> indis;
				cout << FindIndis(q, indis);
				break;
			}
			default:
				break;
			}
			emptyQue(q);
			break;
		}
		default:
			break;
		}

	}
}
