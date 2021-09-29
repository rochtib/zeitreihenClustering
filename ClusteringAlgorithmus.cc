#define LINKS 0  //linker Nachbar
#define OBEN 1   //oberer Nachbar
#define RECHTS 2 //rechter Nachbar
#define UNTEN 3  //linker Nachbar

extern bool DEBUGG_algo;
extern bool DEBUGG_optional;
extern bool SEED;
extern double zeit_global;
extern bool KORREKT;
extern bool KantenSpeicher;
extern double linkage_zielfunktionHeap;
extern bool CHECK;
extern bool mitInitialsierungTime;
extern int counterFrechet;

/**
 * @brief Gibt dem Namen des Algorithmus als String zurueck.
 */
std::string get_AlgorithmusName(const int MODUS)
{
   std::string str;
   switch (MODUS)
   {
   case 1:
      str = "SLDN";
      break;
   case 2:
      str = "SLFDN";
      break;
   case 3:
      str = "CLDN";
      break;
   case 4:
      str = "CLFDN";
      break;
   case 5:
      str = "SL";
      break;
   case 6:
      str = "CL";
      break;
   default:
      str = " Den Algorithmus " + to_string(MODUS) + " gibt es nicht!";
      break;
   }
   return str;
}

/**
 * @brief Ein Objekt der Klasse ClusterAlgo wird gebraucht um den Clustering
 * Algorithmus zu starten. Es wird darin das finale Clustering in einem Vektor gespeichert.
 */
class ClusterAlgo
{
   int *clusterBerechnetFeld;
   int *cluster_info_algo;
   int zeitreihenl;

public:
   ClusterAlgo(UndirectedGraph G_Gitter, const int n, const int m, UndirectedGraph G_Nachbar, const int MODUS, const int k, Datenmodell &d, const int anz_gitterpkt, const int data)
   {
      linkage_zielfunktionHeap = 0;
      cluster_info_algo = (int *)malloc(k * 2 * sizeof(int *));
      clusterBerechnetFeld = (int *)malloc(anz_gitterpkt * sizeof(int *));
      zeitreihenl = d.get_Zeitreihenl();
      algorithmus(G_Gitter, n, m, G_Nachbar, cluster_info_algo, clusterBerechnetFeld, d, MODUS, k, data);
   }

   /**
    * Die relevanten Informationen des Clusters werden mit dieser Methode entfernt.
    */
   void clear()
   {
      free(cluster_info_algo);
      free(clusterBerechnetFeld);
   }

   /**
    * @brief Diese Methode macht den Algorithmus berechnet und setzt den Abstand der Cluster.
    * Fuer MODUS == 2: Berechnet fuer alle a in cluster1 und b in cluster2 dF(a,b) und nimmt das Minimum dieser.
    * Fuer MODUS == 3: Berechnet alle kuerzesteten Wege zwischen zwei Clustern und nimmt das Maximum dieser.
    * Fuer MODUS == 4: Berechnet fuer alle a in cluster1 und b in cluster2 dF(a,b) und nimmt das Maximum dieser.
    * Dann wird der Abstand also die Kante in dem NachbarGraph auf das Maximum gesetzt. 
    * Falls der Heap eine kuerzeste Kante enthaelt die sich geaendert hat, muss der Heap auch 
    * geupdatet werden.
    * 
    * @param G_Frecht Dieser Graph hat zwei verschiedene Aufgaben:
    *                 Fuer MODUS == 3 werden auf dem Graph die kuerzesten Wege berechnet
    *                 Fuer MODUS == 2 oder 4 dient der Graph als Speicher der Frechet Abstaende.
    *                 Wenn G_Frecht ist der vollstaendige Graph muessen keine Frechet 
    *                 Berechnungen in SLFDN und CLFDN gemacht werden.
    * @param G_Cluster Graph der alle Cluster samt deren Punkte abspeichert. Jede ClusterID 
    *                  bildet eine ZHK. Alle Punkte des Clusters sind adjazent mit der ClusterID.
    * @param G_Nachbar Graph in dem Kanten als direkte Nachbarn interpretiert werden
    * @param id_update ID des neuen Clusters fuer den die Nachbardistanzen nach CL aktualisiert werden 
    * @param h Schluessel um auf Heap zuzugreifen
    * @param MODUS welches Verbindusstrategie der Clustering soll benutzt werden
    * @param zeitreihenl laenge der Zeitreihe
    * @param n Anzahl Spalten des Gitters
    */
   void updateNachbarGraphAbstand(UndirectedGraph &G_Frecht, ClusterGraph &G_Cluster, UndirectedGraph &G_Nachbar, const int id_update,
                                  MinHeap &h, const int MODUS, Datenmodell &d)
   {
      typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G_Nachbar);
      typename graph_traits<UndirectedGraph>::out_edge_iterator ei, ei_end;
      typename graph_traits<UndirectedGraph>::out_edge_iterator eiq, ei_endq;

      typename graph_traits<UndirectedGraph>::edge_descriptor e1;
      // typename property_map<UndirectedGraph, edge_weight_t>::type weight = get(edge_weight, G_Nachbar);
      typename property_map<UndirectedGraph, edge_weight_t>::type weightF = get(edge_weight, G_Frecht);

      double neuerAbstand = 0;
      double distanz_path;
      double distanz_frechet;

      std::set<int> set_update;
      // Die inneren Punkte eines benachbarten Clusters in Menge set_update einfuegen
      auto cluster_punkte = boost::adjacent_vertices(id_update, G_Cluster);
      for (auto update_pkte : make_iterator_range(cluster_punkte))
      {
         set_update.insert(update_pkte);
      }
      auto first_element = set_update.begin(); // wird spaeter der erste Punkt des Disjktra Algorithmus

      // std::cout << " Es wird das Cluster geupdatet:" << id_update << std::endl;
      // std::cout << "Menge des Clusters set_update" << set_update.size() << std::endl;

      //Alle Nachbarcluster von Knoten id_update
      for (boost::tie(ei, ei_end) = out_edges(id_update, G_Nachbar); ei != ei_end; ++ei)
      {
         const auto nb_cl = boost::target(*ei, G_Nachbar);

         /* Fuer jedes neues Nachbarcluster muessen Abstaende entsprechend initialisert werden.*/
         if (MODUS == 2 || MODUS == 5)
         {
            neuerAbstand = INT_MAX;
         }
         else if (MODUS == 3)
         {
            neuerAbstand = 0;
         }
         else if (MODUS == 4 || MODUS == 6)
         {
            neuerAbstand = 0;
         }

         auto nachbar_cluster = boost::adjacent_vertices(nb_cl, G_Cluster);
         for (auto cl_pkt : make_iterator_range(nachbar_cluster))
         {
            Ver s = vertex(cl_pkt, G_Frecht);
            Ver goal = vertex(*first_element, G_Frecht);
            // std::cout << "Knoten s:" << s << std::endl;
            // std::cout << "Knoten goal:" << goal << std::endl;

            if (MODUS == 3)
            {
               std::vector<Ver> p(num_vertices(G_Frecht));
               std::vector<double> d(num_vertices(G_Frecht));
               visitor_dijkstra vis(goal, set_update);
               try
               {
                  // std::cout<<"Disktra start"<<std::endl;
                  /**
                      * Evaluate Dijkstra on graph g with source s, predecessor_map p and distance_map d
                      * visitor
                      * @source https://www.boost.org/doc/libs/1_63_0/libs/graph/doc/dijkstra_shortest_paths.html
                      */
                  dijkstra_shortest_paths(G_Frecht, s,
                                          boost::predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, G_Frecht))).distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, G_Frecht))).weight_map(get(edge_weight, G_Frecht)).visitor(vis));
               }
               catch (int exception)
               {
                  // std::cout << "Dijskstra has stopped" << std::endl;
               }

               // findet den maximal kuerzesten Weg
               for (auto nb_pkte : make_iterator_range(cluster_punkte))
               {
                  distanz_path = d[nb_pkte];

                  if (distanz_path > neuerAbstand)
                  {
                     neuerAbstand = distanz_path;
                     // std::cout << " neuerAbstand" << neuerAbstand << std::endl;
                  }
               }
               //print_distances(d, s);

               /**
                   * Konsolen Ausgabe KW
                   */
               // Ver current = goal;
               // for (auto update_pkte : make_iterator_range(cluster_punkte))
               // {
               //    std::vector<Ver> path;
               //    current = update_pkte;
               //    Ver start = s;
               //    while (current != start)
               //    {
               //       path.push_back(current);
               //       current = p[current];
               //    }
               //    path.push_back(start);
               //    print_path(path, d, start, update_pkte);
               // }
               // print_set(hilfesmenge);
            }
            else if (MODUS == 2 || MODUS == 4)
            {
               bool found;
               for (auto it = set_update.begin(); it != set_update.end(); ++it)
               {
                  boost::tie(e1, found) = edge((int)*it, (int)s, G_Frecht);
                  if (!found)
                  {
                     distanz_frechet = callcFrechet(d, (size_t)*it - 1, (size_t)s - 1, zeitreihenl);
                     counterFrechet++;
                     if (KantenSpeicher)
                     {
                        add_edge((int)*it, (int)s, distanz_frechet, G_Frecht);
                     }
                  }
                  else
                  {
                     distanz_frechet = get(weightF, e1);
                  }

                  if (MODUS == 2)
                  {
                     if (distanz_frechet < neuerAbstand)
                     {
                        neuerAbstand = distanz_frechet;
                        // std::cout << " neuerAbstand" << neuerAbstand << std::endl;
                     }
                  }
                  if (MODUS == 4)
                  {
                     if (distanz_frechet > neuerAbstand)
                     {
                        neuerAbstand = distanz_frechet;
                        // std::cout << " neuerAbstand" << neuerAbstand << std::endl;
                     }
                  }
               }
            }
         }
         // std::cout << "1 Vorher source:" << boost::source(*ei, G_Nachbar) << " target:" << boost::target(*ei, G_Nachbar) << " EdgeWeightMapNachbar[*ei] " << EdgeWeightMapNachbar[*ei] << "jetzt neuerAbstand" << neuerAbstand << std::endl;
         EdgeWeightMapNachbar[*ei] = neuerAbstand;
         // std::cout << "1 Nachher source:" << boost::source(*ei, G_Nachbar) << " target:" << boost::target(*ei, G_Nachbar) << " EdgeWeightMapNachbar[*ei]  " << EdgeWeightMapNachbar[*ei] << "jetzt neuerAbstand" << neuerAbstand << std::endl;
         // print_NachbarGraph(G_Nachbar);
         /*
          #########################################################################
          Ab hier wird geschaut ob der MinHeap geupdatet werden muss
          #########################################################################
          */

         // h.printMap();
         int pos_heap = h.get_map(nb_cl) - 1;
         // std::cout << "TEST id_checkneu:" << id_checkneu << "befindet sich in Heap an pos" << pos_heap << std::endl;
         // int bas_heap = h.get_ClusterIdHeap(pos_heap - 1);

         int benachbartesCluster = h.get_heap_Gitterpunkt(pos_heap);
         // std::cout << " pos_heap -1:" << pos_heap << std::endl;
         // std::cout << " bas_heap:" << bas_heap << std::endl;
         // std::cout  << " benachbartesCluster:" << benachbartesCluster << std::endl;

         if (benachbartesCluster == id_update)
         {
            size_t test_knoten = nb_cl;
            double test_distanz;
            erhalteMinimumNachbarGraph(G_Nachbar, test_knoten, test_distanz);

            // std::cout << "Es wird geupdated." << std::endl;
            // AUF NUMMER SICHER dann loeschen
            // h.deleteKey(pos_heap);
            // h.insert(test_distanz, nb_cl, test_knoten);

            // h.print_heap();
            // h.printMap();
            {
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
            // h.print_heap();
            // h.printMap();
         }
         else
         {
            if (DEBUGG_optional)
            {
               std::cout << "Kein Heap Update notwendig bei" << nb_cl << std::endl;
            }
         }
      }
   }

   /**
    * @brief Methode die fuer jede ClusterID die Punkte des Cluster zaehlt.
    */
   void clusterInformationAlgo(int *comp, int *cluster_info_ago, const int n, const int m)
   {
      std::set<int> set_ids;
      int index = 0;
      int count = 0;
      set_ids.insert(0);
      for (int i = 0; i <= n * m; i++)
      {
         if (!set_ids.contains(comp[i]))
         {
            set_ids.insert(comp[i]);
            count = 0;
            for (int j = 0; j <= n * m; j++)
            {
               if (comp[i] == comp[j])
               {
                  count++;
               }
            }
            cluster_info_ago[index] = comp[i];
            cluster_info_ago[index + 1] = count;
            index += 2;
         }
      }
   }

   /**
    * @brief Hilfsmethode um eine Menge auszugeben
    */
   void print_set(const std::set<int> hilfesmenge)
   {
      if (DEBUGG_optional)
      {
         std::cout << "Menge:";
         for (auto it = hilfesmenge.begin(); it != hilfesmenge.end(); ++it)
         {
            std::cout << *it << "  ";
         }
         std::cout << std::endl;
      }
   }

   /**
    * @brief Hilfsmethode zum Algorithmus verstehen. Gibt den kuerzesten Weg von start nach ziel
    *     
    * @param pfad Enthaelt alle kuerzesten wege
    * @param start Startknoten
    * @param ziel Zielknoten
    */
   void print_path(std::vector<Ver> pfad, const std::vector<double> d, const Ver start, const Ver ziel)
   {
      if (DEBUGG_optional)
      {
         std::cout << "kuerzester Weg zwischen " << start << " und " << ziel << ": ";
         std::vector<Ver>::reverse_iterator it;
         for (it = pfad.rbegin(); it != pfad.rend(); ++it)
         {
            std::cout << *it << " ";
         }
         std::cout << "mit Distanz:" << d[ziel] << std::endl;
      }
   }

   /**
    * @brief Hilfsmethode um kuerzeste Distanzen auszugeben.
    * 
    * @param d Vektor der alle Distanzen enthaelt
    * @param s Knoten von dem aus alle Distanzen berechnet wurden
    */
   void print_distances(const std::vector<double> d, const Ver s)
   {
      if (DEBUGG_optional)
      {
         // print distanze vekotor
         for (size_t i = 0; i < d.size(); i++)
         {
            std::cout << s << " zu " << i << " d:" << d[i] << std::endl;
         }
      }
   }

   /**
    * @brief Gibt den Namen des ausfuehrenden Algorithmus auf der Konsole aus.
    */
   void print_AlgorithmusName(const int MODUS)
   {
      std::cout << "#########################################################################" << std::endl;
      switch (MODUS)
      {
      case 1:
         std::cout << "SingleLinkage - DirekterNachbar" << std::endl;
         break;
      case 2:
         std::cout << "SingleLinkage - FrechetDirekterNachbar" << std::endl;
         break;
      case 3:
         std::cout << "CompleteLinkage - DirekterNachbar" << std::endl;
         break;
      case 4:
         std::cout << "CompleteLinkage - FrechetDirekterNachbar" << std::endl;
         break;
      case 5:
         std::cout << "SingleLinkage - Klassisch. Keine Lokalitaet" << std::endl;
         break;
      case 6:
         std::cout << "CompleteLinkage - Klassisch. Keine Lokalitaet" << std::endl;
         break;
      default:
         std::cout << RED << "ERROR:" << RESET << " Den Algorithmus " << MODUS << " gibt es nicht!" << std::endl;
         break;
      }
   }

   /**
    * @brief In der Methode wird die Algorithmen gesteuert. Hier wird ein 
    * das Clustering berechnet das man ausgewaehlt hat.
    */
   void algorithmus(UndirectedGraph &G_Frechet, const int n, const int m, UndirectedGraph &G_Nachbar,
                    int *cluster_info_algo, int *clusterBerechnetFeld, Datenmodell &d,
                    const int MODUS, const int k, const int data)
   {
      typedef adjacency_list<listS, vecS, bidirectionalS> ClusterGraph;
      typedef property<edge_weight_t, double> Weight;
      typedef adjacency_list<listS, vecS, undirectedS, no_property, Weight> UndirectedGraph;
      int anz_gitterpkt = n * m;

      ClusterGraph G_Cluster(anz_gitterpkt);

      int anz = anz_gitterpkt;

      int clusterID;
      int nachbarCluster;

      int neueCLID;
      int alteCLID;
      std::cout << "######################################" << std::endl;
      std::cout << "n:" << n << " m:" << m << " k:" << k << std::endl;

      if (MODUS != 1 && MODUS != 5)
      {
         MinHeap h(anz_gitterpkt);

         initialisierenHeapGraph(h, G_Nachbar, n, m);
         h.print_heap();

         printUngerichteterGraph(G_Nachbar, n, m, "NachbarGraph:");
         erstelleClusterGraph(G_Cluster, n, m);
         printClusterGraph(G_Cluster, n, m);

         printClusterGraphAlsGitterMitAbstand(G_Cluster, G_Nachbar, n, m, "ClusterGraph als Gitter zu Abstand NachbarClustern", MODUS, false, anz_gitterpkt);
         print_AlgorithmusName(MODUS);
         d.print_DatenmodellName(data);

         if (mitInitialsierungTime)
         {
            reset_time();
         }

         counterFrechet = 0;

         // START ZEITMESSUNG
         zeitMessen_start();
         while (anz != k)
         {
            if (DEBUGG_algo)
            {
               std::cout << "#################################" << std::endl;
               std::cout << "Aktuelle Anzahl an Cluster:" << anz << std::endl;
            }
            if (!check_Clustergraph(G_Cluster, anz))
            {
               std::cout << RED << "ERROR:" << RESET << " Abbruch Algorithmus!" << std::endl;
               std::cout << "Aktuelle Anzahl an Cluster:" << anz << std::endl;
               KORREKT = false;
               break;
            }

            if (h.get_size() != anz)
            {
               std::cout << RED << "ERROR:" << RESET << " Abbruch Algorithmus!" << std::endl;
               std::cout << "Aktuelle Anzahl an Cluster:" << anz << std::endl;
               KORREKT = false;
               break;
            }

            anz--;
            h.print_heap();

            nachbarCluster = h.get_minHeap();
            clusterID = h.extractMin();

            // h.print_heap();
            // std::cout << "heap entferntes Miniumum  clusterID:" << clusterID << "  nachbarCluster:" << nachbarCluster << std::endl;
            // std::cout << " ClusterID" << nachbarCluster << " von Gitterpunkt" << nachbarCluster << std::endl;

            /**
             * SAFTY!!!!
             * Objekt loeschen und neu einfuegen anstatt hoch oder runterzububbeln
             */
            // int pos_heap = h.get_map(nachbarCluster);
            // std::cout<<"nachbarCluster "<<nachbarCluster<<std::endl;
            // h.print_map();
            //  h.deleteKey(pos_heap - 1);
            // h.print_heap();

            /**
             * Cluster werden auf die kleiner ID vereinigt.
             */
            neueCLID = std::min(clusterID, nachbarCluster);
            alteCLID = std::max(clusterID, nachbarCluster);

            if (DEBUGG_optional)
            {
               // h.print_map();
               std::cout << "clusterID:" << clusterID << std::endl;
               std::cout << "nachbarCluster:" << nachbarCluster << "" << std::endl;
               std::cout << "neueCLID:" << neueCLID << " alteCLID:" << alteCLID << std::endl;
               // std::cout << " pos_heap:" << pos_heap << std::endl;
            }

            vereingeClusterGraph(G_Cluster, neueCLID, alteCLID);
            printClusterGraph(G_Cluster, n, m);
            printUngerichteterGraph(G_Nachbar, n, m, "NachbarGraph vor verbindenCluster:");

            if (MODUS != 6)
            {
               verschmelzeNachbarGraph(G_Nachbar, neueCLID, alteCLID, h);
               updateNachbarGraphAbstand(G_Frechet, G_Cluster, G_Nachbar, neueCLID,
                                         h, MODUS, d);
            }
            else
            {
               // VARIANTE CL Klassisch
               verschmelzeNachbarGraphCL(G_Nachbar, neueCLID, alteCLID, h, MODUS);
            }

            printUngerichteterGraph(G_Nachbar, n, m, "NachbarGraph nach verbindenCluster:");

            // h.print_heap();
            // h.printMap();

            size_t test_knoten = neueCLID;
            double test_distanz;
            erhalteMinimumNachbarGraph(G_Nachbar, test_knoten, test_distanz);
            //  h.insert(test_distanz, neueCLID, test_knoten); //SAFTY!!!

            int pos_heap = h.get_map(nachbarCluster) - 1;
            double alt = h.get_heap_gewichte(pos_heap);
            h.set_heap_gewichte(pos_heap, test_distanz);
            h.set_heap_Gitterpunkt(pos_heap, test_knoten);
            h.set_ClusterIdHeap(pos_heap, neueCLID);

            h.mapKorrigieren(nachbarCluster, clusterID);
            if (pos_heap != 0 && alt > test_distanz)
            {
               h.BubbleUp(pos_heap);
            }
            else
            {
               h.BubbleDown(pos_heap);
            }

            printClusterGraphAlsGitterMitAbstand(G_Cluster, G_Nachbar, n, m, "ClusterGraph  mit Abstaenden zu NachbarClustern", MODUS, true, anz);

            if (CHECK) // Nur wenn CHECK == true werden die Checks gemacht.
            {
               if (anz - 1 == k && (MODUS == 4 || MODUS == 6)) // CLFDN || CL
               {
                  linkage_zielfunktionHeap = h.get_last_heap_gewichte();
                  std::cout << "Zielfunktion aus Heap: " << linkage_zielfunktionHeap << std::endl;
               }
               if (MODUS == 2) // SLFDN
               {
                  if (h.get_last_heap_gewichte() > linkage_zielfunktionHeap)
                  {
                     linkage_zielfunktionHeap = h.get_last_heap_gewichte();
                  }
                  if (anz - 1 == k)
                  {
                     std::cout << "Zielfunktion aus Heap: " << linkage_zielfunktionHeap << std::endl;
                  }
               }
            }
         }
         erstelleArrayAusClusterGraph(G_Cluster, clusterBerechnetFeld, n, m);
         // ENDE ZEITMESSUNG
         zeitMessen_ende();

         check_Clustergraph(G_Cluster, k);

         h.print_heap();

         printClusterGraph(G_Cluster, n, m);
         // erstelleArrayAusClusterGraph(G_Cluster, clusterBerechnetFeld, n, m);
         check_anzahlMuster(k, clusterBerechnetFeld, n, m);

         h.clear();
      }
      if (MODUS == 1 || MODUS == 5)
      {
         print_AlgorithmusName(MODUS);
         d.print_DatenmodellName(data);

         if (MODUS == 1)
         {
            ClusterGraph G_Cluster_dummy(anz_gitterpkt);
            erstelleClusterGraph(G_Cluster_dummy, n, m);
            printClusterGraphAlsGitterMitAbstand(G_Cluster_dummy, G_Frechet, n, m, "Erstelle " + to_string(k) + " Clustering nicht agglomerativ ausgehend vom folgenden Graph:", MODUS, false, anz_gitterpkt);
            G_Cluster_dummy.clear();
         }

         /**
          * Berechne Minimal Spanning Tree
          * @source https://www.boost.org/doc/libs/1_53_0/libs/graph/doc/kruskal_min_spanning_tree.html
          */
         std::vector<graph_traits<UndirectedGraph>::edge_descriptor> mst;

         if (mitInitialsierungTime)
         {
            reset_time();
         }

         UndirectedGraph G_cl(anz_gitterpkt);
         property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMap = get(edge_weight, G_Frechet);
         std::vector<int> component((n * m) + 1);
         zeitMessen_start();
         kruskal_minimum_spanning_tree(G_Frechet, std::back_inserter(mst));

         for (size_t e = 0; e < mst.size() - k + 1; e++)
         {
            add_edge(source(mst[e], G_Frechet), target(mst[e], G_Frechet), get(EdgeWeightMap, mst[e]), G_cl);
         }

         /**
          * Es kann sein das Knoten n*m isoliert ist in dem Falle wird er nicht als ein weiteres Cluster 
          * bemerkt werden weil seine Existenz nicht bekannt ist.
          */
         add_edge(n * m, n * m, G_cl);
         // printUngerichteterGraph(G_cl, n, m);

         /**
          * @source https://www.boost.org/doc/libs/1_65_1/libs/graph/example/connected_components.cpp
          */
         int num = connected_components(G_cl, &component[0]);
         zeitMessen_ende();
         // if (DEBUGG_optional)
         // {
         //    std::vector<int>::size_type i;
         //    std::cout << "Total number of components: " << num << std::endl;
         //    for (i = 0; i != component.size(); i++)
         //    {
         //       std::cout << "Vertex " << i << " is in component " << component[i] << std::endl;
         //    }
         //    std::cout << std::endl;
         // }
         if (num != k + 1)
         {
            std::cout << RED << "ERROR:" << RESET << "Nicht die Clusteranzahl.";
            std::cout << " num:" << num << " k+1:" << k + 1 << std::endl;
         }

         erstelleClusterGraphSL(G_Cluster, clusterBerechnetFeld, n, m, component);
         printClusterGraph(G_Cluster, n, m);

         component.clear();
         G_cl.clear();

         if (DEBUGG_algo)
         {
            printClusterGraphAlsGitterMitAbstand(G_Cluster, G_Frechet, n, m, to_string(k) + " Clustering nach " + get_AlgorithmusName(MODUS), MODUS, false, k);
         }
      } // ENDE nicht agglomerativer Algo

      clusterInformationAlgo(clusterBerechnetFeld, cluster_info_algo, n, m);

      if (CHECK)
      {
         printVergleichGitterModell(d, clusterBerechnetFeld, n, m, k, data);
         if (d.get_datenType() == "SINbekanntPurity" || d.get_datenType() == "SINbekanntPurityKW")
         {
            bool cluster_identisch = check_identisch(d, G_Cluster, n, m, data);
            if (!cluster_identisch)
            {
               KORREKT = false;
               std::string str1 = "MODELL ClusterID";
               std::string str2 = " Anz";
               std::string str3 = "     ";
               std::string str4 = "    ALGO ClusterID";
               std::string str5 = " Anz";
               std::cout << str1 + str2 + str3 + str4 + str5 << std::endl;
               for (int j = 0; j < k; j++)
               {
                  std::cout << setw(str1.size()) << cluster_info_algo[j * 2 + 0] << setw(str2.size()) << cluster_info_algo[j * 2 + 1] << setw(str3.size()) << "|";
                  std::cout << setw(str4.size()) << d.get_clusterinfo(j * 2 + 0) << setw(str5.size()) << d.get_clusterinfo(j * 2 + 1) << std::endl;
               }
            }
         }
      }
      std::cout << "Dauer:" << get_stringTime(zeit_global) << std::endl;

      G_Cluster.clear();
   }
   /**
    * @brief Gibt fuer einen Konten das Cluster Label zurueck.
    * 
    * @param pos ist der Index des Arrays was dem Knoten entspricht
    * @return double ist das Cluster Label des Knoten
    */
   double get_clusterBerechnetFeld(const int pos)
   {
      return clusterBerechnetFeld[pos];
   }
};

/**
 ***********************************************************************************
 ***********************************************************************************
  METHODEN METHODEN UM DAS CLUSTER ZU MESSEN
 ***********************************************************************************
 ***********************************************************************************
 */

/**
 * @brief Berechnet die Zielfunktion von Complete Linkage
 * @param c Schluessel um auf die berechneten CLuster zuzugreifen
 * @param G_voll enthaehlt alle Frechet Berechnungen
 * @return double groesster Durchmesser im Clustering
 */
double zielfunktionCL(ClusterAlgo &c, UndirectedGraph &G_voll, const int n, const int m, const int k)
{
   std::set<int> schon_bearbeitet;
   typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G_voll);
   typename boost::graph_traits<UndirectedGraph>::edge_descriptor edgee;
   bool found;
   int i = 1;
   double zfkt_cl = 0;
   int cl_durchmesser = 0;
   while ((int)schon_bearbeitet.size() != k)
   {
      if (!schon_bearbeitet.contains(c.get_clusterBerechnetFeld(i)))
      {
         if (DEBUGG_optional)
         {
            std::cout << "CLUSTER: " << c.get_clusterBerechnetFeld(i) << std::endl;
            std::cout << "cluster pkt: ";
         }
         schon_bearbeitet.insert(c.get_clusterBerechnetFeld(i));
         std::set<int> clusterPkte;

         for (int p = 1; p <= n * m; p++)
         {
            if (c.get_clusterBerechnetFeld(i) == c.get_clusterBerechnetFeld(p))
            {
               clusterPkte.insert(p);
               if (DEBUGG_optional)
               {
                  std::cout << p << " ";
               }
            }
         }
         if (DEBUGG_optional)
         {
            std::cout << std::endl;
         }
         for (auto it_source = clusterPkte.begin(); it_source != clusterPkte.end(); ++it_source)
         {
            // std::cout << *it_source << "  " << std::endl;
            for (auto it_target = it_source; it_target != clusterPkte.end(); ++it_target)
            {
               // std::cout << *it_target << "  ";
               tie(edgee, found) = edge(*it_source, *it_target, G_voll);
               if (found)
               {
                  // std::cout << "EdgeWeightMapNachbar" << EdgeWeightMapNachbar[edgee] << "  kante" << *it_source << "," << *it_target << std::endl;
                  if (EdgeWeightMapNachbar[edgee] > zfkt_cl)
                  {
                     zfkt_cl = EdgeWeightMapNachbar[edgee];
                     cl_durchmesser = c.get_clusterBerechnetFeld(i);
                  }
               }
               else
               {
                  if (*it_source != *it_target)
                  {
                     std::cerr << RED << "ERROR:" << RESET << "Kante muss berechnet sein!!! (" << *it_source << "," << *it_target << ")\n";
                  }
               }
            }
         }
         clusterPkte.clear();
      }
      i++;
   }
   if (DEBUGG_optional)
   {
      std::cout << "Cluster mit groeßtem Durchmesser:" << cl_durchmesser << std::endl;
   }
   schon_bearbeitet.clear();
   return zfkt_cl;
}

/**
 * @brief Berechnet die Zielfunktion von Single Linkage
 * @param c Schluessel um auf die berechneten Cluster zuzugreifen
 * @param G_voll enthaehlt alle Frechet Berechnungen
 * @return double groesster Durchmesser im Clustering
 */
double zielfunktionSL(ClusterAlgo &c, UndirectedGraph &G_voll, const int n, const int m, const int k)
{
   std::set<int> schon_bearbeitet;
   typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMapNachbar = get(edge_weight, G_voll);
   typename boost::graph_traits<UndirectedGraph>::edge_descriptor edgee;
   typename boost::graph_traits<UndirectedGraph>::edge_descriptor edgee_2;
   bool found;
   bool found_2;
   int i = 1;
   double sum_mst = 0;
   while ((int)schon_bearbeitet.size() != k)
   {
      if (!schon_bearbeitet.contains(c.get_clusterBerechnetFeld(i)))
      {
         if (DEBUGG_optional)
         {
            std::cout << "CLUSTER: " << c.get_clusterBerechnetFeld(i) << std::endl;
         }
         schon_bearbeitet.insert(c.get_clusterBerechnetFeld(i));
         std::set<int> clusterPkte;
         for (int p = i; p <= n * m; p++)
         {
            if (c.get_clusterBerechnetFeld(i) == c.get_clusterBerechnetFeld(p))
            {
               clusterPkte.insert(p);
            }
         }
         if (clusterPkte.size() > 1)
         {
            std::unordered_map<int, int> map;
            int index = 1;
            for (auto it = clusterPkte.begin(); it != clusterPkte.end(); ++it)
            {
               map[*it] = index;
               if (DEBUGG_optional)
               {
                  std::cout << " NEW:" << map[*it] << " OLD:" << *it << std::endl;
               }
               index++;
            }
            UndirectedGraph G_Frecht(map.size());
            typename property_map<UndirectedGraph, edge_weight_t>::type EdgeWeightMap = get(edge_weight, G_Frecht);
            for (auto it_source = clusterPkte.begin(); it_source != clusterPkte.end(); ++it_source)
            {
               // std::cout << *it_source << "  " << std::endl;
               for (auto it_target = clusterPkte.begin(); it_target != clusterPkte.end(); ++it_target)
               {
                  // std::cout << *it_target << "  ";
                  tie(edgee, found) = edge(*it_source, *it_target, G_voll);
                  tie(edgee_2, found_2) = edge(map[*it_source], map[*it_target], G_Frecht);
                  if (found && !found_2)
                  {
                     add_edge(map[*it_source], map[*it_target], EdgeWeightMapNachbar[edgee], G_Frecht);
                  }
                  else if (!found)
                  {
                     if (*it_source != *it_target)
                     {
                        std::cerr << RED << "ERROR:" << RESET << "Kante muss berechnet sein!!! (" << *it_source << "," << *it_target << ")\n";
                     }
                  }
               }
            }

            printUngerichteterGraph(G_Frecht, num_vertices(G_Frecht) - 1, 1, "Suche MST:");
            std::vector<typename graph_traits<UndirectedGraph>::edge_descriptor> mst;
            kruskal_minimum_spanning_tree(G_Frecht, std::back_inserter(mst));

            for (size_t e = 0; e < mst.size(); e++)
            {
               sum_mst += get(EdgeWeightMap, mst[e]);
            }

            mst.clear();
            clusterPkte.clear();
            G_Frecht.clear();
         }
      }
      i++;
   }
   schon_bearbeitet.clear();
   return sum_mst;
}

/**
 * @brief Berechnet den Purity Wert zwischen zwei Clusterings
 * @param c Schluessel fuer die berechneten Cluster,
 * @param d Schluessel fuer die Anfangspartition
 * @return double ist der purity Wert
 * @source https://stackoverflow.com/questions/13448064/how-to-find-the-intersection-of-two-stdset-in-c
 */
double purity(ClusterAlgo &c, Datenmodell d, const int n, const int m, const int k)
{
   double purity = 0;
   std::set<int> schon_bearbeitet;
   int i = 1;
   while ((int)schon_bearbeitet.size() != k)
   {
      if (!schon_bearbeitet.contains(c.get_clusterBerechnetFeld(i)))
      {
         if (DEBUGG_optional)
         {
            std::cout << "CLUSTER " << c.get_clusterBerechnetFeld(i) << " besteht aus : ";
         }
         schon_bearbeitet.insert(c.get_clusterBerechnetFeld(i));
         std::set<int> clusterPkte;
         for (int p = 1; p <= n * m; p++)
         {
            if (c.get_clusterBerechnetFeld(i) == c.get_clusterBerechnetFeld(p))
            {
               clusterPkte.insert(p);
               if (DEBUGG_optional)
               {
                  std::cout << p << " ";
               }
            }
         }
         if (DEBUGG_optional)
         {
            std::cout << std::endl;
         }
         if (clusterPkte.size() > 1)
         {
            int k_index = 1;
            int purity_help = 0;
            while (k_index <= k)
            {
               std::set<int> anfangsPartition;
               if (DEBUGG_optional)
               {
                  std::cout << "Anfangspartition Cluster:" << k_index << std::endl;
               }
               for (int partition_index = 1; partition_index <= n * m; partition_index++)
               {
                  if (d.get_musterunion(partition_index) == k_index)
                  {
                     anfangsPartition.insert(partition_index);
                     if (DEBUGG_optional)
                     {
                        std::cout << partition_index << " ";
                     }
                  }
               }

               std::set<int> intersect;
               set_intersection(clusterPkte.begin(), clusterPkte.end(), anfangsPartition.begin(), anfangsPartition.end(),
                                std::inserter(intersect, intersect.begin()));
               if (DEBUGG_optional)
               {
                  std::cout << std::endl;
                  std::cout << "intersect.size(): " << intersect.size();
                  std::cout << std::endl;
               }
               if ((int)intersect.size() > purity_help)
               {
                  purity_help = intersect.size();
               }
               k_index++;
            }
            purity += purity_help;
         }
         else
         {
            purity += 1;
         }

         clusterPkte.clear();
      }
      i++;
   }
   purity = (double)((double)purity / (double)(n * m));
   schon_bearbeitet.clear();
   return purity;
}

/**
 ***********************************************************************************
 ***********************************************************************************
  ALGORITHMUS TESTEN 
 ***********************************************************************************
 ***********************************************************************************
 */

/**
 * @brief gehoert zu main_AlgoTesten. Fuehrt einen Algorithmus aus.
 */
void algoTesten(const int MODUS, Datenmodell &d, const int data, double amplitude, const int k, const int n, const int m)
{
   const int anz_gitterpkt = n * m;
   double *direkte4Nachbarn;
   direkte4Nachbarn = (double *)malloc(anz_gitterpkt * 4 * sizeof(double *));
   GeneratorSeed seed_samen(0);
   d.erstelleDaten(n, m, k, nullptr, data, amplitude, seed_samen);
   double cluster_purity = 0;
   reset_time();
   switch (MODUS)
   {
   case 1: //"SLDN"
   case 3: //"CLDN"
   {
      berechneDirekteNachbarn(d, direkte4Nachbarn, n, m, d.get_Zeitreihenl());
      printDirekte4Nachbarn(direkte4Nachbarn, n, m);
      UndirectedGraph G_Frecht(anz_gitterpkt);
      erstelleGitterGraph(G_Frecht, direkte4Nachbarn, n, m);
      UndirectedGraph Graphdummy(0);
      ClusterAlgo DN(G_Frecht, n, m, G_Frecht, MODUS, k, d, anz_gitterpkt, data);

      UndirectedGraph G_direkteNB(anz_gitterpkt);
      kompletterGraph(G_direkteNB, d, anz_gitterpkt, d.get_Zeitreihenl());
      std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      if (MODUS == 1)
      {
         std::cout << "Zielfunktionswert SL " << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         double sl_wert = zielfunktionSL(DN, G_direkteNB, n, m, k);
         std::cout << "-> zielfunktion = " << sl_wert << std::endl;
      }
      if (MODUS == 3)
      {
         std::cout << "Zielfunktionswert CL: " << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         double cl_wert = zielfunktionCL(DN, G_direkteNB, n, m, k);
         std::cout << "-> zielfunktion =  " << cl_wert << std::endl;
      }
      if (data == 1 || data == 2 || data == 3 || data == 4)
      {
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         std::cout << "Purity" << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         cluster_purity = purity(DN, d, n, m, k);
         std::cout << "-> purity = " << cluster_purity << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      }
      Graphdummy.clear();
      G_Frecht.clear();
      DN.clear();
      break;
   }
   case 2: // "SLFDN"
   case 4: // "CLFDN"
   {
      UndirectedGraph G_Gitter(anz_gitterpkt + 1);
      initialerGitterGraph(G_Gitter, d, n, m, d.get_Zeitreihenl());
      ClusterAlgo FDN(G_Gitter, n, m, G_Gitter, MODUS, k, d, anz_gitterpkt, data);

      UndirectedGraph G_vollst(anz_gitterpkt);
      kompletterGraph(G_vollst, d, anz_gitterpkt, d.get_Zeitreihenl());
      std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      if (MODUS == 2)
      {
         std::cout << "Zielfunktionswert SL" << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         double sl_wert = zielfunktionSL(FDN, G_vollst, n, m, k);
         std::cout << "-> zielfunktion = " << sl_wert << std::endl;
      }
      if (MODUS == 4)
      {
         std::cout << "Zielfunktionswert CL" << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         double cl_wert = zielfunktionCL(FDN, G_vollst, n, m, k);
         std::cout << "-> zielfunktion = " << cl_wert << std::endl;
      }
      if (data == 1 || data == 2 || data == 3 || data == 4)
      {
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         std::cout << "Purity" << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         cluster_purity = purity(FDN, d, n, m, k);
         std::cout << "-> purity = " << cluster_purity << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      }
      std::cout << "Anzahl Frechet Berechnungen" << counterFrechet << std::endl;
      G_vollst.clear();
      G_Gitter.clear();
      FDN.clear();
      break;
   }
   case 5: //"SL"
   case 6: //"CL"
   {
      UndirectedGraph G_vollst(anz_gitterpkt);
      kompletterGraph(G_vollst, d, anz_gitterpkt, d.get_Zeitreihenl());
      printUngerichteterGraph(G_vollst, n, m, "vollstaendigerGraph:");
      ClusterAlgo klassisch(G_vollst, n, m, G_vollst, MODUS, k, d, anz_gitterpkt, data);

      std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      if (MODUS == 5)
      {
         std::cout << "Zielfunktionswert SL " << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         double sl_wert = zielfunktionSL(klassisch, G_vollst, n, m, k);
         std::cout << "-> zielfunktion = " << sl_wert << std::endl;
      }
      if (MODUS == 6)
      {
         std::cout << "Zielfunktionswert CL " << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         double cl_wert = zielfunktionCL(klassisch, G_vollst, n, m, k);
         std::cout << "-> zielfunktion = " << cl_wert << std::endl;
      }
      if (data == 1 || data == 2 || data == 3 || data == 4)
      {
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         std::cout << "Purity" << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
         cluster_purity = purity(klassisch, d, n, m, k);
         std::cout << "-> purity =" << cluster_purity << std::endl;
         std::cout << "+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      }
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

/**
 * @brief Demonstriert die implementierten Algorithmen
 * 
 * @param SLDN true/false, ob Algorithmus ausgefuehrt wird
 * @param SLFDN true/false, ob Algorithmus ausgefuehrt wird
 * @param SL true/false, ob Algorithmus ausgefuehrt wird
 * @param CLDN true/false, ob Algorithmus ausgefuehrt wird
 * @param CLFDN true/false, ob Algorithmus ausgefuehrt wird
 * @param CL true/false, ob Algorithmus ausgefuehrt wird
 * @param _DEBUGG_optional true/false, erweitert die Konsolen Ausgabe von DEBUGG_algo
 * @param _DEBUGG_algo true/false, um Schritte nachvollziehen zu koennen
 * @param _SEED true/false, soll es reproduzierbar sein
 * @param data 
 * @param type_code 
 *                   type_code = 1 -> "SINbekanntPurityKW"
 *                   type_code = 2 -> "kontrollAmplitude"
 *                   type_code = 4 -> "RunTimeSeriesRANDOM"
 * @param amplitude nur fuer bestimmte Zeitreihen Generatoren type_code = 1,2 relevant
 * @param Jahr nur fuer RealWorld Daten relevant
 */
void main_Clustering(const int n, const int m, const int k, const int zeitreihenl,
                     const bool SLDN, const bool SLFDN, const bool SL,
                     const bool CLDN, const bool CLFDN, const bool CL,
                     const bool _DEBUGG_optional, const bool _DEBUGG_algo, const bool _SEED,
                     const int data, const int type_code, const double amplitude, const int Jahr)
{
   DEBUGG_optional = _DEBUGG_optional;
   DEBUGG_algo = _DEBUGG_algo;
   SEED = _SEED;
   CHECK = true;
   const int anz_gitterpkt = n * m;
   std::string type;
   if (type_code == 1)
   {
      type = "SINbekanntPurityKW";
   }
   else if (type_code == 2)
   {
      type = "kontrollAmplitude";
   }
   else if (type_code == 4)
   {
      type = "RunTimeSeriesRANDOM";
   }

   int MODUS;

   if (SLDN)
   {
      Datenmodell d(anz_gitterpkt, k, zeitreihenl, Jahr, type);
      algoTesten(MODUS = 1, d, data, amplitude, k, n, m);
      d.clear();
   }
   if (SLFDN)
   {
      counterFrechet = 0;
      Datenmodell d(anz_gitterpkt, k, zeitreihenl, Jahr, type);
      algoTesten(MODUS = 2, d, data, amplitude, k, n, m);
      d.clear();
   }
   if (SL)
   {
      Datenmodell d(anz_gitterpkt, k, zeitreihenl, Jahr, type);
      algoTesten(MODUS = 5, d, data, amplitude, k, n, m);
      d.clear();
   }
   if (CLDN)
   {
      Datenmodell d(anz_gitterpkt, k, zeitreihenl, Jahr, type);
      algoTesten(MODUS = 3, d, data, amplitude, k, n, m);
      d.clear();
   }
   if (CLFDN)
   {
      counterFrechet = 0;
      Datenmodell d(anz_gitterpkt, k, zeitreihenl, Jahr, type);
      algoTesten(MODUS = 4, d, data, amplitude, k, n, m);
      d.clear();
   }
   if (CL)
   {
      Datenmodell d(anz_gitterpkt, k, zeitreihenl, Jahr, type);
      algoTesten(MODUS = 6, d, data, amplitude, k, n, m);
      d.clear();
   }
   CHECK = false;
}