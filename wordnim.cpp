#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Woord {
	char letters[26]{};
	string woord;
	Woord(const string& woord): woord(woord) {
		for (char c : woord) ++letters[c-'a'];
	}

	int size() const {
		return woord.size();
	}

	bool operator<(const Woord& o) const {
		return woord < o.woord;
	}
};

vector<Woord> woorden;
Woord beschikbare("");//("aaaabcddeeeeeeeeefgghiiiijkklllmnnnnooopqrrrrsssttttuvwxyz");

void lees_in() {
	ifstream bestand("wordlist.txt");
	string woord;
	while (bestand >> woord) {
		bool goed_woord = woord.size() >= 2;
		if (!goed_woord) continue;
		for (int i = 0; i < (int)woord.size(); ++i) {
			woord[i] = tolower(woord[i]);
			if (!(woord[i] >= 'a' && woord[i] <= 'z')) {
				goed_woord = false;
				break;
			}
		}
		if (goed_woord) woorden.emplace_back(woord);
	}
	bestand.close();
	stable_sort(woorden.begin(), woorden.end());
}

void haal_letters_weg(Woord& woord) {
	for (int i = 0; i < 26; ++i) {
		beschikbare.letters[i] -= woord.letters[i];
	}
}

void haal_letters_terug(Woord& woord) {
	for (int i = 0; i < 26; ++i) {
		beschikbare.letters[i] += woord.letters[i];
	}
}


bool valide_woord(Woord& woord) {
	for (int i = 0; i < 26; ++i) {
		if (woord.letters[i] > beschikbare.letters[i]) return false;
	}
	return true;
}

unordered_map<string, bool> cache;
bool minimax(bool beurt) {
	if (none_of(beschikbare.letters, beschikbare.letters+26, [](auto x) {return x;})) {
		return !beurt;
	}
	string W = string(beschikbare.letters, 26);
	auto it = cache.find(W);
	if (it != cache.end()) {
		return it->second == beurt;
	}

	for (auto& woord : woorden) {
		if (valide_woord(woord)) {
			haal_letters_weg(woord);
			bool r = minimax(!beurt);
			haal_letters_terug(woord);
			if (r == beurt) {
				cache[W] = true;
				return beurt;
			}
		}
	}

	cache[W] = false;
	return !beurt;
}

void mens_zet() {
	string str;
	do {
		cout << "Kies een woord: ";
		cin >> str;
		Woord w(str);
		if (binary_search(woorden.begin(), woorden.end(), w)) {
			if (valide_woord(w)) {
				cout << "Goed woord!\n";
				haal_letters_weg(w);
				break;
			}
			else
				cout << "Kies een woord die bestaat uit de beschikbare letters!!!\n";
		} else
			cout << "Woord bestaat niet :(\n";
	} while (true);
}

int main() {
	lees_in();
	cout << "Hallo, dit is geen virus!\n\n     Word NIM\nWord NIM is een variatie op het normale NIM spel.\nBij Word NIM starten de twee spelers met een set aan verschillende letters.\nOm de beurt maken de spelers een woord uit de letters,\nwaarna de letters die dat woord opmaken worden verwijderd uit het spel.\nZo gaat het door totdat iemand geen woord meer kan vormen,\ndie speler zal dan verliezen.\n\nIn dit programma speel je tegen een slimme robot genaamd Kasja. Veel succes! ( Zij begint ;) )\n";
	cout << "\n\nKies hier de letters voor het spel of een woord (niet te lang!!): ";
	string aansteeklont;
	cin >> aansteeklont;
	beschikbare = Woord(aansteeklont);

	bool beurt = false;
	while (!none_of(woorden.begin(), woorden.end(), [](auto& x) {return valide_woord(x);})) {
		beurt = !beurt;
		cout << "\n\n";
		if (beurt) {
			cout << "Kasja is aan de beurt.\n";
		} else {
			cout << "Nu ben jij aan de beurt.\n";
		}
		cout << "De beschikbare letters: ";
		for (int i = 0; i < 26; ++i) {
			cout << string(beschikbare.letters[i], i + 'a');
		}
		cout << '\n';
		if (!beurt) mens_zet();
		else {
			cout << "Bezig met nadenken...\n";
			Woord* keuze = nullptr;
			bool winnen = false;
			for (auto& woord : woorden) {
				//cout << woord.woord << '\n';
				if (valide_woord(woord)) {
					keuze = &woord;
					haal_letters_weg(woord);
					bool r = minimax(!beurt);
					haal_letters_terug(woord);
					if (r) {
						winnen = true;
						break;
					}
				}
			}
			if (!winnen) cout << "\":( Ik ga verliezen!\" zegt de treurige Kasja.\n";
			if (keuze) {
				cout << "Kasja kiest '" << keuze->woord << "'\n";
				haal_letters_weg(*keuze);
			}
			else {
				cout << "Er is geen keuze voor Kasja.\n Spel gecrasht :(\n";
				cin.ignore();
				cin.ignore();
				return 0;
			}
		}
	}

	cout << "\n\n";
	cout << "De beschikbare letters: ";
	for (int i = 0; i < 26; ++i) {
		cout << string(beschikbare.letters[i], i + 'a');
	}
	cout << "\n\n";
	if (beurt) {
		cout << "Kasja wint!\n";
	} else {
		cout << "Je hebt Kasja overmeesterd! Een echte prestatie hoor!\n";
	}

	cin.ignore();
	cin.ignore();

	return 0;
}
