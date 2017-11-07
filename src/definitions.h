#ifndef definitions_h
#define definitions_h

// --- Hardware pin definitions ---
#define out_tx 8           // Output TX
#define out_rx 9           // Output RX

#define cs_gl 2            // Chip Select Generator Low
#define cs_gh 3            // Chip Select Generator High
#define cs_a 4             // Chip Select amplitude
#define cs_n 5             // Chip select noise
#define cs_o 6             // Chip Select offset

#define prg_sel 7          // Program Select

// --- EEPROM Memory Directions ---
#define nonv_device 0       // Device EEPROM address
#define nonv_amplitude 8    // Amplitude EEPROM address
#define nonv_noise 16       // Noise EEPROM address
#define nonv_offset 24      // Offset EEPROM address

#endif
