// File: NV-packet.cc
// Added by myself 2024-08-08

#include "ns3/NV-packet.h"

namespace ns3
{
NVPacket::NVPacket()
{
    m_src = 0;
    m_dst = 0;
}

NVPacket::NVPacket(uint32_t size)
    : Packet(size)
{
    m_src = 0;
    m_dst = 0;
}
} // namespace ns3