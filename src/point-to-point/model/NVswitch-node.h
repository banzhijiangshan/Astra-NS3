// File: NVswitch-node.h
// Added by myself 2024-08-08

#ifndef NVSWITCH_NODE_H
#define NVSWITCH_NODE_H

#include "pint.h"
#include "switch-mmu.h"

#include "ns3/NV-packet.h"
#include "ns3/NVLink-net-device.h"
#include <ns3/node.h>

#include <unordered_map>
#include <vector>

namespace ns3
{
class NVPacket;

class NVSwitchNode : public Node
{
  public:
    static TypeId GetTypeId();
    static const int nDevices = 8;
    NVSwitchNode();

    int NodeId;
    std::vector<Ptr<NVLinkNetDevice>> m_devices;
    std::unordered_map<uint32_t, Address> m_rtTable; // map from id to address

    int GetOutDev(int dst);
    void GetAndSend(Ptr<NVPacket> p, int dst);
    uint32_t AddDevice(Ptr<NVLinkNetDevice> device);
};

} // namespace ns3

#endif // NVSWITCH_NODE_H