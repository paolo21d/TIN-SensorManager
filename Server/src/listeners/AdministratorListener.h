#ifndef SERVER_ADMINISTRATORLISTENER_H
#define SERVER_ADMINISTRATORLISTENER_H

#include "IRequestListener.h"
#include <vector>
#include <string>
#include <src/serializers/SerializerAdministratorMessage.h>
#include <src/serverModel/IModelForAdministrator.h>
#include "src/Sensor.h"

class AdministratorListener : public IRequestListener {
    IModelForAdministrator *model;
public:
    AdministratorListener(IModelForAdministrator *model);
    void onGotRequest(int clientId, std::vector<unsigned char> msg) override;

private:
    SerializerAdministratorMessage serializer;
};


#endif //SERVER_ADMINISTRATORLISTENER_H
