// File: NV-helper.h
// Added by myself 2024-08-09

#ifndef NV_HELPER_H
#define NV_HELPER_H

#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/object-factory.h"

#include <cstring>
#include <iostream>

namespace ns3
{
class NVLinkNetDevice;
class NVPacket;
class NVLink;
class NVSwitchNode;

class NVHelper
{
  public:
    NVHelper();
    ~NVHelper();

    void SetNVLinkNetDeviceAttribute(std::string name, const AttributeValue& value);
    void SetChannelAttribute(std::string name, const AttributeValue& value);

    NetDeviceContainer Install(NodeContainer c);
    NetDeviceContainer Install(Ptr<Node> a, Ptr<NVSwitchNode> b);
    NetDeviceContainer Install(std::string aName, Ptr<NVSwitchNode> b);
    NetDeviceContainer Install(Ptr<Node> a, std::string bName);
    NetDeviceContainer Install(std::string aName, std::string bName);

    ObjectFactory m_queueFactory;
    ObjectFactory m_deviceFactory;
    ObjectFactory m_channelFactory;
    ObjectFactory m_remoteChannelFactory;
};
} // namespace ns3

#endif // NV_HELPER_H