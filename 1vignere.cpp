#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

string alfabeto = "ABCDEFGHIJKLMN#OPQRSTUVWXYZ";
string alfabeto191[191];

//2
string mensaje =
    "Creer que es posible es el paso número uno hacia el éxito. "
    "Despertarse y pensar en algo positivo puede cambiar el transcurso "
    "de todo el día. No eres lo suficientemente viejo como para no "
    "iniciar un nuevo camino hacia tus sueños. Levántate cada mañana "
    "creyendo que vas a vivir el mejor día de tu vida";
//6
string criptograma =
    "WPIXHVYYOSRTECSZBEEGHUUFWRWTZGRWUFSRIWESSXVOHAIHOHWWHCWHUZOBOZEAOYBMCRLTEYOTI";

//mod27
int buscarPosicion(char letra) {
    for (int i = 0; i < alfabeto.size(); i++) {
        if (alfabeto[i] == letra)
            return i;
    }
    return -1;
}

void reemplazar(string &texto, string a, string b) {
    size_t pos = 0;

    while ((pos = texto.find(a, pos)) != string::npos) {
        texto.replace(pos, a.size(), b);
        pos += b.size();
    }
}

string normalizar27(string texto) {
    //evitar error de Ñ (N y ~) reemplazar # por Ñ o ñ
    reemplazar(texto, "ñ", "#");
    reemplazar(texto, "Ñ", "#");

    string tildes[] = {"á", "é", "í", "ó", "ú", "Á", "É", "Í", "Ó", "Ú"};
    string letras = "AEIOUAEIOU";

    for (int i = 0; i < 10; i++) { 
        reemplazar(texto, tildes[i], string(1, letras[i]));
    }
    string resultado = "";

    for (char c : texto) {
        if (c >= 'a' && c <= 'z') c -= 32;
        if (buscarPosicion(c) != -1) resultado += c;
    }

    return resultado;
}

//mostrar Ñ 
string mostrar27(string texto) {
    reemplazar(texto, "#", "Ñ");
    return texto;
}

// mod 191

void preparar191() {
    int j = 0;

    for (int c = 33; c <= 126; c++) {
        alfabeto191[j++] = string(1, char(c));
    }
    alfabeto191[j++] = "€";

    for (int c = 160; c <= 255; c++) {
        string letra;

        if (c < 192) {
            letra += char(0xC2);
            letra += char(c);
        } else {
            letra += char(0xC3);
            letra += char(c - 64);
        }
        alfabeto191[j++] = letra;
    }
}

int buscar191(string letra) {
    for (int i = 0; i < 191; i++) {
        if (alfabeto191[i] == letra) return i;
    }
    return -1;
}

string siguiente(string texto, size_t &i) {
    unsigned char c = texto[i];
    int n = 1;

    if (c >= 240) n = 4;
    else if (c >= 224) n = 3;
    else if (c >= 192) n = 2;

    string letra = texto.substr(i, n);
    i += n;

    return letra;
}

string normalizar191(string texto) {
    string resultado = "";

    for (size_t i = 0; i < texto.size();) {
        string letra = siguiente(texto, i);

        if (buscar191(letra) != -1)
            resultado += letra;
    }

    return resultado;
}

// cigrado y descifrdo
string vigenere(string texto, string clave, int modulo, int metodo) {
    if (modulo == 27) {
        texto = normalizar27(texto);
        clave = normalizar27(clave);
    } else {
        texto = normalizar191(texto);
        clave = normalizar191(clave);
    }
    if (clave.empty()) return "";

    string resultado = "";
    size_t i = 0, j = 0;

    while (i < texto.size()) {
        string letra, letraClave;

        if (modulo == 27) {
            letra = texto.substr(i++, 1);
        } else {
            letra = siguiente(texto, i);
        }
        
        if (j >= clave.size()) j = 0;

        if (modulo == 27) {
            letraClave = clave.substr(j++, 1);
        } else {
            letraClave = siguiente(clave, j);
        }
        int m, k;

        if (modulo == 27) {
            m = buscarPosicion(letra[0]);
            k = buscarPosicion(letraClave[0]);
        } else {
            m = buscar191(letra);
            k = buscar191(letraClave);
        }

        int c;

        if (metodo == 1) {   // Vigenere
            c = (m + k) % modulo;
        } else if (metodo == 2) {  // Descifrado Vigenere
            c = (m - k + modulo) % modulo;  
        } else if (metodo == 3) {  // Beaufort
            c = (k - m + modulo) % modulo;
        } else {  // Variante de Beaufort
            c = (m - k + modulo) % modulo;
        }
        if (modulo == 27) {
            resultado += alfabeto[c];
        } else {
            resultado += alfabeto191[c];
        }
    }

    if (modulo == 27)
        resultado = mostrar27(resultado);

    return resultado;
}

// 5 frecuencias
void frecuencias(string texto) {
    string claves[] = {"POSITIVO", "HIELO", "MAR"};
    int f[4][27] = {};
    string original = normalizar27(texto);

    for (char c : original) {
        f[0][buscarPosicion(c)]++;
    }

    for (int j = 0; j < 3; j++) {
        string cifrado = normalizar27( vigenere(texto, claves[j], 27, 1));
        for (char c : cifrado) {
            f[j + 1][buscarPosicion(c)]++;
        }
    }

    cout << left << setw(8) << "Letra" << setw(12) << "Original" << setw(12) << "POSITIVO" << setw(12) << "HIELO" << setw(12) << "MAR" << endl;

    for (int i = 0; i < 27; i++) {
        string letra(1, alfabeto[i]);
        if (letra == "#") letra = "Ñ";

        cout << left << setw(8) << letra;

        for (int j = 0; j < 4; j++) {
            cout << setw(12) << f[j][i];
        }
        cout << endl;
    }
}

int main() {
    preparar191();

    // Ejercicio 1
    string texto = "HERMOSO";
    string clave = "CIELO";

    cout << "Ejercicio 1 " << endl;
    cout << "Modulo 27:\n";
    cout << vigenere(texto, clave, 27, 1) << endl;
    cout << "\nModulo 191:\n";
    cout << vigenere(texto, clave, 191, 1) << endl;

    // Ejercicio 2
    cout << "\nEjercicio 2" << endl;
    cout << "Modulo 27:\n";
    cout << vigenere(mensaje, "POSITIVO", 27, 1) << endl;
    cout << "\nModulo 191:\n";
    cout << vigenere(mensaje, "POSITIVO", 191, 1) << endl;

    // Ejercicio 4
    cout << "\nEjercicio 4 " << endl;
    cout << "Beaufort:\n";
    cout << vigenere(mensaje, "POSITIVO", 27, 3) << endl;
    cout << "\nVariante de Beaufort:\n";
    cout << vigenere(mensaje, "POSITIVO", 27, 4) << endl;

    // Ejercicio 5
    cout << "\nEjercicio 5" << endl;
    cout << "Frecuencias:\n";
    frecuencias(mensaje);

    // Ejercicio 6
    cout << "\nEjercicio 6" << endl;
    cout << "Descifrado:\n";
    cout << vigenere(criptograma, "HIELO", 27, 2) << endl;

    return 0;
}