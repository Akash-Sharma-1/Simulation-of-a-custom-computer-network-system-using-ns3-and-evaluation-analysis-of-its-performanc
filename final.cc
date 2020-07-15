#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/netanim-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"

#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("SocketBoundRoutingExample");


int 
main (int argc, char *argv[])
{

  // Allow the user to override any of the defaults and the above
  // DefaultValue::Bind ()s at run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  Ptr<Node> nSrc1 = CreateObject<Node> ();
  Ptr<Node> nSrc2 = CreateObject<Node> ();
  Ptr<Node> nSrc3 = CreateObject<Node> ();
  Ptr<Node> nSrc4 = CreateObject<Node> ();
  Ptr<Node> nRtr1 = CreateObject<Node> ();
  Ptr<Node> nRtr2 = CreateObject<Node> ();
  Ptr<Node> nRtr3 = CreateObject<Node> ();
  Ptr<Node> nRtr4 = CreateObject<Node> ();
  Ptr<Node> nRtr5 = CreateObject<Node> ();
  Ptr<Node> nDst5 = CreateObject<Node> ();


  NodeContainer c1 = NodeContainer (nSrc1,nSrc2,nSrc3,nSrc4,nRtr1);
  NodeContainer c2 = NodeContainer (nRtr2, nRtr3,nRtr4, nRtr5,nDst5);


  InternetStackHelper internet;
  internet.Install (c1);
  internet.Install (c2);


  // Point-to-point links
  NodeContainer nSrc1nRtr1 = NodeContainer (nSrc1, nRtr1);
  NodeContainer nSrc2nRtr2 = NodeContainer (nSrc2, nRtr2);
  NodeContainer nSrc3nRtr3 = NodeContainer (nSrc3, nRtr3);
  NodeContainer nSrc4nRtr4 = NodeContainer (nSrc4, nRtr4);

  NodeContainer nRtr1nRtr2 = NodeContainer (nRtr1, nRtr2);
  NodeContainer nRtr2nRtr4 = NodeContainer (nRtr2, nRtr4);
  NodeContainer nRtr4nRtr5 = NodeContainer (nRtr4, nRtr5);
  NodeContainer nRtr1nRtr3 = NodeContainer (nRtr1, nRtr3);
  NodeContainer nRtr3nRtr5 = NodeContainer (nRtr3, nRtr5);
  NodeContainer nRtr5nDst5 = NodeContainer (nRtr5, nDst5);




  // We create the channels first without any IP addressing information
PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  p2p.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("1p"));

  NetDeviceContainer dSrc1dRtr1 = p2p.Install (nSrc1nRtr1);
  NetDeviceContainer dSrc2dRtr2 = p2p.Install (nSrc2nRtr2);
  NetDeviceContainer dSrc3dRtr3 = p2p.Install (nSrc3nRtr3);

  p2p.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms"));
  p2p.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("1p"));

  NetDeviceContainer dSrc4dRtr4 = p2p.Install (nSrc4nRtr4);
  NetDeviceContainer dRtr1dRtr3 = p2p.Install (nRtr1nRtr3);
  NetDeviceContainer dRtr1dRtr2 = p2p.Install (nRtr1nRtr2);
  
  p2p.SetDeviceAttribute ("DataRate", StringValue ("2Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("5ms"));
  p2p.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("1p"));

  NetDeviceContainer dRtr2dRtr4 = p2p.Install (nRtr2nRtr4);
  NetDeviceContainer dRtr4dRtr5 = p2p.Install (nRtr4nRtr5);

  p2p.SetDeviceAttribute ("DataRate", StringValue ("0.5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
  p2p.SetQueue ("ns3::DropTailQueue","MaxSize", StringValue ("2p"));


  NetDeviceContainer dRtr3dRtr5 = p2p.Install (nRtr3nRtr5);
  NetDeviceContainer dRtr5dDst5 = p2p.Install (nRtr5nDst5);
  // Ptr<NetDevice> SrcToRtr1=dSrcdRtr1.Get (0);
  // Ptr<NetDevice> SrcToRtr2=dRt1dRtr2.Get (0);
  // Later, we add IP addresses.
  Ipv4AddressHelper ipv4;
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer iSrciRtr1 = ipv4.Assign (dSrc1dRtr1);
  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iSrc2iRtr2 = ipv4.Assign (dSrc2dRtr2);
  ipv4.SetBase ("10.10.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iSrc3iRtr3 = ipv4.Assign (dSrc3dRtr3);
  ipv4.SetBase ("10.20.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iSrc4iRtr4 = ipv4.Assign (dSrc4dRtr4);
  ipv4.SetBase ("10.30.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iRtr1iRtr3 = ipv4.Assign (dRtr1dRtr3);
  ipv4.SetBase ("10.40.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iRtr1iRtr2 = ipv4.Assign (dRtr1dRtr2);
  ipv4.SetBase ("10.50.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iRtr2iRtr4 = ipv4.Assign (dRtr2dRtr4);
  ipv4.SetBase ("10.60.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iRtr4iRtr5 = ipv4.Assign (dRtr4dRtr5);
  ipv4.SetBase ("10.70.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iRtr3iRtr5 = ipv4.Assign (dRtr3dRtr5);
  ipv4.SetBase ("10.80.2.0", "255.255.255.0");
  Ipv4InterfaceContainer iRtr5iDst5 = ipv4.Assign (dRtr5dDst5);


  Ptr<Ipv4> ipv4Src1 = nSrc1->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4Src2 = nSrc2->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4Src3 = nSrc3->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4Src4 = nSrc4->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4Rtr1 = nRtr1->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4Rtr2 = nRtr2->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4Rtr3 = nRtr3->GetObject<Ipv4> ();
  Ptr<Ipv4> ipv4Rtr4 = nRtr4->GetObject<Ipv4> (); 
  Ptr<Ipv4> ipv4Rtr5 = nRtr5->GetObject<Ipv4> (); 
  Ptr<Ipv4> ipv4Dst5 = nDst5->GetObject<Ipv4> ();

  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> staticRoutingSrc1 = ipv4RoutingHelper.GetStaticRouting (ipv4Src1);
  Ptr<Ipv4StaticRouting> staticRoutingSrc2 = ipv4RoutingHelper.GetStaticRouting (ipv4Src2);
  Ptr<Ipv4StaticRouting> staticRoutingSrc3 = ipv4RoutingHelper.GetStaticRouting (ipv4Src3);
  Ptr<Ipv4StaticRouting> staticRoutingSrc4 = ipv4RoutingHelper.GetStaticRouting (ipv4Src4);
  Ptr<Ipv4StaticRouting> staticRoutingRtr1 = ipv4RoutingHelper.GetStaticRouting (ipv4Rtr1);
  Ptr<Ipv4StaticRouting> staticRoutingRtr2 = ipv4RoutingHelper.GetStaticRouting (ipv4Rtr2);
  Ptr<Ipv4StaticRouting> staticRoutingRtr3 = ipv4RoutingHelper.GetStaticRouting (ipv4Rtr3);
  Ptr<Ipv4StaticRouting> staticRoutingRtr4 = ipv4RoutingHelper.GetStaticRouting (ipv4Rtr4);
  Ptr<Ipv4StaticRouting> staticRoutingRtr5 = ipv4RoutingHelper.GetStaticRouting (ipv4Rtr5);
  Ptr<Ipv4StaticRouting> staticRoutingDst5 = ipv4RoutingHelper.GetStaticRouting (ipv4Dst5);

  // Create static routes from Src1 to Dst5
  staticRoutingSrc1->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.1.1.2"), 1); // SRC1 -> R1 
  staticRoutingRtr1->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.30.2.2"), 2);// R1 -> R3
  staticRoutingRtr3->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.70.2.2"), 3);// R3 -> R5
  staticRoutingRtr5->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.80.2.2"), 3);// R5 -> DST5


 // Create static routes from Src2 to Dst5
  staticRoutingSrc2->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.1.2.2"), 1); // SRC2 -> R2 
  staticRoutingRtr2->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.50.2.2"), 3);// R2 -> R4
  staticRoutingRtr4->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.60.2.2"), 3);// R4 -> R5
  // staticRoutingRtr5->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.80.2.2"), 3);// R5 -> DST5



 // Create static routes from Src3 to Dst5
  staticRoutingSrc3->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.10.2.2"), 1); // SRC2 -> R2 
  staticRoutingRtr4->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.70.2.2"), 3);// R3 -> R5
  // staticRoutingRtr5->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.80.2.2"), 3);// R5 -> DST5

  // Create static routes from Src4 to Dst5
  staticRoutingSrc4->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.20.2.2"), 1); // SRC4 -> R4 
  staticRoutingRtr4->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.60.2.2"), 3);// R4 -> R5
  // staticRoutingRtr5->AddHostRouteTo (Ipv4Address ("10.80.2.2"), Ipv4Address ("10.80.2.2"), 3);// R5 -> DST5


  // Two routes to same destination - setting separate metrics. 
  // You can switch these to see how traffic gets diverted via different routes
  // staticRoutingSrc->AddHostRouteTo (Ipv4Address ("10.40.2.2"), Ipv4Address ("10.1.1.2"), 1); // sr -> r1 -> dst

  OnOffHelper onoff ("ns3::UdpSocketFactory", Address(InetSocketAddress (iRtr5iDst5.GetAddress(1),9)));
  // char stringDurationOn[300];
  // double durationOn;
  // durationOn=(double) 
  onoff.SetAttribute("OnTime",StringValue("ns3::ConstantRandomVariable[Constant=0.0001583]"));        
  onoff.SetAttribute("OffTime",StringValue("ns3::ExponentialRandomVariable[Mean=0.01]")); 
  onoff.SetConstantRate (DataRate (119500));
  ApplicationContainer apps = onoff.Install (nSrc1);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (60.0));

  // UdpEchoServerHelper echoServer (9);

  // ApplicationContainer serverApps = echoServer.Install (nRtr5nDst5.Get (1));
  // serverApps.Start (Seconds (1.0));
  // serverApps.Stop (Seconds (20.0));

  // UdpEchoClientHelper echoClient (iRtr5iDst5.GetAddress (1), 9);
  // echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  // echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  // echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  // ApplicationContainer clientApps = echoClient.Install (nSrc1nRtr1.Get (0));
  // clientApps.Start (Seconds (2.0));
  // clientApps.Stop (Seconds (20.0));

  OnOffHelper onoff2 ("ns3::UdpSocketFactory", Address(InetSocketAddress (iRtr5iDst5.GetAddress(1),10)));
  // char stringDurationOn[300];
  // double durationOn;
  // durationOn=(double) 
  onoff2.SetConstantRate (DataRate (119500));
  ApplicationContainer apps2 = onoff2.Install (nSrc2);
  apps2.Start (Seconds (1.0));
  apps2.Stop (Seconds (60.0));

  OnOffHelper onoff3 ("ns3::UdpSocketFactory", Address(InetSocketAddress (iRtr5iDst5.GetAddress(1),11)));
  // char stringDurationOn[300];
  // double durationOn;
  // durationOn=(double) 
  onoff3.SetConstantRate (DataRate (119500));
  ApplicationContainer apps3 = onoff3.Install (nSrc3);
  apps3.Start (Seconds (1.0));
  apps3.Stop (Seconds (60.0));

  OnOffHelper onoff4 ("ns3::UdpSocketFactory", Address(InetSocketAddress (iRtr5iDst5.GetAddress(1),12)));
  // char stringDurationOn[300];
  // double durationOn;
  // durationOn=(double) 
  onoff4.SetConstantRate (DataRate (119500));
  ApplicationContainer apps4= onoff4.Install (nSrc4);
  apps4.Start (Seconds (1.0));
  apps4.Stop (Seconds (60.0)); 


  // ApplicationContainer clientApps_two = echoClient_two.Install (nSrc2nRtr2.Get (0));
  // clientApps_two.Start (Seconds (2.0));
  // clientApps_two.Stop (Seconds (10.0));

  // AsciiTraceHelper ascii;
  // p2p.EnableAsciiAll (ascii.CreateFileStream ("socket-bound-static-routing.tr"));
  // p2p.EnablePcapAll ("socket-bound-static-routing");

  // UdpEchoServerHelper echoServer_three (11);

  // ApplicationContainer serverApps_three = echoServer_three.Install (nRtr5nDst5.Get (1));
  // serverApps_three.Start (Seconds (1.0));
  // serverApps_three.Stop (Seconds (10.0));

  // UdpEchoClientHelper echoClient_three (iRtr5iDst5.GetAddress (1), 11);
  // echoClient_three.SetAttribute ("MaxPackets", UintegerValue (1));
  // echoClient_three.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  // echoClient_three.SetAttribute ("PacketSize", UintegerValue (1024));

  // ApplicationContainer clientApps_three = echoClient_three.Install (nSrc3nRtr3.Get (0));
  // clientApps_three.Start (Seconds (2.0));
  // clientApps_three.Stop (Seconds (10.0));

  //  UdpEchoServerHelper echoServer_four (12);

  // ApplicationContainer serverApps_four = echoServer_four.Install (nRtr5nDst5.Get (1));
  // serverApps_four.Start (Seconds (1.0));
  // serverApps_four.Stop (Seconds (10.0));

  // UdpEchoClientHelper echoClient_four (iRtr5iDst5.GetAddress (1), 12);
  // echoClient_four.SetAttribute ("MaxPackets", UintegerValue (1));
  // echoClient_four.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  // echoClient_four.SetAttribute ("PacketSize", UintegerValue (1024));

  // ApplicationContainer clientApps_four = echoClient_four.Install (nSrc4nRtr4.Get (0));
  // clientApps_four.Start (Seconds (2.0));
  // clientApps_four.Stop (Seconds (10.0));


  AnimationInterface anim("final_net.xml");
  anim.SetConstantPosition(nSrc1nRtr1.Get(0),0.0,0.0);
  anim.SetConstantPosition(nSrc2nRtr2.Get(0),60.0,0.0);
  anim.SetConstantPosition(nSrc3nRtr3.Get(0),0.0,20.0);
  anim.SetConstantPosition(nSrc4nRtr4.Get(0),60.0,20.0);
  anim.SetConstantPosition(nSrc1nRtr1.Get(1),20.0,0.0);
  anim.SetConstantPosition(nSrc2nRtr2.Get(1),40.0,0.0);
  anim.SetConstantPosition(nSrc3nRtr3.Get(1),20.0,20.0);
  anim.SetConstantPosition(nSrc4nRtr4.Get(1),40.0,20.0);
  anim.SetConstantPosition(nRtr3nRtr5.Get(1),30.0,30.0);  
  anim.SetConstantPosition(nRtr5nDst5.Get(1),30.0,40.0);    

  Ptr<FlowMonitor> flowmonitor;
  FlowMonitorHelper flowHelper;
  flowmonitor = flowHelper.InstallAll();
  Simulator::Stop((Seconds(60)));


  Simulator::Run ();
  Simulator::Destroy ();

  flowmonitor->SerializeToXmlFile("final_flowmonitor.xml",true,true);
  return 0;

}