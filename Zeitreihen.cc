using namespace std;
using namespace boost;
#include <ctime>
#include <boost/math/constants/constants.hpp>

typedef property<edge_weight_t, double> Weight;
typedef adjacency_list<listS, vecS, undirectedS, no_property, Weight> UndirectedGraph;

extern bool DEBUGG_optional;
extern bool DEBUGG_algo;
extern double zeit_global;
extern bool SEED;
extern struct timespec begintt, endtt;

/**
 * @brief Get current date/time, format is YYYY-MM-DD, HH:mm:ss
 * Only used in print statement in the created files.
 * @source https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c 
 */
const std::string currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d, %X", &tstruct);

    return buf;
}

/**
 * @brief Returned die Zeit als String mit passeneden Sekunde/Minuten/Stunden Format
 */
std::string get_stringTime(const double time_exp1)
{
    if (((time_exp1 / 60) / 60) > 1)
    {
        return to_string((time_exp1 / 60) / 60) + " Stunden ";
    }
    else if ((time_exp1 / 60) > 1)
    {
        return to_string(time_exp1 / 60) + " Minuten ";
    }
    else
    {
        return to_string(time_exp1) + " Sekunden ";
    }
    return "ERROR";
}

/**
 * @brief Methode startet die Zeit Messung
 * @source https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9
 */
void zeitMessen_start()
{
    begintt = {0, 0};
    endtt = {0, 0};
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &begintt); //CLOCK_REALTIME   CLOCK_PROCESS_CPUTIME_ID
}

/**
 * @brief Methode beendet die Zeit Messung
 */
void zeitMessen_ende()
{
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endtt);
    long seconds = endtt.tv_sec - begintt.tv_sec;
    long nanoseconds = endtt.tv_nsec - begintt.tv_nsec;
    zeit_global += seconds + nanoseconds * 1e-9;
    //  std::cout << "CPU time used: " << seconds + nanoseconds * 1e-9 << " ms\n";
}

/**
 * @brief Methode resetet die globale Variable fuer die Zeit Messung
 */
void reset_time()
{
    zeit_global = 0.;
}

/**
 * @brief Die Klasse enthaelt alle Methoden um den Zufall zu erzeigen und reproduzieren
 * zu koennen.
 */
class GeneratorSeed
{
    std::mt19937 rng;
    std::default_random_engine generator;

public:
    GeneratorSeed(int zeit)
    {
        if (SEED)
        {
            std::mt19937 ttt(zeit);
            rng = ttt;
            srand(zeit);
        }
        else
        {
            std::random_device rd;
            std::mt19937 ttt(rd());
            rng = ttt;
            generator.seed(rd());
        }
    }

    std::mt19937 get_samen()
    {
        return rng;
    }

    double double_normal(const double norm_id, const double norm_sigma)
    {
        std::normal_distribution<double> distribution(norm_id, norm_sigma);
        return distribution(generator);
    }

    double random_int_Intervall(const double d1, const double d2)
    {
        std::uniform_int_distribution<int> uni(d1, d2);
        return uni(rng);
    }

    double random_double_Intervall(const double d1, const double d2)
    {

        std::uniform_real_distribution<float> distr(d1, d2);
        return distr(rng);
    }

    int random_int()
    {
        return rand();
    }
};

/**
 * @brief Jeder Knoten ist zu höchsten 4 direkten Nachbarn benachbart. Diese Methode
 * gibt die aktuellen Frechet Abstaende jedes Punktes zu seinen direkten Nachbarn aus.
 * 
 * @param direkte4Nachbarn speichert alle direkte Nachbar Abstände
 * @param n Anzahl Spalten des Gitters
 * @param m Anzahl Zeilen des Gitters
 */
void printDirekte4Nachbarn(const double *direkte4Nachbarn, const int n, const int m)
{
    if (DEBUGG_optional)
    {
        std::cout << "Direkte Frechet Abstände:" << std::endl;
        for (int i = 0; i < n * m; i++)
        {
            std::cout << setw(3) << i + 1 << ":  ";
            for (int j = 0; j < 4; j++)
            {
                // std::cout.precision(2);
                if (direkte4Nachbarn[i * 4 + j] != INT_MAX)
                {
                    std::cout << std::setprecision(4) << direkte4Nachbarn[i * 4 + j] << " ";
                }
                else
                {
                    std::cout << "X ";
                }
                if (j != 3)
                {
                    std::cout << "|| ";
                }
            }
            std::cout << std::endl;
        }
    }
}

/**
 * @brief Gibt die Zeitreihe auf der Konsole aus
 * 
 * @param zeitreihenMatrix Matrix enthaelt alle Zeitreihen aller Gitterpunkte
 * @param zeitreihenl laenge der Zeitreihe
 * @param n Anzahl Spalten des Gitters
 * @param m Anzahl Zeilen des Gitters
 */
void print_Zeitreihe(const double *zeitreihenMatrix, int zeitreihenl, const int n, const int m)
{
    if (DEBUGG_optional)
    {
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
 * @brief Methode prüft ob man auf die Datei zugreifen kann.
 */
bool check_datei(const std::string pfad)
{
    std::ifstream datei(pfad);
    if (!datei)
    {
        // std::cerr << RED << "Error:" << RESET << "Datei beim Oeffnen der Datei " << pfad << "\n";
        return false;
    }
    return true;
}

/**
 * @brief Liest die Zeitreihen von Station auswahl ein und checkt ob die Laenge der 
 * Zeitreihe
 * @return true: a > Zeitreihenl/2
 * @return false a <= Zeitreihenl/2
 */
bool einlesen_ExterneZeitreihen(const int auswahl, const std::string str, const int start_jahr, const int length)
{
    std::string line;
    std::string eintrag;
    const std::string delim = ";";
    std::string filename = "srcPSMSL/" + str + "/data/" + to_string(auswahl) + ".rlrdata";
    std::ifstream input(filename);
    if (check_datei(filename))
    {
        size_t pos = 0;
        int a = 0;
        int daten_jahr = 0;
        int daten_wert = 0;

        while (std::getline(input, line))
        {
            int count_semi = 1;
            while ((pos = line.find(delim)) != std::string::npos)
            {
                eintrag = line.substr(0, pos);
                line.erase(0, pos + delim.length());
                if (count_semi == 1)
                {
                    daten_jahr = std::stod(eintrag);
                    count_semi++;
                }
                else if (count_semi == 2)
                {
                    daten_wert = std::stod(eintrag);

                    if (daten_wert > 1 && daten_wert != -99999 && daten_jahr >= start_jahr)
                    {
                        a++;
                    }
                    if (a > (length / 2))
                    {
                        return true;
                    }
                    break;
                }
            }
        }
    }
    return false;
}

/**
 * @brief Diese Methode liest Zeitreihen Datensatz von Permenat Service for Mean Sea Level ein.
 */
void einlesen_ExterneZeitreihenRANDOM(double *zeitreihenMatrix, const int n, const int m, const int zeitreihenl, const std::string str, const int start_jahr, GeneratorSeed &seed_samen)
{
    std::string line;
    std::string eintrag;
    const std::string delim = ";";
    const int anzahl_an_zeitreihen = 2360;

    int index_array = 0;
    int i = 1;

    while (index_array < n * m)
    {
        // std::cout << "i:" << i << std::endl;
        i = seed_samen.random_int() % anzahl_an_zeitreihen;
        std::string filename = "srcPSMSL/" + str + "/data/" + to_string(i) + ".rlrdata";

        //zeitreihen werden in Matrixform eingelesen
        std::ifstream input(filename);
        if (einlesen_ExterneZeitreihen(i, str, start_jahr, zeitreihenl))
        {
            size_t pos = 0;
            int a = 0;
            int daten_jahr = 0;
            int daten_wert = 0;

            while (std::getline(input, line))
            {
                // std::cout << line << '\n';
                int count_semi = 1;
                while ((pos = line.find(delim)) != std::string::npos)
                {
                    eintrag = line.substr(0, pos);
                    line.erase(0, pos + delim.length());
                    if (count_semi == 1)
                    {
                        daten_jahr = std::stod(eintrag);
                        count_semi++;
                    }
                    else if (count_semi == 2)
                    {
                        daten_wert = std::stod(eintrag);

                        // std::cout << i << ":" << daten_jahr << " " << daten_wert << std::endl;

                        if (a < zeitreihenl && daten_wert > 1 && daten_jahr >= start_jahr)
                        {
                            zeitreihenMatrix[index_array * zeitreihenl + a] = daten_wert;
                            a++;
                        }
                        break;
                    }
                }
            }
            index_array++;
        }
    }
}

/**
 * @brief Liest die Zeitreihen ein. Behaelt die Lokalitaetsstrucktur. Nur Stationen mit
 * einer Zeitreihenlaenge > Zeitreihenl/2 werden genommen.
 */
void einlesen_ExterneZeitreihen(double *zeitreihenMatrix, int *NachbarStationen, const int auswahl, const int zeitreihenl, const std::string str, const int start_jahr)
{
    std::string line;
    std::string eintrag;
    const std::string delim = ";";

    int index_array = 0;

    for (int i = 0; i < auswahl; i++)
    {
        index_array = NachbarStationen[i * 6];
        std::string filename = "srcPSMSL/" + str + "/data/" + to_string(index_array) + ".rlrdata";
        std::ifstream input(filename);
        std::cout << "filename:" << filename << std::endl;
        if (check_datei(filename))
        {
            size_t pos = 0;
            int a = 0;
            int daten_jahr = 0;
            int daten_wert = 0;

            while (std::getline(input, line))
            {
                // std::cout << line << '\n';
                int count_semi = 1;
                while ((pos = line.find(delim)) != std::string::npos)
                {
                    eintrag = line.substr(0, pos);
                    line.erase(0, pos + delim.length());
                    if (count_semi == 1)
                    {
                        daten_jahr = std::stod(eintrag);
                        count_semi++;
                    }
                    else if (count_semi == 2)
                    {
                        daten_wert = std::stod(eintrag);

                        if (a < zeitreihenl && daten_wert > 1 && daten_jahr >= start_jahr) //daten_jahr > 1980
                        {
                            zeitreihenMatrix[i * zeitreihenl + a] = daten_wert;
                            a++;
                            // std::cout << daten_jahr << " ";
                            // std::cout << daten_wert << " ";
                        }
                        break;
                    }
                }
            }
        }
    }
}

/**
 * @brief Zeitreihen werden synthehisch erstellt.
 * Man kann einfach weitere Zeitreihen hinzufuegen.
 */
void erstellenSynthetischeZeitreihen(double *zeitreihenMatrix, const int *muster_union, const int n, const int m, const int k, const int zeitreihenl, const std::string SETTING, const double a, GeneratorSeed &seed_samen)
{
    int freq;
    double phase;
    double amplitude;
    int id = 0;
    int index_array;
    const double pi = boost::math::constants::pi<double>();
    double help3;

    double *f_groundTrouth;
    f_groundTrouth = (double *)malloc(k * sizeof(double *));

    double *a_groundTrouth;
    a_groundTrouth = (double *)malloc(k * sizeof(double *));

    double *p_groundTrouth;
    p_groundTrouth = (double *)malloc(k * sizeof(double *));

    double *zeitreihe_groundTrouth;
    zeitreihe_groundTrouth = (double *)malloc(k * zeitreihenl * sizeof(double *));

    if (SETTING == "APPROX_GT_type1")
    {
        for (int i = 0; i < k; i++)
        {
            a_groundTrouth[i] = (int)seed_samen.random_double_Intervall(-(a), (a));
            f_groundTrouth[i] = seed_samen.random_int() % 10;
            p_groundTrouth[i] = seed_samen.random_double_Intervall(0, 2 * pi);
        }
        for (int i = 0; i < k; i++)
        {
            for (int h = 0; h < zeitreihenl; h++)
            {
                zeitreihe_groundTrouth[i * zeitreihenl + h] = a_groundTrouth[i] * std::sin(f_groundTrouth[i] * h + p_groundTrouth[i]);
            }
        }
    }
    if (SETTING == "APPROX_GT_type2b")
    {
        for (int i = 0; i < k; i++)
        {
            a_groundTrouth[i] = (int)seed_samen.random_double_Intervall(-(0.25), (0.25));
            f_groundTrouth[i] = seed_samen.random_int() % 10;
            p_groundTrouth[i] = seed_samen.random_double_Intervall(0, 2 * pi);
        }
        for (int i = 0; i < k; i++)
        {
            for (int h = 0; h < zeitreihenl; h++)
            {
                zeitreihe_groundTrouth[i * zeitreihenl + h] = a_groundTrouth[i] * std::sin(f_groundTrouth[i] * h + p_groundTrouth[i]);
            }
        }
    }
    if (SETTING == "APPROX_GT_type2c")
    {
        for (int i = 0; i < k; i++)
        {
            a_groundTrouth[i] = (int)seed_samen.random_double_Intervall(-(k), (k));
            f_groundTrouth[i] = seed_samen.random_int() % 10;
            p_groundTrouth[i] = seed_samen.random_double_Intervall(0, 2 * pi);
        }
        for (int i = 0; i < k; i++)
        {
            for (int h = 0; h < zeitreihenl; h++)
            {
                zeitreihe_groundTrouth[i * zeitreihenl + h] = a_groundTrouth[i] * std::sin(f_groundTrouth[i] * h + p_groundTrouth[i]);
            }
        }
    }
    if (SETTING == "APPROX_GT_type2a")
    {
        for (int i = 0; i < k; i++)
        {
            a_groundTrouth[i] = (int)seed_samen.random_double_Intervall(-(0.25), (0.25));
            f_groundTrouth[i] = seed_samen.random_int() % 10;
            p_groundTrouth[i] = seed_samen.random_double_Intervall(0, 2 * pi);
        }
        for (int i = 0; i < k; i++)
        {
            for (int h = 0; h < zeitreihenl; h++)
            {
                zeitreihe_groundTrouth[i * zeitreihenl + h] = i + a_groundTrouth[i] * std::sin(f_groundTrouth[i] * h + p_groundTrouth[i]);
            }
        }
    }
    if (SETTING == "APPROX_GT_type4")
    {
        for (int i = 0; i < k; i++)
        {
            a_groundTrouth[i] = (int)seed_samen.random_double_Intervall(0, zeitreihenl);
            f_groundTrouth[i] = seed_samen.random_int() % 10;
            p_groundTrouth[i] = seed_samen.random_double_Intervall(0, 2 * pi);
        }
        for (int i = 0; i < k; i++)
        {
            for (int h = 0; h < zeitreihenl; h++)
            {
                zeitreihe_groundTrouth[i * zeitreihenl + h] = a_groundTrouth[i] * std::sin(f_groundTrouth[i] * h + p_groundTrouth[i]);
            }
        }
    }
    if (SETTING == "RandomWalk")
    {
        for (int i = 0; i < k; i++)
        {
            a_groundTrouth[i] = i + 1;
        }
        for (int i = 0; i < k; i++)
        {
            help3 = a_groundTrouth[i];
            for (int h = 0; h < zeitreihenl; h++)
            {
                int rand_help = seed_samen.random_int();
                int zzz;
                if (rand_help % 2 == 1)
                {
                    zzz = help3 - 1 * seed_samen.random_double_Intervall(0, m * n);
                }
                else
                {
                    zzz = help3 + 1 * seed_samen.random_double_Intervall(0, m * n);
                }
                zeitreihe_groundTrouth[i * zeitreihenl + h] = zzz;
                help3 = zzz;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (SETTING == "SINbekanntPurity")
            {
                double min_d = std::numeric_limits<double>::min();
                id = muster_union[i * m + j];
                amplitude = seed_samen.random_double_Intervall((double)-0.25 + min_d, (double)0.25 - min_d);
                freq = seed_samen.random_int() % 10;
                phase = seed_samen.random_double_Intervall(0, 2 * pi);
            }
            else if (SETTING == "SINbekanntPurityKW")
            {
                double min_d = std::numeric_limits<double>::min();
                id = muster_union[i * m + j];
                amplitude = seed_samen.random_double_Intervall((double)((double)(-0.25 + min_d) / (double)(n * m)), (double)((double)(0.25 + min_d) / (double)(n * m)));
                freq = seed_samen.random_int() % 10;
                phase = seed_samen.random_double_Intervall(0, 2 * pi);
            }
            else if (SETTING == "kontrollAmplitude")
            {
                id = muster_union[i * m + j];
                amplitude = seed_samen.random_double_Intervall(-a, a);
                freq = seed_samen.random_int() % 10;
                phase = seed_samen.random_double_Intervall(0, 2 * pi);
            }
            else if (SETTING == "RunTimeSeriesRANDOM")
            {
                amplitude = seed_samen.random_double_Intervall(-m * n, m * n);
                id = seed_samen.random_double_Intervall(0, m * n);
                freq = seed_samen.random_int() % 10;
                phase = seed_samen.random_double_Intervall(0, 2 * pi);
            }
            else if (SETTING == "APPROX_GT_type1" || SETTING == "APPROX_GT_type2a" || SETTING == "APPROX_GT_type2b" || SETTING == "APPROX_GT_type2c" || SETTING == "APPROX_GT_type4" || SETTING == "RunTimeSeriesKONSTANT")
            {
                id = muster_union[i * m + j] - 1;
            }
            else if (SETTING == "APPROX_GT_type3")
            {
                amplitude = seed_samen.random_double_Intervall(-zeitreihenl, zeitreihenl);
                id = muster_union[i * m + j] - 1;
                freq = seed_samen.random_int() % 10;
                phase = seed_samen.random_double_Intervall(0, 2 * pi);
            }
            else if (SETTING == "RandomWalk")
            {
                id = muster_union[i * m + j] - 1;
                // id = seed_samen.random_double_Intervall(0, m * n);
                // help3 = id;
            }

            index_array = (i)*m + (j - 1);
            // double jo = id + 1 + seed_samen.double_normal(0, a_groundTrouth[id] / (n * m * k)); //id+seed_samen.double_normal(0, a_groundTrouth[id] / 100);

            for (int h = 0; h < zeitreihenl; h++)
            {
                if (SETTING == "SINbekanntPurity" || SETTING == "SINbekanntPurityKW")
                {
                    zeitreihenMatrix[index_array * zeitreihenl + h] = (double)((double)id + (double)amplitude * std::sin(freq * h + phase));
                }
                else if (SETTING == "kontrollAmplitude")
                {
                    zeitreihenMatrix[index_array * zeitreihenl + h] = (double)((double)id + (double)amplitude * std::sin(freq * h + phase));
                }
                else if (SETTING == "RunTimeSeriesRANDOM")
                {
                    zeitreihenMatrix[index_array * zeitreihenl + h] = (double)(id + (double)amplitude * std::sin(freq * h + phase)) + (double)seed_samen.double_normal(0, (double)amplitude / 10);
                }
                else if (SETTING == "APPROX_GT_type1")
                {
                    double jo = id + 1 + seed_samen.double_normal(0, a_groundTrouth[id] / (n * m));                      //id+seed_samen.double_normal(0, a_groundTrouth[id] / 100);
                    zeitreihenMatrix[index_array * zeitreihenl + h] = zeitreihe_groundTrouth[id * zeitreihenl + h] + jo; //+ seed_samen.double_normal(0, std::min(a_groundTrouth[id] / (m * n), 0.25));
                }
                else if (SETTING == "APPROX_GT_type2a")
                {
                    double jo = seed_samen.double_normal(id, a);                                                         //id+seed_samen.double_normal(0, a_groundTrouth[id] / 100);
                    zeitreihenMatrix[index_array * zeitreihenl + h] = zeitreihe_groundTrouth[id * zeitreihenl + h] + jo; //+ seed_samen.double_normal(0, std::min(a_groundTrouth[id] / (m * n), 0.25));
                }
                else if (SETTING == "APPROX_GT_type2b")
                {
                    double jo = seed_samen.double_normal(0, 0.25 * (double)a / zeitreihenl);                             //id+seed_samen.double_normal(0, a_groundTrouth[id] / 100);
                    zeitreihenMatrix[index_array * zeitreihenl + h] = zeitreihe_groundTrouth[id * zeitreihenl + h] + jo; //+ seed_samen.double_normal(0, std::min(a_groundTrouth[id] / (m * n), 0.25));
                }
                else if (SETTING == "APPROX_GT_type2c")
                {
                    double jo = seed_samen.double_normal(0, (double)a * k / zeitreihenl);                                //id+seed_samen.double_normal(0, a_groundTrouth[id] / 100);
                    zeitreihenMatrix[index_array * zeitreihenl + h] = zeitreihe_groundTrouth[id * zeitreihenl + h] + jo; //+ seed_samen.double_normal(0, std::min(a_groundTrouth[id] / (m * n), 0.25));
                }
                else if (SETTING == "APPROX_GT_type3")
                {
                    zeitreihenMatrix[index_array * zeitreihenl + h] = amplitude * std::sin(freq * h + phase) + seed_samen.double_normal(0, a); //+ seed_samen.double_normal(0, std::min(a_groundTrouth[id] / (m * n), 0.25));
                }
                else if (SETTING == "APPROX_GT_type4")
                {
                    double max_a = *(std::max_element(&a_groundTrouth[0], &a_groundTrouth[k - 1]));
                    double jo = seed_samen.double_normal(0, max_a * a);
                    zeitreihenMatrix[index_array * zeitreihenl + h] = zeitreihe_groundTrouth[id * zeitreihenl + h] + jo;
                }
                else if (SETTING == "RandomWalk")
                {
                    zeitreihenMatrix[index_array * zeitreihenl + h] = zeitreihe_groundTrouth[id * zeitreihenl + h] + seed_samen.double_normal(0, zeitreihenl / 4);
                }
                else if (SETTING == "RunTimeSeriesKONSTANT")
                {
                    zeitreihenMatrix[index_array * zeitreihenl + h] = id;
                }
            }
        }
    }
    free(f_groundTrouth);
    free(a_groundTrouth);
    free(p_groundTrouth);
    free(zeitreihe_groundTrouth);
}

/**
 * @brief Methode um Zeitreihen zu erstellen, wird geschrieben in Output Datei und kann 
 * mit python Programm visualisiert werden. 
 */
void printZeitreihenThesis(const int n, const int m, int k, const int zeitreihenl, const std::string SETTING)
{
    // int freq;
    // double phase;
    // double amplitude;
    int id = 0;
    const double pi = boost::math::constants::pi<double>();
    // double help3;

    double *f_groundTrouth;
    f_groundTrouth = (double *)malloc(k * sizeof(double *));

    double *a_groundTrouth;
    a_groundTrouth = (double *)malloc(k * sizeof(double *));

    double *p_groundTrouth;
    p_groundTrouth = (double *)malloc(k * sizeof(double *));

    double *zeitreihe_groundTrouth;
    zeitreihe_groundTrouth = (double *)malloc(k * zeitreihenl * sizeof(double *));
    GeneratorSeed seed_samen(0);
    int a = sqrt(n * m);
    if (SETTING == "freespace")
    {
        for (int i = 0; i < k; i++)
        {
            a_groundTrouth[i] = seed_samen.random_double_Intervall(-(a), (a));
            f_groundTrouth[i] = (seed_samen.random_int() % 10) + 1;
            p_groundTrouth[i] = seed_samen.random_double_Intervall(0, 2 * pi);
        }
        for (int i = 0; i < k; i++)
        {
            for (int h = 0; h < zeitreihenl; h++)
            {
                zeitreihe_groundTrouth[i * zeitreihenl + h] = a_groundTrouth[i] * std::sin(f_groundTrouth[i] * h + p_groundTrouth[i]);
            }
        }
    }

    for (int i = 1; i <= 4; i++)
    {
        if (i == 1 || i == 2)
        {
            k = 1;
        }
        else
        {
            k = 2;
        }
        std::ofstream offile;
        std::string path = "results/Zeitreihen/Daten/k" + to_string(k) + SETTING + "_" + to_string(i) + ".txt";
        offile.open(path);

        if (SETTING == "freespace")
        {
            id = k - 1;
        }

        for (int h = 0; h < zeitreihenl; h++)
        {
            if (SETTING == "freespace")
            {
                double jo = id + 1 + seed_samen.double_normal(0, a_groundTrouth[id] / (n * m)); //id+seed_samen.double_normal(0, a_groundTrouth[id] / 100);
                offile << (double)h / 10 << ", ";
                offile << zeitreihe_groundTrouth[(id)*zeitreihenl + h] + jo
                       << "\n";
                std::cout << id << ":" << jo << "  " << zeitreihe_groundTrouth[(id)*zeitreihenl + h] << std::endl;
            }
        }
        offile.close();
    }
}

/**
 * @brief Schreibt Zeitreihen in bestimmtes Format.
 */
void main_printZeitreihenVisualisierung(const bool seed)
{
    SEED = seed;
    std::string SETTING;
    const int n = 3;
    const int m = 1;
    const int k = 2;
    int zeitreihenl;

    printZeitreihenThesis(n, m, k, zeitreihenl = 10, SETTING = "freespace");
}