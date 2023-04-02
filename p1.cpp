#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <climits>

using namespace std;

struct Alumno
{
  char codigo [5];
  char nombre [11];
  char apellidos [20];
  char carrera [15];
};


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
            file.read((char*) &a, sizeof(Alumno));
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
        file << "\n";
        file.write((char*) &record, sizeof(Alumno));
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
        file.read((char*) &record, sizeof(Alumno));
        file.close();
        return record;
    }
};

void printAlumno(Alumno alumno) {
    for(int j = 0; j < 5; j++) {
            cout << alumno.codigo[j];
        }
        cout << " ";
            for(int j = 0; j < 11; j++) {
            cout << alumno.nombre[j];
        }
        cout << " ";
        for(int j = 0; j < 20; j++) {
            cout << alumno.apellidos[j];
        }
        cout << " ";
        for(int j = 0; j < 15; j++) {
            cout << alumno.carrera[j];
    }
    cout << endl;
}

void printAlumnos(vector<Alumno> alumnos) {
    for(int i = 0; i < alumnos.size(); i++) {
        printAlumno(alumnos[i]);
    }
}

int main() {
    FixedRecord fr("datos1.txt");
    cout << endl;
    Alumno a;
    strcpy(a.codigo, "0008");
    strcpy(a.nombre, "Ignacio");
    strcpy(a.apellidos, "Rubio Montiel");
    strcpy(a.carrera, "Computacion");
    //Load y print de los records
    printAlumnos(fr.load());
    //Agregar a ignacio
    fr.add(a);
    //otro load y print
    printAlumnos(fr.load());
    cout << endl;
    //read records
    printAlumno(fr.readRecord(0));
    printAlumno(fr.readRecord(1));
    printAlumno(fr.readRecord(5));
    printAlumno(fr.readRecord(7));
}