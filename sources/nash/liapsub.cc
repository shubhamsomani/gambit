//
// FILE: liapsub.cc -- Solve efg by liap on nfg
//
// $Id$
//

#include "liapsub.h"

void efgLiapNfgSolve::SolveSubgame(const FullEfg &E, const EFSupport &sup,
				   gList<BehavSolution> &solns,
				   gStatus &p_status)
{
  BehavProfile<double> bp(sup);
  
  subgame_number++;

  gArray<int> infosets(infoset_subgames.Lengths());

  for (int pl = 1; pl <= E.NumPlayers(); pl++)  {
    int niset = 1;
    for (int iset = 1; iset <= infosets[pl]; iset++)  {
      if (infoset_subgames(pl, iset) == subgame_number)  {
	for (int act = 1; act <= bp.Support().NumActions(pl, niset); act++)
	  bp(pl, niset, act) = start(pl, iset, act);
	niset++;
      }
    }
  }

  Nfg *N = MakeReducedNfg(sup);
  NFSupport support(*N);

  ViewNormal(*N, support);

  MixedProfile<double> mp(bp);
  MixedProfile<gNumber> start(support);

  for (int pl = 1; pl <= N->NumPlayers(); pl++) {
    for (int st = 1; st <= support.NumStrats(pl); st++) {
      start(pl, st) = mp(pl, support.Strategies(pl)[st]->Number());
    }
  }

  long this_nevals, this_niters;

  gList<MixedSolution> subsolns;
  Liap(*N, params, start, subsolns, p_status, this_nevals, this_niters);

  nevals += this_nevals;

  for (int i = 1; i <= subsolns.Length(); i++)  {
    MixedProfile<gNumber> profile(*subsolns[i].Profile());
    solns.Append(BehavProfile<gNumber>(profile));
  }

  delete N;
}

efgLiapNfgSolve::efgLiapNfgSolve(const Efg::Game &E, const NFLiapParams &p,
				 const BehavProfile<gNumber> &s, int max)
  : SubgameSolver(max),
    nevals(0), subgame_number(0),
    infoset_subgames(E.NumInfosets()), params(p), start(s)
{ }

efgLiapNfgSolve::~efgLiapNfgSolve()   { }
