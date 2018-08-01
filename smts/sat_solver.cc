#include "dreal/solver/sat_solver.h"

#include <ostream>
#include <sstream>

#include "dreal/util/assert.h"
#include "dreal/util/exception.h"
#include "dreal/util/logging.h"
#include "dreal/util/stat.h"
#include "sat_solver.h"

namespace dreal {

using std::cout;
using std::experimental::optional;
using std::set;
using std::vector;

SatSolver::SatSolver() : sat_{picosat_init()} {
  // Enable partial checks via picosat_deref_partial. See the call-site in
  // SatSolver::CheckSat().
  this->lemma_push = nullptr;
  this->lemma_pull = nullptr;
  picosat_save_original_clauses(sat_);
}

/*
SatSolver::SatSolver(void (*lemma_push)(vector<string> &clauses), void (*lemma_pull)(vector<string> &clauses)) : sat_{picosat_init()} {
    // set_lemma_push(sat_, lemma_push);
    // set_lemma_pull(sat_, lemma_pull);

    //picosat_save_original_clauses(sat_);
    this->lemma_push = lemma_push;
    this->lemma_pull = lemma_pull;
}
 */


SatSolver::SatSolver(const vector<Formula>& clauses) : SatSolver{} {
  AddClauses(clauses);
}

SatSolver::~SatSolver() { picosat_reset(sat_); }

  SatSolver* tramp_SatSolver_ptr = nullptr;

  size_t SatSolver::SatVarToId(int x) {
    //if (x < 0) return -(sat_var_to_id_[-x]);
    return /*static_cast<int>*/(sat_var_to_id_[x]);
  }

  size_t TrampSatVarToId(int x) {
    return tramp_SatSolver_ptr->SatVarToId(x); //
  }

  int SatSolver::IdToSatVar(size_t x) {
    //if (x < 0) return -(id_to_sat_var_[-x]);
    return id_to_sat_var_[x];
  }

  int TrampIdToSatVar(size_t x) {
    return tramp_SatSolver_ptr->IdToSatVar(x);
  }

  const char* SatSolver::SatVarToStr(int x) {
    if (smtsDrealParams->verbosity >= 5) {
        stringstream temp;
        temp << "[" << x << "]" << to_sym_var_[x].get_name();
        return strdup(temp.str().c_str());
    }
    //if (x >= 0) cout << "[SatSolver] The name of the variable is: " << to_sym_var_[x].get_name().c_str() << endl;
    //if (x < 0) return ("-" + to_sym_var_[-x].get_name()).c_str();
    return strdup(to_sym_var_[x].get_name().c_str());
  }

  const char* TrampSatVarToStr(int x) {
    return tramp_SatSolver_ptr->SatVarToStr(x);
  }

  int SatSolver::StrToSatVar(const char* x) {
    return str_to_sat_var_[x];
  }

  int TrampStrToSatVar(const char* x) {
    return tramp_SatSolver_ptr->StrToSatVar(x);
  }

  void SatSolver::SmtsAddLearnedClause(char* c) {
    sent_clauses[c] = false;
  }

  void TrampSmtsAddLearnedClause(char* c) {
    return tramp_SatSolver_ptr->SmtsAddLearnedClause(c);
  }

  /*

    void SatSolver::pull(PicoSAT* s){
  if (smts mi ha dato il callback smts_pull){
    vector<string> clauses_from_others;
    smts_pull(clauses_from_ohters);  // in smts: void smts_pull(vector<string>&v)
        for (string clause : clauses_from_others){
          //metti clause in s
        }
  }
}

*/


  void SatSolver::DoSmtsPull(PicoSAT* ps /*, const char*** result*/) {
    //vector<const char*> resultvector;
    if (lemma_pull) {
      vector<string> clauses_from_others;
      lemma_pull(clauses_from_others);
      for (string clause: clauses_from_others) {
        char* clausecharptr = strdup(clause.c_str());
        if ((sent_clauses.find(clausecharptr) == sent_clauses.end())
            && (received_clauses.find(clause) == received_clauses.end())) {
          //resultvector.push_back(clause.c_str());
          stringstream iss(clause);
          int lit;
          while (iss>>lit) {
            if (lit == 0) picosat_add(ps, 0);
            else if (lit > 0) picosat_add(ps, id_to_sat_var_[lit]);
            else picosat_add(ps, -id_to_sat_var_[-lit]);
          }
          received_clauses.insert(clause);
        }
        free(clausecharptr);
      }
      //*result = &resultvector[0];
      //return resultvector.size();
    }
  }

  void TrampDoSmtsPull(PicoSAT* ps) {
    tramp_SatSolver_ptr->DoSmtsPull(ps);
  }

  /*

  void SatSolver::push(PicoSAT* s){
    if (smts mi ha dato il callback smts_push){
      vector<string> my_clauses;
      for (clause in s){
        if (ho gia mandato clause in precedenza)
        continue;
        string str = string_representation(clasue);
        my_clauses.push_back(str)
      }
      smts_push(my_clauses);
    }
  }

  */

  void SatSolver::DoSmtsPush() {
    if (lemma_push) {
      vector<string> my_clauses;
      for (auto it = sent_clauses.cbegin(); it != sent_clauses.cend(); ++it) {
        string clausestr(it->first);
        if ((it->second) || (received_clauses.find(clausestr)) != received_clauses.end()) {
          //DEBUG: cout << "Skipping a clause as it was already sent: " << it->first << endl;
          continue;} //Meaning: If it has already been sent/received, don't send it again
        string clstring(it->first);
        my_clauses.push_back(clstring);
        sent_clauses[it->first] = true; //Meaning: Set the clause as sent
      }
      lemma_push(my_clauses);
    }
  }

  void TrampDoSmtsPush() {
    tramp_SatSolver_ptr->DoSmtsPush();
  }

  void SatSolver::PrintLearnedClauses() {
    cout << "LIST OF LEARNED CLAUSES:" << endl;
    auto myMap = sent_clauses;
    int i = 0;
    //for(auto it = myMap.cbegin(); it != myMap.cend(); ++it)
    for(auto it = myMap.cbegin(); (i<10) && (it != myMap.cend()); ++i)
    {
      std::cout << it->first << " " << it->second << true << false << "\n";
      ++it;
    }
  }

// void SatSolver::SetCallbacks(void (*lemma_push)(vector<string> &clauses), void (*lemma_pull)(vector<string> &clauses))
void SatSolver::SetSmtsCallbacks(function<void(vector<string> &)> lemma_push, function<void(vector<string> &)> lemma_pull)
{
    this->lemma_push = lemma_push;
    this->lemma_pull = lemma_pull;

    tramp_SatSolver_ptr = this;
    picosat_set_smts_params(getPicosat(), smtsPicosatParams);
    picosat_set_smts_callbacks(getPicosat(), TrampDoSmtsPush, TrampDoSmtsPull, TrampSatVarToId, TrampIdToSatVar, TrampSatVarToStr, TrampStrToSatVar, TrampSmtsAddLearnedClause);
}


void SatSolver::AddFormula(const Formula& f) {
  DREAL_LOG_DEBUG("SatSolver::AddFormula({})", f);
  vector<Formula> clauses{cnfizer_.Convert(f)};
  // Collect Tseitin variables.
  for (const auto& p : cnfizer_.map()) {
    tseitin_variables_.insert(p.first);
  }
  for (Formula& clause : clauses) {
    clause = predicate_abstractor_.Convert(clause);
  }
  AddClauses(clauses);
}

void SatSolver::AddFormulas(const vector<Formula>& formulas) {
  for (const Formula& f : formulas) {
    AddFormula(f);
  }
}

//void SatSolver::SmtsAddTheoryClause(const set<Formula>& formulas) {
//  std::stringstream clstream;
//  for (const Formula& f : formulas) {
//    DREAL_ASSERT(is_variable(f) ||
//                 (is_negation(f) && is_variable(get_operand(f))));
//    if (is_variable(f)) {
//      // f = b
//      const Variable& var{get_variable(f)};
//      DREAL_ASSERT(var.get_type() == Variable::Type::BOOLEAN);
//      // Add l = b
//      clstream << /*to_sat_var_[var]*/ /* var.get_name() */ 5 << " ";
//    } else {
//      // f = ¬b
//      DREAL_ASSERT(is_negation(f) && is_variable(get_operand(f)));
//      const Variable& var{get_variable(get_operand(f))};
//      DREAL_ASSERT(var.get_type() == Variable::Type::BOOLEAN);
//      // Add l = ¬b
//      clstream << "-" << /*to_sat_var_[var]*/ /* var.get_name() */ 5 << " ";
//    }
//  }
//  clstream << "0";
//  char* clchar = strdup(clstream.str().c_str());
//  if (smtsDrealParams->verbosity >= 2) cout << "LEARNED THEORY CLAUSE " << endl;
//  if (smtsDrealParams->verbosity >= 4) cout << clchar << endl;
//  SmtsAddLearnedClause(clchar);
//  }

void SatSolver::AddLearnedClause(const set<Formula>& formulas) {
  for (const Formula& f : formulas) {
    AddLiteral(!predicate_abstractor_.Convert(f));
  }
  picosat_add(sat_, 0);
}

void SatSolver::AddClauses(const vector<Formula>& formulas) {
  for (const Formula& f : formulas) {
    AddClause(f);
  }
}

void SatSolver::AddClause(const Formula& f) {
  DREAL_LOG_DEBUG("SatSolver::AddClause({})", f);
  // Set up Variable ⇔ Literal (in SAT) map.
  for (const Variable& var : f.GetFreeVariables()) {
    MakeSatVar(var);
  }
  // Add clauses to SAT solver.
  DoAddClause(f);
}

namespace {
class SatSolverStat : public Stat {
 public:
  explicit SatSolverStat(const bool enabled) : Stat{enabled} {};
  SatSolverStat(const SatSolverStat&) = default;
  SatSolverStat(SatSolverStat&&) = default;
  SatSolverStat& operator=(const SatSolverStat&) = default;
  SatSolverStat& operator=(SatSolverStat&&) = default;
  ~SatSolverStat() override {
    if (enabled()) {
      using fmt::print;
      print(cout, "{:<45} @ {:<20} = {:>15}\n", "Total # of CheckSat",
            "SAT level", num_check_sat_);
    }
  }

  int num_check_sat_{0};
};
}  // namespace

void SatSolver::SmtsSetParameters(smts_dreal_params params) {
    smtsDrealParams = params;
    assert(params->verbosity >= 0);
    assert(params->lcwidth > 0);
    assert(params->conflicts > 0);
    smtsPicosatParams = static_cast<smts_picosat_params>(malloc(sizeof(SMTSpicosatparams)));
    smtsPicosatParams->verbosity = params->verbosity;
    smtsPicosatParams->lcwidth = params->lcwidth;
    smtsPicosatParams->conflicts = params->conflicts;
    this->random_generator = new std::default_random_engine(smtsDrealParams->seed);
    this->random_distribution = new std::uniform_int_distribution<int>(0, 2);
    //DEBUG: cout << "lcwidth should be: " << smtsPicosatParams->lcwidth << endl;
    picosat_set_smts_params(getPicosat(), smtsPicosatParams);
  }

std::experimental::optional<SatSolver::Model> SatSolver::CheckSat() {
  /*DEBUG
  cout << "VALUES OF MAP:" << endl;
  auto myMap = sat_var_to_id_;
  for(auto it = myMap.cbegin(); it != myMap.cend(); ++it)
  {
    std::cout << it->first << " " << it->second << "\n";
  }
   */
  static SatSolverStat stat{DREAL_LOG_INFO_ENABLED};
  DREAL_LOG_DEBUG("SatSolver::CheckSat(#vars = {}, #clauses = {})",
                  picosat_variables(sat_),
                  picosat_added_original_clauses(sat_));
  stat.num_check_sat_++;
  const int ret{picosat_sat(sat_, -1)};
  SmtsNumStats["picosat_original"] = picosat_stat_original(sat_);
  SmtsNumStats["picosat_tlearned"] = picosat_stat_added_original(sat_);
  SmtsNumStats["picosat_learned"] = picosat_stat_learned(sat_);
  SmtsNumStats["picosat_conflicts"] = picosat_stat_conflicts(sat_);
  SmtsNumStats["picosat_decisions"] = picosat_stat_decisions(sat_);
  Model model;
  auto& boolean_model = model.first;
  auto& theory_model = model.second;
  if (ret == PICOSAT_SATISFIABLE) {
    // SAT Case.
    const auto& var_to_formula_map = predicate_abstractor_.var_to_formula_map();
    for (int i = 1; i <= picosat_variables(sat_); ++i) {
      const int model_i{picosat_deref_partial(sat_, i)};
      if (model_i == 0) {
        continue;
      }
      const Variable& var{to_sym_var_[i]};
      const auto it = var_to_formula_map.find(var);
      if (it != var_to_formula_map.end()) {
        DREAL_LOG_TRACE("SatSolver::CheckSat: Add theory literal {}{} to Model",
                        model_i ? "" : "¬", var);
        theory_model.emplace_back(var, model_i == 1);
      } else if (tseitin_variables_.find(var) == tseitin_variables_.end()) {
        DREAL_LOG_TRACE(
            "SatSolver::CheckSat: Add Boolean literal {}{} to Model ",
            model_i ? "" : "¬", var);
        boolean_model.emplace_back(var, model_i == 1);
      } else {
        DREAL_LOG_TRACE(
            "SatSolver::CheckSat: Skip {}{} which is a temporary variable.",
            model_i ? "" : "¬", var);
      }
    }
    DREAL_LOG_DEBUG("SatSolver::CheckSat() Found a model.");
    return model;
  } else if (ret == PICOSAT_UNSATISFIABLE) {
    DREAL_LOG_DEBUG("SatSolver::CheckSat() No solution.");
    // UNSAT Case.
    return {};
  } else {
    DREAL_ASSERT(ret == PICOSAT_UNKNOWN);
    DREAL_LOG_CRITICAL("PICOSAT returns PICOSAT_UNKNOWN.");
    throw DREAL_RUNTIME_ERROR("PICOSAT returns PICOSAT_UNKNOWN.");
  }
}

void SatSolver::Pop() {
  DREAL_LOG_DEBUG("SatSolver::Pop()");
  picosat_pop(sat_);
}

void SatSolver::Push() {
  DREAL_LOG_DEBUG("SatSolver::Push()");
  picosat_push(sat_);
}

void SatSolver::AddLiteral(const Formula& f) {
  DREAL_ASSERT(is_variable(f) ||
               (is_negation(f) && is_variable(get_operand(f))));
  if (is_variable(f)) {
    // f = b
    const Variable& var{get_variable(f)};
    DREAL_ASSERT(var.get_type() == Variable::Type::BOOLEAN);
    // Add l = b
    picosat_add(sat_, to_sat_var_[var]);
    //if (var.get_name().find("obj") != std::string::npos)
    //  cout << "{SatSolver} ADDING variable [" << to_sat_var_[var] << "]" << var.get_name() << endl;
  } else {
    // f = ¬b
    DREAL_ASSERT(is_negation(f) && is_variable(get_operand(f)));
    const Variable& var{get_variable(get_operand(f))};
    DREAL_ASSERT(var.get_type() == Variable::Type::BOOLEAN);
    // Add l = ¬b
    picosat_add(sat_, -to_sat_var_[var]);
    //if (var.get_name().find("obj") != std::string::npos)
    //  cout << "{SatSolver} ADDING variable [" << to_sat_var_[var] << "]" << var.get_name() << endl;
  }
}

void SatSolver::DoAddClause(const Formula& f) {
  if (is_disjunction(f)) {
    // f = l₁ ∨ ... ∨ lₙ
    for (const Formula& l : get_operands(f)) {
      AddLiteral(l);
    }
  } else {
    // f = b or f = ¬b.
    AddLiteral(f);
  }
  picosat_add(sat_, 0);
}

void SatSolver::MakeSatVar(const Variable& var) {
  auto it = to_sat_var_.find(var);
  if (it != to_sat_var_.end()) {
    // Found.
    return;
  }

  std::default_random_engine generator(smtsDrealParams->seed);
  std::uniform_int_distribution<int> distribution(0, 2);

  // It's not in the maps, let's make one and add it.
  const int sat_var{picosat_inc_max_var(sat_)};

  int randval = (*random_distribution)(*random_generator);

  if (randval == 1) {
      picosat_set_more_important_lit(sat_, (int)sat_var); picosat_set_more_important_lit(sat_, -(int)sat_var);
  }

  if (randval == 2) {
      picosat_set_less_important_lit(sat_, (int)sat_var); picosat_set_less_important_lit(sat_, -(int)sat_var);
  }

  randval = (*random_distribution)(*random_generator);


  if (randval == 1) {
      picosat_set_default_phase_lit(sat_, (int)sat_var, 1); picosat_set_default_phase_lit(sat_, -(int)sat_var, -1);
  }

  if (randval == 2) {
      picosat_set_default_phase_lit(sat_, (int)sat_var, -1); picosat_set_default_phase_lit(sat_, -(int)sat_var, 1);
  }

  SMTS_id_count++;

  if (smtsDrealParams->verbosity >= 5) printf("Assigning ID %lu (sat_var %d) to variable %s\n", /*SMTS_id_count*/ var.get_id(), sat_var, var.get_name().c_str());

  to_sat_var_.emplace_hint(it, var, sat_var);
  //id_to_sat_var_.emplace(SMTS_id_count, sat_var);
  id_to_sat_var_.emplace(var.get_id(), sat_var);
  //sat_var_to_id_.emplace(sat_var, SMTS_id_count);
  sat_var_to_id_.emplace(sat_var, var.get_id());
  str_to_sat_var_.emplace(var.get_name().c_str(), sat_var);
  to_sym_var_.emplace(sat_var, var);
  id_to_sym_var.emplace(var.get_id(), var);

  DREAL_LOG_DEBUG("SatSolver::MakeSatVar({} ↦ {})", var, sat_var);
}

PicoSAT* SatSolver::getPicosat() {
  return sat_;
}

}  // namespace dreal
