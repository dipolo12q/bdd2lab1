#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <climits>
using namespace std;

struct Alumno//Record
{	
	char Codigo [5];
    char Nombre [11];
    char Apellidos [20];
    char Carrera [15];	
};

ostream & operator << (ostream & stream, Alumno & p)
{	
	stream << "\n";
	stream.write((char*) &p, sizeof(Alumno));
	//stream << flush;
	return stream;
}

istream & operator >> (istream & stream, Alumno & p)
{	
	stream.read((char*) &p, sizeof(Alumno));
	return stream;
}

class FixedRecord {
private:
    string filename;
public:
    FixedRecord(string filename) {
        this->filename = filename;
    }

    vector<Alumno> load() {
        ifstream file(filename);
        vector<Alumno> result;
        Alumno a;
        try {
            if(!file.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        while(!file.eof()) {
            a = Alumno();
            file >> a;
            file.ignore(1);
            result.push_back(a);
        }
        file.close();
        return result;
    }

    void add(Alumno record) {
        ofstream file(filename, ios::app);
        try {
            if(!file.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        file << record;
        file.close();
    }

    Alumno readRecord(int pos) {
        ifstream file(filename);
        try {
            if(!file.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        Alumno record;
        file.seekg(pos * sizeof(Alumno) + 2*pos, ios::beg);
        file >> record;
        file.close();
        return record;
    }
};

void printAlumno(Alumno alumno) {
    for(int j = 0; j < 5; j++) {
            cout << alumno.Codigo[j];
        }
        cout << " ";
            for(int j = 0; j < 11; j++) {
            cout << alumno.Nombre[j];
        }
        cout << " ";
        for(int j = 0; j < 20; j++) {
            cout << alumno.Apellidos[j];
        }
        cout << " ";
        for(int j = 0; j < 15; j++) {
            cout << alumno.Carrera[j];
    }
    cout << endl;
}

void printAlumnos(vector<Alumno> alumnos) {
    for(int i = 0; i < alumnos.size(); i++) {
        printAlumno(alumnos[i]);
    }
}

char* completeblankspaces(string str, int size) {
    char* result = new char[size];
    for(int i = 0; i < size; i++) {
        result[i] = ' ';
    }
    for(int i = 0; i < str.length(); i++) {
        result[i] = str[i];
    }
    return result;
}

Alumno crearAlumno(vector<string> data){
    Alumno a;
    strcpy(a.Codigo, completeblankspaces(data[0], 5));
    strcpy(a.Nombre, completeblankspaces(data[1], 11));
    strcpy(a.Apellidos, completeblankspaces(data[2], 20));
    strcpy(a.Carrera, completeblankspaces(data[3], 15));
    return a;
}

void testWrite(FixedRecord fr){
    printAlumnos(fr.load());
    vector<string> reg = {"0008", "Franco", "Pacheco Espino", "Computacion"};
    Alumno nuevo = crearAlumno(reg);
    fr.add(nuevo);
    cout << endl;
    //printAlumnos(fr.load());	
}

void testRead(FixedRecord fr)
{
	printAlumnos(fr.load());
    cout << endl;
    printAlumno(fr.readRecord(0));
    printAlumno(fr.readRecord(1));
    printAlumno(fr.readRecord(5));
    printAlumno(fr.readRecord(7));
}

int main()
{
	FixedRecord fr("datos1.txt");
    testWrite(fr);
	testRead(fr);
	return 0;
}
