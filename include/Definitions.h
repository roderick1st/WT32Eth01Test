#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#define ETH_CLK_MODE ETH_CLOCK_GPIO0_IN     //  ETH_CLOCK_GPIO17_OUT
#define ETH_POWER_PIN  16                   // Pin# of the enable signal for the external crystal oscillator (-1 to disable for internal APLL source)
#define ETH_TYPE ETH_PHY_LAN8720            // Type of the Ethernet PHY (LAN8720 or TLK110)
#define ETH_ADDR        1                   // I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_MDC_PIN     23                  // Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDIO_PIN    18                  // Pin# of the I²C IO signal for the Ethernet PHY

#define def_LengthOfPandaString 100
#define def_LengthOfNtripString 128
#define def_NtripPort 2233
#define def_ntripPacketHeaderLen 10
#define def_numberOfStoredPakets 16
#define def_maxDataSizeOfStoredPacket 256



#endif