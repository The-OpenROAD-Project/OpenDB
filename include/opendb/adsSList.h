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

///
/// This class implements an instrusive singlely-linked list.
///
/// See test example below.
///

template <class T>
class adsSListEntry
{
 public:
  T* _next;
};

template <class T, adsSListEntry<T>* ENTRY(T*)>
class adsSList;

template <class T, adsSListEntry<T>* ENTRY(T*)>
class adsSListIterator
{
  T* _cur;

  T**  NEXT(T* t) { return &ENTRY(t)->_next; }
  void incr() { _cur = *NEXT(_cur); }

 public:
  adsSListIterator() { _cur = NULL; }
  adsSListIterator(T* cur) { _cur = cur; }
  adsSListIterator(const adsSListIterator& i) { _cur = i._cur; }
  adsSListIterator& operator=(const adsSListIterator& i)
  {
    _cur = i._cur;
    return *this;
  }

  bool operator==(const adsSListIterator& i) const { return _cur == i._cur; }
  bool operator!=(const adsSListIterator& i) const { return _cur != i._cur; }
  T*   operator*() { return _cur; }

  adsSListIterator<T, ENTRY>& operator++()
  {
    incr();
    return *this;
  }

  adsSListIterator<T, ENTRY> operator++(int)
  {
    adsSListIterator<T, ENTRY> i = *this;
    incr();
    return i;
  }

  friend class adsSList<T, ENTRY>;
};

template <class T, adsSListEntry<T>* ENTRY(T*)>
class adsSList
{
  /*
    public:
      typedef adsSListIterator<T,ENTRY> iterator;
  */
 private:
  T* _head;
  T* _tail;

  T**  NEXT(T* t) { return &ENTRY(t)->_next; }
  bool lessthan(const T& p1, const T& p2) { return *p1 < *p2; }

 public:
  typedef adsSListIterator<T, ENTRY> iterator;

  adsSList()
  {
    _head = NULL;
    _tail = NULL;
  }

  T* front() { return _head; }
  T* back() { return _tail; }

  void push_front(T* p)
  {
    *NEXT(p) = _head;
    _head    = p;

    if (_tail == NULL)
      _tail = p;
  }

  void push_back(T* p)
  {
    if (_head == NULL) {
      _head    = p;
      _tail    = p;
      *NEXT(p) = NULL;
    } else {
      *NEXT(_tail) = p;
      _tail        = p;
      *NEXT(p)     = NULL;
    }
  }

  void swap(adsSList& l)
  {
    T* head = l._head;
    l._head = _head;
    _head   = head;
    T* tail = l._tail;
    l._tail = _tail;
    _tail   = tail;
  }

  iterator remove(T* p) { return remove(iterator(p)); }
  bool     empty() const { return _head == NULL; }
  void     clear()
  {
    _head = NULL;
    _tail = NULL;
  }

  iterator begin() { return iterator(_head); }
  iterator end() { return iterator(NULL); }

  template <class CMP>
  void merge(adsSList<T, ENTRY>& l, CMP cmp)
  {
    iterator first1 = begin();
    iterator last1  = end();
    iterator first2 = l.begin();
    iterator last2  = l.end();
    iterator prev1;

    while (first1 != last1 && first2 != last2) {
      if (cmp(**first2, **first1)) {
        iterator next = first2;
        ++next;
        move(prev1, first1, first2);
        prev1  = first2;
        first2 = next;
      } else {
        prev1 = first1;
        ++first1;
      }
    }

    if (first2 != last2) {
      if (_head == NULL) {
        _head = first2._cur;
        _tail = l._tail;
      } else {
        *NEXT(*prev1) = first2._cur;
        _tail         = l._tail;
      }
    }

    l._head = NULL;
    l._tail = NULL;
  }

  template <class CMP>
  void sort(CMP cmp)
  {
    iterator prev;

    if ((_head != NULL) && (*NEXT(_head) != NULL)) {
      adsSList<T, ENTRY> carry;
      adsSList<T, ENTRY> counter[64];
      int                fill = 0;

      while (!empty()) {
        T* head = *NEXT(_head);
        carry.move(prev, carry.begin(), begin());
        _head = head;

        int i = 0;
        while (i < fill && !counter[i].empty()) {
          counter[i].merge(carry, cmp);
          carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill)
          ++fill;
      }

      for (int i = 1; i < fill; ++i)
        counter[i].merge(counter[i - 1], cmp);

      swap(counter[fill - 1]);
    }
  }

  void move(typename adsSList<T, ENTRY>::iterator prev1,
            typename adsSList<T, ENTRY>::iterator itr1,
            typename adsSList<T, ENTRY>::iterator itr2)
  {
    if (itr1._cur == _head) {
      *NEXT(itr2._cur) = _head;

      if (_head == NULL)
        _tail = itr2._cur;

      _head = itr2._cur;
    } else {
      *NEXT(itr2._cur)  = itr1._cur;
      *NEXT(prev1._cur) = itr2._cur;
    }
  }

  adsSListIterator<T, ENTRY> remove(iterator prev, iterator cur)
  {
    if (cur._cur == _head)
      _head = *NEXT(cur._cur);
    else {
      assert(*NEXT(prev._cur) == cur._cur);
      *NEXT(prev._cur) = *NEXT(cur._cur);
    }

    if (cur._cur == _tail)
      _tail = prev._cur;

    return iterator(*NEXT(cur._cur));
  }

  adsSListIterator<T, ENTRY> remove(iterator cur)
  {
    iterator prev;
    iterator itr;

    for (itr = begin(); itr != end(); prev = itr++)
      if (itr == cur)
        return remove(prev, itr);

    return end();
  }

  void reverse()
  {
    if (_head == _tail)
      return;

    T* l = NULL;
    T* c;
    _tail = _head;

    for (c = _head; c != NULL;) {
      T* n     = *NEXT(c);
      *NEXT(c) = l;
      l        = c;
      c        = n;
    }

    *NEXT(_tail) = NULL;
    _head        = l;
  }

  int size()
  {
    T*  c;
    int i = 0;
    for (c = _head; c != NULL; c = *NEXT(c))
      ++i;
    return i;
  }

  void merge(adsSList<T, ENTRY>& l)
  {
    iterator first1 = begin();
    iterator last1  = end();
    iterator first2 = l.begin();
    iterator last2  = l.end();
    iterator prev1;

    while (first1 != last1 && first2 != last2) {
      if (lessthan(**first2, **first1)) {
        iterator next = first2;
        ++next;
        move(prev1, first1, first2);
        prev1  = first2;
        first2 = next;
      } else {
        prev1 = first1;
        ++first1;
      }
    }

    if (first2 != last2) {
      if (_head == NULL) {
        _head = first2._cur;
        _tail = l._tail;
      } else {
        *NEXT(*prev1) = first2._cur;
        _tail         = l._tail;
      }
    }

    l._head = NULL;
    l._tail = NULL;
  }

  void sort()
  {
    iterator prev;

    if ((_head != NULL) && (*NEXT(_head) != NULL)) {
      adsSList<T, ENTRY> carry;
      adsSList<T, ENTRY> counter[64];
      int                fill = 0;

      while (!empty()) {
        T* head = *NEXT(_head);
        carry.move(prev, carry.begin(), begin());
        _head = head;

        int i = 0;
        while (i < fill && !counter[i].empty()) {
          counter[i].merge(carry);
          carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill)
          ++fill;
      }

      for (int i = 1; i < fill; ++i)
        counter[i].merge(counter[i - 1]);

      swap(counter[fill - 1]);
    }
  }
};

}  // namespace odb

#if 0
#include <assert.h>
#include <stdio.h>
#include <vector>

using namespace odb;

struct elem
{
    int a;
    adsSListEntry<elem> entry;

    elem(int n )
    {
        a = n;
    }

    bool operator<( const elem & e ) { return a < e.a; }
    
    static adsSListEntry<elem> * getEntry( elem * e ) 
    {
        return &e->entry;
    }
};

struct cmp
{
    bool operator()( const elem & e1, const elem & e2 )
    {
        return e1.a < e2.a;
    }
};

#define M 100000

main()
{
    int i;

    std::vector<elem *> elems;


    for( i = 0; i < M; ++i )
    {
        elem * e = new elem(i);
        elems.push_back(e);
    }
    
    for( i = 0; i < M; ++i )
    {
        int r1 = random() % M;
        int r2 = random() % M;
        elem * e = elems[r1];
        elems[r1] = elems[r2];
        elems[r2] = e;
    }
    
    adsSList<elem, elem::getEntry> l1;

    for( i = 0; i < M; ++i )
    {
        elem * e = elems[i];
        l1.push_back(e);
    }
    
    l1.sort(cmp());
    
    adsSList<elem, elem::getEntry>::iterator itr;

    for( i = 0, itr = l1.begin(); itr != l1.end(); ++itr, i++ )
    {
        elem * e = *itr;
        printf("l1: %d\n", e->a );
        assert( e->a == i );
    }

    l1.reverse();

    for( i = 1, itr = l1.begin(); itr != l1.end(); ++itr, i++ )
    {
        elem * e = *itr;
        printf("l1: %d\n", e->a );
        assert( e->a == (M - i) );
    }
    
}
#endif


