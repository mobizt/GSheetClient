#ifndef SCORECARD_CHART_SPEC_H
#define SCORECARD_CHART_SPEC_H

#include <Arduino.h>
#include "./Config.h"
#include "./core/JSON.h"
#include "./core/ObjectWriter.h"
#include "./spreadsheets/requests/charts/ChartData.h"
#include "./spreadsheets/requests/Theme.h"
#include "./spreadsheets/requests/CellFormat.h"

/**
 * ScorecardChartSpec       ChartData*
 *                          (enum) ChartAggregateType
 *                          KeyValueFormat                      TextFormat
 *                                                              TextPosition
 * 
 *                          BaselineValueFormat                 (enum) ComparisonType
 *                                                              TextFormat
 *                                                              TextPosition
 *                                                              ColorStyle*
 *                          (enum) ChartNumberFormatSource
 *                          ChartCustomNumberFormatOptions
 *                      
 * 
 * See ChartData.h, Theme.h
*/
namespace GSHEET
{
}

#endif