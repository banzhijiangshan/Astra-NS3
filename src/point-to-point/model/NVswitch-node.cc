// File: NVswitch-node.cc
// Added by myself 2024-08-08

#include "ns3/NVswitch-node.h"

#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/int-header.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"

#include <cmath>

namespace ns3
{

TypeId
NVSwitchNode::GetTypeId()
{
    static TypeId tid = TypeId("ns3::NVSwitchNode")
                            .SetParent<Node>()
                            .AddConstructor<NVSwitchNode>()
                            .AddAttribute("id",
                                          "The id of the NVSwitchNode",
                                          UintegerValue(0),
                                          MakeUintegerAccessor(&NVSwitchNode::NodeId),
                                          MakeUintegerChecker<uint32_t>());
    return tid;
}

NVSwitchNode::NVSwitchNode()
{
    NodeId = 0;
    m_node_type = 2;
}

uint32_t
NVSwitchNode::AddDevice(Ptr<NVLinkNetDevice> device)
{
    m_devices.push_back(device);
    device->SetNode(this);
    device->SetIfIndex(m_devices.size() - 1);
    return m_devices.size() - 1;
}

int
NVSwitchNode::GetOutDev(int dst) // 0~7
{
    return dst % nDevices;
}

void
NVSwitchNode::GetAndSend(Ptr<NVPacket> p, int dst) // dst: node id
{
    int outDev = GetOutDev(dst);
    Ptr<NVLinkNetDevice> outDevice = m_devices[outDev];
    outDevice->SwitchSend(p);
}

} // namespace ns3