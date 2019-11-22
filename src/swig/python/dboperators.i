%rename(_print) print;

%define WRAP_OBJECT_OPERATOR(T)
%rename(incr) T::operator++();
%rename(incr_int) T::operator++(int);
%enddef

WRAP_OBJECT_OPERATOR(odb::dbRtNodeEdgeIterator)
WRAP_OBJECT_OPERATOR(odb::dbSetIterator<dbBlock>)
WRAP_OBJECT_OPERATOR(odb::dbSetIterator<dbCCSeg>)

%define WRAP_OBJECT_STREAM(T)
%rename(IStream) operator>>(dbIStream &, T &);
%rename(OStream) operator<<(dbOStream &, const T &);
%rename(dbDiff)  operator<<(dbDiff &, const dbTransform &);
%rename(equal)  odb::T::operator=(const T &);

%enddef
WRAP_OBJECT_STREAM(adsPoint)
WRAP_OBJECT_STREAM(adsRect)
WRAP_OBJECT_STREAM(dbTransform)
WRAP_OBJECT_STREAM(_dbViaParams)
WRAP_OBJECT_STREAM(dbRtNodeEdgeIterator)

%rename(equal) odb::dbString::operator=(const char *);
%rename(equal) odb::dbString::operator=(const dbString &);

