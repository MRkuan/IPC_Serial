#include "Msg2MCUhandler.h"

Msg2MCUhandler::Msg2MCUhandler(){
     p_tansportLayer = new TransportLayer();
}

void Msg2MCUhandler::sendDoorInfo(){
    U_PacketHeader ph;
    S_TLV tlv;
    unsigned char value[FramLenMax];

    tlv.tag.Tag.BusinessType=0x2;
    tlv.tag.Tag.BusinessSub1Type=0x11;
    tlv.tag.Tag.BusinessSub2Type=0x0;
    tlv.len=2;
    ph.Packet_Header.PT=0;
    ph.Packet_Header.CID=1;
    ph.Packet_Header.SN=getSN();
    ph.Packet_Header.Reserved=0;
    ph.Packet_Header.LENapp=sizeof(tlv.tag)+1+tlv.len;
    value[0]=1;
    value[1]=2;
    tlv.value=value;
    p_tansportLayer->sendSingleTLV2MCU(ph,tlv);
}

void Msg2MCUhandler::sendLampInfo(){
    U_PacketHeader ph;
    S_TLV tlv;
    unsigned char value[FramLenMax];

    tlv.tag.Tag.BusinessType=0x2;
    tlv.tag.Tag.BusinessSub1Type=0x11;
    tlv.tag.Tag.BusinessSub2Type=0x01;
    tlv.len=2;
    ph.Packet_Header.PT=0;
    ph.Packet_Header.CID=1;
    ph.Packet_Header.SN=getSN();
    ph.Packet_Header.Reserved=0;
    ph.Packet_Header.LENapp=sizeof(tlv.tag)+1+tlv.len;
    value[0]=1;
    value[1]=2;
    tlv.value=value;
    p_tansportLayer->sendSingleTLV2MCU(ph,tlv);
}

void Msg2MCUhandler::sendKeyInfo(){
    U_PacketHeader ph;
    S_TLV tlv;
    unsigned char value[FramLenMax];

    tlv.tag.Tag.BusinessType=0x2;
    tlv.tag.Tag.BusinessSub1Type=0x11;
    tlv.tag.Tag.BusinessSub2Type=0x02;
    tlv.len=2;
    ph.Packet_Header.PT=0;
    ph.Packet_Header.CID=1;
    ph.Packet_Header.SN=getSN();
    ph.Packet_Header.Reserved=0;
    ph.Packet_Header.LENapp=sizeof(tlv.tag)+1+tlv.len;
    value[0]=1;
    value[1]=2;
    tlv.value=value;
    p_tansportLayer->sendSingleTLV2MCU(ph,tlv);
}

void Msg2MCUhandler::sendSpeedInfo(){
    U_PacketHeader ph;
    S_TLV tlv;
    unsigned char value[FramLenMax];

    tlv.tag.Tag.BusinessType=0x2;
    tlv.tag.Tag.BusinessSub1Type=0x11;
    tlv.tag.Tag.BusinessSub2Type=0x03;
    tlv.len=3;
    ph.Packet_Header.PT=0;
    ph.Packet_Header.CID=1;
    ph.Packet_Header.SN=getSN();
    ph.Packet_Header.Reserved=0;
    ph.Packet_Header.LENapp=sizeof(tlv.tag)+1+tlv.len;
    value[0]=1;
    value[1]=2;
    value[2]=3;
    tlv.value=value;
    p_tansportLayer->sendSingleTLV2MCU(ph,tlv);
}
