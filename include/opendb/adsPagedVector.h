///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "ads.h"

namespace odb {

template <class T, const uint P, const uint S>
class adsPagedVector;

template <class T, const uint P, const uint S>
class adsPagedVectorIterator
{
  adsPagedVector<T, P, S>* _table;
  unsigned int             _id;

 public:
  typedef adsPagedVector<T, P, S>         table_type;
  typedef adsPagedVectorIterator<T, P, S> self_type;
  typedef std::random_access_iterator_tag iterator_category;
  typedef T                               value_type;
  typedef T&                              reference;
  typedef T*                              pointer;
  typedef int                             difference_type;

  adsPagedVectorIterator();
  adsPagedVectorIterator(table_type* table);
  adsPagedVectorIterator(table_type* table, unsigned int id);
  adsPagedVectorIterator(const self_type& it);
  bool       operator==(const self_type& it) const;
  bool       operator!=(const self_type& it) const;
  T&         operator*() const;
  T*         operator->() const;
  self_type& operator++();
  self_type  operator++(int);
  self_type& operator--();
  self_type  operator--(int);

  self_type& operator+=(difference_type id) const
  {
    _id += id;
    return *this;
  }
  self_type& operator-=(difference_type id) const
  {
    _id -= id;
    return *this;
  }
  self_type operator-(difference_type id) const
  {
    return self_type(_table, _id - id);
  }
  self_type operator+(difference_type id) const
  {
    return self_type(_table, _id + id);
  }
  T& operator[](difference_type id) const { return (*_table)[_id]; }

  friend difference_type operator-<>(const self_type&, const self_type&);
  friend difference_type operator+<>(const self_type&, const self_type&);
  friend bool            operator<<>(const self_type&, const self_type&);
};

template <class T, const uint P, const uint S>
typename adsPagedVectorIterator<T, P, S>::difference_type operator-(
    const adsPagedVectorIterator<T, P, S>& lhs,
    const adsPagedVectorIterator<T, P, S>& rhs)
{
  return lhs._id - rhs._id;
}

template <class T, const uint P, const uint S>
typename adsPagedVectorIterator<T, P, S>::difference_type operator+(
    const adsPagedVectorIterator<T, P, S>& lhs,
    const adsPagedVectorIterator<T, P, S>& rhs)
{
  return lhs._id - rhs._id;
}

template <class T, const uint P, const uint S>
bool operator<(const adsPagedVectorIterator<T, P, S>& lhs,
               const adsPagedVectorIterator<T, P, S>& rhs)
{
  return lhs._id < rhs._id;
}

//
// Vector - Creates a vector of type T. However, the vector is created
// as a series of pages of type T. The size of the page is specified
// by the template params.
//
// PAGE_SIZE = number of objects per page (MUST BE POWER OF TWO)
// PAGE_SHIFT = log2(PAGE_SIZE)
//
template <class T, const uint PAGE_SIZE = 128, const uint PAGE_SHIFT = 7>
class adsPagedVector
{
 public:
  typedef adsPagedVectorIterator<T, PAGE_SIZE, PAGE_SHIFT> iterator;
  friend class adsPagedVectorIterator<T, PAGE_SIZE, PAGE_SHIFT>;
  typedef T    value_type;
  typedef T&   reference;
  typedef T*   pointer;
  typedef int  difference_type;
  typedef uint size_type;

 private:
  T**          _pages;
  unsigned int _page_cnt;
  unsigned int _page_tbl_size;
  unsigned int _next_idx;

  void resizePageTbl();
  void newPage();

 public:
  adsPagedVector();
  ~adsPagedVector();

  // Append an element to the vector.
  void push_back(const T& item);

  // Get the number of elements in the vector.
  unsigned int size() const { return _next_idx; }

  // Clear all elements and memory used by the vector.
  void clear();

  // Clear all elements in the vector
  void reset() { _next_idx = 0; }

  // Returns true if the vector is empty.
  bool empty() { return _next_idx == 0; }

  // Remove the last element from the vector.
  void pop_back()
  {
    assert(!_next_idx);
    --_next_idx;
  }

  iterator begin() { return iterator(this); }
  iterator end() { return iterator(this, _next_idx); }

  T& operator[](unsigned int id)
  {
    assert(id < _next_idx);
    unsigned int page   = (id & ~(PAGE_SIZE - 1)) >> PAGE_SHIFT;
    unsigned int offset = id & (PAGE_SIZE - 1);
    return _pages[page][offset];
  }

  const T& operator[](unsigned int id) const
  {
    assert(id < _next_idx);
    unsigned int page   = (id & ~(PAGE_SIZE - 1)) >> PAGE_SHIFT;
    unsigned int offset = id & (PAGE_SIZE - 1);
    return _pages[page][offset];
  }

  T& back()
  {
    assert(!_next_idx);
    unsigned int page   = _page_cnt - 1;
    unsigned int offset = (_next_idx - 1) & (PAGE_SIZE - 1);
    return _pages[page][offset];
  }

  const T& back() const
  {
    assert(!_next_idx);
    unsigned int page   = _page_cnt - 1;
    unsigned int offset = (_next_idx - 1) & (PAGE_SIZE - 1);
    return _pages[page][offset];
  }

  T& front()
  {
    assert(!_next_idx);
    return _pages[0][0];
  }

  const T& front() const
  {
    assert(!_next_idx);
    return _pages[0][0];
  }

  T* last()
  {
    if (!_next_idx)
      return NULL;
    unsigned int page   = _page_cnt - 1;
    unsigned int offset = (_next_idx - 1) & (PAGE_SIZE - 1);
    return _pages[page] + offset;
  }

  T* first()
  {
    if (!_next_idx)
      return NULL;
    return _pages[0];
  }
};

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S>::adsPagedVectorIterator()
{
  _table = NULL;
  _id    = 0;
}

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S>::adsPagedVectorIterator(
    table_type* table)
{
  _table = table;
  _id    = 0;
}

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S>::adsPagedVectorIterator(
    table_type*  table,
    unsigned int id)
{
  _table = table;
  _id    = id;
}

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S>::adsPagedVectorIterator(
    const self_type& it)
{
  _table = it._table;
  _id    = it._id;
}

template <class T, const uint P, const uint S>
inline bool adsPagedVectorIterator<T, P, S>::operator==(
    const self_type& it) const
{
  return (_table == it._table) && (_id == it._id);
}

template <class T, const uint P, const uint S>
inline bool adsPagedVectorIterator<T, P, S>::operator!=(
    const self_type& it) const
{
  return (_table != it._table) || (_id != it._id);
}

template <class T, const uint P, const uint S>
inline T& adsPagedVectorIterator<T, P, S>::operator*() const
{
  return (*_table)[_id];
}

template <class T, const uint P, const uint S>
inline T* adsPagedVectorIterator<T, P, S>::operator->() const
{
  return &(*_table)[_id];
}

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S>& adsPagedVectorIterator<T, P, S>::
                                        operator++()
{
  ++_id;
  return *this;
}

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S> adsPagedVectorIterator<T, P, S>::
                                       operator++(int)
{
  adsPagedVectorIterator<T, P, S> it(*this);
  ++_id;
  return it;
}

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S>& adsPagedVectorIterator<T, P, S>::
                                        operator--()
{
  --_id;
  return *this;
}

template <class T, const uint P, const uint S>
inline adsPagedVectorIterator<T, P, S> adsPagedVectorIterator<T, P, S>::
                                       operator--(int)
{
  adsPagedVectorIterator<T, P, S> it(*this);
  --_id;
  return it;
}

template <class T, const uint P, const uint S>
adsPagedVector<T, P, S>::adsPagedVector<T, P, S>()
{
  _pages         = NULL;
  _page_cnt      = 0;
  _page_tbl_size = 0;
  _next_idx      = 0;
}

template <class T, const uint P, const uint S>
void adsPagedVector<T, P, S>::clear()
{
  if (_pages) {
    unsigned int i;

    for (i = 0; i < _page_cnt; ++i)
      delete[] _pages[i];

    delete[] _pages;
  }

  _pages         = NULL;
  _page_cnt      = 0;
  _page_tbl_size = 0;
  _next_idx      = 0;
}

template <class T, const uint P, const uint S>
adsPagedVector<T, P, S>::~adsPagedVector<T, P, S>()
{
  clear();
}

template <class T, const uint P, const uint S>
void adsPagedVector<T, P, S>::resizePageTbl()
{
  T**          old_tbl      = _pages;
  unsigned int old_tbl_size = _page_tbl_size;

  if (_page_tbl_size == 1)
    ++_page_tbl_size;
  else
    _page_tbl_size += (unsigned int) ((float) _page_tbl_size * (0.5));

  _pages = new T*[_page_tbl_size];
  assert(_pages);

  unsigned int i;

  for (i = 0; i < old_tbl_size; ++i)
    _pages[i] = old_tbl[i];

  for (; i < _page_tbl_size; ++i)
    _pages[i] = NULL;

  delete[] old_tbl;
}

template <class T, const uint P, const uint S>
void adsPagedVector<T, P, S>::newPage()
{
  T* page = new T[P];
  assert(page);

  if (_page_tbl_size == 0) {
    _pages = new T*[1];
    assert(_pages);
    _page_tbl_size = 1;
  } else if (_page_tbl_size == _page_cnt) {
    resizePageTbl();
  }

  _pages[_page_cnt] = page;
  ++_page_cnt;
}

template <class T, const uint P, const uint S>
void adsPagedVector<T, P, S>::push_back(const T& item)
{
  unsigned int page = (_next_idx & ~(P - 1)) >> S;

  if (page == _page_cnt)
    newPage();

  unsigned int offset = _next_idx & (P - 1);
  ++_next_idx;

  T* objects      = _pages[page];
  objects[offset] = item;
}

}  // namespace odb


