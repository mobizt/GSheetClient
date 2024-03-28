#ifndef CHART_DATA_SPEC_H
#define CHART_DATA_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/GridRange.h"

/**
 * CHART DATA CLASS DEPENDENCIES
 * 
 * ChartData        ChartGroupRule                  ChartDateTimeRule       (enum) ChartDateTimeRuleType
 *                                                  ChartHistogramRule
 * 
 *                  (enum) ChartAggregateType
 *                  ChartSourceRange                GridRange
 *                  DataSourceColumnReference
 * 
 * See Theme.h
*/

namespace GSHEET
{
}

#endif