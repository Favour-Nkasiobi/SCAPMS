
#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include "PatientRecord.h"
#include "Appointable.h"

#include <string>
#include <memory>

namespace SCAPMS {

class Appointment : public Appointable {
private:
    int appointmentId;
    std::shared_ptr<PatientRecord> patient;
    std::string appointmentDate;
    std::string appointmentTime;
    std::string reason;
    bool scheduled;
    bool completed;
    bool cancelled;

public:
    Appointment(int id,
                std::shared_ptr<PatientRecord> patientRecord,
                const std::string& date,
                const std::string& time,
                const std::string& appointmentReason)
        : appointmentId(id),
        patient(patientRecord),
        appointmentDate(date),
        appointmentTime(time),
        reason(appointmentReason),
        scheduled(false),
        completed(false),
        cancelled(false) {
    }

    bool scheduleAppointment() override {
        if (!patient ||
            appointmentId <= 0 ||
            appointmentDate.empty() ||
            appointmentTime.empty() ||
            reason.empty() ||
            scheduled) {
            return false;
        }

        scheduled = true;
        cancelled = false;
        completed = false;
        return true;
    }

    bool cancelAppointment() override {
        if (!scheduled || completed) {
            return false;
        }

        scheduled = false;
        cancelled = true;
        return true;
    }

    bool markAsCompleted() {
        if (!scheduled || cancelled) {
            return false;
        }

        scheduled = false;
        completed = true;
        return true;
    }

    int getAppointmentId() const {
        return appointmentId;
    }

    std::shared_ptr<PatientRecord> getPatient() const {
        return patient;
    }

    std::string getAppointmentDate() const {
        return appointmentDate;
    }

    std::string getAppointmentTime() const {
        return appointmentTime;
    }

    std::string getReason() const {
        return reason;
    }

    bool isScheduled() const {
        return scheduled;
    }

    bool isCompleted() const {
        return completed;
    }

    bool isCancelled() const {
        return cancelled;
    }

    std::string getStatus() const {
        if (completed) {
            return "Completed";
        }

        if (cancelled) {
            return "Cancelled";
        }

        if (scheduled) {
            return "Pending";
        }

        return "Not Scheduled";
    }
};

}

#endif
