# Compilation
Standardtargets von cmake/make
# Nutzung
    SATsolver $(inputfile)
Disclaimer: Ich habe den Parser nicht auf Instanzen getestet, bei denen eine Klausel über mehrere Zeilen geht, eine Zeile mehrere Klauseln beinhaltet. 
Sollte funktionieren, muss es aber nicht.
# Output
`$(inputfile).sol`
Als DIMACS-File dort, wo auch `$(inputfile)` herkam.

    [FINISHED]: SATISFIABLE
bzw.

    [FINISHED]: UNSATISFIABLE
auf stdout.
# Sonstiges
Falls interessant, könnt ihr in der `logger.h` das Logging-Level für stdout setzen.