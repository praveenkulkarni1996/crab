#include <ikos/tests/Cfg_impl.hpp>
#include <ikos/cfg/VarFactory.hpp>
#include <ikos/cfg/ConcSys.hpp>
#include <ikos/analysis/ConcAnalyzer.hpp>
#include <ikos/common/types.hpp>
#include <ikos/domains/intervals.hpp>                      

using namespace std;

namespace domain_impl
{
  using namespace cfg_impl;
  // Numerical domains
  typedef interval_domain< z_number, varname_t >             interval_domain_t;
  typedef DBM< z_number, varname_t >                         dbm_domain_t;
} // end namespace

using namespace cfg_impl;
using namespace domain_impl;
using namespace analyzer;
using namespace conc;

typedef ConcSys< basic_block_label_t, varname_t> conc_sys_t;
typedef conc_sys_t::thread_t thread_t;

cfg_t thread1 (VariableFactory &vfac) 
{

  ////
  // Building the CFG
  ////

  // Definining program variables
  z_var x (vfac ["x"]);
  z_var y (vfac ["y"]);
  z_var z (vfac ["z"]);
  
  // entry and exit block
  cfg_t cfg ("entry","ret");
  // adding blocks
  basic_block_t& entry = cfg.insert ("entry");
  basic_block_t& bb1   = cfg.insert ("bb1");
  basic_block_t& bb1_t = cfg.insert ("bb1_t");
  basic_block_t& bb1_f = cfg.insert ("bb1_f");
  basic_block_t& bb2   = cfg.insert ("bb2");
  basic_block_t& bb3_t   = cfg.insert ("bb3_t");
  basic_block_t& bb3_f   = cfg.insert ("bb3_f");
  basic_block_t& bb4   = cfg.insert ("bb4");
  basic_block_t& ret   = cfg.insert ("ret");
  // adding control flow
  entry >> bb1;
  bb1 >> bb1_t; bb1 >> bb1_f;
  bb1_t >> bb2; 
  bb2 >> bb3_t; bb2 >> bb3_f; 
  bb3_t >> bb4; bb3_f >> bb4; bb4 >> bb1; 
  bb1_f >> ret;
  // adding statements
  bb1_t.assume (x <= y-1); // Narrowing cannot infer these constraints (we need thresholds!)
  bb3_t.assume (x <= y-1);
  bb3_t.add (x, x, 1);
  bb3_t.add (z, z, 1);
  bb3_f.assume (x >= y+ 1);
  return cfg;
}

cfg_t thread2 (VariableFactory &vfac) 
{

  ////
  // Building the CFG
  ////

  // Definining program variables
  z_var x (vfac ["x"]);
  z_var y (vfac ["y"]);
  z_var z (vfac ["z"]);
  z_var w (vfac ["w"]);
  // entry and exit block
  cfg_t cfg ("entry","ret");
  // adding blocks
  basic_block_t& entry = cfg.insert ("entry");
  basic_block_t& bb1   = cfg.insert ("bb1");
  basic_block_t& bb1_t = cfg.insert ("bb1_t");
  basic_block_t& bb1_f = cfg.insert ("bb1_f");
  basic_block_t& bb2   = cfg.insert ("bb2");
  basic_block_t& bb3_t   = cfg.insert ("bb3_t");
  basic_block_t& bb3_f   = cfg.insert ("bb3_f");
  basic_block_t& bb4_t   = cfg.insert ("bb4_t");
  basic_block_t& bb4_f   = cfg.insert ("bb4_f");
  basic_block_t& bb5   = cfg.insert ("bb5");
  basic_block_t& bb6   = cfg.insert ("bb6");
  basic_block_t& ret   = cfg.insert ("ret");
  // adding control flow
  entry >> bb1;
  bb1 >> bb1_t; bb1 >> bb1_f;
  bb1_t >> bb2; 
  bb2 >> bb3_t; bb2 >> bb3_f; 
  bb3_t >> bb4_t; 
  bb3_t >> bb4_f; 
  bb4_t >> bb5;
  bb4_f >> bb5;
  bb5 >> bb6;
  bb3_f >> bb6; bb6 >> bb1; 
  bb1_f >> ret;
  // adding statements
  bb1_t.assume (y <= 102); // Narrowing cannot infer these constraints (we need thresholds!)
  bb3_t.assume (y <= 99);
  bb4_t.assume (z >= 0);
  bb4_t.assign (w, 1);
  bb4_f.assume (z <= 0);
  bb4_f.assign (w, 3);
  bb5.add (y, y , w);
  bb3_f.assume (y >= 100);
  return cfg;
}


int main (int argc, char** argv )
{
  VariableFactory vfac;

  cfg_t t1 = thread1 (vfac);
  t1.simplify ();

  cfg_t t2 = thread2 (vfac);
  t2.simplify ();

  conc_sys_t concSys;
  vector<varname_t> shared_vars;
  shared_vars.push_back (vfac ["x"]);
  shared_vars.push_back (vfac ["y"]);

  concSys.add_thread (&t1, shared_vars.begin (), shared_vars.end ());
  concSys.add_thread (&t2, shared_vars.begin (), shared_vars.end ());

  cout << concSys << endl;

  const bool run_live = true;
  auto global_inv = interval_domain_t::top ();
  global_inv.assign (vfac ["x"], interval_domain_t::linear_expression_t (0));
  global_inv.assign (vfac ["y"], interval_domain_t::linear_expression_t (0));

  typedef ConcAnalyzer <cfg_t,interval_domain_t,VariableFactory> conc_analyzer_t;

  conc_analyzer_t a (concSys, vfac, run_live);
  a.Run (global_inv);
  
  cout << "Thread 1\n";
  {  
    conc_analyzer_t::inv_map_t &inv_map = a.getInvariants (&t1);
    for (auto p : inv_map)
      cout << p.first << ": " << p.second << endl;
  }

  cout << "Thread 2\n";
  {  
    conc_analyzer_t::inv_map_t &inv_map = a.getInvariants (&t2);
    for (auto p : inv_map)
      cout << p.first << ": " << p.second << endl;
  }

  return 0;
}
