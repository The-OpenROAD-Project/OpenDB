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

#include <fstream>
#include <iostream>
#ifdef __WIN32
#include <crtdbg.h>
#endif

using namespace std;

#include "util.h"

void test_list(void)
{
  AthList<int> list;
  list.push(1);
  list.push(2);

  AthList<int>::iterator iter = list.start();
  while (!iter.end()) {
    cout << "List Element " << iter.getVal() << endl;
    iter.next();
  }
}
void test_hash(void)
{
  AthHash<int> smallHash;

  char ptr[100];
  int  i, tmp_val;
  for (i = 0; i < 55; i++) {
    sprintf(ptr, "temp string %d", i);
    smallHash.add(ptr, i);
    tmp_val = i - 1;
    assert(smallHash.get(ptr, tmp_val));
    assert(tmp_val == i);
  }

  // And now verify that if you ask for something that does not exist
  // we will not find it.
  assert(smallHash.get("not there", tmp_val) == false);

  // Finally test the iterator
  AthHash<int>::iterator iter = smallHash.start();
  while (!iter.end()) {
    cout << "Hash Element " << iter.getKey() << " with value " << iter.getVal()
         << endl;
    iter.next();
  }
}

void checkMem()
{
  _ASSERTE(_CrtCheckMemory());
  if (_CrtDumpMemoryLeaks()) {
    cerr << "MEMORY LEAK found " << endl;
  }
}

void test_Array(void)
{
  AthArray<int> arr(1);

  int max_num_to_test = 6;
  int i;
  for (i = 0; i < max_num_to_test; i++) {
    arr.add(i);
    int j = arr[i];
    assert(arr[i] == i);
  }
  for (i = 0; i < max_num_to_test; i++) {
    assert(arr[i] == i);
  }
}
void test_Array_dkf(void)
{
  AthArray<char*> arr(10);

  char* name1 = "first";
  char* name2 = "second";
  arr.add(name1);
  arr.add(name2);
  fprintf(stdout, "%s is stored as %s\n", name1, arr.get(0));
  fprintf(stdout, "%s is stored as %s\n", name2, arr.get(1));
}
class structABC
{
  int A;
  int B;
};

void test_memPool(void)
{
  AthPool<structABC>* pool = new AthPool<structABC>();
  // structABC *abc= pool->alloc();

  AthPool<int> heap;
  heap.setDbg(1);
  AthArray<int*> heap_of_ptr;
  int*           ptr;
  int            i;
  for (i = 0; i < 6; i++) {
    ptr  = heap.alloc();
    *ptr = i;
    heap_of_ptr.add(ptr);
  }

  for (i = 0; i < 6; i++) {
    assert((*heap_of_ptr[i]) == i);
  }
}

void test_main_mem_util()
{
  test_Array_dkf();
  _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF
                 | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  _CrtMemState mem_state;
  _CrtMemCheckpoint(&mem_state);

  test_list();
  checkMem();
  test_Array();
  checkMem();
  test_memPool();
  checkMem();
  test_hash();
  checkMem();
}
