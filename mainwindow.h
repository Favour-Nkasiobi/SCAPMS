#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ClinicDatabase.h"
#include "PatientCache.h"
#include "UndoManager.h"
#include "WaitingQueue.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_addPatientButton_clicked();
    void on_updatePatientButton_2_clicked();
    void on_deletePatientButton_clicked();
    void on_searchPatientButton_clicked();
    void on_clearPatientFormButton_clicked();
    void on_refreshPatientsButton_clicked();

    void on_appointmentPatientIdLineEdit_textChanged(const QString &text);
    void on_scheduleAppointmentButton_clicked();
    void on_completeAppointmentButton_clicked();
    void on_cancelAppointmentButton_clicked();
    void on_clearAppointmentFormButton_clicked();
    void on_searchAppointmentButton_clicked();
    void on_refreshAppointmentsButton_clicked();

    // Waiting Queue slots
    void on_queuePatientIdLineEdit_textChanged(const QString &text);
    void on_addToQueueButton_clicked();
    void on_serveNextPatientButton_clicked();
    void on_removePatientFromQueueButton_clicked();
    void on_clearQueueButton_clicked();
    void on_refreshQueueButton_clicked();

    // // Search & Sort slots
    // void on_searchButton_clicked();
    // void on_sortButton_clicked();
    // void on_resetSearchSortButton_clicked();

private:
    Ui::MainWindow *ui;

    SCAPMS::ClinicDatabase database;
    SCAPMS::PatientCache patientCache;
    SCAPMS::UndoManager undoManager;
    SCAPMS::WaitingQueue waitingQueue;

    void saveData();
    void loadData();
};
#endif // MAINWINDOW_H
