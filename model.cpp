/*
 * model.cpp
 *
 *  Created on: Oct 8, 2026
 *      Author: sandro
 */

#include "model.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <math.h>
#include <gio/gio.h>
#include "generated/echo.h"

using namespace std::chrono_literals;

TemperatureModel::TemperatureModel(QObject *parent) :
		QObject(parent),
		m_temperature(0),
		m_timestamp(0),
		m_t0(0),
		m_minTemperature(0),
		m_maxTemperature(0),
		m_serviceAvailable(false) {

//	std::thread([this]() {
//		double t = 0;
//		double step = 0.1;
//
//		std::cout << "\nEntering simulation thread..." << std::endl;
//		do {
//			//std::this_thread::sleep_for(2000ms);
//			std::cout << "\nstep " << t << std::endl;
//			//double m = getTemperature();
//			//setTemperature(m * cos(M_PI * t));
//			t += step;
//		} while (t < 1);
//	});
	m_pTemperatures = new QLineSeries();

};

void TemperatureModel::startSimulation() {
	using namespace std::chrono_literals;
	static double t = 0;
	std::cout << "\nEntering simulation thread..." << std::endl;
	double step = 0.1;
	double m = getTemperature();
	setTemperature(t, (2 * cos(M_PI * t) - 1 ) * m);
	t += step;


}
