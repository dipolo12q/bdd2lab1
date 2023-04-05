#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <climits>

using namespace std;

struct Alumno {
  char codigo [5];
  char nombre [11];
  char apellidos [20];
  char carrera [15];
  int ciclo;
  float mensualidad;
  int nextdel;
};

class FixedRecord {
private:
    string filename;
public:
    FixedRecord(string filename) {
        this->filename = filename;
        ifstream file(filename, ios::binary);
        file.seekg(0, ios::beg);
        if(is_empty(file)) {
            ofstream filex(filename, ios::app | ios::binary);
            int posfdel = -1;
            filex.write((char*) &posfdel, sizeof(int));
        }
    }

    bool is_empty(ifstream& pFile ){
        return pFile.peek() == ifstream::traits_type::eof();
    }

    vector<Alumno> load() {
        ifstream file(filename, ios::binary);
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
        file.seekg(sizeof(int), ios::beg);
        while(!file.eof()) {
            a = Alumno();
            file.read((char*) &a, sizeof(Alumno));
            result.push_back(a);
        }
        file.close();
        return result;
    }

    void add(Alumno record) {
        record.nextdel = -100;
        ifstream filef(filename, ios::binary);
        try {
            if(!filef.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        int holepos;
        filef.seekg(0, ios::beg);
        filef.read((char*) &holepos, sizeof(int));
        if(holepos == -1) {
            ofstream file(filename, ios::app | ios::binary);
            file.write((char*) &record, sizeof(Alumno));
            file.close();
            filef.close();
        }
        else {
            int newhead;
            ofstream file(filename, ios::in | ios::binary);
            filef.seekg((holepos + 1) * sizeof(Alumno), ios::beg);
            filef.read((char*) &newhead, sizeof(int));
            file.seekp((holepos) * sizeof(Alumno) + sizeof(int), ios::beg);
            file.write((char*) &record, sizeof(Alumno));
            file.seekp(0, ios::beg);
            file.write((char*) &newhead, sizeof(int));
            file.close();
            filef.close();
        }
        
    }

    Alumno readRecord(int pos) {
        ifstream file(filename, ios::binary);
        try {
            if(!file.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        Alumno record;
        file.seekg(pos * sizeof(Alumno) + sizeof(int), ios::beg);
        file.read((char*) &record, sizeof(Alumno));
        file.close();
        return record;
    }

    bool deleteRecord(int pos) {
        ifstream filer(filename, ios::binary);
        try {
            if(!filer.is_open()) {
                throw 20;
            }
        }
        catch(int x) {
            cout << "Error: " << x <<". No se pudo abrir el archivo" << endl;
        }
        filer.seekg(0, ios::beg);
        int nextdel;
        filer.read((char*) &nextdel, sizeof(int));
        int nextdelpos;
        filer.seekg((pos+1) * sizeof(Alumno), ios::beg);
        filer.read((char*) &nextdelpos, sizeof(int));
        if(nextdelpos != -100) {
            cout << "Record with pos: " << pos << " has already been deleted.";
            return false;
        }
        filer.close();
        ofstream filew(filename, ios::in | ios::binary);
        filew.seekp((pos+1) * sizeof(Alumno), ios::beg);
        filew.write((char*) &nextdel, sizeof(int));
        filew.seekp(0, ios::beg);
        filew.write((char*) &pos, sizeof(int));
        filew.close();
        filer.seekg(0, ios::beg);
        filer.read((char*) &nextdel, sizeof(int));
        return true;
    }

    void printHeader() {
        int result;
        ifstream file(filename, ios::binary);
        file.seekg(0, ios::beg);
        file.read((char*) &result, sizeof(int));
        cout << result << endl;
        file.close();
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
    cout << " " << alumno.ciclo << " " << alumno.mensualidad << " " << alumno.nextdel << endl;
}

void printAlumnos(vector<Alumno> alumnos) {
    for(int i = 0; i < alumnos.size() - 1; i++) {
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

int main() {
    FixedRecord fr("datos2.bin");
    cout << endl;
    Alumno a;
    strcpy(a.codigo, completeblankspaces("0008", 5));
    strcpy(a.nombre, completeblankspaces("Matthias", 11));
    strcpy(a.apellidos, completeblankspaces("Espina Ospino", 20));
    strcpy(a.carrera, completeblankspaces("Arte", 15));
    a.ciclo = 8;
    a.mensualidad = 4000.05f;
    // se agregan 2 ignacios
    fr.add(a);
    fr.add(a);
    //se cargan e imprimen todos los alumnos
    printAlumnos(fr.load());
    //se borra el primer record
    fr.deleteRecord(0);
    cout << endl;
    printAlumnos(fr.load());
    Alumno b;
    strcpy(b.codigo, completeblankspaces("0009", 5));
    strcpy(b.nombre, completeblankspaces("Alejandro", 11));
    strcpy(b.apellidos, completeblankspaces("Perez Pacheco", 20));
    strcpy(b.carrera, completeblankspaces("Arte", 15));
    b.ciclo = 1;
    b.mensualidad = 8000.20f;
    cout << endl;
    // se agrega un alejandro (en donde se borro el record pos = 0)
    fr.add(b);
    // se buscan a los 2 records individualmente y se imprimer
    printAlumno(fr.readRecord(0));
    cout << endl;
    printAlumno(fr.readRecord(1));
    cout << endl;
    // se imprimen todos los records
    printAlumnos(fr.load());

}