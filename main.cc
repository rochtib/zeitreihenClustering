/**
 * @file main.cc
 * @author Benjamin Rochti
 * @brief Zeitreihen Clusterings auf dem Gitter unter Lokalitaetsbedingungen
 * @date 2021-09-27
 */
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <tuple>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <iomanip>
#include <sys/time.h>
#include <ctime>
#include <vector>
#include <list>
#include <iterator>
#include <limits>
#include <set>
#include <unordered_map>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <unistd.h>
#include <time.h>

#define RESET "\033[0m"
#define RED "\033[31m"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>

using namespace boost;
using namespace std;

#include "Zeitreihen.cc"            // Erstellt die Zeitreihen
#include "Datenmodell.cc"           // Erstellt die Anfangspartitionen
#include "Frechet_Distance.cc"      // Berechnet den Frechet Abstand zweier Zeitreihen
#include "MinHeap.cc"               // Naehste Nachbar Datenstrucktur: MinHeap
#include "DatenstruckturGraph.cc"   // Einfuege/Loesch/print/.. Methoden
#include "Checks.cc"                // Checks fuer Clustering
#include "BGL_Visitor.cc"           // Abbruchbedingung BGL
#include "ClusteringAlgorithmus.cc" // Algorithmus/Zielfunktionen/purity/testMethoden
#include "Exp_RealWorld.cc"
#include "Exp_QualityPurity.cc"
#include "Exp_LSGquality.cc"
#include "Exp_RunTime.cc"
#include "EXP_SLaufCL.cc"

bool DEBUGG_algo = false;        // Konsolen Ausgabe einschalten
bool DEBUGG_optional = false;    // weitere Ausgaben Methoden einschalten
bool KORREKT = true;             // zur Verifizierung der Algorithmen
bool KantenSpeicher = true;      // sollen alle berechnetn Frechet Abstaende gespeichert werden
bool CHECK = false;              // Checks einschalten
struct timespec begintt;         // Zeitmessung
struct timespec endtt;           // Zeitmessung
bool SEED = false;               // Reproduzierbarkeit
double zeit_global;              // Zeitmessung
double linkage_zielfunktionHeap; // speichert abstand aus dem MinHeap
int counterFrechet;              // Zaehler fuer Frechet Berechnungen

int main()
{
   bool SLDN, SLFDN, SL, CLDN, CLFDN, CL, perm;
   int n, m, k, zeitreihenl, jahr;
   int data, type_code;
   double amplitude;

   /**
    * Demonstriert Clustering Algorithmen auf synthischen Daten
    * -> type_code=1,2,4  Wahl des Zeitreihen Generators
    * -> data=1,2,3,4 Wahl der Anfangspartition
    * -> amplitude bei Zeitreihen Erstellung
    * (Es koennnen auch permutierte Real World Daten gewaehlt werden daher Variable jahr)
    */
   // main_Clustering(n = 2, m = 2, k = 1, zeitreihenl = 5,
   //                 SLDN = false, SLFDN = false, SL = false,
   //                 CLDN = true, CLFDN = false, CL = false,
   //                 DEBUGG_optional = false, DEBUGG_algo = true, SEED = true,
   //                 data = 4, type_code = 1, amplitude = 0, jahr = 2000);

   /**
    * Demonstriert Clustering Algorithmen auf Real World PSMSL Daten
    * -> data=7,8 bestimmt Real World Daten
    * -> perm sollen daten zusaetzlich permutiert werden?
    */
  // main_RealWorldtesten(data = 8, n = 50, k = 4, zeitreihenl = 100, jahr = 2000,
                        // SLDN = true, SLFDN = false, SL = false,
                        // CLDN = false, CLFDN = false, CL = false,
                        // DEBUGG_algo = false, DEBUGG_optional = false, perm = false, SEED = true);

   /**
    * Experiment 0: Verifizierung des Codes
    */
    //main_Korrektheit(SEED = true, n = 10);

   /**
    * Experiment 1: Purity
    */
   //  main_QualityPurity(SEED = true);

   /**
    * Experiment 2: Loesungsqualitaet 
    */
   //  main_LSGquality(SEED = true);

   /**
   * Experiment 3: PSMSL Real World Daten 
   */
   // main_origDaten(SEED = true);

   /**
    * Experiment 4: Laufzeit 
    */
   // main_Laufzeit(SEED = true);

   /**
    * Experiment 5: Single Linkage auf Complete Linkage Zielfunktion 
    */
   //  main_SLaufCL(SEED = true);

   /**
    * Test Methoden
    */
   // main_printZeitreihenVisualisierung(SEED = true);
   // main_zumtesten_Datenmodell(SEED=true);
   // main_zumtesten_MainHeap();
   // main_zumtesten_Frechet();

   return 0;
}
