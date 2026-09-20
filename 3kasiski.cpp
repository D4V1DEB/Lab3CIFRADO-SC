#include <iostream>
#include <string>

using namespace std;

string alfabeto = "ABCDEFGHIJKLMN#OPQRSTUVWXYZ";

string criptograma =
    "MAXYHGAVAPUUGZHEGZQOWOBNIPQKRNÑMEXIGONIICUCAWIGCTEAGMNOLRSZJNLWÑAWWIGLDDZSNIZDNBIXGZLAYMXÑCVEKIETMOEOPBEWPTNIXCXUIHMECXLNOCECYXEQPBWUFANIICÑJIKISCZUAILBGSOANKBFWUAYWNSCHLCWYDZHDZAQVMPTVGFGPVAJWFVPUOYMXCWERVLQCZWECIFVITUZSNCZUAIKBFMÑALIEGLBSZLQUXÑOHWOCGHNYWÑQKDANZUDIFOIMXNPHNUWQOKLMVBNNKRMKONDPDPNMIKAWOXMEEIVEKGBGSFHVADWPGOYMHOIUEEIPGOLENZBSCHAGKQTZDRÑMÑNWTUZIÑCMÑAXKQUWDLVANNIHLÑCQNWGEHIPGZDTZTÑNWÑEEWFUMGIÑXNTWXNVIXCZOAZSOQUVENDNFWUSZYHGLRACPGGUGIYWHOTRMZUGQQDDZIZFWHVVSHCUGOGIFKBXAXPBOBRDVDUCMVTKGIKDRSZLUQSDVPMXVIVEYMFGTEANIMQLHLGPQOHRYWCFEWFOISNÑPUAYINNÑXNÑPGKWGOILQGAFOILQTAHEIIDWMÑEÑXNEPRCVDQTURSK";

int buscarPosicion(char c) {
    for (int i = 0; i < 27; i++) {
        if (alfabeto[i] == c) return i;
    }
    return -1;
}

// reemplazar Ñ por # 
string normalizar(string texto) {
    size_t pos = 0;
    while ((pos = texto.find("Ñ", pos)) != string::npos) {
        texto.replace(pos, 2, "#");
        pos++;
    }
    return texto;
}

string mostrar(string texto) {
    string resultado = "";
    for (char c : texto) {
        if (c == '#') resultado += "Ñ";
        else resultado += c;
    }
    return resultado;
}

int hallarLongitud(string texto) {
    int votos[21] = {};
    for (int i = 0; i + 2 < texto.size(); i++) {
        for (int j = i + 3; j + 2 < texto.size(); j++) {
            if (texto.substr(i, 3) == texto.substr(j, 3)) {
                int distancia = j - i;
                for (int k = 2; k <= 20; k++) {
                    if (distancia % k == 0)
                        votos[k]++;
                }
            }
        }
    }

    int longitud = 2;

    for (int k = 3; k <= 20; k++) {
        if (votos[k] > votos[longitud])
            longitud = k;
    }

    cout << "Longitud probable: " << longitud << endl;
    int contador = 0;

    for (int i = 0; i + 2 < texto.size() && contador < 5; i++) {
        for (int j = i + 3; j + 2 < texto.size() && contador < 5; j++) {
            int distancia = j - i;

            if (texto.substr(i, 3) == texto.substr(j, 3) && distancia % longitud == 0 && distancia <= 120) {
                cout << mostrar(texto.substr(i, 3))<< ": distancia " << distancia << endl;
                contador++;
            }
        }
    }

    return longitud;
}

string hallarClave(string texto, int longitud) {

    double frecuencia[27] = {
        12.53, 1.42, 4.68, 5.86, 13.68, 0.69, 1.01,
        0.70, 6.25, 0.44, 0.02, 4.97, 3.15, 6.71,
        0.31, 8.68, 2.51, 0.88, 6.87, 7.98, 4.63,
        3.93, 0.90, 0.01, 0.22, 0.90, 0.52
    };

    double suma = 0;

    for (double f : frecuencia)
        suma += f;

    string clave = "";

    for (int col = 0; col < longitud; col++) {
        int conteo[27] = {};
        int total = 0;
        for (int i = col; i < texto.size(); i += longitud) {
            conteo[buscarPosicion(texto[i])]++;
            total++;
        }

        double mejorError = 1e100;
        int mejorDesplazamiento = 0;

        for (int k = 0; k < 27; k++) {
            double error = 0;
            for (int letra = 0; letra < 27; letra++) {
                double esperado = total * frecuencia[letra] / suma;
                double diferencia = conteo[(letra + k) % 27] - esperado;
                error += diferencia * diferencia / esperado;
            }
            if (error < mejorError) {
                mejorError = error;
                mejorDesplazamiento = k;
            }
        }

        clave += alfabeto[mejorDesplazamiento];
    }
    return clave;
}

string descifrar(string texto, string clave) {
    string resultado = "";
    for (int i = 0; i < texto.size(); i++) {
        int c = buscarPosicion(texto[i]);
        int k = buscarPosicion(clave[i % clave.size()]);
        int m = (c - k + 27) % 27;
        resultado += alfabeto[m];
    }

    return mostrar(resultado);
}

int main() {

    string texto = normalizar(criptograma);
    int longitud = hallarLongitud(texto);
    string clave = hallarClave(texto, longitud);
    cout << "Clave: " << mostrar(clave) << endl;
    cout << "\nTexto claro:\n";
    cout << descifrar(texto, clave) << endl;

    return 0;
}