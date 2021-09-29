/**
 * 
 * Autor der Frecht liabrary der Autoren Bringmann, Marvin Künnemann, André Nusser
 * https://github.com/chaot4/frechet_distance
 * Code Lizens zu finden in LIZENS
 * 
 * Code Aenderung:
 * In Programm frechet_light.cpp                                  
 * Zeile auskommentiert: assert(outputs.id1.valid() || outputs.id2.valid());
 * 
 * Error error: use of undeclared identifier
      'outputs'
        assert(outputs.id1.valid() || outputs.id2.valid()); 
 */

#include "frechetLIB/geometry_basics.h"
#include "frechetLIB/id.h"
#include "frechetLIB/kdtree.h"
//#include "calc_frechet_distance.cpp"
#include "frechetLIB/orth_range_search.cpp"
//#include "certificate.cpp"
//#include "certificate.h"
//#include "compare_implementations.cpp"
#include "frechetLIB/parser.h"
//#include "create_benchmark_decider.cpp"
#include "frechetLIB/curve.cpp"
#include "frechetLIB/curve.h"
#include "frechetLIB/curves.h"
#include "frechetLIB/defs.h"
//#include "export_freespace_diagram.cpp"
#include "frechetLIB/filter.cpp"
#include "frechetLIB/filter.h"
#include "frechetLIB/frechet_abstract.h"
#include "frechetLIB/frechet_light.cpp"
#include "frechetLIB/frechet_light.h"
#include "frechetLIB/frechet_light_types.h"
#include "frechetLIB/frechet_naive.cpp"
#include "frechetLIB/frechet_naive.h"
#include "frechetLIB/times.h"
//#include "freespace_light_vis.cpp"
//#include "freespace_light_vis.h"
#include "frechetLIB/geometry_basics.cpp"
#include "frechetLIB/layer_queue.h"
#include "frechetLIB/orth_range_search.h"
#include "frechetLIB/parser.cpp"
#include "frechetLIB/priority_search_tree.h"
//#include "pruning_progress.cpp"
//#include "query.cpp"
#include "frechetLIB/query.h"
#include "frechetLIB/query_helper.h"
#include "frechetLIB/range.h"
#include "frechetLIB/range_tree.h"
//#include "run_tests.cpp"
//#include "test_curves.cpp"
#include "frechetLIB/times.cpp"
//#include "unit_tests.cpp"
#include "frechetLIB/unit_tests.h"

extern bool DEBUGG_optional;

/**
 * @brief Berechnet die Frechet Distanz zwischen zweier Zeitreihen. Die Zeitreihen sind in der
 * zeitreihenMatrix gespeichert.
 * 
 * @param zeitreihenMatrix Matrix enthaelt alle Zeitreihen aller Gitterpunkte
 * @param curve1 Index der Zeitreihe in zeitreihenMatrix ACHTUNG Matrix startet bei 0!
 * @param curve2 Index der Zeitreihe in zeitreihenMatrix ACHTUNG Matrix startet bei 0!
 * @param zeitreihen_l laenge der Zeitreihe
 * @return double wert der frechetdistanz
 */
double callcFrechet(Datenmodell &d, const int curve1, const int curve2, const int zeitreihen_l)
{
    FrechetLight frechet;
    distance_t y1, y2; 
    Curve curve3;
    Curve curve4;
    distance_t zeit_curve3 = 0;
    distance_t zeit_curve4 = 0;

    for (int i = 0; i < zeitreihen_l; i++)
    {
        y1 = d.get_Zeitreihenwert(curve1 * zeitreihen_l + i);
        curve3.push_back({zeit_curve3, y1});
        zeit_curve3++;
        // std::cout<<std::setw(2)<<d.get_Zeitreihenwert(curve1*zeitreihen_l+i)<<"  ";

        y2 = d.get_Zeitreihenwert(curve2 * zeitreihen_l + i);
        curve4.push_back({zeit_curve4, y2});
        zeit_curve4++;
        // std::cout<<std::setw(2)<<d.get_Zeitreihenwert(curve2*zeitreihen_l+i)<<"  "<<std::endl;;
    }

    // print_berechneteZeitreihen(zeitreihenMatrix,curve1,curve2,zeitreihen_l);
    auto distance = frechet.calcDistance(curve3, curve4);
    // std::cout << "The ARRAY Fréchet distance is: " << std::setprecision(20) << distance << "\n";
    // std::cout<<"CPP ARRAY Distanzce "<<distance<<std::endl;

    return distance;
}

/**
 * @brief Diese Methode berechnet die Frechetabstaende der direkten Nachbarn von Gitterpunkten
 * und fuegt die werte in direkte4Nachbarn ein. Es wird jede Gitterkante genau ein mal berechnet.
 * 
 */
void berechneDirekteNachbarn(Datenmodell &d, double *direkte4Nachbarn, const int n, const int m, const int zeitreihen_l)
{
    zeitMessen_start();
    // std::cout << "Start berechne Frechet Distanzen"<<std::endl;
    for (int i = 0; i < n * m; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            direkte4Nachbarn[i * 4 + j] = INT_MAX;
        }
    }
    int help;
    double distanz;
    int count = 0;
    const int konstant = 2 * n * m - m - n;
    double prozent = 0.25;
    std::cout << "Frechet Distanzen Status: || ";
    for (int i = 1; i <= n * m; i++)
    {
        help = i - 1;
        // std::cout<<count<<"   "<<std::endl;
        if ((double)count / konstant >= prozent)
        {
            std::cout << prozent * 100 << "% || ";
            prozent = prozent + 0.25;
        }
        // std::cout<<" help "<<help<< "  i"<<i <<std::endl;
        if (i % n != 0)
        {
            // std::cout<<"Case Rechts: "<<i <<" zu"<<i+1<<std::endl;
            distanz = callcFrechet(d, help, help + 1, zeitreihen_l);
            direkte4Nachbarn[help * 4 + 2] = distanz;
            direkte4Nachbarn[(help + 1) * 4] = distanz;
            count++;
        }
        if (i + n <= n * m)
        {
            // std::cout<<"Case Runter: i:"<<i<<" zu"<<i+n<<std::endl;
            distanz = callcFrechet(d, help, help + n, zeitreihen_l);
            direkte4Nachbarn[help * 4 + 3] = distanz;
            direkte4Nachbarn[(help + n) * 4 + 1] = distanz;
            count++;
        }
    }
    zeitMessen_ende();
    std::cout << "Abgeschlossen. " << std::endl;
}

/**
 * @brief Test Methode fuer berechnung.cc. Damit die man hier funktioniert muss das extern in obigen
 * Ausdruck <<extern const bool DEBUGG_optional>> entfernt werden
 */
int main_zumtesten_Frechet()
{
    // callc("srcPSMSL/11.txt", "srcPSMSL/22.txt");

    int *array = (int *)malloc(4 * 2 * sizeof(int *));

    for (int i = 0; i < 4; i++)
    {
        array[i * 2] = i;
        array[i * 2] = 1;
    }

    for (int i = 0; i < 4; i++)
    {
        std::cout << array[i * 2] << " " << array[i * 2] << std::endl;
    }

    // std::cout << "The Fréchet distance is: " << std::setprecision(20) << distance << "\n";

    std::cout << "Done" << std::endl;
    return 0;
}