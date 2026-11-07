#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <memory>
#include <vector>

#include "PatientRecord.h"
#include "GeneralPatient.h"
#include "EmergencyPatient.h"
#include "FollowUpPatient.h"
#include "Appointable.h"
#include "Appointment.h"
#include "ClinicDatabase.h"
#include "WaitingQueue.h"
#include "UndoManager.h"
#include "PatientCache.h"
#include "SearchSortAlgorithms.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    using namespace SCAPMS;

    try {
        auto patient1 = std::make_shared<GeneralPatient>(
            101, "John Smith", 30, "08012345678"
            );

        auto patient2 = std::make_shared<EmergencyPatient>(
            102, "Mary James", 25, "08087654321", 1
            );

        auto patient3 = std::make_shared<FollowUpPatient>(
            103, "Peter Brown", 40, "08055555555", "2026-10-10"
            );

        std::vector<std::shared_ptr<PatientRecord>> patients;

        patients.push_back(patient1);
        patients.push_back(patient2);
        patients.push_back(patient3);

        qDebug() << "Patient records created:" << patients.size();

        PatientRecord* found =
            SearchSortAlgorithms::linearSearchById(patients, 102);

        if (found != nullptr) {
            qDebug() << "Search successful. Patient:"
                     << QString::fromStdString(found->getName());
        } else {
            qDebug() << "Patient not found.";
        }

        ClinicDatabase database;

        database.addPatient(patient1);
        database.addPatient(patient2);
        database.addPatient(patient3);

        qDebug() << "Database patient count:"
                 << database.getPatients().size();

        WaitingQueue waitingQueue;

        waitingQueue.addPatient(patient1);
        waitingQueue.addPatient(patient2);

        qDebug() << "Patients in waiting queue:"
                 << waitingQueue.size();

        PatientRecord* nextPatient = waitingQueue.nextPatient();

        if (nextPatient != nullptr) {
            qDebug() << "Next patient in queue:"
                     << QString::fromStdString(nextPatient->getName());
        }

        waitingQueue.serveNextPatient();

        qDebug() << "Queue size after serving one patient:"
                 << waitingQueue.size();

        qDebug() << "SCAPMS patient, database, and queue tests completed.";

    } catch (const std::exception& e) {
        qDebug() << "Test error:" << e.what();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
