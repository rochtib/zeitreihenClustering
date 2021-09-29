typedef adjacency_list<listS, vecS, bidirectionalS> ClusterGraph;
typedef graph_traits<ClusterGraph>::vertex_iterator vertex_iter;
typedef property<edge_weight_t, double> Weight;
typedef adjacency_list<listS, vecS, undirectedS, no_property, Weight> UndirectedGraph;

extern bool DEBUGG_optional;
extern bool DEBUGG_algo;
extern bool CHECK;

// Graphen  https://www.boost.org/doc/libs/1_63_0/libs/graph/doc/quick_tour.html
// https://www.boost.org/doc/libs/1_63_0/libs/graph/doc/adjacency_list.html#sec:iterator-constructor
// https://www.boost.org/doc/libs/1_54_0/libs/graph/doc/using_adjacency_list.html

/**
 * @brief Kopiert 1:1 einen Graphen
 */
void erstelleGitterVonGraph(UndirectedGraph &undigraph, UndirectedGraph &G_direkteNachbarn, const int n)
{
    typedef typename UndirectedGraph::edge_property_type Weight;
    typename graph_traits<UndirectedGraph>::out_edge_iterator ei, ei_end;
    typename boost::graph_traits<UndirectedGraph>::edge_descriptor edgee;
    typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G_direkteNachbarn);
    bool found;

    zeitMessen_start();
    for (int i = 1; i <= n; i++)
    {
        for (boost::tie(ei, ei_end) = out_edges(i, G_direkteNachbarn); ei != ei_end; ++ei)
        {
            auto target = boost::target(*ei, undigraph);
            auto source = boost::source(*ei, undigraph);
            double value = EdgeWeightMapNachbar[*ei];
            tie(edgee, found) = edge(source, target, undigraph);
            if (!found)
            {
                add_edge(source, target, Weight(value), undigraph);
            }
        }
    }
    zeitMessen_ende();
}

/**
 * @brief Erstellt die Adjazensliste der Cluster. Am Anfang ist jeder Punkt allein im Cluster.
 * Mit jeder Iteration werden 2 Cluster vereinigt. Der Graph G einen gerichten Graph.
 * Die Wurzel bildet die ID des Clusters und alle Knoten des gleichen Clusters sind zu der Cluster ID
 * adjazent.
 */
void erstelleClusterGraph(ClusterGraph &G, const int n, const int m)
{
    zeitMessen_start();
    for (int i = 1; i <= n * m; i++)
    {
        add_edge(i, i, G);
    }
    zeitMessen_ende();
}

/**
 * @brief In der nicht agglomerativen  Methode wird ohne den ClusterGraph gearbeitet. 
 * Hier wird der ClusterGraph aus dem enstprechenden Array erstellt.
 */
void erstelleClusterGraphSL(ClusterGraph &G, int *clusterBerechnetFeld, const int n, const int m, const std::vector<int> component)
{
    for (int i = 0; i <= n * m; i++)
    {
        clusterBerechnetFeld[i] = component[i];
    }

    std::set<int> hilfsmenge;
    for (int i = 1; i <= n * m; i++)
    {
        if (!hilfsmenge.contains(i))
        {
            hilfsmenge.insert(i);
            for (int j = i; j <= n * m; j++)
            {
                if (clusterBerechnetFeld[j] == i)
                {
                    add_edge(i, j, G);
                }
            }
        }
    }
}

/**
 * @brief Gibt den ClusterGraph auf der Konsole aus. Dabei werden nur Knoten ausgegeben
 * die nicht isoliert sind. 
 */
void printClusterGraph(ClusterGraph &G, const int n, const int m)
{
    if (DEBUGG_algo)
    {
        std::cout << "ClusterGraph:" << std::endl;
        int count;
        for (int i = 0; i <= n * m; i++)
        {
            auto nb = boost::adjacent_vertices(i, G);
            count = 0;

            int help;
            for (const auto vd : make_iterator_range(nb))
            {
                help = vd; //um die warning zu umgehen
                count++;
            }
            if (count > 0)
            {
                std::cout << setw(3) << i << " --> ";
                for (auto vd : make_iterator_range(nb))
                {
                    std::cout << vd << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}

/**
 * @brief Vereinigt Adjazenslisten bzw die Punkte eines Clusters. Das Cluster mit der 
 * kleineren ID wird mit den Punkten des groesseren Clusters erweitert.
 * 
 * @param G ClusterGraph. Jeder ClusterID ist ZHK. Die Knoten des Clusters sind adjazent zur ClusterID.
 * @param cl1 ClusterID eins
 * @param cl2 clusterID zwei
 */
void vereingeClusterGraph(ClusterGraph &G, const int cl1, const int cl2)
{
    auto nb2 = boost::adjacent_vertices(cl2, G);
    for (auto vd : make_iterator_range(nb2))
    {
        if ((int)vd != cl1)
        {
            add_edge(cl1, vd, G);
        }
    }
    /**
     * Loescht alle Kanten des Knoten knoten_zu_entfernen.
     * Wenn man den Knoten aus dem Graph loescht aendert sich die Nummerierung der Knoten. Dann muss man
     * den Knoten ueber property name einen Namen geben.
     */
    clear_out_edges(cl2, G);
}

/**
 * @brief Gibt die Knoten des ClusterGraph auf der Konsole aus.
 */
void printClusterGraphKnoten(ClusterGraph &G)
{
    std::cout << "Anzahlvertices(G): " << num_vertices(G) << std::endl;
    std::cout << "Output Knoten" << std::endl;

    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(G); vp.first != vp.second; ++vp.first)
    {
        std::cout << *vp.first << " ";
    }
    std::cout << std::endl;
}

/**
 * @brief Gibt die Knoten eines Ungerichteten Graph auf der Konsole aus.
 */
void printUngerichtetGraphKnoten(UndirectedGraph &G)
{
    std::cout << "Anzahlvertices(G): " << num_vertices(G) << std::endl;
    std::cout << "Output Knoten" << std::endl;

    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(G); vp.first != vp.second; ++vp.first)
    {
        std::cout << *vp.first << " ";
    }
    std::cout << std::endl;
}

/**
 * @brief Der ClusterAlgorithmus erstellt ein Algorithmus mit k Clustern. 
 * Die Cluster sind aber nicht von 1,2,..,k nummeriert. 
 * Das Array wird umnummeriert.
 */
void renummeriereClusterArray(int *renummeriert, const int n, const int m, const int k)
{
    /**
     * temporaeres Array zum Speichern der noch freien nicht vergebenen ClusterIDs.
     */
    int *work;
    work = (int *)malloc((k + 1) * sizeof(int *));

    std::set<int> set;
    for (int i = 0; i <= k; i++)
    {
        work[i] = 0;
    }

    for (int i = 1; i <= n * m; i++)
    {
        int a = renummeriert[i];
        if (a > k)
        {
            int index;
            for (int j = 1; j <= k; j++)
            {
                if (work[j] != INT_MAX)
                {
                    work[j] = INT_MAX;
                    index = j;
                    break;
                }
            }
            for (int j = 1; j <= n * m; j++)
            {
                if (renummeriert[j] == a)
                {
                    renummeriert[j] = index;
                }
            }
        }
        else
        {
            work[a] = INT_MAX;
        }
    }
    free(work);
}

/**
 * @brief Gibt farbig links das Clustering vom Datenmodell aus und rechts das 
 * berechnete Clustering vom Algorithmus.
 */
void printGitterMitArray(Datenmodell d, int *gitterFeld, const int n, const int m, const int k)
{
    if (n * m <= 15 * 15)
    {
        int *work;
        work = (int *)malloc((n * m + 1) * sizeof(int *));

        for (int i = 0; i <= n * m; i++)
        {
            work[i] = gitterFeld[i];
        }
        renummeriereClusterArray(work, n, m, k);

        std::cout << setw((n * 4) - 1) << "MODELL";
        std::cout << "     ";
        std::cout << setw((n * 4) - 1) << "ALGO" << std::endl;
        for (int j = 0; j < m; j++)
        {
            for (int i = 1; i <= n; i++)
            {
                /**
                 * @source https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
                 * @source https://medium.com/@vitorcosta.matias/print-coloured-texts-in-console-a0db6f589138
                 * printf(farbedefinition, Hintergrundfarbe                  , text)
                 */
                printf("\033[48;5;%dm %3d\033[m", d.get_musterunion(j * n + i) % k, d.get_musterunion(j * n + i));
                // std::cout << std::setw(3) << d.get_musterunion(j * n + i) << " "; //OHNE FARBE
            }
            std::cout << " || ";
            for (int i = 1; i <= n; i++)
            {
                printf("\033[48;5;%dm %3d\033[m", work[j * n + i], gitterFeld[j * n + i]);
                //std::cout << std::setw(3) << gitterFeld[j * n + i] << " "; //OHNE FARBE
            }
            std::cout << std::endl;
        }
        free(work);
    }
}

/**
 * @brief Zeichnet in Farbe die Cluster mit ihren IDs.
 */
void printGitterArray(int *gitterFeld, const int n, const int m, const int k)
{
    if (n * m <= 25 * 25)
    {
        std::cout << "Berechnetes Clustergitter:" << std::endl;
        for (int j = 0; j < m; j++)
        {
            for (int i = 1; i <= n; i++)
            {
                std::setw(3);
                printf("\033[48;5;%dm %3d\033[m", gitterFeld[j * n + i] % k, gitterFeld[j * n + i]);
                // std::cout << std::setw(3) << gitterFeld[j * n + i] << " ";
            }
            std::cout << std::endl;
        }
    }
}

/**
 * @brief Zeichnet das auf einem Schritt das bereits berechnete Clustering
 */
void printGitterMitArray(int *gitterFeld, const int n, const int m, const int aktuell_k)
{
    int k = aktuell_k;
    if (n * m <= 25 * 25)
    {
        int *work;
        work = (int *)malloc((n * m) * sizeof(int *));

        for (int i = 0; i < n * m; i++)
        {
            work[i + 1] = gitterFeld[i + 1];
        }
        renummeriereClusterArray(work, n, m, k);
        std::cout << "Berechnetes Clustergitter:" << std::endl;
        for (int j = 0; j < m; j++)
        {
            for (int i = 0; i < n; i++)
            {
                std::setw(3);
                printf("\033[48;5;%dm %3d\033[m", work[j * n + i + 1] % k, gitterFeld[j * n + i + 1]);
                // std::cout << std::setw(3) << gitterFeld[j * n + i] << " ";
            }
            std::cout << std::endl;
        }
        free(work);
    }
}

void printGitterMitArray2(int *gitterFeld, const int n, const int m, const int aktuell_k)
{
    int k = aktuell_k;
    if (n * m <= 25 * 25)
    {
        int *work;
        work = (int *)malloc((n * m) * sizeof(int *));

        for (int i = 0; i < n * m; i++)
        {
            work[i + 1] = gitterFeld[i];
        }
        renummeriereClusterArray(work, n, m, k);

        std::cout << "Berechnetes Clustergitter:" << std::endl;
        for (int j = 0; j < m; j++)
        {
            for (int i = 0; i < n; i++)
            {
                std::setw(3);
                printf("\033[48;5;%dm %3d\033[m", work[j * n + i + 1] % k, gitterFeld[j * n + i]);
                // std::cout << std::setw(3) << gitterFeld[j * n + i] << " ";
            }
            std::cout << std::endl;
        }
        free(work);
    }
}

/**
 * @brief Aus dem ClusterGraph wird ein Array erstellt. Jeder Knoten hat eine ID von 1 bis n*m. Jeder
 * index des Clusters enstpricht genau seiner Knoten Nummer. In jedem Feld des Array steht die ClusterId 
 * des Knotens.
 */
// template <typename ClusterGraph>
void erstelleArrayAusClusterGraph(ClusterGraph &G, int *gitterFeld, const int n, const int m)
{
    int count;
    for (int i = 1; i <= n * m; i++)
    {
        auto nb = boost::adjacent_vertices(i, G);
        count = 0;

        int help;
        for (const auto vd : make_iterator_range(nb))
        {
            help = vd; //um die warning zu umgehen
            count++;
        }
        if (count > 0)
        {
            int t;
            for (auto vd : make_iterator_range(nb))
            {
                t = (int)vd;
                gitterFeld[t] = i;
            }
        }
    }
    // for (int i = 1; i <= n * m; i++)
    // {
    //     std::cout << "gitterFeld[i]:" << gitterFeld[i] << std::endl;
    // }
}

/**
 * @brief Zeichnet das Clustering vom Modell und Algo entweder nebeneinander oder untereinander.
 */
void printVergleichGitterModell(Datenmodell d, int *gitterFeld, const int n, const int m, const int k, const int data)
{
    if (data < 5 && CHECK && DEBUGG_algo)
    {
        if (n > 15)
        {
            printGitterMitArray(gitterFeld, n, m, k);
            d.print_DatenModellCluster(n, m);
        }
        else
        {
            printGitterMitArray(d, gitterFeld, n, m, k);
        }
    }
    else if (CHECK && DEBUGG_algo)
    {
        printGitterMitArray(gitterFeld, n, m, k);
    }
}

/**
* @brief Erhalte aus dem Cluster des Knoten return_knoten, den Gitterpunkt mit kleinste Distanz
* zu dem naechsten Cluster. Dies enstricht einer Kante mit Quelle ClusterID. Die ausgehende
* Kante mit kleinstem Gewicht wird der naehste Nachbar.
* Solange k>1 gibt es immer eine Distanz zu einem anderen Cluster.
* 
* @param G Graph wobei jede Zusammenhangskomponente ein Cluster mit seinen Nachbarn repraesentiert.
* @param u Schluessel um auf die UnionFind Methoden zuzugreifen
* @param return_knoten Gitterpunkt des naehsten Clusters
* @param return_distanz die Distanz zum naechsten Cluster
*/
void erhalteMinimumNachbarGraph(UndirectedGraph &G, size_t &return_knoten, double &return_distanz)
{
    double min_distanz = INT_MAX;
    typename property_map<UndirectedGraph, edge_weight_t>::type weight = get(edge_weight, G);
    typename graph_traits<UndirectedGraph>::out_edge_iterator ei, ei_end;

    if (DEBUGG_optional)
    {
        std::cout << "SucheUpdate min in CLuster " << return_knoten << "." << std::endl;
        std::cout << "Das Cluster " << return_knoten << " hat ";
    }

    for (boost::tie(ei, ei_end) = out_edges(return_knoten, G); ei != ei_end; ++ei)
    {
        // if (DEBUGG_optional)
        // {
        //     auto target = boost::target(*ei, G);
        //     std::cout << "NachbarClusterID: " << target << " Abstand: " << weight[*ei] << " mit";
        //     auto source = boost::source(*ei, G);
        //     std::cout << " (ClusterID: " << source << ") || ";
        // }
        if (weight[*ei] < min_distanz)
        {
            min_distanz = weight[*ei];
            return_knoten = boost::target(*ei, G);
        }
    }
    if (DEBUGG_optional)
    {
        std::cout << "als naehste NachbarCluster ist " << return_knoten << " mit Abstand: " << min_distanz << std::endl;
    }
    return_distanz = min_distanz;
}

/**
 * @brief Vereinigt werden die Nachbarcluster. Sei cl1>cl2. Dann wird ueber alle Kanten (cl1,N)
 * iterariert und geschaut, ob (cl2,N) noch nicht existiert, dann wird die Kante hinzugefuegt.
 * Alle Kanten (cl1,N) werden dann geloescht.
 * 
 * @param G NachbarGraph. Jede clusterID hat ausgehende Kanten zu den direkten Nachbarclustern.
 * Jede dieser Kante hat ein Gewicht des Abstands der beiden CLuster.
 * @param cl1 ClusterID des ersten Clusters das vereinigt werden soll
 * @param cl2 ClusterID des zweiten Clusters das vereinigt werden soll
 */
void verschmelzeNachbarGraphCL(UndirectedGraph &G, const int cl1, const int cl2, MinHeap &h, const int MODUS)
{
    typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G);
    typename boost::graph_traits<UndirectedGraph>::edge_descriptor edgee;
    typename graph_traits<UndirectedGraph>::out_edge_iterator e, e_end;
    bool found;
    std::set<int> set_heap;
    for (boost::tie(e, e_end) = out_edges(cl2, G); e != e_end; ++e)
    {
        // std::cout << "target(*e, G)" << target(*e, G) << std::endl;
        tie(edgee, found) = edge(target(*e, G), cl1, G);

        if ((int)target(*e, G) != cl1 && (int)target(*e, G) != cl2)
        {
            int pos_heap = h.get_map(target(*e, G)) - 1;

            int benachbartesCluster = h.get_heap_Gitterpunkt(pos_heap);

            if (benachbartesCluster == cl2)
            {
                h.set_heap_Gitterpunkt(pos_heap, cl1);
            }
        }
        if (!found)
        {
            if ((int)target(*e, G) != cl1 && (int)target(*e, G) != cl2)
            {
                add_edge(cl1, target(*e, G), EdgeWeightMapNachbar[*e], G);
            }
        }
        else if (MODUS == 5)
        {
            // std::cout << " EdgeWeightMapNachbar[edgee]" << EdgeWeightMapNachbar[edgee] << std::endl;
            // std::cout << " EdgeWeightMapNachbar[*e]" << EdgeWeightMapNachbar[*e] << std::endl;
            double min = std::min(EdgeWeightMapNachbar[edgee], EdgeWeightMapNachbar[*e]);
            if (EdgeWeightMapNachbar[edgee] != min)
            {
                EdgeWeightMapNachbar[edgee] = min;
            }
        }
        else if (MODUS == 6)
        {
            // std::cout << " EdgeWeightMapNachbar[edgee]" << EdgeWeightMapNachbar[edgee] << std::endl;
            // std::cout << " EdgeWeightMapNachbar[*e]" << EdgeWeightMapNachbar[*e] << std::endl;
            double max = std::max(EdgeWeightMapNachbar[edgee], EdgeWeightMapNachbar[*e]);
            if (EdgeWeightMapNachbar[edgee] != max)
            {
                EdgeWeightMapNachbar[edgee] = max;
            }
        }
    }
    clear_vertex(cl2, G);

    for (boost::tie(e, e_end) = out_edges(cl1, G); e != e_end; ++e)
    {
        size_t test_knoten = target(*e, G);
        if ((int)test_knoten != cl1 && (int)test_knoten != cl2)
        {
            double test_distanz;
            erhalteMinimumNachbarGraph(G, test_knoten, test_distanz);

            // std::cout << " target(*e, G):" << target(*e, G) << "  test_knoten:" << test_knoten << " test_distanz:" << test_distanz << "  "<<std::endl;
            // int pos_heap = h.get_map(target(*e, G));
            // h.deleteKey(pos_heap - 1);

            // h.insert(test_distanz, target(*e, G), test_knoten);

            int pos_heap = h.get_map(target(*e, G)) - 1;
            double alt = h.get_heap_gewichte(pos_heap);
            h.set_heap_gewichte(pos_heap, test_distanz);
            h.set_heap_Gitterpunkt(pos_heap, test_knoten);
            if (alt > test_distanz && pos_heap != 0)
            {
                h.BubbleUp(pos_heap);
            }
            else
            {
                h.BubbleDown(pos_heap);
            }
        }
    }
}

/**
 * @brief Aktualisiert die Nachbarcluster.
 */
// template <typename UndirectedGraph>
void verschmelzeNachbarGraph(UndirectedGraph &G, const int cl1, const int cl2, MinHeap &h)
{
    typename boost::graph_traits<UndirectedGraph>::edge_descriptor edgee;
    typename graph_traits<UndirectedGraph>::out_edge_iterator e, e_end;
    bool found;

    for (boost::tie(e, e_end) = out_edges(cl2, G); e != e_end; ++e)
    {
        int pos_heap = h.get_map(target(*e, G));

        int gitter_heap = h.get_heap_Gitterpunkt(pos_heap - 1);
        if (gitter_heap == cl2)
        {
            h.set_heap_Gitterpunkt(pos_heap - 1, cl1);
        }
        tie(edgee, found) = edge(cl1, target(*e, G), G);
        if (!found && (int)target(*e, G) != cl1)
        {
            add_edge(cl1, target(*e, G), INT_MAX, G);
        }
    }
    clear_vertex(cl2, G);
}

/**
 * @brief Zeichnet den Ungerichten Graphen auf der Konsole. Nach folgenden Schema: Zeichne alle ausgehenden
 * Kanten von einem Knoten mit dem gewicht der Kante.
 */
// template <typename UndirectedGraph>
void printUngerichteterGraph(UndirectedGraph &G, const int n, const int m, const std::string str)
{
    if (DEBUGG_algo)
    {
        typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G);

        size_t a = 0;
        bool test = true;
        std::cout << str;
        for (int i = 0; i <= n * m; i++)
        {
            typename graph_traits<UndirectedGraph>::out_edge_iterator ei, ei_end;
            for (boost::tie(ei, ei_end) = out_edges(i, G); ei != ei_end; ++ei)
            {
                if (a != source(*ei, G))
                {
                    std::cout << std::endl;
                    std::cout << setw(3) << source(*ei, G) << ": ";
                    test = false;
                    a = source(*ei, G);
                }
                std::cout << " (" << target(*ei, G);
                std::cout << "," << std::setprecision(4) << EdgeWeightMapNachbar[*ei] << ") ";
            }
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Es werden die gemeinesamen Nachbarn des Clusters aktualisiert. Die gemeinsamen Nachbarn 
 * des Clusters sind zu beiden adjazent. Damit das Cluster nicht doppelt oder mehrfach auftritt
 * wird ein gemeinsamer Nachbar geloescht.
 * 
 * @param G NachbarGraph. Jede clusterID hat ausgehende Kanten zu den direkten Nachbarclustern.
 * Jede dieser Kante hat ein Gewicht des Abstands der beiden CLuster.
 * @param u Schluessel des fuer UnionFind Strucktur
 * @param nach 
 * @param gitterKnotenID 
 * @source https://www.boost.org/doc/libs/1_60_0/libs/graph/example/undirected_adjacency_list.cpp
 * @source https://www.boost.org/doc/libs/1_60_0/libs/graph/doc/graph_concepts.html
 */
// template <typename UndirectedGraph>
void erstelleGitterGraph(UndirectedGraph &undigraph, const double *direkte4Nachbarn, const int n, const int m)
{
    typedef typename UndirectedGraph::edge_property_type Weight;
    double value = 0;
    int help;

    zeitMessen_start();
    for (int i = 1; i <= n * m; i++)
    {
        help = i % n;

        if (help != 0)
        {
            value = direkte4Nachbarn[(i - 1) * 4 + RECHTS];
            add_edge(i, i + 1, Weight(value), undigraph);
        }
        if (i + n <= n * m)
        {
            value = direkte4Nachbarn[(i - 1) * 4 + UNTEN];
            add_edge(i, i + n, Weight(value), undigraph);
        }
    }
    zeitMessen_ende();
}

/**
 * @brief Berechnet alle Frechet Distanzen und der vollstaendige Graph wird aufgebaut.
 */
// template <typename UndirectedGraph>
void kompletterGraph(UndirectedGraph &digraph, Datenmodell &d, const int auswahl, const int zeitreihenl)
{
    double abstand;
    // const int konstant = 2 * auswahl;
    // double prozent = 0.25;
    // std::cout << "Frechet Distanzen Status: || ";

    zeitMessen_start();
    for (int i = 1; i <= auswahl; i++)
    {
        for (int j = i + 1; j <= auswahl; j++)
        {
            // if ((double)count / konstant >= prozent)
            // {
            //     std::cout << prozent * 100 << "% || ";
            //     prozent = prozent + 0.25;
            // }
            abstand = callcFrechet(d, i - 1, j - 1, zeitreihenl);
            // std::cout << i << " nach " << j << " mit abstand " << abstand << std::endl;
            add_edge(i, j, Weight(abstand), digraph);
        }
    }
    zeitMessen_ende();

    if ((int)num_edges(digraph) != auswahl * (auswahl - 1) * 0.5 || (int)num_vertices(digraph) != auswahl + 1)
    {
        std::cout << RED << "ERROR:" << RESET << " Die Anzahl Kanten passt nicht!" << std::endl;
        std::cout << "Es gibt " << auswahl << " Knoten." << std::endl;
        std::cout << "num_vertices(G)" << num_vertices(digraph) << std::endl;
        std::cout << "num_edges(G)" << num_edges(digraph) << std::endl;
    }
}

/**
 * @brief Baut vollstaendigen Graphen auf. Es werden alle Distanzen berechnet. Bereits
 * berechnete Distanzen werden nicht neu berechnet sondern aus dem Gittergraph gelesen.
 */
// template <typename UndirectedGraph>
void erstelleCompleteGraphVonGraph(UndirectedGraph &G_voll, UndirectedGraph &G_Gitter, Datenmodell &d, const int auswahl, const int zeitreihenl)
{
    typename boost::graph_traits<UndirectedGraph>::edge_descriptor edgee;
    typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G_Gitter);
    bool found;
    double abstand;

    // zeitMessen_start();
    for (int i = 1; i <= auswahl; i++)
    {
        for (int j = 1; j <= auswahl; j++)
        {
            if (i != j)
            {
                tie(edgee, found) = edge(i, j, G_voll);
                if (!found)
                {
                    tie(edgee, found) = edge(i, j, G_Gitter);
                    if (found)
                    {
                        abstand = EdgeWeightMapNachbar[edgee];
                    }
                    else
                    {
                        abstand = callcFrechet(d, i - 1, j - 1, zeitreihenl);
                    }
                    add_edge(i, j, abstand, G_voll);
                }
            }
        }
    }
    // zeitMessenFrechet_ende();

    if ((int)num_edges(G_voll) != auswahl * (auswahl - 1) * 0.5 || (int)num_vertices(G_voll) != auswahl + 1)
    {
        std::cout << RED << "ERROR:" << RESET << " Die Anzahl Kanten passt nicht!" << std::endl;
        std::cout << "Es gibt " << auswahl << " Knoten." << std::endl;
        std::cout << "num_vertices(G)" << num_vertices(G_voll) << std::endl;
        std::cout << "num_edges(G)" << num_edges(G_voll) << std::endl;
    }
}

/**
 * @brief Checkt ob eine Kante im Graph ist damit die Kante nicht doppelt vorkommt
 * @return true: existiert
 * @return false; existiert nicht
 */
bool exist_edge(UndirectedGraph &digraph, const int knoten, const int NACH)
{
    boost::graph_traits<UndirectedGraph>::out_edge_iterator e, e_end;
    boost::graph_traits<UndirectedGraph>::vertex_descriptor
        s = vertex(knoten, digraph);

    for (tie(e, e_end) = out_edges(s, digraph); e != e_end; ++e)
    {
        if ((int)source(*e, digraph) == NACH || (int)target(*e, digraph) == NACH)
        {
            return false;
        }
    }
    return true;
}

/**
 * @brief Baut den GitterGraph fuer RealWorld Daten.
 */
// template <typename UndirectedGraph>
void initialerAbstandsClusterGraphGitter(UndirectedGraph &digraph, Datenmodell &d, int *NachBarStationen, const int auswahl, const int zeitreihenl)
{
    double abstand = INT_MAX;
    bool found;
    zeitMessen_start();
    for (int i = 0; i < auswahl; i++)
    {
        int v = NachBarStationen[i * 6 + 1];
        // std::cout << "v" << v << std::endl;
        // std::cout << "NachBarStationen[i * 6 + 2]" << NachBarStationen[i * 6 + 2] << std::endl;
        found = exist_edge(digraph, v, NachBarStationen[i * 6 + 2]);
        // std::cout << "?? " << v << " " << NachBarStationen[i * 6 + 2] << std::endl;
        if (found)
        {
            abstand = callcFrechet(d, v - 1, NachBarStationen[i * 6 + 2] - 1, zeitreihenl);
            // std::cout << "insert" << std::endl;
            add_edge(v, NachBarStationen[i * 6 + 2], Weight(abstand), digraph);
        }

        // std::cout << "NachBarStationen[i * 6 + 3]" << NachBarStationen[i * 6 + 3] << std::endl;
        found = exist_edge(digraph, v, NachBarStationen[i * 6 + 3]);
        // std::cout << "?? " << v << " " << NachBarStationen[i * 6 + 3] << std::endl;
        if (found)
        {
            abstand = callcFrechet(d, v - 1, NachBarStationen[i * 6 + 3] - 1, zeitreihenl);
            add_edge(v, NachBarStationen[i * 6 + 3], (abstand), digraph);
            // std::cout << "insert" << std::endl;
        }

        // std::cout << "NachBarStationen[i * 6 + 4]" << NachBarStationen[i * 6 + 4] << std::endl;
        found = exist_edge(digraph, v, NachBarStationen[i * 6 + 4]);
        // std::cout << "?? " << v << " " << NachBarStationen[i * 6 + 4] << std::endl;
        if (found)
        {
            abstand = callcFrechet(d, v - 1, NachBarStationen[i * 6 + 4] - 1, zeitreihenl);
            add_edge(v, NachBarStationen[i * 6 + 4], (abstand), digraph);
            // std::cout << "insert" << std::endl;
        }

        // std::cout << "NachBarStationen[i * 6 + 5]" << NachBarStationen[i * 6 + 5] << std::endl;
        found = exist_edge(digraph, v, NachBarStationen[i * 6 + 5]);
        // std::cout << "?? " << v << " " << NachBarStationen[i * 6 + 5] << std::endl;
        if (found)
        {
            abstand = callcFrechet(d, v - 1, NachBarStationen[i * 6 + 5] - 1, zeitreihenl);
            add_edge(v, NachBarStationen[i * 6 + 5], (abstand), digraph);
            // std::cout << "insert" << std::endl;
        }
    }

    std::vector<int> component(auswahl + 1);

    int num = connected_components(digraph, &component[0]);
    // for (int i = 0; i <= auswahl; i++)
    // {
    //     std::cout << "Vertex " << i << " is in component " << component[i] << std::endl;
    // }
    // std::cout << "initialerAbstandsClusterGraph:" << num << std::endl;
    if (num > 2)
    {
        std::cout << RED << "ACHTUNG:" << RESET << " Der Graph ist nicht zusammenhaengend.";
        std::cout << "Folgende Kanten kommen hinzu:" << std::endl;
        std::set<int> set;
        set.insert(1);
        for (int i = 1; i <= auswahl; i++)
        {
            if (!set.contains(component[i]))
            {
                abstand = callcFrechet(d, 0, i - 1, zeitreihenl);
                std::cout << 1 << " nach " << i << " mit Abstand " << abstand << std::endl;
                set.insert(component[i]);
                add_edge(1, i, abstand, digraph);
            }
        }
        set.clear();
    }
    zeitMessen_ende();
}

/**
 * @brief Baut den Gittergraph, berechnet alle Frechet Distanzen.
 */
void initialerGitterGraph(UndirectedGraph &digraph, Datenmodell &d, const int n, const int m, const int zeitreihenl)
{
    double abstand = 1.5;
    int help;
    // int count = 0;
    // const int konstant = 2 * n * m - m - n;
    // double prozent = 0.25;
    // std::cout << "Frechet Distanzen Status: || ";
    zeitMessen_start();
    for (int i = 1; i <= n * m; i++)
    {
        help = i - 1;
        // if ((double)count / konstant >= prozent)
        // {
        //     std::cout << prozent * 100 << "% || ";
        //     prozent = prozent + 0.25;
        // }
        if (i % n != 0)
        {
            abstand = callcFrechet(d, help, help + 1, zeitreihenl);
            add_edge(i, i + 1, abstand, digraph);
        }
        if (i + n <= n * m)
        {
            abstand = callcFrechet(d, help, help + n, zeitreihenl);
            add_edge(i, i + n, abstand, digraph);
        }
    }
    zeitMessen_ende();
    // std::cout << "Abgeschlossen. " << std::endl;
}

/**
 * @brief Methode zum Schrittweiten verstehen des Algroithmus. Zeichnet das 
 * aktuelle Clustering im Gitter farbig in der Konsole.
 */
void printClusterGraphAlsGitterMitAbstand(ClusterGraph &G, UndirectedGraph &G_nachbar, const int n, const int m, std::string s, const int MODUS, const bool ausgabe_clusterID, const int aktuell_k)
{
    if (DEBUGG_algo)
    {
        int *gitter;
        gitter = (int *)malloc((n * m) * sizeof(int *));
        std::cout << s << std::endl;
        std::cout << "______________________________________________________________________________________________________________________" << std::endl;
        std::cout << std::endl;
        for (int i = 1; i <= n * m; i++)
        {
            graph_traits<ClusterGraph>::in_edge_iterator e2, e_end2;
            boost::graph_traits<ClusterGraph>::vertex_descriptor s = vertex(i, G);

            int help = INT_MAX;
            for (boost::tie(e2, e_end2) = in_edges(s, G); e2 != e_end2; ++e2)
            {
                if (help > (int)source(*e2, G))
                {
                    help = source(*e2, G);
                }
            }
            gitter[i - 1] = help;
        }

        if (n <= 6 && m <= 6)
        {
            typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G_nachbar);
            boost::graph_traits<UndirectedGraph>::edge_descriptor edgee;
            graph_traits<UndirectedGraph>::out_edge_iterator e, e_end;

            float dumm_gew;
            std::string str;
            int runden = 1000000;
            for (int j = 0; j < m; j++)
            {
                for (int i = 0; i < n; i++)
                {
                    int a = gitter[j * n + i];
                    if (i == 0)
                    {
                        if (ausgabe_clusterID)
                        {
                            std::cout << "       ";
                            printf("\033[48;5;%dm %3d\033[m", a, a);
                        }
                        else
                        {
                            std::cout << "       ";
                            printf("\033[48;5;%dm %3d\033[m", a, j * n + i + 1);
                        }
                    }
                    else
                    {
                        if (ausgabe_clusterID)
                        {
                            std::cout << " ";
                            printf("\033[48;5;%dm %3d\033[m", a, a);
                        }
                        else
                        {
                            std::cout << " ";
                            printf("\033[48;5;%dm %3d\033[m", a, j * n + i + 1);
                        }
                    }
                    if (i + 1 < n && MODUS != 5 && MODUS != 6)
                    {
                        int b = gitter[j * n + i + 1];
                        double gewicht;

                        if (a != b)
                        {
                            for (boost::tie(e, e_end) = out_edges(a, G_nachbar); e != e_end; ++e)
                            {
                                if ((int)target(*e, G) == b)
                                {
                                    gewicht = EdgeWeightMapNachbar[*e];
                                }
                            }
                            dumm_gew = floorf(gewicht * runden) / runden;
                            str = "[" + to_string(dumm_gew) + "]";

                            std::cout << std::setw(13) << str;
                            std::cout << "  ";
                        }
                        else if (MODUS != 5 && MODUS != 6)
                        {
                            std::cout << std::setw(13) << "   ----   ";
                            std::cout << "  ";
                        }
                    }
                }
                std::cout << std::endl;

                for (int i = 0; i < n; i++)
                {
                    if (j * n + i + 1 + n <= n * m)
                    {
                        int aa;
                        int bb;
                        int aa_d;
                        int bb_d;
                        if (MODUS == 1)
                        {
                            aa = gitter[j * n + i];
                            bb = gitter[j * n + i + n];
                            aa_d = j * n + i + 1;
                            bb_d = j * n + i + n + 1;
                        }
                        else
                        {
                            aa = gitter[j * n + i];
                            bb = gitter[j * n + i + n];
                            aa_d = gitter[j * n + i];
                            bb_d = gitter[j * n + i + n];
                        }
                        double gewicht;

                        if (aa != bb && MODUS != 5 && MODUS != 6)
                        {
                            for (boost::tie(e, e_end) = out_edges(aa_d, G_nachbar); e != e_end; ++e)
                            {
                                if ((int)target(*e, G) == bb_d)
                                {
                                    gewicht = EdgeWeightMapNachbar[*e];
                                }
                            }
                            dumm_gew = floorf(gewicht * runden) / runden;
                            str = "[" + to_string(dumm_gew) + "]";
                            std::cout << "" << std::setw(13) << str;
                            std::cout << "       ";
                        }
                        else if (MODUS != 5 && MODUS != 6)
                        {
                            std::cout << setw(18) << "|        ";
                            std::cout << "  ";
                        }
                    }
                }
                std::cout << std::endl;
            }
            std::cout << "______________________________________________________________________________________________________________________" << std::endl;
            std::cout << std::endl;
        }
        else
        {
            printGitterMitArray2(gitter, n, m, aktuell_k);
        }
        free(gitter);
    }
}

/**
 * @brief Sucht die minimale Ausgehende Kante eines Knoten im Ungerichteten gewichteten
 * Graphen.
 */
void erhalteMinimumUndirctedGraph(UndirectedGraph &G, size_t &return_knoten, double &return_distanz)
{
    double min_distanz = INT_MAX;
    typename property_map<UndirectedGraph, edge_weight_t>::type weight = get(edge_weight, G);
    typename graph_traits<UndirectedGraph>::out_edge_iterator ei, ei_end;

    if (DEBUGG_optional)
    {
        std::cout << "SucheUpdate min in Cluster " << return_knoten << "." << std::endl;
        std::cout << "Das Cluster " << return_knoten << " hat als";
    }

    for (boost::tie(ei, ei_end) = out_edges(return_knoten, G); ei != ei_end; ++ei)
    {
        if (weight[*ei] < min_distanz)
        {
            min_distanz = weight[*ei];
            return_knoten = boost::target(*ei, G);
        }
    }
    if (DEBUGG_optional)
    {
        std::cout << " naehste NachbarCluster ist " << return_knoten << " mit Abstand:" << min_distanz << std::endl;
    }
    return_distanz = min_distanz;
}

/**
 * @brief initialisiert den Heap mit ungerichteten Graphen.
 */
void initialisierenHeapGraph(MinHeap &h, UndirectedGraph &G_direkteNachbarnFrechet, const int n, const int m)
{
    double return_abstand;
    size_t return_gitterpkt;
    zeitMessen_start();
    for (size_t i = 1; i <= (size_t)n * m; i++)
    {
        return_gitterpkt = i;
        return_abstand = INT_MAX;
        erhalteMinimumUndirctedGraph(G_direkteNachbarnFrechet, return_gitterpkt, return_abstand);

        if (i != return_gitterpkt && i != return_gitterpkt)
        {
            h.insert(return_abstand, i, return_gitterpkt);
        }
    }
    zeitMessen_ende();
}

/**
 * @brief Methode zum testen der obigen Programme.
 */
int test_adj()
{
    const int n = 2;
    const int m = 2;
    const int num_nodes = n * m;

    ClusterGraph G(num_nodes);
    int help;

    for (int i = 1; i <= num_nodes; i++)
    {
        help = (i) % n;
        std::cout << " help " << help << "  i" << i << " n" << n << std::endl;

        if (help != 0)
        {
            add_edge(i, i + 1, G);
            add_edge(i + 1, i, G);
        }

        if (i + n <= n * m)
        {
            add_edge(i, i + n, G);
            add_edge(i + n, i, G);
        }
    }
    //erstelleClusterGraph(G,n,m);
    printClusterGraphKnoten(G);

    // print_graph(G, get(vertex_index, G));

    //vereinge_DirekteNachbarCluster(G,union ,2, 3);

    // auto neighbours = boost::adjacent_vertices(5, G);
    // for (auto vd : make_iterator_range(neighbours))
    // std::cout << "5 has adjacent vertex " << vd << "\n";

    //    vereinge_ClusterGraph(G, 2, 3);
    //    vereinge_ClusterGraph(G, 4, 2);

    std::cout << std::endl;
    // print_graph(G, get(vertex_index, G));

    // int value=INT_MAX;
    // int nb_richtung;
    // erhalteMinimumCluster(G,direkte4Nachbarn, 2, value, nb_richtung);

    std::cout << "Done" << std::endl;

    return 0;
}