// File: p2p-NVlink-channel.h
// Added by myself 2024-08-07

#ifndef QBB_CHANNEL_H
#define QBB_CHANNEL_H

#include "ns3/NV-packet.h"
#include "ns3/channel.h"
#include "ns3/data-rate.h"
#include "ns3/nstime.h"
#include "ns3/point-to-point-channel.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"

#include <list>

namespace ns3
{
class NVLinkNetDevice;
class NVPacket;

class NVLink : public PointToPointChannel
{
  public:
    static TypeId GetTypeId(void);

    NVLink();

    void Attach(Ptr<NVLinkNetDevice> device);

    virtual bool TransmitStart(Ptr<NVPacket> p, Ptr<NVLinkNetDevice> src, Time txTime);

    virtual size_t GetNDevices(void) const;

    Ptr<NVLinkNetDevice> GetNVNetdevice(size_t i) const;
    virtual Ptr<NetDevice> GetDevice(std::size_t i) const;

    Time GetDelay(void) const;

    bool IsInitialized(void) const;

    Ptr<NVLinkNetDevice> GetSource(uint32_t i) const;
    Ptr<NVLinkNetDevice> GetDestination(uint32_t i) const;

    static const int N_DEVICES = 2;

    Time m_delay;
    int32_t m_nDevices;

    TracedCallback<Ptr<const Packet>, Ptr<NVLinkNetDevice>, Ptr<NVLinkNetDevice>, Time, Time>
        m_txrxNV;

    enum WireState
    {
        INITIALIZING,
        IDLE,
        TRANSMITTING,
        PROPAGATING
    };

    class Link
    {
      public:
        Link()
            : m_state(INITIALIZING),
              m_src(0),
              m_dst(0)
        {
        }

        WireState m_state;
        Ptr<NVLinkNetDevice> m_src;
        Ptr<NVLinkNetDevice> m_dst;
    };

    Link m_link[N_DEVICES];
};

} // namespace ns3

#endif // QBB_CHANNEL_H