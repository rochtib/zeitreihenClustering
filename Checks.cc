/**
 * @brief Testet ob der ClusterGraph k ZHK entaehlt.
 */
bool check_Clustergraph(const ClusterGraph &G, const int k)
{
    std::vector<int> component(num_vertices(G));
    int num = connected_components(G, &component[0]);

    // std::vector<int>::size_type i;
    // for (i = 1; i < component.size(); i++)
    // {
    //     cout << "Vertex " << i << " is in component " << component[i] << endl;
    // }

    if (num - 1 != k) // Knoten 0 ist im Grah enthalten
    {
        std::cerr << RED << "ERROR:" << RESET << std::endl;
        std::cout << "Anzahl Zusammenhangskomponenten in CLusterGraph" << num - 1 << " verabredet " << k << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Checkt die Anzahl der Cluster im Vektor. Die Anzahl muss k sein.
 */
void check_anzahlMuster(const int k, const int *muster_union, const int n, const int m)
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
 * @brief Prueft auf Uebereinstimmung von Datenmodell Clustering und dem berechnete
 * Clustering aus dem Algorithmus
 */
bool check_identisch(Datenmodell d, const ClusterGraph &G, const int n, const int m, const int data)
{
    if (data != 5 && data != 6)
    {
        int *gitter_akt;
        gitter_akt = (int *)malloc(((n * m) + 1) * sizeof(int *));

        for (int i = 0; i <= n * m; i++)
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
            gitter_akt[i] = help;
        }

        int *gitter_check;
        gitter_check = (int *)malloc(((n * m) + 1) * sizeof(int *));

        for (int i = 0; i <= n * m; i++)
        {
            gitter_check[i] = gitter_akt[i] - d.get_musterunion(i);
        }
        // print_gitter_mitVek(gitter_check, n, m);
        int count;
        for (int i = 1; i <= n * m; i++)
        {
            auto nb = boost::adjacent_vertices(i, G);
            std::set<int> set;
            count = 0;

            int help;
            for (const auto vd : make_iterator_range(nb))
            {
                help = vd; //um die warning zu umgehen
                count++;
            }
            if (count > 0)
            {
                for (auto vd : make_iterator_range(nb))
                {
                    set.insert(gitter_check[(int)vd]);
                }
            }
            // if (!set.empty())
            // {
            //     print_set(set);
            // }
            if (set.size() > 1)
            {
                std::cout << "datenmodellclustering " << RED << "!=" << RESET << " berechnetes Clustering" << std::endl;
                return false;
            }
        }
        std::cout << "datenmodellclustering == berechnetes Clustering" << std::endl;
        free(gitter_check);
        free(gitter_akt);
    }
    return true;
}