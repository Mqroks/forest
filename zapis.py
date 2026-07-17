import os
import ctypes
import json

sciezka_os = os.path.abspath("pliki.so")
lib = ctypes.CDLL(sciezka_os)


def zapis(struktura, sciezka, sciezka_do_pliku):
    # 1. DIAGNOZA: Wyświetlamy, co Python otrzymał od C
    print(
        f"Odczytano {struktura.ilosc_plikow} plików i {struktura.ilosc_folderow} folderów z: {sciezka.decode('utf-8')}"
    )

    # 2. TARCZA: Jeśli liczby są z kosmosu (śmieci z pamięci), przerywamy!
    if (
        struktura.ilosc_folderow < 0
        or struktura.ilosc_folderow > 100000
        or struktura.ilosc_plikow < 0
    ):
        print(
            "UWAGA: Funkcja w C zwróciła uszkodzone dane z pamięci! Pomijam ten folder."
        )
        return

    # 3. TARCZA: Sprawdzamy, czy wskaźniki w ogóle istnieją (nie są NULL)
    if struktura.ilosc_plikow > 0 and not struktura.pliki:
        print("UWAGA: Brakuje wskaźnika na pliki! Pomijam.")
        return

    if struktura.ilosc_folderow > 0 and not struktura.foldery:
        print("UWAGA: Brakuje wskaźnika na foldery! Pomijam.")
        return
    pliki_list = []
    for i in range(struktura.ilosc_plikow):
        bajtowa_nazwa = struktura.pliki[i]
        if bajtowa_nazwa:
            pliki_list.append(bajtowa_nazwa.decode("utf-8"))

    with open(sciezka_do_pliku, "a", encoding="utf-8") as plik:
        json.dump(
            {"pliki": pliki_list, "ilosc_plikow": struktura.ilosc_plikow},
            plik,
            indent=4,
            ensure_ascii=False,
        )

    foldery_list = []
    for i in range(struktura.ilosc_folderow):
        bajtowa_nazwa = struktura.foldery[i]
        if bajtowa_nazwa and bajtowa_nazwa not in [b".", b".."]:
            foldery_list.append(bajtowa_nazwa.decode("utf-8"))

            if sciezka.endswith(b"/"):
                nowa_sciezka = sciezka + bajtowa_nazwa
            else:
                nowa_sciezka = sciezka + b"/" + bajtowa_nazwa
            if not nowa_sciezka.startswith(
                (b"/proc", b"/dev", b"/run", b"/sys", b"/tmp")
            ):
                nowa_struktura = lib.lista_rzeczy(nowa_sciezka)

                zapis(nowa_struktura, nowa_sciezka, sciezka_do_pliku)

    with open(sciezka_do_pliku, "a", encoding="utf-8") as plik:
        json.dump(
            {"foldery": foldery_list, "ilosc_folderow": struktura.ilosc_folderow},
            plik,
            indent=4,
            ensure_ascii=False,
        )


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
zapis(struktura, sciezka, sciezka_do_pliku)
