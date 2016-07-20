#include "BitVM.h"
#include "MicroBit.h"

namespace SatelliteMicrokit2 {
  void test();
}

void app_main() {
  //bitvm::exec_binary((uint16_t*)bitvm::functionsAndBytecode);
  //printf("Starting radio test\r");    
  SatelliteMicrokit2::test();
  //printf("Finished radio test\r");        
}
