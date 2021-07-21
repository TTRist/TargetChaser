// -*- C++ -*-
/*!
 * @file  TargetChaser.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "TargetChaser.h"
#include "confReader.hpp"
#define PI 3.14159265358979
#define RANGE_CHECK(x,min,max) ((x= (x<min  ? min : x<max ? x : max)))

using namespace std;

// Module specification
// <rtc-template block="module_spec">
static const char* targetchaser_spec[] =
{
  "implementation_id", "TargetChaser",
  "type_name",         "TargetChaser",
  "description",       "ModuleDescription",
  "version",           "1.0.0",
  "vendor",            "Kitajima",
  "category",          "Category",
  "activity_type",     "PERIODIC",
  "kind",              "DataFlowComponent",
  "max_instance",      "1",
  "language",          "C++",
  "lang_type",         "compile",
  // Configuration variables
  "conf.default.K_straight", "0.5",
  "conf.default.K_rot", "0.5",
  "conf.default.pix_max", "25000",
  "conf.default.pix_min", "1000",
  "conf.default.center_range", "10",

  // Widget
  "conf.__widget__.K_straight", "text",
  "conf.__widget__.K_rot", "text",
  "conf.__widget__.pix_max", "text",
  "conf.__widget__.pix_min", "text",
  "conf.__widget__.center_range", "text",
  // Constraints
  "conf.__constraints__.center_range", "0<=x<=100",

  "conf.__type__.K_straight", "double",
  "conf.__type__.K_rot", "double",
  "conf.__type__.pix_max", "long",
  "conf.__type__.pix_min", "long",
  "conf.__type__.center_range", "double",

  ""
};
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
TargetChaser::TargetChaser(RTC::Manager* manager)
// <rtc-template block="initializer">
	: RTC::DataFlowComponentBase(manager),
	m_pix_numIn("pix_num", m_pix_num),
	m_center_posIn("center_pos", m_center_pos),
	m_velOut("vel", m_vel)

	// </rtc-template>
{
}

/*!
 * @brief destructor
 */
TargetChaser::~TargetChaser()
{
}



RTC::ReturnCode_t TargetChaser::onInitialize()
{
	printf("onInitialize [TargetChaser]\n");
	// Registration: InPort/OutPort/Service
	// <rtc-template block="registration">
	// Set InPort buffers
	addInPort("pix_num", m_pix_numIn);
	addInPort("center_pos", m_center_posIn);

	// Set OutPort buffer
	addOutPort("vel", m_velOut);

	// Set service provider to Ports

	// Set service consumers to Ports

	// Set CORBA Service Ports

	// </rtc-template>

	// <rtc-template block="bind_config">
	// Bind variables and configuration variable
	bindParameter("K_straight", m_K_straight, "0.5");
	bindParameter("K_rot", m_K_rot, "0.5");
	bindParameter("pix_max", m_pix_max, "25000");
	bindParameter("pix_min", m_pix_min, "1000");
	bindParameter("center_range", m_center_range, "10"); 
	bindParameter("straight_max", m_straight_max, "1");
	bindParameter("rot_max", m_rot_max, "1");
	// </rtc-template>

	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TargetChaser::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaser::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaser::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

double width;
RTC::ReturnCode_t TargetChaser::onActivated(RTC::UniqueId ec_id)
{
	ifstream infile("./../../../MakedFile/param.ini");
	if (infile) conf::setMap(conf::config_map, infile, R"(=|\s)");
	else printf("パスが違う\n");
	infile.close();

	if (conf::config_map["eula"] == "true") {
		m_K_straight	= conf::readMap("K_straight");
		m_K_rot			= conf::readMap("K_rot");
		m_pix_max		= conf::readMap("pix_max");
		m_pix_min		= conf::readMap("pix_min");
		m_center_range	= conf::readMap("center_range"); 
		m_straight_max  = conf::readMap("straight_max");
		m_rot_max		= conf::readMap("rot_max");
		width			= conf::readMap("width");
	}
	else printf("Not Using Map\n");

	return RTC::RTC_OK;
}


RTC::ReturnCode_t TargetChaser::onDeactivated(RTC::UniqueId ec_id)
{
	m_vel.data.vx = 0;
	m_vel.data.va = 0;
	m_velOut.write();
	return RTC::RTC_OK;
}


RTC::ReturnCode_t TargetChaser::onExecute(RTC::UniqueId ec_id)
{
	if (m_pix_numIn.isNew() && m_center_posIn.isNew()) {
		while (!m_pix_numIn.isEmpty())		m_pix_numIn.read();
		while (!m_center_posIn.isEmpty())	m_center_posIn.read();

		// 大きさに対して移動
		double size_rate,tgt_spd, tgt_rot;
		size_rate = 1.0 * m_pix_num.data / m_pix_max;
		if ((m_pix_min / m_pix_max < size_rate) && (size_rate < 1.0f)) {
			tgt_spd = m_straight_max * (1.0 - size_rate);
		}
		else 
			tgt_spd = 0;
		m_vel.data.vx = tgt_spd;
		

		m_vel.data.va = 1.0 * m_K_rot * m_center_pos.data.x / (width / 2);
		m_vel.data.va = m_vel.data.va * PI / 180;
		RANGE_CHECK(m_vel.data.va, -m_rot_max, m_rot_max);

		m_velOut.write();

		printf("Vx:%.3f   Va:%.3f\n", m_vel.data.vx, m_vel.data.va);

	}
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TargetChaser::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaser::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaser::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaser::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaser::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

	void TargetChaserInit(RTC::Manager* manager)
	{
		coil::Properties profile(targetchaser_spec);
		manager->registerFactory(profile,
			RTC::Create<TargetChaser>,
			RTC::Delete<TargetChaser>);
	}

};


