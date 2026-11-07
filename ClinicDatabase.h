#ifndef CLINICDATABASE_H
#define CLINICDATABASE_H

#include "PatientRecord.h"
#include "Appointment.h"

#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

namespace SCAPMS {

class ClinicDatabase {
private:
    std::vector<std::shared_ptr<PatientRecord>> patients;
    std::vector<std::shared_ptr<Appointment>> appointments;

public:
    void addPatient(std::shared_ptr<PatientRecord> patient) {
        if (!patient) {
            throw std::invalid_argument("Patient cannot be null.");
        }

        for (const auto& existing : patients) {
            if (existing->getPatientId() == patient->getPatientId()) {
                throw std::invalid_argument("Duplicate patient ID.");
            }
        }

        patients.push_back(patient);
    }

    bool deletePatient(int patientId) {
        for (auto it = patients.begin(); it != patients.end(); ++it) {
            if ((*it)->getPatientId() == patientId) {
                patients.erase(it);
                return true;
            }
        }

        return false;
    }

    PatientRecord* findPatientById(int patientId) const {
        for (const auto& patient : patients) {
            if (patient->getPatientId() == patientId) {
                return patient.get();
            }
        }

        return nullptr;
    }


    std::shared_ptr<PatientRecord> getSharedPatientById(int patientId) const {
        for (const auto& patient : patients) {
            if (patient->getPatientId() == patientId) {
                return patient;
            }
        }

        return nullptr;
    }


    const std::vector<std::shared_ptr<PatientRecord>>&
    getPatients() const {
        return patients;
    }

    void addAppointment(std::shared_ptr<Appointment> appointment) {
        if (!appointment) {
            throw std::invalid_argument("Appointment cannot be null.");
        }

        for (const auto& existing : appointments) {
            if (existing->getAppointmentId() ==
                appointment->getAppointmentId()) {
                throw std::invalid_argument("Duplicate appointment ID.");
            }
        }

        appointments.push_back(appointment);
    }

    const std::vector<std::shared_ptr<Appointment>>&
    getAppointments() const {
        return appointments;
    }
};

}

#endif