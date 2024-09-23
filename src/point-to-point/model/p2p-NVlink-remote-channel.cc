// Added by myself 2024-09-20

#include "p2p-NVlink-remote-channel.h"

#include "NVLink-net-device.h"

#include "ns3/NV-packet.h"
#include "ns3/log.h"
#include "ns3/mpi-interface.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"

#include <iostream>

using namespace std;

namespace ns3
{
NS_LOG_COMPONENT_DEFINE("RemoteNVLink");

NS_OBJECT_ENSURE_REGISTERED(RemoteNVLink);

TypeId
RemoteNVLink::GetTypeId(void)
{
    static TypeId tid =
        TypeId("ns3::RemoteNVLink").SetParent<NVLink>().AddConstructor<RemoteNVLink>();
    return tid;
}

RemoteNVLink::RemoteNVLink()
{
}

RemoteNVLink::~RemoteNVLink()
{
}

bool
RemoteNVLink::TransmitStart(Ptr<NVPacket> p, Ptr<NVLinkNetDevice> src, Time txTime)
{
    uint32_t wire = src == GetSource(0) ? 0 : 1;
    Ptr<NVLinkNetDevice> dst = GetDestination(wire);

#ifdef NS3_MPI
    // Calculate the rxTime (absolute)
    Time rxTime = Simulator::Now() + txTime + GetDelay();
    MpiInterface::SendPacket(p, rxTime, dst->GetNode()->GetId(), dst->GetIfIndex());
#else
    NS_FATAL_ERROR("Can't use distributed simulator without MPI compiled in");
#endif
    return true;
}
} // namespace ns3