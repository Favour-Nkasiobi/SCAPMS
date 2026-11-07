#ifndef EMERGENCYPATIENT_H
#define EMERGENCYPATIENT_H

#include "PatientRecord.h"
#include <iostream>

namespace SCAPMS {

class EmergencyPatient : public PatientRecord {
private:
    int emergencyLevel;

public:
    EmergencyPatient(int id, const std::string& name,
                     int age, const std::string& contact,
                     int level)
        : PatientRecord(id, name, age, contact),
        emergencyLevel(level) {

        if (emergencyLevel < 1 || emergencyLevel > 5) {
            throw std::invalid_argument(
                "Emergency level must be between 1 and 5."
                );
        }
    }

    int getEmergencyLevel() const {
        return emergencyLevel;
    }

    void displayDetails() const override {
        std::cout << "\n--- Emergency Patient ---\n";
        std::cout << "Patient ID: " << patientId << '\n';
        std::cout << "Name: " << name << '\n';
        std::cout << "Age: " << age << '\n';
        std::cout << "Contact: " << contact << '\n';
        std::cout << "Category: Emergency Patient\n";
        std::cout << "Emergency Level: "
                  << emergencyLevel << '\n';
    }
};

}

#endif