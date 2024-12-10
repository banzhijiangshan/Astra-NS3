// File: NVLink-net-device.cc
// Added by myself 2024-08-07

#include "NVLink-net-device.h"

#include "ns3/NV-packet.h"
#include "ns3/log.h"
#include "ns3/mac48-address.h"
#include "ns3/p2p-NVlink-channel.h"
#include "ns3/pointer.h"
#include "ns3/queue-size.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/uinteger.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("NVLinkNetDevice");

NS_OBJECT_ENSURE_REGISTERED(NVLinkNetDevice);

TypeId
NVLinkNetDevice::GetTypeId()
{
    static TypeId tid = TypeId("ns3::NVLinkNetDevice")
                            .SetParent<PointToPointNetDevice>()
                            .AddConstructor<NVLinkNetDevice>();

    return tid;
}

NVLinkNetDevice::NVLinkNetDevice()
    : PointToPointNetDevice()
{
    NS_LOG_FUNCTION(this);
}

NVLinkNetDevice::~NVLinkNetDevice()
{
    NS_LOG_FUNCTION(this);
}

bool
NVLinkNetDevice::Attach(Ptr<NVLink> ch)
{
    NS_LOG_FUNCTION(this << ch);
    m_channel = ch;
    m_channel->Attach(this);

    NotifyLinkUp();
    return true;
}

void
NVLinkNetDevice::AddHeader(Ptr<NVPacket> p, uint16_t protocolNumber)
{
}

bool
NVLinkNetDevice::ProcessHeader(Ptr<NVPacket> p, uint16_t& protocol)
{
    return true;
}

bool
NVLinkNetDevice::Send(Ptr<NVPacket> packet, const Address& dest, uint16_t protocolNumber)
{
    NS_LOG_FUNCTION(this << packet << dest << protocolNumber);
    if (!IsLinkUp())
    {
        m_macTxDropTrace(packet);
        return false;
    }
    AddHeader(packet, protocolNumber);

    m_macTxTrace(packet);

    if (m_queue->Enqueue(packet))
    {
        if (m_txMachineState == READY)
        {
            packet = m_queue->Dequeue();
            bool ret = TransmitStart(packet);
            return ret;
        }
        return true;
    }
    m_macTxDropTrace(packet);
    return false;
}

void
NVLinkNetDevice::Receive(Ptr<NVPacket> packet)
{
    NS_LOG_FUNCTION(this << packet);
    uint16_t protocol = 0;

    if (!ProcessHeader(packet, protocol))
    {
        NS_LOG_DEBUG("Packet dropped by ProcessHeader");
        return;
    }
    Ptr<Packet> originalPacket = packet->Copy();
    if (!m_promiscCallback.IsNull())
    {
        m_macPromiscRxTrace(originalPacket);
        m_promiscCallback(this,
                          packet,
                          protocol,
                          GetRemote(),
                          GetAddress(),
                          NetDevice::PACKET_HOST);
    }
    if (m_node->GetNodeType() == 2) // NVSwitchNode
    {
        Ptr<NVSwitchNode> switchNode = DynamicCast<NVSwitchNode>(m_node);
        switchNode->GetAndSend(packet,
                               packet->m_dst); // m_dst is the destination node id(最终目的)
    }
    else
    {
        // printf("%d\n", packet->m_leftnum);
        if (packet->m_leftnum == 0)
            m_flowfinishCb(packet->m_src, packet->m_dst, packet->totalSize, packet->srcPort);
    }
    // m_rxCallback(this, packet, protocol, GetRemote());
}

bool
NVLinkNetDevice::TransmitStart(Ptr<NVPacket> p)
{
    m_txMachineState = BUSY;
    m_currentPkt = p;

    Time txTime = m_bps.CalculateBytesTxTime(p->GetSize());
    Time txcompleteTime = txTime + m_tInterframeGap;

    Simulator::Schedule(txcompleteTime, &NVLinkNetDevice::TransmitComplete, this);
    bool res = m_channel->TransmitStart(p, this, txTime);
    return res;
}

void
NVLinkNetDevice::TransmitComplete()
{
    m_txMachineState = READY;
    m_currentPkt = nullptr;
    Ptr<NVPacket> p = m_queue->Dequeue();
    if (!p)
    {
        return;
    }
    TransmitStart(p);
}

bool
NVLinkNetDevice::SwitchSend(Ptr<NVPacket> p)
{
    m_queue->Enqueue(p);
    if (m_txMachineState == READY)
    {
        p = m_queue->Dequeue();
        bool ret = TransmitStart(p);
        return ret;
    }
    return true;
}

void
NVLinkNetDevice::SetQueue(Ptr<Queue<NVPacket>> queue)
{
    m_queue = queue;
}

void
NVLinkNetDevice::SetQueueSize(uint32_t queueSize)
{
    QueueSize qSize(QueueSizeUnit::PACKETS, queueSize);
    m_queue->SetMaxSize(qSize);
}

} // namespace ns3