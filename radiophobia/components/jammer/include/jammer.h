#ifndef JAMMER
#define JAMMER

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

void wifiJamAll(void);
void wifiJamSingle(uint8_t channel);
void bluetoothJam(void);

#ifdef __cplusplus
}
#endif

#endif
