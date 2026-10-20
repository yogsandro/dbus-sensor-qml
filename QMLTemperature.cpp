#include <iostream>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <thread>
#include <chrono>
#include <math.h>

#include "config.h"
#include "model.h"

#include <gio/gio.h>
#include "generated/echo.h"
#include "c_api.h"


using namespace std::chrono_literals;

static GMainLoop *loop;

static void set_temperature(void *context, double temperature, unsigned long timestamp)
{
    auto *object = static_cast<TemperatureModel *>(context);
    object->setTemperature(temperature, timestamp);
}

static void set_service_available(void *context, int b)
{
    auto *object = static_cast<TemperatureModel *>(context);
    object->setServiceAvailable(b ? true : false);
}

void initialize_c(TemperatureModel& model)
{
    static TemperatureModel *object = &model;

	std::cout << "in initialize_c " << object << std::endl;
    Callbacks callbacks = {
        .do_work = set_temperature,
		.set_service_available = set_service_available,
        .object = object
    };

    c_register_callbacks(&callbacks);
}

static void
on_owner_changed (ExampleComExampleEcho    *object,
                  GParamSpec *pspec,
                  gpointer    user_data)
{
    GDBusProxy *proxy = G_DBUS_PROXY (object);
    const gchar *owner = g_dbus_proxy_get_name_owner (proxy);

    if (owner != NULL) {
        g_print ("Service is available\n");
        c_service_available(1);
    } else {
        g_print ("Service is unavailable\n");
        c_service_available(0);
    }
}

static void
properties_changed(ExampleComExampleEcho  *proxy,
                   GVariant    *changed_properties,
                   GStrv        invalidated_properties,
                   gpointer      user_data)
{
	g_print("properties_changed\n");




//    GVariant *value;
//    gdouble temperature;
//
//    value = g_variant_lookup_value(
//        changed_properties,
//        "Temperature",
//        G_VARIANT_TYPE_DOUBLE
//    );
//
//    if (value != NULL) {
//        temperature = g_variant_get_double(value);
//        c_run(temperature);
//
//        g_print("Temperature changed: %f\n", temperature);
//
//        g_variant_unref(value);
//    }


    GVariant *v_temperature_value;
    GVariant *v_timestamp_value;
    gdouble temperature;
    guint64 timestamp;
    gboolean available = example_com_example_echo_get_reading_available(proxy);

    on_owner_changed(proxy, NULL, NULL);

   	if (available) {
		v_temperature_value = g_variant_lookup_value(
			changed_properties,
			"Temperature",
			G_VARIANT_TYPE_DOUBLE
		);

		v_timestamp_value = g_variant_lookup_value(
			changed_properties,
			"Timestamp",
			G_VARIANT_TYPE_UINT64
		);

		if (v_temperature_value != NULL && v_timestamp_value != NULL) {
			temperature = g_variant_get_double(v_temperature_value);
			timestamp = g_variant_get_uint64(v_timestamp_value);

			c_run(temperature, timestamp);

			g_print("Temperature changed: %f (%ld)\n", temperature, timestamp);

			g_variant_unref(v_temperature_value);
			g_variant_unref(v_timestamp_value);
		}
   	}


    for (guint i = 0;
         invalidated_properties != NULL &&
         invalidated_properties[i] != NULL;
         i++) {
        g_print("Property invalidated: %s\n",
                invalidated_properties[i]);
    }
}

int
start(void) {
	ExampleComExampleEcho *proxy;
	gchar *reply;
	gint32 add_reply;
	GError *error = NULL;

	loop = g_main_loop_new(NULL, FALSE);

	proxy = example_com_example_echo_proxy_new_for_bus_sync(
		G_BUS_TYPE_SESSION,
		G_DBUS_PROXY_FLAGS_NONE,
		"com.example.Echo",
		"/com/example/Echo",
		NULL,
		&error
	);

	if (proxy == NULL) {
		g_printerr("Could not create proxy: %s\n",
				   error->message);
		g_error_free(error);
		return 1;
	}

	g_signal_connect(
		proxy,
		"g-properties-changed",
		G_CALLBACK(properties_changed),
		NULL
	);

	/* After creating the proxy: */
	g_signal_connect (
		proxy,
		"notify::g-name-owner",
	    G_CALLBACK (on_owner_changed),
		NULL
	);

	//gdouble temperature = example_com_example_echo_get_temperature(proxy);
	//guint64 timestamp = example_com_example_echo_get_timestamp(proxy);

	//g_print("Server replied: T=%f (%ld)\n", temperature, timestamp);
	//c_run(temperature, timestamp);

	g_free(reply);

	/*
	 * This blocks while processing D-Bus events.
	 */
	g_main_loop_run(loop);

	g_print("Client stopping...\n");

	g_object_unref(proxy);
	g_main_loop_unref(loop);

	return 0;
}




int main(int argc, char **argv) {
	QApplication app(argc, argv);
	QQmlApplicationEngine engine;
	TemperatureModel model;
	engine.rootContext()->setContextProperty("model", &model);

	engine.load(QUrl(QStringLiteral("qrc:/QmlTemperatureApp/main.qml")));
	if (engine.rootObjects().isEmpty()) return -1;

	qInfo() << qVersion();

//	std::thread t([&model]() {
//		double t = 0;
//		double step = 0.01;
//		double m = model.getTemperature();
//
//		std::cout << "\nEntering simulation thread..." << std::endl;
//		do {
//			std::this_thread::sleep_for(1000ms);
//			std::cout << "\nstep " << t << std::endl;
//			model.setTemperature(m * cos(M_PI * t));
//			t += step;
//		} while (1);
//	});

	initialize_c(model);

	std::thread tg ([]() {
		start();
	});


	return app.exec();
}
