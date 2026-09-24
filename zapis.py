import os
import ctypes
import json

sciezka_os = os.path.abspath("pliki.so")
lib = ctypes.CDLL(sciezka_os)


def zbierz(struktura, sciezka):
    """Buduje i zwraca słownik reprezentujący dany folder wraz z podfolderami (rekurencyjnie)."""

    print(
        f"Odczytano {struktura.ilosc_plikow} plików i {struktura.ilosc_folderow} folderów z: {sciezka.decode('utf-8')}"
    )

    # TARCZA: liczby z kosmosu = śmieci z pamięci
    if (
        struktura.ilosc_folderow < 0
        or struktura.ilosc_folderow > 100000
        or struktura.ilosc_plikow < 0
    ):
        print(
            "UWAGA: Funkcja w C zwróciła uszkodzone dane z pamięci! Pomijam ten folder."
        )
        return None

    if struktura.ilosc_plikow > 0 and not struktura.pliki:
        print("UWAGA: Brakuje wskaźnika na pliki! Pomijam.")
        return None

    if struktura.ilosc_folderow > 0 and not struktura.foldery:
        print("UWAGA: Brakuje wskaźnika na foldery! Pomijam.")
        return None

    # --- pliki w tym folderze ---
    pliki_list = []
    for i in range(struktura.ilosc_plikow):
        bajtowa_nazwa = struktura.pliki[i]
        if bajtowa_nazwa:
            pliki_list.append(bajtowa_nazwa.decode("utf-8"))

    # --- podfoldery: rekurencja, wynik trafia do słownika "foldery" ---
    podfoldery = {}
    for i in range(struktura.ilosc_folderow):
        bajtowa_nazwa = struktura.foldery[i]
        if not bajtowa_nazwa or bajtowa_nazwa in (b".", b".."):
            continue

        nazwa_str = bajtowa_nazwa.decode("utf-8")

        if sciezka.endswith(b"/"):
            nowa_sciezka = sciezka + bajtowa_nazwa
        else:
            nowa_sciezka = sciezka + b"/" + bajtowa_nazwa

        if nowa_sciezka.startswith((b"/proc", b"/dev", b"/run", b"/sys", b"/tmp")):
            podfoldery[nazwa_str] = {"pominieto": "wykluczony folder systemowy"}
            continue

        nowa_struktura = lib.lista_rzeczy(nowa_sciezka)
        wynik = zbierz(nowa_struktura, nowa_sciezka)
        if wynik is not None:
            podfoldery[nazwa_str] = wynik

    return {
        "pliki": pliki_list,
        "foldery": podfoldery,
    }


class ZawartoscFolderu(ctypes.Structure):
    _fields_ = [
        ("pliki", ctypes.POINTER(ctypes.c_char_p)),
        ("ilosc_plikow", ctypes.c_int),
        ("foldery", ctypes.POINTER(ctypes.c_char_p)),
        ("ilosc_folderow", ctypes.c_int),
    ]


lib.lista_rzeczy.argtypes = [ctypes.c_char_p]
lib.lista_rzeczy.restype = ZawartoscFolderu

sciezka_do_pliku = "zawartosc.json"
sciezka = b"/"

struktura = lib.lista_rzeczy(sciezka)
drzewo = zbierz(struktura, sciezka)

with open(sciezka_do_pliku, "w", encoding="utf-8") as plik:
    json.dump(drzewo, plik, indent=4, ensure_ascii=False)
