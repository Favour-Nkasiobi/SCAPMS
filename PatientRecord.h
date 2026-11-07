#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

#include <string>
#include <stdexcept>

namespace SCAPMS {

class PatientRecord {
protected:
    int patientId;
    std::string name;
    int age;
    std::string contact;

public:
    PatientRecord(int id, const std::string& patientName,
                  int patientAge, const std::string& patientContact)
        : patientId(id),
        name(patientName),
        age(patientAge),
        contact(patientContact) {

        if (patientId <= 0) {
            throw std::invalid_argument("Patient ID must be positive.");
        }

        if (name.empty()) {
            throw std::invalid_argument("Patient name cannot be empty.");
        }

        if (age <= 0 || age > 120) {
            throw std::invalid_argument("Patient age must be between 1 and 120.");
        }

        if (contact.empty()) {
            throw std::invalid_argument("Patient contact cannot be empty.");
        }
    }

    virtual void displayDetails() const = 0;

    int getPatientId() const {
        return patientId;
    }

    std::string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    std::string getContact() const {
        return contact;
    }

    virtual ~PatientRecord() = default;
};

}

#endif