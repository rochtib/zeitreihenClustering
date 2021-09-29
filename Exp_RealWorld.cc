/**
 ***********************************************************************************
 ***********************************************************************************
 REAL WORLD DATEN PSMSL
 ***********************************************************************************
 ***********************************************************************************
 */

extern bool DEBUGG_algo;
extern bool DEBUGG_optional;

/**
 * @brief Methode die ein Cluster in eine Text Datei schreibt.
 */
void write_clusteringDaten(ClusterAlgo CLFR, const int n, float *Stationen, std::unordered_map<int, std::string> map, const int k, const std::string type)
{
    int *work;
    int m = 1;
    work = (int *)malloc((n + 1) * sizeof(int *));

    for (int i = 1; i <= n; i++)
    {
        work[i] = CLFR.get_clusterBerechnetFeld(i);
    }
    renummeriereClusterArray(work, n, m, k);

    std::ofstream offile;
    std::string path = "results/Clustering/Clustering_" + type + ".txt";

    offile.open(path);

    for (int i = 0; i < n * 3; i += 3)
    {
        std::string stationname = map[i / 3 + 1];
        stationname.erase(std::remove(stationname.begin(), stationname.end(), ' '), stationname.end());
        offile << Stationen[i + 1] << " " << Stationen[i + 2] << " " << work[i / 3 + 1]
               << " " << stationname << "\n";
    }
    offile.close();
    free(work);
}

/**
 * @brief Zeitreihen werden rein zufaellig permutiert.
 */
void permutiereZeitreihe(Datenmodell d, int zeitreihen_l, int n, int m, GeneratorSeed &seed_samen)
{
    double *zeitreihenOLD;
    zeitreihenOLD = (double *)malloc((n * m) * zeitreihen_l * sizeof(double *));

    for (int i = 0; i < n * m; i++)
    {
        for (int j = 0; j < zeitreihen_l; j++)
        {
            zeitreihenOLD[i * zeitreihen_l + j] = d.get_Zeitreihenwert(i * zeitreihen_l + j);
        }
    }
    d.print_Zeitreihe_help(zeitreihen_l, n, m, "Zeitreihen nicht permutiert:");
    std::vector<int> permutationen;

    auto rd = seed_samen.get_samen();
    std::default_random_engine rng(rd());

    for (int i = 0; i < n * m; i++)
    {
        permutationen.push_back(i);
    }

    std::shuffle(std::begin(permutationen), std::end(permutationen), rng);

    for (int i = 0; i < n * m; i++)
    {
        int index = permutationen.at(i);
        for (int j = 0; j < zeitreihen_l; j++)
        {
            d.set_Zeitreihenwert(i * zeitreihen_l + j, zeitreihenOLD[index * zeitreihen_l + j]);
        }
    }
    d.print_Zeitreihe_help(zeitreihen_l, n, m, "permutierte Zeitreihen:");
    free(zeitreihenOLD);
}

/**
 * @brief Zeitreihen aus 
 */
void permutiereEineZeitreiheNB(Datenmodell &d, int zeitreihen_l, int n, int m, int k, GeneratorSeed &seed_samen)
{
    double *zeitreihenOLD;
    zeitreihenOLD = (double *)malloc((n * m) * zeitreihen_l * sizeof(double *));

    for (int i = 0; i < n * m; i++)
    {
        for (int j = 0; j < zeitreihen_l; j++)
        {
            zeitreihenOLD[i * zeitreihen_l + j] = d.get_Zeitreihenwert(i * zeitreihen_l + j);
        }
    }
    d.print_Zeitreihe_help(zeitreihen_l, n, m, "Zeitreihen nicht permutiert:");
    std::vector<int> permutationen;

    auto rd = seed_samen.get_samen();
    std::default_random_engine rng(rd());

    for (int i = 0; i < n * m; i++)
    {
        permutationen.push_back(i);
    }

    std::shuffle(std::begin(permutationen), std::end(permutationen), rng);

    // int index = seed_samen.random_int() % (n * m);
    int cluster_index = (seed_samen.random_int() % k) + 1;
    // std::cout << "cluster_index" << cluster_index << std::endl;
    std::set<int> cluster;
    for (int p = 1; p <= n * m; p++)
    {
        if (d.get_musterunion(p) == cluster_index)
        {
            cluster.insert(p);
        }
    }
    // DEBUGG_algo = true;
    // d.print_DatenModellCluster(n, m);
    // DEBUGG_algo = false;
    std::set<int> nachbarCluster;
    for (auto it = cluster.begin(); it != cluster.end(); ++it)
    {
        for (int i = 1; i <= n * m; i++)
        {
            if (*it + 1 == i && i % n != 0)
            {
                nachbarCluster.insert(d.get_musterunion(i));
            }
            else if (*it - 1 == i && i % n != 1)
            {
                nachbarCluster.insert(d.get_musterunion(i));
            }
            else if (*it + n == i && *it + n <= n * m)
            {
                nachbarCluster.insert(d.get_musterunion(i));
            }
            else if (*it - n == i)
            {
                nachbarCluster.insert(d.get_musterunion(i));
            }
        }
    }
    std::cout << "nachbarCluster.size()" << nachbarCluster.size() << std::endl;
    auto ittt = nachbarCluster.begin();
    for (int i = 0; i < (int)(seed_samen.random_int() % nachbarCluster.size()); i++)
    {
        ittt++;
    }
    int clusterTausch = *ittt;

    std::set<int> set_clusterTausch;
    for (int p = 1; p <= n * m; p++)
    {
        if (d.get_musterunion(p) == clusterTausch)
        {
            set_clusterTausch.insert(p);
        }
    }

    auto it_index1 = cluster.begin();
    for (int i = 0; i < (int)(seed_samen.random_int() % cluster.size()); i++)
    {
        it_index1++;
    }
    int cluster_index1 = *it_index1 - 1;

    auto it_index2 = set_clusterTausch.begin();
    for (int i = 0; i < (int)(seed_samen.random_int() % set_clusterTausch.size()); i++)
    {
        it_index2++;
    }
    int cluster_index2 = *it_index2 - 1;

    std::cout << "Tauschen " << cluster_index1 << " mit " << cluster_index2 << std::endl;
    for (int j = 0; j < zeitreihen_l; j++)
    {
        d.set_Zeitreihenwert((cluster_index1)*zeitreihen_l + j, zeitreihenOLD[cluster_index2 * zeitreihen_l + j]);
    }

    for (int j = 0; j < zeitreihen_l; j++)
    {
        d.set_Zeitreihenwert((cluster_index2)*zeitreihen_l + j, zeitreihenOLD[cluster_index1 * zeitreihen_l + j]);
    }

    d.print_Zeitreihe_help(zeitreihen_l, n, m, "permutierte Zeitreihen:");
    free(zeitreihenOLD);
    set_clusterTausch.clear();
    cluster.clear();
    nachbarCluster.clear();
    permutationen.clear();
}

void permutiereEineZeitreihe(Datenmodell &d, int zeitreihen_l, int n, int m, GeneratorSeed &seed_samen)
{
    double *zeitreihenOLD;
    zeitreihenOLD = (double *)malloc((n * m) * zeitreihen_l * sizeof(double *));

    for (int i = 0; i < n * m; i++)
    {
        for (int j = 0; j < zeitreihen_l; j++)
        {
            zeitreihenOLD[i * zeitreihen_l + j] = d.get_Zeitreihenwert(i * zeitreihen_l + j);
        }
    }
    d.print_Zeitreihe_help(zeitreihen_l, n, m, "Zeitreihen nicht permutiert:");
    std::vector<int> permutationen;

    auto rd = seed_samen.get_samen();
    std::default_random_engine rng(rd());

    for (int i = 0; i < n * m; i++)
    {
        permutationen.push_back(i);
    }

    std::shuffle(std::begin(permutationen), std::end(permutationen), rng);

    int index1 = seed_samen.random_int() % (n * m);
    int index2 = permutationen.at(index1);
    // std::cout << "cluster_index" << cluster_index << std::endl;
    // DEBUGG_algo = true;
    // d.print_DatenModellCluster(n, m);
    // DEBUGG_algo = false;

    std::cout << "Tauschen " << index1 << " mit " << index2 << std::endl;
    for (int j = 0; j < zeitreihen_l; j++)
    {
        d.set_Zeitreihenwert((index1)*zeitreihen_l + j, zeitreihenOLD[index2 * zeitreihen_l + j]);
    }

    for (int j = 0; j < zeitreihen_l; j++)
    {
        d.set_Zeitreihenwert((index2)*zeitreihen_l + j, zeitreihenOLD[index1 * zeitreihen_l + j]);
    }

    d.print_Zeitreihe_help(zeitreihen_l, n, m, "permutierte Zeitreihen:");
    free(zeitreihenOLD);
    permutationen.clear();
}

void get_originalDaten(std::unordered_map<int, std::string> &map, int *NachbarStationen, float *Stationen,
                       const int data, const int N_punkte, const int zeitreihenl,
                       const int Jahr, const bool algo, const bool optional)
{
    DEBUGG_algo = algo;
    DEBUGG_optional = optional;

    const int n_ANZAHL_MESSSTATIONEN = 2360;
    const int Auswahl = N_punkte;

    std::string data_type = get_DatenmodellName(data);
    std::cout << "data" << data << " " << data_type << std::endl;

    std::map<int, std::string> map_dummy;
    std::string line;
    std::string eintrag;
    std::string filename = "srcPSMSL/" + data_type + "/filelist.txt";

    check_datei(filename);
    std::ifstream input(filename);
    const std::string delim = ";";

    std::set<int> set_STATIONEN;
    int anz = 0;
    while (std::getline(input, line) && anz <= n_ANZAHL_MESSSTATIONEN * 3)
    {
        // std::cout<<line<<std::endl;
        int id;
        float x_koord;
        float y_koord;
        int count = 0;
        size_t pos = 0;
        while ((pos = line.find(delim)) != std::string::npos)
        {
            eintrag = line.substr(0, pos);
            line.erase(0, pos + delim.length());
            if (count == 0)
            {
                id = std::stod(eintrag);
                // std::cout << "id:" << setw(4) << id;
            }
            else if (count == 1)
            {
                x_koord = std::stod(eintrag);
                // std::cout << " x_koord:" << x_koord;
            }
            else if (count == 2)
            {
                y_koord = std::stod(eintrag);
                // std::cout << " y_koord:" << y_koord << std::endl;
                filename = "srcPSMSL/" + data_type + "/data/" + to_string(id) + ".rlrdata";

                if (einlesen_ExterneZeitreihen(id, data_type, Jahr, zeitreihenl))
                {
                    Stationen[anz] = id;
                    anz++;
                    Stationen[anz] = x_koord;
                    anz++;
                    Stationen[anz] = y_koord;
                    anz++;
                }
                else
                {
                    break;
                }
            }
            else if (count == 3)
            {
                map_dummy[id] = eintrag;
                // std::cout<<"eintrag"<<eintrag<<std::endl;
                break;
            }
            count++;
        }
    }

    // for (const auto &n : map_dummy)
    // {
    //     std::cout << "Station:[" << n.first << "] Name:[" << n.second << "]\n";
    // }

    // for (int i = 1; i <= Auswahl+2; i++)
    // {
    //     std::cout << "Station:[" << i << "] Name:[" << map[i] << "]\n";
    // }

    int index_auswahl = 0;
    while ((int)set_STATIONEN.size() != Auswahl)
    {
        set_STATIONEN.insert(Stationen[index_auswahl]);
        index_auswahl += 3;
    }

    if (true)
    {
        std::cout << "Stationen" << std::endl;
        for (int i = 0; i < Auswahl * 3; i += 3)
        {
            std::cout << "id:" << setw(4) << Stationen[i];
            std::cout << " x_koord:" << Stationen[i + 1];
            std::cout << " y_koord:" << Stationen[i + 2] << std::endl;
        }
    }
    int nummerid = 1;
    for (int i = 0; i < Auswahl * 3; i += 3)
    {
        float xbasis = Stationen[i + 1];
        float ybasis = Stationen[i + 2];
        NachbarStationen[i * 6 / 3] = Stationen[i];
        NachbarStationen[(i * 6 / 3) + 1] = nummerid;
        nummerid++;
        int idtemp;
        std::set<int> setNB;
        while (setNB.size() != 4)
        {
            float abstand = INT_MAX;
            for (int j = 0; j < n_ANZAHL_MESSSTATIONEN * 3; j += 3)
            {
                if (set_STATIONEN.contains(Stationen[j]))
                {
                    if (!setNB.contains(Stationen[j]) && i != j && Stationen[j + 1] != 0 && Stationen[j + 2] != 0)
                    {
                        if (std::sqrt(std::pow(xbasis - Stationen[j + 1], 2) + std::pow(ybasis - Stationen[j + 2], 2)) < abstand)
                        {
                            idtemp = Stationen[j];
                            abstand = std::sqrt(std::pow(xbasis - Stationen[j + 1], 2) + std::pow(ybasis - Stationen[i + 2], 2));
                        }
                    }
                }
            }
            setNB.insert(idtemp);
            NachbarStationen[i * 6 / 3 + setNB.size() + 1] = idtemp;
        }
    }
    set_STATIONEN.clear();

    //Renummerierug
    for (int i = 0; i < Auswahl * 6; i += 6)
    {
        if (NachbarStationen[i] != NachbarStationen[i + 1])
        {
            for (int j = 0; j < Auswahl * 6; j += 6)
            {
                for (int t = 2; t <= 5; t++)
                {
                    if (NachbarStationen[j + t] == NachbarStationen[i])
                    {
                        NachbarStationen[j + t] = NachbarStationen[i + 1];
                    }
                }
            }
            map[NachbarStationen[i + 1]] = map_dummy[NachbarStationen[i]];
        }
        else
        {
            map[NachbarStationen[i]] = map_dummy[NachbarStationen[i]];
        }
    }
    map_dummy.clear();
    if (true)
    {
        for (int i = 1; i <= Auswahl; i++)
        {
            std::cout << "Station:[" << i << "] Name:[" << map[i] << "]\n";
        }

        std::cout << "NachbarStationen" << std::endl;
        int index = 0;
        while (index < Auswahl * 6)
        {
            std::cout << "alt[" << setw(2) << NachbarStationen[index] << "] neu ";
            index++;
            std::cout << setw(2) << NachbarStationen[index] << ": ";
            index++;
            std::cout << NachbarStationen[index] << " ";
            index++;
            std::cout << NachbarStationen[index] << " ";
            index++;
            std::cout << NachbarStationen[index] << " ";
            index++;
            std::cout << NachbarStationen[index] << " " << std::endl;
            index++;
        }
    }
}

/**
 * @brief Demonstriert die Clustering Algorithmen auf PSMSL Real World Daten
 * 
 * @param data = 7,8 sein um Real World Daten zu waehlen
 * @param N_punkte Anzahl der Punkte
 * @param k_input wie viele Cluster
 * @param zeitreihenl zeitreihen laenge
 * @param Jahr Jahr ab dem die Zeitreihen eingelesen werden soll
 * @param SLDN boolsche Variable des Algorithmus
 * @param SLFDN boolsche Variable des Algorithmus
 * @param SL boolsche Variable des Algorithmus
 * @param CLDN boolsche Variable des Algorithmus
 * @param CLFDN boolsche Variable des Algorithmus
 * @param CL boolsche Variable des Algorithmus
 * @param algo regelt die Konsolenausgabe
 * @param optional regelt die zusaetzliche Konsolenausgabe
 * @param perm Sollen das Clustering auch auf diesen permutierten RealWorld Daten berechnet 
 * werden?
 * @param seed Reproduzierbarkeit
 */
void main_RealWorldtesten(const int data, const int N_punkte, const int k_input, const int zeitreihenl, const int Jahr,
                          const bool SLDN, const bool SLFDN, const bool SL,
                          const bool CLDN, const bool CLFDN, const bool CL,
                          const bool algo, const bool optional, const bool perm, const bool seed)
{
    SEED = seed;
    DEBUGG_algo = algo;
    DEBUGG_optional = optional;

    const int n_ANZAHL_MESSSTATIONEN = 2360;
    const int k = k_input;
    const int Auswahl = N_punkte;

    int MODUS;

    std::unordered_map<int, std::string> map;

    int *NachbarStationen;
    NachbarStationen = (int *)malloc(Auswahl * 6 * sizeof(int *));

    float *Stationen;
    Stationen = (float *)malloc(n_ANZAHL_MESSSTATIONEN * 3 * sizeof(float *));
    GeneratorSeed seed_samen(0);

    get_originalDaten(map, NachbarStationen, Stationen, data, N_punkte, zeitreihenl,
                      Jahr, DEBUGG_algo, DEBUGG_optional);

    Datenmodell d(Auswahl, k, zeitreihenl, (int)NULL, "");
    d.erstelleDaten(Auswahl, 1, k, NachbarStationen, data, (double)NULL, seed_samen);
    UndirectedGraph Graphdummy(0);

    UndirectedGraph G_dijkstra(Auswahl + 1);
    initialerAbstandsClusterGraphGitter(G_dijkstra, d, NachbarStationen, Auswahl, zeitreihenl);
    printUngerichteterGraph(G_dijkstra, Auswahl, 1, "UngerichteterGraph auf Real World Daten");

    UndirectedGraph G_vollst(Auswahl + 1);
    //  kompletterGraph(G_vollst, d, Auswahl, zeitreihenl);
    erstelleCompleteGraphVonGraph(G_vollst, G_dijkstra, d, Auswahl, zeitreihenl);
    printUngerichteterGraph(G_vollst, Auswahl, 1, "vollstaendiger Graph auf Real World Daten");

    /*****************************************************************
     * LOKALITAET der Original Daten
     * Welche Verbindungsstrategie der Clusterings soll verwendet werden?
     * MODUS = 1 -> Single Linkage Graph
     * MODUS = 2 -> Single Linkage Frechet
     * MODUS = 5 -> Single Linkage Klasssisch
     * MODUS = 3 -> Complete Linkage Graph
     * MODUS = 4 -> Complete Linkage Frechet
     * MODUS = 6 -> Complete Linkage Klasssisch
     *****************************************************************/
    if (SL)
    {
        ClusterAlgo SL(G_vollst, Auswahl, 1, Graphdummy, MODUS = 5, k, d, Auswahl, data);
        double klassisch_SL = zielfunktionSL(SL, G_vollst, Auswahl, 1, k);
        std::cout << "klassisch_SL: " << klassisch_SL << std::endl;
        write_clusteringDaten(SL, Auswahl, Stationen, map, k, "SL");
        SL.clear();
    }
    if (SLDN)
    {
        ClusterAlgo SLDN(G_dijkstra, Auswahl, 1, Graphdummy, MODUS = 1, k, d, Auswahl, data);
        double klassisch_SLDN = zielfunktionSL(SLDN, G_vollst, Auswahl, 1, k);
        std::cout << "klassisch_SLDN: " << klassisch_SLDN << std::endl;
        write_clusteringDaten(SLDN, Auswahl, Stationen, map, k, "SLDN");
        SLDN.clear();
    }
    if (SLFDN)
    {
        ClusterAlgo SLFDN(G_dijkstra, Auswahl, 1, G_dijkstra, MODUS = 2, k, d, Auswahl, data);
        double klassisch_SLFDN = zielfunktionSL(SLFDN, G_vollst, Auswahl, 1, k);
        std::cout << "klassisch_SLFDN: " << klassisch_SLFDN << std::endl;
        write_clusteringDaten(SLFDN, Auswahl, Stationen, map, k, "SLFDN");
        SLFDN.clear();
    }
    if (CL)
    {
        ClusterAlgo CL(Graphdummy, Auswahl, 1, G_vollst, MODUS = 6, k, d, Auswahl, data);
        double klassisch_CL = zielfunktionCL(CL, G_vollst, Auswahl, 1, k);
        std::cout << "klassisch_CL: " << klassisch_CL << std::endl;
        write_clusteringDaten(CL, Auswahl, Stationen, map, k, "CL");
        CL.clear();
    }
    if (CLDN)
    {
        ClusterAlgo CLDN(G_dijkstra, Auswahl, 1, G_dijkstra, MODUS = 3, k, d, Auswahl, data);
        double klassisch_CLDN = zielfunktionCL(CLDN, G_vollst, Auswahl, 1, k);
        std::cout << "klassisch_CLDN: " << klassisch_CLDN << std::endl;
        write_clusteringDaten(CLDN, Auswahl, Stationen, map, k, "CLDN");
        CLDN.clear();
    }
    if (CLFDN)
    {
        ClusterAlgo CLFDN(G_dijkstra, Auswahl, 1, G_dijkstra, MODUS = 4, k, d, Auswahl, data);
        double klassisch_CLFDN = zielfunktionCL(CLFDN, G_vollst, Auswahl, 1, k);
        std::cout << "klassisch_CLFDN: " << klassisch_CLFDN << std::endl;
        write_clusteringDaten(CLFDN, Auswahl, Stationen, map, k, "CLFDN");
        CLFDN.clear();
    }
    G_vollst.clear();
    G_dijkstra.clear();

    free(Stationen);

    /*****************************************************************
     *  OHNE Lokalitaet auf den permutierten Zeitreihen
     *****************************************************************/
    if (perm)
    {
        permutiereZeitreihe(d, zeitreihenl, Auswahl, 1, seed_samen);

        UndirectedGraph G_dijkstra1(Auswahl + 1);
        initialerAbstandsClusterGraphGitter(G_dijkstra1, d, NachbarStationen, Auswahl, zeitreihenl);

        UndirectedGraph G_vollst1(Auswahl + 1);
        //  kompletterGraph(G_vollst1, d, Auswahl, zeitreihenl);
        erstelleCompleteGraphVonGraph(G_vollst1, G_dijkstra1, d, Auswahl, zeitreihenl);

        if (SL)
        {
            ClusterAlgo SL_perm(G_vollst1, Auswahl, 1, Graphdummy, MODUS = 5, k, d, Auswahl, data);
            double klassisch_SL_perm = zielfunktionSL(SL_perm, G_vollst1, Auswahl, 1, k);
            std::cout << "klassisch_SL_perm: " << klassisch_SL_perm << std::endl;
            SL_perm.clear();
        }
        if (SLDN)
        {
            ClusterAlgo SLDN_perm(G_dijkstra1, Auswahl, 1, Graphdummy, MODUS = 1, k, d, Auswahl, data);
            double klassisch_SLDN_perm = zielfunktionSL(SLDN_perm, G_vollst1, Auswahl, 1, k);
            std::cout << "klassisch_SLDN_perm: " << klassisch_SLDN_perm << std::endl;
            SLDN_perm.clear();
        }
        if (SLFDN)
        {
            ClusterAlgo SLFDN_perm(G_dijkstra1, Auswahl, 1, G_dijkstra1, MODUS = 2, k, d, Auswahl, data);
            double klassisch_SLFDN_perm = zielfunktionSL(SLFDN_perm, G_vollst1, Auswahl, 1, k);
            std::cout << "klassisch_SLFDN_perm: " << klassisch_SLFDN_perm << std::endl;
            SLFDN_perm.clear();
        }
        if (CL)
        {
            ClusterAlgo CL_perm(Graphdummy, Auswahl, 1, G_vollst1, MODUS = 6, k, d, Auswahl, data);
            double klassisch_CL_perm = zielfunktionCL(CL_perm, G_vollst1, Auswahl, 1, k);
            std::cout << "klassisch_CL_perm: " << klassisch_CL_perm << std::endl;
            CL_perm.clear();
        }
        if (CLDN)
        {
            ClusterAlgo CLDN_perm(G_dijkstra1, Auswahl, 1, G_dijkstra1, MODUS = 3, k, d, Auswahl, data);
            double klassisch_CLDN_perm = zielfunktionCL(CLDN_perm, G_vollst1, Auswahl, 1, k);
            std::cout << "klassisch_CLDN_perm: " << klassisch_CLDN_perm << std::endl;
            CLDN_perm.clear();
        }
        if (CLFDN)
        {
            ClusterAlgo CLFDN_perm(G_dijkstra1, Auswahl, 1, G_dijkstra1, MODUS = 4, k, d, Auswahl, data);
            double klassisch_CLFDN_perm = zielfunktionCL(CLFDN_perm, G_vollst1, Auswahl, 1, k);
            std::cout << "klassisch_CLFDN_perm: " << klassisch_CLFDN_perm << std::endl;
            CLFDN_perm.clear();
        }
        G_vollst1.clear();
        G_dijkstra1.clear();
    }
    Graphdummy.clear();
}

/**
 * @brief Methode fuer das Real World Experiment
 * 
 * @param WDH_PERM wie oft sollen die permutation der daten wiederholt werden?
 * @param datenkontrolle Boolsche Variable die steuert, ob Zeitreihen fuer die ein Clustering
 * berechnet wird in Text Datei geschrieben werden sollen.
 */
void origDaten(const int data, const int N_punkte, const int k_input, const int zeitreihenl, const int jahr, const int WDH_PERM, const bool datenkontrolle)
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

    std::ofstream offileCHECK;
    std::string pathCHECK = "results/RealWorldData/InputKontrolle/CHECK_N" + to_string(Auswahl) + "_ZR" + to_string(zeitreihenl) + ".txt";

    GeneratorSeed seed_samen(0);

    Datenmodell d(Auswahl, k, zeitreihenl, (int)NULL, "");
    d.erstelleDaten(Auswahl, 1, k, NachbarStationen, data, jahr, seed_samen);

    if (datenkontrolle)
    {
        offileCHECK.open(pathCHECK);
        offileCHECK << "Anzahl Punkte:" << Auswahl << "\n"
                    << "unveraenderte Strucktur: \n";
        d.write_data(Auswahl, 1, offileCHECK);
    }

    UndirectedGraph G_dijkstra(Auswahl + 1);
    initialerAbstandsClusterGraphGitter(G_dijkstra, d, NachbarStationen, Auswahl, zeitreihenl);
    printUngerichteterGraph(G_dijkstra, Auswahl, 1, "GitterGraph auf RealWorld Daten");

    UndirectedGraph G_vollst(Auswahl + 1);
    erstelleCompleteGraphVonGraph(G_vollst, G_dijkstra, d, Auswahl, zeitreihenl);
    printUngerichteterGraph(G_vollst, Auswahl, 1, "vollstaendiger Graph auf RealWorld Daten");

    /*****************************************************************
     * Experiment Lokalitaet der Original Daten
     *****************************************************************/

    std::string algo_daten = get_DatenmodellName(data);

    std::ofstream offile;
    std::string path = "results/RealWorldData/results/" + algo_daten + "_N=" + to_string(Auswahl) + "_ZR" + to_string(zeitreihenl) + ".txt";
    offile.open(path);
    offile << "Anzahl Punkte: " << Auswahl << " Clustering: " << k << "\n"
           << "########################################################################\n"
           << "########################################################################\n"
           << "1. Zeile RealWorld Daten\n"
           << "2. permutierte RealWorld Daten:"
           << " Mittelwerte ueber " << WDH_PERM << " Wiederholungen gebildet.\n"
           << "  n" << std::setw(10) << get_AlgorithmusName(5) << " "
           << std::setw(10) << get_AlgorithmusName(2) << " "
           << std::setw(10) << "[SLFDN/SL]"
           << " "
           << std::setw(10) << get_AlgorithmusName(1) << " "
           << std::setw(10) << "[SLDN/SL]"
           << " "
           << std::setw(10) << "[DN/FDN]"
           << " "
           << std::setw(10) << get_AlgorithmusName(6) << " "
           << std::setw(10) << get_AlgorithmusName(4) << " "
           << std::setw(10) << "[CLFDN/CL]"
           << " "
           << std::setw(10) << get_AlgorithmusName(3) << " "
           << std::setw(10) << "[CLDN/CL]"
           << std::setw(10) << "[DN/FDN]"
           << " "
           << "\n";
    UndirectedGraph Graphdummy(0);

    ClusterAlgo SL(G_vollst, Auswahl, 1, Graphdummy, MODUS = 5, k, d, Auswahl, data);
    double klassisch_SL = zielfunktionSL(SL, G_vollst, Auswahl, 1, k);
    // offile << get_AlgorithmusName(MODUS = 5) << "    " << std::setw(8) << klassisch_SL << "\n";
    offile << std::setw(3) << Auswahl << "," << std::setw(10) << klassisch_SL << ",";

    ClusterAlgo SLFDN(G_vollst, Auswahl, 1, G_dijkstra, MODUS = 2, k, d, Auswahl, data);
    double klassisch_SLFDN = zielfunktionSL(SLFDN, G_vollst, Auswahl, 1, k);
    // offile << get_AlgorithmusName(MODUS = 2) << " " << std::setw(8) << klassisch_SLFDN << " [SLFDN/SL] " << klassisch_SLFDN / klassisch_SL << "\n";
    offile << std::setw(10) << klassisch_SLFDN << "," << std::setw(10) << klassisch_SLFDN / klassisch_SL << ",";

    ClusterAlgo SLDN(G_dijkstra, Auswahl, 1, Graphdummy, MODUS = 1, k, d, Auswahl, data);
    double klassisch_SLDN = zielfunktionSL(SLDN, G_vollst, Auswahl, 1, k);
    // offile << get_AlgorithmusName(MODUS = 1) << "  " << std::setw(8) << klassisch_SLDN << " [SLDN/SL]  " << klassisch_SLDN / klassisch_SL << "\n";
    offile << std::setw(10) << klassisch_SLDN << "," << std::setw(10) << klassisch_SLDN / klassisch_SL << ",";
    offile << std::setw(10) << klassisch_SLDN / klassisch_SLFDN << ",";
    ClusterAlgo CL(Graphdummy, Auswahl, 1, G_vollst, MODUS = 6, k, d, Auswahl, data);
    double klassisch_CL = zielfunktionCL(CL, G_vollst, Auswahl, 1, k);
    // offile << get_AlgorithmusName(MODUS = 6) << "    " << std::setw(8) << klassisch_CL << "\n";
    offile << std::setw(10) << klassisch_CL << ",";

    ClusterAlgo CLFDN(G_vollst, Auswahl, 1, G_dijkstra, MODUS = 4, k, d, Auswahl, data);
    double klassisch_CLFDN = zielfunktionCL(CLFDN, G_vollst, Auswahl, 1, k);
    // offile << get_AlgorithmusName(MODUS = 4) << " " << std::setw(8) << klassisch_CLFDN << " [CLFDN/CL] " << klassisch_CLFDN / klassisch_CL << "\n";
    offile << std::setw(10) << klassisch_CLFDN << "," << std::setw(10) << klassisch_CLFDN / klassisch_CL << ",";

    ClusterAlgo CLDN(G_dijkstra, Auswahl, 1, G_dijkstra, MODUS = 3, k, d, Auswahl, data);
    double klassisch_CLDN = zielfunktionCL(CLDN, G_vollst, Auswahl, 1, k);
    // offile << get_AlgorithmusName(MODUS = 3) << "  " << std::setw(8) << klassisch_CLDN << " [CLDN/CL]  " << klassisch_CLDN / klassisch_CL << "\n";
    offile << std::setw(10) << klassisch_CLDN << "," << std::setw(10) << klassisch_CLDN / klassisch_CL << ","
           << std::setw(10) << klassisch_CLDN / klassisch_CLFDN
           << "\n";

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
    double SLDN_SLFDN_perm = 0;
    double CLDN_perm = 0;
    double CLFDN_perm = 0;
    double CLDN_CLFDN_perm = 0;

    for (int iterPerm = 1; iterPerm <= WDH_PERM; iterPerm++)
    {
        std::cout << "EXP RealWorldDaten  Permutation: (" << iterPerm << "/" << WDH_PERM << ")" << std::endl;
        permutiereZeitreihe(d, zeitreihenl, Auswahl, 1, seed_samen);

        if (datenkontrolle)
        {
            offileCHECK << "Permutation: " << iterPerm << "\n";
            d.write_data(Auswahl, 1, offileCHECK);
        }

        UndirectedGraph G_vollst1(Auswahl + 1);
        kompletterGraph(G_vollst1, d, Auswahl, zeitreihenl);

        ClusterAlgo SLperm(G_vollst1, Auswahl, 1, Graphdummy, MODUS = 5, k, d, Auswahl, data);
        double value_SL_perm = zielfunktionSL(SLperm, G_vollst1, Auswahl, 1, k);

        ClusterAlgo CLperm(Graphdummy, Auswahl, 1, G_vollst1, MODUS = 6, k, d, Auswahl, data);
        double value_CL_perm = zielfunktionCL(CLperm, G_vollst1, Auswahl, 1, k);

        UndirectedGraph G_dijkstra1(Auswahl + 1);
        initialerAbstandsClusterGraphGitter(G_dijkstra1, d, NachbarStationen, Auswahl, zeitreihenl);

        ClusterAlgo SLDNperm(G_dijkstra1, Auswahl, 1, Graphdummy, MODUS = 1, k, d, Auswahl, data);
        double value_SLDN_perm = zielfunktionSL(SLDNperm, G_vollst1, Auswahl, 1, k);
        SLDN_perm += value_SLDN_perm;

        ClusterAlgo SLFDNperm(G_vollst1, Auswahl, 1, G_dijkstra1, MODUS = 2, k, d, Auswahl, data);
        double value_SLFDN_perm = zielfunktionSL(SLFDNperm, G_vollst1, Auswahl, 1, k);
        SLFDN_perm += value_SLFDN_perm;

        SLDN_SLFDN_perm += value_SLDN_perm / value_SLFDN_perm;

        ClusterAlgo CLDNperm(G_dijkstra1, Auswahl, 1, G_dijkstra1, MODUS = 3, k, d, Auswahl, data);
        double value_CLDN_perm = zielfunktionCL(CLDNperm, G_vollst1, Auswahl, 1, k);
        CLDN_perm += value_CLDN_perm;

        ClusterAlgo CLFDNperm(G_vollst1, Auswahl, 1, G_dijkstra1, MODUS = 4, k, d, Auswahl, data);
        double value_CLFDN_perm = zielfunktionCL(CLFDNperm, G_vollst1, Auswahl, 1, k);
        CLFDN_perm += value_CLFDN_perm;

        CLDN_CLFDN_perm += value_CLDN_perm / value_CLFDN_perm;

        klassisch_SL_perm += value_SL_perm;
        klassisch_SLDN_perm += value_SLDN_perm / value_SL_perm;
        klassisch_SLFDN_perm += value_SLFDN_perm / value_SL_perm;
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
    CLDN_CLFDN_perm = CLDN_CLFDN_perm / WDH_PERM;
    SLDN_SLFDN_perm = SLDN_SLFDN_perm / WDH_PERM;
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
           << std::setw(10) << SLDN_SLFDN_perm << ","
           << std::setw(10) << klassisch_CL_perm << ","
           << std::setw(10) << CLFDN_perm << ","
           << std::setw(10) << klassisch_CLFDN_perm << ","
           << std::setw(10) << CLDN_perm << ","
           << std::setw(10) << klassisch_CLDN_perm << ","
           << std::setw(10) << CLDN_CLFDN_perm << "\n";

    offile.close();

    if (datenkontrolle)
    {
        offileCHECK.close();
    }

    free(NachbarStationen);
    d.clear();
}

void main_origDaten(const bool seed)
{
    //BEGINN GESAMT LAUFZEIT DES EXPERIEMENNTS
    struct timeval begin_exp, end_exp;
    long seconds_exp;
    long microseconds_exp;
    double time_exp;
    gettimeofday(&begin_exp, 0);

    std::ofstream offile;
    std::string path = "results/RealWorldData/results/INFO.txt";
    offile.open(path);

    SEED = seed;
    DEBUGG_algo = false;
    DEBUGG_optional = false;
    CHECK = false;
    int N, data, k, WDH_PERM;

    bool datenKontrolle = false;
    int jahr = 1990;
    int zeitreihen_l;

    /**
     * data = 8 -> monthly
     * data = 7 -> annual
     */
    zeitreihen_l = 50;
    datenKontrolle = true;
    origDaten(data = 8, N = 10, k = sqrt(3), zeitreihen_l, jahr, WDH_PERM = 4, datenKontrolle);
    origDaten(data = 8, N = 50, k = sqrt(10), zeitreihen_l, jahr, WDH_PERM = 4, datenKontrolle);
    datenKontrolle = false;
    zeitreihen_l = 100;
    origDaten(data = 8, N = 10, k = sqrt(3), zeitreihen_l, jahr, WDH_PERM = 1, datenKontrolle);
    origDaten(data = 8, N = 50, k = sqrt(50), zeitreihen_l, jahr, WDH_PERM = 10, datenKontrolle);
    origDaten(data = 8, N = 100, k = sqrt(100), zeitreihen_l, jahr, WDH_PERM = 10, datenKontrolle);
    origDaten(data = 8, N = 200, k = sqrt(200), zeitreihen_l, jahr, WDH_PERM = 10, datenKontrolle);
    origDaten(data = 8, N = 300, k = sqrt(300), zeitreihen_l, jahr, WDH_PERM = 10, datenKontrolle);
    origDaten(data = 8, N = 400, k = sqrt(400), zeitreihen_l, jahr, WDH_PERM = 10, datenKontrolle);
    origDaten(data = 8, N = 500, k = sqrt(500), zeitreihen_l, jahr, WDH_PERM = 10, datenKontrolle);

    // ENDE ZEITMESSUNG
    gettimeofday(&end_exp, 0);
    seconds_exp = end_exp.tv_sec - begin_exp.tv_sec;
    microseconds_exp = end_exp.tv_usec - begin_exp.tv_usec;
    time_exp = seconds_exp + microseconds_exp * 1e-6;

    offile << "####################################\n"
           << "Gesamt Laufzeit Experiment RealWorld Daten: " << get_stringTime(time_exp) << "\n";

    time_t aktuelleZeit;
    struct tm lokal;
    aktuelleZeit = time(NULL);
    lokal = *localtime(&aktuelleZeit);
    offile << "Durchgefuehrt: " << currentDateTime() << "\n";
    offile.close();
}
