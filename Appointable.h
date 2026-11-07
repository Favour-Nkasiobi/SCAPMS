#ifndef APPOINTABLE_H
#define APPOINTABLE_H

namespace SCAPMS {

class Appointable {
public:
    virtual bool scheduleAppointment() = 0;
    virtual bool cancelAppointment() = 0;

    virtual ~Appointable() = default;
};

}

#endif