#include "EmergencyClearLaneService.h"
#include "Alerts_msgs/EmergencyClearLane_m.h"
#include "artery/traci/VehicleController.h"

using namespace omnetpp;
using namespace vanetza;

Define_Module(EmergencyClearLane)

void EmergencyClearLaneService::initialize()
{
    ItsG5BaseService::initialize();
    mVehicleController = &getFacilities().get_mutable<traci::VehicleController>();
}

void EmergencyClearLaneService::indicate(const vanetza::btp::DataIndication& ind, omnetpp::cPacket* packet)
{
    Enter_Method("EmergencyClearLaneService indicate");
    auto EmergencyClearLaneMessage = check_and_cast<const EmergencyClearLane*>(packet);

    const std::string id = mVehicleController->getVehicleId();
    auto& vehicle_api = mVehicleController->getTraCI()->vehicle;
    if (vehicle_api.getRoadID(id) == EmergencyClearLaneMessage->getEdgeName()) {
        if (vehicle_api.getLaneIndex(id) != EmergencyClearLaneMessage->getLaneIndex()) {
            slowDown();
        }
    }

    delete EmergencyClearLaneMessage;
}

void ClearLaneService::slowDown()
{
    mVehicleController->setMaxSpeed(25 * units::si::meter_per_second);
}
