#include <iostream>
#include <fstream>
#include <cstring>
#include <climits>
using namespace std;

struct Alumno//Record
{	
	char Nombre [12];
	char Apellidos [12];	
};

ostream & operator << (ostream & stream, Alumno & p)
{	
	stream << p.Nombre;
	stream << p.Apellidos;
	stream << "\n";
	stream << flush;
	return stream;
}

istream & operator >> (istream & stream, Alumno & p)
{	
	stream.get(p.Nombre, 12);		
	stream.get(p.Apellidos, 12);	
	stream.get();
	return stream;
}

void readFromConsole(char buffer[], int size){
	string temp;			
	cin >> temp;
	for(int i=0; i<size; i++)		
		buffer[i] = (i < temp.size())? temp[i] : ' ';
	buffer[size-1] = '\0';
	cin.clear();
}

void testWrite(string filename){
	ofstream outFile;
	//escribir
	outFile.open(filename, ios::app);
	if(outFile.is_open()){		
		//leer desde consola datos para person1
		Alumno person1;	
		cout<<"Nombre:"; readFromConsole(person1.Nombre, 12);
		cout<<"Apellidos:"; readFromConsole(person1.Apellidos, 12);
		outFile << person1;
		outFile.close();
		
	}
	else 
		cerr<<"No se pudo abrir el archivo\n";	
}

void testRead(string filename)
{
	ifstream inFile;
	//leer
	inFile.open(filename);
	Alumno person2;	
	if(inFile.is_open()){
		while(!inFile.eof()){
			inFile >> person2;
			cout << "Nombre:" << person2.Nombre << endl;
			cout << "Apellidos:"<< person2.Apellidos << endl;		
		}	
		inFile.close();
	}
	else 
		cerr<<"No se pudo abrir el archivos\n";
}

int main()
{
	testWrite("test.txt");
	testRead("test.txt");
	return 0;
}