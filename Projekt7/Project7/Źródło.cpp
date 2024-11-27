#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cctype> // Dodane do obs³ugi std::tolower

// Funkcja do rysowania wisielca w zale¿noœci od liczby prób
void rysujWisielca(int proby) {
    switch (proby) {
    case 6:
        std::cout << " +---+\n     |\n     |\n     |\n    ===\n";
        break;
    case 5:
        std::cout << " +---+\n O   |\n     |\n     |\n    ===\n";
        break;
    case 4:
        std::cout << " +---+\n O   |\n |   |\n     |\n    ===\n";
        break;
    case 3:
        std::cout << " +---+\n O   |\n/|   |\n     |\n    ===\n";
        break;
    case 2:
        std::cout << " +---+\n O   |\n/|\\  |\n     |\n    ===\n";
        break;
    case 1:
        std::cout << " +---+\n O   |\n/|\\  |\n/    |\n    ===\n";
        break;
    case 0:
        std::cout << " +---+\n O   |\n/|\\  |\n/ \\  |\n    ===\n";
        break;
    default:
        break;
    }
}

// Funkcja do wyœwietlania s³owa z odgadniêtymi literami
void wyswietlSlowo(const std::string& slowo, const std::vector<bool>& odgadniete) {
    for (size_t i = 0; i < slowo.length(); i++) {
        if (odgadniete[i]) {
            std::cout << slowo[i] << " ";
        }
        else {
            std::cout << "_ ";
        }
    }
    std::cout << std::endl;
}

// Funkcja do sprawdzania, czy wszystkie litery s³owa zosta³y odgadniête
bool czySlowoOdgadniete(const std::vector<bool>& odgadniete) {
    for (bool o : odgadniete) {
        if (!o) {
            return false;
        }
    }
    return true;
}

// Funkcja do wczytywania hase³ i kategorii z pliku CSV
std::vector<std::pair<std::string, std::string>> wczytajSlowaZPliku(const std::string& nazwaPliku) {
    std::vector<std::pair<std::string, std::string>> slowaIKategorie;
    std::ifstream plik(nazwaPliku);

    if (!plik.is_open()) {
        std::cerr << "Blad otwarcia pliku: " << nazwaPliku << std::endl;
        return slowaIKategorie;
    }

    std::string linia;
    while (std::getline(plik, linia)) {
        std::stringstream ss(linia);
        std::string haslo, kategoria;

        // Wczytanie has³a i kategorii rozdzielonych przecinkiem
        if (std::getline(ss, haslo, ',') && std::getline(ss, kategoria)) {
            slowaIKategorie.emplace_back(haslo, kategoria);
        }
    }

    plik.close();
    return slowaIKategorie;
}

// Funkcja do obs³ugi jednej rozgrywki
bool graWisielec(std::vector<std::pair<std::string, std::string>>& slowaIKategorie) {
    if (slowaIKategorie.empty()) {
        std::cout << "Brak danych do gry. Upewnij sie, ze plik CSV zawiera dane." << std::endl;
        return false;
    }

    // Wyœwietlenie dostêpnych kategorii i hase³
    std::cout << "Dostepne hasla:\n";
    for (size_t i = 0; i < slowaIKategorie.size(); i++) {
        std::cout << i + 1 << ". " << slowaIKategorie[i].second << " (haslo ukryte)\n";
    }

    // Gracz wybiera has³o
    int wybor = 0;
    std::cout << "\nWybierz haslo wpisujac numer (lub 0, aby wybrac losowo): ";
    std::cin >> wybor;

    // Losowanie lub wybór rêczny
    std::pair<std::string, std::string> wybrane;
    if (wybor == 0) {
        std::srand(std::time(0));
        wybrane = slowaIKategorie[std::rand() % slowaIKategorie.size()];
    }
    else if (wybor > 0 && wybor <= static_cast<int>(slowaIKategorie.size())) {
        wybrane = slowaIKategorie[wybor - 1];
    }
    else {
        std::cout << "Niepoprawny wybor. Gra losuje haslo.\n";
        std::srand(std::time(0));
        wybrane = slowaIKategorie[std::rand() % slowaIKategorie.size()];
    }

    std::string wybraneSlowo = wybrane.first;
    std::string kategoria = wybrane.second;

    // Zamiana liter w wybranym s³owie na ma³e, dla ujednolicenia
    for (char& c : wybraneSlowo) {
        c = std::tolower(c);
    }

    // Inicjalizacja tablicy odgadniêtych liter (spacje traktowane jako odgadniête)
    std::vector<bool> odgadniete(wybraneSlowo.length(), false);
    for (size_t i = 0; i < wybraneSlowo.length(); i++) {
        if (wybraneSlowo[i] == ' ') {
            odgadniete[i] = true;
        }
    }

    int proby = 6; // Liczba prób

    std::cout << "Witamy w grze w wisielca!" << std::endl;
    std::cout << "Kategoria: " << kategoria << std::endl;
    std::cout << "Zgadnij ukryte slowo. Masz " << proby << " prob." << std::endl;

    while (proby > 0 && !czySlowoOdgadniete(odgadniete)) {
        rysujWisielca(proby); // Wyœwietlanie rysunku wisielca
        std::cout << "\nPozostalo prob: " << proby << std::endl;
        wyswietlSlowo(wybraneSlowo, odgadniete);

        std::cout << "Wprowadz litere: ";
        char litera;
        std::cin >> litera;

        // Zamiana litery wejœciowej na ma³¹
        litera = std::tolower(litera);

        bool poprawnaOdpowiedz = false;
        for (size_t i = 0; i < wybraneSlowo.length(); i++) {
            if (wybraneSlowo[i] == litera && !odgadniete[i]) {
                odgadniete[i] = true;
                poprawnaOdpowiedz = true;
            }
        }

        if (!poprawnaOdpowiedz) {
            proby--;
            std::cout << "Zla odpowiedz!" << std::endl;
        }
        else {
            std::cout << "Dobrze!" << std::endl;
        }
    }

    rysujWisielca(proby); // Finalny rysunek wisielca
    if (czySlowoOdgadniete(odgadniete)) {
        std::cout << "\nGratulacje! Odgadles slowo: " << wybraneSlowo << std::endl;
        // Usuniêcie odgadniêtego s³owa z listy
        auto it = std::find(slowaIKategorie.begin(), slowaIKategorie.end(), wybrane);
        if (it != slowaIKategorie.end()) {
            slowaIKategorie.erase(it);
        }
        return true;
    }
    else {
        std::cout << "\nPrzegrales! Slowo to: " << wybraneSlowo << std::endl;
        return false;
    }
}

int main() {
    // Wczytaj has³a z pliku
    std::string nazwaPliku = "E:\\Program Files\\Epic Games\\Git\\Projekt7\\hasla.csv";
    auto slowaIKategorie = wczytajSlowaZPliku(nazwaPliku);

    char kontynuacja;
    do {
        bool wygrana = graWisielec(slowaIKategorie);
        if (slowaIKategorie.empty()) {
            std::cout << "\nBrak dostêpnych hase³. Gra koñczy siê.\n";
            break;
        }
        std::cout << "\nCzy chcesz zagrac ponownie? (t/n): ";
        std::cin >> kontynuacja;
    } while (kontynuacja == 't' || kontynuacja == 'T');

    std::cout << "Dziekujemy za gre! Do zobaczenia!\n";
    return 0;
}
