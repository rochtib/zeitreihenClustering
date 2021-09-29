/**
 ***********************************************************************************
 ***********************************************************************************    
 PURITY UNTERSUCHUNG 
 ***********************************************************************************
 ***********************************************************************************
 **/

extern bool DEBUGG_algo;
extern bool DEBUGG_optional;
extern bool SEED;
extern bool CHECK;
extern double linkage_zielfunktionHeap;
extern int counterFrechet;

void messen_qualityCL(const int data, const int zeitreihenl, const int jahr, const int WDH_INPUT, const int n_ende)
{
    std::string algo_daten = get_DatenmodellName(data);

    GeneratorSeed seed_samen(0);
    for (int n_index = 4; n_index <= n_ende + 1; n_index += 4)
    {
        std::ofstream offilep;
        std::string pathp = "results/quality/results/purityCL_n" + to_string(n_index) + ".txt";
        offilep.open(pathp);
        offilep << "Purity Messung auf Synthethischen Daten\n"
                << "Der Mittelwert wurde ueber " << WDH_INPUT << " Wiederholungen gebildet\n"
                << "########################################################################\n"
                << " n" << std::setw(3) << " "
                << "a" << std::setw(4) << " "
                << "[AVG CL] "
                << "[AVG CLFDN] "
                << " [AVG CLDN] "
                << "[AVG CLFDN/CL] "
                << "[AVG CLDN/CL]\n";
        std::ofstream offilez;
        std::string pathz = "results/quality/results/functionCL_n" + to_string(n_index) + ".txt";
        offilez.open(pathz);
        offilez << "Zielfunktion Messung auf Synthethischen Daten\n"
                << "Der Mittelwert wurde ueber " << WDH_INPUT << " Wiederholungen gebildet\n"
                << "########################################################################\n"
                << " n" << std::setw(3) << " "
                << "a" << std::setw(4) << " "
                << "[AVG CL] "
                << "[AVG CLFDN] "
                << "[AVG CLDN] "
                << "[AVG CLFDN/CL] "
                << "[AVG CLDN/CL] "
                << "[AVG CLDN/CLFDN]\n";

        int MODUS;
        for (double amplitude = 0.25; amplitude <= 1.01; amplitude += 0.05)
        {
            const int n = n_index;
            const int m = n_index;
            const int k = n_index;
            const int anz_gitterpkt = n * m;
            double CLFDNp = 0;
            double CLDNp = 0;
            double CLFDNp_abs = 0;
            double CLDNp_abs = 0;
            double CLp_abs = 0;
            double CLFDNz = 0;
            double CLDNz = 0;
            double CLz_abs = 0;
            double CLFDNz_abs = 0;
            double CLDNz_abs = 0;
            double CLDN_CLFDN = 0;

            for (int iterINPUT = 1; iterINPUT <= WDH_INPUT; iterINPUT++)
            {
                std::cout << "EXPQualityCL Input:(" << iterINPUT << "/" << WDH_INPUT << "), amplitude:("
                          << amplitude << "/" << 1 << "), n:("
                          << n_index << "/" << n_ende << ")" << std::endl;
                double *direkte4Nachbarn;
                direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));
                Datenmodell d(anz_gitterpkt, k, zeitreihenl, jahr, "kontrollAmplitude");
                d.erstelleDaten(n, m, k, nullptr, data, amplitude, seed_samen);
                UndirectedGraph G_vollst(anz_gitterpkt);
                kompletterGraph(G_vollst, d, n * m, zeitreihenl);
                UndirectedGraph Graphdummy(0);
                ClusterAlgo CL(Graphdummy, n, m, G_vollst, MODUS = 6, k, d, anz_gitterpkt, data);

                berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, zeitreihenl);
                printDirekte4Nachbarn(direkte4Nachbarn, n, m);
                UndirectedGraph G_dijkstra(anz_gitterpkt);
                erstelleGitterGraph(G_dijkstra, direkte4Nachbarn, n, m);

                double Iter_CLp = purity(CL, d, n, m, k);
                double Iter_CLz = zielfunktionCL(CL, G_vollst, n, m, k);
                CL.clear();

                ClusterAlgo CLFDN(G_vollst, n, m, G_dijkstra, MODUS = 4, k, d, anz_gitterpkt, data);
                double Iter_CLFDNp = purity(CLFDN, d, n, m, k);
                double Iter_CLFDNz = zielfunktionCL(CLFDN, G_vollst, n, m, k);
                CLFDN.clear();

                ClusterAlgo CLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 3, k, d, anz_gitterpkt, data);
                double Iter_CLDNp = purity(CLDN, d, n, m, k);
                double Iter_CLDNz = zielfunktionCL(CLDN, G_vollst, n, m, k);
                CLDN.clear();

                CLDNp += Iter_CLDNp / Iter_CLp;
                CLDNp_abs += Iter_CLDNp;
                CLFDNp += Iter_CLFDNp / Iter_CLp;
                CLFDNp_abs += Iter_CLFDNp;
                CLp_abs += Iter_CLp;

                CLDNz += Iter_CLDNz / Iter_CLz;
                CLFDNz += Iter_CLFDNz / Iter_CLz;
                CLz_abs += Iter_CLz;
                CLFDNz_abs += Iter_CLFDNz;
                CLDNz_abs += Iter_CLDNz;

                CLDN_CLFDN += Iter_CLDNz / Iter_CLFDNz;

                Graphdummy.clear();
                G_dijkstra.clear();
                G_vollst.clear();

                free(direkte4Nachbarn);
                d.clear();
            }
            CLp_abs = CLp_abs / WDH_INPUT;
            CLDNp_abs = CLDNp_abs / WDH_INPUT;
            CLFDNp_abs = CLFDNp_abs / WDH_INPUT;
            CLFDNp = CLFDNp / WDH_INPUT;
            CLDNp = CLDNp / WDH_INPUT;

            offilep << std::setw(2) << n << ", "
                    << std::setw(4) << amplitude << ","
                    << std::setw(9) << CLp_abs << ","
                    << std::setw(11) << CLFDNp_abs << ","
                    << std::setw(11) << CLDNp_abs << ","
                    << std::setw(14) << CLFDNp << ","
                    << std::setw(13) << CLDNp << "\n";

            CLz_abs = CLz_abs / WDH_INPUT;
            CLDNz_abs = CLDNz_abs / WDH_INPUT;
            CLFDNz_abs = CLFDNz_abs / WDH_INPUT;
            CLFDNz = CLFDNz / WDH_INPUT;
            CLDNz = CLDNz / WDH_INPUT;
            CLDN_CLFDN = CLDN_CLFDN / WDH_INPUT;

            offilez << std::setw(2) << n << ","
                    << std::setw(4) << amplitude << ","
                    << std::setw(9) << CLz_abs << ","
                    << std::setw(11) << CLFDNz_abs << ","
                    << std::setw(11) << CLDNz_abs << ","
                    << std::setw(14) << CLFDNz << ","
                    << std::setw(13) << CLDNz << ","
                    << std::setw(13) << CLDN_CLFDN << "\n";
        }
        offilep.close();
        offilez.close();
    }
}

void messen_qualitySL(const int data, const int zeitreihenl, const int jahr, const int WDH_INPUT, const int n_ende)
{
    std::string algo_daten = get_DatenmodellName(data);

    GeneratorSeed seed_samen(0);

    for (int n_index = 4; n_index <= n_ende + 1; n_index += 4)
    {
        std::ofstream offilep;
        std::string pathp = "results/quality/results/puritySL_n" + to_string(n_index) + ".txt";
        offilep.open(pathp);
        offilep << "Purity Messung auf Synthethischen Daten\n"
                << "Der Mittelwert wurde ueber " << WDH_INPUT << " Wiederholungen gebildet.\n"
                << "########################################################################\n"
                << " n" << std::setw(3) << " "
                << "a" << std::setw(4) << " "
                << "[AVG SL] "
                << "[AVG SLFDN] "
                << " [AVG SLDN] "
                << "[AVG SLFDN/SL] "
                << "[AVG SLDN/SL]\n";

        std::ofstream offilez;
        std::string pathz = "results/quality/results/functionSL_n" + to_string(n_index) + ".txt";
        offilez.open(pathz);
        offilez << "Zielfunktion Messung auf Synthethischen Daten\n"
                << "Der Mittelwert wurde ueber " << WDH_INPUT << " Wiederholungen gebildet.\n"
                << "########################################################################\n"
                << " n" << std::setw(3) << " "
                << "a" << std::setw(4) << " "
                << "[AVG SL] "
                << "[AVG SLFDN] "
                << "[AVG SLDN] "
                << "[AVG SLFDN/SL] "
                << "[AVG SLDN/SL] "
                << "[AVG SLDN/SLFDN]\n";

        int MODUS;
        for (double amplitude = 0.25; amplitude <= 1.01; amplitude += 0.05)
        {
            const int n = n_index;
            const int m = n_index;
            const int k = n_index;
            const int anz_gitterpkt = n * m;
            double SLFDNp = 0;
            double SLDNp = 0;
            double SLp = 0;
            double SLFDNp_abs = 0;
            double SLDNp_abs = 0;
            double SLFDNz = 0;
            double SLDNz = 0;
            double SLz = 0;
            double SLFDNz_abs = 0;
            double SLDNz_abs = 0;
            double SLDN_SLFDN = 0;

            for (int iterINPUT = 1; iterINPUT <= WDH_INPUT; iterINPUT++)
            {
                std::cout << "EXPQualitySL Input:(" << iterINPUT << "/" << WDH_INPUT << "), amplitude:("
                          << amplitude << "/" << 1 << "), n:("
                          << n_index << "/" << n_ende << ")" << std::endl;

                double *direkte4Nachbarn;
                direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));
                Datenmodell d(anz_gitterpkt, k, zeitreihenl, jahr, "kontrollAmplitude");
                d.erstelleDaten(n, m, k, nullptr, data, amplitude, seed_samen);
                UndirectedGraph G_vollst(anz_gitterpkt);
                kompletterGraph(G_vollst, d, n * m, zeitreihenl);
                UndirectedGraph Graphdummy(0);

                ClusterAlgo SL(G_vollst, n, m, G_vollst, MODUS = 5, k, d, anz_gitterpkt, data);
                double Iter_SLp = purity(SL, d, n, m, k);
                double Iter_SLz = zielfunktionSL(SL, G_vollst, n, m, k);
                SL.clear();

                berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, zeitreihenl);
                printDirekte4Nachbarn(direkte4Nachbarn, n, m);
                UndirectedGraph G_dijkstra(anz_gitterpkt);
                erstelleGitterGraph(G_dijkstra, direkte4Nachbarn, n, m);
                ClusterAlgo SLDN(G_dijkstra, n, m, G_dijkstra, MODUS = 1, k, d, anz_gitterpkt, data);
                double Iter_SLDNp = purity(SLDN, d, n, m, k);
                double Iter_SLDNz = zielfunktionSL(SLDN, G_vollst, n, m, k);
                SLDN.clear();

                ClusterAlgo SLFDN(G_vollst, n, m, G_dijkstra, MODUS = 2, k, d, anz_gitterpkt, data);
                double Iter_SLFDNp = purity(SLFDN, d, n, m, k);
                double Iter_SLFDNz = zielfunktionSL(SLFDN, G_vollst, n, m, k);
                SLFDN.clear();

                SLDN_SLFDN += Iter_SLDNz / Iter_SLFDNz;

                SLDNp += Iter_SLDNp / Iter_SLp;
                SLFDNp += Iter_SLFDNp / Iter_SLp;
                SLp += Iter_SLp;
                SLDNp_abs += Iter_SLDNp;
                SLFDNp_abs += Iter_SLFDNp;

                SLDNz += Iter_SLDNz / Iter_SLz;
                SLFDNz += Iter_SLFDNz / Iter_SLz;
                SLz += Iter_SLz;
                SLFDNz_abs += Iter_SLFDNz;
                SLDNz_abs += Iter_SLDNz;

                Graphdummy.clear();
                G_dijkstra.clear();
                G_vollst.clear();

                free(direkte4Nachbarn);
                d.clear();
            }
            SLp = SLp / WDH_INPUT;
            SLFDNp = SLFDNp / WDH_INPUT;
            SLDNp = SLDNp / WDH_INPUT;
            SLDNp_abs = SLDNp_abs / WDH_INPUT;
            SLFDNp_abs = SLFDNp_abs / WDH_INPUT;

            offilep << std::setw(2) << n << ","
                    << std::setw(4) << amplitude << ","
                    << std::setw(9) << SLp << ","
                    << std::setw(11) << SLFDNp_abs << ","
                    << std::setw(11) << SLDNp_abs << ","
                    << std::setw(14) << SLFDNp << ","
                    << std::setw(13) << SLDNp << "\n";

            SLz = SLz / WDH_INPUT;
            SLFDNz = SLFDNz / WDH_INPUT;
            SLDNz = SLDNz / WDH_INPUT;
            SLFDNz_abs = SLFDNz_abs / WDH_INPUT;
            SLDNz_abs = SLDNz_abs / WDH_INPUT;
            SLDN_SLFDN = SLDN_SLFDN / WDH_INPUT;

            offilez << std::setw(2) << n << ","
                    << std::setw(4) << amplitude << ","
                    << std::setw(9) << SLz << ","
                    << std::setw(11) << SLFDNz_abs << ","
                    << std::setw(11) << SLDNz_abs << ","
                    << std::setw(14) << SLFDNz << ","
                    << std::setw(13) << SLDNz << ","
                    << std::setw(13) << SLDN_SLFDN << "\n";
        }
        offilep.close();
        offilez.close();
    }
}

void main_QualityPurity(const bool seed)
{
    //BEGINN GESAMT LAUFZEIT DES EXPERIEMENNTS
    struct timeval begin_exp, end_exp;
    long seconds_exp;
    long microseconds_exp;
    double time_exp;
    gettimeofday(&begin_exp, 0);

    std::ofstream offile;
    std::string path = "results/quality/results/INFO.txt";
    offile.open(path);

    SEED = seed;
    DEBUGG_algo = false;
    DEBUGG_optional = false;
    CHECK = false;
    int data, zeitreihenl, n_ende;
    int WDH_INPUT = 20;

    messen_qualityCL(data = 4, zeitreihenl = 15, (int)NULL, WDH_INPUT, n_ende = 20);
    messen_qualitySL(data = 4, zeitreihenl = 15, (int)NULL, WDH_INPUT, n_ende = 20);

    // ENDE ZEITMESSUNG
    gettimeofday(&end_exp, 0);
    seconds_exp = end_exp.tv_sec - begin_exp.tv_sec;
    microseconds_exp = end_exp.tv_usec - begin_exp.tv_usec;
    time_exp = seconds_exp + microseconds_exp * 1e-6;

    offile << "####################################\n"
           << "Gesamt Laufzeit Experiment Quality: " << get_stringTime(time_exp) << "\n";

    time_t aktuelleZeit;
    struct tm lokal;
    aktuelleZeit = time(NULL);
    lokal = *localtime(&aktuelleZeit);
    offile << "Durchgefuehrt: " << currentDateTime() << "\n";
    offile.close();
}

/**
 ***********************************************************************************
 ***********************************************************************************
 KORREKTHEIT DES ALGORITHMUS PRUEFEN
 ***********************************************************************************
 ***********************************************************************************
 */
void experiment_kor2(const int n, const int m, const int k, const int data, const int zeitreihenl, GeneratorSeed &seed_samen)
{
    int MODUS;

    const int anz_gitterpkt = n * m;

    Datenmodell d(anz_gitterpkt, k, zeitreihenl, (int)NULL, "SINbekanntPurityKW");
    d.erstelleDaten(n, m, k, nullptr, data, (double)NULL, seed_samen);

    UndirectedGraph G_Gitter(anz_gitterpkt);
    initialerGitterGraph(G_Gitter, d, n, m, zeitreihenl);
    printUngerichteterGraph(G_Gitter, n, m, "G_Gitter:");

    /**
     * MODUS = 3 -> Complete Linkage Graph 
     */
    ClusterAlgo CLDN(G_Gitter, n, m, G_Gitter, MODUS = 3, k, d, anz_gitterpkt, data);
    if (!KORREKT)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt.";
        exit(1);
    }

    //Speicher freigeben
    d.clear();
    G_Gitter.clear();
}

void experiment_kor1(const int n, const int m, const int k, const int data, const int zeitreihenl, GeneratorSeed &seed_samen)
{
    int MODUS;

    const int anz_gitterpkt = n * m;

    double *direkte4Nachbarn;
    direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));

    Datenmodell d(anz_gitterpkt, k, zeitreihenl, (int)NULL, "SINbekanntPurity");
    d.erstelleDaten(n, m, k, nullptr, data, (double)NULL, seed_samen);

    berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, zeitreihenl);
    printDirekte4Nachbarn(direkte4Nachbarn, n, m);

    UndirectedGraph G_Gitter(anz_gitterpkt);
    erstelleGitterGraph(G_Gitter, direkte4Nachbarn, n, m);
    printUngerichteterGraph(G_Gitter, n, m, "direkte4Nachbarn:");

    UndirectedGraph G_vollst(anz_gitterpkt);
    kompletterGraph(G_vollst, d, anz_gitterpkt, zeitreihenl);
    printUngerichteterGraph(G_vollst, n, m, "vollstaendigerGraph:");

    UndirectedGraph Graphdummy(0);

    /**
     * MODUS = 1 -> Single Linkage Graph
     * MODUS = 2 -> Single Linkage Frechet
     * MODUS = 4 -> Complete Linkage Frechet
     * MODUS = 5 -> Single Linkage Klasssisch
     * MODUS = 3 -> Complete Linkage Graph -> experiment_kor2
     * MODUS = 6 -> Complete Linkage Klasssisch
     */

    ClusterAlgo CL(Graphdummy, n, m, G_vollst, MODUS = 6, k, d, anz_gitterpkt, data);
    if (!KORREKT)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        exit(1);
    }
    double CL_zielfunktion = zielfunktionCL(CL, G_vollst, n, m, k);
    if (CHECK && CL_zielfunktion != linkage_zielfunktionHeap)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        std::cout << "Zielfunktionen passen nicht. Aus Heap" << linkage_zielfunktionHeap << " Aus Methoede:" << CL_zielfunktion << std::endl;
        exit(1);
    }
    CL.clear();

    ClusterAlgo CLFDN(G_vollst, n, m, G_Gitter, MODUS = 4, k, d, anz_gitterpkt, data);
    int CLFDN_frechet_counter = counterFrechet;
    if (!KORREKT)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        exit(1);
    }
    double CLFDN_zielfunktion = zielfunktionCL(CLFDN, G_vollst, n, m, k);
    if (CHECK && CLFDN_zielfunktion != linkage_zielfunktionHeap)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        std::cout << "Zielfunktionen passen nicht. Aus Heap" << linkage_zielfunktionHeap << " Aus Methoede:" << CLFDN_zielfunktion << std::endl;
        exit(1);
    }
    CLFDN.clear();

    ClusterAlgo SL(G_vollst, n, m, Graphdummy, MODUS = 5, k, d, anz_gitterpkt, data);
    if (!KORREKT)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        exit(1);
    }
    SL.clear();

    ClusterAlgo SLDN(G_Gitter, n, m, Graphdummy, MODUS = 1, k, d, anz_gitterpkt, data);
    if (!KORREKT)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        exit(1);
    }
    SLDN.clear();

    ClusterAlgo SLFDN(G_vollst, n, m, G_Gitter, MODUS = 2, k, d, anz_gitterpkt, data);
    int SLFDN_frechet_counter = counterFrechet;
    if (!KORREKT)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        exit(1);
    }
    double SLFDN_zielfunktion = zielfunktionSL(SLFDN, G_vollst, n, m, k);
    if (CHECK && SLFDN_zielfunktion > (((n * m) - 1) - (k - 1)) * linkage_zielfunktionHeap)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "Nicht korrekt: ";
        std::cout << "MODUS" << MODUS << " n:" << n << " m:" << m << " k:" << k << std::endl;
        std::cout << "Zielfunktionen passen nicht. Aus Heap" << linkage_zielfunktionHeap << " Aus Methoede:" << SLFDN_zielfunktion << std::endl;
        exit(1);
    }
    SLFDN.clear();

    if (CLFDN_frechet_counter > SLFDN_frechet_counter)
    {
        std::cerr << RED << "ERROR:" << RESET;
        std::cout << "CLFDN_frechet_counter>SLFDN_frechet_counter" << CLFDN_frechet_counter << " > " << SLFDN_frechet_counter << std::endl;
        exit(1);
    }

    Graphdummy.clear();
    d.clear();
    free(direkte4Nachbarn);
}

void main_Korrektheit(const bool seed, const int dim)
{
    //BEGINN GESAMT LAUFZEIT DES EXPERIEMENNTS
    struct timeval begin_exp, end_exp;
    long seconds_exp;
    long microseconds_exp;
    double time_exp;
    gettimeofday(&begin_exp, 0);

    std::ofstream offile;
    std::string path = "results/quality/results/INFO_korrektheit.txt";
    offile.open(path);

    DEBUGG_algo = false;
    DEBUGG_optional = false;
    SEED = seed;
    CHECK = true;
    int data;
    GeneratorSeed seed_samen(0);
    for (int i = 4; i <= dim; i++)
    {
        for (int j = 4; j <= dim; j++)
        {
            for (int l = 2; l < i * j - 1; l++)
            {
                const int n = i;
                const int m = j;
                const int k = l;
                const int zeitreihenl = 5;
                /**
                 * Hier funktionieren auch die anderen Methoden das Datenmodells.
                 */
                experiment_kor1(n, m, k, data = 4, zeitreihenl, seed_samen);
                experiment_kor2(n, m, k, data = 4, zeitreihenl, seed_samen);
                experiment_kor1(n, m, k, data = 1, zeitreihenl, seed_samen);
                experiment_kor2(n, m, k, data = 1, zeitreihenl, seed_samen);
            }
        }
    }
    CHECK = false;

    // ENDE ZEITMESSUNG
    gettimeofday(&end_exp, 0);
    seconds_exp = end_exp.tv_sec - begin_exp.tv_sec;
    microseconds_exp = end_exp.tv_usec - begin_exp.tv_usec;
    time_exp = seconds_exp + microseconds_exp * 1e-6;

    offile << "####################################\n"
           << "Gesamt Laufzeit Experiment Korrektheit: " << get_stringTime(time_exp) << "\n";

    time_t aktuelleZeit;
    struct tm lokal;
    aktuelleZeit = time(NULL);
    lokal = *localtime(&aktuelleZeit);
    offile << "Durchgefuehrt: " << currentDateTime() << "\n";
    offile.close();
}
