/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap(size_t s = 0) { if (s != 0) _data.reserve(s); }
   ~MinHeap() {}

   void clear() { _data.clear(); }

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { return _data[0]; }
   void insert(const Data& d)
   { 
      _data.push_back(d);
      size_t t = _data.size() - 1;
      while( t > 0)
      {
         size_t p = (t-1) / 2; // parent = (child-1) /2
         if(d < _data[p])
         {
            _data[t] = _data[p];
            t = p;
         }
         else break;
      }
      _data[t] = d;
   }
   void delMin() { delData(0); }
   void delData(size_t i) 
   { 
      swap(_data[i], _data[size() - 1]);
      _data.erase(_data.end() - 1);
      size_t s = _data.size();
      while (i*2+2 < s) {
         if (_data[i*2+1] < _data[i] && _data[i*2+1] <  _data[i*2+2]) {
            swap(_data[i], _data[i*2+1]);
            i = i*2+1;
         } 
         else if (_data[i*2+2] < _data[i]) {
            swap(_data[i], _data[i*2+2]);
            i = i*2+2;
         } 
         else break;
      }
      if (i*2+2 == s && _data[i*2+1] < _data[i])
         swap(_data[i], _data[i*2+1]);
   }

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
