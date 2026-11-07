#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "GeneralPatient.h"
#include "EmergencyPatient.h"
#include "FollowUpPatient.h"
#include "Appointment.h"
#include "SearchSortAlgorithms.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDate>
#include <memory>

#include <QTime>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidget>

#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <algorithm>
#include <vector>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSaveFile>
#include <QStandardPaths>
#include <QDir>
#include <QFile>

#include "PatientRecord.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
 {
    ui->setupUi(this);

     // Enable vertical and horizontal scrolling for all tables
     QList<QTableWidget*> tables = findChildren<QTableWidget*>();

     for (QTableWidget *table : tables)
     {
         table->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
         table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

         table->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
         table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

         table->setMinimumHeight(250);
         table->setMaximumHeight(350);
     }

    ui->waitingQueueTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // Professional styling for dropdowns and notification messages
    qApp->setStyleSheet(R"(
    QComboBox {
        background-color: white;
        color: #183153;
        border: 1px solid #C9D7E8;
        border-radius: 6px;
        padding: 6px;
    }

    QComboBox QAbstractItemView {
        background-color: white;
        color: #183153;
        selection-background-color: #D9F2EF;
        selection-color: #087F75;
        border: 1px solid #C9D7E8;
        outline: none;
    }

    QMessageBox {
        background-color: white;
    }

    QMessageBox QLabel {
        background-color: white;
        color: #183153;
        font-size: 11pt;
    }

    QMessageBox QPushButton {
        background-color: #0D9488;
        color: white;
        border: none;
        border-radius: 6px;
        padding: 8px 22px;
        min-width: 65px;
    }

    QMessageBox QPushButton:hover {
        background-color: #087F75;
    }
)");


     // Set Appointments table column widths
     ui->appointmentsTableWidget->setColumnWidth(0, 120); // Appointment ID
     ui->appointmentsTableWidget->setColumnWidth(1, 120); // Patient ID
     ui->appointmentsTableWidget->setColumnWidth(2, 150); // Patient Name
     ui->appointmentsTableWidget->setColumnWidth(3, 130); // Date
     ui->appointmentsTableWidget->setColumnWidth(4, 110); // Time
     ui->appointmentsTableWidget->setColumnWidth(5, 180); // Reason
     ui->appointmentsTableWidget->setColumnWidth(6, 120); // Status

     // Modern Light Theme
     this->setStyleSheet(R"(
    QMainWindow {
        background-color: #F4F7FB;
    }

    QWidget {
        color: #243247;
        font-family: "Segoe UI";
        font-size: 10pt;
    }

    QTabWidget::pane {
        background-color: #FFFFFF;
        border: 1px solid #DCE5EF;
        border-radius: 10px;
        top: -1px;
    }

    QTabBar::tab {
        background-color: #EAF0F6;
        color: #52647A;
        padding: 12px 24px;
        margin-right: 4px;
        border-top-left-radius: 8px;
        border-top-right-radius: 8px;
    }

    QTabBar::tab:selected {
        background-color: #FFFFFF;
        color: #0D9488;
        border-bottom: 3px solid #0D9488;
        font-weight: bold;
    }

    QPushButton {
        background-color: #0D9488;
        color: #FFFFFF;
        border: none;
        border-radius: 7px;
        padding: 9px 16px;
        font-weight: bold;
    }

    QPushButton:hover {
        background-color: #0F766E;
    }

    QPushButton:pressed {
        background-color: #115E59;
    }

    QLineEdit, QSpinBox, QDateEdit, QTimeEdit, QComboBox, QTextEdit {
        background-color: #FFFFFF;
        border: 1px solid #CBD5E1;
        border-radius: 6px;
        padding: 7px;
        selection-background-color: #0D9488;
    }

    QLineEdit:focus, QSpinBox:focus, QDateEdit:focus,
    QTimeEdit:focus, QComboBox:focus, QTextEdit:focus {
        border: 2px solid #0D9488;
    }

    QTableWidget {
        background-color: #FFFFFF;
        alternate-background-color: #F1F5F9;
        gridline-color: #E2E8F0;
        border: 1px solid #DCE5EF;
        border-radius: 7px;
        selection-background-color: #CCFBF1;
        selection-color: #134E4A;
    }

    QHeaderView::section {
        background-color: #EAF0F6;
        color: #243247;
        padding: 9px;
        border: none;
        border-bottom: 2px solid #CBD5E1;
        font-weight: bold;
    }

    QTableWidget::item {
        padding: 6px;
    }
)");

    setWindowTitle("SCAPMS | Clinic Management System");


    // Modern Light Theme for SCAPMS
    this->setStyleSheet(R"(
        /* Main application */
        QMainWindow {
            background-color: #F3F6FA;
        }

        QWidget {
            color: #243447;
            font-family: "Segoe UI";
            font-size: 10pt;
        }

        /* Tabs */
        QTabWidget::pane {
            background-color: #FFFFFF;
            border: 1px solid #DCE4EC;
            border-radius: 8px;
            top: -1px;
        }

        QTabBar::tab {
            background-color: #E8EEF5;
            color: #526477;
            padding: 12px 22px;
            margin-right: 4px;
            border-top-left-radius: 7px;
            border-top-right-radius: 7px;
            font-weight: 600;
        }

        QTabBar::tab:selected {
            background-color: #FFFFFF;
            color: #0D9488;
            border-bottom: 3px solid #0D9488;
        }

        QTabBar::tab:hover:!selected {
            background-color: #DCE8F0;
        }

        /* Buttons */
        QPushButton {
            background-color: #0D9488;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 9px 17px;
            font-weight: 600;
        }

        QPushButton:hover {
            background-color: #0F766E;
        }

        QPushButton:pressed {
            background-color: #115E59;
        }

        QPushButton:disabled {
            background-color: #CBD5E1;
            color: #64748B;
        }

        /* Input fields */
        QLineEdit, QTextEdit, QPlainTextEdit,
        QSpinBox, QDoubleSpinBox,
        QDateEdit, QTimeEdit, QDateTimeEdit,
        QComboBox {
            background-color: #FFFFFF;
            color: #243447;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            padding: 8px;
            selection-background-color: #0D9488;
            selection-color: #FFFFFF;
        }

        QLineEdit:focus, QTextEdit:focus,
        QComboBox:focus, QSpinBox:focus,
        QDateEdit:focus, QTimeEdit:focus {
            border: 2px solid #0D9488;
        }

        /* Tables */
        QTableWidget, QTableView {
            background-color: #FFFFFF;
            alternate-background-color: #F1F5F9;
            color: #243447;
            gridline-color: #E2E8F0;
            border: 1px solid #DCE4EC;
            border-radius: 7px;
            selection-background-color: #CCFBF1;
            selection-color: #134E4A;
        }

        QHeaderView::section {
            background-color: #E8EEF5;
            color: #243447;
            padding: 10px;
            border: none;
            border-bottom: 2px solid #CBD5E1;
            font-weight: 700;
        }

        /* Labels and group boxes */
        QLabel {
            color: #334155;
            background: transparent;
        }

        QGroupBox {
            background-color: #FFFFFF;
            border: 1px solid #DCE4EC;
            border-radius: 8px;
            margin-top: 15px;
            padding: 12px;
            font-weight: 700;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 5px;
            color: #0F766E;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: #F1F5F9;
            width: 10px;
            margin: 0;
        }

        QScrollBar::handle:vertical {
            background: #94A3B8;
            border-radius: 5px;
            min-height: 25px;
        }

        QScrollBar::handle:vertical:hover {
            background: #64748B;
        }

        QScrollBar:horizontal {
            background: #F1F5F9;
            height: 10px;
            margin: 0;
        }

        QScrollBar::handle:horizontal {
            background: #94A3B8;
            border-radius: 5px;
            min-width: 25px;
        }
    )");


    // Initialise Patient Category dropdown
    ui->patientCategoryComboBox->addItem("General Patient");
    ui->patientCategoryComboBox->addItem("Emergency Patient");
    ui->patientCategoryComboBox->addItem("Follow-Up Patient");

    // Initialise Emergency Level dropdown
    ui->emergencyLevelComboBox->addItem("Level 1");
    ui->emergencyLevelComboBox->addItem("Level 2");
    ui->emergencyLevelComboBox->addItem("Level 3");
    ui->emergencyLevelComboBox->addItem("Level 4");
    ui->emergencyLevelComboBox->addItem("Level 5");

    // Set the default Previous Visit Date
    ui->previousVisitDateEdit->setDate(QDate::currentDate());

    // Configure Age input
    ui->patientAgeSpinBox->setRange(1, 120);

    // Configure Patients table
    ui->patientsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->patientsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->patientsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->patientsTableWidget->setAlternatingRowColors(true);

    // Configure Appointment form
    ui->appointmentPatientNameLineEdit->setReadOnly(true);

    ui->appointmentDateEdit->setDate(QDate::currentDate());
    ui->appointmentDateEdit->setMinimumDate(QDate::currentDate());

    ui->appointmentTimeEdit->setTime(QTime::currentTime());

    // Configure Appointment Status dropdown
    ui->appointmentStatusComboBox->clear();
    ui->appointmentStatusComboBox->addItem("All Statuses");
    ui->appointmentStatusComboBox->addItem("Pending");
    ui->appointmentStatusComboBox->addItem("Completed");
    ui->appointmentStatusComboBox->addItem("Cancelled");

    // Automatically filter appointments when status changes
    connect(ui->appointmentStatusComboBox,
            &QComboBox::currentTextChanged,
            this,
            [this](const QString &) {
                on_searchAppointmentButton_clicked();
            });

    // Configure Appointments table
    ui->appointmentsTableWidget->setColumnCount(7);

    ui->appointmentsTableWidget->setHorizontalHeaderLabels({
        "Appointment ID",
        "Patient ID",
        "Patient Name",
        "Date",
        "Time",
        "Reason",
        "Status"
    });

    ui->appointmentsTableWidget->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui->appointmentsTableWidget->setSelectionMode(
        QAbstractItemView::SingleSelection);

    ui->appointmentsTableWidget->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    ui->appointmentsTableWidget->setAlternatingRowColors(true);

    // Configure Waiting Queue table
    ui->waitingQueueTableWidget->setColumnCount(4);

    ui->waitingQueueTableWidget->setHorizontalHeaderLabels({
        "Patient ID",
        "Patient Name",
        "Patient Category",
        "Queue Position"
    });

    ui->waitingQueueTableWidget->setSelectionBehavior(
        QAbstractItemView::SelectRows
        );

    ui->waitingQueueTableWidget->setSelectionMode(
        QAbstractItemView::SingleSelection
        );

    ui->waitingQueueTableWidget->setEditTriggers(
        QAbstractItemView::NoEditTriggers
        );

    ui->waitingQueueTableWidget->setAlternatingRowColors(true);

    // Configure patient category dropdown
    ui->queuePatientCategoryComboBox->clear();
    ui->queuePatientCategoryComboBox->addItems({
        "General",
        "Emergency",
        "Follow-Up"
    });

    // Patient name is filled automatically
    ui->queuePatientNameLineEdit->setReadOnly(true);

    // Initially update queue information
    on_refreshQueueButton_clicked();

    // Configure Search & Sort controls and result tables
    ui->searchMethodComboBox->clear();
    ui->searchMethodComboBox->addItems({
        "Linear Search", "Binary Search", "Recursive Search"
    });

    ui->searchByComboBox->clear();
    ui->searchByComboBox->addItems({"Patient ID", "Full Name"});

    ui->searchResultsTableWidget->setColumnCount(4);
    ui->searchResultsTableWidget->setHorizontalHeaderLabels({
        "Patient ID", "Full Name", "Age", "Category"
    });
    ui->searchResultsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->searchResultsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->searchResultsTableWidget->setAlternatingRowColors(true);

    ui->sortingAlgorithmComboBox->clear();
    ui->sortingAlgorithmComboBox->addItems({"Insertion Sort", "Selection Sort"});

    ui->sortByComboBox->clear();
    ui->sortByComboBox->addItems({"Name", "Age"});

    ui->sortingResultsTableWidget->setColumnCount(4);
    ui->sortingResultsTableWidget->setHorizontalHeaderLabels({
        "Patient ID", "Full Name", "Age", "Category"
    });
    ui->sortingResultsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sortingResultsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->sortingResultsTableWidget->setAlternatingRowColors(true);

    // Search patients using the selected manual search algorithm
    connect(ui->searchButton, &QPushButton::clicked, this, [this]() {
        QString value = ui->searchValueLineEdit->text().trimmed();
        if (value.isEmpty()) {
            QMessageBox::warning(this, "Search", "Please enter a search value.");
            return;
        }

        const auto& storedPatients = database.getPatients();
        SCAPMS::PatientRecord* foundPatient = nullptr;
        QString method = ui->searchMethodComboBox->currentText();
        QString searchBy = ui->searchByComboBox->currentText();

        if (searchBy == "Patient ID") {
            bool validId = false;
            int targetId = value.toInt(&validId);
            if (!validId) {
                QMessageBox::warning(this, "Search", "Patient ID must be a valid number.");
                return;
            }

            if (method == "Linear Search") {
                foundPatient = SCAPMS::SearchSortAlgorithms::linearSearchById(
                    storedPatients, targetId);
            } else if (method == "Binary Search") {
                auto sortedPatients = storedPatients;
                // Binary search requires ascending patient ID order.
                for (std::size_t i = 1; i < sortedPatients.size(); ++i) {
                    auto key = sortedPatients[i];
                    std::size_t j = i;
                    while (j > 0 && sortedPatients[j - 1]->getPatientId() > key->getPatientId()) {
                        sortedPatients[j] = sortedPatients[j - 1];
                        --j;
                    }
                    sortedPatients[j] = key;
                }
                foundPatient = SCAPMS::SearchSortAlgorithms::binarySearchById(
                    sortedPatients, targetId);
            } else {
                QMessageBox::warning(this, "Search Method",
                                     "Recursive Search is available for Full Name. Select Full Name under Search By.");
                return;
            }
        } else {
            if (method == "Recursive Search") {
                foundPatient = SCAPMS::SearchSortAlgorithms::recursiveSearchByName(
                    storedPatients, value.toStdString());
            } else if (method == "Linear Search") {
                for (const auto& patient : storedPatients) {
                    if (QString::fromStdString(patient->getName()).compare(
                            value, Qt::CaseInsensitive) == 0) {
                        foundPatient = patient.get();
                        break;
                    }
                }
            } else {
                QMessageBox::warning(this, "Search Method",
                                     "Binary Search is available for Patient ID. Select Patient ID under Search By.");
                return;
            }
        }

        ui->searchResultsTableWidget->setRowCount(0);
        if (!foundPatient) {
            QMessageBox::information(this, "Search", "No matching patient was found.");
            return;
        }

        QString category = "Unknown";
        if (dynamic_cast<SCAPMS::GeneralPatient*>(foundPatient)) category = "General Patient";
        else if (dynamic_cast<SCAPMS::EmergencyPatient*>(foundPatient)) category = "Emergency Patient";
        else if (dynamic_cast<SCAPMS::FollowUpPatient*>(foundPatient)) category = "Follow-Up Patient";

        int row = ui->searchResultsTableWidget->rowCount();
        ui->searchResultsTableWidget->insertRow(row);
        ui->searchResultsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(foundPatient->getPatientId())));
        ui->searchResultsTableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(foundPatient->getName())));
        ui->searchResultsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(foundPatient->getAge())));
        ui->searchResultsTableWidget->setItem(row, 3, new QTableWidgetItem(category));
    });

    // Sort a copy of the patient list; the database's original order is preserved.
    connect(ui->sortButton, &QPushButton::clicked, this, [this]() {
        auto sortedPatients = database.getPatients();
        if (sortedPatients.empty()) {
            QMessageBox::information(this, "Sort", "There are no patient records to sort.");
            return;
        }

        QString algorithm = ui->sortingAlgorithmComboBox->currentText();
        QString sortBy = ui->sortByComboBox->currentText();

        if (sortBy == "Name") {
            if (algorithm == "Insertion Sort") {
                SCAPMS::SearchSortAlgorithms::insertionSortByName(sortedPatients);
            } else {
                // Manual selection sort by name.
                for (std::size_t i = 0; i < sortedPatients.size(); ++i) {
                    std::size_t minimum = i;
                    for (std::size_t j = i + 1; j < sortedPatients.size(); ++j) {
                        if (sortedPatients[j]->getName() < sortedPatients[minimum]->getName()) minimum = j;
                    }
                    if (minimum != i) std::swap(sortedPatients[i], sortedPatients[minimum]);
                }
            }
        } else {
            if (algorithm == "Selection Sort") {
                SCAPMS::SearchSortAlgorithms::selectionSortByAge(sortedPatients);
            } else {
                // Manual insertion sort by age.
                for (std::size_t i = 1; i < sortedPatients.size(); ++i) {
                    auto key = sortedPatients[i];
                    std::size_t j = i;
                    while (j > 0 && sortedPatients[j - 1]->getAge() > key->getAge()) {
                        sortedPatients[j] = sortedPatients[j - 1];
                        --j;
                    }
                    sortedPatients[j] = key;
                }
            }
        }

        ui->sortingResultsTableWidget->setRowCount(0);
        for (const auto& patient : sortedPatients) {
            QString category = "Unknown";
            if (dynamic_cast<SCAPMS::GeneralPatient*>(patient.get())) category = "General Patient";
            else if (dynamic_cast<SCAPMS::EmergencyPatient*>(patient.get())) category = "Emergency Patient";
            else if (dynamic_cast<SCAPMS::FollowUpPatient*>(patient.get())) category = "Follow-Up Patient";

            int row = ui->sortingResultsTableWidget->rowCount();
            ui->sortingResultsTableWidget->insertRow(row);
            ui->sortingResultsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(patient->getPatientId())));
            ui->sortingResultsTableWidget->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(patient->getName())));
            ui->sortingResultsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(patient->getAge())));
            ui->sortingResultsTableWidget->setItem(row, 3, new QTableWidgetItem(category));
        }
    });

    // Reset Search & Sort fields and results
    connect(ui->resetSearchSortButton, &QPushButton::clicked, this, [this]() {
        ui->searchValueLineEdit->clear();
        ui->searchMethodComboBox->setCurrentIndex(0);
        ui->searchByComboBox->setCurrentIndex(0);
        ui->sortingAlgorithmComboBox->setCurrentIndex(0);
        ui->sortByComboBox->setCurrentIndex(0);
        ui->searchResultsTableWidget->setRowCount(0);
        ui->sortingResultsTableWidget->setRowCount(0);
    });

    // ==========================================
    // REPORTS DASHBOARD
    // ==========================================
    auto updateReportsDashboard = [this]() {
        int generalCount = 0;
        int emergencyCount = 0;
        int followUpCount = 0;

        for (const auto& patient : database.getPatients()) {
            if (dynamic_cast<SCAPMS::GeneralPatient*>(patient.get()))
                ++generalCount;
            else if (dynamic_cast<SCAPMS::EmergencyPatient*>(patient.get()))
                ++emergencyCount;
            else if (dynamic_cast<SCAPMS::FollowUpPatient*>(patient.get()))
                ++followUpCount;
        }

        int pendingCount = 0;
        int completedCount = 0;
        for (const auto& appointment : database.getAppointments()) {
            const QString status = QString::fromStdString(appointment->getStatus());
            if (status == "Pending") ++pendingCount;
            else if (status == "Completed") ++completedCount;
        }

        ui->totalPatientsValueLabel->setText(QString::number(database.getPatients().size()));
        ui->totalAppointmentsValueLabel->setText(QString::number(database.getAppointments().size()));
        ui->pendingAppointmentsValueLabel->setText(QString::number(pendingCount));
        ui->completedAppointmentsValueLabel->setText(QString::number(completedCount));
        ui->waitingPatientsValueLabel->setText(QString::number(waitingQueue.size()));
        ui->generalPatientsValueLabel->setText(QString::number(generalCount));
        ui->emergencyPatientsValueLabel->setText(QString::number(emergencyCount));
        ui->followUpPatientsValueLabel->setText(QString::number(followUpCount));
    };

    auto prepareReportTable = [this]() {
        ui->reportResultsTableWidget->clear();

        // Use exactly four columns
        ui->reportResultsTableWidget->setColumnCount(4);
        ui->reportResultsTableWidget->setRowCount(0);

        ui->reportResultsTableWidget->setHorizontalHeaderLabels(
            {"ID", "Patient Name", "Category / Date", "Details"});

        // Table appearance
        ui->reportResultsTableWidget->setSelectionBehavior(
            QAbstractItemView::SelectRows);

        ui->reportResultsTableWidget->setEditTriggers(
            QAbstractItemView::NoEditTriggers);

        ui->reportResultsTableWidget->setAlternatingRowColors(true);

        // Make all four column headings visible
        ui->reportResultsTableWidget->horizontalHeader()->setSectionResizeMode(
            QHeaderView::Stretch);

        // Allow vertical and horizontal scrolling
        ui->reportResultsTableWidget->setVerticalScrollBarPolicy(
            Qt::ScrollBarAsNeeded);

        ui->reportResultsTableWidget->setHorizontalScrollBarPolicy(
            Qt::ScrollBarAsNeeded);
    };

    updateReportsDashboard();

    // Attendance report: completed appointments are treated as attended.
    connect(ui->generateAttendanceReportButton, &QPushButton::clicked, this,
            [this, prepareReportTable, updateReportsDashboard]() {
                prepareReportTable();
                ui->reportResultsTableWidget->setHorizontalHeaderLabels(
                    {"Appointment ID", "Patient Name", "Appointment Date", "Attendance"});
                int count = 0;
                for (const auto& appointment : database.getAppointments()) {
                    if (appointment->getStatus() != "Completed") continue;
                    auto patient = appointment->getPatient();
                    int row = ui->reportResultsTableWidget->rowCount();
                    ui->reportResultsTableWidget->insertRow(row);
                    ui->reportResultsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(appointment->getAppointmentId())));
                    ui->reportResultsTableWidget->setItem(row, 1, new QTableWidgetItem(patient ? QString::fromStdString(patient->getName()) : "Unknown"));
                    ui->reportResultsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(appointment->getAppointmentDate())));
                    ui->reportResultsTableWidget->setItem(row, 3, new QTableWidgetItem("Attended (Completed)"));
                    ++count;
                }
                updateReportsDashboard();
                QMessageBox::information(this, "Attendance Report", QString("Report generated. Completed appointments: %1").arg(count));
            });

    // Pending appointments report.
    connect(ui->generatePendingReportButton, &QPushButton::clicked, this,
            [this, prepareReportTable, updateReportsDashboard]() {
                prepareReportTable();
                ui->reportResultsTableWidget->setHorizontalHeaderLabels(
                    {"Appointment ID", "Patient Name", "Appointment Date", "Reason"});
                int count = 0;
                for (const auto& appointment : database.getAppointments()) {
                    if (appointment->getStatus() != "Pending") continue;
                    auto patient = appointment->getPatient();
                    int row = ui->reportResultsTableWidget->rowCount();
                    ui->reportResultsTableWidget->insertRow(row);
                    ui->reportResultsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(appointment->getAppointmentId())));
                    ui->reportResultsTableWidget->setItem(row, 1, new QTableWidgetItem(patient ? QString::fromStdString(patient->getName()) : "Unknown"));
                    ui->reportResultsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(appointment->getAppointmentDate())));
                    ui->reportResultsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(appointment->getReason())));
                    ++count;
                }
                updateReportsDashboard();
                QMessageBox::information(this, "Pending Appointments", QString("Pending appointments found: %1").arg(count));
            });

    // Patient category distribution report.
    connect(ui->generateCategoryReportButton, &QPushButton::clicked, this,
            [this, prepareReportTable, updateReportsDashboard]() {
                prepareReportTable();
                ui->reportResultsTableWidget->setHorizontalHeaderLabels(
                    {"Category", "Patient Count", "Percentage", "Details"});
                int total = static_cast<int>(database.getPatients().size());
                int general = 0, emergency = 0, followUp = 0;
                for (const auto& patient : database.getPatients()) {
                    if (dynamic_cast<SCAPMS::GeneralPatient*>(patient.get())) ++general;
                    else if (dynamic_cast<SCAPMS::EmergencyPatient*>(patient.get())) ++emergency;
                    else if (dynamic_cast<SCAPMS::FollowUpPatient*>(patient.get())) ++followUp;
                }
                const QStringList names = {"General", "Emergency", "Follow-Up"};
                const int counts[] = {general, emergency, followUp};
                for (int i = 0; i < 3; ++i) {
                    int row = ui->reportResultsTableWidget->rowCount();
                    ui->reportResultsTableWidget->insertRow(row);
                    double percentage = total == 0 ? 0.0 : (counts[i] * 100.0 / total);
                    ui->reportResultsTableWidget->setItem(row, 0, new QTableWidgetItem(names[i]));
                    ui->reportResultsTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(counts[i])));
                    ui->reportResultsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(percentage, 'f', 1) + "%"));
                    ui->reportResultsTableWidget->setItem(row, 3, new QTableWidgetItem("Registered patients"));
                }
                updateReportsDashboard();
            });

    // Daily appointment summary grouped by appointment date.
    connect(ui->generateDailySummaryButton, &QPushButton::clicked, this,
            [this, prepareReportTable, updateReportsDashboard]() {
                prepareReportTable();
                ui->reportResultsTableWidget->setHorizontalHeaderLabels(
                    {"Date", "Total Appointments", "Pending", "Completed"});
                QMap<QString, QPair<int, QPair<int, int>>> daily;
                for (const auto& appointment : database.getAppointments()) {
                    QString date = QString::fromStdString(appointment->getAppointmentDate());
                    auto& counts = daily[date];
                    ++counts.first;
                    if (appointment->getStatus() == "Pending") ++counts.second.first;
                    else if (appointment->getStatus() == "Completed") ++counts.second.second;
                }
                for (auto it = daily.cbegin(); it != daily.cend(); ++it) {
                    int row = ui->reportResultsTableWidget->rowCount();
                    ui->reportResultsTableWidget->insertRow(row);
                    ui->reportResultsTableWidget->setItem(row, 0, new QTableWidgetItem(it.key()));
                    ui->reportResultsTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(it.value().first)));
                    ui->reportResultsTableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(it.value().second.first)));
                    ui->reportResultsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(it.value().second.second)));
                }
                updateReportsDashboard();
            });

    // Export the currently displayed report as CSV.
    connect(ui->exportReportButton, &QPushButton::clicked, this, [this]() {
        if (ui->reportResultsTableWidget->rowCount() == 0) {
            QMessageBox::warning(this, "Export Report", "Generate a report before exporting it.");
            return;
        }
        QString fileName = QFileDialog::getSaveFileName(
            this, "Export Report", "Clinic_Report.csv", "CSV Files (*.csv)");
        if (fileName.isEmpty()) return;
        if (!fileName.endsWith(".csv", Qt::CaseInsensitive)) fileName += ".csv";
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Export Report", "Could not create the report file.");
            return;
        }
        QTextStream out(&file);
        for (int column = 0; column < ui->reportResultsTableWidget->columnCount(); ++column) {
            if (column) out << ',';
            QTableWidgetItem* header = ui->reportResultsTableWidget->horizontalHeaderItem(column);
            QString value = header ? header->text() : QString();
            value.replace('"', """");
            out << '"' << value << '"';
        }
        out << '\n';
        for (int row = 0; row < ui->reportResultsTableWidget->rowCount(); ++row) {
            for (int column = 0; column < ui->reportResultsTableWidget->columnCount(); ++column) {
                if (column) out << ',';
                QTableWidgetItem* item = ui->reportResultsTableWidget->item(row, column);
                QString value = item ? item->text() : QString();
                value.replace('"', """");
                out << '"' << value << '"';
            }
            out << '\n';
        }
        file.close();
        QMessageBox::information(this, "Export Report", "Report exported successfully.");
    });

    // Load previously saved records
    loadData();

    // Refresh the application tables
    on_refreshPatientsButton_clicked();
    on_refreshAppointmentsButton_clicked();
    on_refreshQueueButton_clicked();
}


void MainWindow::on_addPatientButton_clicked()
{
    try {
        // Get information from the form
        bool idValid = false;
        int patientId = ui->patientIdLineEdit->text().toInt(&idValid);

        QString name = ui->patientNameLineEdit->text().trimmed();
        int age = ui->patientAgeSpinBox->value();
        QString contact = ui->patientContactLineEdit->text().trimmed();
        QString category = ui->patientCategoryComboBox->currentText();

        // Validate required fields
        if (!idValid || name.isEmpty() || contact.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input",
                                 "Please enter a valid Patient ID, Name and Contact.");
            return;
        }

        // Create the appropriate patient
        std::shared_ptr<SCAPMS::PatientRecord> patient;

        if (category == "General Patient") {
            patient = std::make_shared<SCAPMS::GeneralPatient>(
                patientId, name.toStdString(), age, contact.toStdString());
        }
        else if (category == "Emergency Patient") {
            int level = ui->emergencyLevelComboBox->currentText()
            .remove("Level ").toInt();

            patient = std::make_shared<SCAPMS::EmergencyPatient>(
                patientId, name.toStdString(), age,
                contact.toStdString(), level);
        }
        else if (category == "Follow-Up Patient") {
            QString previousDate =
                ui->previousVisitDateEdit->date().toString("yyyy-MM-dd");

            patient = std::make_shared<SCAPMS::FollowUpPatient>(
                patientId, name.toStdString(), age,
                contact.toStdString(), previousDate.toStdString());
        }
        else {
            QMessageBox::warning(this, "Category Required",
                                 "Please select a patient category.");
            return;
        }

        // Save patient in the database and cache
        database.addPatient(patient);
        saveData();
        patientCache.addPatient(patient);

        // Add patient to the table
        int row = ui->patientsTableWidget->rowCount();
        ui->patientsTableWidget->insertRow(row);

        ui->patientsTableWidget->setItem(row, 0,
                                         new QTableWidgetItem(QString::number(patientId)));
        ui->patientsTableWidget->setItem(row, 1,
                                         new QTableWidgetItem(name));
        ui->patientsTableWidget->setItem(row, 2,
                                         new QTableWidgetItem(QString::number(age)));
        ui->patientsTableWidget->setItem(row, 3,
                                         new QTableWidgetItem(contact));
        ui->patientsTableWidget->setItem(row, 4,
                                         new QTableWidgetItem(category));

        QString emergencyLevel = "-";
        QString previousVisit = "-";

        if (category.trimmed() == "Emergency Patient") {
            emergencyLevel = ui->emergencyLevelComboBox->currentText();
        }

        if (category == "Follow-Up Patient")
            previousVisit = ui->previousVisitDateEdit->date().toString("yyyy-MM-dd");

        ui->patientsTableWidget->setItem(row, 5,
                                         new QTableWidgetItem(emergencyLevel));
        ui->patientsTableWidget->setItem(row, 6,
                                         new QTableWidgetItem(previousVisit));
        ui->patientsTableWidget->setItem(row, 7,
                                         new QTableWidgetItem(QDate::currentDate().toString("yyyy-MM-dd")));

        // Update dashboard counts
        int general = 0, emergency = 0, followUp = 0;

        for (const auto& p : database.getPatients()) {
            if (dynamic_cast<SCAPMS::GeneralPatient*>(p.get()))
                general++;
            else if (dynamic_cast<SCAPMS::EmergencyPatient*>(p.get()))
                emergency++;
            else if (dynamic_cast<SCAPMS::FollowUpPatient*>(p.get()))
                followUp++;
        }

        ui->label_11->setText("Total Patients: " +
                              QString::number(database.getPatients().size()));
        ui->label_12->setText("General Patients: " + QString::number(general));
        ui->label_13->setText("Emergency Patients: " + QString::number(emergency));
        ui->label_14->setText("Follow-Up Patients: " + QString::number(followUp));

        QMessageBox::information(this, "Success",
                                 "Patient added successfully!");

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

void MainWindow::on_updatePatientButton_2_clicked()
{
    try {
        bool idValid = false;
        int patientId = ui->patientIdLineEdit->text().toInt(&idValid);

        QString name = ui->patientNameLineEdit->text().trimmed();
        int age = ui->patientAgeSpinBox->value();
        QString contact = ui->patientContactLineEdit->text().trimmed();
        QString category = ui->patientCategoryComboBox->currentText();

        if (!idValid || name.isEmpty() || contact.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input",
                                 "Enter the patient's ID, name and contact.");
            return;
        }

        // Check whether the patient exists
        SCAPMS::PatientRecord* existing =
            database.findPatientById(patientId);

        if (existing == nullptr) {
            QMessageBox::warning(this, "Not Found",
                                 "No patient exists with this ID.");
            return;
        }

        // Create the updated patient record
        std::shared_ptr<SCAPMS::PatientRecord> updatedPatient;

        if (category == "General Patient") {
            updatedPatient = std::make_shared<SCAPMS::GeneralPatient>(
                patientId, name.toStdString(), age, contact.toStdString());
        }
        else if (category == "Emergency Patient") {
            int level = ui->emergencyLevelComboBox->currentText()
            .remove("Level ").toInt();

            updatedPatient = std::make_shared<SCAPMS::EmergencyPatient>(
                patientId, name.toStdString(), age,
                contact.toStdString(), level);
        }
        else if (category == "Follow-Up Patient") {
            QString date = ui->previousVisitDateEdit->date()
            .toString("yyyy-MM-dd");

            updatedPatient = std::make_shared<SCAPMS::FollowUpPatient>(
                patientId, name.toStdString(), age,
                contact.toStdString(), date.toStdString());
        }
        else {
            QMessageBox::warning(this, "Category Required",
                                 "Select a patient category.");
            return;
        }

        // Replace the old record
        database.deletePatient(patientId);
        database.addPatient(updatedPatient);
        saveData();
        patientCache.addPatient(updatedPatient);

        // Update the matching table row
        for (int row = 0; row < ui->patientsTableWidget->rowCount(); row++) {
            if (ui->patientsTableWidget->item(row, 0) &&
                ui->patientsTableWidget->item(row, 0)->text() ==
                    QString::number(patientId)) {

                ui->patientsTableWidget->item(row, 1)->setText(name);
                ui->patientsTableWidget->item(row, 2)->setText(QString::number(age));
                ui->patientsTableWidget->item(row, 3)->setText(contact);
                ui->patientsTableWidget->item(row, 4)->setText(category);

                QString level = category == "Emergency Patient"
                                    ? ui->emergencyLevelComboBox->currentText() : "-";

                QString date = category == "Follow-Up Patient"
                                   ? ui->previousVisitDateEdit->date().toString("yyyy-MM-dd") : "-";

                ui->patientsTableWidget->item(row, 5)->setText(level);
                ui->patientsTableWidget->item(row, 6)->setText(date);

                break;
            }
        }

        // Update dashboard totals
        int general = 0, emergency = 0, followUp = 0;

        for (const auto& p : database.getPatients()) {
            if (dynamic_cast<SCAPMS::GeneralPatient*>(p.get()))
                general++;
            else if (dynamic_cast<SCAPMS::EmergencyPatient*>(p.get()))
                emergency++;
            else if (dynamic_cast<SCAPMS::FollowUpPatient*>(p.get()))
                followUp++;
        }

        ui->label_11->setText("Total Patients: " +
                              QString::number(database.getPatients().size()));
        ui->label_12->setText("General Patients: " + QString::number(general));
        ui->label_13->setText("Emergency Patients: " + QString::number(emergency));
        ui->label_14->setText("Follow-Up Patients: " + QString::number(followUp));

        QMessageBox::information(this, "Success",
                                 "Patient updated successfully!");

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Update Error", e.what());
    }
}

void MainWindow::on_deletePatientButton_clicked()
{
    bool validId = false;
    int patientId = ui->patientIdLineEdit->text().toInt(&validId);

    if (!validId) {
        QMessageBox::warning(this, "Invalid Input",
                             "Enter the Patient ID you want to delete.");
        return;
    }

    auto reply = QMessageBox::question(
        this, "Confirm Delete",
        "Are you sure you want to delete this patient?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes)
        return;

    if (!database.deletePatient(patientId)) {
        QMessageBox::warning(this, "Not Found",
                             "Patient with this ID was not found.");
        return;
    }

    saveData();

    // Remove the patient from the table
    for (int row = 0; row < ui->patientsTableWidget->rowCount(); row++) {
        if (ui->patientsTableWidget->item(row, 0) &&
            ui->patientsTableWidget->item(row, 0)->text() ==
                QString::number(patientId)) {
            ui->patientsTableWidget->removeRow(row);
            break;
        }
    }

    // Update patient category totals
    int general = 0, emergency = 0, followUp = 0;

    for (const auto& p : database.getPatients()) {
        if (dynamic_cast<SCAPMS::GeneralPatient*>(p.get()))
            general++;
        else if (dynamic_cast<SCAPMS::EmergencyPatient*>(p.get()))
            emergency++;
        else if (dynamic_cast<SCAPMS::FollowUpPatient*>(p.get()))
            followUp++;
    }

    ui->label_11->setText("Total Patients: " +
                          QString::number(database.getPatients().size()));
    ui->label_12->setText("General Patients: " + QString::number(general));
    ui->label_13->setText("Emergency Patients: " + QString::number(emergency));
    ui->label_14->setText("Follow-Up Patients: " + QString::number(followUp));

    QMessageBox::information(this, "Success",
                             "Patient deleted successfully!");
}

void MainWindow::on_searchPatientButton_clicked()
{
    QString searchValue = ui->searchPatientLineEdit->text().trimmed();

    if (searchValue.isEmpty()) {
        QMessageBox::warning(this, "Search",
                             "Enter a Patient ID or name.");
        return;
    }

    ui->patientsTableWidget->clearSelection();

    bool found = false;

    for (int row = 0; row < ui->patientsTableWidget->rowCount(); row++) {
        QString id = ui->patientsTableWidget->item(row, 0)->text();
        QString name = ui->patientsTableWidget->item(row, 1)->text();

        if (id == searchValue ||
            name.contains(searchValue, Qt::CaseInsensitive)) {

            ui->patientsTableWidget->selectRow(row);
            ui->patientsTableWidget->scrollToItem(
                ui->patientsTableWidget->item(row, 0));

            found = true;
            break;
        }
    }

    if (!found) {
        QMessageBox::information(this, "Search Result",
                                 "No matching patient found.");
    }
}

void MainWindow::on_clearPatientFormButton_clicked()
{
    ui->patientIdLineEdit->clear();
    ui->patientNameLineEdit->clear();
    ui->patientAgeSpinBox->setValue(1);
    ui->patientContactLineEdit->clear();
    ui->patientCategoryComboBox->setCurrentIndex(0);
    ui->emergencyLevelComboBox->setCurrentIndex(0);
    ui->previousVisitDateEdit->setDate(QDate::currentDate());
    ui->searchPatientLineEdit->clear();

    ui->patientsTableWidget->clearSelection();
}

void MainWindow::on_refreshPatientsButton_clicked()
{
    ui->patientsTableWidget->setRowCount(0);

    int general = 0;
    int emergency = 0;
    int followUp = 0;

    for (const auto& patient : database.getPatients()) {

        int row = ui->patientsTableWidget->rowCount();
        ui->patientsTableWidget->insertRow(row);

        ui->patientsTableWidget->setItem(row, 0,
                                         new QTableWidgetItem(
                                             QString::number(patient->getPatientId())));

        ui->patientsTableWidget->setItem(row, 1,
                                         new QTableWidgetItem(
                                             QString::fromStdString(patient->getName())));

        ui->patientsTableWidget->setItem(row, 2,
                                         new QTableWidgetItem(
                                             QString::number(patient->getAge())));

        ui->patientsTableWidget->setItem(row, 3,
                                         new QTableWidgetItem(
                                             QString::fromStdString(patient->getContact())));

        QString category = "Unknown";
        QString level = "-";
        QString previousDate = "-";

        if (dynamic_cast<SCAPMS::GeneralPatient*>(patient.get())) {
            category = "General Patient";
            general++;
        }
        else if (auto* p =
                 dynamic_cast<SCAPMS::EmergencyPatient*>(patient.get())) {
            category = "Emergency Patient";
            level = "Level " + QString::number(p->getEmergencyLevel());
            emergency++;
        }
        else if (auto* followUpPatient =
                 dynamic_cast<SCAPMS::FollowUpPatient*>(patient.get())) {
            category = "Follow-Up Patient";
            previousDate =
                QString::fromStdString(followUpPatient->getPreviousVisitDate());
            followUp++;
        }

        ui->patientsTableWidget->setItem(row, 4,
                                         new QTableWidgetItem(category));

        ui->patientsTableWidget->setItem(row, 5,
                                         new QTableWidgetItem(level));

        ui->patientsTableWidget->setItem(row, 6,
                                         new QTableWidgetItem(previousDate));

        ui->patientsTableWidget->setItem(row, 7,
                                         new QTableWidgetItem("-"));
    }

    ui->label_11->setText("Total Patients: " +
                          QString::number(database.getPatients().size()));

    ui->label_12->setText("General Patients: " +
                          QString::number(general));

    ui->label_13->setText("Emergency Patients: " +
                          QString::number(emergency));

    ui->label_14->setText("Follow-Up Patients: " +
                          QString::number(followUp));

    QMessageBox::information(this, "Refresh",
                             "Patient records refreshed successfully.");
}


// ==========================================
// APPOINTMENT MANAGEMENT
// ==========================================

// Automatically find patient name using Patient ID
void MainWindow::on_appointmentPatientIdLineEdit_textChanged(
    const QString &text)
{
    if (text.trimmed().isEmpty()) {
        ui->appointmentPatientNameLineEdit->clear();
        return;
    }

    bool validId;
    int patientId = text.toInt(&validId);

    if (!validId) {
        ui->appointmentPatientNameLineEdit->clear();
        return;
    }

    auto patient = database.getSharedPatientById(patientId);

    if (patient) {
        ui->appointmentPatientNameLineEdit->setText(
            QString::fromStdString(patient->getName()));
    } else {
        ui->appointmentPatientNameLineEdit->clear();
    }
}


// Schedule Appointment
void MainWindow::on_scheduleAppointmentButton_clicked()
{
    bool validId;

    int appointmentId =
        ui->appointmentIdLineEdit->text().toInt(&validId);

    if (!validId || appointmentId <= 0) {
        QMessageBox::warning(this, "Invalid Appointment ID",
                             "Enter a valid appointment ID.");
        return;
    }

    int patientId =
        ui->appointmentPatientIdLineEdit->text().toInt(&validId);

    if (!validId || patientId <= 0) {
        QMessageBox::warning(this, "Invalid Patient ID",
                             "Enter a valid patient ID.");
        return;
    }

    auto patient = database.getSharedPatientById(patientId);

    if (!patient) {
        QMessageBox::warning(this, "Patient Not Found",
                             "No registered patient has this ID.");
        return;
    }

    QString reason = ui->appointmentReasonTextEdit->toPlainText().trimmed();

    if (reason.isEmpty()) {
        QMessageBox::warning(this, "Missing Information",
                             "Enter the reason for the appointment.");
        return;
    }

    QString date =
        ui->appointmentDateEdit->date().toString("dd/MM/yyyy");

    QString time =
        ui->appointmentTimeEdit->time().toString("hh:mm AP");

    try {
        auto appointment = std::make_shared<SCAPMS::Appointment>(
            appointmentId,
            patient,
            date.toStdString(),
            time.toStdString(),
            reason.toStdString()
            );

        if (!appointment->scheduleAppointment()) {
            QMessageBox::warning(this, "Scheduling Failed",
                                 "The appointment could not be scheduled.");
            return;
        }

        database.addAppointment(appointment);
        saveData();

        on_refreshAppointmentsButton_clicked();

        QMessageBox::information(this, "Success",
                                 "Appointment scheduled successfully.");

        on_clearAppointmentFormButton_clicked();
    }
    catch (const std::exception &e) {
        QMessageBox::warning(this, "Appointment Error", e.what());
    }
}


// Mark Appointment as Completed
void MainWindow::on_completeAppointmentButton_clicked()
{
    int row = ui->appointmentsTableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "No Selection",
                             "Select an appointment from the table.");
        return;
    }

    int appointmentId =
        ui->appointmentsTableWidget->item(row, 0)->text().toInt();

    for (const auto& appointment : database.getAppointments()) {

        if (appointment->getAppointmentId() == appointmentId) {

            if (!appointment->markAsCompleted()) {
                QMessageBox::warning(
                    this, "Cannot Complete",
                    "This appointment is not pending.");
                return;
            }

            saveData();

            on_refreshAppointmentsButton_clicked();

            QMessageBox::information(
                this, "Success",
                "Appointment marked as completed.");

            return;
        }
    }

    QMessageBox::warning(this, "Not Found",
                         "Appointment could not be found.");
}


// Cancel Appointment
void MainWindow::on_cancelAppointmentButton_clicked()
{
    int row = ui->appointmentsTableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(this, "No Selection",
                             "Select an appointment from the table.");
        return;
    }

    int appointmentId =
        ui->appointmentsTableWidget->item(row, 0)->text().toInt();

    auto answer = QMessageBox::question(
        this,
        "Cancel Appointment",
        "Are you sure you want to cancel this appointment?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (answer != QMessageBox::Yes) {
        return;
    }

    for (const auto& appointment : database.getAppointments()) {

        if (appointment->getAppointmentId() == appointmentId) {

            if (!appointment->cancelAppointment()) {
                QMessageBox::warning(
                    this, "Cannot Cancel",
                    "This appointment cannot be cancelled.");
                return;
            }

            saveData();

            on_refreshAppointmentsButton_clicked();

            QMessageBox::information(
                this, "Success",
                "Appointment cancelled successfully.");

            return;
        }
    }

    QMessageBox::warning(this, "Not Found",
                         "Appointment could not be found.");
}


// Clear Appointment Form
void MainWindow::on_clearAppointmentFormButton_clicked()
{
    ui->appointmentIdLineEdit->clear();
    ui->appointmentPatientIdLineEdit->clear();
    ui->appointmentPatientNameLineEdit->clear();
    ui->appointmentReasonTextEdit->clear();

    ui->appointmentDateEdit->setDate(QDate::currentDate());
    ui->appointmentTimeEdit->setTime(QTime::currentTime());

    ui->appointmentsTableWidget->clearSelection();
    ui->searchAppointmentLineEdit->clear();

    ui->appointmentIdLineEdit->setFocus();
}


// Search Appointment
void MainWindow::on_searchAppointmentButton_clicked()
{
    QString searchText =
        ui->searchAppointmentLineEdit->text().trimmed().toLower();

    QString selectedStatus =
        ui->appointmentStatusComboBox->currentText();

    int matches = 0;

    for (int row = 0;
         row < ui->appointmentsTableWidget->rowCount();
         ++row) {

        QString appointmentId =
            ui->appointmentsTableWidget->item(row, 0)->text();

        QString patientName =
            ui->appointmentsTableWidget->item(row, 2)->text();

        QString status =
            ui->appointmentsTableWidget->item(row, 6)->text();

        bool matchesSearch =
            searchText.isEmpty() ||
            appointmentId.toLower().contains(searchText) ||
            patientName.toLower().contains(searchText);

        bool matchesStatus =
            selectedStatus == "All Statuses" ||
            status == selectedStatus;

        bool visible = matchesSearch && matchesStatus;

        ui->appointmentsTableWidget->setRowHidden(row, !visible);

        if (visible) {
            ++matches;
        }
    }

    if (matches == 0) {
        QMessageBox::information(
            this, "Search Results",
            "No matching appointments were found.");
    }
}


// Refresh Appointments Table
void MainWindow::on_refreshAppointmentsButton_clicked()
{
    ui->appointmentsTableWidget->setRowCount(0);

    int totalAppointments = 0;
    int pendingAppointments = 0;
    int completedAppointments = 0;

    for (const auto& appointment : database.getAppointments()) {

        totalAppointments++;

        if (appointment->getStatus() == "Pending") {
            pendingAppointments++;
        }
        else if (appointment->getStatus() == "Completed") {
            completedAppointments++;
        }

        int row = ui->appointmentsTableWidget->rowCount();

        ui->appointmentsTableWidget->insertRow(row);

        auto patient = appointment->getPatient();

        QString patientId = patient
                                ? QString::number(patient->getPatientId())
                                : "-";

        QString patientName = patient
                                  ? QString::fromStdString(patient->getName())
                                  : "Unknown";

        QString status =
            QString::fromStdString(appointment->getStatus());

        ui->appointmentsTableWidget->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(appointment->getAppointmentId())));

        ui->appointmentsTableWidget->setItem(
            row, 1,
            new QTableWidgetItem(patientId));

        ui->appointmentsTableWidget->setItem(
            row, 2,
            new QTableWidgetItem(patientName));

        ui->appointmentsTableWidget->setItem(
            row, 3,
            new QTableWidgetItem(
                QString::fromStdString(
                    appointment->getAppointmentDate())));

        ui->appointmentsTableWidget->setItem(
            row, 4,
            new QTableWidgetItem(
                QString::fromStdString(
                    appointment->getAppointmentTime())));

        ui->appointmentsTableWidget->setItem(
            row, 5,
            new QTableWidgetItem(
                QString::fromStdString(
                    appointment->getReason())));

        ui->appointmentsTableWidget->setItem(
            row, 6,
            new QTableWidgetItem(status));
    }

    // Reset filters after refreshing
    for (int row = 0;
         row < ui->appointmentsTableWidget->rowCount();
         ++row) {
        ui->appointmentsTableWidget->setRowHidden(row, false);
    }

    ui->appointmentStatusComboBox->setCurrentIndex(0);

    ui->totalAppointmentsLabel->setText(
        "Total Appointments: " + QString::number(totalAppointments));

    ui->pendingAppointmentsLabel->setText(
        "Pending Appointments: " + QString::number(pendingAppointments));

    ui->completedAppointmentsLabel->setText(
        "Completed Appointments: " + QString::number(completedAppointments));
}

// Automatically find patient details using Patient ID
void MainWindow::on_queuePatientIdLineEdit_textChanged(
    const QString &text)
{
    ui->queuePatientNameLineEdit->clear();

    if (text.trimmed().isEmpty()) {
        return;
    }

    bool validId;
    int patientId = text.toInt(&validId);

    if (!validId || patientId <= 0) {
        return;
    }

    auto patient = database.getSharedPatientById(patientId);

    if (!patient) {
        return;
    }

    ui->queuePatientNameLineEdit->setText(
        QString::fromStdString(patient->getName())
        );

    if (std::dynamic_pointer_cast<SCAPMS::EmergencyPatient>(patient)) {
        ui->queuePatientCategoryComboBox->setCurrentText("Emergency");
    }
    else if (std::dynamic_pointer_cast<SCAPMS::FollowUpPatient>(patient)) {
        ui->queuePatientCategoryComboBox->setCurrentText("Follow-Up");
    }
    else {
        ui->queuePatientCategoryComboBox->setCurrentText("General");
    }
}


// Add patient to waiting queue
void MainWindow::on_addToQueueButton_clicked()
{
    bool validId;

    int patientId = ui->queuePatientIdLineEdit->text().toInt(&validId);

    if (!validId || patientId <= 0) {
        QMessageBox::warning(
            this,
            "Invalid Patient ID",
            "Please enter a valid Patient ID."
            );
        return;
    }

    auto patient = database.getSharedPatientById(patientId);

    if (!patient) {
        QMessageBox::warning(
            this,
            "Patient Not Found",
            "This patient is not registered in the clinic database."
            );
        return;
    }

    try {
        waitingQueue.addPatient(patient);

        on_refreshQueueButton_clicked();

        ui->queuePatientIdLineEdit->clear();

        QMessageBox::information(
            this,
            "Success",
            "Patient successfully added to the waiting queue."
            );
    }
    catch (const std::exception &e) {
        QMessageBox::warning(
            this,
            "Queue Error",
            e.what()
            );
    }
}


// Serve the next patient
void MainWindow::on_serveNextPatientButton_clicked()
{
    if (waitingQueue.isEmpty()) {
        QMessageBox::information(
            this,
            "Waiting Queue",
            "There are no patients waiting."
            );
        return;
    }

    SCAPMS::PatientRecord* patient = waitingQueue.nextPatient();

    QString patientName = QString::fromStdString(patient->getName());

    auto reply = QMessageBox::question(
        this,
        "Serve Next Patient",
        "Are you sure you want to serve " + patientName + "?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes) {
        return;
    }

    waitingQueue.serveNextPatient();

    on_refreshQueueButton_clicked();

    QMessageBox::information(
        this,
        "Patient Served",
        patientName + " has been removed from the waiting queue."
        );
}


// Remove selected patient from queue
void MainWindow::on_removePatientFromQueueButton_clicked()
{
    int row = ui->waitingQueueTableWidget->currentRow();

    if (row < 0) {
        QMessageBox::warning(
            this,
            "No Selection",
            "Please select a patient from the waiting queue."
            );
        return;
    }

    QTableWidgetItem* item =
        ui->waitingQueueTableWidget->item(row, 0);

    if (!item) {
        return;
    }

    int patientId = item->text().toInt();

    auto reply = QMessageBox::question(
        this,
        "Remove Patient",
        "Are you sure you want to remove this patient from the queue?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes) {
        return;
    }

    if (waitingQueue.removePatient(patientId)) {
        on_refreshQueueButton_clicked();

        QMessageBox::information(
            this,
            "Success",
            "Patient successfully removed from the queue."
            );
    }
}


// Clear the entire waiting queue
void MainWindow::on_clearQueueButton_clicked()
{
    if (waitingQueue.isEmpty()) {
        QMessageBox::information(
            this,
            "Waiting Queue",
            "The waiting queue is already empty."
            );
        return;
    }

    auto reply = QMessageBox::question(
        this,
        "Clear Queue",
        "Are you sure you want to remove all patients from the waiting queue?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply != QMessageBox::Yes) {
        return;
    }

    waitingQueue.clear();

    on_refreshQueueButton_clicked();

    QMessageBox::information(
        this,
        "Success",
        "The waiting queue has been cleared."
        );
}


// Refresh waiting queue table and dashboard
void MainWindow::on_refreshQueueButton_clicked()
{
    ui->waitingQueueTableWidget->setRowCount(0);

    auto queuedPatients = waitingQueue.getPatients();

    int position = 1;

    for (const auto& patient : queuedPatients) {
        int row = ui->waitingQueueTableWidget->rowCount();

        ui->waitingQueueTableWidget->insertRow(row);

        QString category = "General";

        if (std::dynamic_pointer_cast<SCAPMS::EmergencyPatient>(patient)) {
            category = "Emergency";
        }
        else if (std::dynamic_pointer_cast<SCAPMS::FollowUpPatient>(patient)) {
            category = "Follow-Up";
        }

        ui->waitingQueueTableWidget->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(patient->getPatientId())
                )
            );

        ui->waitingQueueTableWidget->setItem(
            row, 1,
            new QTableWidgetItem(
                QString::fromStdString(patient->getName())
                )
            );

        ui->waitingQueueTableWidget->setItem(
            row, 2,
            new QTableWidgetItem(category)
            );

        ui->waitingQueueTableWidget->setItem(
            row, 3,
            new QTableWidgetItem(QString::number(position))
            );

        position++;
    }

    ui->totalWaitingValueLabel->setText(
        QString::number(static_cast<int>(waitingQueue.size()))
        );

    SCAPMS::PatientRecord* next = waitingQueue.nextPatient();

    if (next) {
        ui->nextPatientValueLabel->setText(
            QString::fromStdString(next->getName())
            );
    }
    else {
        ui->nextPatientValueLabel->setText("None");
    }
}

void MainWindow::saveData()
{
    try
    {
        QJsonArray patientsArray;
        QJsonArray appointmentsArray;

        // Save patient records
        for (const auto& patient : database.getPatients())
        {
            QJsonObject patientObject;

            patientObject["id"] = patient->getPatientId();
            patientObject["name"] = QString::fromStdString(patient->getName());
            patientObject["age"] = patient->getAge();
            patientObject["contact"] = QString::fromStdString(patient->getContact());

            if (dynamic_cast<SCAPMS::EmergencyPatient*>(patient.get()))
            {
                auto emergency =
                    dynamic_cast<SCAPMS::EmergencyPatient*>(patient.get());

                patientObject["category"] = "Emergency Patient";
                patientObject["emergencyLevel"] = emergency->getEmergencyLevel();
            }
            else if (dynamic_cast<SCAPMS::FollowUpPatient*>(patient.get()))
            {
                auto followUp =
                    dynamic_cast<SCAPMS::FollowUpPatient*>(patient.get());

                patientObject["category"] = "Follow-Up Patient";
                patientObject["previousVisitDate"] =
                    QString::fromStdString(followUp->getPreviousVisitDate());
            }
            else
            {
                patientObject["category"] = "General Patient";
            }

            patientsArray.append(patientObject);
        }

        // Save appointment records
        for (const auto& appointment : database.getAppointments())
        {
            QJsonObject appointmentObject;

            appointmentObject["id"] = appointment->getAppointmentId();
            appointmentObject["patientId"] =
                appointment->getPatient()->getPatientId();

            appointmentObject["date"] =
                QString::fromStdString(appointment->getAppointmentDate());

            appointmentObject["time"] =
                QString::fromStdString(appointment->getAppointmentTime());

            appointmentObject["reason"] =
                QString::fromStdString(appointment->getReason());

            appointmentObject["status"] =
                QString::fromStdString(appointment->getStatus());

            appointmentsArray.append(appointmentObject);
        }

        QJsonObject rootObject;
        rootObject["patients"] = patientsArray;
        rootObject["appointments"] = appointmentsArray;

        QJsonDocument document(rootObject);

        QString directory =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        QDir().mkpath(directory);

        QString filePath = QDir(directory).filePath("clinic_data.json");

        QSaveFile file(filePath);

        if (!file.open(QIODevice::WriteOnly))
        {
            throw std::runtime_error("Could not open the data file for saving.");
        }

        file.write(document.toJson(QJsonDocument::Indented));

        if (!file.commit())
        {
            throw std::runtime_error("Could not save the clinic data.");
        }
    }
    catch (const std::exception& error)
    {
        QMessageBox::warning(
            this,
            "Save Error",
            QString("Unable to save data: %1").arg(error.what())
            );
    }
}

void MainWindow::loadData()
{
    QString directory =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QString filePath = QDir(directory).filePath("clinic_data.json");

    QFile file(filePath);

    // First launch: no saved file exists yet.
    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(
            this,
            "Load Error",
            "Unable to open the saved clinic data."
            );
        return;
    }

    QJsonParseError parseError;

    QJsonDocument document =
        QJsonDocument::fromJson(file.readAll(), &parseError);

    file.close();

    if (parseError.error != QJsonParseError::NoError ||
        !document.isObject())
    {
        QMessageBox::warning(
            this,
            "Load Error",
            "The saved clinic data is invalid or corrupted."
            );
        return;
    }

    try
    {
        QJsonObject rootObject = document.object();

        // Restore patients first
        QJsonArray patientsArray = rootObject["patients"].toArray();

        for (const QJsonValue& value : patientsArray)
        {
            QJsonObject object = value.toObject();

            int id = object["id"].toInt();
            QString name = object["name"].toString();
            int age = object["age"].toInt();
            QString contact = object["contact"].toString();
            QString category = object["category"].toString();

            std::shared_ptr<SCAPMS::PatientRecord> patient;

            if (category == "Emergency Patient")
            {
                patient = std::make_shared<SCAPMS::EmergencyPatient>(
                    id,
                    name.toStdString(),
                    age,
                    contact.toStdString(),
                    object["emergencyLevel"].toInt()
                    );
            }
            else if (category == "Follow-Up Patient")
            {
                patient = std::make_shared<SCAPMS::FollowUpPatient>(
                    id,
                    name.toStdString(),
                    age,
                    contact.toStdString(),
                    object["previousVisitDate"].toString().toStdString()
                    );
            }
            else
            {
                patient = std::make_shared<SCAPMS::GeneralPatient>(
                    id,
                    name.toStdString(),
                    age,
                    contact.toStdString()
                    );
            }

            database.addPatient(patient);
            patientCache.addPatient(patient);
        }

        // Restore appointments after patients
        QJsonArray appointmentsArray = rootObject["appointments"].toArray();

        for (const QJsonValue& value : appointmentsArray)
        {
            QJsonObject object = value.toObject();

            int appointmentId = object["id"].toInt();
            int patientId = object["patientId"].toInt();

            auto patient = database.getSharedPatientById(patientId);

            // Skip appointments whose patients no longer exist.
            if (!patient)
                continue;

            auto appointment = std::make_shared<SCAPMS::Appointment>(
                appointmentId,
                patient,
                object["date"].toString().toStdString(),
                object["time"].toString().toStdString(),
                object["reason"].toString().toStdString()
                );

            QString status = object["status"].toString();

            if (status != "Not Scheduled")
                appointment->scheduleAppointment();

            if (status == "Completed")
                appointment->markAsCompleted();
            else if (status == "Cancelled")
                appointment->cancelAppointment();

            database.addAppointment(appointment);
        }
    }
    catch (const std::exception& error)
    {
        QMessageBox::warning(
            this,
            "Load Error",
            QString("An error occurred while loading data: %1")
                .arg(error.what())
            );
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}



