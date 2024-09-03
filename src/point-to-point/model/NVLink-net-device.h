// File: NVLink-net-device.h
// Added by myself 2024-08-07

#ifndef NVLINK_NET_DEVICE_H
#define NVLINK_NET_DEVICE_H

#include "p2p-NVlink-channel.h"

#include "ns3/NV-packet.h"
#include "ns3/NVswitch-node.h"
#include "ns3/event-id.h"
#include "ns3/node.h"
#include "ns3/nstime.h"
#include "ns3/packet.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/ptr.h"
#include "ns3/queue-fwd.h"
#include "ns3/traced-callback.h"

#include <map>
#include <vector>

namespace ns3
{

class NVLinkNetDevice : public PointToPointNetDevice
{
  public:
    static TypeId GetTypeId();

    NVLinkNetDevice();
    ~NVLinkNetDevice();

    bool Attach(Ptr<NVLink> ch);

    void AddHeader(Ptr<NVPacket> p, uint16_t protocolNumber);

    bool ProcessHeader(Ptr<NVPacket> p, uint16_t& protocol);

    bool TransmitStart(Ptr<NVPacket> p);
    void TransmitComplete();
    bool Send(Ptr<NVPacket> p, const Address& dest, uint16_t protocolNumber);
    void Receive(Ptr<NVPacket> p);
    bool SwitchSend(Ptr<NVPacket> p);
    void SetQueue(Ptr<Queue<NVPacket>> queue);

    Ptr<NVLink> m_channel;
    Ptr<Queue<NVPacket>> m_queue;

    typedef Callback<void, int, int, uint32_t, uint32_t> NVLinkNetDeviceCallback;
    NVLinkNetDeviceCallback m_flowfinishCb;
};

} // namespace ns3

#endif // NVLINK_NET_DEVICE_H