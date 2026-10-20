/*
 * model.h
 *
 *  Created on: Oct 8, 2026
 *      Author: sandro
 */

#pragma once

#include <iostream>

#include <QVariant>
#include <QLineSeries>

class TemperatureModel : public QObject {
	Q_OBJECT
	Q_PROPERTY(double temperature READ getTemperature NOTIFY temperatureChanged)
	Q_PROPERTY(double minTemperature READ getMinTemperature NOTIFY temperatureChanged)
	Q_PROPERTY(double maxTemperature READ getMaxTemperature NOTIFY temperatureChanged)
	Q_PROPERTY(double timestamp READ getTimestamp NOTIFY temperatureChanged)
	Q_PROPERTY(double t0Timestamp READ getT0Timestamp NOTIFY temperatureChanged)
	Q_PROPERTY(bool serviceAvailable READ isServiceAvailable NOTIFY serviceAvailabilityChanged)
	Q_PROPERTY(QLineSeries* temperatures READ getTemperatures NOTIFY temperatureChanged)

public:
	TemperatureModel(QObject *parent = nullptr);
	~TemperatureModel() = default;

	double getTemperature() const {
		return m_temperature;
	}

	double getMinTemperature() const {
		return m_minTemperature;
	}

	double getMaxTemperature() const {
		return m_maxTemperature;
	}

	unsigned long getTimestamp() const {
		return m_timestamp;
	}

	unsigned long getT0Timestamp() const {
		return m_t0;
	}

	bool isServiceAvailable() const {
		return m_serviceAvailable;
	}

	void setServiceAvailable(bool serviceAvailable) {
		m_serviceAvailable = serviceAvailable;
		Q_EMIT serviceAvailabilityChanged();
	}

	QLineSeries* getTemperatures() const {
		return m_pTemperatures;
	}

	void setTemperature(double temperature, unsigned long timestamp) {
		m_temperature = temperature;
		m_timestamp = timestamp;

		if (m_temperature < m_minTemperature) {
			m_minTemperature = m_temperature;
		} else if (m_temperature > m_maxTemperature) {
			m_maxTemperature = m_temperature;
		}

		if (m_pTemperatures->points().length() == 0) {
			m_t0 = timestamp;
			m_minTemperature = m_maxTemperature = m_temperature;
			std::cout << "T0=" << m_t0 << std::endl;
		}

		std::cout << "(" << m_timestamp - m_t0 << "," << m_temperature << ")" << std::endl;
		//double t0 = m_pTemperatures->points().length() == 0 ? timestamp : m_pTemperatures->points().value(0).xp;

//		std::cout << m_pTemperatures->points().length() << std::endl;

		m_pTemperatures->append((m_timestamp - m_t0) / 1000, m_temperature);

//		std::cout << "D=" << (m_timestamp - m_t0) / 1000 - m_pTemperatures->points().at(0).x() << std::endl;

		while ((m_timestamp - m_t0) / 1000 - m_pTemperatures->points().at(0).x() > 60 * 60 * 5) {
			m_pTemperatures->remove(0);
		}

		Q_EMIT temperatureChanged();
	}

	Q_INVOKABLE void startSimulation();

Q_SIGNALS:	//we use Q_SIGNALS instead of signals because glib also uses the signals keyword
	void temperatureChanged();
	void serviceAvailabilityChanged();


	//void requestPaint();

private:
	double m_temperature;
	double m_timestamp;
	QLineSeries *m_pTemperatures;
	double m_t0;
	double m_minTemperature;
	double m_maxTemperature;
	bool m_serviceAvailable;

};

