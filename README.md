
# KRY Projekt 2: MAC za použití SHA-256 & Length extension attack
Autor: Vojtěch Šíma, xsimav01, 222085
Rok: 2023/2024, odevzdání 30.4.2024

## Popis funkčnosti
Nejprve jsou zpracovány jednotlivé argumenty. Pokud není zadán žádný, je vytištěna dokumetnace o tom jak program spuštět.
Pomocí getopt jsou načteny argumenty a uloženy jednotlivé flagy a k nim uloženy případné potřebné hodnoty.
Pokud nejsou nějaké argumenty validní či jim něco chybí, případně nesedí regex, je vypsána na obrazovku chyba
a běh programu je ukončen.

### Funkce checksum()
Jedná se o hlavní funkci, která vypočítává hash. Pokud se nejdné o length extension attack, důležitý je pouze první argument, který obsahuje zprávu k hashování, případně zprávu a heslo.
Zpráva je uložena do bloku (v případě že je delší, tak do více bloků), kam je vložena a následuje za ní oddělovač  0b10000000. Do posledních bloků je pak vložena délka zprávy,
v případě attacku je tato délka již předána této funkci argumentem. Pak již dochází k postupnému kopírování, rotacím a tím je získán celkový hash. Princip tohoto byl postupně krok po kroku
implementován podle https://sha256algorithm.com/. Pokud se jedná o attack, tak jako init hash je nastavena hodnota, které byla předána argumentem programu i této funkci. Na konci jsou hodnoty
výsledného hase poskládány a vráceny jako výstupní hodnota této funkce.

### ./kry -c
Dochází pouze k výpočtu hashe pro vstupní zprávu a jeho výpis na STDOUT
příkaz: echo -ne "zprava" | ./kry -c
výstup: d8305a064cd0f827df85ae5a7732bf25d578b746b8434871704e98cde3208ddf

### ./kry -s -k KRY
Dochází k výpočtu hashe pomocí složení hesla a vstupní zprávy
příkaz: echo -ne "zprava" | ./kry -s -k heslo
výstup: 23158796a45a9392951d9a72dffd6a539b14a07832390b937b94a80ddb6dc18e

### ./kry -v -k KRY -m MAC
Kontrola, zda vstupní zpráva a heslo vygeneruje totožný hash, které je zadán pomocí -m
příkaz:  echo -ne "zprava" | ./kry -v -k heslo -m 23158796a45a9392951d9a72dffd6a539b14a07832390b937b94a80ddb6dc18e
výstup: Není vypysováno nic, pouze je podle toho, zda se jedná o validní klíč či ne vrácena hodnota 0/1

### LENGHT EXTRANSION ATTACK
příkaz: echo -ne "zprava" | ./kry -e -n 5 -a ==message -m 23158796a45a9392951d9a72dffd6a539b14a07832390b937b94a80ddb6dc18e
výstup: d8305a064cd0f827df85ae5a7732bf25d578b746b8434871704e98cde3208ddf
        zprava\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x
        00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x
        00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x
        00\x00\x00\x00\x00\x00\x58==message

### Funkčnost
Program by měl bez problému pracovat správně ve všech případech

### Příklad překladu a spouštění
unzip 222085.zip
make
echo -ne "zprava" | ./kry -c

Makefile obsahuje sanizizéry, takže na serveru Merlin jde pouze přeložit a není možné ho spustit. Bez nich byla funkčnost bezprolémově otestována