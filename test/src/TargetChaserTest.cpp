// -*- C++ -*-
/*!
 * @file  TargetChaserTest.cpp
 * @brief ModuleDescription
 * @date $Date$
 *
 * $Id$
 */

#include "TargetChaserTest.h"

// Module specification
// <rtc-template block="module_spec">
static const char* targetchaser_spec[] =
  {
    "implementation_id", "TargetChaserTest",
    "type_name",         "TargetChaserTest",
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
TargetChaserTest::TargetChaserTest(RTC::Manager* manager)
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
TargetChaserTest::~TargetChaserTest()
{
}



RTC::ReturnCode_t TargetChaserTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("vel", m_velIn);

  // Set OutPort buffer
  addOutPort("pix_num", m_pix_numOut);
  addOutPort("center_pos", m_center_posOut);

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
  // </rtc-template>

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TargetChaserTest::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaserTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaserTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t TargetChaserTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TargetChaserTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t TargetChaserTest::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TargetChaserTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaserTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaserTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaserTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TargetChaserTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{

  void TargetChaserTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(targetchaser_spec);
    manager->registerFactory(profile,
                             RTC::Create<TargetChaserTest>,
                             RTC::Delete<TargetChaserTest>);
  }

};


