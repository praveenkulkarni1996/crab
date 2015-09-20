#ifndef CALLGRAPH_BOOST_GRAPH_TRAITS_HPP__
#define CALLGRAPH_BOOST_GRAPH_TRAITS_HPP__

/* Convert a CallGraph into BGL graph */

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>

#include <crab/cg/Cg.hpp>

namespace boost {

  template<class CFG>
  struct graph_traits < crab::cg::CallGraph<CFG> >  {

    typedef crab::cg::CallGraph<CFG> cg_t;

    typedef typename cg_t::node_t vertex_descriptor;
    typedef typename cg_t::edge_t edge_descriptor;
    typedef typename cg_t::node_iterator vertex_iterator;
    typedef typename cg_t::pred_iterator in_edge_iterator;
    typedef typename cg_t::succ_iterator out_edge_iterator;
    
    typedef disallow_parallel_edge_tag edge_parallel_category;
    typedef bidirectional_tag directed_category;
    struct  this_graph_tag : virtual bidirectional_graph_tag, 
                             virtual vertex_list_graph_tag {};
    typedef this_graph_tag traversal_category;
    
    typedef size_t vertices_size_type;
    typedef size_t edges_size_type;
    typedef size_t degree_size_type;

    static vertex_descriptor null_vertex() { 
      vertex_descriptor n;
      return n; 
    }    

  }; // end class graph_traits

}

namespace crab { namespace cg {

   // --- Functions for crab::cg::CallGraph<CFG>

   template<class CFG> 
   typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_descriptor
   source (typename graph_traits< crab::cg::CallGraph<CFG> >::edge_descriptor e, 
             const crab::cg::CallGraph<CFG> &g) {
     return e.Src (); 
  } 

  template<class CFG>
  typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_descriptor
  target (typename graph_traits< crab::cg::CallGraph<CFG> >::edge_descriptor e, 
          const crab::cg::CallGraph<CFG> &g) {
    return e.Dest ();
  }

  template<class CFG>
  std::pair< typename graph_traits< crab::cg::CallGraph<CFG> >::in_edge_iterator, 
             typename graph_traits< crab::cg::CallGraph<CFG> >::in_edge_iterator >
  in_edges (typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_descriptor v, 
            const crab::cg::CallGraph<CFG> &g) {
    return g.preds (v);
  }

  template<class CFG>
  typename graph_traits< crab::cg::CallGraph<CFG> >::degree_size_type
  in_degree (typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_descriptor v, 
             const crab::cg::CallGraph<CFG> &g) {
    return g.num_preds (v);
  }

  template<class CFG>
  std::pair< typename graph_traits< crab::cg::CallGraph<CFG> >::out_edge_iterator, 
             typename graph_traits< crab::cg::CallGraph<CFG> >::out_edge_iterator >
  out_edges (typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_descriptor v, 
             const crab::cg::CallGraph<CFG> &g) {
    return g.succs (v);
  }

  template<class CFG>
  typename graph_traits< crab::cg::CallGraph<CFG> >::degree_size_type
  out_degree (typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_descriptor v, 
              const crab::cg::CallGraph<CFG> &g) { 
    return g.num_succs (v);
  }

  template<class CFG>
  typename graph_traits< crab::cg::CallGraph<CFG> >::degree_size_type
  degree (typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_descriptor v, 
          const crab::cg::CallGraph<CFG> &g) {
    return g.num_preds (v) + g.num_succs (v);
  }

  template<class CFG>
  std::pair<typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_iterator, 
            typename graph_traits< crab::cg::CallGraph<CFG> >::vertex_iterator > 
  vertices (const crab::cg::CallGraph<CFG> &g) {
    return g.nodes ();
  }
  
  template<class CFG>
  typename graph_traits< crab::cg::CallGraph<CFG> >::vertices_size_type
  num_vertices (const crab::cg::CallGraph<CFG> &g) {
    return g.num_nodes ();
  }


} } // end namespaces

#endif 
