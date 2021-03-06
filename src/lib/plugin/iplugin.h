#pragma once

#include "sims/isimulation.h"

/**
 * A return value of != 0 say that everything is okey, nullptr, a failure happened
 */
#define PS_INITPLUGIN extern "C" Q_DECL_EXPORT ISimulation* ps_initPlugin()
#define PS_INITPLUGIN_NAME "ps_initPlugin"
typedef ISimulation* (*PS_INITPLUGIN_FUNCTION)();