#ifndef GENERALPATIENT_H
#define GENERALPATIENT_H

#include "PatientRecord.h"
#include <iostream>

namespace SCAPMS {

class GeneralPatient : public PatientRecord {
public:
    GeneralPatient(int id, const std::string& name,
                   int age, const std::string& contact)
        : PatientRecord(id, name, age, contact) {
    }

    void displayDetails() const override {
        std::cout << "\n--- General Patient ---\n";
        std::cout << "Patient ID: " << patientId << '\n';
        std::cout << "Name: " << name << '\n';
        std::cout << "Age: " << age << '\n';
        std::cout << "Contact: " << contact << '\n';
        std::cout << "Category: General Patient\n";
    }
};

}

#endif