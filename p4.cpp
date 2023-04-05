#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Matricula
{
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;
};

class VariableRecord
{
public:
    VariableRecord();

    void add(Matricula record);
    Matricula readRecord(int pos);
    vector<Matricula> load();

private:
    fstream dataFile;
    fstream metadataFile;
    int nextPos;
};

VariableRecord::VariableRecord() {
    dataFile.open("data.bin", ios::binary | ios::in | ios::out | ios::app);
    metadataFile.open("metadata.bin", ios::binary | ios::in | ios::out | ios::app);

    nextPos = 0;
}

void VariableRecord::add(Matricula record) {
    // Escribir los campos en el data.bin
    int codeSize = record.codigo.size();
    int obsSize = record.observaciones.size();
    dataFile.write((char*)&codeSize, sizeof(codeSize));
    dataFile.write(record.codigo.c_str(), codeSize);
    dataFile.write((char*)&record.ciclo, sizeof(record.ciclo));
    dataFile.write((char*)&record.mensualidad, sizeof(record.mensualidad));
    dataFile.write((char*)&obsSize, sizeof(obsSize));
    dataFile.write(record.observaciones.c_str(), obsSize);

    // Escribir la posición en el metadata.bin
    metadataFile.write((char*)&nextPos, sizeof(nextPos));
    nextPos = dataFile.tellp();
}

Matricula VariableRecord::readRecord(int pos) {
    // Ponerl el seekg en la posición
    dataFile.seekg(pos);

    // Leer los campos del data.bin
    int codeSize, obsSize;
    dataFile.read((char*)&codeSize, sizeof(codeSize));
    char* codigo = new char[codeSize + 1];
    dataFile.read(codigo, codeSize);
    codigo[codeSize] = '\0';
    int ciclo;
    dataFile.read((char*)&ciclo, sizeof(ciclo));
    float mensualidad;
    dataFile.read((char*)&mensualidad, sizeof(mensualidad));
    dataFile.read((char*)&obsSize, sizeof(obsSize));
    char* observaciones = new char[obsSize + 1];
    dataFile.read(observaciones, obsSize);
    observaciones[obsSize] = '\0';

    // Crear y retornar el registro
    Matricula record;
    record.codigo = string(codigo);
    record.ciclo = ciclo;
    record.mensualidad = mensualidad;
    record.observaciones = string(observaciones);
    delete[] codigo;
    delete[] observaciones;
    return record;
}

vector<Matricula> VariableRecord::load() {
    // Poner el seekg al inicio del metadata.bin
    metadataFile.seekg(0);

    // Leer cada posición y retornar los registros
    vector<Matricula> records;
    int pos;
    while (metadataFile.read((char*)&pos, sizeof(pos))) {
        records.push_back(readRecord(pos));
    }

    return records;
}

int main() {
    VariableRecord variableRecord;

    // Añadir registros
    Matricula record1 = {"MAT001", 1, 1000.0, "Observaciones 1"};
    Matricula record2 = {"MAT002", 2, 1500.0, "Observaciones 2"};
    Matricula record3 = {"MAT003", 3, 2000.0, "Observaciones 3"};
    Matricula record4 = {"MAT004", 4, 2500.0, "Observaciones 4"};
    variableRecord.add(record1);
    variableRecord.add(record2);
    variableRecord.add(record3);
    variableRecord.add(record4);

    // Leer registros (se usa el readreecord dentro de load)
    vector<Matricula> records = variableRecord.load();
    for (Matricula& record : records) {
        cout << "Codigo: " << record.codigo << endl;
        cout << "Ciclo: " << record.ciclo << endl;
        cout << "Mensualidad: " << record.mensualidad << endl;
        cout << "Observaciones: " << record.observaciones << endl;
        cout << endl;
    }

    return 0;
}