// File: p2p-NVlink-channel.cc
// Added by myself 2024-08-07

#include "ns3/p2p-NVlink-channel.h"

#include "ns3/NVLink-net-device.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"

#include <fstream>
#include <iostream>

namespace ns3
{
NS_OBJECT_ENSURE_REGISTERED(NVLink);

TypeId
NVLink::GetTypeId(void)
{
    static TypeId tid = TypeId("ns3::NVLink")
                            .SetParent<Channel>()
                            .AddConstructor<NVLink>()
                            .AddAttribute("Delay",
                                          "Transmission delay through the channel",
                                          TimeValue(Seconds(0)),
                                          MakeTimeAccessor(&NVLink::m_delay),
                                          MakeTimeChecker())
                            .AddTraceSource("TxRxNV",
                                            "Trace source indicating transmission of packet from "
                                            "the NVLink, used by the Animation interface.",
                                            MakeTraceSourceAccessor(&NVLink::m_txrxNV),
                                            "ns3::Packet::TracedCallback");

    return tid;
}

NVLink::NVLink()
    : PointToPointChannel()
{
    m_nDevices = 0;
}

void
NVLink::Attach(Ptr<NVLinkNetDevice> device)
{
    NS_ASSERT_MSG(m_nDevices < N_DEVICES, "Only two devices permitted");
    NS_ASSERT(device);

    m_link[m_nDevices++].m_src = device;

    if (m_nDevices == N_DEVICES)
    {
        m_link[0].m_state = WireState::IDLE;
        m_link[1].m_state = WireState::IDLE;
        m_link[0].m_dst = m_link[1].m_src;
        m_link[1].m_dst = m_link[0].m_src;
    }
}

bool
NVLink::TransmitStart(Ptr<NVPacket> p, Ptr<NVLinkNetDevice> src, Time txTime)
{
    uint32_t wire = src == m_link[0].m_src ? 0 : 1;

    Simulator::ScheduleWithContext(m_link[wire].m_dst->GetNode()->GetId(),
                                   txTime + m_delay,
                                   &NVLinkNetDevice::Receive,
                                   m_link[wire].m_dst,
                                   p);

    m_txrxNV(p, src, m_link[wire].m_dst, txTime, txTime + m_delay);
    return true;
}

size_t
NVLink::GetNDevices(void) const
{
    return m_nDevices;
}

Ptr<NVLinkNetDevice>
NVLink::GetNVNetdevice(size_t i) const
{
    NS_ASSERT(i < N_DEVICES);
    return m_link[i].m_src;
}

Ptr<NetDevice>
NVLink::GetDevice(std::size_t i) const
{
    return GetNVNetdevice(i);
}

Time
NVLink::GetDelay(void) const
{
    return m_delay;
}

Ptr<NVLinkNetDevice>
NVLink::GetSource(uint32_t i) const
{
    return m_link[i].m_src;
}

Ptr<NVLinkNetDevice>
NVLink::GetDestination(uint32_t i) const
{
    return m_link[i].m_dst;
}

bool
NVLink::IsInitialized(void) const
{
    NS_ASSERT(m_link[0].m_state != WireState::INITIALIZING);
    NS_ASSERT(m_link[1].m_state != WireState::INITIALIZING);
    return true;
}

} // namespace ns3