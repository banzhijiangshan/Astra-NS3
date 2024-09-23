// File: NV-helper.cc
// Added by myself 2024-08-09

#include "NV-helper.h"

#include "ns3/NVLink-net-device.h"
#include "ns3/NVswitch-node.h"
#include "ns3/abort.h"
#include "ns3/config.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/log.h"
#include "ns3/mpi-interface.h"
#include "ns3/mpi-receiver.h"
#include "ns3/names.h"
#include "ns3/p2p-NVlink-channel.h"
#include "ns3/p2p-NVlink-remote-channel.h"
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/simulator.h"
#include "ns3/uinteger.h"

NS_LOG_COMPONENT_DEFINE("NVHelper");

namespace ns3
{
NVHelper::NVHelper()
{
    m_queueFactory.SetTypeId("ns3::DropTailQueue<NVPacket>");
    m_deviceFactory.SetTypeId("ns3::NVLinkNetDevice");
    m_channelFactory.SetTypeId("ns3::NVLink");
    m_remoteChannelFactory.SetTypeId("ns3::RemoteNVLink");
}

NVHelper::~NVHelper()
{
}

void
NVHelper::SetNVLinkNetDeviceAttribute(std::string name, const AttributeValue& value)
{
    m_deviceFactory.Set(name, value);
}

void
NVHelper::SetChannelAttribute(std::string name, const AttributeValue& value)
{
    m_channelFactory.Set(name, value);
}

NetDeviceContainer
NVHelper::Install(NodeContainer c)
{
    NS_ASSERT(c.GetN() == 2);
    return Install(c.Get(0), DynamicCast<NVSwitchNode>(c.Get(1)));
}

NetDeviceContainer
NVHelper::Install(Ptr<Node> a, Ptr<NVSwitchNode> b)
{
    NetDeviceContainer container;
    Ptr<NVLinkNetDevice> deviceA = m_deviceFactory.Create<NVLinkNetDevice>();
    Ptr<NVLinkNetDevice> deviceB = m_deviceFactory.Create<NVLinkNetDevice>();
    deviceA->SetAddress(Mac48Address::Allocate());
    deviceB->SetAddress(Mac48Address::Allocate());
    a->AddDevice(deviceA);
    b->AddDevice(deviceB);

    Ptr<DropTailQueue<NVPacket>> queueA = m_queueFactory.Create<DropTailQueue<NVPacket>>();
    Ptr<DropTailQueue<NVPacket>> queueB = m_queueFactory.Create<DropTailQueue<NVPacket>>();
    deviceA->SetQueue(queueA);
    deviceB->SetQueue(queueB);

    bool useNormalChannel = true;
    Ptr<NVLink> channel = 0;
    if (MpiInterface::IsEnabled())
    {
        uint32_t n1SystemId = a->GetSystemId();
        uint32_t n2SystemId = b->GetSystemId();
        uint32_t currSystemId = MpiInterface::GetSystemId();
        if (n1SystemId != currSystemId || n2SystemId != currSystemId)
        {
            useNormalChannel = false;
        }
    }
    if (useNormalChannel)
        channel = m_channelFactory.Create<NVLink>();
    else
    {
        channel = m_remoteChannelFactory.Create<RemoteNVLink>();
        Ptr<MpiReceiver> mpiRecA = CreateObject<MpiReceiver>();
        Ptr<MpiReceiver> mpiRecB = CreateObject<MpiReceiver>();
        mpiRecA->SetReceiveCallbackNV(MakeCallback(&NVLinkNetDevice::Receive, deviceA));
        mpiRecB->SetReceiveCallbackNV(MakeCallback(&NVLinkNetDevice::Receive, deviceB));
        deviceA->AggregateObject(mpiRecA);
        deviceB->AggregateObject(mpiRecB);
    }

    deviceA->Attach(channel);
    deviceB->Attach(channel);

    // b->m_devices.push_back(deviceB);
    container.Add(deviceA);
    container.Add(deviceB);
    return container;
}

NetDeviceContainer
NVHelper::Install(std::string aName, Ptr<NVSwitchNode> b)
{
    Ptr<Node> a = Names::Find<Node>(aName);
    return Install(a, b);
}

NetDeviceContainer
NVHelper::Install(Ptr<Node> a, std::string bName)
{
    Ptr<NVSwitchNode> b = Names::Find<NVSwitchNode>(bName);
    return Install(a, b);
}

NetDeviceContainer
NVHelper::Install(std::string aName, std::string bName)
{
    Ptr<Node> a = Names::Find<Node>(aName);
    Ptr<NVSwitchNode> b = Names::Find<NVSwitchNode>(bName);
    return Install(a, b);
}

} // namespace ns3