set datafile separator whitespace
set key outside
set grid
set term png size 1200,800
set output 'czas.png'
set title 'Czas wykonania'
set xlabel 'n'
set ylabel 'czas [ms]'
set logscale y
plot 'wyniki.dat' using 1:12 title 'BFS' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:13 title 'DFS' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:14 title 'H1' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:15 title 'H2' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:16 title 'Hdod' with linespoints lw 2 pt 7
set output 'open.png'
set title 'Rozmiar OPEN'
set xlabel 'n'
set ylabel 'rozmiar OPEN'
plot 'wyniki.dat' using 1:2 title 'BFS' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:4 title 'DFS' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:6 title 'H1' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:8 title 'H2' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:10 title 'Hdod' with linespoints lw 2 pt 7
set output 'closed.png'
set title 'Rozmiar CLOSED'
set xlabel 'n'
set ylabel 'rozmiar CLOSED'
plot 'wyniki.dat' using 1:3 title 'BFS' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:5 title 'DFS' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:7 title 'H1' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:9 title 'H2' with linespoints lw 2 pt 7, \
     'wyniki.dat' using 1:11 title 'Hdod' with linespoints lw 2 pt 7
