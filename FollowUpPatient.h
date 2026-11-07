#ifndef FOLLOWUPPATIENT_H
#define FOLLOWUPPATIENT_H

#include "PatientRecord.h"
#include <iostream>

namespace SCAPMS {

class FollowUpPatient : public PatientRecord {
private:
    std::string previousVisitDate;

public:
    FollowUpPatient(int id, const std::string& name,
                    int age, const std::string& contact,
                    const std::string& visitDate)
        : PatientRecord(id, name, age, contact),
        previousVisitDate(visitDate) {

        if (previousVisitDate.empty()) {
            throw std::invalid_argument(
                "Previous visit date cannot be empty."
                );
        }
    }

    std::string getPreviousVisitDate() const {
        return previousVisitDate;
    }

    void displayDetails() const override {
        std::cout << "\n--- Follow-Up Patient ---\n";
        std::cout << "Patient ID: " << patientId << '\n';
        std::cout << "Name: " << name << '\n';
        std::cout << "Age: " << age << '\n';
        std::cout << "Contact: " << contact << '\n';
        std::cout << "Category: Follow-Up Patient\n";
        std::cout << "Previous Visit Date: "
                  << previousVisitDate << '\n';
    }
};

}

#endif