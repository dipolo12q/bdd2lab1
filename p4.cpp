#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <climits>
#include <iomanip>
#include <limits>

using namespace std;

struct Matricula
{
    string codigo;
    int ciclo;
    float mensualidad;
    string observaciones;
};

class VariableRecord {
private:
    string metadata_filename;
    string data_filename;
    vector<int> records_start_pos;
    vector<int> records_size;
    
public:
    VariableRecord(string metadata_filename, string data_filename) {
        this->metadata_filename = metadata_filename;
        this->data_filename = data_filename;
        
        // Leer metadata
        ifstream metadata(metadata_filename);
        if (metadata.good()) {
            int pos, size;
            while (metadata >> pos >> size) {
                records_start_pos.push_back(pos);
                records_size.push_back(size);
            }
            metadata.close();
        } else {
            // Si no existe, crearlo
            ofstream new_metadata(metadata_filename);
            new_metadata.close();
        }
    }
    
    vector<Matricula> load() {
        vector<Matricula> records;
        
        // Leer registros
        ifstream data(data_filename);
        if (data.good()) {
            for (int i = 0; i < records_start_pos.size(); i++) {
                data.seekg(records_start_pos[i]);
                
                // Leer tamaño de los campos
                int codigo_size, ciclo_size, mensualidad_size, observaciones_size;
                data.read(reinterpret_cast<char*>(&codigo_size), sizeof(int));
                data.read(reinterpret_cast<char*>(&ciclo_size), sizeof(int));
                data.read(reinterpret_cast<char*>(&mensualidad_size), sizeof(int));
                data.read(reinterpret_cast<char*>(&observaciones_size), sizeof(int));
                
                // Leer campos
                char codigo[codigo_size];
                data.read(codigo, codigo_size);
                int ciclo;
                data.read(reinterpret_cast<char*>(&ciclo), ciclo_size);
                float mensualidad;
                data.read(reinterpret_cast<char*>(&mensualidad), mensualidad_size);
                char observaciones[observaciones_size];
                data.read(observaciones, observaciones_size);
                
                // Agregar registro
                Matricula record = {string(codigo, codigo_size), ciclo, mensualidad, string(observaciones, observaciones_size)};
                records.push_back(record);
            }
            data.close();
        }
        
        return records;
    }
    
    void add(Matricula record) {
        // Escribir nuevo registro al final del archivo
        ofstream data(data_filename, ios::app | ios::binary);
        if (data.good()) {
            // Escribir tamaño de los campos
            int codigo_size = record.codigo.size();
            int ciclo_size = sizeof(record.ciclo);
            int mensualidad_size = sizeof(record.mensualidad);
            int observaciones_size = record.observaciones.size();
            data.write(reinterpret_cast<const char*>(&codigo_size), sizeof(int));
            data.write(reinterpret_cast<const char*>(&ciclo_size), sizeof(int));
            data.write(reinterpret_cast<const char*>(&mensualidad_size), sizeof(int));
            data.write(reinterpret_cast<const char*>(&observaciones_size), sizeof(int));
            
            // Escribir campos
            data.write(record.codigo.c_str(), codigo_size);
            data.write(reinterpret_cast<const char*>(&record.ciclo), ciclo_size);
            data.write(reinterpret_cast<const char*>(&record.mensualidad), mensualidad_size);
            data.write(record.observaciones.c_str(), observaciones_size);
            
            // Guardar nueva posición y tamaño en metadata
            int pos = data.tellp() - (sizeof(int) * 4 + codigo_size + observaciones_size);
            int size = sizeof(int) * 4 + codigo_size + observaciones_size + mensualidad_size;
            records_start_pos.push_back(pos);
            records_size.push_back(size);
            
            ofstream metadata(metadata_filename, ios::app);
            metadata << pos << " " << size << endl;
            metadata.close();
            
            data.close();
        }
    }
    
    Matricula readRecord(int pos) {
        Matricula record = {};
        
        // Leer registro en la posición especificada
        if (pos >= 0 && pos < records_start_pos.size()) {
            ifstream data(data_filename);
            if (data.good()) {
                data.seekg(records_start_pos[pos]);
                
                // Leer tamaño de los campos
                int codigo_size, ciclo_size, mensualidad_size, observaciones_size;
                data.read(reinterpret_cast<char*>(&codigo_size), sizeof(int));
                data.read(reinterpret_cast<char*>(&ciclo_size), sizeof(int));
                data.read(reinterpret_cast<char*>(&mensualidad_size), sizeof(int));
                data.read(reinterpret_cast<char*>(&observaciones_size), sizeof(int));
                
                // Leer campos
                char codigo[codigo_size];
                data.read(codigo, codigo_size);
                int ciclo;
                data.read(reinterpret_cast<char*>(&ciclo), ciclo_size);
                float mensualidad;
                data.read(reinterpret_cast<char*>(&mensualidad), mensualidad_size);
                char observaciones[observaciones_size];
                data.read(observaciones, observaciones_size);
                
                // Asignar valores al registro
                record.codigo = string(codigo, codigo_size);
                record.ciclo = ciclo;
                record.mensualidad = mensualidad;
                record.observaciones = string(observaciones, observaciones_size);
                
                data.close();
            }
        }
        
        return record;
    }
};

int main() {
    VariableRecord variableRecord("metadata.txt", "data.bin");
    
    // Agregar registros
    Matricula record1 = {"001", 2, 1500.50, "Nuevo estudiante"};
    variableRecord.add(record1);
    Matricula record2 = {"002", 3, 2000.75, "Becado"};
    variableRecord.add(record2);
    Matricula record3 = {"003", 1, 1800.25, "Primer ciclo"};
    variableRecord.add(record3);
    Matricula record4 = {"004", 4, 1850.00, "Estudiante de intercambio"};
    variableRecord.add(record4);
    
    // Leer todos los registros
    vector<Matricula> records = variableRecord.load();
    for (int i = 0; i < records.size(); i++) {
        cout << "Registro " << i + 1 << endl;
        cout << "Código: " << records[i].codigo << endl;
        cout << "Ciclo: " << records[i].ciclo << endl;
        cout << "Mensualidad: " << records[i].mensualidad << endl;
        cout << "Observaciones: " << records[i].observaciones << endl;
        cout << endl;
    }
    
    // Leer un registro específico
    Matricula record5 = variableRecord.readRecord(1);
    cout << "Registro 2" << endl;
    cout << "Código: " << record5.codigo << endl;
    cout << "Ciclo: " << record5.ciclo << endl;
    cout << "Mensualidad: " << record5.mensualidad << endl;
    cout << "Observaciones: " << record5.observaciones << endl;
}