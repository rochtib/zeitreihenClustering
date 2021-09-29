/**
 ***********************************************************************************
 ***********************************************************************************    
 LAUFZEIT UNTERSUCHUNG 
 ***********************************************************************************
 ***********************************************************************************
 */

extern bool DEBUGG_algo;
extern bool DEBUGG_optional;
extern bool SEED;
extern bool KantenSpeicher;
extern double zeit_global;
bool mitInitialsierungTime = false;
extern int counterFrechet;

void main_ZeitMessenExp(const int MODUS, Datenmodell &d, const int data, const int zeitreihenl, const int k, const int n, const int m)
{
    const int anz_gitterpkt = n * m;
    double *direkte4Nachbarn;
    direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));
    counterFrechet = 0;
    reset_time();
    switch (MODUS)
    {
    case 1: // SLDN
    case 3: // CLDN
    {
        berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, zeitreihenl);
        printDirekte4Nachbarn(direkte4Nachbarn, n, m);
        UndirectedGraph G_Gitter(anz_gitterpkt);
        erstelleGitterGraph(G_Gitter, direkte4Nachbarn, n, m);
        UndirectedGraph Graphdummy(0);
        ClusterAlgo DN(G_Gitter, n, m, G_Gitter, MODUS, k, d, anz_gitterpkt, data);

        Graphdummy.clear();
        G_Gitter.clear();
        DN.clear();
        break;
    }
    case 2: // SLFDN
    case 4: // CLFDN
    {
        UndirectedGraph G_Gitter(anz_gitterpkt);
        initialerGitterGraph(G_Gitter, d, n, m, zeitreihenl);
        UndirectedGraph Graphdummy(0);
        ClusterAlgo FDN(G_Gitter, n, m, G_Gitter, MODUS, k, d, anz_gitterpkt, data);

        Graphdummy.clear();
        G_Gitter.clear();
        FDN.clear();
        break;
    }
    case 5: // SL
    case 6: // CL
    {
        UndirectedGraph G_vollst(anz_gitterpkt);
        kompletterGraph(G_vollst, d, anz_gitterpkt, zeitreihenl);
        ClusterAlgo klassisch(G_vollst, n, m, G_vollst, MODUS, k, d, anz_gitterpkt, data);

        G_vollst.clear();
        klassisch.clear();
        break;
    }
    default:
        std::cout << " Den Algorithmus " << MODUS << " gibt es nicht!";
        break;
    }
    free(direkte4Nachbarn);
}

void experiment_Laufzeit(const std::string SETTING, const int data, const int MODUS, const bool datenkontrolle, const int n_start, const int n_ende,
                         const int m_start, const int m_ende, const int k_start, const int k_ende)
{
    std::string speicher = "";
    if (MODUS == 2 || MODUS == 4)
    {
        if (KantenSpeicher == true)
        {
            speicher = "_Eff";
        }
        if (KantenSpeicher == false)
        {
            speicher = "_NOTEff";
        }
    }

    int n;
    int m;
    int k;
    int zeitreihenl;

    std::string algo_name = get_AlgorithmusName(MODUS);
    std::string algo_daten = get_DatenmodellName(data);

    std::string initial = "";

    if (mitInitialsierungTime)
    {
        initial = "OHNEinitial";
    }

    const int WDH_CPU = 15;
    for (int z = 1; z <= 3; z++)
    {
        switch (z)
        {
        case 1:
            zeitreihenl = 5;
            break;
        case 2:
            zeitreihenl = 25;
            break;
        case 3:
            zeitreihenl = 50;
            break;
        default:
            zeitreihenl = 5;
            break;
        }

        for (int k_index = k_start; k_index <= k_ende + 1; k_index += 10)
        {
            k = k_index;

            std::ofstream offile;
            std::string path = "results/runTime/results/" + SETTING + algo_name + speicher + +"_ZR" + to_string(zeitreihenl) + "k" + to_string(k) + initial + ".txt";

            offile.open(path);
            offile << algo_name << " auf " << algo_daten << "\n";
            offile << "Die Laenge der Zeitreihe betraegt " << zeitreihenl << ".\n";
            offile << "Jede Berechnung wurde " << WDH_CPU << " Mal wiederholt und CPU Zeit gemessen.\n";

            std::ofstream offileCHECK;
            std::string pathCHECK = "results/runTime/InputKontrolle/" + SETTING + algo_name + speicher + "_ZR" + to_string(zeitreihenl) + " k " + to_string(k) + initial + ".txt ";

            if (datenkontrolle)
            {
                offileCHECK.open(pathCHECK);
                offileCHECK << algo_name << "\n"
                            << "Jede Berechnung wurde " << WDH_CPU << " Mal wiederholt und CPU Zeit gemessen.\n";
            }
            offile << "##########################################################################################################################\n"
                   << "##########################################################################################################################\n"
                   << " n" << std::setw(2) << " "
                   << "m" << std::setw(2) << " "
                   << "k" << std::setw(2) << " "
                   << "AvgTime[Sec] "
                   << "MedTime[Sec] "
                   << "MinTime[Sec] "
                   << "MaxTime[Sec] "
                   << "FREQ[Frechet]"
                   << "\n";
            offileCHECK << "##########################################################################################################################\n"
                        << "##########################################################################################################################\n";
            for (int n_index = n_start; n_index <= n_ende; n_index++)
            {
                n = n_index;
                for (int m_index = m_start; m_index <= m_ende; m_index++)
                {
                    m = m_index;
                    if (n_index >= m_index)
                    {
                        if (n * m > k)
                        {
                            double min_time = std::numeric_limits<double>::max();
                            double max_time = 0;
                            double time = 0;
                            std::vector<double> time_vector;

                            GeneratorSeed seed_samen1(0);
                            GeneratorSeed seed_samen2(0);
                            double iter_tot_time = 0;
                            double iter_median_time = 0;
                            int iter_counter_frechet = 0;
                            std::vector<double> iter_time_vector;
                            Datenmodell d(n * m, k, zeitreihenl, (int)NULL, SETTING);
                            d.erstelleDatenLaufzeit(n, m, k, seed_samen1, seed_samen2);
                            for (int iterCPU = 1; iterCPU <= WDH_CPU; iterCPU++)
                            {
                                std::cout << "EXPRunTime " << algo_name << speicher << initial << "," + algo_daten + ", ZR:(" << zeitreihenl << "/50), n:(" << n << "/" << n_ende << "), m:(" << m << "/" << m_ende << "), k:(" << k << "/" << k_ende << "), Wdh:"
                                          << "(" << iterCPU << "/" << WDH_CPU << ")" << std::endl;
                                if (datenkontrolle)
                                {
                                    offileCHECK << "n:" << std::setw(2) << n << " m:" << std::setw(2) << m << " k:" << std::setw(2) << k << "\n";
                                    d.write_data(n, m, offileCHECK);
                                }
                                main_ZeitMessenExp(MODUS, d, data, zeitreihenl, k, n, m);
                                iter_tot_time += zeit_global;
                                iter_time_vector.push_back(zeit_global);
                                iter_counter_frechet += counterFrechet;
                                // Minimum Zeit finden
                                if (min_time > zeit_global)
                                {
                                    min_time = zeit_global;
                                }
                                // Maximum Zeit finden
                                if (max_time < zeit_global)
                                {
                                    max_time = zeit_global;
                                }
                            }
                            std::sort(iter_time_vector.begin(), iter_time_vector.end());
                            if (iter_time_vector.size() % 2 == 0)
                            {
                                double median1 = iter_time_vector[(iter_time_vector.size() / 2)];
                                double median2 = iter_time_vector[((iter_time_vector.size() - 1) / 2)];
                                iter_median_time = (median1 + median2) / 2.;
                            }
                            else
                            {
                                iter_median_time = iter_time_vector[((iter_time_vector.size()) / 2)];
                            }
                            time = iter_tot_time / iter_time_vector.size();

                            iter_counter_frechet = iter_counter_frechet / iter_time_vector.size();

                            iter_time_vector.clear();
                            d.clear();
                            offile << std::setw(2) << n << "," << std::setw(2) << m << "," << std::setw(2) << k << ","
                                   << std::setw(12) << time << "," << std::setw(12) << iter_median_time << ","
                                   << std::setw(12) << min_time << "," << std::setw(12) << max_time << ","
                                   << std::setw(12) << iter_counter_frechet << "\n";
                            time_vector.clear();
                        }
                    }
                }
            }

            offile.close();

            if (datenkontrolle)
            {
                offileCHECK.close();
            }
        }
    }
}

void main_Laufzeit(const bool seed)
{
    //BEGINN GESAMT LAUFZEIT DES EXPERIEMENNTS
    struct timeval begin_exp, end_exp;
    long seconds_exp;
    long microseconds_exp;
    double time_exp;
    gettimeofday(&begin_exp, 0);

    std::ofstream offile;
    std::string path = "results/runTime/results/INFO.txt";
    offile.open(path);

    DEBUGG_algo = false;
    DEBUGG_optional = false;
    SEED = seed;
    int data, MODUS, n_start, n_ende, m_start, m_ende, k_start, k_ende;
    std::string SETTING;
    CHECK = false;
    mitInitialsierungTime = false;
    bool datenkontrolle;
    /**
     * data = 1 -> RECHTECKE && kennen optimales k Clustering
     * data = 2 -> RECHTECKE_ineinander &&  kennen optimales k Clustering
     * data = 3 -> STREIFEN && okennen optimales k Clustering
     * data = 4 -> RANDOM && kennen optimales k Clustering
     * data = 5 -> RLR MONAT && wissen kein optimales Clustering
     * data = 6 -> RLR JAHR && wissen kein optimales Clustering
     * 
     * MODUS = 1 -> Single Linkage Direkte Nachbar
     * MODUS = 2 -> Single Linkage Frechet Direkter Nachbar
     * MODUS = 3 -> Complete Linkage Direkte Nachbar
     * MODUS = 4 -> Complete Linkage Frechet Direkter Nachbar
     * MODUS = 5 -> Single Linkage Klasssisch
     * MODUS = 6 -> Complete Linkage Klasssisch
     */

    datenkontrolle = true;
    KantenSpeicher = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 2, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = false;
    datenkontrolle = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 2, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 4, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 4, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = true;

    mitInitialsierungTime = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 5, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    mitInitialsierungTime = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 5, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);

    mitInitialsierungTime = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 6, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    mitInitialsierungTime = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 6, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);

    mitInitialsierungTime = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 3, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    mitInitialsierungTime = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 3, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);

    mitInitialsierungTime = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 1, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    mitInitialsierungTime = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesRANDOM", data = 4, MODUS = 1, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);

    KantenSpeicher = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesKONSTANT", data = 4, MODUS = 2, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesKONSTANT", data = 4, MODUS = 2, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = true;
    experiment_Laufzeit(SETTING = "RunTimeSeriesKONSTANT", data = 4, MODUS = 4, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = false;
    experiment_Laufzeit(SETTING = "RunTimeSeriesKONSTANT", data = 4, MODUS = 4, datenkontrolle, n_start = 4, n_ende = 10, m_start = 4, m_ende = 10, k_start = 5, k_ende = 25);
    KantenSpeicher = true;

    // ENDE ZEITMESSUNG
    gettimeofday(&end_exp, 0);
    seconds_exp = end_exp.tv_sec - begin_exp.tv_sec;
    microseconds_exp = end_exp.tv_usec - begin_exp.tv_usec;
    time_exp = seconds_exp + microseconds_exp * 1e-6;

    offile << "####################################\n"
           << "Gesamt Laufzeit Experiment runTime: " << get_stringTime(time_exp) << "\n";

    time_t aktuelleZeit;
    struct tm lokal;
    aktuelleZeit = time(NULL);
    lokal = *localtime(&aktuelleZeit);
    offile << "Durchgefuehrt: " << currentDateTime() << "\n";
    offile.close();
}
