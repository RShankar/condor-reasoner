#include "factory.h"

Factory::Factory() {
  top_tracker = new TopConcept();
  concept_register[top_tracker->positive()] = top_tracker;
  bottom_tracker = new BottomConcept();
  concept_register[bottom_tracker->positive()] = bottom_tracker;
}

Factory::~Factory() {
  delete top_tracker;
  delete bottom_tracker;
}

const Role* Factory::role(RoleID id) {
  return role_register[id];
}

const Concept* Factory::concept(ConceptID id) {
  return concept_register[id];
}

const Role* Factory::role(const string& name) {
  const Role* r = role_tracker(name);
  if (role_tracker.was_new()) {
    role_register[r->ID()] = r;
    role_register[r->inverse()->ID()] = r->inverse();
  }
  return r;
}

const AtomicConcept* Factory::atomic(const string& name) {
  const AtomicConcept* d = atomic_tracker(name);
  if (atomic_tracker.was_new())
    concept_register[d->positive()] = d;
  return d;
}

const TopConcept* Factory::top() {
  return top_tracker;
}

const BottomConcept* Factory::bottom() {
  return bottom_tracker;
}

const NegationConcept* Factory::negation(const Concept* c) {
  const NegationConcept* d = negation_tracker(c);
  if (negation_tracker.was_new())
    concept_register[d->positive()] = d;
  return d;
}

//sorts v!
const ConjunctionConcept* Factory::conjunction(vector<const Concept*>& v) {
  sort(v.begin(), v.end());
  const ConjunctionConcept* d = conjunction_tracker(v);
  if (conjunction_tracker.was_new())
    concept_register[d->positive()] = d;
  return d;
}

const Concept* Factory::improper_conjunction(vector<const Concept*>& v) {
  if (v.empty())
      return top_tracker;
  if (v.size() == 1)
      return v[0];

  sort(v.begin(), v.end());
  const ConjunctionConcept* d = conjunction_tracker(v);
  if (conjunction_tracker.was_new())
    concept_register[d->positive()] = d;
  return d;
}

//sorts v!
const DisjunctionConcept* Factory::disjunction(vector<const Concept*>& v) {
  sort(v.begin(), v.end());
  const DisjunctionConcept *d = disjunction_tracker(v);
  if (disjunction_tracker.was_new())
    concept_register[d->positive()] = d;
  return d;
}

const Concept* Factory::improper_disjunction(vector<const Concept*>& v) {
  if (v.empty())
      return bottom_tracker;
  if (v.size() == 1)
      return v[0];

  sort(v.begin(), v.end());
  const DisjunctionConcept *d = disjunction_tracker(v);
  if (disjunction_tracker.was_new())
    concept_register[d->positive()] = d;
  return d;
}

const ExistentialConcept* Factory::existential(const Role* r, const Concept* c) {
  const ExistentialConcept* d = existential_tracker(make_pair(r, c));
  if (existential_tracker.was_new())  {
    concept_register[d->negative()] = d;
    concept_register[d->positive()] = d;
  }
  return d;
}

const UniversalConcept* Factory::universal(const Role* r, const Concept* c) {
  const UniversalConcept* d = universal_tracker(make_pair(r, c));
  if (universal_tracker.was_new()) {
    concept_register[d->negative()] = d;
    concept_register[d->positive()] = d;
  }
  return d;
}

vector<const AtomicConcept*> Factory::all_atomic_ordered() {
  vector<const AtomicConcept*> l = atomic_tracker.get_range();
  sort(l.begin(), l.end(), AtomicConcept::AlphaLess());
  return l;
}
