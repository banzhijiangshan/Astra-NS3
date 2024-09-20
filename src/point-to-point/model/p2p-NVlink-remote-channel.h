// Added by myself 2024-09-20

#ifndef P2P_NVLINK_REMOTE_CHANNEL_H
#define P2P_NVLINK_REMOTE_CHANNEL_H

#include "p2p-NVlink-channel.h"

namespace ns3
{
class RemoteNVLink : public NVLink
{
  public:
    static TypeId GetTypeId(void);

    RemoteNVLink();
    ~RemoteNVLink();

    virtual bool TransmitStart(Ptr<NVPacket> p, Ptr<NVLinkNetDevice> src, Time txTime);
};
} // namespace ns3

#endif