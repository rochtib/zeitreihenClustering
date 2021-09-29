extern bool DEBUGG_optional;
extern bool DEBUGG_algo;
extern bool SEED;
extern bool CHECK;

/**
 * data = 1 -> RECHTECKE  
 * data = 2 -> RINGE 
 * data = 3 -> STREIFEN  
 * data = 4 -> RANDOM  
 * data = 5 -> RLR MONAT  
 * data = 6 -> RLR JAHR  
 * data = 8 -> rlr_monthly originale Lokalitaet beibehalten
 * data = 7 -> rlr_annual originale Lokalitaet beibehalten
 */

class Datenmodell
{
    int *cluster_info;
    int *muster_union;
    double *zeitreihenMatrix;
    int zeitreihenl;
    int zeitreihen_start_jahr;
    std::string type_daten;

public:
    Datenmodell(const int anz_gitterpkt, const int k, const int data_zeitreihenl, const int input_jahr, const std::string input_type)
    {
        cluster_info = (int *)(malloc(k * 2 * sizeof(int *)));

        muster_union = (int *)(malloc((anz_gitterpkt + 1) * sizeof(int *)));

        zeitreihenMatrix = (double *)malloc(anz_gitterpkt * data_zeitreihenl * sizeof(double *));

        zeitreihenl = data_zeitreihenl;

        zeitreihen_start_jahr = input_jahr;
        type_daten = input_type;
    }

    /**
     * @brief Prueft den Cluster Zusammenhang. 
     * Hat das Datenmodell Zusammenhaengende Cluster erstellt?
     * @param muster_union speichert die IDs der Gitterpunkte
     * @param cluster_info 2xk Matrix. Enthaelt ClusterID mit zugehoeriger Anzahl an Clusterpunkten
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     * @return true  -> Zusammenhaengend
     * @return false -> nicht Zusammenhaengend
     */
    bool checkDatenmodell(const int n, const int m, const int k)
    {

        int help_id;
        int help_calc;
        int counter;
        for (int i = 1; i <= n * m; i++)
        {
            help_id = muster_union[i];
            int anz_pkt = 0;
            for (int r = 0; r < k; r++)
            {
                if (cluster_info[r * 2] == help_id)
                {
                    anz_pkt = cluster_info[(help_id - 1) * 2 + 1];
                    break;
                }
            }
            if (anz_pkt > 1)
            {
                counter = 0;
                help_calc = i % n;
                if (help_calc != 0)
                {
                    if (help_id == muster_union[i + 1])
                    {
                        counter++;
                    }
                }
                if (help_calc != 1)
                {
                    if (help_id == muster_union[i - 1])
                    {
                        counter++;
                    }
                }
                if ((int)i + n <= n * m)
                {
                    if (help_id == muster_union[i + n])
                    {
                        counter++;
                    }
                }
                if ((int)i - n >= 1)
                {
                    if (help_id == muster_union[i - n])
                    {
                        counter++;
                    }
                }
                if (counter == 0 && muster_union[i] != 0) // es gibt keinen direkten Nachbar
                {
                    std::cout << RED << "ERROR:" << RESET << "Fehler bei Knoten:" << i << std::endl;

                    std::cout << RED << "ERROR:" << RESET << "Fehlerhaftes cluster" << std::endl;

                    std::cout << "Cluster info" << std::endl;
                    for (int j = 0; j < k; j++)
                    {
                        std::cout << cluster_info[j * 2 + 0] << " " << cluster_info[j * 2 + 1] << std::endl;
                    }
                    return false;
                }
            }
        }
        return true;
    }

    /**
     * @brief Checkt ob alle Knoten ein Cluster haben.
     * @return true 
     * @return false 
     */
    bool check_sumCluster(const int n, const int m, const int k)
    {
        int sum = 0;

        for (int j = 0; j < k; j++)
        {
            sum += cluster_info[j * 2 + 1];
        }
        if (sum != n * m)
        {
            std::cout << "Cluster info" << std::endl;
            for (int j = 0; j < k; j++)
            {
                std::cout << cluster_info[j * 2 + 0] << " " << cluster_info[j * 2 + 1] << std::endl;
            }
            return false;
        }
        return true;
    }

    /**
     * @brief Gibt das IDs der Gitterpunkte in Form des Gitters auf der Konsole aus.
     * 
     * @param datenmodellClustering Gitterarray enthaelt zu jedem Gitterpunkt eine ClusterID. 
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     */
    void print_DatenModellCluster(const int n, const int m)
    {
        if (n * m <= 25 * 25 && DEBUGG_algo)
        {
            std::cout << "Clustering aus Datenmodell:" << std::endl;
            for (int j = 0; j < m; j++)
            {
                for (int i = 1; i <= n; i++)
                {
                    std::setw(3);
                    printf("\033[48;5;%dm %3d\033[m", muster_union[j * n + i], muster_union[j * n + i]);
                }
                std::cout << std::endl;
            }
        }
    }

    /**
     * @brief Initialisert ein Array mit Nullen. Methode wird gebraucht in der Methode RECHTECKE.
     * Es muss nach jeder Iteration ein Array bereingt werden.
     * 
     * @param array (n*m) x 4 Array
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     */
    void initialisieren(int *array, const int n, const int m)
    {
        for (int i = 0; i <= m * n; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                array[i * 4 + j] = 0;
            }
        }
    }

    /**
     * @brief Zeichnet Eckkoordinaten der Quadrate, wobei mindestens eine Koordinate ungleich 0 sein muss.
     * Funktion ist eine Hilfe zur Methode RECHTECKE.
     * 
     * @param cluster_fin enthaelt die Eckkoordinaten des Clusters
     * @param k Anzahl der Cluster
     */
    void print_KoordinatenCluster(const int *cluster_fin, const int k)
    {
        int p = 4;
        int x1;
        int x2;
        int y1;
        int y2;
        std::cout << "print Cluster Eckkoordinaten:" << std::endl;
        for (int i = 0; i < k; i++)
        {
            x1 = cluster_fin[i * p + i];
            x2 = cluster_fin[i * p + i + 1];
            y1 = cluster_fin[i * p + i + 2];
            y2 = cluster_fin[i * p + i + 3];
            //std::cout<<"print"<<std::endl;
            if (x2 != 0 && y2 != 0)
            {
                std::cout << cluster_fin[i * p + i] << " ";
                std::cout << cluster_fin[i * p + i + 1] << " ";
                std::cout << cluster_fin[i * p + i + 2] << " ";
                std::cout << cluster_fin[i * p + i + 3] << "" << std::endl;
            }
        }
    }

    /**
     * @brief  cluster_fin bekommt 1:1 die Werte von cluster_work und cluster_work
     * wird vollstaendig auf 0 gesetzt. Methode fuer RECHTECKE.
     */
    void copy_cluster(int *cluster_fin, int *cluster_work, const int n, const int m, const int k)
    {
        int p = 4;
        for (int x = 0; x <= k; x++)
        {
            cluster_fin[x * p + x] = cluster_work[x * p + x];
            cluster_fin[x * p + x + 1] = cluster_work[x * p + x + 1];
            cluster_fin[x * p + x + 2] = cluster_work[x * p + x + 2];
            cluster_fin[x * p + x + 3] = cluster_work[x * p + x + 3];
        }
        initialisieren(cluster_work, n, m);
    }

    /**
     * @brief Erstellt ein Cluster nach dem Schema: Halbiere solange ein Viereck bis man die gewuenschte 
     * Clusterzahl erhaelt. Grundsaetzlich wird sich in der Richtung der Halbierung (horizontal/vertikal)
     * abgewechselt.
     * 
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     * @param k Anzahl der Cluster
     * @param arr Gitterarray enthaelt zu jedem Gitterpunkt eine ClusterID.
     * @param cluster_info 2xk Matrix. Enthaelt ClusterID mit zugehoeriger Anzahl an Clusterpunkten
     * @param muster_union speichert die IDs der Gitterpunkte
     */
    void RECHTECKE(const int n_daten, const int m_daten, int k)
    {
        int n = n_daten;
        int m = m_daten;

        int count = 0;
        int p = 4;
        int *cluster_fin;
        cluster_fin = (int *)(malloc(p * (n * m) * sizeof(int *)));
        /**
         * cluster_fin ist eine n*m x 4 Matrix. Die Spalte 0 und 1 entsprechen der Seitenlaenge auf
         * einer xAchse. Analog Spalte 2 und 3.
         */

        int *cluster_work;
        cluster_work = (int *)(malloc(p * (n * m) * sizeof(int *)));
        /**
         * In cluster_work werden die neuen halbierten Koordinaten gespeichert. Anschließend wird 
         * cluster_fin auf cluster_work gesetzt und cluster_work geloescht. Es wird so lange ein
         * Rechteck geteilt bis man auf die gewuenschte Anzahl kommt.
         */

        for (int i = 0; i < (n * m) + 1; i++)
        {
            muster_union[i] = 0;
        }

        initialisieren(cluster_fin, n, m);
        initialisieren(cluster_work, n, m);

        //das Initiale Rechteck hat die Groesse vom Gitter
        cluster_fin[0] = 0;
        cluster_fin[1] = n;
        cluster_fin[2] = 0;
        cluster_fin[3] = m;

        // print_muster(cluster_fin,k);
        // print_muster(cluster_work,k);

        int x1, x2;
        int y1, y2;
        int count_work = 0;
        bool richtung = true;
        int c = 1;

        //wenn nur ein Cluster erstellt werden soll, muss nichts berechnet werden
        while (c < k && k != 1)
        {
            int uu = c;
            for (int i = 0; i < uu; i++)
            {
                x1 = cluster_fin[i * p + i];
                x2 = cluster_fin[i * p + i + 1];
                y1 = cluster_fin[i * p + i + 2];
                y2 = cluster_fin[i * p + i + 3];

                if (c < k) // Abbruch wenn in einem Durchgang die gewuenschte Anzahl erreicht ist
                {
                    if (x2 != 0 && y2 != 0)
                    {
                        if (richtung == true)
                        {
                            if (x1 + 1 == x2)
                            {
                                // Cluster kann nicht weiter geteilt werden
                                cluster_work[count_work * p + count_work] = x1;
                                cluster_work[count_work * p + count_work + 1] = x2;
                                cluster_work[count_work * p + count_work + 2] = y1;
                                cluster_work[count_work * p + count_work + 3] = y2;
                                count_work++;
                            }
                            else
                            {
                                cluster_work[count_work * p + count_work] = x1;
                                cluster_work[count_work * p + count_work + 1] = (x1 + x2) / 2;
                                cluster_work[count_work * p + count_work + 2] = y1;
                                cluster_work[count_work * p + count_work + 3] = y2;
                                count_work++;

                                cluster_work[count_work * p + count_work] = (x1 + x2) / 2;
                                cluster_work[count_work * p + count_work + 1] = x2;
                                cluster_work[count_work * p + count_work + 2] = y1;
                                cluster_work[count_work * p + count_work + 3] = y2;
                                count_work++;
                                c = c + 1;
                            }
                        }
                        else if (richtung == false)
                        {
                            if (y1 + 1 == y2)
                            {
                                // Cluster kann nicht weiter geteilt werden
                                cluster_work[count_work * p + count_work] = x1;
                                cluster_work[count_work * p + count_work + 1] = x2;
                                cluster_work[count_work * p + count_work + 2] = y1;
                                cluster_work[count_work * p + count_work + 3] = y2;
                                count_work++;
                            }
                            else
                            {
                                cluster_work[count_work * p + count_work] = x1;
                                cluster_work[count_work * p + count_work + 1] = x2;
                                cluster_work[count_work * p + count_work + 2] = y1;
                                cluster_work[count_work * p + count_work + 3] = (y1 + y2) / 2;
                                count_work++;

                                cluster_work[count_work * p + count_work] = x1;
                                cluster_work[count_work * p + count_work + 1] = x2;
                                cluster_work[count_work * p + count_work + 2] = (y1 + y2) / 2;
                                cluster_work[count_work * p + count_work + 3] = y2;
                                count_work++;
                                c = c + 1;
                            }
                        }
                    }
                }
                else
                {
                    // wenn Anzahl erreicht dann werden alle ungleich 0 einfach uebernommen
                    if (x2 != 0 && y2 != 0)
                    {
                        cluster_work[count_work * p + count_work] = x1;
                        cluster_work[count_work * p + count_work + 1] = x2;
                        cluster_work[count_work * p + count_work + 2] = y1;
                        cluster_work[count_work * p + count_work + 3] = y2;
                        count_work++;
                    }
                }
            }

            //  print_KoordinatenCluster(cluster_work, count_work);

            /**********************************+***++++++++++++*************************
             * Richtungswechsel Wie sollen Rechtecke geteilt werden?
             * Horizontal oder Vertikal. Nach jedem Durchgang wird die Richtung geaendert.
             **********************************+***++++++++++++*************************/
            if (richtung == false)
            {
                richtung = true;
            }
            else if (richtung == true)
            {
                richtung = false;
            }

            copy_cluster(cluster_fin, cluster_work, n, m, count_work);

            if (count_work >= k)
            {
                count = k; // fuehrt zum Abbruch der While Schleife
            }
            count_work = 0;
        }

        int ID = 1;
        int cluster_pkt_zaehler = 0; // zaehlt die Clusterpunkte fuer cluster_info
        for (int x = 0; x < k; x++)
        {
            x1 = cluster_fin[x * p + x];
            x2 = cluster_fin[x * p + x + 1];
            y1 = cluster_fin[x * p + x + 2];
            y2 = cluster_fin[x * p + x + 3];
            cluster_pkt_zaehler = 0;
            for (int i = x1; i < x2; i++)
            {
                for (int j = y1; j < y2; j++)
                {
                    if (m != n)
                    {
                        muster_union[i + 1 + j * n] = ID;
                    }
                    else
                    {
                        muster_union[i * n + j + 1] = ID;
                    }
                    cluster_pkt_zaehler++;
                }
            }
            cluster_info[(ID - 1) * 2] = ID;
            cluster_info[(ID - 1) * 2 + 1] = cluster_pkt_zaehler;
            ID++;
        }

        print_DatenModellCluster(n, m);

        free(cluster_fin);
        free(cluster_work);
    }

    /**
     * @brief Erstellt ein verschachteltes Cluster. Vierecke werden ineinander gemalt.
     * 
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     * @param k Anzahl der Cluster
     * @param arr Gitterarray enthaelt zu jedem Gitterpunkt eine ClusterID.
     * @param cluster_info 2xk Matrix. Enthaelt ClusterID mit zugehoeriger Anzahl an Clusterpunkten
     * @param muster_union speichert die IDs der Gitterpunkte
     */
    void RINGE(const int n, const int m, int k)
    {
        const int min_dimension = std::min(n, m);
        int seite = min_dimension;
        int hoehe = std::max(n, m);

        if (n > m)
        {
            int dummy_seite = seite;
            seite = hoehe;
            hoehe = dummy_seite;
        }
        int cluster_max = min_dimension / 2;

        // Die Anzahl der Cluster ist beschraenkt
        if (min_dimension - 2 * k != 0)
        {
            if (min_dimension - 2 * k < 0)
            {
                std::cout << "k=" << k << " ist zu groß.";
                k = cluster_max;
                if (cluster_max * 2 < min_dimension)
                {
                    k++;
                    cluster_max++;
                }
                std::cout << "k wurde auf cluster_max gesetzt, k:=" << k << std::endl;
            }
            else
            {
                cluster_max++;
            }
        }
        // std::cout << "cluster_max" << cluster_max << std::endl;

        int *cluster_array;
        cluster_array = (int *)(malloc(k * sizeof(int *)));
        /**
         * cluster_array enthaelt fuer jedes Cluster die Anzahl der Ringe. Die Insgesamte Anzahl von
         * Ringen darf der minimalen Dimension nicht uerbersteigen.
         */

        int counter = 0; // zaehler im Programm

        for (int i = 0; i < k; i++)
        {
            cluster_array[i] = 0; //Initial
        }

        /**
         * Es werden den Clustern nach der Reihe ihre Anzahl an Ringen zugeordnet.
         * Im Fall wenn es nicht glatt auf geht, bekommen die aussersten mehr zugewiesen.
         */
        while (counter < cluster_max)
        {
            for (int i = 0; i < k; i++)
            {
                if (counter == cluster_max)
                {
                    break;
                }
                cluster_array[i] += 1;
                counter++;
            }
        }
        // int sum = 0;
        // for (int i = 0; i < k; i++)
        // {
        //     std::cout << cluster_array[i] << std::endl;
        //     sum = sum + cluster_array[i];
        // }
        // std::cout << "k:" << k << " summe: " << sum << std::endl;

        counter = 0;
        int count_cluster_groesse = 0; // fuer cluster_info
        int anz_ringe, cl_ring;
        int num = 0;
        int ID_cl = 1;

        {
            while (counter < cluster_max)
            {
                anz_ringe = cluster_array[num];
                cl_ring = 0;
                count_cluster_groesse = 0;
                while (cl_ring < anz_ringe)
                {
                    for (int i = 0 + counter + 1; i <= seite - counter; i++)
                    {
                        count_cluster_groesse += 2;
                        muster_union[counter * seite + i] = ID_cl;
                        muster_union[(hoehe - counter - 1) * seite + i] = ID_cl;
                    }
                    for (int i = counter + 1; i < hoehe - counter - 1; i++)
                    {
                        count_cluster_groesse += 2;
                        muster_union[i * seite + counter + 1] = ID_cl;
                        muster_union[i * seite + seite - counter] = ID_cl;
                    }
                    cl_ring++;
                    counter++;
                }
                cluster_info[(ID_cl - 1) * 2] = ID_cl;
                cluster_info[(ID_cl - 1) * 2 + 1] = count_cluster_groesse - 2;
                ID_cl++;
                num++;
            }
        }
        checkDatenmodell(n, m, k);

        print_DatenModellCluster(n, m);
        free(cluster_array);
    }

    /**
     * @brief Erstellt strikt horizontale oder vertikale Streifen Clusterstreifen. 
     * Limitiert durch die Groesse der Dimension.
     * 
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     * @param k Anzahl der Cluster
     * @param datenmodellClustering Gitterarray enthaelt zu jedem Gitterpunkt eine ClusterID.
     * @param cluster_info 2xk Matrix. Enthaelt ClusterID mit zugehoeriger Anzahl an Clusterpunkten
     * @param muster_union speichert die IDs der Gitterpunkte
     */
    void STREIFEN(const int n, const int m, int k)
    {
        int *cluster_array;
        cluster_array = (int *)(malloc(k * sizeof(int *)));
        /**
         * cluster_array speichert fuer jedes Clustering die breite. Die Anzahl der Cluster ist
         * durch die groesste Dimension.
         */

        for (int i = 0; i < (n * m) + 1; i++)
        {
            muster_union[i] = 0;
        }

        const int max_dimension = std::max(n, m);

        if (k > max_dimension)
        {
            std::cout << "k=" << k << " ist zu groß";
            k = max_dimension;
            std::cout << "k wurde auf cluster_max gesetzt, k:=" << k << std::endl;
        }

        //Aufteilen des Gitters in gleich große horizontale oder vertikale Streifen
        int test1 = max_dimension / k;
        int rest = max_dimension - test1 * k;
        int teil;
        if (rest == 0)
        {
            teil = test1;
        }
        else if (rest > 0)
        {
            teil = test1;
        }
        else
        {
            teil = 1;
            rest = max_dimension - k;
        }

        // Jedes Cluster bekommt auf jeden Fall die Groesse teil
        for (int i = 0; i < k; i++)
        {
            cluster_array[i] = teil;
        }

        // Wenn es nicht auf geht wird der Rest gleichmassig aufgeteilt
        if (rest != 0)
        {
            for (int i = 0; i < rest; i++)
            {
                cluster_array[i] += 1;
            }
        }

        int x1 = 0;
        int x2;
        int ID = 1;
        int count_cluster_groesse = 0; // fuer cluster_info
        for (int x = 0; x < k; x++)
        {
            x2 = cluster_array[x] + x1 - 1;
            count_cluster_groesse = 0;

            for (int i = x1; i <= x2; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    if (n <= m)
                    {
                        if (m != n)
                        {
                            // datenmodellClustering[i * n + j] = ID;
                            muster_union[i * n + j + 1] = ID;
                        }
                        else
                        {
                            // datenmodellClustering[i + j * n] = ID;
                            muster_union[i + j * n + 1] = ID;
                        }
                    }
                    else
                    {
                        if (m != n)
                        {
                            // datenmodellClustering[j * n + i] = ID;
                            muster_union[j * n + i + 1] = ID;
                        }
                        else
                        {
                            // datenmodellClustering[j + i * n] = ID;
                            muster_union[j + i * n + 1] = ID;
                        }
                    }
                    count_cluster_groesse++;
                }
            }
            cluster_info[(ID - 1) * 2] = ID;
            cluster_info[(ID - 1) * 2 + 1] = count_cluster_groesse;
            x1 = x2 + 1;
            ID++;
        }
        print_DatenModellCluster(n, m);

        checkDatenmodell(n, m, k);
        free(cluster_array);
    }

    /**
     * @brief Fuegt alle freien Nachbarn eines Kontens der Menge hinzu.
     * 
     * @param set_nachbar 
     * @param muster_union speichert die IDs der Gitterpunkte
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters 
     * @param knoten 
     */
    void insert_set(std::set<int> &set_nachbar, const int n, const int m, const int knoten)
    {
        int neuer_clusterpkt = knoten;
        int help = neuer_clusterpkt % n;

        // std::cout << "neuer_clusterpkt:" << neuer_clusterpkt << "   help:" << help << std::endl;

        if (help != 0)
        {
            // std::cout<<"Hier1"<<std::endl;
            if (muster_union[neuer_clusterpkt + 1] == 0 && !set_nachbar.contains(neuer_clusterpkt + 1))
            {
                // std::cout<<"insert"<<neuer_clusterpkt+1<<std::endl;
                set_nachbar.insert(neuer_clusterpkt + 1);
            }
        }
        if (help != 1)
        {
            // std::cout<<"Hier2"<<std::endl;
            if (muster_union[neuer_clusterpkt - 1] == 0 && !set_nachbar.contains(neuer_clusterpkt - 1))
            {
                // std::cout<<"insert"<<neuer_clusterpkt-1<<std::endl;
                set_nachbar.insert(neuer_clusterpkt - 1);
            }
        }
        if ((int)neuer_clusterpkt + n <= n * m)
        {
            // std::cout<<"Hier3"<<std::endl;
            if (muster_union[neuer_clusterpkt + n] == 0 && !set_nachbar.contains(neuer_clusterpkt + n))
            {
                // std::cout<<"insert"<<neuer_clusterpkt+n<<std::endl;
                set_nachbar.insert(neuer_clusterpkt + n);
            }
        }
        if ((int)neuer_clusterpkt - n >= 1)
        {
            // std::cout<<"Hier4"<<std::endl;
            if (muster_union[neuer_clusterpkt - n] == 0 && !set_nachbar.contains(neuer_clusterpkt - n))
            {
                // std::cout<<"insert"<<neuer_clusterpkt-n<<std::endl;
                set_nachbar.insert(neuer_clusterpkt - n);
            }
            // std::cout << "set_nachbar groeßße" << set_nachbar.size() << std::endl;
        }
    }

    /**
     * @brief Findet zufaellig einen Knoten der noch kein Cluster hat.
     * 
     * @param muster_union speichert die IDs der Gitterpunkte
     * @param ids
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     * @param knoten
     */
    int find_next_NB(const int n, const int m, GeneratorSeed seed_samen)
    {

        auto rd = seed_samen.get_samen();
        std::default_random_engine rng(rd());
        std::vector<int> perm;

        for (int i = 1; i <= n * m; i++)
        {
            perm.push_back(i);
        }

        std::shuffle(std::begin(perm), std::end(perm), rng);

        for (int i = 0; i < n * m; i++)
        {
            if (muster_union[perm.at(i)] == 0)
            {
                return (int)perm.at(i);
            }
        }
        return INT_MAX;
    }

    /**
     * @brief Erstellt zufaellig ein k - Clustering. Die Groesse des Clusters
     * wird auf zufaellig festgelegt im Bereich 1 bis n*m/2.
     * 
     * @param n Anzahl Spalten des Gitters
     * @param m Anzahl Spalten des Gitters
     * @param k Anzahl der Cluster
     * @param arr Gitterarray enthaelt zu jedem Gitterpunkt eine ClusterID.
     * @param cluster_info 2xk Matrix. Enthaelt ClusterID mit zugehoeriger Anzahl an Clusterpunkten
     * @param muster_union speichert die IDs der Gitterpunkte
     */
    void RANDOM(const int n, const int m, int k, GeneratorSeed &seed_samen)
    {
        int samen = 1; // Start Punkt oben links
        int ID = 1;
        muster_union[samen] = ID;
        int muster_count = 0;
        int neuer_clusterpkt;
        int zufall_index;
        int unzugewiesene_punkte = n * m;
        int durchschnitt;

        int limit = (n * m) / 2;

        while (muster_count < k && ID <= k)
        {
            // std::cout << "-----------------------------------------" << std::endl;
            // std::cout << "Iteration:" << muster_count << std::endl;
            // std::cout << "-----------------------------------------" << std::endl;

            std::set<int> set_samenCluster;
            set_samenCluster.insert(samen);
            std::set<int> set_nachbar;
            insert_set(set_nachbar, n, m, samen);

            // std::cout << "ID:" << ID << std::endl;
            // std::cout << "samen" << samen << std::endl;

            if (unzugewiesene_punkte - (k - muster_count) > 0)
            {
                durchschnitt = seed_samen.random_int_Intervall(1, limit); //unzugewiesene_punkte - (k - muster_count)
                if (durchschnitt > limit)
                {
                    durchschnitt = limit;
                }
                //  std::cout << "unzugewiesene_punkte - (k - muster_count) " << unzugewiesene_punkte - (k - muster_count) << std::endl;
                // if (ID == 1)
                // {
                //     durchschnitt = durchschnitt - 1;
                // }
                if (unzugewiesene_punkte - durchschnitt < k - muster_count) //k-1
                {
                    durchschnitt = 1;
                }
                muster_union[samen] = ID;
                unzugewiesene_punkte--;
            }
            else
            {
                durchschnitt = 1;
                muster_union[samen] = ID;
                unzugewiesene_punkte--;

                muster_union[samen] = ID;
            }

            // std::cout << "muster_count:" << muster_count << "  k:" << k << "  unzugewiesene_punkte:" << unzugewiesene_punkte << std::endl;
            // std::cout
            //     << "durchschnitt" << durchschnitt << " unzugewiesene_punkte - k" << unzugewiesene_punkte - k << std::endl;
            // std::cout << "Start insert_set" << set_nachbar.size() << std::endl;

            if (durchschnitt > 1 && set_nachbar.size() != 0)
            {
                for (int i = 1; i <= durchschnitt; i++)
                {
                    // std::cout << " set_nachbar.size()" << set_nachbar.size() << std::endl;
                    zufall_index = seed_samen.random_int() % (set_nachbar.size());
                    auto itt = set_nachbar.begin();
                    for (int i = 0; i < zufall_index; i++)
                    {
                        itt++;
                    }
                    set_nachbar.erase(itt);
                    neuer_clusterpkt = *itt;

                    set_samenCluster.insert(neuer_clusterpkt);

                    muster_union[neuer_clusterpkt] = ID;
                    unzugewiesene_punkte--;
                    insert_set(set_nachbar, n, m, neuer_clusterpkt);

                    if (set_nachbar.size() != 0)
                    {
                        zufall_index = seed_samen.random_int() % set_nachbar.size();
                        auto ittt = set_nachbar.begin();
                        for (int i = 0; i < zufall_index; i++)
                        {
                            ittt++;
                        }
                        samen = *ittt;
                    }
                    if (set_nachbar.size() == 0)
                    {
                        break;
                    }
                }
            }

            if (muster_count < k && ID < k)
            {
                int jo = find_next_NB(n, m, seed_samen);
                if (jo != INT_MAX)
                {
                    samen = jo;
                }
            }
            ID++;
            muster_count++;
            set_samenCluster.clear();
            set_nachbar.clear();
            // print_DatenModellCluster(n, m);
        }

        // print_DatenModellCluster(n, m);

        // std::cout << "cluster_definiton" << std::endl;
        // for (int i = 0; i < 9; i += 3)
        // {
        //     std::cout << cluster_definiton[i] << "  " << cluster_definiton[i + 1] << "  " << cluster_definiton[i + 2] << std::endl;
        // }

        //std::cout << "unzugewiesene_punkte" << unzugewiesene_punkte << std::endl;
        while (unzugewiesene_punkte > 0)
        {
            for (int i = 1; i <= n * m; i++)
            {
                if (muster_union[i] == 0)
                {
                    // std::cout << "i:" << i << ": ";
                    std::set<int> moeglicheNachbarn;
                    std::set<int> moeglicheCluster;
                    int help = i % n;

                    if (help != 0 && muster_union[i + 1] != 0)
                    {
                        // std::cout << i + 1 << " ";
                        moeglicheNachbarn.insert(i + 1);
                        moeglicheCluster.insert(muster_union[i + 1]);
                    }
                    if (help != 1 && muster_union[i - 1] != 0 && !moeglicheCluster.contains(muster_union[i - 1]))
                    {
                        // std::cout << i - 1 << " ";
                        moeglicheNachbarn.insert(i - 1);
                        moeglicheCluster.insert(muster_union[i - 1]);
                    }
                    if (i + n <= n * m && muster_union[i + n] != 0 && !moeglicheCluster.contains(muster_union[i + n]))
                    {
                        // std::cout << i + n << " ";
                        moeglicheNachbarn.insert(i + n);
                        moeglicheCluster.insert(muster_union[i + n]);
                    }
                    if ((int)i - n >= 1 && muster_union[i - n] != 0 && !moeglicheCluster.contains(muster_union[i - n]))
                    {
                        // std::cout << i - n << " ";
                        moeglicheNachbarn.insert(i - n);
                        moeglicheCluster.insert(muster_union[i - n]);
                    }
                    // std::cout << std::endl;
                    zufall_index = seed_samen.random_int() % moeglicheNachbarn.size();
                    // std::cout<<"rand()"<<rand()<<"  zufall_index"<<zufall_index<<std::endl;
                    auto ittt = moeglicheNachbarn.begin();
                    for (int i = 0; i < zufall_index; i++)
                    {
                        ittt++;
                    }
                    muster_union[i] = muster_union[*ittt];

                    // for (int x = 0; x < k; x++)
                    // {
                    //     if (cluster_info[x * 2] == muster_union[*ittt])
                    //     {
                    //         cluster_info[x * 2 + 1] += 1;
                    //         break;
                    //     }
                    // }
                    unzugewiesene_punkte--;
                    moeglicheNachbarn.clear();
                    moeglicheCluster.clear();
                }
            }
        }

        for (int i = 1; i <= k; i++)
        {
            int counter = 0;
            for (int j = 1; j <= n * m; j++)
            {
                if (muster_union[j] == i)
                {
                    counter++;
                }
            }
            cluster_info[(i - 1) * 2] = i;
            cluster_info[(i - 1) * 2 + 1] = counter;
        }

        if (!check_sumCluster(n, m, k))
        {
            std::cout << RED << "ERROR: "
                      << "Nicht alle Punkte zugewiesen." << RESET << std::endl;
        }

        print_DatenModellCluster(n, m);
        checkDatenmodell(n, m, k);
    }

    /**
     * @brief erstellt ein stark nicht zusammenhaengendes Clustering.
     * Fuer k=2 erhawhlt man ein typisches Schachbrett
     */
    void schachbrett(const int n, int k)
    {
        int id = 1;
        for (int i = 1; i <= n * n; i++)
        {
            if (id == muster_union[i - n] && i >= n)
            {
                id++;
                muster_union[i] = id;
            }
            else
            {
                muster_union[i] = id;
            }
            id++;
            if (k == n && i % n == 0)
            {
                id = muster_union[i - (n - 1)] + 1;
            }
            if (id > k)
            {
                id = 1;
            }
        }

        for (int i = 1; i <= k; i++)
        {
            int counter = 0;
            for (int j = 1; j <= n * n; j++)
            {
                if (muster_union[j] == i)
                {
                    counter++;
                }
            }
            cluster_info[(i - 1) * 2] = i;
            cluster_info[(i - 1) * 2 + 1] = counter;
        }

        print_DatenModellCluster(n, n);
    }

    std::string get_datenType()
    {
        return type_daten;
    }

    /**
     * @brief Clustering im Datenmodell
     */
    void print_DatenmodellName(const int data)
    {
        std::cout << "Daten: ";
        switch (data)
        {
        case 1:
            std::cout << "RECHTECKE ";
            break;
        case 2:
            std::cout << "RINGE ";
            break;
        case 3:
            std::cout << "STREIFEN ";
            break;
        case 4:
            std::cout << "RANDOM ";
            break;
        case 10:
            std::cout << "SCHACHBRETT ";
            break;
        default:
            break;
        }
        std::cout << "Generator ";
        std::cout << get_datenType();
        std::cout << " Zeitreihe mit Laenge:" << zeitreihenl << std::endl;
    }

    void check_anzahlMusterDOUBLE(const int k, const int *muster_union, const int n, const int m)
    {
        std::set<int> set;
        for (int j = 1; j <= n * m; j++)
        {
            if (!set.contains(muster_union[j]))
            {
                set.insert(muster_union[j]);
            }
        }
        if ((int)set.size() != k)
        {
            std::cerr << RED << "ERROR:" << RESET << std::endl;
            std::cout << "Anzahl an Cluster" << set.size() << " verabredet " << k << std::endl;
        }
        set.clear();
    }

    /**
     * @brief Steuermethode des Datenmodells. Erstellt Clustering und die zugehoerigen
     * Zeitreihen.
     */
    void erstelleDaten(const int n, const int m, int k, int *NachbarStationen, const int data, const double amplitude, GeneratorSeed &seed_samen)
    {
        for (int i = 0; i < (n * m) + 1; i++)
        {
            muster_union[i] = 0;
        }

        for (int i = 0; i < k * 2; i++)
        {
            cluster_info[i] = 0;
        }

        for (int i = 0; i < (n * m) * zeitreihenl; i++)
        {
            zeitreihenMatrix[i] = 0;
        }

        switch (data)
        {
        case 1:
            RECHTECKE(n, m, k);
            erstellenSynthetischeZeitreihen(zeitreihenMatrix, muster_union, n, m, k, zeitreihenl, get_datenType(), amplitude, seed_samen);
            break;
        case 2:
            RINGE(n, m, k);
            erstellenSynthetischeZeitreihen(zeitreihenMatrix, muster_union, n, m, k, zeitreihenl, get_datenType(), amplitude, seed_samen);
            break;
        case 3:
            STREIFEN(n, m, k);
            erstellenSynthetischeZeitreihen(zeitreihenMatrix, muster_union, n, m, k, zeitreihenl, get_datenType(), amplitude, seed_samen);
            break;
        case 4:
            RANDOM(n, m, k, seed_samen);
            erstellenSynthetischeZeitreihen(zeitreihenMatrix, muster_union, n, m, k, zeitreihenl, get_datenType(), amplitude, seed_samen);
            break;
        case 5:
        {
            einlesen_ExterneZeitreihenRANDOM(zeitreihenMatrix, n, m, zeitreihenl, "rlr_annual", get_ZeitreihenStartJahr(), seed_samen);
            break;
        }
        case 6:
        {
            einlesen_ExterneZeitreihenRANDOM(zeitreihenMatrix, n, m, zeitreihenl, "rlr_monthly", get_ZeitreihenStartJahr(), seed_samen);
            break;
        }
        case 7:
        {
            einlesen_ExterneZeitreihen(zeitreihenMatrix, NachbarStationen, n, zeitreihenl, "rlr_annual", get_ZeitreihenStartJahr());
            break;
        }
        case 8:
        {
            einlesen_ExterneZeitreihen(zeitreihenMatrix, NachbarStationen, n, zeitreihenl, "rlr_monthly", get_ZeitreihenStartJahr());
            break;
        }
        case 10:
        {
            schachbrett(n, k);
            erstellenSynthetischeZeitreihen(zeitreihenMatrix, muster_union, n, m, k, zeitreihenl, get_datenType(), amplitude, seed_samen);
            break;
        }
        default:
            std::cout << RED << "ERROR:" << RESET << "Diese Datenmodell gibt es nicht." << std::endl;
        }
        // std::cout << "Cluster info" << std::endl;
        // for (int j = 0; j < k; j++)
        // {
        //     std::cout << cluster_info[j * 2 + 0] << " " << cluster_info[j * 2 + 1] << std::endl;
        // }
        if (data != 5 && data != 6 && data != 7 && data != 8)
        {
            check_anzahlMusterDOUBLE(k, muster_union, n, m);
        }
        print_Zeitreihe(zeitreihenMatrix, zeitreihenl, n, m);
    }
    void erstelleDatenLaufzeit(const int n, const int m, int k, GeneratorSeed &seed_samen1, GeneratorSeed &seed_samen2)
    {
        for (int i = 0; i < (n * m) + 1; i++)
        {
            muster_union[i] = 0;
        }

        for (int i = 0; i < k * 2; i++)
        {
            cluster_info[i] = 0;
        }

        for (int i = 0; i < (n * m) * zeitreihenl; i++)
        {
            zeitreihenMatrix[i] = 0;
        }

        RANDOM(n, m, k, seed_samen1);
        erstellenSynthetischeZeitreihen(zeitreihenMatrix, muster_union, n, m, k, zeitreihenl, get_datenType(), (int)NULL, seed_samen2);

        print_Zeitreihe(zeitreihenMatrix, zeitreihenl, n, m);
    }

    /**
     * @brief print Zeitreihe.
     */
    void print_Zeitreihe_help(int zeitreihenl, const int n, const int m, const std::string str)
    {
        if (DEBUGG_optional)
        {
            std::cout << str << std::endl;
            for (int i = 0; i < n * m; i++)
            {
                for (int j = 0; j < zeitreihenl; j++)
                {
                    if (j == 0)
                    {
                        std::cout << std::setw(2) << i << ": ";
                    }
                    std::cout.precision(4);
                    std::cout << zeitreihenMatrix[i * zeitreihenl + j] << "  ";
                }
                std::cout << std::endl;
            }
        }
    }

    /**
     * @brief Methode zur Datenkkontrolle, schreibt Zeitreihen in eine Text Datei.
     * So kann man zum Beispiel testen, ob die Reproduzierbarkeit wie gefordert 
     * funktioniert.
     */
    void write_data(const int n, const int m, std::ofstream &offileCHECK)
    {
        for (int i = 0; i < n * m; i++)
        {
            for (int j = 0; j < get_Zeitreihenl(); j++)
            {
                if (j == 0)
                {
                    offileCHECK << std::setw(2) << i << ": ";
                }
                std::cout.precision(4);
                offileCHECK << zeitreihenMatrix[i * get_Zeitreihenl() + j] << "  ";
            }
            offileCHECK << "\n";
        }
    }

    /**
     * getter und setter methoden 
     */
    double get_Zeitreihenwert(const int pos)
    {
        return zeitreihenMatrix[pos];
    }

    void set_Zeitreihenwert(const int pos, const double wert)
    {
        zeitreihenMatrix[pos] = wert;
    }

    int get_musterunion(const int pos)
    {
        return muster_union[pos];
    }

    int get_clusterinfo(const int pos)
    {
        return cluster_info[pos];
    }
    int get_ZeitreihenStartJahr()
    {
        return zeitreihen_start_jahr;
    }
    int get_Zeitreihenl()
    {
        return zeitreihenl;
    }
    void clear()
    {
        free(zeitreihenMatrix);
        free(cluster_info);
        free(muster_union);
    }
};

std::string get_DatenmodellName(const int data)
{
    std::string str;
    switch (data)
    {
    case 1:
        str = "RECHTECKE";
        break;
    case 2:
        str = "RINGE";
        break;
    case 3:
        str = "STREIFEN";
        break;
    case 4:
        str = "RANDOM";
        break;
    case 5:
    case 7:
        str = "rlr_annual";
        break;
    case 6:
    case 8:
        str = "rlr_monthly";
        break;
    case 9:
        str = "unbekannt";
        break;
    case 10:
        str = "Schachbrett";
        break;
    case 11:
        str = "RandomWalk";
        break;
    default:
        str = "ERROR";
        break;
    }
    return str;
}

/**
 * @brief test Methode fuer das Datenmodell.cc
 */
int main_zumtesten_Datenmodell(bool seed)
{
    SEED = seed;
    GeneratorSeed seed_samen(0);
    DEBUGG_algo = true;
    CHECK = true;
    for (int i = 2; i <= 5; i++)
    {
        for (int j = 2; j <= 5; j++)
        {
            for (int l = 2; l < i * j; l++)
            {
                for (int z = 1; z <= 1; z += 25)
                {
                    const int n = 4;
                    const int m = 4;
                    const int k = i;
                    const int zeitreihenl = 1;
                    int data;
                    Datenmodell d(n * m, k, zeitreihenl, (int)NULL, "SINbekanntPurity");
                    std::cout << "n:" << n << " m:" << m << " k:" << k << std::endl;
                    std::cout << get_DatenmodellName(4) << std::endl;
                    d.erstelleDaten(n, m, k, nullptr, data = 4, 0.1, seed_samen);
                    std::cout << get_DatenmodellName(3) << std::endl;
                    d.erstelleDaten(n, m, k, nullptr, data = 3, 0.1, seed_samen);
                    std::cout << get_DatenmodellName(2) << std::endl;
                    d.erstelleDaten(n, m, k, nullptr, data = 2, 0.1, seed_samen);
                    std::cout << get_DatenmodellName(1) << std::endl;
                    d.erstelleDaten(m, n, k, nullptr, data = 1, 1, seed_samen);
                    std::cout << get_DatenmodellName(10) << std::endl;
                    d.erstelleDaten(m, n, k, nullptr, data = 10, 1, seed_samen);
                    d.clear();
                }
            }
        }
    }
    return 0;
}