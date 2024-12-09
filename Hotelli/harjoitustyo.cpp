// Harjoitustyö Anton Markki 24TIETOB, 4-5p
// Kirjastot
#include<iostream>
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include<locale>
#include<fstream>
#include<sstream>

using namespace std;

// Structi varaustietojen käsittelyä varten
struct Varaus
{
    int varausNumero;
    string varaajanNimi;
    int huoneNumero;
    int oidenMaara;
    double loppusumma;
};

// Aliohjelma huoneiden varaustilanteen tarkistamiseen
bool tarkistaHuone(const vector<bool>& huoneet, int huonenumero)
{
    return !huoneet[huonenumero - 1];
}

// Aliohjelma varauksen tallentamiseen tiedostoon
void tallennaVarausTiedostoon(const Varaus& varaus)
{
    ofstream tiedosto("varaukset.txt", ios::app);
    if (tiedosto.is_open())
    {
        tiedosto << varaus.varausNumero << ";"
            << varaus.varaajanNimi << ";"
            << varaus.huoneNumero << ";"
            << varaus.oidenMaara << ";"
            << varaus.loppusumma << "\n";
        tiedosto.close();
    }
    else
    {
        cout << "Virhe: Tiedostoa ei voitu avata tallentamista varten.\n";
    }
}

// Aliohjelma varauksen tekemiseen
void teeVaraus(vector<bool>& yhdenHengenHuoneet, vector<bool>& kahdenHengenHuoneet, vector<Varaus>& varaukset)
{
    string nimi;
    int huonetyyppi, yot;
    double hinta;
    double alennusPros = (rand() % 3) * 10; // 0%, 10% tai 20%

    cin.ignore();
    cout << "Anna varaajan nimi: ";
    getline(cin, nimi);

    cout << "Valitse huonetyyppi (1 = yhden hengen, 2 = kahden hengen): ";
    cin >> huonetyyppi;

    if (huonetyyppi == 1)
        hinta = 100;
    else
        hinta = 150;

    cout << "Anna öiden määrä: ";
    cin >> yot;

    int huone;
    bool vapaana = false;
    while (!vapaana)
    {
        huone = rand() % (huonetyyppi == 1 ? yhdenHengenHuoneet.size() : kahdenHengenHuoneet.size()) + 1;
        vapaana = tarkistaHuone(huonetyyppi == 1 ? yhdenHengenHuoneet : kahdenHengenHuoneet, huone);
    }

    if (huonetyyppi == 1)
        yhdenHengenHuoneet[huone - 1] = true;
    else
        kahdenHengenHuoneet[huone - 1] = true;

    int varausNumero = rand() % 90000 + 10000;
    double loppusumma = hinta * yot * (1 - alennusPros / 100);

    Varaus uusiVaraus = { varausNumero, nimi, huone, yot, loppusumma };
    varaukset.push_back(uusiVaraus);
    tallennaVarausTiedostoon(uusiVaraus);

    cout << "Varaus tehty onnistuneesti!\n";
    cout << "Varausnumero: " << varausNumero << "\n";
    cout << "Huone: " << huone << "\n";
    cout << "Loppusumma alennuksen (" << alennusPros << "%) jälkeen: " << loppusumma << " euroa\n";
}

// Aliohjelma varauksen hakemiseen tiedostosta
void haeVaraus()
{
    ifstream tiedosto("varaukset.txt");
    if (!tiedosto.is_open())
    {
        cout << "Virhe: Tiedostoa ei voitu avata.\n";
        return;
    }

    int valinta;
    cout << "Hae varausta:\n";
    cout << "1. Nimen perusteella\n";
    cout << "2. Varausnumeron perusteella\n";
    cout << "Valinta: ";
    cin >> valinta;
    cin.ignore();

    string haku;
    bool loydetty = false;

    if (valinta == 1)
    {
        cout << "Anna varaajan nimi: ";
        getline(cin, haku);
    }
    else if (valinta == 2)
    {
        cout << "Anna varausnumero: ";
        getline(cin, haku);
    }
    else
    {
        cout << "Virheellinen valinta.\n";
        return;
    }

    string rivi;
    while (getline(tiedosto, rivi))
    {
        istringstream ss(rivi);
        vector<string> osat;
        string osa;

        // Pilkotaan rivi puolipisteen perusteella
        while (getline(ss, osa, ';'))
        {
            osat.push_back(osa);
        }

        // Tarkistetaan, että rivejä on tarpeeksi
        if (osat.size() < 5)
        {
            continue;
        }

        if ((valinta == 1 && osat[1] == haku) || (valinta == 2 && osat[0] == haku))
        {
            cout << "\n--- Varauksen tiedot ---\n";
            cout << "Varausnumero: " << osat[0] << "\n";
            cout << "Varaajan nimi: " << osat[1] << "\n";
            cout << "Huonenumero: " << osat[2] << "\n";
            cout << "Öiden määrä: " << osat[3] << "\n";
            cout << "Loppusumma: " << osat[4] << " euroa\n";
            loydetty = true;
        }
    }

    if (!loydetty)
    {
        cout << "Varausta ei löytynyt.\n";
    }

    tiedosto.close();
}

// Pääohjelma
int main()
{
    setlocale(LC_ALL, "fi-FI");  // Ääkköstuki

    srand(time(0)); // Satunnaislukugeneraattorin siemen

    // Arvotaan huonemäärät
    int huoneita = (rand() % 131) * 2 + 40;
    int yhdenHengenMaara = huoneita / 2;
    int kahdenHengenMaara = huoneita / 2;
    // Vektorit varaustilanteen seurantaan
    vector<bool> yhdenHengenHuoneet(yhdenHengenMaara, false);
    vector<bool> kahdenHengenHuoneet(kahdenHengenMaara, false);
    vector<Varaus> varaukset;

    int valinta;
    do
    {   // Valikko
        cout << "\n--- Hotellihuoneen varausohjelma ---\n";
        cout << "1. Tee varaus\n";
        cout << "2. Hae varausta\n";
        cout << "3. Lopeta\n";
        cout << "Valinta: ";
        cin >> valinta;

        switch (valinta)
        {
        case 1:
            system("cls"); // Tyhjentää valikon sen siistimiseksi
            teeVaraus(yhdenHengenHuoneet, kahdenHengenHuoneet, varaukset);
            break;
        case 2:
            system("cls");
            haeVaraus();
            break;
        case 3:
            system("cls");
            cout << "Ohjelma lopetettu.\n";
            break;
        default:
            cout << "Virheellinen valinta!\n";
        }
    } while (valinta != 3);

    return 0;
}