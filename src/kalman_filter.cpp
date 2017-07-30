#include "kalman_filter.h"

#include <iostream>
using namespace std; // REMOVEME and the cout's that need this

using Eigen::MatrixXd;
using Eigen::VectorXd;

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Predict() {
    /**
    TODO:
      * predict the state
    */
    x_ = F_ * x_;
    MatrixXd Ft = F_.transpose();
    P_ = F_ * P_ * Ft + Q_;

    if (tools_.debug) {
        // print the output
        cout << "\nPredict:" << endl;
        cout << "x_ = " << x_ << endl;
        cout << "P_ = " << P_ << endl;
    }
}

void KalmanFilter::Update(const VectorXd &z) {
    /**
    TODO:
      * update the state by using Kalman Filter equations
    */
    VectorXd z_pred = H_ * x_;
    VectorXd y = z - z_pred;
    MatrixXd Ht = H_.transpose();
    MatrixXd S = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd PHt = P_ * Ht;
    MatrixXd K = PHt * Si;

    //new estimate
    x_ = x_ + (K * y);
    long x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;

    if (tools_.debug) {
        cout << "\nUpdate:" << endl;
        cout << "x_ = " << x_ << endl;
        cout << "P_ = " << P_ << endl;
    }
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
    /**
    TODO:
      * update the state by using Extended Kalman Filter equations
    */

    float px = x_(0);
    float py = x_(1);
    if ((px*px + py*py) < 0.0001) {
        return;
    }

    VectorXd z_pred = H_ * x_;
    VectorXd y = z - tools_.h(x_);

    float phi = y(1);
    if (phi > M_PI) {
        y(1) = 2*M_PI - phi;
    } else if (phi < -M_PI ) {
        y(1) = 2*M_PI + phi;
    }

    MatrixXd Ht = H_.transpose();
    MatrixXd S = H_ * P_ * Ht + R_;
    MatrixXd Si = S.inverse();
    MatrixXd PHt = P_ * Ht;
    MatrixXd K = PHt * Si;

    //new estimate
    x_ = x_ + (K * y);
    long x_size = x_.size();
    MatrixXd I = MatrixXd::Identity(x_size, x_size);
    P_ = (I - K * H_) * P_;

    if (tools_.debug) {
        cout << "\nUpdateEKF:" << endl;
        cout << "x_ = " << x_ << endl;
        cout << "P_ = " << P_ << endl;
    }
}
