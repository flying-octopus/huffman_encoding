# Huffman coding
This is my project for second semester of computer science course.

## Problem (in Polish)
Napisz program, który tworzy kod Huffmana dla ustalonego pliku tekstowego (program musi w tym celu policzyć wystąpienia poszczególnych znaków),
a następnie koduje ten plik. Napisz program, który dekoduje zakodowany plik.

## Problem (in English)
Write a program, which creates a Huffman's code for given text file (program needs to count instances of each character for the text), and then encodes the file. Write a program that decodes an encoded file.

### Algorithm (in Polish)
1. Tworzymy `n` liści (jeden na każdy unikalny znak) dla każdego znaku, który występuje przynajmniej raz i zapamiętujemy w nim jego ilość wystąpień (wagę).
2. Liście sortujemy w kolejności rosnących wag i wstawiamy (w tej kolejności) do kolejki.
3. Tworzymy drugą, początkowo pustą kolejkę, w której będziemy umieszczać wierzchołki wewnętrzne tworzonego drzewa.
4. Następnie wykonujemy `n` razy następujące czynności:
	1. Z kolejek wyjmujemy 2 wierzchołki (każdy z nich jest liściem albo wierzchołkiem wewnętrznym), o najmniejszych wagach. Wierzchołków tych należy poszukiwać na początku kolejek, ponieważ w kolejce z liścmi wagi wstawionych do niej elementów są uporządkowane rosnąco, zaś w kolejce z wierzchołkami wewnętrznymi elementy, które będziemy wstawiać kolejno, również będą miały co raz większe wagi.
	2. Aby wyjąć każdy z potrzebnych wierzchołków, jeśli obie kolejki są niepuste, to porównujemy wagi wierzchołków na początku tych kolejek i wyjmujemy wierzchołek o wadze mniejszej.
	3. Mając 2 wierzchołki wyjęte z kolejek tworzymy nowy wierzchołek wewnętrzny, korzeniami jego poddrzew czynimy wierzchołki wyjęte z kolejek.
	4. Nowemu wierzchołkowi przypisujemy wagę równą sumie wag korzeni poddrzew.
	5. Nowy wierzchołek wstawiamy na koniec kolejki wierzchołków wewnętrznych.
5. Po wykonaniu operacji z punktu 4 mamy jeden wierzchołek drzewa będący korzeniem drzewa Huffmana, którego używamy do zakodowania i dekodowania danych.
