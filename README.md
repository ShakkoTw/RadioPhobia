# RadioPhobia - RF Jammer & WiFi pentesting tool basato su ESP32-C5 e moduli NRF24L01

## Descrizione

Dispositivo di pen-testing delle RF, basato su esp32 C5 e 2x transceiver 2.4GHz NRF24L01 pa lna, che permette di esplorare e studiare le vulnerabilità che riguardano il mondo delle radiofrequenze.

Il dispositivo è reso molto interattivo e autonomo grazie all’utilizzo di un piccolo schermo oled, un sottile joystick e un pulsante di selezione.

Inoltre è dotato di una batteria 18650 con modulo di ricarica e protezione, e di un circuito di regolazione del voltaggio per l’alimentazione, per tenere i 3.3V costanti e sopportare i diversi carichi delle varie modalità di utilizzo.

### Funzionalità

_**Jamming**_

- jamming delle frequenze dalla 2400MHz alla 2525mHz (Bluetooth, BLE, WiFi 2.4GHz, droni RC, ecc…)

_**WiFi pen-testing suite**_

- beacon scanner
- deauther
- evil-twin attack (Fake AccessPoint)
- attacchi combinati deauth+evil-twin in base a frequenza (2.4/5 GHz) e versione WPA(2/3)
    
    
    |  | WPA2 | WPA3 |
    | --- | --- | --- |
    | 2.4GHz | ✅ jamming / deauther + evil twin | ✅ jamming segnale + evil twin |
    | 5GHz | ✅ deauther + evil twin | ⚠️ NIENTE |

    ⚠️ Questo dispositivo non è in grado di effettuare jamming 5GHz e WPA3 non è vulnerabile al deauth (in fututro potrei provare un auth-req spam, a cui alcuni AP WPA3 sono vulnerabili).
