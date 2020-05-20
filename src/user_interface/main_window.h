#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QShortcut>
#include <algorithm>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <regex>
#include <sstream>
#include <thread>

#include "utility.h"
#include "camera.h"
#include "config.h"
#include "file_storage.h"
#include "lane_detector.h"
#include "object_detector.h"
#include "car_gps_reader.h"
#include "input_source.h"
#include "simulation.h"
#include "car_status.h"
#include "timer.h"
#include "traffic_sign_monitor.h"
#include "speed_limit.h"
#include "traffic_sign_images.h"
#include "camera_model.h"
#include "collision_warning.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startVideoGrabber();
    void refreshCams();
    void playAudio(std::string audio_file);
    void alert(std::string audio_file);

   protected:
    void closeEvent(QCloseEvent *event);
    void loadEffects();

   private slots:
    void openSimulationSelector();
    void toggleMute();
    void toggleAlert();
    void showCameraWizard();

   private:
    Ui::MainWindow *ui;
    std::shared_ptr<CameraModel> camera_model;

    // Input source
    InputSource input_source;
    Simulation *simulation;

    QGraphicsPixmapItem pixmap;

    // Processors
    std::shared_ptr<ObjectDetector> object_detector;
    std::shared_ptr<LaneDetector> lane_detector;
    std::shared_ptr<CarGPSReader> car_gps_reader;
    std::shared_ptr<CollisionWarning> collision_warning;

    MaxSpeedLimit speed_limit;
    std::mutex speed_limit_mutex;

    // Images
    TrafficSignImages traffic_sign_images;

    // Audio
    std::atomic<bool> is_mute = false;
    std::atomic<bool> is_alert = true;

   public:
    CarStatus car_status;

   private:

    static void objectDetectionThread(
        std::shared_ptr<ObjectDetector> object_detector, CarStatus *,
        CollisionWarning *collision_warning);
    static void laneDetectionThread(
        std::shared_ptr<LaneDetector> lane_detector, CarStatus *);
    static void carPropReaderThread(
        std::shared_ptr<CarGPSReader> car_gps_reader);
    static void speedWarningThread(CarStatus *car_status, MainWindow *main_window);

   public:
    void setInputSource(InputSource input_source);
    void setSimulation(Simulation *simulation);
    void setSpeedLimit(MaxSpeedLimit speed_limit);
    MaxSpeedLimit getSpeedLimit();

};

#endif  // MAINWINDOW_H
