#############################################################################################
Die Clustering Algorithmen und Experimente wurden auf der Boost Graph Library 
implementiert. Eine Anleitung zur Installation findet man unter: https://www.boost.org/

Diagramme zu Experimenten werden über Python mit den Packeten matplotlib, pandas, os erstellt.
Eine Anleitung zur Installation findet man unter: https://www.python.org/

Die Real World Daten PSMSL, welche man im Ordner srcPSMSL findet wurden direkt bei PSMSL unter 
https://www.psmsl.org/data/ heruntergeladen.

Zum Ausführen der verschiedenen Programme müssen Methoden in main.cc ein/aus kommentiert
werden.

Das Programm kann unter Berücksichtigung der folgenden Lizenzen 
Fréchet Distance, BGL, und Real World Daten  für
Wissenschaftliche Zwecke modifiziert und verwendet werden.

Es wird empfohlen zunächst eines der Programme der main.cc zu testen:

main_Clustering(n = 3, m = 3, k = 3, zeitreihenl = 5,
                SLDN = false, SLFDN = false, SL = false,
                CLDN = true, CLFDN = false, CL = false,
                DEBUGG_optional = true, DEBUGG_algo = true, SEED = true,
                data = 4, type_code = 1, amplitude = 0, jahr = 2000);

main_RealWorldtesten(data = 8, n = 50, k = 4, zeitreihenl = 100, jahr = 2000,
                        SLDN = true, SLFDN = false, SL = false,
                        CLDN = false, CLFDN = false, CL = false,
                        DEBUGG_algo = false, DEBUGG_optional = false, perm = false, SEED = true);


@author Benjamin Rochti
#############################################################################################
#############################################################################################



