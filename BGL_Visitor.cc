typedef boost::graph_traits<UndirectedGraph>::vertex_descriptor Ver;
typedef boost::graph_traits<UndirectedGraph>::edge_descriptor Edge;

/**
 * @brief Die Klasse definiert einen BGL visitor um den BGL Dijkstra Algorithmus
 * bei erreichen einer bestimmen Anzahl von Punkten abzubrechen.
 * 
 *  (void)g; -> Nur um die Warnings auszuschalten
 * 
 * @source http://www.systomath.com/include/Boost-1_36/libs/graph/doc/DijkstraVisitor.html
 * @source http://www.systomath.com/include/Boost-1_36/boost/graph/dijkstra_shortest_paths.hpp
 */
class visitor_dijkstra : boost::default_dijkstra_visitor
{
protected:
   Ver destination_vertex_m;

public:
   std::set<int> set_m;
   visitor_dijkstra(Ver destination_vertex_l, std::set<int> set)
       : destination_vertex_m(destination_vertex_l), set_m(set){};
   void initialize_vertex(const Ver &s, const UndirectedGraph &g) const
   {
      (void)g;
      (void)s;
   }
   void discover_vertex(const Ver &s, const UndirectedGraph &g) const
   {
      (void)g;
      (void)s;
   }
   void examine_vertex(const Ver &s, const UndirectedGraph &g) const
   {
      (void)g;
      (void)s;
   }
   void examine_edge(const Edge &e, const UndirectedGraph &g) const
   {
      (void)e;
      (void)g;
   }
   void edge_relaxed(const Edge &e, const UndirectedGraph &g) const
   {
      (void)e;
      (void)g;
   }
   void edge_not_relaxed(const Edge &e, const UndirectedGraph &g) const
   {
      (void)e;
      (void)g;
   }

   /**
    * Wenn alle Punkt einer Menge abgearbeitet wurden, wird ein Error geworfen.
    */
   void finish_vertex(Ver &s, const UndirectedGraph &g)
   {
      (void)g;
      if (set_m.contains(s))
      {
         auto it = set_m.find(s);
         set_m.erase(it);

         if (set_m.empty())
         {
            throw(2);
         }
      }
   }
};