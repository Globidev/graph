// #include <boost/network/protocol/http/server.hpp>
// #include <iostream>

// namespace http = boost::network::http;

// struct handler;
// typedef http::server<handler> http_server;

// struct handler {
//     void operator() (http_server::request const &request,
//                      http_server::connection_ptr connection) {
//         http_server::string_type ip = source(request);
//         auto port = request.source_port;

//         std::ostringstream data;
//         data << "Hello, " << ip << ':' << port << '!';

//         connection->write(data.str());
//     }

//     void log(http_server::string_type const &info) {
//         std::cerr << "ERROR: " << info << '\n';
//     }
// };

// int main(int arg, char * argv[]) {
//     handler handler_;
//     http_server::options options(handler_);
//     http_server server_(options.address("0.0.0.0").port("8000"));
//     server_.run();
// }


#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/property_map/property_map.hpp>


#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using namespace boost;

int
main(int, char *[])
{
  typedef adjacency_list < listS, vecS, directedS,
    no_property, property < edge_weight_t, int > > graph_t;
  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  using edge_descriptor = graph_t::edge_descriptor;
  typedef std::pair<int, int> Edge;

  const int num_nodes = 5;
  enum nodes { A, B, C, D, E };
  char name[] = "ABCDE";
  Edge edge_array[] = { Edge(A, C), Edge(C, B)
  };
  int weights[] = { 1, 2 };
  int num_arcs = sizeof(edge_array) / sizeof(Edge);
  // graph_t g(edge_array, edge_array + num_arcs, weights, num_nodes);
  graph_t g;
  // property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);

  // {
  //   std::ofstream ofs("filename.grp");
  //   archive::binary_oarchive oa(ofs);
  //   oa << g;
  // }

  {
    std::ifstream ifs("filename.grp");
    archive::binary_iarchive ia(ifs);
    ia >> g;
  }

  std::vector<vertex_descriptor> p(num_vertices(g));
  std::vector<int> d(num_vertices(g));
  vertex_descriptor s = vertex(A, g);

  struct MyVisitor: default_bfs_visitor {

    void initialize_vertex(const vertex_descriptor &, const graph_t &) const {}
    void discover_vertex(const vertex_descriptor &s, const graph_t &) const {
        std::cout << "discover_vertex  " << static_cast<char>('A' + s) << std::endl;
        // if (s == C)
        // throw(2);
    }
    void examine_vertex(const vertex_descriptor &, const graph_t &) const {}
    void examine_edge(const edge_descriptor &, const graph_t &) const {}
    void edge_relaxed(const edge_descriptor &, const graph_t &) const {}
    void edge_not_relaxed(const edge_descriptor &, const graph_t &) const {}

    void finish_vertex(const vertex_descriptor &s, const graph_t &) const {
        std::cout << "finish_vertex  " << static_cast<char>('A' + s) << std::endl;
      // if (s == C)
      //   throw(2);
    }

  };

  MyVisitor v;

  try {
      dijkstra_shortest_paths(g, s,
                              predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
                              distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))).
                              visitor(v)
                              );
  }
  catch(int e) {
    std::cout << "gg" << e << std::endl;
  }

  std::cout << "distances and parents:" << std::endl;
  graph_traits < graph_t >::vertex_iterator vi, vend;
  for (boost::tie(vi, vend) = vertices(g); vi != vend; ++vi) {
    std::cout << "distance(" << name[*vi] << ") = " << d[*vi] << ", ";
    std::cout << "parent(" << name[*vi] << ") = " << name[p[*vi]] << std::
      endl;
  }
  std::cout << std::endl;

}
