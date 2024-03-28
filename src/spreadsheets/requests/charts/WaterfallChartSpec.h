#ifndef WATERFALL_CHART_SPEC_H
#define WATERFALL_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"

/**
 * WATERFALL CHART CLASS DEPENDENCIES
 * 
 * WaterfallChartSpec       WaterfallChartDomain                    ChartData*
 * 
 *                          WaterfallChartSeries                    ChartData*
 *                                                                  WaterfallChartColumnStyle           ColorStyle*
 *                                                                  WaterfallChartCustomSubtotal
 *                                                                  DataLabel*
 * 
 *                          (enum) WaterfallChartStackedType
 *                          LineStyle
 *                          DataLabel*            
 *                          
 * 
 * See ChartData.h, Theme.h
*/

namespace GSHEET
{
}

#endif