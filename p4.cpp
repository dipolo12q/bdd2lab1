#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Definición de la estructura Matricula
struct Matricula
{
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;
};

class VariableRecord
{
private:
    string filename;
    string metadata_name;

public:
    VariableRecord(string fname, string mname) : filename(fname), metadata_name(mname)
    {
        ofstream fout(metadata_name, ios::out | ios::binary);
        fout.close();
    }

    void add(Matricula record)
    {
        ofstream fout(filename, ios::out | ios::app | ios::binary);

        // Escribir el codigo
        int len = record.codigo.length();
        fout.write((char*)&len, sizeof(int));
        fout.write(record.codigo.c_str(), len);

        // Escribir el ciclo
        fout.write((char*)&record.ciclo, sizeof(int));

        // Escribir la mensualidad
        fout.write((char*)&record.mensualidad, sizeof(float));

        // Escribir las observaciones
        len = record.observaciones.length();
        fout.write((char*)&len, sizeof(int));
        fout.write(record.observaciones.c_str(), len);

        // Almacenar la información del registro en el archivo metadata
        int pos = static_cast<int>(fout.tellp()) - (static_cast<int>(sizeof(int)) + static_cast<int>(record.codigo.length()) + static_cast<int>(sizeof(int)) + static_cast<int>(sizeof(float)) + static_cast<int>(sizeof(int)) + static_cast<int>(record.observaciones.length()));
        fout.close();

        fout.open(metadata_name, ios::out | ios::app | ios::binary);
        fout.write((char*)&pos, sizeof(int));
        fout.write((char*)&len, sizeof(int));
        fout.close();
    }

    Matricula readRecord(int pos)
    {
        ifstream fin(filename, ios::in | ios::binary);
        fin.seekg(pos);

        // Leer el codigo
        int len = 0;
        fin.read((char*)&len, sizeof(int));
        char* buffer = new char[len];
        fin.read(buffer, len);
        string codigo(buffer, len);
        delete[] buffer;

        // Leer el ciclo
        int ciclo = 0;
        fin.read((char*)&ciclo, sizeof(int));

        // Leer la mensualidad
        float mensualidad = 0.0f;
        fin.read((char*)&mensualidad, sizeof(float));

        // Leer las observaciones
        len = 0;
        fin.read((char*)&len, sizeof(int));
        buffer = new char[len];
        fin.read(buffer, len);
        string observaciones(buffer, len);
        delete[] buffer;

        Matricula record = { codigo, ciclo, mensualidad, observaciones };
        fin.close();
        return record;
    }

    vector<Matricula> load()
    {
        vector<Matricula> records;

        ifstream fin(metadata_name, ios::in | ios::binary);
        while (!fin.eof())
        {
            int pos = 0;
            fin.read((char*)&pos, sizeof(int));

            int len = 0;
            fin.read((char*)&len, sizeof(int));

            if (pos != 0 && len != 0)
            {
                records.push_back(readRecord(pos));
            }
        }
        fin.close();

        return records;
    }
};

int main()
{
    VariableRecord vr("matriculas.bin", "matriculas.metadata");

    // Agregar registros
    Matricula m1 = { "001", 2022, 1500.25f, "Observaciones 1" };
    Matricula m2 = { "002", 2023, 1750.50f, "Observaciones 2" };
    vr.add(m1);
    vr.add(m2);

    // Leer un registro
    Matricula m = vr.readRecord(0);
    cout << "Codigo: " << m.codigo << endl;
    cout << "Ciclo: " << m.ciclo << endl;
    cout << "Mensualidad: " << m.mensualidad << endl;
    cout << "Observaciones: " << m.observaciones << endl;

    // Cargar todos los registros
    vector<Matricula> records = vr.load();
    for (int i = 0; i < records.size(); i++)
    {
        cout << "Registro " << i + 1 << endl;
        cout << "Codigo: " << records[i].codigo << endl;
        cout << "Ciclo: " << records[i].ciclo << endl;
        cout << "Mensualidad: " << records[i].mensualidad << endl;
        cout << "Observaciones: " << records[i].observaciones << endl;
    }

    return 0;
}