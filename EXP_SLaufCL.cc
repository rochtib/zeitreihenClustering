/**
 ***********************************************************************************
 ***********************************************************************************
 ALLE ALGORITHMEN AUF CL auswerten 
 ***********************************************************************************
 ***********************************************************************************
 */
extern bool DEBUGG_algo;
extern bool DEBUGG_optional;
extern bool SEED;

void origDatenAufCL(const int data, const int N_punkte, const int k_input, const int zeitreihenl, const int jahr, const int WDH_PERM)
{
    const int n_ANZAHL_MESSSTATIONEN = 2360;
    const int k = k_input;
    const int Auswahl = N_punkte;
    int MODUS;

    std::unordered_map<int, std::string> map;

    int *NachbarStationen;
    NachbarStationen = (int *)malloc(Auswahl * 6 * sizeof(int *));
    float *Stationen;
    Stationen = (float *)malloc(n_ANZAHL_MESSSTATIONEN * 3 * sizeof(float *));

    get_originalDaten(map, NachbarStationen, Stationen, data, N_punkte, zeitreihenl,
                      jahr, DEBUGG_algo, DEBUGG_optional);
    free(Stationen);

    GeneratorSeed seed_samen(0);

    Datenmodell d(Auswahl, k, zeitreihenl, (int)NULL, "");
    d.erstelleDaten(Auswahl, 1, k, NachbarStationen, data, jahr, seed_samen);

    UndirectedGraph G_dijkstra(Auswahl + 1);
    initialerAbstandsClusterGraphGitter(G_dijkstra, d, NachbarStationen, Auswahl, zeitreihenl);
    printUngerichteterGraph(G_dijkstra, Auswahl, 1, "GitterGraph auf RealWorldDaten");

    UndirectedGraph G_vollst(Auswahl + 1);
    erstelleCompleteGraphVonGraph(G_vollst, G_dijkstra, d, Auswahl, zeitreihenl);
    printUngerichteterGraph(G_vollst, Auswahl, 1, "vollstaendiger Graph auf RealWorldDaten");

    /*****************************************************************
     * Experiment Lokalitaet der RealWorldDaten  
     *****************************************************************/

    std::string algo_daten = get_DatenmodellName(data);

    std::ofstream offile;
    std::string path = "results/SLaufCL/results/" + algo_daten + "_N=" + to_string(Auswahl) + "_ZR" + to_string(zeitreihenl) + "TEST.txt";
    offile.open(path);
    offile << "Anzahl Punkte: " << Auswahl << " Clustering: " << k << "\n"
           << "1. Zeile RealWorldDaten Daten\n"
           << "2. permutierte Daten:"
           << " Mittelwerte ueber " << WDH_PERM << " Wiederholungen gebildet.\n"
           << "####################################\n"
           << "####################################\n"
           << "  n" << std::setw(10) << get_AlgorithmusName(5) << " "
           << std::setw(10) << get_AlgorithmusName(2) << " "
           << std::setw(10) << "[SLFDN/CL]"
           << " "
           << std::setw(10) << get_AlgorithmusName(1) << " "
           << std::setw(10) << "[SLDN/CL]"
           << " "
           << std::setw(10) << get_AlgorithmusName(6) << " "
           << std::setw(10) << get_AlgorithmusName(4) << " "
           << std::setw(10) << "[CLFDN/CL]"
           << " "
           << std::setw(10) << get_AlgorithmusName(3) << " "
           << std::setw(10) << "[CLDN/CL]"
           << "\n";
    UndirectedGraph Graphdummy(0);

    ClusterAlgo SL(G_vollst, Auswahl, 1, Graphdummy, MODUS = 5, k, d, Auswahl, data);
    double klassisch_SL = zielfunktionCL(SL, G_vollst, Auswahl, 1, k);

    ClusterAlgo SLFDN(G_vollst, Auswahl, 1, G_dijkstra, MODUS = 2, k, d, Auswahl, data);
    double klassisch_SLFDN = zielfunktionCL(SLFDN, G_vollst, Auswahl, 1, k);

    ClusterAlgo SLDN(G_dijkstra, Auswahl, 1, Graphdummy, MODUS = 1, k, d, Auswahl, data);
    double klassisch_SLDN = zielfunktionCL(SLDN, G_vollst, Auswahl, 1, k);

    ClusterAlgo CL(Graphdummy, Auswahl, 1, G_vollst, MODUS = 6, k, d, Auswahl, data);
    double klassisch_CL = zielfunktionCL(CL, G_vollst, Auswahl, 1, k);

    ClusterAlgo CLFDN(G_vollst, Auswahl, 1, G_dijkstra, MODUS = 4, k, d, Auswahl, data);
    double klassisch_CLFDN = zielfunktionCL(CLFDN, G_vollst, Auswahl, 1, k);

    ClusterAlgo CLDN(G_dijkstra, Auswahl, 1, G_dijkstra, MODUS = 3, k, d, Auswahl, data);
    double klassisch_CLDN = zielfunktionCL(CLDN, G_vollst, Auswahl, 1, k);

    offile << std::setw(3) << Auswahl << "," << std::setw(10) << klassisch_SL << ",";
    offile << std::setw(10) << klassisch_SLFDN << "," << std::setw(10) << klassisch_SLFDN / klassisch_CL << ",";
    offile << std::setw(10) << klassisch_SLDN << "," << std::setw(10) << klassisch_SLDN / klassisch_CL << ",";
    offile << std::setw(10) << klassisch_CL << ",";
    offile << std::setw(10) << klassisch_CLFDN << "," << std::setw(10) << klassisch_CLFDN / klassisch_CL << ",";
    offile << std::setw(10) << klassisch_CLDN << "," << std::setw(10) << klassisch_CLDN / klassisch_CL << "\n";

    SL.clear();
    SLDN.clear();
    SLFDN.clear();
    CL.clear();
    CLDN.clear();
    CLFDN.clear();
    G_dijkstra.clear();

    /**
     * Experiment OHNE Lokalitaet permutierte Zeitreihe
     */
    double klassisch_SL_perm = 0;
    double klassisch_SLDN_perm = 0;
    double klassisch_SLFDN_perm = 0;
    double klassisch_CL_perm = 0;
    double klassisch_CLDN_perm = 0;
    double klassisch_CLFDN_perm = 0;

    double SLDN_perm = 0;
    double SLFDN_perm = 0;
    double CLDN_perm = 0;
    double CLFDN_perm = 0;

    for (int iterPerm = 1; iterPerm <= WDH_PERM; iterPerm++)
    {
        std::cout << "EXP RealWorldDaten  Permutation: (" << iterPerm << "/" << WDH_PERM << ")" << std::endl;
        permutiereZeitreihe(d, zeitreihenl, Auswahl, 1, seed_samen);

        UndirectedGraph G_vollst1(Auswahl + 1);
        kompletterGraph(G_vollst1, d, Auswahl, zeitreihenl);

        ClusterAlgo SLperm(G_vollst1, Auswahl, 1, Graphdummy, MODUS = 5, k, d, Auswahl, data);
        double value_SL_perm = zielfunktionCL(SLperm, G_vollst1, Auswahl, 1, k);

        ClusterAlgo CLperm(Graphdummy, Auswahl, 1, G_vollst1, MODUS = 6, k, d, Auswahl, data);
        double value_CL_perm = zielfunktionCL(CLperm, G_vollst1, Auswahl, 1, k);

        UndirectedGraph G_dijkstra1(Auswahl + 1);
        initialerAbstandsClusterGraphGitter(G_dijkstra1, d, NachbarStationen, Auswahl, zeitreihenl);

        ClusterAlgo SLDNperm(G_dijkstra1, Auswahl, 1, Graphdummy, MODUS = 1, k, d, Auswahl, data);
        double value_SLDN_perm = zielfunktionCL(SLDNperm, G_vollst1, Auswahl, 1, k);
        SLDN_perm += value_SLDN_perm;

        ClusterAlgo SLFDNperm(G_vollst1, Auswahl, 1, G_dijkstra1, MODUS = 2, k, d, Auswahl, data);
        double value_SLFDN_perm = zielfunktionCL(SLFDNperm, G_vollst1, Auswahl, 1, k);
        SLFDN_perm += value_SLFDN_perm;

        ClusterAlgo CLDNperm(G_dijkstra1, Auswahl, 1, G_dijkstra1, MODUS = 3, k, d, Auswahl, data);
        double value_CLDN_perm = zielfunktionCL(CLDNperm, G_vollst1, Auswahl, 1, k);
        CLDN_perm += value_CLDN_perm;

        ClusterAlgo CLFDNperm(G_vollst1, Auswahl, 1, G_dijkstra1, MODUS = 4, k, d, Auswahl, data);
        double value_CLFDN_perm = zielfunktionCL(CLFDNperm, G_vollst1, Auswahl, 1, k);
        CLFDN_perm += value_CLFDN_perm;

        klassisch_SL_perm += value_SL_perm;
        klassisch_SLDN_perm += value_SLDN_perm / value_CL_perm;
        klassisch_SLFDN_perm += value_SLFDN_perm / value_CL_perm;
        klassisch_CL_perm += value_CL_perm;
        klassisch_CLDN_perm += value_CLDN_perm / value_CL_perm;
        klassisch_CLFDN_perm += value_CLFDN_perm / value_CL_perm;

        G_vollst1.clear();
        G_dijkstra1.clear();
        SLDNperm.clear();
        SLFDNperm.clear();
        CLDNperm.clear();
        CLFDNperm.clear();
        CLperm.clear();
        SLperm.clear();
    }
    Graphdummy.clear();
    CLDN_perm = CLDN_perm / WDH_PERM;
    CLFDN_perm = CLFDN_perm / WDH_PERM;
    SLDN_perm = SLDN_perm / WDH_PERM;
    SLFDN_perm = SLFDN_perm / WDH_PERM;
    klassisch_SL_perm = klassisch_SL_perm / WDH_PERM;
    klassisch_SLDN_perm = klassisch_SLDN_perm / WDH_PERM;
    klassisch_SLFDN_perm = klassisch_SLFDN_perm / WDH_PERM;
    klassisch_CL_perm = klassisch_CL_perm / WDH_PERM;
    klassisch_CLDN_perm = klassisch_CLDN_perm / WDH_PERM;
    klassisch_CLFDN_perm = klassisch_CLFDN_perm / WDH_PERM;

    offile << std::setw(3) << Auswahl << ","
           << std::setw(10) << klassisch_SL_perm << ","
           << std::setw(10) << SLFDN_perm << ","
           << std::setw(10) << klassisch_SLFDN_perm << ","
           << std::setw(10) << SLDN_perm << ","
           << std::setw(10) << klassisch_SLDN_perm << ","
           << std::setw(10) << klassisch_CL_perm << ","
           << std::setw(10) << CLFDN_perm << ","
           << std::setw(10) << klassisch_CLFDN_perm << ","
           << std::setw(10) << CLDN_perm << ","
           << std::setw(10) << klassisch_CLDN_perm << "\n";

    offile.close();

    free(NachbarStationen);
    d.clear();
}

void main_SLaufCL(const bool seed)
{
    //BEGINN GESAMT LAUFZEIT DES EXPERIEMENNTS
    struct timeval begin_exp, end_exp;
    long seconds_exp;
    long microseconds_exp;
    double time_exp;
    gettimeofday(&begin_exp, 0);

    std::ofstream offile;
    std::string path = "results/SLaufCL/results/INFO.txt";
    offile.open(path);

    SEED = seed;
    DEBUGG_algo = false;
    DEBUGG_optional = false;
    CHECK = false;

    int data, zr, N, k;
    std::string data_type;
    int WDH_PERM = 10;
    int jahr = 1990;

    origDatenAufCL(data = 8, N = 50, k = 5, zr = 100, jahr, WDH_PERM);
    origDatenAufCL(data = 8, N = 100, k = 10, zr = 100, jahr, WDH_PERM);
    origDatenAufCL(data = 8, N = 200, k = 10, zr = 100, jahr, WDH_PERM);
    origDatenAufCL(data = 8, N = 300, k = 20, zr = 100, jahr, WDH_PERM);
    origDatenAufCL(data = 8, N = 400, k = 20, zr = 100, jahr, WDH_PERM);
    origDatenAufCL(data = 8, N = 500, k = 20, zr = 100, jahr, WDH_PERM);

    // ENDE ZEITMESSUNG
    gettimeofday(&end_exp, 0);
    seconds_exp = end_exp.tv_sec - begin_exp.tv_sec;
    microseconds_exp = end_exp.tv_usec - begin_exp.tv_usec;
    time_exp = seconds_exp + microseconds_exp * 1e-6;

    offile << "####################################\n"
           << "Gesamt Laufzeit Experiment SLaufCL: " << get_stringTime(time_exp) << "\n";

    time_t aktuelleZeit;
    struct tm lokal;
    aktuelleZeit = time(NULL);
    lokal = *localtime(&aktuelleZeit);
    offile << "Durchgefuehrt: " << currentDateTime() << "\n";
    offile.close();
}