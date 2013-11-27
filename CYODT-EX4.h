#include <cstring> //for memcpy

#define ALLOC_SIZE 1280
#define NEW(type) CYODT::MemBank<type>::allocate()
#define DELETE(ptr) CYODT::_MemBankDeleteProxy(ptr)

namespace CYODT
   {
   template <class T>
   class MemBank
      {
      private:
         static std::stack<void*> avail;
         static std::stack<char*> buffers;
         struct MemBankClear
            {
            ~MemBankClear() {MemBank<T>::clearBank();}
            };
         static MemBankClear clear;
      protected:
         MemBank() {}
      public:
         static T* allocate()
            {
            clear; //this does nothing, just gets around VS bug
            if(!avail.size())
               {
               auto t = new char[ALLOC_SIZE * sizeof(T)];
               buffers.push(t);
               T* c = (T*)t;
               unsigned int a = ALLOC_SIZE;
               while(a--)
                  avail.push(c + a);
               }
            auto r = avail.top();
            avail.pop();
            *(T*)r = T();//relies on assignment operator and default ctor
            return (T*)r;
            }

         static void deallocate(T* a)
            {
            a->~T();
            avail.push(a);
            }

         static void clearBank()
            {
            while(avail.size())
               avail.pop();
            while(buffers.size())
               {
               delete [] buffers.top();
               buffers.pop();
               }
            }
      };

   template <class T>
   Stack<void*> MemBank<T>::avail;

   template <class T>
   Stack<char*> MemBank<T>::buffers;

   template <class T>
   typename MemBank<T>::MemBankClear MemBank<T>::clear;

   template <class T>
   static void _MemBankDeleteProxy(T* a) {MemBank<T>::deallocate(a);}
   };
