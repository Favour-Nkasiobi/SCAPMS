#ifndef PATIENTCACHE_H
#define PATIENTCACHE_H

#include "PatientRecord.h"

#include <array>
#include <memory>
#include <cstddef>

namespace SCAPMS {

class PatientCache {
private:
    static constexpr std::size_t CAPACITY = 5;

    std::array<std::shared_ptr<PatientRecord>, CAPACITY> cache;
    std::size_t nextIndex = 0;
    std::size_t currentSize = 0;

public:
    void addPatient(const std::shared_ptr<PatientRecord>& patient) {
        if (!patient) {
            return;
        }

        cache[nextIndex] = patient;
        nextIndex = (nextIndex + 1) % CAPACITY;

        if (currentSize < CAPACITY) {
            ++currentSize;
        }
    }

    std::size_t size() const {
        return currentSize;
    }

    std::size_t capacity() const {
        return CAPACITY;
    }

    PatientRecord* getMostRecentPatient() const {
        if (currentSize == 0) {
            return nullptr;
        }

        std::size_t index =
            (nextIndex + CAPACITY - 1) % CAPACITY;

        return cache[index].get();
    }

    void clear() {
        for (auto& patient : cache) {
            patient.reset();
        }

        nextIndex = 0;
        currentSize = 0;
    }
};

}

#endif