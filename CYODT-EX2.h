#include <iostream>
#define SCLASS(name) class name : public CYODT::MemCheck<name>

namespace CYODT 
   {
   template <class T>
   class MemCheck
      {
      static unsigned int cnt;
      class Alert
         {
         ~Alert()
            {
            if(cnt != 0u)
               {
               int a;
               std::cout << "Warning: " << cnt << " objects were not deallocated" << std::endl;
               std::cin >> a;
               }
            }
         };
      static Alert alert;
      protected:
      MemCheck() {cnt++;}
      virtual ~MemCheck() {cnt--;}
      };
   }