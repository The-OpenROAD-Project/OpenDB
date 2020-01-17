// This is a very basic polygon API for tcl.  In C++ you should
// directly use the Boost Polygon classes.

%{
#include <boost/polygon/polygon.hpp>
#include <array>

using namespace boost::polygon::operators;

using Polygon90 = boost::polygon::polygon_90_with_holes_data<int>;
using Polygon90Set = std::vector<Polygon90>;

Polygon90Set* odb_newSetFromRect(int xLo, int yLo, int xHi, int yHi)
{
  using Pt = Polygon90::point_type;
  std::array<Pt, 4> pts
    = {Pt(xLo, yLo), Pt(xHi, yLo), Pt(xHi, yHi), Pt(xLo, yHi)};

  Polygon90 poly;
  poly.set(pts.begin(), pts.end());

  return new Polygon90Set({poly});
}
  
void odb_bloatSet(Polygon90Set* set, int bloating)
{
  *set += bloating;
}

void odb_shrinkSet(Polygon90Set* set, int shrinking)
{
  *set -= shrinking;
}

void odb_andSet(Polygon90Set* set1, const Polygon90Set* set2)
{
  *set1 &= *set2;
}

void odb_orSet(Polygon90Set* set1, const Polygon90Set* set2)
{
  *set1 |= *set2;
}

void odb_subtractSet(Polygon90Set* set1, const Polygon90Set* set2)
{
  *set1 -= *set2;
}

std::vector<Polygon90> odb_getPolygons(const Polygon90Set* set)
{
  return *set;
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

// Simple constructor
%newobject odb_newSetFromRect;
Polygon90Set* odb_newSetFromRect(int xLo, int yLo, int xHi, int yHi);

// Query methods - return vectors for easy swig'ing
std::vector<odb::adsPoint> odb_getPoints(const Polygon90* polygon);
std::vector<Polygon90> odb_getPolygons(const Polygon90Set* set);

// All modify the first arg
void odb_bloatSet(Polygon90Set* set, int bloating);
void odb_shrinkSet(Polygon90Set* set, int shrinking);
void odb_andSet(Polygon90Set* set1, const Polygon90Set* set2);
void odb_orSet(Polygon90Set* set1, const Polygon90Set* set2);
void odb_subtractSet(Polygon90Set* set1, const Polygon90Set* set2);
