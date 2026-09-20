
#include <iostream>
#include <string>

using namespace std;

string alfabeto = "ABCDEFGHIJKLMN#OPQRSTUVWXYZ";
string clave = "UNODELOSMASGRANDESCRIPTOGRAFOS";

string criptograma =
    "XHGDQESDMPK~NDEEDKNGJZPFJSUIFZOLFCINFJCESVZTGBFXCIUDAYNUUDIZYWWZBEYNVQWIVUN"
    "KZEPHDODQUZZLBDNDRWTHQSER~NIVMLERCMGIFLSORZXTSDIGLOXQSDJHWVCIWQXQJCKMBPOK"
    "MPSKMUVIMNJDNBLCSZHXHNYYUIXDBSOXHZLXWVGDJGXHWLTDWK~NSAQIMZLNBVMLXHUOQQXI"
    "QGWGUFTWKZKMOKUDNINSIFJDUOZIJBSVVOWFAIE~NGYOWPSOAP";

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

string descifrarAutoclave(string texto, string clave) {
    // reemplazar Ñ por #
    reemplazar(texto, "~N", "#");
    reemplazar(texto, "Ñ", "#");

    string resultado = "";

    for (int i = 0; i < texto.size(); i++) {

        int c = buscarPosicion(texto[i]);
        int k = buscarPosicion(clave[i]);

        int m = (c - k + 27) % 27;
        char letra = alfabeto[m];
        resultado += letra;
        clave += letra;
    }

    reemplazar(resultado, "#", "Ñ");
    return resultado;
}

int main() {
    cout << "Ejercicio 4.2 - Autoclave" << endl;
    cout << "Texto descifrado:" << endl;
    cout << descifrarAutoclave(criptograma, clave) << endl;

    return 0;
}