#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <climits>
#include <iomanip>
#include <limits>

using namespace std;

struct Alumno
{
  string nombre;
  string apellidos;
  string carrera;
  float mensualidad;
};

class VariableRecord {
private:
    string filename;
public:
    VariableRecord(string filename) {
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
            string mensuali;
            if(getline(file, a.nombre, '|')&&
            getline(file, a.apellidos, '|')&&
            getline(file, a.carrera, '|')&&
            getline(file, mensuali)) {
                a.mensualidad = stof(mensuali);
                result.push_back(a);
            }
        }
        file.close();
        return result;
    }

    void add(Alumno record){
        ofstream file(filename, ios::app);
        try {
            if(!file.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        file << record.nombre << "|" << record.apellidos << "|" << record.carrera << "|" << record.mensualidad << endl;
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
        string linepos;
        for (int i = 1; i <= pos; i++){
            std::getline(file, linepos);
        }
        string tmp;
        stringstream ss(linepos);
        vector<string> attributes;
        while(getline(ss, tmp, '|')){
            attributes.push_back(tmp);
        }
        record.nombre = attributes[0];
        record.apellidos = attributes[1];
        record.carrera = attributes[2];
        record.mensualidad = stof(attributes[3]);
        file.close();
        return record;
    }
};

void printAlumno(Alumno alumno) {
    cout << alumno.nombre << "|" << alumno.apellidos << "|" << alumno.carrera << "|" <<  alumno.mensualidad << endl;
    cout << endl;
}

void printAlumnos(vector<Alumno> alumnos) {
    for(int i = 0; i < alumnos.size(); i++) {
        printAlumno(alumnos[i]);
    }
}

int main() {
    VariableRecord fr("datos2.txt");
    cout << endl;
    Alumno a;
    a.nombre = "Ignacio";
    a.apellidos = "Perez";
    a.carrera = "Ingenieria en Sistemas";
    a.mensualidad = 1000;
    printAlumnos(fr.load());
    fr.add(a);
    printAlumnos(fr.load());
    cout << endl;
    printAlumno(fr.readRecord(1));
    printAlumno(fr.readRecord(2));
    printAlumno(fr.readRecord(5));
}