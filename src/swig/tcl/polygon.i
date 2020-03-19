// This is a very basic polygon API for tcl.  In C++ you should
// directly use the Boost Polygon classes.

%{
#include <boost/polygon/polygon.hpp>
#include <array>

using namespace boost::polygon::operators;

using Polygon90 = boost::polygon::polygon_90_with_holes_data<int>;
using Polygon90Set = boost::polygon::polygon_90_set_data<int>;

Polygon90Set* odb_newSetFromRect(int xLo, int yLo, int xHi, int yHi)
{
  using Pt = Polygon90::point_type;
  std::array<Pt, 4> pts
    = {Pt(xLo, yLo), Pt(xHi, yLo), Pt(xHi, yHi), Pt(xLo, yHi)};

  Polygon90 poly;
  poly.set(pts.begin(), pts.end());

  std::array<Polygon90, 1> arr{poly};
  return new Polygon90Set(boost::polygon::HORIZONTAL, arr.begin(), arr.end());
}
  
Polygon90Set* odb_bloatSet(const Polygon90Set* set, int bloating)
{
  return new Polygon90Set(*set + bloating);
}

Polygon90Set* odb_shrinkSet(const Polygon90Set* set, int shrinking)
{
  return new Polygon90Set(*set - shrinking);
}

Polygon90Set* odb_andSet(const Polygon90Set* set1, const Polygon90Set* set2)
{
  return new Polygon90Set(*set1 & *set2);
}

Polygon90Set* odb_orSet(const Polygon90Set* set1, const Polygon90Set* set2)
{
  return new Polygon90Set(*set1 | *set2);
}

Polygon90Set* odb_orSets(const std::vector<Polygon90Set>& sets)
{
  Polygon90Set* result = new Polygon90Set;
  for (const Polygon90Set& poly_set : sets) {
    *result |= poly_set;
  }
  return result;
}

Polygon90Set* odb_subtractSet(const Polygon90Set* set1, const Polygon90Set* set2)
{
  return new Polygon90Set(*set1 - *set2);
}

std::vector<Polygon90> odb_getPolygons(const Polygon90Set* set)
{
  std::vector<Polygon90> s;
  set->get(s);
  return s;
}

std::vector<adsPoint> odb_getPoints(const Polygon90* polygon)
{
  std::vector<adsPoint> pts;
  for (auto& pt : *polygon) {
    pts.emplace_back(adsPoint(pt.x(), pt.y()));
  }
  return pts;
}
%}

%template(Points) std::vector<odb::adsPoint>;
%template(Polygon90Set) std::vector<Polygon90>;
%template(Polygon90Sets) std::vector<Polygon90Set>;

// Simple constructor
%newobject odb_newSetFromRect;
Polygon90Set* odb_newSetFromRect(int xLo, int yLo, int xHi, int yHi);

// Query methods - return vectors for easy swig'ing
std::vector<odb::adsPoint> odb_getPoints(const Polygon90* polygon);
std::vector<Polygon90> odb_getPolygons(const Polygon90Set* set);


%newobject odb_bloatSet;
Polygon90Set* odb_bloatSet(Polygon90Set* set, int bloating);

%newobject odb_shrinkSet;
Polygon90Set* odb_shrinkSet(Polygon90Set* set, int shrinking);

%newobject odb_andSet;
Polygon90Set* odb_andSet(Polygon90Set* set1, const Polygon90Set* set2);

%newobject odb_orSet;
Polygon90Set* odb_orSet(Polygon90Set* set1, const Polygon90Set* set2);

// It makese no sense to me that we have a vector by value and not
// pointer but swig seems to automatically add the pointer in the
// generated wrapper.
%newobject odb_orSets;
Polygon90Set* odb_orSets(const std::vector<Polygon90Set>& sets);

%newobject odb_subtractSet;
Polygon90Set* odb_subtractSet(Polygon90Set* set1, const Polygon90Set* set2);
