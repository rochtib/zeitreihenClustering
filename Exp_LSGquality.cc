/**
 ***********************************************************************************
 ***********************************************************************************
 LOESUNGSQULITAET
 ***********************************************************************************
 ***********************************************************************************
 */

#include <limits>
extern bool DEBUGG_algo;
extern bool DEBUGG_optional;
extern bool SEED;

void LSGqualityRAND(const int data, const int zeitreihenl, const int n_ende, const int WDH_PERM, const int WDH_INPUT, const int jahr, const std::string algo_daten, const bool datenkontrolle, std::ofstream &offileBEZ)
{
    std::ofstream offileAVG;
    std::string pathAvg = "results/LSGquality/results/" + algo_daten + "_AvgMaxMin_zr" + to_string(zeitreihenl) + ".txt";
    offileAVG.open(pathAvg);

    std::ofstream offileCHECK;
    std::string pathCHECK = "results/LSGquality/InputKontrolle/CHECK_" + algo_daten + "_ZR" + to_string(zeitreihenl) + ".txt";

    if (datenkontrolle)
    {
        offileCHECK.open(pathCHECK);
        offileCHECK << algo_daten << "\n"
                    << "Es wurden " << WDH_INPUT << " Inputs betrachten.\n"
                    << "Jeder Input wurde " << WDH_PERM << " Mal permutiert.\n"
                    << "##########################################################################################################################\n"
                    << "##########################################################################################################################\n";
    }

    offileAVG << "Es wurden " << WDH_INPUT << " Inputs betrachten.\n"
              << "Jeder Input wurde " << WDH_PERM << " Mal permutiert.\n"
              << "##########################################################################################################################\n"
              << "##########################################################################################################################\n"
              << " n" << std::setw(3) << " "
              << "[AVG SL] "
              << "[AVG SLFDN/SL] "
              << "[MAX SLFDN/SL] "
              << "[MIN SLFDN/SL] "
              << "[AVG SLDN/SL] "
              << "[MAX SLDN/SL] "
              << "[MIN SLDN/SL] "
              << "[AVG CL] "
              << "[AVG CLFDN/CL] "
              << "[MAX CLFDN/CL] "
              << "[MIN CLFDN/CL] "
              << "[AVG CLDN/CL] "
              << "[MAX CLDN/CL] "
              << "[MIN CLDN/CL]\n";

    int MODUS;
    int counter_ablehung = 0;
    int counter_anzahl = 0;

    for (int n_index = 4; n_index <= n_ende + 1; n_index += 4)
    {
        double CL = 0;
        double SL = 0;
        double CLFDN = 0;
        double CLDN = 0;
        double SLFDN = 0;
        double SLDN = 0;

        double CL_abs = 0;
        double SL_abs = 0;

        double max_CLDN = 0;
        double max_CLFDN = 0;
        double max_SLDN = 0;
        double max_SLFDN = 0;

        double min_CLDN = std::numeric_limits<double>::max();
        double min_CLFDN = std::numeric_limits<double>::max();
        double min_SLDN = std::numeric_limits<double>::max();
        double min_SLFDN = std::numeric_limits<double>::max();

        const int n = n_index;
        const int m = n_index;
        const int k = n_index;
        const int anz_gitterpkt = n * m;
        GeneratorSeed seed_samen(0);
        for (int wdh_input = 1; wdh_input <= WDH_INPUT; wdh_input++)
        {
            Datenmodell d(anz_gitterpkt, k, zeitreihenl, jahr, algo_daten);
            d.erstelleDaten(n, m, k, nullptr, data, (int)NULL, seed_samen);

            // DEBUGG_algo = true;
            // d.print_DatenModellCluster(n, m);
            // DEBUGG_algo = false;

            if (datenkontrolle)
            {
                offileCHECK << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << "\n";
                d.write_data(n, m, offileCHECK);
            }

            double iter_CLFDN = 0;
            double iter_CLDN = 0;
            double iter_SLFDN = 0;
            double iter_SLDN = 0;

            for (int wdh_perm = 1; wdh_perm <= WDH_PERM; wdh_perm++)
            {
                std::cout << "EXP LSGquality " << algo_daten << " n:(" << n << "/" << n_ende << "), Input:(" << wdh_input << "/" << WDH_INPUT << "), Perm:("
                          << wdh_perm << "/" << WDH_PERM << ")" << std::endl;
                permutiereZeitreihe(d, zeitreihenl, anz_gitterpkt, 1, seed_samen);

                if (datenkontrolle)
                {
                    offileCHECK << "Permutation: " << wdh_perm << "\n";
                    d.write_data(n, m, offileCHECK);
                }
                counter_anzahl++;
                double *direkte4Nachbarn;
                direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));

                UndirectedGraph G_vollst(anz_gitterpkt);
                kompletterGraph(G_vollst, d, anz_gitterpkt, zeitreihenl);
                UndirectedGraph Graphdummy(0);
                ClusterAlgo SL(G_vollst, n, m, G_vollst, MODUS = 5, k, d, anz_gitterpkt, data);
                double zielfunktion_SL = zielfunktionSL(SL, G_vollst, n, m, k);
                SL_abs += zielfunktion_SL;
                SL.clear();

                berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, zeitreihenl);
                printDirekte4Nachbarn(direkte4Nachbarn, n, m);
                UndirectedGraph G_dijkstra(anz_gitterpkt);
                erstelleGitterGraph(G_dijkstra, direkte4Nachbarn, n, m);
                ClusterAlgo SLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 1, k, d, anz_gitterpkt, data);
                double zielfunktion_SLDN = zielfunktionSL(SLDN, G_vollst, n, m, k);
                iter_SLDN += zielfunktion_SLDN / zielfunktion_SL;
                SLDN.clear();

                if (max_SLDN < (double)(zielfunktion_SLDN / zielfunktion_SL))
                {
                    max_SLDN = (double)(zielfunktion_SLDN / zielfunktion_SL);
                }
                if (min_SLDN > (double)(zielfunktion_SLDN / zielfunktion_SL))
                {
                    min_SLDN = (double)(zielfunktion_SLDN / zielfunktion_SL);
                }

                ClusterAlgo SLFDN(G_vollst, n, m, G_dijkstra, MODUS = 2, k, d, anz_gitterpkt, data);
                double zielfunktion_SLFDN = zielfunktionSL(SLFDN, G_vollst, n, m, k);
                iter_SLFDN += zielfunktion_SLFDN / zielfunktion_SL;
                SLFDN.clear();

                if (max_SLFDN < (double)(zielfunktion_SLFDN / zielfunktion_SL))
                {
                    max_SLFDN = (double)(zielfunktion_SLFDN / zielfunktion_SL);
                }
                if (min_SLFDN > (double)(zielfunktion_SLFDN / zielfunktion_SL))
                {
                    min_SLFDN = (double)(zielfunktion_SLFDN / zielfunktion_SL);
                }

                if (zielfunktion_SL > zielfunktion_SLDN || zielfunktion_SL > zielfunktion_SLFDN ||
                    zielfunktion_SLFDN > zielfunktion_SLDN)
                {
                    counter_ablehung++;
                    offileBEZ << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << " ";
                    offileBEZ << "SL: " << zielfunktion_SL << " "
                              << "SLFDN: " << zielfunktion_SLFDN << " "
                              << "SLDN: " << zielfunktion_SLDN << " \n";
                }

                ClusterAlgo CL(Graphdummy, n, m, G_vollst, MODUS = 6, k, d, anz_gitterpkt, data);
                double zielfunktion_CL = zielfunktionCL(CL, G_vollst, n, m, k);
                CL_abs += zielfunktion_CL;
                CL.clear();

                ClusterAlgo CLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 3, k, d, anz_gitterpkt, data);
                double zielfunktion_CLDN = zielfunktionCL(CLDN, G_vollst, n, m, k);
                iter_CLDN += zielfunktion_CLDN / zielfunktion_CL;
                CLDN.clear();

                if (max_CLDN < (double)(zielfunktion_CLDN / zielfunktion_CL))
                {
                    max_CLDN = (double)(zielfunktion_CLDN / zielfunktion_CL);
                }
                if (min_CLDN > (double)(zielfunktion_CLDN / zielfunktion_CL))
                {
                    min_CLDN = (double)(zielfunktion_CLDN / zielfunktion_CL);
                }

                ClusterAlgo CLFDN(G_vollst, n, m, G_dijkstra, MODUS = 4, k, d, anz_gitterpkt, data);
                double zielfunktion_CLFDN = zielfunktionCL(CLFDN, G_vollst, n, m, k);
                iter_CLFDN += zielfunktion_CLFDN / zielfunktion_CL;
                CLFDN.clear();

                if (max_CLFDN < (double)(zielfunktion_CLFDN / zielfunktion_CL))
                {
                    max_CLFDN = (double)(zielfunktion_CLFDN / zielfunktion_CL);
                }
                if (min_CLFDN > (double)(zielfunktion_CLFDN / zielfunktion_CL))
                {
                    min_CLFDN = (double)(zielfunktion_CLFDN / zielfunktion_CL);
                }

                if (zielfunktion_CL > zielfunktion_CLDN || zielfunktion_CL > zielfunktion_CLFDN ||
                    zielfunktion_CLFDN > zielfunktion_CLDN)
                {
                    counter_ablehung++;
                    offileBEZ << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << " ";
                    offileBEZ << "CL: " << zielfunktion_CL << " "
                              << "CLFDN: " << zielfunktion_CLFDN << " "
                              << "CLDN: " << zielfunktion_CLDN << " \n";
                }

                Graphdummy.clear();
                G_dijkstra.clear();
                G_vollst.clear();

                free(direkte4Nachbarn);
            }
            d.clear();

            SL += SL_abs / WDH_PERM;
            SLDN += iter_SLDN / WDH_PERM;
            SLFDN += iter_SLFDN / WDH_PERM;

            CL += CL_abs / WDH_PERM;
            CLDN += iter_CLDN / WDH_PERM;
            CLFDN += iter_CLFDN / WDH_PERM;
        }

        SL = SL / WDH_INPUT;
        SLDN = SLDN / WDH_INPUT;
        SLFDN = SLFDN / WDH_INPUT;

        CL = CL / WDH_INPUT;
        CLDN = CLDN / WDH_INPUT;
        CLFDN = CLFDN / WDH_INPUT;

        offileAVG << std::setw(2) << n << ", ";

        offileAVG << std::setw(8) << SL << ","
                  << std::setw(14) << SLFDN << ","
                  << std::setw(14) << max_SLFDN << ","
                  << std::setw(14) << min_SLFDN << ",";

        offileAVG << std::setw(13) << SLDN << ","
                  << std::setw(13) << max_SLDN << ","
                  << std::setw(13) << min_SLDN << ",";

        offileAVG << std::setw(9) << CL << ","
                  << std::setw(14) << CLFDN << ","
                  << std::setw(14) << max_CLFDN << ","
                  << std::setw(14) << min_CLFDN << ",";

        offileAVG << std::setw(13) << CLDN << ","
                  << std::setw(13) << max_CLDN << ","
                  << std::setw(13) << min_CLDN << "\n";
    }
    offileBEZ << "####################################\n"
              << "counter_ablehung: " << counter_ablehung << "\n"
              << "counter_anzahl: " << counter_anzahl << "\n";

    offileAVG.close();

    if (datenkontrolle)
    {
        offileCHECK.close();
    }
}

void LSGqualityAmplitudeWachsend(const int data, const int WDH_INPUT, const int n_ende, const std::string zeitType, std::ofstream &offileBEZ)
{
    const int zeitreihenl = 15;
    int counter_ablehung = 0;
    int counter_anzahl = 0;
    std::string algo_daten = get_DatenmodellName(data);

    GeneratorSeed seed_samen(0);
    for (double amplitude = 1; amplitude <= 64; amplitude *= 2)
    {
        std::ofstream offilez;
        std::string pathz = "results/LSGquality/results/" + zeitType + to_string((int)amplitude) + ".txt";
        offilez.open(pathz);
        offilez << "Messung auf Synthethischen Daten\n"
                << "Der Mittelwert wurde ueber " << WDH_INPUT << " Wiederholungen gebildet\n"
                << "########################################################################\n"
                << " n" << std::setw(3) << " "
                << "a" << std::setw(4) << " "
                << "[AVG CL] "
                << "[AVG CLFDN] "
                << "[AVG CLDN] "
                << "[AVG CLFDN/CL] "
                << "[AVG CLDN/CL] "
                << "[AVG DN/FDN] "
                << " [AVG SL] "
                << "[AVG SLFDN] "
                << " [AVG SLDN] "
                << "[AVG SLFDN/SL] "
                << "[AVG SLDN/SL] "
                << "[AVG DN/FDN] "
                << "\n";

        int MODUS;
        for (int n_index = 4; n_index <= n_ende; n_index += 4)
        {
            const int n = n_index;
            const int m = n_index;
            const int k = n_index;
            const int anz_gitterpkt = n * m;

            double CLFDNz = 0;
            double CLDNz = 0;
            double CLz_abs = 0;
            double CLFDNz_abs = 0;
            double CLDNz_abs = 0;
            double CLDN_CLFDN_z = 0;

            double SLFDNz = 0;
            double SLDNz = 0;
            double SLz = 0;
            double SLFDNz_abs = 0;
            double SLDNz_abs = 0;
            double SLDN_SLFDN_z = 0;

            for (int iterINPUT = 1; iterINPUT <= WDH_INPUT; iterINPUT++)
            {
                std::cout << "EXP LSGquality Input:(" << iterINPUT << "/" << WDH_INPUT << "), amplitude:("
                          << amplitude << "/" << 64 << "), n:("
                          << n_index << "/" << n_ende << ")" << std::endl;
                double *direkte4Nachbarn;
                direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));
                Datenmodell d(anz_gitterpkt, k, zeitreihenl, (int)NULL, zeitType);
                d.erstelleDaten(n, m, k, nullptr, data, amplitude, seed_samen);
                counter_anzahl++;
                UndirectedGraph G_vollst(anz_gitterpkt);
                kompletterGraph(G_vollst, d, n * m, zeitreihenl);
                UndirectedGraph Graphdummy(0);
                ClusterAlgo CL(Graphdummy, n, m, G_vollst, MODUS = 6, k, d, anz_gitterpkt, data);

                berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, zeitreihenl);
                printDirekte4Nachbarn(direkte4Nachbarn, n, m);
                UndirectedGraph G_dijkstra(anz_gitterpkt);
                erstelleGitterGraph(G_dijkstra, direkte4Nachbarn, n, m);
                double iter_CLz = zielfunktionCL(CL, G_vollst, n, m, k);
                CL.clear();

                ClusterAlgo CLFDN(G_vollst, n, m, G_dijkstra, MODUS = 4, k, d, anz_gitterpkt, data);
                double iter_CLFDNz = zielfunktionCL(CLFDN, G_vollst, n, m, k);
                CLFDN.clear();

                ClusterAlgo CLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 3, k, d, anz_gitterpkt, data);
                double iter_CLDNz = zielfunktionCL(CLDN, G_vollst, n, m, k);
                CLDN.clear();

                CLDN_CLFDN_z += iter_CLDNz / iter_CLFDNz;
                CLDNz += iter_CLDNz / iter_CLz;
                CLFDNz += iter_CLFDNz / iter_CLz;
                CLz_abs += iter_CLz;
                CLFDNz_abs += iter_CLFDNz;
                CLDNz_abs += iter_CLDNz;

                if (iter_CLz > iter_CLDNz || iter_CLz > iter_CLFDNz ||
                    iter_CLFDNz > iter_CLDNz)
                {
                    counter_ablehung++;
                    offileBEZ << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << " ";
                    offileBEZ << "CL: " << iter_CLz << " "
                              << "CLFDN: " << iter_CLFDNz << " "
                              << "CLDN: " << iter_CLDNz << " \n";
                }

                ClusterAlgo SL(G_vollst, n, m, G_vollst, MODUS = 5, k, d, anz_gitterpkt, data);
                double iter_SLz = zielfunktionSL(SL, G_vollst, n, m, k);
                SL.clear();

                ClusterAlgo SLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 1, k, d, anz_gitterpkt, data);
                double iter_SLDNz = zielfunktionSL(SLDN, G_vollst, n, m, k);
                SLDN.clear();

                ClusterAlgo SLFDN(G_vollst, n, m, G_dijkstra, MODUS = 2, k, d, anz_gitterpkt, data);
                double iter_SLFDNz = zielfunktionSL(SLFDN, G_vollst, n, m, k);
                SLFDN.clear();

                if (iter_SLz > iter_SLDNz || iter_SLz > iter_SLFDNz ||
                    iter_SLFDNz > iter_SLDNz)
                {
                    counter_ablehung++;
                    offileBEZ << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << " ";
                    offileBEZ << "SL: " << iter_SLz << " "
                              << "SLFDN: " << iter_SLFDNz << " "
                              << "SLDN: " << iter_SLDNz << " \n";
                }

                SLDNz += iter_SLDNz / iter_SLz;
                SLFDNz += iter_SLFDNz / iter_SLz;
                SLz += iter_SLz;
                SLFDNz_abs += iter_SLFDNz;
                SLDNz_abs += iter_SLDNz;
                SLDN_SLFDN_z += iter_SLDNz / iter_SLFDNz;

                Graphdummy.clear();
                G_dijkstra.clear();
                G_vollst.clear();

                free(direkte4Nachbarn);
                d.clear();
            }

            CLz_abs = CLz_abs / WDH_INPUT;
            CLDNz_abs = CLDNz_abs / WDH_INPUT;
            CLFDNz_abs = CLFDNz_abs / WDH_INPUT;
            CLFDNz = CLFDNz / WDH_INPUT;
            CLDNz = CLDNz / WDH_INPUT;
            CLDN_CLFDN_z = CLDN_CLFDN_z / WDH_INPUT;
            offilez
                << std::setw(2) << n << ","
                << std::setw(4) << amplitude << ","
                << std::setw(9) << CLz_abs << ","
                << std::setw(11) << CLFDNz_abs << ","
                << std::setw(11) << CLDNz_abs << ","
                << std::setw(14) << CLFDNz << ","
                << std::setw(13) << CLDNz << ","
                << std::setw(13) << CLDN_CLFDN_z << ",";

            SLz = SLz / WDH_INPUT;
            SLFDNz = SLFDNz / WDH_INPUT;
            SLDNz = SLDNz / WDH_INPUT;
            SLFDNz_abs = SLFDNz_abs / WDH_INPUT;
            SLDNz_abs = SLDNz_abs / WDH_INPUT;
            SLDN_SLFDN_z = SLDN_SLFDN_z / WDH_INPUT;

            offilez << std::setw(9) << SLz << ","
                    << std::setw(11) << SLFDNz_abs << ","
                    << std::setw(11) << SLDNz_abs << ","
                    << std::setw(14) << SLFDNz << ","
                    << std::setw(13) << SLDNz << ","
                    << std::setw(13) << SLDN_SLFDN_z << "\n";
        }
        offilez.close();
        offileBEZ << "####################################\n"
                  << "counter_ablehung: " << counter_ablehung << "\n"
                  << "counter_anzahl: " << counter_anzahl << "\n";
    }
}

void LSGquality(const int data, const int zeitreihenl, const int n_ende, const int WDH_INPUT, const std::string algo_daten, const bool datenkontrolle, const double cluster_MULT, std::ofstream &offileBEZ)
{
    std::ofstream offileCHECK;
    std::string pathCHECK = "results/LSGquality/InputKontrolle/" + to_string(data) + "/CHECK_" + algo_daten + "_ZR" + to_string(zeitreihenl) + "Gen" + to_string((int)cluster_MULT) + ".txt";

    if (datenkontrolle)
    {
        offileCHECK.open(pathCHECK);
        offileCHECK << algo_daten << "\n"
                    << "Es wurden " << WDH_INPUT << " Inputs betrachten.\n"
                    << "##########################################################################################################################\n"
                    << "##########################################################################################################################\n";
    }

    std::ofstream offileSEP;
    std::string pathSEP = "results/approximation/results/" + algo_daten + "nicht_aggregiert.txt";
    offileSEP.open(pathSEP);

    offileSEP << "Alle einzelnen Werte.\n"
              << "##########################################################################################################################\n"
              << "##########################################################################################################################\n"
              << " n" << std::setw(3)
              << "     [SL]"
              << "       [SLFDN]"
              << "        [SLDN]"
              << "     [SLFDN/SL] "
              << "     [SLDN/SL] "
              << "    [DN/FDN] "
              << "     [CL] "
              << "      [CLFDN] "
              << "       [CLDN] "
              << "    [CLFDN/CL] "
              << "     [CLDN/CL] "
              << "    [DN/FDN] "
              << "\n";

    int MODUS;
    int counter_ablehung = 0;
    int counter_anzahl = 0;
    GeneratorSeed seed_samen(0);

    for (int index_err = 1; index_err <= 6; index_err++)
    {
        std::ofstream offileAVG;
        std::string pathAvg = "results/LSGquality/results/" + algo_daten + to_string(index_err) + "GT.txt";
        offileAVG.open(pathAvg);

        double max_CLDN = 0;
        double max_CLFDN = 0;
        double max_SLDN = 0;
        double max_SLFDN = 0;

        double min_CLDN = std::numeric_limits<double>::max();
        double min_CLFDN = std::numeric_limits<double>::max();
        double min_SLDN = std::numeric_limits<double>::max();
        double min_SLFDN = std::numeric_limits<double>::max();

        offileAVG << "Es wurden " << index_err - 1 << "x k  Permutationen gemacht.\n"
                  << "##########################################################################################################################\n"
                  << "##########################################################################################################################\n"
                  << " n" << std::setw(3)
                  << "  AVG[SL]"
                  << "    AVG[SLFDN]"
                  << "     AVG[SLDN]"
                  << "  AVG[SLFDN/SL] "
                  << "  AVG[SLDN/SL] "
                  << " AVG[DN/FDN] "
                  << "  AVG[CL] "
                  << "   AVG[CLFDN] "
                  << "    AVG[CLDN] "
                  << " AVG[CLFDN/CL] "
                  << "  AVG[CLDN/CL] "
                  << " AVG[DN/FDN] "
                  << "\n";

        for (int n_index = 4; n_index <= n_ende + 1; n_index += 4)
        {
            const int n = n_index;
            const int m = n_index;
            const int k = sqrt(n * m);
            const int anz_gitterpkt = n * m;
            double zielfunktion_CLFDN = 0;
            double zielfunktion_CLDN = 0;
            double zielfunktion_CL = 0;
            double CLDN_CLFDN_z = 0;
            double CLDN_CL = 0;
            double CLFDN_CL = 0;
            double zielfunktion_SLFDN = 0;
            double zielfunktion_SLDN = 0;
            double zielfunktion_SL = 0;
            double SLDN_SLFDN_z = 0;
            double SLDN_SL = 0;
            double SLFDN_SL = 0;

            for (int wdh_input = 1; wdh_input <= WDH_INPUT; wdh_input++)
            {
                Datenmodell d(anz_gitterpkt, k * cluster_MULT, zeitreihenl, (int)NULL, algo_daten);
                if (algo_daten == "APPROX_GT_type2a")
                {
                    d.erstelleDaten(n, m, k * cluster_MULT, nullptr, data, n_ende * index_err, seed_samen);
                }
                else if (algo_daten == "APPROX_GT_type2b")
                {
                    d.erstelleDaten(n, m, k * cluster_MULT, nullptr, data, index_err, seed_samen);
                }
                else if (algo_daten == "APPROX_GT_type2c")
                {
                    d.erstelleDaten(n, m, k * cluster_MULT, nullptr, data, index_err, seed_samen);
                }
                std::cout << "n" << n << "m" << m << "k" << k * cluster_MULT << std::endl;

                // DEBUGG_algo = true;
                // d.print_DatenModellCluster(n, m);
                // DEBUGG_algo = false;

                std::cout << "EXP LSGquality " << algo_daten << " n:(" << n << "/" << n_ende << "), Input:(" << wdh_input << "/" << WDH_INPUT << ") error:(" << index_err << "/6)" << std::endl;

                if (datenkontrolle)
                {
                    offileCHECK << "Input: " << wdh_input << "\n";
                    d.write_data(n, m, offileCHECK);
                }
                counter_anzahl++;
                double *direkte4Nachbarn;
                direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));
                UndirectedGraph G_vollst(anz_gitterpkt);
                kompletterGraph(G_vollst, d, anz_gitterpkt, zeitreihenl);
                UndirectedGraph Graphdummy(0);
                ClusterAlgo SL(G_vollst, n, m, G_vollst, MODUS = 5, k, d, anz_gitterpkt, data);
                double iter_SL = zielfunktionSL(SL, G_vollst, n, m, k);
                SL.clear();
                berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, zeitreihenl);
                printDirekte4Nachbarn(direkte4Nachbarn, n, m);
                UndirectedGraph G_dijkstra(anz_gitterpkt);
                erstelleGitterGraph(G_dijkstra, direkte4Nachbarn, n, m);
                ClusterAlgo SLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 1, k, d, anz_gitterpkt, data);
                double iter_SLDN = zielfunktionSL(SLDN, G_vollst, n, m, k);
                SLDN.clear();
                if (max_SLDN < (double)(zielfunktion_SLDN / zielfunktion_SL))
                {
                    max_SLDN = (double)(zielfunktion_SLDN / zielfunktion_SL);
                }
                if (min_SLDN > (double)(zielfunktion_SLDN / zielfunktion_SL))
                {
                    min_SLDN = (double)(zielfunktion_SLDN / zielfunktion_SL);
                }
                ClusterAlgo SLFDN(G_vollst, n, m, G_dijkstra, MODUS = 2, k, d, anz_gitterpkt, data);
                double iter_SLFDN = zielfunktionSL(SLFDN, G_vollst, n, m, k);

                SLFDN.clear();

                if (max_SLFDN < (double)(zielfunktion_SLFDN / zielfunktion_SL))
                {
                    max_SLFDN = (double)(zielfunktion_SLFDN / zielfunktion_SL);
                }
                if (min_SLFDN > (double)(zielfunktion_SLFDN / zielfunktion_SL))
                {
                    min_SLFDN = (double)(zielfunktion_SLFDN / zielfunktion_SL);
                }

                if (iter_SL > iter_SLDN || iter_SL > iter_SLFDN ||
                    iter_SLFDN > iter_SLDN)
                {
                    counter_ablehung++;
                    offileBEZ << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << " ";
                    offileBEZ << "SL: " << iter_SL << " "
                              << "SLFDN: " << iter_SLFDN << " "
                              << "SLDN: " << iter_SLDN << " \n";
                }

                ClusterAlgo CL(Graphdummy, n, m, G_vollst, MODUS = 6, k, d, anz_gitterpkt, data);
                double iter_CL = zielfunktionCL(CL, G_vollst, n, m, k);
                CL.clear();
                ClusterAlgo CLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 3, k, d, anz_gitterpkt, data);
                double iter_CLDN = zielfunktionCL(CLDN, G_vollst, n, m, k);
                CLDN.clear();

                if (max_CLDN < (double)(zielfunktion_CLDN / zielfunktion_CL))
                {
                    max_CLDN = (double)(zielfunktion_CLDN / zielfunktion_CL);
                }
                if (min_CLDN > (double)(zielfunktion_CLDN / zielfunktion_CL))
                {
                    min_CLDN = (double)(zielfunktion_CLDN / zielfunktion_CL);
                }
                ClusterAlgo CLFDN(G_vollst, n, m, G_dijkstra, MODUS = 4, k, d, anz_gitterpkt, data);
                double iter_CLFDN = zielfunktionCL(CLFDN, G_vollst, n, m, k);
                CLFDN.clear();

                if (max_CLFDN < (double)(zielfunktion_CLFDN / zielfunktion_CL))
                {
                    max_CLFDN = (double)(zielfunktion_CLFDN / zielfunktion_CL);
                }
                if (min_CLFDN > (double)(zielfunktion_CLFDN / zielfunktion_CL))
                {
                    min_CLFDN = (double)(zielfunktion_CLFDN / zielfunktion_CL);
                }

                if (iter_CL > iter_CLDN || iter_CL > iter_CLFDN ||
                    iter_CLFDN > iter_CLDN)
                {
                    counter_ablehung++;
                    offileBEZ << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << " ";
                    offileBEZ << "CL: " << iter_CL << " "
                              << "CLFDN: " << iter_CLFDN << " "
                              << "CLDN: " << iter_CLDN << " \n";
                }

                zielfunktion_SL += iter_SL;
                zielfunktion_SLDN += iter_SLDN;
                zielfunktion_SLFDN += iter_SLFDN;
                SLDN_SL += iter_SLDN / iter_SL;
                SLFDN_SL += iter_SLFDN / iter_SL;
                SLDN_SLFDN_z += iter_SLDN / iter_SLFDN;

                zielfunktion_CL += iter_CL;
                zielfunktion_CLDN += iter_CLDN;
                zielfunktion_CLFDN += iter_CLFDN;
                CLDN_CL += iter_CLDN / iter_CL;
                CLFDN_CL += iter_CLFDN / iter_CL;
                CLDN_CLFDN_z += iter_CLDN / iter_CLFDN;

                offileSEP << std::setw(2) << n_index << ", ";

                offileSEP << std::setw(9) << iter_CL << ","
                          << std::setw(13) << iter_CLFDN << ","
                          << std::setw(13) << iter_CLDN << ","
                          << std::setw(14) << iter_CLFDN / iter_CL << ","
                          << std::setw(14) << iter_CLDN / iter_CL << ","
                          << std::setw(13) << iter_CLDN / iter_CLFDN << ",";
                offileSEP << std::setw(8) << iter_SL << ","
                          << std::setw(13) << iter_SLFDN << ","
                          << std::setw(13) << iter_SLDN << ","
                          << std::setw(14) << iter_SLFDN / iter_SL << ","
                          << std::setw(14) << iter_SLDN / iter_SL << ","
                          << std::setw(13) << iter_SLDN / iter_SLFDN << "\n";

                Graphdummy.clear();
                G_dijkstra.clear();
                G_vollst.clear();

                free(direkte4Nachbarn);
                d.clear();
            }
            zielfunktion_SL = zielfunktion_SL / WDH_INPUT;
            zielfunktion_SLDN = zielfunktion_SLDN / WDH_INPUT;
            zielfunktion_SLFDN = zielfunktion_SLFDN / WDH_INPUT;
            SLDN_SL = SLDN_SL / WDH_INPUT;
            SLFDN_SL = SLFDN_SL / WDH_INPUT;
            SLDN_SLFDN_z = SLDN_SLFDN_z / WDH_INPUT;

            zielfunktion_CL = zielfunktion_CL / WDH_INPUT;
            zielfunktion_CLDN = zielfunktion_CLDN / WDH_INPUT;
            zielfunktion_CLFDN = zielfunktion_CLFDN / WDH_INPUT;
            CLDN_CL = CLDN_CL / WDH_INPUT;
            CLFDN_CL = CLFDN_CL / WDH_INPUT;
            CLDN_CLFDN_z = CLDN_CLFDN_z / WDH_INPUT;

            offileAVG << std::setw(2) << n_index << ", ";

            offileAVG << std::setw(8) << zielfunktion_SL << ","
                      << std::setw(13) << zielfunktion_SLFDN << ","
                      << std::setw(13) << zielfunktion_SLDN << ","
                      << std::setw(14) << SLFDN_SL << ","
                      << std::setw(14) << SLDN_SL << ","
                      << std::setw(13) << SLDN_SLFDN_z << ",";

            offileAVG << std::setw(9) << zielfunktion_CL << ","
                      << std::setw(13) << zielfunktion_CLFDN << ","
                      << std::setw(13) << zielfunktion_CLDN << ","
                      << std::setw(14) << CLFDN_CL << ","
                      << std::setw(14) << CLDN_CL << ","
                      << std::setw(13) << CLDN_CLFDN_z << "\n";
        }
        offileAVG.close();
    }
    offileSEP.close();
    if (datenkontrolle)
    {
        offileCHECK.close();
    }
    offileBEZ << "####################################\n"
              << "counter_ablehung: " << counter_ablehung << "\n"
              << "counter_anzahl: " << counter_anzahl << "\n"
              << "####################################\n";
}

void main_LSGquality(const bool seed)
{
    //BEGINN GESAMT LAUFZEIT DES EXPERIEMENNTS
    struct timeval begin_exp, end_exp;
    long seconds_exp;
    long microseconds_exp;
    double time_exp;
    gettimeofday(&begin_exp, 0);

    SEED = seed;
    DEBUGG_algo = false;
    DEBUGG_optional = false;
    CHECK = false;

    int data, zr, jahr, n_ende;
    double ClusterMULT;
    std::string data_type;
    int WDH_INPUT = 15;
    int WDH_PERM;
    bool datenkontrolle = false;
    KantenSpeicher = true;

    std::ofstream offile;
    std::string path = "results/LSGquality/results/INFO.txt";
    offile.open(path);

    std::ofstream offileBEZ;
    std::string pathBEZ = "results/LSGquality/results/Beziehungen_report.txt";
    offileBEZ.open(pathBEZ);

    // LSGqualityRAND(data = 6, zr = 15, n_ende = 20, WDH_PERM = 10, 10, jahr = 1990, data_type = "RealWorld", datenkontrolle, offileBEZ);
    LSGqualityAmplitudeWachsend(data = 4, WDH_INPUT, n_ende = 20, "kontrollAmplitude", offileBEZ);
    // LSGquality(data = 4, zr = 15, n_ende = 20, WDH_INPUT, data_type = "APPROX_GT_type2a", datenkontrolle, ClusterMULT = 1, offileBEZ);
    LSGquality(data = 4, zr = 15, n_ende = 20, WDH_INPUT, data_type = "APPROX_GT_type2b", datenkontrolle, ClusterMULT = 1, offileBEZ);
    LSGquality(data = 4, zr = 15, n_ende = 20, WDH_INPUT, data_type = "APPROX_GT_type2c", datenkontrolle, ClusterMULT = 1, offileBEZ);
    // ENDE ZEITMESSUNG
    gettimeofday(&end_exp, 0);
    seconds_exp = end_exp.tv_sec - begin_exp.tv_sec;
    microseconds_exp = end_exp.tv_usec - begin_exp.tv_usec;
    time_exp = seconds_exp + microseconds_exp * 1e-6;

    offile << "####################################\n"
           << "Gesamt Laufzeit Experiment approximation: " << get_stringTime(time_exp) << "\n";

    time_t aktuelleZeit;
    struct tm lokal;
    aktuelleZeit = time(NULL);
    lokal = *localtime(&aktuelleZeit);
    offile << "Durchgefuehrt: " << currentDateTime() << "\n";
    offile.close();
    offileBEZ.close();
}