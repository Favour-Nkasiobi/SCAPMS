#ifndef WAITINGQUEUE_H
#define WAITINGQUEUE_H

#include "PatientRecord.h"

#include <queue>
#include <memory>
#include <stdexcept>
#include <cstddef>
#include <vector>

namespace SCAPMS {

class WaitingQueue {
private:
    std::queue<std::shared_ptr<PatientRecord>> patients;

public:
    void addPatient(const std::shared_ptr<PatientRecord>& patient) {
        if (!patient) {
            throw std::invalid_argument(
                "Cannot add an invalid patient to the queue."
                );
        }

        if (containsPatient(patient->getPatientId())) {
            throw std::invalid_argument(
                "This patient is already in the waiting queue."
                );
        }

        patients.push(patient);
    }

    bool isEmpty() const {
        return patients.empty();
    }

    std::size_t size() const {
        return patients.size();
    }

    std::vector<std::shared_ptr<PatientRecord>> getPatients() const {
        std::vector<std::shared_ptr<PatientRecord>> result;

        std::queue<std::shared_ptr<PatientRecord>> temp = patients;

        while (!temp.empty()) {
            result.push_back(temp.front());
            temp.pop();
        }

        return result;
    }

    PatientRecord* nextPatient() const {
        if (patients.empty()) {
            return nullptr;
        }

        return patients.front().get();
    }

    bool containsPatient(int patientId) const {
        std::queue<std::shared_ptr<PatientRecord>> temp = patients;

        while (!temp.empty()) {
            if (temp.front()->getPatientId() == patientId) {
                return true;
            }

            temp.pop();
        }

        return false;
    }

    bool removePatient(int patientId) {
        std::queue<std::shared_ptr<PatientRecord>> temp;
        bool removed = false;

        while (!patients.empty()) {
            auto patient = patients.front();
            patients.pop();

            if (patient->getPatientId() == patientId && !removed) {
                removed = true;
            } else {
                temp.push(patient);
            }
        }

        patients.swap(temp);
        return removed;
    }

    bool serveNextPatient() {
        if (patients.empty()) {
            return false;
        }

        patients.pop();
        return true;
    }

    void clear() {
        while (!patients.empty()) {
            patients.pop();
        }
    }
};

}

#endif