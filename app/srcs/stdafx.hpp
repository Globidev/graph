// STL
#include <fstream>
#include <iostream>

// Boost
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
namespace bgl = boost;

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
namespace archive = boost::archive;

#include <boost/property_map/property_map.hpp>

#include <boost/optional.hpp>
template <class T> using Maybe = boost::optional<T>;
