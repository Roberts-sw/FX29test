#ifndef FX29_h
#define FX29_h
/* fx29.h
------------------------------------------------------------------------
datasheet:
    output = 1000 + 14000 * Force / Range

Type FX29K0-040B-0010-L heeft Range 10 lbf

https://www.convertunits.com/from/lbs/to/Newton
    1 N ~ 0,22480894244319 lbs

omrekening output --> mN, Arduino deelt traag,
    dus vermenigvuldigen en schuiven:

    Force
    = (output - 1000) * Range / 14000 [lbf]
    = (output - 1000) * 10 / 14000 / 0,22... [N]
    ~ (output - 1000) * 3,1773011630363
    ~ (output - 1000) * 52057 >> 14
    ~ 6 significante cijfers, dus ruim voldoende voor 16-bit

auteur: Robert van Lierop
datum:  29 maart 2021
--------------------------------------------------------------------- */
#include <Arduino.h>
#include <Wire.h>

typedef unsigned char   u08;
typedef unsigned short  u16;
typedef unsigned long   u32;

#define FX29_DEVICE 0x28

struct FX29 {
    u08 _i2c_adres; //interne gegevens

    void begin(u08 address)
    {   this->_i2c_adres=address;//ter verduidelijking: this-> is niet nodig 
        Wire.begin();
    }
    
    u16 read(void)
    {   //0..16383 - sensoruitlezing 14-bit vlg. datasheet
        // -1      - fout
        u16 data;
        uint8_t a=_i2c_adres;   //ter verduideliking: hier is het weggelaten
        uint8_t nB=2;
        switch (Wire.requestFrom(a, nB) )//levert als het lukt nB Bytes op
        {   case 2://OK: Big-Endian data, Horner-algoritme, 14-bit
                data = ( Wire.read()<<8 | Wire.read() ) & 0x3fff;
                break;
            default://fout, waarde -1 is de grootste unsigned waarde
                data = (u16)-1;
                break;
        }
        return data;
    }
    
    u16 in_mN(u16 rawdata)
    {   return (u16)-1 != rawdata           //test 
        ? (rawdata - 1000) * 52057UL >> 14  //? gelukt-waarde
        : 0;                                //: mislukt-waarde
    }
};

#endif//FX29_h
