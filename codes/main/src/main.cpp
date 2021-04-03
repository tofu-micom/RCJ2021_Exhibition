#include <mbed.h>

#include "comms/comms.h"
#include "line/lineTrace.h"

int main() {
  while (1) {
    lineTrace();
  }
}
