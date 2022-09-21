#include "EmergencyVehicleService.h"
#include "Alerts_msgs/EmergencyClearLane_m.h"
#include "artery/traci/VehicleController.h"
#include <vanetza/btp/data_request.hpp>
#include <vanetza/dcc/profile.hpp>
#include <vanetza/geonet/interface.hpp>

using namespace omnetpp;
using namespace vanetza;

Define_Module(EmergencyVehicleService)

void EmergencyVehicleService::initialize()
{
    ItsG5BaseService::initialize();
    mVehicleController = &getFacilities().get_const<traci::VehicleController>();
}

void EmergencyVehicleService::trigger()
{
    Enter_Method("EmergencyVehicleService trigger");
    btp::DataRequestB req;
    req.destination_port = host_cast<EmergencyVehicleService::port_type>(getPortNumber());
    req.gn.transport_type = geonet::TransportType::SHB;
    req.gn.traffic_class.tc_id(static_cast<unsigned>(dcc::Profile::DP1));
    req.gn.communication_profile = geonet::CommunicationProfile::ITS_G5;

    const std::string id = mVehicleController->getVehicleId();
    auto& vehicle_api = mVehicleController->getTraCI()->vehicle;

    auto packet = new EmergencyClearLane();
    packet->setEdgeName(vehicle_api.getRoadID(id).c_str());
    packet->setLaneIndex(vehicle_api.getLaneIndex(id));
    packet->setByteLength(40);
    request(req, packet);
}
