#ifndef SEARCHSORTALGORITHMS_H
#define SEARCHSORTALGORITHMS_H

#include "PatientRecord.h"

#include <vector>
#include <memory>
#include <string>
#include <utility>

namespace SCAPMS {

class SearchSortAlgorithms {
public:

    // 1. Linear search by patient ID
    static PatientRecord* linearSearchById(
        const std::vector<std::shared_ptr<PatientRecord>>& patients,
        int targetId
        ) {
        for (const auto& patient : patients) {
            if (patient->getPatientId() == targetId) {
                return patient.get();
            }
        }

        return nullptr;
    }

    // 2. Binary search by patient ID.
    // The vector must already be sorted by ID in ascending order.
    static PatientRecord* binarySearchById(
        const std::vector<std::shared_ptr<PatientRecord>>& patients,
        int targetId
        ) {
        int left = 0;
        int right = static_cast<int>(patients.size()) - 1;

        while (left <= right) {
            int middle = left + (right - left) / 2;
            int currentId = patients[middle]->getPatientId();

            if (currentId == targetId) {
                return patients[middle].get();
            }

            if (currentId < targetId) {
                left = middle + 1;
            } else {
                right = middle - 1;
            }
        }

        return nullptr;
    }

    // 3. Recursive search by patient name.
    static PatientRecord* recursiveSearchByName(
        const std::vector<std::shared_ptr<PatientRecord>>& patients,
        const std::string& targetName,
        std::size_t index = 0
        ) {
        if (index >= patients.size()) {
            return nullptr;
        }

        if (patients[index]->getName() == targetName) {
            return patients[index].get();
        }

        return recursiveSearchByName(
            patients, targetName, index + 1
            );
    }

    // 4. Manual insertion sort by patient name.
    static void insertionSortByName(
        std::vector<std::shared_ptr<PatientRecord>>& patients
        ) {
        for (std::size_t i = 1; i < patients.size(); ++i) {
            auto key = patients[i];
            std::size_t j = i;

            while (j > 0 &&
                   patients[j - 1]->getName() > key->getName()) {
                patients[j] = patients[j - 1];
                --j;
            }

            patients[j] = key;
        }
    }

    // 5. Manual selection sort by patient age.
    static void selectionSortByAge(
        std::vector<std::shared_ptr<PatientRecord>>& patients
        ) {
        for (std::size_t i = 0; i < patients.size(); ++i) {
            std::size_t minimumIndex = i;

            for (std::size_t j = i + 1; j < patients.size(); ++j) {
                if (patients[j]->getAge() <
                    patients[minimumIndex]->getAge()) {
                    minimumIndex = j;
                }
            }

            if (minimumIndex != i) {
                std::swap(patients[i], patients[minimumIndex]);
            }
        }
    }
};

}

#endif