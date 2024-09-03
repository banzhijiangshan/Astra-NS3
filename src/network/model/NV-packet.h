// File: NV-packet.h
// Added by myself 2024-08-08

#ifndef NV_PACKET_H
#define NV_PACKET_H

#include "ns3/packet.h"
#include "ns3/ptr.h"

namespace ns3
{
class NVPacket : public Packet // convenient for the NVLinkNetDevice to send packets
{
  public:
    int m_src;
    int m_dst;
    uint32_t m_leftnum;
    uint32_t srcPort;
    uint32_t totalSize;
    NVPacket();
    NVPacket(uint32_t size);
};
} // namespace ns3

#endif // NV_PACKET_H