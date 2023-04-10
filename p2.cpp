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

Alumno crearAlumno(vector<string> data, int Ciclo, float Mens){
    Alumno a;
    strcpy(a.codigo, completeblankspaces(data[0], 5));
    strcpy(a.nombre, completeblankspaces(data[1], 11));
    strcpy(a.apellidos, completeblankspaces(data[2], 20));
    strcpy(a.carrera, completeblankspaces(data[3], 15));
    a.ciclo = Ciclo;
    a.mensualidad = Mens;
    return a;
}

int main() {
    FixedRecord fr("datos2.bin");
    cout << endl;
    vector<string> reg = {"0008", "Nincol", "Quiroz Maquin", "computacion"};
    Alumno nuevo1 = crearAlumno(reg, 8, 4000.05);
    //se agrega 2 veces un 1er alumno
    fr.add(nuevo1);
    fr.add(nuevo1);
    //se cargan e imprimen todos los alumnos
    printAlumnos(fr.load());
    //se borra el primer record
    fr.deleteRecord(0);
    cout << endl;
    printAlumnos(fr.load());
    cout << endl;
    vector<string> reg2 = {"0009", "Franco", "Pacheco Espino", "computacion"};
    Alumno nuevo2 = crearAlumno(reg2, 1, 8000.2);
    cout << endl;
    // se agrega un nuevo alumno (en donde se borro el record pos = 0)
    fr.add(nuevo2);
    // se buscan a los 2 records individualmente y se imprimer
    printAlumno(fr.readRecord(0));
    cout << endl;
    printAlumno(fr.readRecord(1));
    cout << endl;
    // se imprimen todos los records
    printAlumnos(fr.load());

}
