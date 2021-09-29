extern bool DEBUGG_optional;
extern bool DEBUGG_algo;

#define LINKS 0
#define OBEN 1
#define RECHTS 2
#define UNTEN 3

using namespace std;

class MinHeap
{
    double *heap_abstand;
    int *heap_ClusterID;
    int *heap_NachbarClusterID;
    std::unordered_map<int, int> map;

    int size;
    int capacity;

public:
    MinHeap(int c)
    {
        size = 0;
        capacity = c;
        heap_abstand = (double *)malloc(c * sizeof(double *));
        heap_ClusterID = (int *)malloc(c * sizeof(int *));
        heap_NachbarClusterID = (int *)malloc(c * sizeof(int *));
    }

    void clear()
    {
        free(heap_NachbarClusterID);
        free(heap_abstand);
        free(heap_ClusterID);
        map.clear();
    }

    int left(int i) { return (2 * i + 1); }
    int right(int i) { return (2 * i + 2); }
    int parent(int i) { return (i - 1) / 2; }

    /**
     * @brief Tauchst die Elemente im MinHeap
     * http://geeksquiz.com/binary-heap/
     */
    void tauschen(const int x, const int y)
    {
        int id1_first, id1_second;
        int id2_first, id2_second;
        auto id = map.find(heap_ClusterID[x]);
        id1_first = id->first;
        id1_second = id->second;
        id = map.find(heap_ClusterID[y]);
        id2_first = id->first;
        id2_second = id->second;

        map[id1_first] = id2_second;
        map[id2_first] = id1_second;

        swap(heap_abstand[x], heap_abstand[y]);
        swap(heap_ClusterID[x], heap_ClusterID[y]);
        swap(heap_NachbarClusterID[x], heap_NachbarClusterID[y]);
    }

    /**
     * @brief Fuegt neues Element dem MinHeap hinzu
     * @param abstand Frechet Abstand zum naehsten Clusten
     * @param cluster das betrachtete Cluster
     * @param benachbartesCluster das Nachbarcluser von cluster
     * http://geeksquiz.com/binary-heap/
     */
    void insert(const double abstand, const int cluster, const int benachbartesCluster)
    {
        if (size == capacity)
        {
            return;
        }
        size++;
        heap_abstand[size - 1] = abstand;
        heap_ClusterID[size - 1] = cluster;
        heap_NachbarClusterID[size - 1] = benachbartesCluster;

        map.insert({cluster, size});

        int i = size - 1;

        while (i != 0 && heap_abstand[parent(i)] > heap_abstand[i])
        {
            tauschen(i, parent(i));

            i = parent(i);
        }
    }

    /**
     * @brief Wenn sich Gewicht vergroessert hat kann der Schluessel im 
     * MinHeap absteigen
     * @source https://www.codeproject.com/tips/816934/min-binary-heap-implementation-in-cplusplus
     */
    void BubbleDown(int index)
    {
        // std::cout << "BubbleDown" << std::endl;
        int leftChildIndex = left(index);
        int rightChildIndex = right(index);

        int id1_first, id1_second;
        int id2_first, id2_second;

        if (leftChildIndex > size - 1)
            return; //index is a leaf

        int minIndex = index;
        bool test = true;

        if (heap_abstand[index] > heap_abstand[leftChildIndex])
        {
            minIndex = leftChildIndex;
            test = true;
        }

        if ((rightChildIndex < size) && (heap_abstand[minIndex] > heap_abstand[rightChildIndex]))
        {
            minIndex = rightChildIndex;
            test = false;
        }

        if (minIndex != index)
        {
            //need to swap
            auto id = map.find(heap_ClusterID[index]);
            id1_first = id->first;
            id1_second = id->second;

            if (test)
            {
                id = map.find(heap_ClusterID[left(index)]);
                id2_first = id->first;
                id2_second = id->second;
            }
            else
            {
                id = map.find(heap_ClusterID[right(index)]);
                id2_first = id->first;
                id2_second = id->second;
            }
            map[id1_first] = id2_second;
            map[id2_first] = id1_second;

            swap(heap_abstand[index], heap_abstand[minIndex]);
            swap(heap_ClusterID[index], heap_ClusterID[minIndex]);
            swap(heap_NachbarClusterID[index], heap_NachbarClusterID[minIndex]);
            BubbleDown(minIndex);
        }
    }

    /**
     * @brief Aendern der ClusterID in der Map
     */
    void mapKorrigieren(const int neueclid, const int alteclid)
    {
        int id1_first, id1_second;
        int id2_first, id2_second;

        if (alteclid < neueclid)
        {
            auto tt = map.find(neueclid);
            id1_first = tt->first;
            id1_second = tt->second;

            tt = map.find(alteclid);
            id2_first = tt->first;
            id2_second = tt->second;

            map[id1_first] = id2_second;
            map[id2_first] = id1_second;
        }
    }

    /**
     * @brief Wenn sich Gewicht vergroessert hat kann der Schluessel im MinHeap
     * absteigen.
     * @source https://www.codeproject.com/tips/816934/min-binary-heap-implementation-in-cplusplus
     */
    void BubbleUp(int index)
    {
        int id1_first, id1_second;
        int id2_first, id2_second;

        if (index == 0)
            return;

        int parentIndex = parent(index);

        if (heap_abstand[parentIndex] > heap_abstand[index])
        {
            auto id = map.find(heap_ClusterID[index]);
            id1_first = id->first;
            id1_second = id->second;
            id = map.find(heap_ClusterID[parent(index)]);
            id2_first = id->first;
            id2_second = id->second;

            map[id1_first] = id2_second;
            map[id2_first] = id1_second;

            swap(heap_abstand[index], heap_abstand[parentIndex]);
            swap(heap_ClusterID[index], heap_ClusterID[parentIndex]);
            swap(heap_NachbarClusterID[index], heap_NachbarClusterID[parentIndex]);

            BubbleUp(parentIndex);
        }
    }

    /**
     * @brief Erhaelt die MinHeap Eigenschaft.
     * @param i 
     * http://geeksquiz.com/binary-heap/
     */
    void minHeapify(const int i)
    {
        int lt = left(i);
        int rt = right(i);
        int smallest = i;

        if (lt < size && heap_abstand[lt] < heap_abstand[i])
            smallest = lt;
        if (rt < size && heap_abstand[rt] < heap_abstand[smallest])
            smallest = rt;
        if (smallest != i)
        {
            tauschen(i, smallest);
            minHeapify(smallest);
        }
    }

    /**
     * @brief Loescht den kleinsten/obersten Eintrag im MinHeap
     * @return das clusterID 
     * http://geeksquiz.com/binary-heap/
     */
    int extractMin()
    {
        if (size <= 0)
            return INT_MAX;
        if (size == 1)
        {
            size--;
            return heap_abstand[0];
        }

        tauschen(0, size - 1);

        size--;
        minHeapify(0);

        return heap_ClusterID[size];
    }

    int get_ClusterIdHeap(const int schluessel)
    {
        return heap_ClusterID[schluessel];
    }

    void set_ClusterIdHeap(const int schluessel, const int neu)
    {
        heap_ClusterID[schluessel] = neu;
    }

    int get_minHeap()
    {
        if (size >= 1)
        {
            return heap_NachbarClusterID[0];
        }
        return INT_MAX;
    }

    int get_map(const int key)
    {
        return map[key];
    }

    int get_size()
    {
        return size;
    }

    double get_heap_gewichte(const int schluessel)
    {
        return heap_abstand[schluessel];
    }

    double get_last_heap_gewichte()
    {
        return heap_abstand[0];
    }

    void set_heap_gewichte(const int schluessel, const double wert)
    {
        heap_abstand[schluessel] = wert;
    }

    int get_heap_Gitterpunkt(const int schluessel)
    {
        return heap_NachbarClusterID[schluessel];
    }

    void set_heap_Gitterpunkt(const int schluessel, const int knoten)
    {
        heap_NachbarClusterID[schluessel] = knoten;
    }

    /**
     * http://geeksquiz.com/binary-heap/
     */
    void decreaseKey(int i, int x)
    {
        heap_abstand[i] = x;

        while (i != 0 && heap_abstand[parent(i)] > heap_abstand[i])
        {
            tauschen(i, parent(i));
            i = parent(i);
        }
    }
    /**
     * @brief Loescht einen Schluessel indem Schluessel i nach oben verschoben wird
     * @param i Schluessel
     * http://geeksquiz.com/binary-heap/
     */
    void deleteKey(int i)
    {
        decreaseKey(i, INT_MIN);
        extractMin();
    }

    /**
     * @brief Gibt die Reihenfolge der Cluster im Heap aus.
     */
    void print_heap_ClusterID()
    {
        if (DEBUGG_optional)
        {
            for (int i = 0; i < capacity; i++)
            {
                std::cout << setw(3) << i << ": " << heap_ClusterID[i] << std::endl;
                ;
            }
        }
    }

    /**
     * @brief Gibt die Reihenfolge der Gewichte im Heap aus.
     */
    void print_gewichte()
    {
        if (DEBUGG_optional)
        {
            for (int i = 0; i < capacity; i++)
            {
                std::cout << setw(3) << i << ": " << heap_abstand[i] << std::endl;
                ;
            }
        }
    }

    /**
     * @brief Aktuelle LookUp Tabelle wird auf Konsoale ausgegeben.
     */
    void printMap()
    {
        for (const auto &n : map)
        {
            std::cout << "ID:[" << setw(3) << n.first << "] Pos:[" << n.second << "]\n";
        }
    }

    /**
     * @brief Heap wird auf der Konsole ausgegeben.
     * 
     * @param heap_abstand 
     * @param heap_ClusterID 
     * @param heap_idbenachbart 
     */
    void print_heap()
    {
        if (DEBUGG_algo)
        {
            std::cout << "aktueller HEAP:" << std::endl;
            for (int i = 0; i < size; i++) 
            {
                cout << setw(3) << i << ":  Abstand: " << std::setprecision(4) << heap_abstand[i] << " ClusterID: " << heap_ClusterID[i];
                cout << " NachbarClusterID:" << heap_NachbarClusterID[i] << endl;
            }
        }
    }

    /**
     * @brief Berechnet aus der Eingabe min_heapIDBasis und min_heapIDNach den zugehörigen Gitterpunkt
     * 
     * @param min_heapIDNach 
     * @param min_heapIDBasis 
     * @param n 
     * @param return_richtung 
     * @param return_gitterpkt 
     */
    void get_GitterRichung(const int min_heapIDNach, const int min_heapIDBasis, const int n, int &return_gitterpkt)
    {
        switch (min_heapIDNach)
        {
        case LINKS:
            return_gitterpkt = min_heapIDBasis - 1;
            break;
        case OBEN:
            return_gitterpkt = min_heapIDBasis - n;
            break;
        case RECHTS:
            return_gitterpkt = min_heapIDBasis + 1;
            break;
        case UNTEN:
            return_gitterpkt = min_heapIDBasis + n;
            break;
        default:
            std::cout << RED << "ERROR:" << RESET << " NACHBAR" << std::endl;
            break;
        }
    }

    /**
     * @brief Initialiseren des Heaps über n*m x 4 Matrix
     */
    void initialisierenHeap(double *direkte4Nachbarn, const int n, const int m)
    {
        double min;
        double curr;
        int nb;
        int gitter_pkt;
        zeitMessen_start();
        for (int i = 0; i < n * m; i++)
        {
            min = INT_MAX;
            for (int j = 0; j < 4; j++)
            {
                curr = direkte4Nachbarn[i * 4 + j];
                if (curr < min)
                {
                    min = curr;
                    nb = j;
                }
            }
            get_GitterRichung(nb, i + 1, n, gitter_pkt);
            insert(min, i + 1, gitter_pkt);
        }
        zeitMessen_ende();
    }
    // BACKUP
    // void erstelle_map()
    // {
    //     map.clear();
    //     for (int i = 0; i < size; i++)
    //     {
    //         map.insert({heap_ClusterID[i], i + 1});
    //     }
    // }
};

/**
 * @brief Anstatt Cluster zu loeschen und neu einzufuegen wird geschaut, ob es noch oben oder
 * nach unten verschoben muss.
 */
void set_neuesNachbarCluster(MinHeap h, const int pos_heap, const double neuer_wert, const int set_cl)
{
    double alt = h.get_heap_gewichte(pos_heap);
    h.set_heap_gewichte(pos_heap, neuer_wert);
    h.set_heap_Gitterpunkt(pos_heap, set_cl);
    if (alt > neuer_wert && pos_heap != 0)
    {
        h.BubbleUp(pos_heap);
    }
    else
    {
        h.BubbleDown(pos_heap);
    }
}

/**
 * @brief testmethode für heap.cc
 */
int main_zumtesten_MainHeap()
{
    int n = 3;

    MinHeap h(n * n);

    //abstand ID
    h.insert(8, 1, 100);
    h.insert(4, 2, 101);
    h.insert(9, 3, 102);
    h.insert(3, 4, 103);
    h.insert(5, 5, 104);
    h.insert(6, 6, 105);
    h.insert(1, 7, 106);
    h.print_heap();

    h.printMap();

    h.set_heap_gewichte(3, 2);
    h.set_heap_Gitterpunkt(3, 90);
    h.minHeapify(3);

    h.print_heap();
    h.printMap();

    std::cout << "size:" << h.get_size() << std::endl;
    int test = h.extractMin();
    std::cout << "test" << test << std::endl;
    h.print_heap();

    h.printMap();
    std::cout << "size:" << h.get_size() << std::endl;

    int pos_heap = h.get_map(1);
    std::cout << "pos_heap" << pos_heap << std::endl;

    h.deleteKey(pos_heap - 1);

    h.print_heap();

    h.printMap();

    std::cout << "size:" << h.get_size() << std::endl;

    h.insert(4, 1, 200);

    h.print_heap();

    h.printMap();

    std::cout << "size:" << h.get_size() << std::endl;

    // h.update(0, 6, heap_abstand, heap_ClusterID, heap_idbenachbart, map);

    h.clear();
    std::cout << "Done" << std::endl;

    return 0;
}
